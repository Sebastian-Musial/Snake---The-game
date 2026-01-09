extends RefCounted

class_name Fruit

#Enkapsulacja zgodnie z konwencja OOP czyli stosujemy _<nazwa zmiennej>
#Private - konwencja ponieważ GDscript pomimo obiektowości tego nie oferuje
var _fruit_body: Vector2i = Vector2i.ZERO
var _exist:= true
#Public
#Get
func get_body() -> Vector2i:
	return _fruit_body 
func get_exist() -> bool:
	return _exist
	
#Set
func set_body(N_body) -> void:
	_fruit_body = N_body 
func set_exist(N_exist) -> void:
	_exist = N_exist
	
#Metody
#Te argumenty bedzie trzeba doprecyzowac. Na tym etapie sa zapisane aby wiedziec co potrzebne ale chwilowo to nie dziala
func spawn(board_size: Vector2i, snake_body: Array[Vector2i], rng: RandomNumberGenerator) -> void:
	#Tutaj zapewne vector2i dla board - chwilowy szkic metody
	for i in range(4000):
		var pos := Vector2i(
			rng.randi_range(1, board_size.x - 2), 
			rng.randi_range(1, board_size.y - 2))
		if pos not in snake_body:
			_fruit_body = pos
			return
	_exist = false
	_fruit_body = Vector2i.ZERO
