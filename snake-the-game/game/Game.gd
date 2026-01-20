extends Node2D
class_name Game
#Zmienna typu Timer odwołującą się do node TurnTimer
@onready var Turn_Timer: Timer = $TurnTimer

@export var board_width: int = 16
@export var board_height: int = 16
@onready var tile_map: TileMap = $TileMap
@onready var lose := $LoseDialog
@onready var winner := $WinDialog


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
	
	for y in range(board_height):
		for x in range(board_width):
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
		fruit.spawn(Vector2i(board_width, board_height), snake.get_body(), rng)

func win():
	var board_set: Dictionary = {}
	for x in range(board_width):
		for y in range(board_height):
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
	rng.randomize()	
	snake = Snake.new()
	fruit = Fruit.new()
	board = Board.new()
	rules = Rules_Normal.new()
	wall = Wall.new()
	
	snake.init_snake(Vector2i(board_width, board_height))
	fruit.spawn(Vector2i(board_width, board_height), snake.get_body(), rng)
	wall.init_wall(board_width, board_height)
	draw_board()
	draw_wall()
	draw_snake()
	draw_fruit()
	Turn_Timer.wait_time = 0.5   # 500 ms
	Turn_Timer.one_shot = false  # Timer powtarzalny - Gdyby True to wywołał by się tylko raz
	Turn_Timer.start()           #Start odliczania czasu

func _next_turn() -> void:
	print("Next turn")	
	snake.move_snake()
	collision()
	draw_board()
	draw_wall()
	draw_snake()
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
	snake.reset(board_width / 2, board_height / 2)
	fruit.set_exist(true)
	Turn_Timer.wait_time = 0.1   # 1000 ms
	Turn_Timer.start()
	Turn_Timer.wait_time = 0.5   # 500 ms
	$AudioStreamPlayer.play()

func _on_lose_dialog_canceled() -> void:
	get_tree().quit()

func _on_win_dialog_confirmed() -> void:
	snake.reset(board_width / 2, board_height / 2)
	fruit.set_exist(true)
	fruit.spawn(Vector2i(board_width, board_height), snake.get_body(), rng)
	Turn_Timer.wait_time = 0.1   # 1000 ms
	Turn_Timer.start()
	Turn_Timer.wait_time = 0.5   # 500 ms
	$AudioStreamPlayer.play()
	
func _on_win_dialog_canceled() -> void:
	get_tree().quit()
	
func win_popup():
	$AudioStreamPlayer.stop()
	winner.dialog_text = "WYGRAŁES - Chcesz zagrać jeszcze raz?"
	winner.popup_centered()
