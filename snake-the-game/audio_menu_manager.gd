extends AudioStreamPlayer

func play_music(stream: AudioStream, volume_db := -12):
	if self.stream == stream and playing:
		return
	self.stream = stream
	volume_db = volume_db
	play()

func stop_music():
	stop()
