extends GridContainer

const cell_indexes: Array = [0, 1, 2, 3, 4, 5, 6, 7, 8]
const win_conditions : Array = [
	[0, 1, 2], [1, 4, 7],
	[3, 4, 5], [2, 5, 8],
	[6, 7, 8], [0, 4, 8],
	[0, 3, 6], [2, 4, 6]
]
var winner: String = ""
var x_moves: Array
var o_moves: Array
var current_player: String = "X"

func _ready():
	for index in cell_indexes:
		var cell = _get_cell(index)
		print(index)
		if cell:
			cell.cell_pressed.connect(func(): _on_cell_pressed(index))
			
# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass
	
func clear_board():
	self.winner = ""
	x_moves.clear()
	o_moves.clear()
	current_player = "X"
	_dim_all_cells_except(cell_indexes)
	for index in cell_indexes:
		_set_state(index, "")
	_update_cell_frames()

func _on_cell_pressed(index: int):
	if self.winner:
		return
	var cell = _get_cell(index)
	if not cell:
		return
	
	if cell.State != "":
		return
		
	cell.State = current_player
	if current_player == "X":
		x_moves.append(index)
		current_player = "O"
		if x_moves.size() >= 4:
			var first_move_cell = _get_cell(x_moves.pop_front())
			if first_move_cell:
				first_move_cell.State = ""
				first_move_cell.Dimmed = false
	else:
		o_moves.append(index)
		current_player = "X"
		if o_moves.size() >= 4:
			var first_move_cell = _get_cell(o_moves.pop_front())
			if first_move_cell:
				first_move_cell.State = ""
				first_move_cell.Dimmed = false
		
	if x_moves.size() >= 3:
		var first_move_cell = _get_cell(x_moves.front())
		if first_move_cell:
			first_move_cell.Dimmed = true
	if o_moves.size() >= 3:
		var first_move_cell = _get_cell(o_moves.front())
		if first_move_cell:
			first_move_cell.Dimmed = true
			
	var win_condition = _check_win_conditions()
	if not win_condition.is_empty():
		self.winner = _get_state(win_condition.front())
		print(win_condition)
		print(get_winner())
		_dim_all_cells_except(win_condition)
	_update_cell_frames()

func _get_cell(index: int) -> DisplayCell:
	var node_name = "Cell%d" % index
	var node = get_node(node_name) as DisplayCell
	if not node:
		var err_text = "Node name '{name}' not found".format({"name":node_name})
		push_error(err_text)
		printerr(err_text)
	return node

func get_winner() -> String:
	return self.winner
	
func is_empty():
	return self.x_moves.is_empty() && self.o_moves.is_empty()

func _dim_all_cells_except(win_condition: Array):
	for index in cell_indexes:
		var cell = _get_cell(index)
		if not cell:
			continue
		cell.Dimmed = not win_condition.has(index)

func _update_cell_frames():
	for index in cell_indexes:
		var cell = _get_cell(index)
		if not cell:
			continue
		if cell.State.is_empty() and self.winner.is_empty():
			cell.set_frame(self.current_player)
		else:
			cell.set_frame("")

func _check_win_conditions() -> Array:
	for win_condition in win_conditions:
		var state = ""
		for index in win_condition:
			var cell_state = _get_state(index)
			if not cell_state:
				state = ""
				break
			if not state:
				state = cell_state
				continue
			if state != cell_state:
				state = ""
				break
		if state:
			return win_condition
	return []
	
func get_current_player() -> String:
	return self.current_player 

func _get_state(index: int) -> String:
	var cell = _get_cell(index)
	if not cell:
		return ""
	return cell.State
	
func _set_state(index: int, state: String):
	var cell = _get_cell(index)
	if cell:
		cell.State = state
