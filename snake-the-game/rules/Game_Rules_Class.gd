extends RefCounted

class_name Game_Rules

#Enkapsulacja zgodnie z konwencja OOP czyli stosujemy _<nazwa zmiennej>
#Private - konwencja ponieważ GDscript pomimo obiektowości tego nie oferuje
#Public
#Get
#Set
#Metody
func get_board_size() -> Vector2i:
	push_error("Implementacja w podklasie - wzorzec STRATEGY")
	return Vector2i.ZERO

func get_turn_time() -> float:
	push_error("Implementacja w podklasie - wzorzec STRATEGY")
	return 1.0 #Sekunda
