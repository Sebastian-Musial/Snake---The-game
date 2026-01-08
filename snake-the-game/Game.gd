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

#Aktualnie rysuje się tylko pierwszy segment węża
func init_snake() -> void:
	snake.get_body().clear()
	snake.get_body().append(Vector2i(int(board_width / 2), int(board_height / 2)))

func draw_snake() -> void:
	var body := snake.get_body()
	for segment in body:
		tile_map.set_cell(0, segment, SOURCE_ID, ATLAS_SNAKE)


#Narazie testowo głowa się przesuwa w prawo 
func move_snake() -> void:
	var head := snake.get_body()[0]
	var new_head := head + Vector2i(1, 0) # w prawo
	
	snake.get_body().insert(0, new_head)	#Nowa głowa
	snake.get_body().remove_at(snake.get_body().size() - 1)			#Kasowanie ogona - ostatniego elementu


#Owoc ma już swoj spawn z RNG dla pozycji [bug:owov moze sie generowac w scianie]
func init_fruit() -> void:
	while true:
		var pos := Vector2i(rng.randi_range(0, board_width - 1), rng.randi_range(0, board_height - 1))
		if pos not in snake.get_body():
			fruit.set_body(pos)
			return
	
func draw_fruit() -> void:
	tile_map.set_cell(0, fruit.get_body(), SOURCE_ID, ATLAS_FRUIT)	
	
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
	
	init_snake()
	init_fruit()
	wall.init_wall(board_width, board_height)
	draw_board()
	draw_wall()
	draw_snake()
	draw_fruit()
	Turn_Timer.wait_time = 1.0   # 1000 ms
	Turn_Timer.one_shot = false  # Timer powtarzalny - Gdyby True to wywołał by się tylko raz
	Turn_Timer.start()           #Start odliczania czasu

func _next_turn() -> void:
	move_snake()
	draw_board()
	draw_wall()
	draw_snake()
	draw_fruit()
	print("Next turn")
	

#Sygnał - metoda łącząca Game z timer
func _on_Turn_Timer_timeout() -> void:
	_next_turn()
