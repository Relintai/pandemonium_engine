/*
Copyright (c) 2019 Flairieve
Copyright (c) 2020-2022 cobrapitz
Copyright (c) 2022 Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "paint_window.h"

#include "actions/paint_action.h"
#include "paint_canvas.h"
#include "paint_canvas_layer.h"
#include "paint_color_grid.h"
#include "paint_layer_button.h"
#include "paint_navbar.h"
#include "scene/resources/style_box.h"

#include "scene/gui/button.h"
#include "scene/gui/check_button.h"
#include "scene/gui/color_picker.h"
#include "scene/gui/label.h"
#include "scene/gui/margin_container.h"
#include "scene/gui/panel_container.h"
#include "scene/gui/rich_text_label.h"
#include "scene/gui/scroll_container.h"
#include "scene/gui/slider.h"
#include "scene/gui/texture_button.h"

#include "dialogs/paint_canvas_dialog.h"
#include "dialogs/paint_change_grid_size_dialog.h"
#include "dialogs/paint_load_file_dialog.h"
#include "dialogs/paint_save_file_dialog.h"
#include "paint_settings.h"

void PaintWindow::_input(const Ref<InputEvent> &event) {
	/*
	if is_any_menu_open():
		return
	if not is_visible_in_tree():
		return
	if paint_canvas_container_node == null or paint_canvas == null:
		return

	if event is InputEventKey and event.is_pressed() and not event.is_echo():
		_handle_shortcuts(event.scancode)

	if is_mouse_in_canvas() and paint_canvas.mouse_on_top:
		_handle_zoom(event)

	if paint_canvas.is_active_layer_locked():
		return

	if brush_mode == Tools.CUT:
		if event is InputEventMouseButton:
			if event.button_index == BUTTON_LEFT:
				if not event.pressed:
					commit_action()

	if (paint_canvas.mouse_in_region and paint_canvas.mouse_on_top):
		if event is InputEventMouseButton:
			match brush_mode:
				Tools.BUCKET:
					if event.button_index == BUTTON_LEFT:
						if event.pressed:
							if _current_action == null:
								_current_action = get_action()
							do_action([cell_mouse_position, last_cell_mouse_position, selected_color])

				Tools.COLORPICKER:
					if event.button_index == BUTTON_LEFT:
						if event.pressed:
							if paint_canvas.get_pixel(cell_mouse_position.x, cell_mouse_position.y).a == 0:
								return
							selected_color = paint_canvas.get_pixel(cell_mouse_position.x, cell_mouse_position.y)
							_picked_color = true
							find_node("Colors").add_color_prefab(selected_color)
						elif _picked_color:
							set_brush(_previous_tool)
					elif event.button_index == BUTTON_RIGHT:
						if event.pressed:
							set_brush(_previous_tool)

				Tools.PASTECUT:
					if event.button_index == BUTTON_RIGHT:
						if event.pressed:
							commit_action()
							set_brush(Tools.PAINT)
	*/
}
void PaintWindow::_process(float delta) {
	/*
	if not is_visible_in_tree():
		return
	if paint_canvas_container_node == null or paint_canvas == null:
		return
	if is_any_menu_open():
		return

	if is_mouse_in_canvas():
		_handle_scroll()

	#Update commonly used variables
	var grid_size = paint_canvas.pixel_size
	mouse_position = get_global_mouse_position() #paint_canvas.get_local_mouse_position()
	canvas_position = paint_canvas.rect_global_position
	canvas_mouse_position = Vector2(mouse_position.x - canvas_position.x, mouse_position.y - canvas_position.y)

	if is_mouse_in_canvas() && paint_canvas.mouse_on_top:
		cell_mouse_position = Vector2(
				floor(canvas_mouse_position.x / grid_size),
				floor(canvas_mouse_position.y / grid_size))
		cell_color = paint_canvas.get_pixel(cell_mouse_position.x, cell_mouse_position.y)

	update_text_info()

#	if not is_mouse_in_canvas():
#		paint_canvas.tool_layer.clear()
#		paint_canvas.update()
#		paint_canvas.tool_layer.update_texture()
#	else:
	if is_mouse_in_canvas() && paint_canvas.mouse_on_top:
		if not paint_canvas.is_active_layer_locked():
			if is_position_in_canvas(get_global_mouse_position()) or \
					is_position_in_canvas(_last_mouse_pos_canvas_area):
				brush_process()
			else:
				print(cell_mouse_position, " not in ", paint_canvas_container_node.rect_size)
				print("not in canvas")

		_draw_tool_brush()

	#Update last variables with the current variables
	last_mouse_position = mouse_position
	last_canvas_position = canvas_position
	last_canvas_mouse_position = canvas_mouse_position
	last_cell_mouse_position = cell_mouse_position
	last_cell_color = cell_color
	_last_mouse_pos_canvas_area = get_global_mouse_position() #paint_canvas_container_node.get_local_mouse_position()
	*/
}

