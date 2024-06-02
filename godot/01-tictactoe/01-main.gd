extends Node2D


# Called when the node enters the scene tree for the first time.
func _ready():
	$NewGameBtn.pressed.connect(start_new_game)
	start_new_game()

func start_new_game():
	$Board.clear_board()
	$NewGameBtn.disabled = true

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	if not $Board.is_empty():
		$NewGameBtn.disabled = false
