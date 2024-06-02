extends Node2D

var current_winner: String = ""

# Called when the node enters the scene tree for the first time.
func _ready():
	$NewGameBtn.pressed.connect(start_new_game)
	start_new_game()

func start_new_game():
	$Board.clear_board()
	current_winner = $Board.get_winner()
	$NewGameBtn.disabled = true
	sync_winner_palyer_with_ui()
	
func sync_winner_palyer_with_ui():
	if current_winner == "X":
		$WinnerPlayer.texture = load("res://icon-x-idle.svg")
	elif current_winner == "O":
		$WinnerPlayer.texture = load("res://icon-o-idle.svg")
	else:
		$WinnerPlayer.texture = null

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	if not $Board.is_empty():
		$NewGameBtn.disabled = false
	var new_current_winner = $Board.get_winner()
	if new_current_winner != current_winner:
		current_winner = new_current_winner
		if current_winner:
			$GameOverSoundFx.play()
		sync_winner_palyer_with_ui()
