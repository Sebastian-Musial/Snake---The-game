extends RefCounted

class_name Fruit

#Enkapsulacja zgodnie z konwencja OOP czyli stosujemy _<nazwa zmiennej>
#Private - konwencja ponieważ GDscript pomimo obiektowości tego nie oferuje
var _fruit_body: Vector2i = Vector2i.ZERO
#Public
#Get
func get_body() -> Vector2i:
	return _fruit_body 
#Set
func set_body(N_body) -> void:
	_fruit_body = N_body 
	
#Metody
#Te argumenty bedzie trzeba doprecyzowac. Na tym etapie sa zapisane aby wiedziec co potrzebne ale chwilowo to nie dziala
func spawn(board_size: Vector2i, snake_body: Array[Vector2i], rng: RandomNumberGenerator) -> void:
	#Tutaj zapewne vector2i dla board - chwilowy szkic metody
	while true:
		var pos := Vector2i(
			rng.randi_range(0, board_size.x - 1), 
			rng.randi_range(0, board_size.y - 1))
		if pos not in snake_body:
			_fruit_body = pos
			return
		