void PaintWindow::_handle_shortcuts(const int scancode) {
	/*
	match scancode:
		K_UNDO:
			undo_action()

		K_REDO:
			redo_action()

		K_PENCIL:
			set_brush(Tools.PAINT)

		K_BRUSH:
			set_brush(Tools.BRUSH)

		K_BUCKET:
			set_brush(Tools.BUCKET)

		K_RAINBOW:
			set_brush(Tools.RAINBOW)

		K_LINE:
			set_brush(Tools.LINE)

		K_DARK:
			set_brush(Tools.DARKEN)

		K_BRIGHT:
			set_brush(Tools.BRIGHTEN)

		K_CUT:
			set_brush(Tools.CUT)

		K_PICK:
			set_brush(Tools.COLORPICKER)
	*/
}
void PaintWindow::_draw_tool_brush() {
	/*
	paint_canvas.tool_layer.clear()

	match brush_mode:
		Tools.PASTECUT:
			for idx in range(_selection_cells.size()):
				var pixel = _selection_cells[idx]
#				if pixel.x < 0 or pixel.y < 0:
#					print(pixel)
				var color = _selection_colors[idx]
				pixel -= _cut_pos + _cut_size / 2
				pixel += cell_mouse_position
				paint_canvas._set_pixel_v(paint_canvas.tool_layer, pixel, color)
		Tools.BRUSH:
			var pixels = BrushPrefabs.get_brush(selected_brush_prefab, find_node("BrushSize").value)
			for pixel in pixels:

				paint_canvas._set_pixel(paint_canvas.tool_layer,
						cell_mouse_position.x + pixel.x, cell_mouse_position.y + pixel.y, selected_color)

		Tools.RAINBOW:
			paint_canvas._set_pixel(paint_canvas.tool_layer,
					cell_mouse_position.x, cell_mouse_position.y, Color(0.46875, 0.446777, 0.446777, 0.196078))

		Tools.COLORPICKER:
			paint_canvas._set_pixel(paint_canvas.tool_layer,
					cell_mouse_position.x, cell_mouse_position.y, Color(0.866667, 0.847059, 0.847059, 0.196078))
		_:
			paint_canvas._set_pixel(paint_canvas.tool_layer,
					cell_mouse_position.x, cell_mouse_position.y, selected_color)

	paint_canvas.update()
	#TODO add here brush prefab drawing
	paint_canvas.tool_layer.update_texture()
	*/
}

void PaintWindow::_handle_scroll() {
	/*
	if Input.is_mouse_button_pressed(BUTTON_MIDDLE):
		if _middle_mouse_pressed_start_pos == null:
			_middle_mouse_pressed_start_pos = paint_canvas.rect_position
			_middle_mouse_pressed_pos = get_global_mouse_position()

		paint_canvas.rect_position = _middle_mouse_pressed_start_pos
		paint_canvas.rect_position += get_global_mouse_position() - _middle_mouse_pressed_pos

	elif _middle_mouse_pressed_start_pos != null:
		_middle_mouse_pressed_start_pos = null
	*/
}

void PaintWindow::_handle_zoom(const Ref<InputEvent> &event) {
	/*
	if not event is InputEventMouseButton:
		return
	if event.is_pressed():
		if event.button_index == BUTTON_WHEEL_UP:
			var px = min(paint_canvas.pixel_size * 2, max_zoom_in)
			if px == paint_canvas.pixel_size:
				return
			paint_canvas.set_pixel_size(px)
			find_node("CanvasBackground").material.set_shader_param(
					"pixel_size", 8 * pow(0.5, big_grid_pixels)/paint_canvas.pixel_size)
			paint_canvas.rect_position -= paint_canvas.get_local_mouse_position()
			paint_canvas.rect_position.x = clamp(paint_canvas.rect_position.x, -paint_canvas.rect_size.x * 0.8, rect_size.x)
			paint_canvas.rect_position.y = clamp(paint_canvas.rect_position.y, -paint_canvas.rect_size.y * 0.8, rect_size.y)
		elif event.button_index == BUTTON_WHEEL_DOWN:
			var px = max(paint_canvas.pixel_size / 2.0, max_zoom_out)
			if px == paint_canvas.pixel_size:
				return
			paint_canvas.set_pixel_size(px)
			find_node("CanvasBackground").material.set_shader_param(
					# 4 2 1
					"pixel_size", 8 * pow(0.5, big_grid_pixels)/paint_canvas.pixel_size)
			paint_canvas.rect_position += paint_canvas.get_local_mouse_position() / 2
			paint_canvas.rect_position.x = clamp(paint_canvas.rect_position.x, -paint_canvas.rect_size.x * 0.8, rect_size.x)
			paint_canvas.rect_position.y = clamp(paint_canvas.rect_position.y, -paint_canvas.rect_size.y * 0.8, rect_size.y)
	*/
}

void PaintWindow::_handle_cut() {
	/*
	if Input.is_mouse_button_pressed(BUTTON_RIGHT):
		paint_canvas.clear_preview_layer()
		set_brush(_previous_tool)
		return

	if Input.is_mouse_button_pressed(BUTTON_LEFT):
		for pixel_pos in GEUtils.get_pixels_in_line(cell_mouse_position, last_cell_mouse_position):
			for idx in range(_selection_cells.size()):
				var pixel = _selection_cells[idx]
				var color = _selection_colors[idx]
				pixel -= _cut_pos + _cut_size / 2
				pixel += pixel_pos
				paint_canvas.set_pixel_v(pixel, color)
	else:
		if _last_preview_draw_cell_pos == cell_mouse_position:
			return
		paint_canvas.clear_preview_layer()
		for idx in range(_selection_cells.size()):
			var pixel = _selection_cells[idx]
			var color = _selection_colors[idx]
			pixel -= _cut_pos + _cut_size / 2
			pixel += cell_mouse_position
			paint_canvas.set_preview_pixel_v(pixel, color)
		_last_preview_draw_cell_pos = cell_mouse_position
	*/
}
void PaintWindow::brush_process() {
	/*
	if Input.is_mouse_button_pressed(BUTTON_LEFT):
		if _current_action == null:
			_current_action = get_action()
		if brush_mode == Tools.COLORPICKER:
			_current_action = null

		match brush_mode:
			Tools.PAINT:
				do_action([cell_mouse_position, last_cell_mouse_position, selected_color])
			Tools.BRUSH:
				do_action([cell_mouse_position, last_cell_mouse_position, selected_color,
						selected_brush_prefab, find_node("BrushSize").value])
			Tools.LINE:
				do_action([cell_mouse_position, last_cell_mouse_position, selected_color])
			Tools.RECT:
				do_action([cell_mouse_position, last_cell_mouse_position, selected_color])
			Tools.DARKEN:
				do_action([cell_mouse_position, last_cell_mouse_position, selected_color])
			Tools.BRIGHTEN:
				do_action([cell_mouse_position, last_cell_mouse_position, selected_color])
			Tools.COLORPICKER:
				pass
			Tools.CUT:
				do_action([cell_mouse_position, last_cell_mouse_position, selected_color])
			Tools.PASTECUT:
				do_action([cell_mouse_position, last_cell_mouse_position,
						_selection_cells, _selection_colors,
						_cut_pos, _cut_size])
			Tools.RAINBOW:
				do_action([cell_mouse_position, last_cell_mouse_position])
		paint_canvas.update()

	elif Input.is_mouse_button_pressed(BUTTON_RIGHT):
		paint_canvas.update()
		if _current_action == null:
			_current_action = get_action()

		match brush_mode:
			Tools.PAINT:
				do_action([cell_mouse_position, last_cell_mouse_position, Color.transparent])
			Tools.BRUSH:
				do_action([cell_mouse_position, last_cell_mouse_position, Color.transparent,
						selected_brush_prefab, find_node("BrushSize").value])
	else:
		if _current_action and _current_action.can_commit():
			commit_action()
			paint_canvas.update()
	*/
}
void PaintWindow::update_text_info() {
	/*
	var text = ""

	var cell_color_text = cell_color
	cell_color_text = Color(0, 0, 0, 0)

	text += \
	str("FPS %s\t" + \
	"Mouse Position %s\t" + \
	"Canvas Mouse Position %s \t" + \
	"Canvas Position %s\t\n" + \
	"Cell Position %s \t" + \
	"Cell Color %s\t") % [
		str(Engine.get_frames_per_second()),
		str(mouse_position),
		str(canvas_mouse_position),
		str(canvas_position),
		str(cell_mouse_position),
		str(cell_color_text),
	]

	find_node("DebugTextDisplay").display_text(text)
	*/
}

