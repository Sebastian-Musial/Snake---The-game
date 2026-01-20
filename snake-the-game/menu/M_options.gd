extends Control

class_name M_Options

#Po dodaniu opcji zmień wymiary kontenerów

func _on_button_pressed() -> void:
	get_tree().change_scene_to_file("res://menu/Menu.tscn")
