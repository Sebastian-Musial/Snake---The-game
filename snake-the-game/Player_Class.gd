extends RefCounted

class_name Player
#Enkapsulacja zgodnie z konwencja OOP czyli stosujemy _<nazwa zmiennej>
#Private - konwencja ponieważ GDscript pomimo obiektowości tego nie oferuje
var _score:= 0

#Public
#Get
func get_score() -> int:
	return _score
#Set
func set_score(N_score) -> void:
	_score = N_score

#Metody
func add_one_point() -> void:
	set_score(_score + 1)

func reset_score() -> void:
	set_score(0)