void PaintWindow::_on_Save_pressed() {
	/*
	get_node("SaveFileDialog").show()
	*/
}

void PaintWindow::do_action(const Array &data) {
	/*
	if _current_action == null:
		#print("clear redo")
		_redo_history.clear()
	_current_action.do_action(paint_canvas, data)
	*/
}
void PaintWindow::commit_action() {
	/*
	if not _current_action:
		return

	#print("commit action")
	var commit_data = _current_action.commit_action(paint_canvas)
	var action = get_action()
	action.action_data = _current_action.action_data.duplicate(true)
	_actions_history.push_back(action)
	_redo_history.clear()

	match brush_mode:
		Tools.CUT:
			_cut_pos = _current_action.mouse_start_pos
			_cut_size = _current_action.mouse_end_pos - _current_action.mouse_start_pos
			_selection_cells = _current_action.action_data.redo.cells.duplicate()
			_selection_colors = _current_action.action_data.redo.colors.duplicate()
			set_brush(Tools.PASTECUT)
		_:
			_current_action = null
	*/
}
void PaintWindow::redo_action() {
	/*
	if _redo_history.empty():
		print("nothing to redo")
		return
	var action = _redo_history.pop_back()
	if not action:
		return
	_actions_history.append(action)
	action.redo_action(paint_canvas)
	paint_canvas.update()
	#print("redo action")
	*/
}
void PaintWindow::undo_action() {
	/*
	var action = _actions_history.pop_back()
	if not action:
		return
	_redo_history.append(action)
	action.undo_action(paint_canvas)
	update()
	paint_canvas.update()
	#print("undo action")
	*/
}

Ref<PaintAction> PaintWindow::get_action() {
	/*
	match brush_mode:
		Tools.PAINT:
			return GEPencil.new()
		Tools.BRUSH:
			return GEBrush.new()
		Tools.LINE:
			return GELine.new()
		Tools.RAINBOW:
			return GERainbow.new()
		Tools.BUCKET:
			return GEBucket.new()
		Tools.RECT:
			return GERect.new()
		Tools.DARKEN:
			return GEDarken.new()
		Tools.BRIGHTEN:
			return GEBrighten.new()
		Tools.CUT:
			return GECut.new()
		Tools.PASTECUT:
			return GEPasteCut.new()
		_:
			#print("no tool!")
			return null
	*/

	return Ref<PaintAction>();
}

void PaintWindow::set_selected_color(const Color &color) {
	/*
	selected_color = color
	*/
}
void PaintWindow::set_brush(const PaintWindow::Tools new_mode) {
	/*
	if brush_mode == new_mode:
		return
	_previous_tool = brush_mode
	brush_mode = new_mode

	_current_action = get_action()

	match _previous_tool:
		Tools.CUT:
			paint_canvas.clear_preview_layer()
		Tools.PASTECUT:
			_selection_cells.clear()
			_selection_colors.clear()
		Tools.BUCKET:
			_current_action = null
	#print("Selected: ", Tools.keys()[brush_mode])
	*/
}

void PaintWindow::change_color(const Color &new_color) {
	/*
	if new_color.a == 0:
		return
	selected_color = new_color
	find_node("ColorPicker").color = selected_color
	*/
}

void PaintWindow::_on_ColorPicker_color_changed(const Color &color) {
	/*
	selected_color = color
	*/
}
void PaintWindow::_on_PaintTool_pressed() {
	/*
	set_brush(Tools.PAINT)
	*/
}
void PaintWindow::_on_BucketTool_pressed() {
	/*
	set_brush(Tools.BUCKET)
	*/
}
void PaintWindow::_on_RainbowTool_pressed() {
	/*
	set_brush(Tools.RAINBOW)
	*/
}
void PaintWindow::_on_BrushTool_pressed() {
	/*
	set_brush(Tools.BRUSH)
	*/
}
void PaintWindow::_on_LineTool_pressed() {
	/*
	set_brush(Tools.LINE)
	*/
}
void PaintWindow::_on_RectTool_pressed() {
	/*
	set_brush(Tools.RECT)
	*/
}
void PaintWindow::_on_DarkenTool_pressed() {
	/*
	set_brush(Tools.DARKEN)
	*/
}
void PaintWindow::_on_BrightenTool_pressed() {
	/*
	set_brush(Tools.BRIGHTEN)
	*/
}
void PaintWindow::_on_ColorPickerTool_pressed() {
	/*
	set_brush(Tools.COLORPICKER)
	*/
}
void PaintWindow::_on_CutTool_pressed() {
	/*
	set_brush(Tools.CUT)
	*/
}
void PaintWindow::_on_Editor_visibility_changed() {
	/*
	pause_mode = not visible
	*/
}

