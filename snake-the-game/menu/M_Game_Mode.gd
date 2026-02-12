extends Control


func _on_normal_pressed() -> void:
	GState.rules = Rules_Normal.new()
	get_tree().change_scene_to_file("res://game/Game.tscn")

func _on_hard_pressed() -> void:
	GState.rules = Rules_Hard.new()
	get_tree().change_scene_to_file("res://game/Game.tscn")
