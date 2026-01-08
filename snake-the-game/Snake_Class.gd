extends Player
class_name Snake

#Enkapsulacja zgodnie z konwencja OOP czyli stosujemy _<nazwa zmiennej>
#Private - konwencja ponieważ GDscript pomimo obiektowości tego nie oferuje
var _snake_body: Array[Vector2i] = []
var _dir: Types.directions
var _grows:= false

#Public
#Get
func get_body() -> Array[Vector2i]:
	return _snake_body 
	
#Set
#Hybryda z szablonu C++ set dir + try set dir
func set_dir(N_dir) -> void:
	if not Game_Mechanic.is_opposite(_dir, N_dir):
		_dir = N_dir
	
func set_grows(N_grows) -> void:
	_grows = N_grows
	
#Metody
func head() -> Vector2i:
	return _snake_body[0] if _snake_body.size() > 0 else Vector2i.ZERO

func reset(start_X, start_Y, N_dir = Types.directions.UP) -> void:
	_snake_body.clear()
	_snake_body.append(Vector2i(start_X, start_Y))
	_dir = N_dir #Czy to jest konieczne z argumentu?
	_grows = false
	#reset_score - musze stworzyc dla klasy player

func eat() -> void:
	if not _snake_body.is_empty():
		var last_element: Vector2i = _snake_body[-1]
		_snake_body.append(last_element)

#func occupies(vec2i) -> const bool
#Metoda ze szkicu C++ ale na tą chwile nie potrzebna bo wszystko mam w Vector2i

func grows() -> void:
	set_grows(true) 
	