void PaintWindow::highlight_layer(const String &layer_name) {
	/*
	for button in layer_buttons.get_children():
		if paint_canvas.find_layer_by_name(button.name).locked:
			button.get("custom_styles/panel").set("bg_color", locked_layer_highlight)
		elif button.name == layer_name:
			button.get("custom_styles/panel").set("bg_color", current_layer_highlight)
		else:
			button.get("custom_styles/panel").set("bg_color", other_layer_highlight)
	*/
}
void PaintWindow::toggle_layer_visibility(Node *button, const String &layer_name) {
	/*
	#print("toggling: ", layer_name)
	paint_canvas.toggle_layer_visibility(layer_name)
	*/
}
void PaintWindow::select_layer(const String &layer_name) {
	/*
	#print("select layer: ", layer_name)
	paint_canvas.select_layer(layer_name)
	highlight_layer(layer_name)
	*/
}
void PaintWindow::lock_layer(Node *button, const String &layer_name) {
	/*
	paint_canvas.toggle_lock_layer(layer_name)
	highlight_layer(paint_canvas.get_active_layer().name)
	*/
}

Ref<PaintCanvasLayer> PaintWindow::add_new_layer() {
	/*
	var new_layer_button = layer_buttons.get_child(0).duplicate()
	new_layer_button.set("custom_styles/panel", layer_buttons.get_child(0).get("custom_styles/panel").duplicate())
	layer_buttons.add_child_below_node(
			layer_buttons.get_child(layer_buttons.get_child_count() - 1), new_layer_button, true)
	_total_added_layers += 1
	new_layer_button.find_node("Select").text = "Layer " + str(_total_added_layers)
	_layer_button_ref[new_layer_button.name] = new_layer_button
	_connect_layer_buttons()

	var layer: GELayer = paint_canvas.add_new_layer(new_layer_button.name)

	highlight_layer(paint_canvas.get_active_layer().name)
	#print("added layer: ", layer.name)
	return layer
	*/

	return Ref<PaintCanvasLayer>();
}
void PaintWindow::remove_active_layer() {
	/*
	if layer_buttons.get_child_count() <= 1:
		return
	var layer_name = paint_canvas.active_layer.name
	paint_canvas.remove_layer(layer_name)
	layer_buttons.remove_child(_layer_button_ref[layer_name])
	_layer_button_ref[layer_name].queue_free()
	_layer_button_ref.erase(layer_name)

	highlight_layer(paint_canvas.get_active_layer().name)
	*/
}
void PaintWindow::duplicate_active_layer() {
	/*
	var new_layer_button = layer_buttons.get_child(0).duplicate()
	new_layer_button.set("custom_styles/panel", layer_buttons.get_child(0).get("custom_styles/panel").duplicate())
	layer_buttons.add_child_below_node(
			layer_buttons.get_child(layer_buttons.get_child_count() - 1), new_layer_button, true)

	_total_added_layers += 1 # for keeping track...
	new_layer_button.find_node("Select").text = "Layer " + str(_total_added_layers)

	var new_layer = paint_canvas.duplicate_layer(paint_canvas.active_layer.name, new_layer_button.name)
	new_layer.update_texture()
	_layer_button_ref[new_layer.name] = new_layer_button

	new_layer_button.find_node("Select").connect("pressed", self, "select_layer", [new_layer_button.name])
	new_layer_button.find_node("Visible").connect("pressed", self, "toggle_layer_visibility",
			[new_layer_button.find_node("Visible"), new_layer_button.name])
	new_layer_button.find_node("Up").connect("pressed", self, "move_down", [new_layer_button])
	new_layer_button.find_node("Down").connect("pressed", self, "move_up", [new_layer_button])
	new_layer_button.find_node("Lock").connect("pressed", self, "lock_layer", [new_layer_button, new_layer_button.name])

	# update highlight
	highlight_layer(paint_canvas.get_active_layer().name)
	#print("added layer: ", new_layer.name, " (total:", layer_buttons.get_child_count(), ")")
	*/
}

void PaintWindow::move_up(Node *layer_btn) {
	/*
	var new_idx = min(layer_btn.get_index() + 1, layer_buttons.get_child_count())
	#print("move_up: ", layer_btn.name, " from ", layer_btn.get_index(), " to ", new_idx)
	layer_buttons.move_child(layer_btn, new_idx)
	paint_canvas.move_layer_back(layer_btn.name)
	*/
}
void PaintWindow::move_down(Node *layer_btn) {
	/*
	var new_idx = max(layer_btn.get_index() - 1, 0)
	#print("move_down: ", layer_btn.name, " from ", layer_btn.get_index(), " to ", new_idx)
	layer_buttons.move_child(layer_btn, new_idx)
	paint_canvas.move_layer_forward(layer_btn.name)
	*/
}

