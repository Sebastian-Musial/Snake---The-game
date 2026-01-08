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

#Elementy które będą rysowane


var wall_body: Array[Vector2i] = []

var rng := RandomNumberGenerator.new()

enum directions { UP, DOWN, LEFT, RIGHT }

#Metody
func draw_board() -> void:
	tile_map.clear()
	
	for y in board_height:
		for x in board_width:
			tile_map.set_cell(0, Vector2i(x, y), SOURCE_ID, ATLAS_BG)

func init_wall() -> void:
	wall_body.clear()
	for X in range(board_width):
		wall_body.append(Vector2i(X, 0))
		wall_body.append(Vector2i(X, board_height))
	for Y in range(board_width):
		wall_body.append(Vector2i(0, Y))
		wall_body.append(Vector2i(board_width, Y))
	wall_body.append(Vector2i(board_width, board_height))	
	
func draw_wall() -> void:	#ściany są rysowane poza planszą - w grze nie widać ale matematyka się nie zgadza
	for segment in wall_body:
		tile_map.set_cell(0, segment, SOURCE_ID, ATLAS_WALL)

#Aktualnie rysuje się tylko pierwszy segment węża
func init_snake() -> void:
	snake_body.clear()
	snake_body.append(Vector2i(int(board_width / 2), int(board_height / 2)))

func draw_snake() -> void:
	for segment in snake_body:
		tile_map.set_cell(0, segment, SOURCE_ID, ATLAS_SNAKE)


#Narazie testowo głowa się przesuwa w prawo 
func move_snake() -> void:
	var head := snake_body[0]
	var new_head := head + Vector2i(1, 0) # w prawo
	
	snake_body.insert(0, new_head)	#Nowa głowa
	snake_body.pop_back()			#Kasowanie ogona - ostatniego elementu


#Owoc ma już swoj spawn z RNG dla pozycji [bug:owov moze sie generowac w scianie]
func init_fruit() -> void:
	while true:
		var pos := Vector2i(rng.randi_range(0, board_width - 1), rng.randi_range(0, board_height - 1))
		if pos not in snake_body:
			fruit_body = pos
			return
	
func draw_fruit() -> void:
	tile_map.set_cell(0, fruit_body, SOURCE_ID, ATLAS_FRUIT)
	
#Z template C++ - Game_Mechanic.Is_Opposite 
static func is_opposite(input_dir: direction, temp_dir: direction) -> bool: 
	return ( 
		(input_dir == direction.UP and temp_dir == direction.DOWN) or 
		(input_dir == direction.DOWN and temp_dir == direction.UP) or 
		(input_dir == direction.LEFT and temp_dir == direction.RIGHT) or
		(input_dir == direction.RIGHT and temp_dir == direction.LEFT))
	
	
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
	init_snake()
	init_fruit()
	init_wall()
	draw_board()
	draw_wall()
	draw_snake()
	draw_fruit()
	Turn_Timer.wait_time = 1.0   # 1000 ms
	Turn_Timer.one_shot = false  # Timer powtarzalny - Gdyby True to wywołał by się tylko raz
	Turn_Timer.start()           #Start odliczania czasu


#Sygnał - metoda łącząca Game z timer
func _on_Turn_Timer_timeout() -> void:
	_next_turn()

func _next_turn() -> void:
	move_snake()
	draw_board()
	draw_wall()
	draw_snake()
	draw_fruit()
	print("Next turn")
	
