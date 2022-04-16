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

#include "paint_canvas.h"
#include "paint_window.h"

#include "scene/gui/label.h"
#include "scene/gui/menu_button.h"
#include "scene/gui/popup_menu.h"

#include "dialogs/paint_canvas_dialog.h"
#include "dialogs/paint_change_grid_size_dialog.h"
#include "dialogs/paint_load_file_dialog.h"
#include "dialogs/paint_save_file_dialog.h"
#include "paint_canvas_layer.h"
#include "paint_settings.h"

void PaintNavbar::handle_menu_item_pressed(const int id) {
	switch (id) {
		case MENU_FILE_NEW:
			paint_window->paint_canvas_dialog->popup_centered();
			break;
		case MENU_FILE_SAVE:
			paint_window->paint_save_file_dialog->popup_centered();
			break;
		case MENU_FILE_LOAD:
			paint_window->paint_load_file_dialog->popup_centered();
			break;
		case MENU_EDIT_UNDO:
			break;
		case MENU_EDIT_REDO:
			break;
		case MENU_EDIT_CUT:
			break;
		case MENU_EDIT_COPY:
			break;
		case MENU_EDIT_PASTE:
			break;
		case MENU_CANVAS_CHANGE_SIZE:
			paint_window->paint_canvas_dialog->popup_centered();
			break;
		case MENU_CANVAS_CROP_TO_CONTENT:
			canvas->crop_to_content();
			break;
		case MENU_LAYER_ADD:
			paint_window->add_new_layer();
			break;
		case MENU_LAYER_DELETE:
			paint_window->remove_active_layer();
			break;
		case MENU_LAYER_DUPLICATE:
			paint_window->duplicate_active_layer();
			break;
		case MENU_LAYER_CLEAR:
			canvas->clear_active_layer();
			break;
		case MENU_LAYER_TOGGLE_ALPHA_LOCKED:

			//owner.paint_canvas.active_layer.toggle_alpha_locked()
			//$Buttons/Layer.get_popup().set_item_checked(id, not $Buttons/Layer.get_popup().is_item_checked(id))
			//owner.find_node("LockAlpha").pressed = $Buttons/Layer.get_popup().is_item_checked(id)

			break;
		case MENU_GRID_TOGGLE:
			canvas->toggle_grid();
			break;
		case MENU_GRID_CHANGE_SIZE:
			paint_window->paint_change_grid_size_dialog->popup_centered();
			break;
		case MENU_MAGIC_CHANGE_SINGLE_COLOR:
			break;
		case MENU_MAGIC_CHANGE_COLOR_RANGE:
			break;
		case MENU_MAGIC_HSV_NOISE:
			break;
		case MENU_MAGIC_HSV_COLOR_MODULATION:
			break;
		case MENU_EDITOR_SETTINGS:
			paint_window->paint_settings_dialog->popup_centered();
			break;
		default:
			break;
	}
}

bool PaintNavbar::is_any_menu_open() {
	if (file_menu_button->get_popup()->is_visible() || edit_menu_button->get_popup()->is_visible() ||
			canvas_menu_button->get_popup()->is_visible() || layer_menu_button->get_popup()->is_visible() ||
			grid_menu_button->get_popup()->is_visible() || magic_menu_button->get_popup()->is_visible() ||
			editor_menu_button->get_popup()->is_visible()) {
		return true;
	}

	return false;
}