void PaintWindow::_connect_layer_buttons() {
	/*
	for layer_btn in layer_buttons.get_children():
		if layer_btn.find_node("Select").is_connected("pressed", self, "select_layer"):
			continue
		layer_btn.find_node("Select").connect("pressed", self, "select_layer", [layer_btn.name])
		layer_btn.find_node("Visible").connect("pressed", self, "toggle_layer_visibility",
				[layer_btn.find_node("Visible"), layer_btn.name])
		layer_btn.find_node("Up").connect("pressed", self, "move_down", [layer_btn])
		layer_btn.find_node("Down").connect("pressed", self, "move_up", [layer_btn])
		layer_btn.find_node("Lock").connect("pressed", self, "lock_layer",
				[layer_btn, layer_btn.name])
	*/
}

void PaintWindow::_on_Button_pressed() {
	/*
	add_new_layer()
	*/
}
void PaintWindow::_on_PaintCanvasContainer_mouse_entered() {
	/*
	if mouse_on_top == true:
		return
	mouse_on_top = true
	paint_canvas.tool_layer.clear()
	paint_canvas.update()
	paint_canvas.tool_layer.update_texture()
	*/
}
void PaintWindow::_on_PaintCanvasContainer_mouse_exited() {
	/*
	if mouse_on_top == false:
		return
	mouse_on_top = false
	paint_canvas.tool_layer.clear()
	paint_canvas.update()
	paint_canvas.tool_layer.update_texture()
	*/
}
void PaintWindow::_on_ColorPicker_popup_closed() {
	/*
	find_node("Colors").add_color_prefab(find_node("ColorPicker").color)
	*/
}

bool PaintWindow::is_position_in_canvas(const Vector2 &pos) {
	/*
	if Rect2(paint_canvas_container_node.rect_global_position,
			 paint_canvas_container_node.rect_global_position + paint_canvas_container_node.rect_size).has_point(pos):
		return true
	return false
	*/

	return false;
}

bool PaintWindow::is_mouse_in_canvas() {
	/*
	if is_position_in_canvas(get_global_mouse_position()):
		return true #mouse_on_top # check if mouse is inside canvas
	else:
		return false
	*/
	return false;
}

bool PaintWindow::is_any_menu_open() {
	/*
	return $ChangeCanvasSize.visible or \
			$ChangeGridSizeDialog.visible or \
			$Settings.visible or \
			$LoadFileDialog.visible or \
			$SaveFileDialog.visible or \
			find_node("Navbar").is_any_menu_open()
	*/
	return false;
}

void PaintWindow::_on_LockAlpha_pressed() {
	/*
	var checked = find_node("LockAlpha").pressed
	paint_canvas.active_layer.toggle_alpha_locked()
	for i in range(find_node("Layer").get_popup().get_item_count()):
		if find_node("Layer").get_popup().get_item_text(i) == "Toggle Alpha Locked":
			find_node("Layer").get_popup().set_item_checked(i, not find_node("Layer").get_popup().is_item_checked(i))
	*/
}
void PaintWindow::_on_BrushRect_pressed() {
	/*
	if brush_mode != Tools.BRUSH:
		set_brush(Tools.BRUSH)
	selected_brush_prefab = BrushPrefabs.Type.RECT
	*/
}
void PaintWindow::_on_BrushCircle_pressed() {
	/*
	if brush_mode != Tools.BRUSH:
		set_brush(Tools.BRUSH)
	selected_brush_prefab = BrushPrefabs.Type.CIRCLE
	*/
}
void PaintWindow::_on_BrushVLine_pressed() {
	/*
	if brush_mode != Tools.BRUSH:
		set_brush(Tools.BRUSH)
	selected_brush_prefab = BrushPrefabs.Type.V_LINE
	*/
}
void PaintWindow::_on_BrushHLine_pressed() {
	/*
	if brush_mode != Tools.BRUSH:
		set_brush(Tools.BRUSH)
	selected_brush_prefab = BrushPrefabs.Type.H_LINE
	*/
}
void PaintWindow::_on_BrushSize_value_changed(float value) {
	/*
	find_node("BrushSizeLabel").text = str(int(value))
	*/
}
void PaintWindow::_on_XSymmetry_pressed() {
	/*
	paint_canvas.symmetry_x = not paint_canvas.symmetry_x
	*/
}
void PaintWindow::_on_YSymmetry_pressed() {
	/*
	paint_canvas.symmetry_y = not paint_canvas.symmetry_y
	*/
}

