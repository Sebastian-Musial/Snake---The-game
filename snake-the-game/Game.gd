extends Node2D
class_name Game
#Zmienna typu Timer odwołującą się do node TurnTimer
@onready var Turn_Timer: Timer = $TurnTimer

@export var board_width: int = 20
@export var board_height: int = 20
@onready var tile_map: TileMap = $TileMap

#Zmienne do odbioru pixeli z pliku graficznego przez TileMap
const SOURCE_ID := 0
const ATLAS_BG := Vector2i(3, 3)
const ATLAS_SNAKE := Vector2i(0, 0)
const ATLAS_FRUIT := Vector2i(0, 3)
const ATLAS_WALL := Vector2i(3, 0)

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
	if head_snake_pos not in wall.get_body(): print("Brak kolizji_W") 
#Snake = Snake [Brak wyjątku gdzie głowa to tez ciało]
	if head_snake_pos in snake.get_body().slice(1): print("Kolizja_S") 
#Snake = Fruit 
	if head_snake_pos == fruit.get_body(): print("Am...Am...")
	
#Sterowanie - TEST DZIALA
func _unhandled_input(event):
	if event is InputEventKey and event.pressed:
		match event.keycode:
			Key.KEY_UP: print("Gora")
			Key.KEY_W: print("Gora")
			Key.KEY_DOWN: print("Dol")
			Key.KEY_S: print("Dol")
			Key.KEY_LEFT: print("Lewo")
			Key.KEY_A: print("Lewo")
			Key.KEY_RIGHT: print("Prawo")
			Key.KEY_D: print("Prawo")

	
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
	Turn_Timer.wait_time = 1.0   # 1000 ms
	Turn_Timer.one_shot = false  # Timer powtarzalny - Gdyby True to wywołał by się tylko raz
	Turn_Timer.start()           #Start odliczania czasu

func _next_turn() -> void:
	snake.move_snake()
	draw_board()
	draw_wall()
	draw_snake()
	draw_fruit()
	print("Next turn")
	

#Sygnał - metoda łącząca Game z timer
func _on_Turn_Timer_timeout() -> void:
	_next_turn()
