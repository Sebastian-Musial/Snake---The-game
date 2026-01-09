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
#Init
#Aktualnie rysuje się tylko pierwszy segment węża
func init_snake(board_size: Vector2i) -> void:
	_snake_body.clear()
	_snake_body.append(Vector2i(int(board_size.x / 2), int(board_size.y / 2)))

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
	
#Narazie testowo głowa się przesuwa w prawo 
func move_snake() -> void:
	var head := get_body()[0]
	var new_head := head + Vector2i(1, 0) # w prawo
	
	get_body().insert(0, new_head)	#Nowa głowa
	get_body().remove_at(get_body().size() - 1)			#Kasowanie ogona - ostatniego elementu