PaintNavbar::PaintNavbar() {
	paint_window = nullptr;
	canvas = nullptr;

	HBoxContainer *button_hbox_container = memnew(HBoxContainer);
	add_child(button_hbox_container);

	file_menu_button = memnew(MenuButton);
	file_menu_button->set_switch_on_hover(true);
	file_menu_button->set_text("File");
	file_menu_button->set_flat(false);
	file_menu_button->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	file_menu_button->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	button_hbox_container->add_child(file_menu_button);

	PopupMenu *menu = file_menu_button->get_popup();
	menu->add_item("New", MENU_FILE_NEW);
	menu->add_item("Save", MENU_FILE_SAVE);
	menu->add_item("Load", MENU_FILE_LOAD);
	menu->connect("id_pressed", this, "handle_menu_item_pressed");

	edit_menu_button = memnew(MenuButton);
	edit_menu_button->set_switch_on_hover(true);
	edit_menu_button->set_text("Edit");
	edit_menu_button->set_disabled(true);
	edit_menu_button->set_flat(false);
	edit_menu_button->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	edit_menu_button->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	button_hbox_container->add_child(edit_menu_button);

	menu = edit_menu_button->get_popup();
	menu->add_item("Undo", MENU_EDIT_UNDO);
	menu->add_item("Redo", MENU_EDIT_REDO);
	menu->add_item("Cut", MENU_EDIT_CUT);
	menu->add_item("Copy", MENU_EDIT_COPY);
	menu->add_item("Paste", MENU_EDIT_PASTE);
	menu->connect("id_pressed", this, "handle_menu_item_pressed");

	canvas_menu_button = memnew(MenuButton);
	canvas_menu_button->set_switch_on_hover(true);
	canvas_menu_button->set_text("Canvas");
	canvas_menu_button->set_flat(false);
	canvas_menu_button->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	canvas_menu_button->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	button_hbox_container->add_child(canvas_menu_button);

	menu = canvas_menu_button->get_popup();
	menu->add_item("Change Size", MENU_CANVAS_CHANGE_SIZE);
	//menu->add_item("Crop To Content", MENU_CANVAS_CROP_TO_CONTENT);
	menu->connect("id_pressed", this, "handle_menu_item_pressed");

	layer_menu_button = memnew(MenuButton);
	layer_menu_button->set_switch_on_hover(true);
	layer_menu_button->set_text("Layer");
	layer_menu_button->set_flat(false);
	layer_menu_button->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	layer_menu_button->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	button_hbox_container->add_child(layer_menu_button);

	menu = layer_menu_button->get_popup();
	menu->add_item("Add Layer", MENU_LAYER_ADD);
	menu->add_item("Delete Layer", MENU_LAYER_DELETE);
	menu->add_item("Duplicate Layer", MENU_LAYER_DUPLICATE);
	menu->add_item("Clear Layer", MENU_LAYER_CLEAR);
	menu->add_separator();
	menu->add_check_item("Toggle Alpha Locked", MENU_LAYER_TOGGLE_ALPHA_LOCKED);
	menu->connect("id_pressed", this, "handle_menu_item_pressed");

	grid_menu_button = memnew(MenuButton);
	grid_menu_button->set_switch_on_hover(true);
	grid_menu_button->set_text("Grid");
	grid_menu_button->set_flat(false);
	grid_menu_button->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	grid_menu_button->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	button_hbox_container->add_child(grid_menu_button);

	menu = grid_menu_button->get_popup();
	menu->add_item("Toggle Grid", MENU_GRID_TOGGLE);
	menu->add_item("Change Grid Size", MENU_GRID_CHANGE_SIZE);
	menu->connect("id_pressed", this, "handle_menu_item_pressed");

	magic_menu_button = memnew(MenuButton);
	magic_menu_button->set_switch_on_hover(true);
	magic_menu_button->set_text("Magic");
	magic_menu_button->set_flat(false);
	magic_menu_button->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	magic_menu_button->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	button_hbox_container->add_child(magic_menu_button);

	menu = magic_menu_button->get_popup();
	menu->add_item("Change Single Color", MENU_MAGIC_CHANGE_SINGLE_COLOR);
	menu->add_item("Change Color Range", MENU_MAGIC_CHANGE_COLOR_RANGE);
	menu->add_item("HSV Noise", MENU_MAGIC_HSV_NOISE);
	menu->add_item("HSV Color Modulation", MENU_MAGIC_HSV_COLOR_MODULATION);
	menu->connect("id_pressed", this, "handle_menu_item_pressed");

	editor_menu_button = memnew(MenuButton);
	editor_menu_button->set_switch_on_hover(true);
	editor_menu_button->set_text("Editor");
	editor_menu_button->set_flat(false);
	editor_menu_button->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	editor_menu_button->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	button_hbox_container->add_child(editor_menu_button);

	menu = editor_menu_button->get_popup();
	menu->add_item("Settings", MENU_EDITOR_SETTINGS);
	menu->connect("id_pressed", this, "handle_menu_item_pressed");

	Control *spacer = memnew(Control);
	spacer->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	add_child(spacer);

	Label *urlabel = memnew(Label);
	urlabel->set_text("Undo (Z)  Redo (Y)");
	urlabel->set_align(Label::ALIGN_CENTER);
	urlabel->set_valign(Label::VALIGN_CENTER);
	add_child(spacer);
}

PaintNavbar::~PaintNavbar() {
}

void PaintNavbar::_bind_methods() {
	ClassDB::bind_method(D_METHOD("handle_menu_item_pressed"), &PaintNavbar::handle_menu_item_pressed);
}
