extends Node2D

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
		var cell = get_cell(index)
		if cell:
			cell.cell_pressed.connect(func(): _on_cell_pressed(index))

func _on_cell_pressed(index: int):
	if self.winner:
		return
	var cell = get_cell(index)
	if not cell:
		return
	
	if cell.State != "":
		return
		
	cell.State = current_player
	if current_player == "X":
		x_moves.append(index)
		current_player = "O"
		if x_moves.size() >= 4:
			var first_move_cell = get_cell(x_moves.pop_front())
			if first_move_cell:
				first_move_cell.State = ""
				first_move_cell.Dimmed = false
	else:
		o_moves.append(index)
		current_player = "X"
		if o_moves.size() >= 4:
			var first_move_cell = get_cell(o_moves.pop_front())
			if first_move_cell:
				first_move_cell.State = ""
				first_move_cell.Dimmed = false
		
	if x_moves.size() >= 3:
		var first_move_cell = get_cell(x_moves.front())
		if first_move_cell:
			first_move_cell.Dimmed = true
	if o_moves.size() >= 3:
		var first_move_cell = get_cell(o_moves.front())
		if first_move_cell:
			first_move_cell.Dimmed = true
			
	var win_condition = check_win_conditions()
	if win_condition.is_empty():
		return
	
	self.winner = get_state(win_condition.front())
	print(win_condition)
	print(get_winner())
	dim_all_cells_except(win_condition)

func get_cell(index: int) -> DisplayCell:
	return get_node("GridContainer/Cell%d" % index) as DisplayCell

func get_winner() -> String:
	return self.winner

func dim_all_cells_except(win_condition: Array):
	for index in cell_indexes:
		var cell = get_cell(index)
		if not cell:
			continue
		cell.Dimmed = not win_condition.has(index)
		

func check_win_conditions() -> Array:
	for win_condition in win_conditions:
		var state = ""
		for index in win_condition:
			var cell_state = get_state(index)
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

func get_state(index: int) -> String:
	var cell = get_cell(index)
	if not cell:
		return ""
	return cell.State
	
func set_state(index: int, state: String):
	var cell = get_cell(index)
	if cell:
		cell.State = state

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass
