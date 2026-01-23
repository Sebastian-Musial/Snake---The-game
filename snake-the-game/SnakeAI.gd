extends RefCounted
class_name SnakeAI

#Odgległość taksówkowa(manhattan) - odległość jaką należy pokonać z miejsca A do miejsca B
#W moim przypadku ilość kratek od węża do owocu
static func _distance(a: Vector2i, b: Vector2i) -> int:
	return abs(a.x - b.x) + abs(a.y - b.y)

static func _all_dir() -> Array:
	return [
		Types.directions.UP,
		Types.directions.DOWN,
		Types.directions.LEFT,
		Types.directions.RIGHT
	]

#Czy allow tail konieczny?
static func _occupied_tiles(snake_body: Array[Vector2i], wall_body: Array[Vector2i], allow_tail: bool = true) -> Dictionary:
	var occ: Dictionary = {}

	for p in wall_body:
		occ[p] = true

#Zbiera wszystkie współrzędne snake body poza ogonem
	var limit := snake_body.size()
	if allow_tail and limit > 0:
		limit -= 1

	for i in range(limit):
		occ[snake_body[i]] = true

	return occ

#Funkcja przygotowująca tablicę dostępnych kierunków które nie zabiją węża
static func _safe_dirs(board: Board, snake: Snake, occ: Dictionary) -> Array:
	var head: Vector2i = snake.get_body()[0]
	var dirs: Array = []

	for d in _all_dir():
		
		if Game_Mechanic.is_opposite(snake.get_dir(), d):
			continue

		var next := Game_Mechanic.next_move(head, d)
		if board.inside_board(next) and not occ.has(next):
			dirs.append(d)
	return dirs


#Greedy - Algorytm zachłanny
#Założenia wybiera najbardziej w danym momencie korzysty ruch - nie planuje przyszłych ruchów więc jest mało optymalny - łatwo przegrywa
static func choose_dir_greedy(board: Board, snake: Snake, fruit: Fruit, wall: Wall) -> Types.directions:
	var body := snake.get_body()
	if body.is_empty():
		return Types.directions.UP

	var head: Vector2i = body[0]
	var target: Vector2i = fruit.get_body()

	var occ := _occupied_tiles(body, wall.get_body(), true)
	var safe_dirs := _safe_dirs(board, snake, occ)

#Brak dostępnych bezpiecznych dróg - wybierz obecny kierunek
	if safe_dirs.is_empty():
		return snake.get_dir()

	var best_dir: Types.directions = safe_dirs[0]
	var best_score := 1_000_000

	for d in safe_dirs:
		var next := Game_Mechanic.next_move(head, d)
		var score := _distance(next, target)
		if score < best_score:
			best_score = score
			best_dir = d

	return best_dir
