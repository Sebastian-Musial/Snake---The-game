extends Control

class_name M_Author

func _on_button_pressed() -> void:
	get_tree().change_scene_to_file("res://menu/Menu.tscn")
