extends Game_Rules
class_name Rules_Hard

#Enkapsulacja zgodnie z konwencja OOP czyli stosujemy _<nazwa zmiennej>
#Private - konwencja ponieważ GDscript pomimo obiektowości tego nie oferuje

#Public
#Get
#Set
#Metody
func get_board_size() -> Vector2i:	#Taki sam w normal, do zmiany rozmiar później
	return Vector2i(20, 20)

func get_turn_time() -> float:
	return 0.5 #Sekundy
