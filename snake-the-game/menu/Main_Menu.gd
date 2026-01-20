extends Control

class_name Main_Menu



func _on_play_pressed() -> void:
	get_tree().change_scene_to_file("res://game/Game.tscn")


func _on_options_pressed() -> void:
	get_tree().change_scene_to_file("res://menu/Options.tscn")


func _on_author_pressed() -> void:
	get_tree().change_scene_to_file("res://menu/Author.tscn")


func _on_exit_pressed() -> void:
	get_tree().quit()


#Menu -> Author
