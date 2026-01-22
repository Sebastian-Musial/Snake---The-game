extends Node2D
class_name Game
#Zmienna typu Timer odwołującą się do node TurnTimer
@onready var Turn_Timer: Timer = $TurnTimer
@onready var tile_map: TileMap = $TileMap
@onready var lose := $LoseDialog
@onready var winner := $WinDialog

@onready var music_N: AudioStreamPlayer = $Normal
@onready var music_H: AudioStreamPlayer = $Hard

#Zmienne do odbioru pixeli z pliku graficznego przez TileMap
const SOURCE_ID := 1
const ATLAS_BG := Vector2i(1, 1)
const ATLAS_SNAKE := Vector2i(0, 0)
const ATLAS_FRUIT := Vector2i(1, 0)
const ATLAS_WALL := Vector2i(2, 0)

#Tworzenie obiektów z moich klas
var snake: Snake
var fruit: Fruit
var board: Board
var rules: Game_Rules
var wall: Wall

var rng := RandomNumberGenerator.new()

#Metody
func draw_board() -> void:
	tile_map.clear()
	
	for y in range(board.get_height()):
		for x in range(board.get_width()):
			tile_map.set_cell(0, Vector2i(x, y), SOURCE_ID, ATLAS_BG)
	
func draw_wall() -> void:	#ściany są rysowane poza planszą - w grze nie widać ale matematyka się nie zgadza
	for segment in wall.get_body():
		tile_map.set_cell(0, segment, SOURCE_ID, ATLAS_WALL)

func draw_snake() -> void:
	var body := snake.get_body()
	for segment in body:
		tile_map.set_cell(0, segment, SOURCE_ID, ATLAS_SNAKE)
	
func draw_fruit() -> void:
	tile_map.set_cell(0, fruit.get_body(), SOURCE_ID, ATLAS_FRUIT)	
	
#Kolizje - Mechanika w kodzie pomimo że godot udostępnia swoje node do kolicji postaci 2D 
#Metoda ze szkicu C++ - 3 w 1
func collision() -> void: 
	var head_snake_pos: Vector2i = snake.get_body()[0] 
#Snake = Wall 
	if head_snake_pos in wall.get_body(): 
		Turn_Timer.stop()
		ask_restart()
		#Turn_Timer.one_shot = true  # Timer powtarzalny - Gdyby True to wywołał by się tylko raz 
#Snake = Snake [Brak wyjątku gdzie głowa to tez ciało]
	if head_snake_pos in snake.get_body().slice(1): 
		Turn_Timer.stop()
		ask_restart()
		#Turn_Timer.one_shot = true  # Timer powtarzalny - Gdyby True to wywołał by się tylko raz 
#Snake = Fruit 
	if head_snake_pos == fruit.get_body(): 
		snake.eat()
		fruit.spawn(Vector2i(board.get_width(), board.get_height()), snake.get_body(), rng)
		snake.add_one_point()

func win():
	var board_set: Dictionary = {}
	for x in range(board.get_width()):
		for y in range(board.get_height()):
			board_set[Vector2i(x, y)] = true
			
	var occupied_set: Dictionary = {}
	for i in snake.get_body():
		occupied_set[i] = true
	for i in wall.get_body():
		occupied_set[i] = true

	for cell in board_set.keys():	#Sprawdza caly slownik po jego elementach
		if not occupied_set.has(cell):	#Test czy pole jest zajete 
			return
	Turn_Timer.stop()
	win_popup()

#Sterowanie - TEST DZIALA
func _unhandled_input(event):
	if event is InputEventKey and event.pressed:
		match event.keycode:
			Key.KEY_UP, Key.KEY_W:
				snake.request_dir(Types.directions.UP)
			Key.KEY_DOWN, Key.KEY_S: 
				snake.request_dir(Types.directions.DOWN)
			Key.KEY_LEFT, Key.KEY_A: 
				snake.request_dir(Types.directions.LEFT)
			Key.KEY_RIGHT, Key.KEY_D: 
				snake.request_dir(Types.directions.RIGHT)
	
#!!!!TUTAJ JUZ MECHANIKA JAK WSZYSTKO PRZEPLYWA PRZEZ GRE!!!#
func _ready():
	AudioMenuManager.stop_music()
	rng.randomize()	
	rules = GState.rules
	if GState.rules.get_turn_time() == 0.5:
		music_H.play()
	if GState.rules.get_turn_time() == 1.0:
		music_N.play()
	board = Board.new()
	board.set_size(rules.get_board_size())
	snake = Snake.new()
	fruit = Fruit.new()
	wall = Wall.new()
	
	
	snake.init_snake(Vector2i(board.get_width(), board.get_height()))
	fruit.spawn(Vector2i(board.get_width(), board.get_height()), snake.get_body(), rng)
	wall.init_wall(board.get_width(), board.get_height())
	draw_board()
	draw_wall()
	draw_snake()
	draw_fruit()
	snake.reset_score()
	$Score.text = "Score: %d" % snake.get_score()
	Turn_Timer.wait_time = rules.get_turn_time()   # 500 ms
	Turn_Timer.one_shot = false  # Timer powtarzalny - Gdyby True to wywołał by się tylko raz
	Turn_Timer.start()           #Start odliczania czasu

func _next_turn() -> void:
	print("Next turn")	
	snake.move_snake()
	collision()
	draw_board()
	draw_wall()
	draw_snake()
	$Score.text = "Score: %d" % snake.get_score()
	if(fruit.get_exist()): 
		draw_fruit()
	win()

#Sygnał - metoda łącząca Game z timer
func _on_Turn_Timer_timeout() -> void:
	_next_turn()

func ask_restart():
	$AudioStreamPlayer.stop()
	lose.dialog_text = "Restartować grę?"
	lose.popup_centered()

#Sygnał - metoda łącząca Game z ConfirmationDialog
func _on_lose_dialog_confirmed() -> void:
	snake.reset(board.get_width() / 2, board.get_height() / 2)
	fruit.set_exist(true)
	Turn_Timer.wait_time = 0.1   # 1000 ms
	Turn_Timer.start()
	Turn_Timer.wait_time = rules.get_turn_time()  
	$AudioStreamPlayer.play()
	snake.reset_score()

func _on_lose_dialog_canceled() -> void:
	get_tree().quit()

func _on_win_dialog_confirmed() -> void:
	snake.reset(board.get_width() / 2, board.get_height() / 2)
	fruit.set_exist(true)
	fruit.spawn(Vector2i(board.get_width(), board.get_height()), snake.get_body(), rng)
	Turn_Timer.wait_time = 0.1   # 1000 ms
	Turn_Timer.start()
	Turn_Timer.wait_time = rules.get_turn_time()   # 500 ms
	$AudioStreamPlayer.play()
	snake.reset_score()
	
func _on_win_dialog_canceled() -> void:
	get_tree().quit()
	
func win_popup():
	$AudioStreamPlayer.stop()
	winner.dialog_text = "WYGRAŁES - Chcesz zagrać jeszcze raz?"
	winner.popup_centered()
