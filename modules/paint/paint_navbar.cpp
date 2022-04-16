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

#include "paint_navbar.h"

/*
tool
extends MenuButton

var popup = get_popup()
signal item_pressed

func _ready():
	popup.connect("id_pressed", self, "id_pressed")

func id_pressed(id):
	emit_signal("item_pressed", name, popup.get_item_text(id), id)

*/

void PaintNavbar::_ready() {
	/*
	editor = owner
	paint_canvas = editor.find_node("PaintCanvas")

	for i in get_node("Buttons").get_children():
		i.connect("item_pressed", self, "button_pressed")
	*/
}
void PaintNavbar::button_pressed(const String &button_name, Node *button_item, const int id) {
	/*
#	print("pressed: ", button_name)
#	print("pressed item is: '%s'" % button_item)

	match button_name:
		"File":
			handle_file_menu(button_item, id)
		"Edit":
			handle_edit_menu(button_item, id)
		"Canvas":
			handle_canvas_menu(button_item, id)
		"Layer":
			handle_layer_menu(button_item, id)
		"Grid":
			handle_grid_menu(button_item, id)
		"Magic":
			handle_magic_menu(button_item, id)
		"Editor":
			handle_editor_menu(button_item, id)
	*/
}

void PaintNavbar::handle_file_menu(const String &pressed_item, const int id) {
	/*
	match pressed_item:
		"Save":
			owner.get_node("SaveFileDialog").show()
		"Load":
			owner.get_node("LoadFileDialog").show()
		"New":
			owner.get_node("ConfirmationDialog").show()
	*/
}
void PaintNavbar::handle_edit_menu(const String &pressed_item, const int id) {
	/*
	match pressed_item:
		"Add Layer":
			editor.add_new_layer()
	*/
}
void PaintNavbar::handle_canvas_menu(const String &pressed_item, const int id) {
	/*
	match pressed_item:
		"Change Size":
			owner.get_node("ChangeCanvasSize").show()
		"Crop To Content":
			owner.paint_canvas.crop_to_content()
	*/
}
void PaintNavbar::handle_layer_menu(const String &pressed_item, const int id) {
	/*
	match pressed_item:
		"Add Layer":
			editor.add_new_layer()
		"Delete Layer":
			editor.remove_active_layer()
		"Duplicate Layer":
			editor.duplicate_active_layer()
		"Clear Layer":
			owner.paint_canvas.clear_active_layer()
		"Toggle Alpha Locked":
			owner.paint_canvas.active_layer.toggle_alpha_locked()
			$Buttons/Layer.get_popup().set_item_checked(id, not $Buttons/Layer.get_popup().is_item_checked(id))
			owner.find_node("LockAlpha").pressed = $Buttons/Layer.get_popup().is_item_checked(id)
	*/
}
void PaintNavbar::handle_grid_menu(const String &pressed_item, const int id) {
	/*
	match pressed_item:
		"Change Grid Size":
			owner.get_node("ChangeGridSizeDialog").show()
		"Toggle Grid":
			owner.paint_canvas.toggle_grid()
	*/
}

void PaintNavbar::handle_magic_menu(const String &pressed_item, const int id) {
	/*
	match pressed_item:
		"Add Layer":
			editor.add_new_layer()
	*/
}
void PaintNavbar::handle_editor_menu(const String &pressed_item, const int id) {
	/*
	match pressed_item:
		"Settings":
			owner.get_node("Settings").show()
		"Toggle Grid":
			var grids_node = owner.find_node("Grids")
			grids_node.visible = !grids_node.visible
		"Reset Canvas Position":
			owner.paint_canvas_node.rect_position = Vector2(0, 0)
	*/
}
bool PaintNavbar::is_any_menu_open() {
	/*
	for child in $Buttons.get_children():
		if child.get_popup().visible:
			return true
	return false
	*/
}

PaintNavbar::PaintNavbar() {
}

PaintNavbar::~PaintNavbar() {
}

void PaintNavbar::_bind_methods() {
}
