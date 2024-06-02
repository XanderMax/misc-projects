class_name DisplayCell extends CenterContainer

@export_group("Extra Properties")
@export var State: String = ""
@export var Dimmed: bool = false
signal cell_pressed

func _gui_input(event):
	if not (event is InputEventMouseButton):
		return
	if event.is_pressed() and event.button_index == MOUSE_BUTTON_LEFT:
		on_click()

func on_click():
	cell_pressed.emit()

func sync_state_with_ui():
	$X.visible = false
	$O.visible = false
	if State == "X":
		$X.visible = true
	if State == "O":
		$O.visible = true
	if Dimmed:
		modulate = Color(1., 1., 1., .3)
	else:
		modulate = Color(1., 1., 1., 1.)
		

# Called when the node enters the scene tree for the first time.
func _ready():
	print(State)
	sync_state_with_ui()

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	sync_state_with_ui()
