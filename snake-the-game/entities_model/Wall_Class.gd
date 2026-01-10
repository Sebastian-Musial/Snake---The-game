extends RefCounted

class_name Wall

#Enkapsulacja zgodnie z konwencja OOP czyli stosujemy _<nazwa zmiennej>
#Private - konwencja ponieważ GDscript pomimo obiektowości tego nie oferuje
var _wall_body: Array[Vector2i] = []
#Public
#Get
func get_body() -> Array[Vector2i]:
	return _wall_body 
#Set
func set_body(N_body) -> void:
	_wall_body = N_body
#Metody

#Init
func init_wall(board_width, board_height) -> void:
	_wall_body.clear()
	for X in range(board_width):
		_wall_body.append(Vector2i(X, 0))
		_wall_body.append(Vector2i(X, board_height - 1))
	for Y in range(board_height):
		_wall_body.append(Vector2i(0, Y))
		_wall_body.append(Vector2i(board_width - 1, Y))
		
