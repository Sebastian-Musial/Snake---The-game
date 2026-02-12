extends RefCounted

class_name Board

#Enkapsulacja zgodnie z konwencja OOP czyli stosujemy _<nazwa zmiennej>
#Private - konwencja ponieważ GDscript pomimo obiektowości tego nie oferuje
var _size: Vector2i = Vector2i.ZERO
#Public
#Get
func get_size() -> Vector2i:
	return _size
func get_width() -> int:
	return _size.x
func get_height() -> int:
	return _size.y
#Set
func set_size(N_size) -> void:
	_size = N_size
#Metody
#Draw_Boards byl w C++ ale my to przeniesiemy do node2D który bedzie rysować wszystko

func inside_board(pos) -> bool:
	return ( (pos.x >= 0 and pos.x < _size.x) and (pos.y >= 0 and pos.y < _size.y) )
