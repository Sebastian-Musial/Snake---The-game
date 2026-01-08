extends RefCounted

class_name Game_Mechanic

#Enkapsulacja zgodnie z konwencja OOP czyli stosujemy _<nazwa zmiennej>
#Private - konwencja ponieważ GDscript pomimo obiektowości tego nie oferuje
#Public
#Get
#Set
#Metody
static func next_move(act_pos: Vector2i, dir) -> Vector2i:
	var next_pos := act_pos
	match dir:
		direction.UP:
			next_pos.y = act_pos.y - 1 
		direction.DOWN:
			next_pos.y = act_pos.y + 1 
		direction.LEFT:
			next_pos.x = act_pos.x - 1
		direction.RIGHT:
			next_pos.x = act_pos.x + 1
	return next_pos
	
static func is_opposite(input_dir: direction, temp_dir: direction) -> bool: 
	return ( 
		(input_dir == direction.UP and temp_dir == direction.DOWN) or 
		(input_dir == direction.DOWN and temp_dir == direction.UP) or 
		(input_dir == direction.LEFT and temp_dir == direction.RIGHT) or
		(input_dir == direction.RIGHT and temp_dir == direction.LEFT))