PaintWindow::PaintWindow() {
	/*

	var layer_buttons: Control
	var paint_canvas_container_node
	var paint_canvas: GECanvas
	var canvas_background: TextureRect
	var grids_node
	var colors_grid
	var selected_color = Color(1, 1, 1, 1) setget set_selected_color
	var util = preload("res://addons/Godoxel/Util.gd")
	var textinfo
	var allow_drawing = true

	var mouse_in_region = false
	var mouse_on_top = false


	var _middle_mouse_pressed_pos = null
	var _middle_mouse_pressed_start_pos = null
	var _left_mouse_pressed_start_pos = Vector2()
	var _previous_tool
	var brush_mode

	var _layer_button_ref = {}

	var _total_added_layers = 1

	var selected_brush_prefab = 0
	var _last_drawn_pixel = Vector2.ZERO
	var _last_preview_draw_cell_pos = Vector2.ZERO

	var _selection_cells = []
	var _selection_colors = []

	var _cut_pos = Vector2.ZERO
	var _cut_size = Vector2.ZERO

	var _actions_history = [] # for undo
	var _redo_history = []
	var _current_action

	var _last_mouse_pos_canvas_area = Vector2.ZERO

	var _picked_color = false

	var mouse_position = Vector2()
	var canvas_position = Vector2()
	var canvas_mouse_position = Vector2()
	var cell_mouse_position = Vector2()
	var cell_color = Color()

	var last_mouse_position = Vector2()
	var last_canvas_position = Vector2()
	var last_canvas_mouse_position = Vector2()
	var last_cell_mouse_position = Vector2()
	var last_cell_color = Color()

	const current_layer_highlight = Color(0.354706, 0.497302, 0.769531)
	const other_layer_highlight = Color(0.180392, 0.176471, 0.176471)
	const locked_layer_highlight = Color(0.098039, 0.094118, 0.094118)

	var big_grid_pixels = 4 # 1 grid-box is big_grid_pixels big



	#--------------------
	#Setup nodes
	#--------------------

	paint_canvas_container_node = find_node("PaintCanvasContainer")
	textinfo = find_node("DebugTextDisplay")
	selected_color = find_node("ColorPicker").color
	colors_grid = find_node("Colors")
	paint_canvas = paint_canvas_container_node.find_node("Canvas")
	layer_buttons = find_node("LayerButtons")
	canvas_background = find_node("CanvasBackground")

	set_process(true)

	#--------------------
	#connect nodes
	#--------------------
	if not colors_grid.is_connected("color_change_request", self, "change_color"):
		colors_grid.connect("color_change_request", self, "change_color")

	if not is_connected("visibility_changed", self, "_on_Editor_visibility_changed"):
		connect("visibility_changed", self, "_on_Editor_visibility_changed")

	find_node("CanvasBackground").material.set_shader_param(
			"pixel_size", 8 * pow(0.5, big_grid_pixels)/paint_canvas.pixel_size)

	# ready

	set_brush(Tools.PAINT)
	_layer_button_ref[layer_buttons.get_child(0).name] = layer_buttons.get_child(0) #ugly
	_connect_layer_buttons()
	highlight_layer(paint_canvas.get_active_layer().name)

	find_node("BrushSizeLabel").text = str(int(find_node("BrushSize").value))

	paint_canvas.update()

	*/

	set_clip_contents(true);
	set_h_size_flags(SIZE_EXPAND_FILL);
	set_v_size_flags(SIZE_EXPAND_FILL);

	//Main content panel
	PanelContainer *main_content_panel = memnew(PanelContainer);
	add_child(main_content_panel);
	main_content_panel->set_anchors_and_margins_preset(Control::PRESET_WIDE);

	VBoxContainer *main_content_container = memnew(VBoxContainer);
	main_content_panel->add_child(main_content_container);

	//Main Content Top (Navbar)
	navbar = memnew(PaintNavbar);
	navbar->paint_window = this;
	navbar->set_h_size_flags(SIZE_EXPAND_FILL);
	main_content_container->add_child(navbar);

	//Main Content Mid (App)
	HBoxContainer *app_mid_container = memnew(HBoxContainer);
	app_mid_container->set_h_size_flags(SIZE_EXPAND_FILL);
	app_mid_container->set_v_size_flags(SIZE_EXPAND_FILL);
	main_content_container->add_child(app_mid_container);

	//Main Content Mid (App) -- Left Panel
	PanelContainer *left_content_panel = memnew(PanelContainer);
	left_content_panel->set_h_size_flags(SIZE_EXPAND_FILL);
	left_content_panel->set_v_size_flags(SIZE_EXPAND_FILL);
	app_mid_container->add_child(left_content_panel);

	Ref<StyleBoxFlat> style_box;
	style_box.instance();
	style_box->set("bg_color", Color(0.2, 0.2, 0.2));
	left_content_panel->set("custom_styles/panel", style_box);

	//Main Content Mid (App) -- Left Panel -- Margin container
	MarginContainer *left_main_margin_container = memnew(MarginContainer);
	left_content_panel->add_child(left_main_margin_container);

	//Main Content Mid (App) -- Left Panel -- Margin container -- VBoxContainer
	VBoxContainer *left_main_vbox_container = memnew(VBoxContainer);
	left_main_vbox_container->set("custom_constants/separation", 12);
	left_main_margin_container->add_child(left_main_vbox_container);

	//Main Content Mid (App) -- Left Panel -- Margin container -- VBoxContainer -- Colors
	ScrollContainer *left_color_scroll_container = memnew(ScrollContainer);
	left_color_scroll_container->set_custom_minimum_size(Size2(0, 145));
	left_main_vbox_container->add_child(left_color_scroll_container);

	color_grid = memnew(PaintColorGrid);
	color_grid->set_columns(4);
	color_grid->set_custom_minimum_size(Size2(0, 145));
	color_grid->set_h_size_flags(SIZE_EXPAND_FILL);
	color_grid->set_v_size_flags(SIZE_EXPAND_FILL);
	left_color_scroll_container->add_child(color_grid);

	//Main Content Mid (App) -- Left Panel -- Margin container -- VBoxContainer -- Lock Alpha Button
	lock_alpha_button = memnew(CheckButton);
	lock_alpha_button->set_text("Lock Alpha");
	lock_alpha_button->set_text_align(Button::ALIGN_RIGHT);
	left_main_vbox_container->add_child(lock_alpha_button);

	//Main Content Mid (App) -- Left Panel -- Margin container -- VBoxContainer -- Brush Selection
	GridContainer *brush_container = memnew(GridContainer);
	brush_container->set_columns(4);
	left_main_vbox_container->add_child(brush_container);

	TextureButton *brush_rect_button = memnew(TextureButton);
	//brush_rect_button->set_normal_texture(); //res://addons/Godoxel/assets/BrushRect.png
	//brush_rect_button->set_hover_texture(); //res://addons/Godoxel/assets/BrushRect_Hovered.png
	brush_rect_button->set_custom_minimum_size(Size2(25, 25));
	brush_container->add_child(brush_rect_button);

	TextureButton *brush_circle_button = memnew(TextureButton);
	//brush_circle_button->set_normal_texture(); //res://addons/Godoxel/assets/BrushCircle.png
	//brush_circle_button->set_hover_texture(); //res://addons/Godoxel/assets/BrushCircle_Hovered.png
	brush_circle_button->set_custom_minimum_size(Size2(25, 25));
	brush_container->add_child(brush_circle_button);

	TextureButton *brush_v_line_button = memnew(TextureButton);
	//brush_v_line_button->set_normal_texture(); //res://addons/Godoxel/assets/BrushVLine.png
	//brush_v_line_button->set_hover_texture(); //res://addons/Godoxel/assets/BrushVLine_Hovered.png
	brush_v_line_button->set_custom_minimum_size(Size2(25, 25));
	brush_container->add_child(brush_v_line_button);

	TextureButton *brush_h_line_button = memnew(TextureButton);
	//brush_h_line_button->set_normal_texture(); //res://addons/Godoxel/assets/BrushHLine.png
	//brush_h_line_button->set_hover_texture(); //res://addons/Godoxel/assets/BrushHLine_Hovered.png
	brush_h_line_button->set_custom_minimum_size(Size2(25, 25));
	brush_container->add_child(brush_h_line_button);

	//Main Content Mid (App) -- Left Panel -- Margin container -- VBoxContainer -- Brush Size
	Label *brush_size_name_label = memnew(Label);
	brush_size_name_label->set_text("Brush Size");
	left_main_vbox_container->add_child(brush_size_name_label);

	HBoxContainer *brush_size_container = memnew(HBoxContainer);
	left_main_vbox_container->add_child(brush_size_container);

	brush_size_label = memnew(Label);
	brush_size_label->set_text("1");
	brush_size_container->add_child(brush_size_label);

	brush_size_slider = memnew(HSlider);
	brush_size_slider->set_min(1);
	brush_size_slider->set_value(1);
	brush_size_slider->set_h_size_flags(SIZE_EXPAND_FILL);
	brush_size_slider->set_v_size_flags(SIZE_EXPAND);
	brush_size_container->add_child(brush_size_slider);

	//Main Content Mid (App) -- Left Panel -- Margin container -- VBoxContainer -- Symmetries
	x_symmetry_button = memnew(CheckButton);
	x_symmetry_button->set_text("X Symmetry");
	left_main_vbox_container->add_child(x_symmetry_button);

	y_symmetry_button = memnew(CheckButton);
	y_symmetry_button->set_text("Y Symmetry");
	left_main_vbox_container->add_child(y_symmetry_button);

	//Main Content Mid (App) -- Mid (PaintCanvas)
	paint_canvas_container = memnew(Control);
	paint_canvas_container->set_clip_contents(true);
	paint_canvas_container->set_focus_mode(Control::FOCUS_CLICK);
	paint_canvas_container->set_mouse_filter(Control::MOUSE_FILTER_PASS);
	paint_canvas_container->set_h_size_flags(SIZE_EXPAND_FILL);
	paint_canvas_container->set_v_size_flags(SIZE_EXPAND_FILL);
	paint_canvas_container->set_stretch_ratio(6);
	app_mid_container->add_child(paint_canvas_container);

	paint_canvas = memnew(PaintCanvas);
	paint_canvas->set_pixel_size(4);
	paint_canvas->set_canvas_width(64);
	paint_canvas->set_canvas_height(64);
	paint_canvas->set_grid_size(1);
	paint_canvas->set_big_grid_size(8);
	paint_canvas->set_mouse_filter(Control::MOUSE_FILTER_PASS);
	paint_canvas->set_h_size_flags(SIZE_EXPAND_FILL);
	paint_canvas->set_v_size_flags(SIZE_EXPAND_FILL);
	paint_canvas->set_size(Size2(256, 256));
	paint_canvas->set_position(Point2());
	paint_canvas->set_anchors_and_margins_preset(Control::PRESET_CENTER);
	paint_canvas_container->add_child(paint_canvas);
	navbar->canvas = paint_canvas;

	//Main Content Mid (App) -- Right Panel
	PanelContainer *mid_right_panel_container = memnew(PanelContainer);
	mid_right_panel_container->set_h_size_flags(SIZE_EXPAND_FILL);
	mid_right_panel_container->set_v_size_flags(SIZE_EXPAND_FILL);
	app_mid_container->add_child(mid_right_panel_container);

	style_box.instance();
	style_box->set("bg_color", Color(0.2, 0.2, 0.2));
	mid_right_panel_container->set("custom_styles/panel", style_box);

	ScrollContainer *mid_right_main_scroll_container = memnew(ScrollContainer);
	mid_right_main_scroll_container->set_h_size_flags(SIZE_EXPAND_FILL);
	mid_right_main_scroll_container->set_v_size_flags(SIZE_EXPAND_FILL);
	mid_right_panel_container->add_child(mid_right_main_scroll_container);

	VBoxContainer *main_tool_contianer = memnew(VBoxContainer);
	main_tool_contianer->set_h_size_flags(SIZE_EXPAND_FILL);
	main_tool_contianer->set_v_size_flags(SIZE_EXPAND_FILL);
	mid_right_main_scroll_container->add_child(main_tool_contianer);

	//Main Content Mid (App) -- Right Panel -- Tools
	VBoxContainer *tools_contianer = memnew(VBoxContainer);
	tools_contianer->set_h_size_flags(SIZE_EXPAND_FILL);
	main_tool_contianer->add_child(tools_contianer);

	paint_tool_button = memnew(Button);
	paint_tool_button->set_text("Pencil (Q)");
	paint_tool_button->set_h_size_flags(SIZE_EXPAND_FILL);
	paint_tool_button->set_v_size_flags(SIZE_EXPAND_FILL);
	tools_contianer->add_child(paint_tool_button);

	brush_tool_button = memnew(Button);
	brush_tool_button->set_text("Brush");
	brush_tool_button->set_h_size_flags(SIZE_EXPAND_FILL);
	brush_tool_button->set_v_size_flags(SIZE_EXPAND_FILL);
	tools_contianer->add_child(brush_tool_button);
	brush_tool_button->hide();

	multi_tool_button = memnew(Button);
	multi_tool_button->set_text("Polygon");
	multi_tool_button->set_h_size_flags(SIZE_EXPAND_FILL);
	multi_tool_button->set_v_size_flags(SIZE_EXPAND_FILL);
	tools_contianer->add_child(multi_tool_button);
	multi_tool_button->hide();

	bucket_tool_button = memnew(Button);
	bucket_tool_button->set_text("Bucket Fill (F)");
	bucket_tool_button->set_h_size_flags(SIZE_EXPAND_FILL);
	bucket_tool_button->set_v_size_flags(SIZE_EXPAND_FILL);
	tools_contianer->add_child(bucket_tool_button);

	rainbow_tool_button = memnew(Button);
	rainbow_tool_button->set_text("Rainbow (R)");
	rainbow_tool_button->set_h_size_flags(SIZE_EXPAND_FILL);
	rainbow_tool_button->set_v_size_flags(SIZE_EXPAND_FILL);
	tools_contianer->add_child(rainbow_tool_button);

	line_tool_button = memnew(Button);
	line_tool_button->set_text("Line (L)");
	line_tool_button->set_h_size_flags(SIZE_EXPAND_FILL);
	line_tool_button->set_v_size_flags(SIZE_EXPAND_FILL);
	tools_contianer->add_child(line_tool_button);

	rect_tool_button = memnew(Button);
	rect_tool_button->set_text("Rectangle");
	rect_tool_button->set_h_size_flags(SIZE_EXPAND_FILL);
	rect_tool_button->set_v_size_flags(SIZE_EXPAND_FILL);
	tools_contianer->add_child(rect_tool_button);

	darken_tool_button = memnew(Button);
	darken_tool_button->set_text("Darken (D)");
	darken_tool_button->set_h_size_flags(SIZE_EXPAND_FILL);
	darken_tool_button->set_v_size_flags(SIZE_EXPAND_FILL);
	tools_contianer->add_child(darken_tool_button);

	brighten_tool_button = memnew(Button);
	brighten_tool_button->set_text("Brighten (B)");
	brighten_tool_button->set_h_size_flags(SIZE_EXPAND_FILL);
	brighten_tool_button->set_v_size_flags(SIZE_EXPAND_FILL);
	tools_contianer->add_child(brighten_tool_button);

	color_picker_tool_button = memnew(Button);
	color_picker_tool_button->set_text("Color Picker (P)");
	color_picker_tool_button->set_h_size_flags(SIZE_EXPAND_FILL);
	color_picker_tool_button->set_v_size_flags(SIZE_EXPAND_FILL);
	tools_contianer->add_child(color_picker_tool_button);

	cut_tool_button = memnew(Button);
	cut_tool_button->set_text("Cut Section (C)");
	cut_tool_button->set_h_size_flags(SIZE_EXPAND_FILL);
	cut_tool_button->set_v_size_flags(SIZE_EXPAND_FILL);
	tools_contianer->add_child(cut_tool_button);

	color_picker_button = memnew(ColorPickerButton);
	color_picker_button->set_text("Cut Section (C)");
	color_picker_button->set_h_size_flags(SIZE_EXPAND_FILL);
	color_picker_button->set_v_size_flags(SIZE_EXPAND_FILL);
	tools_contianer->add_child(color_picker_button);

	//Main Content Mid (App) -- Right Panel -- Layers
	ScrollContainer *layers_scroll_contianer = memnew(ScrollContainer);
	layers_scroll_contianer->set_h_size_flags(SIZE_EXPAND_FILL);
	layers_scroll_contianer->set_v_size_flags(SIZE_EXPAND_FILL);
	main_tool_contianer->add_child(layers_scroll_contianer);

	VBoxContainer *main_layers_box_container = memnew(VBoxContainer);
	main_layers_box_container->set_h_size_flags(SIZE_EXPAND_FILL);
	main_layers_box_container->set_v_size_flags(SIZE_EXPAND_FILL);
	layers_scroll_contianer->add_child(main_layers_box_container);

	layers_box_container = memnew(VBoxContainer);
	layers_box_container->set_h_size_flags(SIZE_EXPAND_FILL);
	layers_box_container->set_v_size_flags(SIZE_EXPAND_FILL);
	main_layers_box_container->add_child(layers_box_container);

	PaintLayerButton *layer_button = memnew(PaintLayerButton);
	layer_button->set_h_size_flags(SIZE_EXPAND_FILL);
	layers_box_container->add_child(layer_button);

	add_layer_button = memnew(Button);
	add_layer_button->set_text("+");
	add_layer_button->set_custom_minimum_size(Size2(0, 25));
	add_layer_button->set_h_size_flags(SIZE_EXPAND_FILL);
	main_layers_box_container->add_child(add_layer_button);

	//Main Content Bottom (Text display)
	PanelContainer *bottom_content_panel = memnew(PanelContainer);
	bottom_content_panel->set_h_size_flags(SIZE_EXPAND_FILL);
	main_content_container->add_child(bottom_content_panel);

	text_info = memnew(RichTextLabel);
	text_info->set_scroll_active(false);
	bottom_content_panel->add_child(text_info);

	//PaintCanvasDialog
	paint_canvas_dialog = memnew(PaintCanvasDialog);
	add_child(paint_canvas_dialog);

	//PaintChangeGridSizeDialog
	paint_change_grid_size_dialog = memnew(PaintChangeGridSizeDialog);
	add_child(paint_change_grid_size_dialog);

	//PaintLoadFileDialog
	paint_load_file_dialog = memnew(PaintLoadFileDialog);
	add_child(paint_load_file_dialog);

	//PaintSaveFileDialog
	paint_save_file_dialog = memnew(PaintSaveFileDialog);
	add_child(paint_save_file_dialog);

	//PaintSettings
	paint_settings_dialog = memnew(PaintSettings);
	add_child(paint_settings_dialog);
}

PaintWindow::~PaintWindow() {
}

void PaintWindow::_bind_methods() {
}
