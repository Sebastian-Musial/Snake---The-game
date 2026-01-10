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
		Types.directions.UP:
			next_pos.y = act_pos.y - 1 
		Types.directions.DOWN:
			next_pos.y = act_pos.y + 1 
		Types.directions.LEFT:
			next_pos.x = act_pos.x - 1
		Types.directions.RIGHT:
			next_pos.x = act_pos.x + 1
	return next_pos
	
static func is_opposite(input_dir: Types.directions, temp_dir: Types.directions) -> bool: 
	return ( 
		(input_dir == Types.directions.UP and temp_dir == Types.directions.DOWN) or 
		(input_dir == Types.directions.DOWN and temp_dir == Types.directions.UP) or 
		(input_dir == Types.directions.LEFT and temp_dir == Types.directions.RIGHT) or
		(input_dir == Types.directions.RIGHT and temp_dir == Types.directions.LEFT))
