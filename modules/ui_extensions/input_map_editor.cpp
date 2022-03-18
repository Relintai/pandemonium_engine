/*************************************************************************/
/*  originally project_settings_editor.cpp                               */
/*************************************************************************/
/*                This file is (originally) the part of:                 */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2020 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2020 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "input_map_editor.h"

#include "core/global_constants.h"
#include "core/project_settings.h"
#include "core/translation.h"
#include "core/ustring.h"

#include "core/os/keyboard.h"
#include "editor/editor_export.h"
#include "editor/editor_node.h"
#include "editor/editor_scale.h"
#include "scene/gui/margin_container.h"

#include "core/input_map.h"

static const char *_button_names[JOY_BUTTON_MAX] = {
	"DualShock Cross, Xbox A, Nintendo B",
	"DualShock Circle, Xbox B, Nintendo A",
	"DualShock Square, Xbox X, Nintendo Y",
	"DualShock Triangle, Xbox Y, Nintendo X",
	"L, L1",
	"R, R1",
	"L2",
	"R2",
	"L3",
	"R3",
	"Select, DualShock Share, Nintendo -",
	"Start, DualShock Options, Nintendo +",
	"D-Pad Up",
	"D-Pad Down",
	"D-Pad Left",
	"D-Pad Right"
};

static const char *_axis_names[JOY_AXIS_MAX * 2] = {
	" (Left Stick Left)",
	" (Left Stick Right)",
	" (Left Stick Up)",
	" (Left Stick Down)",
	" (Right Stick Left)",
	" (Right Stick Right)",
	" (Right Stick Up)",
	" (Right Stick Down)",
	"", "", "", "",
	"", " (L2)",
	"", " (R2)"
};

void InputMapEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			_update_actions();
			popup_add->add_icon_item(_keyboard_texture, "Key ", INPUT_KEY); //"Key " - because the word 'key' has already been used as a key animation
			popup_add->add_icon_item(_joybutton_texture, "Joy Button", INPUT_JOY_BUTTON);
			popup_add->add_icon_item(_joyaxis_texture, "Joy Axis", INPUT_JOY_MOTION);
			popup_add->add_icon_item(_mouse_texture, "Mouse Button", INPUT_MOUSE_BUTTON);

		} break;
	}
}

static bool _validate_action_name(const String &p_name) {
	const CharType *cstr = p_name.c_str();

	for (int i = 0; cstr[i]; i++)
		if (cstr[i] == '/' || cstr[i] == ':' || cstr[i] == '"' ||
				cstr[i] == '=' || cstr[i] == '\\' || cstr[i] < 32)
			return false;
	return true;
}

void InputMapEditor::_action_selected() {
	TreeItem *ti = input_editor->get_selected();
	if (!ti || !ti->is_editable(0))
		return;

	add_at = "input/" + ti->get_text(0);
	edit_idx = -1;
}

void InputMapEditor::_action_edited() {
	TreeItem *ti = input_editor->get_selected();
	if (!ti)
		return;

	if (input_editor->get_selected_column() == 0) {
		String new_name = ti->get_text(0);
		String old_name = add_at.substr(add_at.find("/") + 1, add_at.length());

		if (new_name == old_name)
			return;

		if (new_name == "" || !_validate_action_name(new_name)) {
			ti->set_text(0, old_name);
			add_at = "input/" + old_name;

			message->set_text("Invalid action name. it cannot be empty nor contain '/', ':', '=', '\\' or '\"'");
			message->popup_centered(Size2(300, 100));
			return;
		}

		String action_prop = "input/" + new_name;

		if (ProjectSettings::get_singleton()->has_setting(action_prop)) {
			ti->set_text(0, old_name);
			add_at = "input/" + old_name;

			message->set_text(vformat("An action with the name '%s' already exists.", new_name));
			message->popup_centered(Size2(300, 100));
			return;
		}

		int order = ProjectSettings::get_singleton()->get_order(add_at);
		Dictionary action = ProjectSettings::get_singleton()->get(add_at);

		setting = true;

		ProjectSettings::get_singleton()->clear(add_at);
		ProjectSettings::get_singleton()->set(action_prop, action);
		ProjectSettings::get_singleton()->set_order(action_prop, order);
		_update_actions();
		_settings_changed();

		setting = false;

		add_at = action_prop;
	} else if (input_editor->get_selected_column() == 1) {
		String name = "input/" + ti->get_text(0);
		Dictionary old_action = ProjectSettings::get_singleton()->get(name);
		Dictionary new_action = old_action.duplicate();
		new_action["deadzone"] = ti->get_range(1);

		ProjectSettings::get_singleton()->set(name, new_action);
		_settings_changed();
	}
}

void InputMapEditor::_device_input_add() {
	Ref<InputEvent> ie;
	String name = add_at;
	int idx = edit_idx;
	Dictionary old_val = ProjectSettings::get_singleton()->get(name);
	Dictionary action = old_val.duplicate();
	Array events = action["events"];

	switch (add_type) {
		case INPUT_MOUSE_BUTTON: {
			Ref<InputEventMouseButton> mb;
			mb.instance();
			mb->set_button_index(device_index->get_selected() + 1);
			mb->set_device(_get_current_device());

			for (int i = 0; i < events.size(); i++) {
				Ref<InputEventMouseButton> aie = events[i];
				if (aie.is_null())
					continue;
				if (aie->get_device() == mb->get_device() && aie->get_button_index() == mb->get_button_index()) {
					return;
				}
			}

			ie = mb;

		} break;
		case INPUT_JOY_MOTION: {
			Ref<InputEventJoypadMotion> jm;
			jm.instance();
			jm->set_axis(device_index->get_selected() >> 1);
			jm->set_axis_value((device_index->get_selected() & 1) ? 1 : -1);
			jm->set_device(_get_current_device());

			for (int i = 0; i < events.size(); i++) {
				Ref<InputEventJoypadMotion> aie = events[i];
				if (aie.is_null())
					continue;

				if (aie->get_device() == jm->get_device() && aie->get_axis() == jm->get_axis() && aie->get_axis_value() == jm->get_axis_value()) {
					return;
				}
			}

			ie = jm;

		} break;
		case INPUT_JOY_BUTTON: {
			Ref<InputEventJoypadButton> jb;
			jb.instance();
			jb->set_button_index(device_index->get_selected());
			jb->set_device(_get_current_device());

			for (int i = 0; i < events.size(); i++) {
				Ref<InputEventJoypadButton> aie = events[i];
				if (aie.is_null())
					continue;
				if (aie->get_device() == jb->get_device() && aie->get_button_index() == jb->get_button_index()) {
					return;
				}
			}
			ie = jb;

		} break;
		default: {
		}
	}

	if (idx < 0 || idx >= events.size()) {
		events.push_back(ie);
	} else {
		events[idx] = ie;
	}
	action["events"] = events;

	ProjectSettings::get_singleton()->set(name, action);
	_update_actions();
	_settings_changed();

	_show_last_added(ie, name);
}

void InputMapEditor::_set_current_device(int i_device) {
	device_id->select(i_device + 1);
}

int InputMapEditor::_get_current_device() {
	return device_id->get_selected() - 1;
}

String InputMapEditor::_get_device_string(int i_device) {
	if (i_device == InputMap::ALL_DEVICES)
		return "All Devices";

	return "Device " + itos(i_device);
}

void InputMapEditor::_press_a_key_confirm() {
	if (last_wait_for_key.is_null())
		return;

	Ref<BSInputEventKey> ie;

	ie.instance();
	ie->set_scancode(last_wait_for_key->get_scancode());
	ie->set_shift(last_wait_for_key->get_shift());
	ie->set_alt(last_wait_for_key->get_alt());
	ie->set_control(last_wait_for_key->get_control());
	ie->set_metakey(last_wait_for_key->get_metakey());

	String name = add_at;
	int idx = edit_idx;

	Dictionary old_val = ProjectSettings::get_singleton()->get(name);
	Dictionary action = old_val.duplicate();
	Array events = action["events"];

	for (int i = 0; i < events.size(); i++) {
		Ref<InputEventKey> aie = events[i];
		if (aie.is_null())
			continue;

		if (aie->get_scancode_with_modifiers() == ie->get_scancode_with_modifiers()) {
			return;
		}
	}

	if (idx < 0 || idx >= events.size()) {
		events.push_back(ie);
	} else {
		events[idx] = ie;
	}
	action["events"] = events;

	ProjectSettings::get_singleton()->set(name, action);
	_update_actions();
	_settings_changed();

	_show_last_added(ie, name);
}

void InputMapEditor::_show_last_added(const Ref<InputEvent> &p_event, const String &p_name) {
	TreeItem *r = input_editor->get_root();

	String name = p_name;
	name.erase(0, 6);
	if (!r)
		return;

	r = r->get_children();

	if (!r)
		return;
	bool found = false;
	while (r) {
		if (r->get_text(0) != name) {
			r = r->get_next();
			continue;
		}

		TreeItem *child = r->get_children();

		while (child) {
			Variant input = child->get_meta("__input");
			if (p_event == input) {
				r->set_collapsed(false);
				child->select(0);
				found = true;
				break;
			}
			child = child->get_next();
		}
		if (found)
			break;
		r = r->get_next();
	}

	if (found)
		input_editor->ensure_cursor_is_visible();
}

void InputMapEditor::_wait_for_key(const Ref<InputEvent> &p_event) {
	Ref<InputEventKey> k = p_event;

	if (k.is_valid() && k->is_pressed() && k->get_scancode() != 0) {
		last_wait_for_key = p_event;

		const String str = keycode_get_string(k->get_scancode_with_modifiers());

		press_a_key_label->set_text(str);
		press_a_key->get_ok()->set_disabled(false);
		press_a_key->accept_event();
	}
}

void InputMapEditor::_add_item(int p_item, Ref<InputEvent> p_exiting_event) {
	add_type = InputType(p_item);

	switch (add_type) {
		case INPUT_KEY: {
			press_a_key_label->set_text(("Press a Key..."));

			press_a_key->get_ok()->set_disabled(true);

			last_wait_for_key = Ref<InputEvent>();
			press_a_key->popup_centered(Size2(250, 80));
			press_a_key->grab_focus();

		} break;
		case INPUT_MOUSE_BUTTON: {
			device_index_label->set_text(("Mouse Button Index:"));
			device_index->clear();
			device_index->add_item(("Left Button"));
			device_index->add_item(("Right Button"));
			device_index->add_item(("Middle Button"));
			device_index->add_item(("Wheel Up Button"));
			device_index->add_item(("Wheel Down Button"));
			device_index->add_item(("Wheel Left Button"));
			device_index->add_item(("Wheel Right Button"));
			device_index->add_item(("X Button 1"));
			device_index->add_item(("X Button 2"));

			device_input->popup_centered_minsize(Size2(350, 95));

			Ref<InputEventMouseButton> mb = p_exiting_event;
			if (mb.is_valid()) {
				device_index->select(mb->get_button_index() - 1);
				_set_current_device(mb->get_device());
				device_input->get_ok()->set_text("Change");

			} else {
				_set_current_device(0);

				device_input->get_ok()->set_text("Add");
			}

		} break;
		case INPUT_JOY_MOTION: {
			device_index_label->set_text("Joypad Axis Index:");
			device_index->clear();

			for (int i = 0; i < JOY_AXIS_MAX * 2; i++) {
				String desc = _axis_names[i];
				device_index->add_item("Axis " + itos(i / 2) + " " + ((i & 1) ? "+" : "-") + desc);
			}

			device_input->popup_centered_minsize(Size2(350, 95));

			Ref<InputEventJoypadMotion> jm = p_exiting_event;
			if (jm.is_valid()) {
				device_index->select(jm->get_axis() * 2 + (jm->get_axis_value() > 0 ? 1 : 0));

				_set_current_device(jm->get_device());

				device_input->get_ok()->set_text("Change");
			} else {
				_set_current_device(0);

				device_input->get_ok()->set_text("Add");
			}

		} break;
		case INPUT_JOY_BUTTON: {
			device_index_label->set_text("Joypad Button Index:");
			device_index->clear();

			for (int i = 0; i < JOY_BUTTON_MAX; i++) {
				device_index->add_item(itos(i) + ": " + String(_button_names[i]));
			}

			device_input->popup_centered_minsize(Size2(350, 95));

			Ref<InputEventJoypadButton> jb = p_exiting_event;
			if (jb.is_valid()) {
				device_index->select(jb->get_button_index());
				_set_current_device(jb->get_device());

				device_input->get_ok()->set_text("Change");
			} else {
				_set_current_device(0);

				device_input->get_ok()->set_text("Add");
			}

		} break;
		default: {
		}
	}
}

void InputMapEditor::_edit_item(Ref<InputEvent> p_exiting_event) {
	InputType ie_type;

	if ((Ref<InputEventKey>(p_exiting_event)).is_valid()) {
		ie_type = INPUT_KEY;

	} else if ((Ref<InputEventJoypadButton>(p_exiting_event)).is_valid()) {
		ie_type = INPUT_JOY_BUTTON;

	} else if ((Ref<InputEventMouseButton>(p_exiting_event)).is_valid()) {
		ie_type = INPUT_MOUSE_BUTTON;

	} else if ((Ref<InputEventJoypadMotion>(p_exiting_event)).is_valid()) {
		ie_type = INPUT_JOY_MOTION;

	} else {
		return;
	}

	_add_item(ie_type, p_exiting_event);
}
void InputMapEditor::_action_activated() {
	TreeItem *ti = input_editor->get_selected();

	if (!ti || ti->get_parent() == input_editor->get_root())
		return;

	String name = "input/" + ti->get_parent()->get_text(0);
	int idx = ti->get_metadata(0);
	Dictionary action = ProjectSettings::get_singleton()->get(name);
	Array events = action["events"];

	ERR_FAIL_INDEX(idx, events.size());
	Ref<InputEvent> event = events[idx];
	if (event.is_null())
		return;

	add_at = name;
	edit_idx = idx;
	_edit_item(event);
}

void InputMapEditor::_action_button_pressed(Object *p_obj, int p_column, int p_id) {
	TreeItem *ti = Object::cast_to<TreeItem>(p_obj);

	ERR_FAIL_COND(!ti);

	if (p_id == 1) {
		// Add action event
		Point2 ofs = input_editor->get_global_position();
		Rect2 ir = input_editor->get_item_rect(ti);
		ir.position.y -= input_editor->get_scroll().y;
		ofs += ir.position + ir.size;
		ofs.x -= 100;
		popup_add->set_position(ofs);
		popup_add->popup();
		add_at = "input/" + ti->get_text(0);
		edit_idx = -1;

	} else if (p_id == 2) {
		// Remove

		if (ti->get_parent() == input_editor->get_root()) {
			// Remove action
			String name = "input/" + ti->get_text(0);
			Dictionary old_val = ProjectSettings::get_singleton()->get(name);
			//int order = ProjectSettings::get_singleton()->get_order(name);

			ProjectSettings::get_singleton()->clear(name);
			_update_actions();
			_settings_changed();

		} else {
			// Remove action event
			String name = "input/" + ti->get_parent()->get_text(0);
			Dictionary old_val = ProjectSettings::get_singleton()->get(name);
			Dictionary action = old_val.duplicate();
			int idx = ti->get_metadata(0);

			Array events = action["events"];
			ERR_FAIL_INDEX(idx, events.size());
			events.remove(idx);
			action["events"] = events;

			ProjectSettings::get_singleton()->set(name, action);
			_update_actions();
			_settings_changed();
		}
	} else if (p_id == 3) {
		// Edit

		if (ti->get_parent() == input_editor->get_root()) {
			// Edit action name
			ti->set_as_cursor(0);
			input_editor->edit_selected();

		} else {
			// Edit action event
			String name = "input/" + ti->get_parent()->get_text(0);
			int idx = ti->get_metadata(0);
			Dictionary action = ProjectSettings::get_singleton()->get(name);

			Array events = action["events"];
			ERR_FAIL_INDEX(idx, events.size());

			Ref<InputEvent> event = events[idx];

			if (event.is_null())
				return;

			ti->set_as_cursor(0);
			add_at = name;
			edit_idx = idx;
			_edit_item(event);
		}
	}
}

void InputMapEditor::_update_actions() {
	if (setting)
		return;

	Map<String, bool> collapsed;

	if (input_editor->get_root() && input_editor->get_root()->get_children()) {
		for (TreeItem *item = input_editor->get_root()->get_children(); item; item = item->get_next()) {
			collapsed[item->get_text(0)] = item->is_collapsed();
		}
	}

	input_editor->clear();
	TreeItem *root = input_editor->create_item();
	input_editor->set_hide_root(true);

	List<PropertyInfo> props;
	ProjectSettings::get_singleton()->get_property_list(&props);

	for (List<PropertyInfo>::Element *E = props.front(); E; E = E->next()) {
		const PropertyInfo &pi = E->get();
		if (!pi.name.begins_with("input/"))
			continue;

		String name = pi.name.get_slice("/", 1);
		if (name == "")
			continue;

		Dictionary action = ProjectSettings::get_singleton()->get(pi.name);
		Array events = action["events"];

		TreeItem *item = input_editor->create_item(root);
		item->set_text(0, name);

		item->set_custom_bg_color(0, get_color("prop_subsection", "Editor"));

		if (collapsed.has(name))
			item->set_collapsed(collapsed[name]);

		item->set_editable(1, true);
		item->set_cell_mode(1, TreeItem::CELL_MODE_RANGE);
		item->set_range_config(1, 0.0, 1.0, 0.01);
		item->set_range(1, action["deadzone"]);

		item->set_custom_bg_color(1, get_color("prop_subsection", "Editor"));

		item->add_button(2, _add_texture, 1, false, "Add Event");

		for (int i = 0; i < events.size(); i++) {
			Ref<InputEvent> event = events[i];
			if (event.is_null())
				continue;

			TreeItem *action2 = input_editor->create_item(item);

			Ref<InputEventKey> k = event;
			if (k.is_valid()) {
				const String str = keycode_get_string(k->get_scancode_with_modifiers());

				action2->set_text(0, str);
				action2->set_icon(0, _keyboard_texture);
			}

			Ref<InputEventJoypadButton> jb = event;

			if (jb.is_valid()) {
				String str = _get_device_string(jb->get_device()) + ", Button " + itos(static_cast<int>(jb->get_button_index()));
				if (jb->get_button_index() >= 0 && jb->get_button_index() < JOY_BUTTON_MAX)
					str += String() + " (" + _button_names[static_cast<int>(jb->get_button_index())] + ").";
				else
					str += ".";

				action2->set_text(0, str);
				action2->set_icon(0, _joybutton_texture);
			}

			Ref<InputEventMouseButton> mb = event;

			if (mb.is_valid()) {
				String str = _get_device_string(mb->get_device()) + ", ";
				switch (mb->get_button_index()) {
					case BUTTON_LEFT:
						str += "Left Button.";
						break;
					case BUTTON_RIGHT:
						str += "Right Button.";
						break;
					case BUTTON_MIDDLE:
						str += "Middle Button.";
						break;
					case BUTTON_WHEEL_UP:
						str += "Wheel Up.";
						break;
					case BUTTON_WHEEL_DOWN:
						str += "Wheel Down.";
						break;
					default:
						str += "Button " + itos(mb->get_button_index()) + ".";
				}

				action2->set_text(0, str);
				action2->set_icon(0, _mouse_texture);
			}

			Ref<InputEventJoypadMotion> jm = event;

			if (jm.is_valid()) {
				int ax = static_cast<int>(jm->get_axis());
				int n = 2 * ax + (jm->get_axis_value() < 0 ? 0 : 1);
				String desc = _axis_names[n];
				String str = _get_device_string(jm->get_device()) + ", " + "Axis" + " " + itos(ax) + " " + (jm->get_axis_value() < 0 ? "-" : "+") + desc + ".";
				action2->set_text(0, str);
				action2->set_icon(0, _joyaxis_texture);
			}
			action2->set_metadata(0, i);
			action2->set_meta("__input", event);

			action2->add_button(2, _edit_texture, 3, false, "Edit");
			action2->add_button(2, _remove_texture, 2, false, "Remove");
		}
	}

	//_action_check(action_name->get_text());
}

void InputMapEditor::_item_checked(const String &p_item, bool p_check) {
}

void InputMapEditor::_save() {
	Error err = ProjectSettings::get_singleton()->save();
	message->set_text(err != OK ? "Error saving settings." : "Settings saved OK.");
	message->popup_centered(Size2(300, 100));
}

void InputMapEditor::_settings_prop_edited(const String &p_name) {
	// Method needed to discard the mandatory argument of the property_edited signal
	_settings_changed();
}

void InputMapEditor::_settings_changed() {
	timer->start();
}

Ref<Texture> InputMapEditor::get_add_texture() {
	return _add_texture;
}
void InputMapEditor::set_add_texture(const Ref<Texture> &tex) {
	_add_texture = tex;
}

Ref<Texture> InputMapEditor::get_remove_texture() {
	return _remove_texture;
}
void InputMapEditor::set_remove_texture(const Ref<Texture> &tex) {
	_remove_texture = tex;
}

Ref<Texture> InputMapEditor::get_edit_texture() {
	return _edit_texture;
}
void InputMapEditor::set_edit_texture(const Ref<Texture> &tex) {
	_edit_texture = tex;
}

Ref<Texture> InputMapEditor::get_keyboard_texture() {
	return _keyboard_texture;
}
void InputMapEditor::set_keyboard_texture(const Ref<Texture> &tex) {
	_keyboard_texture = tex;
}

Ref<Texture> InputMapEditor::get_joybutton_texture() {
	return _joybutton_texture;
}
void InputMapEditor::set_joybutton_texture(const Ref<Texture> &tex) {
	_joybutton_texture = tex;
}

Ref<Texture> InputMapEditor::get_joyaxis_texture() {
	return _joyaxis_texture;
}
void InputMapEditor::set_joyaxis_texture(const Ref<Texture> &tex) {
	_joyaxis_texture = tex;
}

Ref<Texture> InputMapEditor::get_mouse_texture() {
	return _mouse_texture;
}
void InputMapEditor::set_mouse_texture(const Ref<Texture> &tex) {
	_mouse_texture = tex;
}

void InputMapEditor::queue_save() {
	_settings_changed();
}

Variant InputMapEditor::get_drag_data_fw(const Point2 &p_point, Control *p_from) {
	TreeItem *selected = input_editor->get_selected();
	if (!selected || selected->get_parent() != input_editor->get_root())
		return Variant();

	String name = selected->get_text(0);
	VBoxContainer *vb = memnew(VBoxContainer);
	HBoxContainer *hb = memnew(HBoxContainer);
	Label *label = memnew(Label(name));
	hb->set_modulate(Color(1, 1, 1, 1.0f));
	hb->add_child(label);
	vb->add_child(hb);
	set_drag_preview(vb);

	Dictionary drag_data;
	drag_data["type"] = "nodes";

	input_editor->set_drop_mode_flags(Tree::DROP_MODE_INBETWEEN);

	return drag_data;
}

bool InputMapEditor::can_drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from) const {
	Dictionary d = p_data;
	if (!d.has("type") || d["type"] != "nodes")
		return false;

	TreeItem *selected = input_editor->get_selected();
	TreeItem *item = input_editor->get_item_at_position(p_point);
	if (!selected || !item || item == selected || item->get_parent() == selected)
		return false;

	return true;
}

void InputMapEditor::drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from) {
	if (!can_drop_data_fw(p_point, p_data, p_from))
		return;

	TreeItem *selected = input_editor->get_selected();
	TreeItem *item = input_editor->get_item_at_position(p_point);
	if (!item)
		return;
	TreeItem *target = item->get_parent() == input_editor->get_root() ? item : item->get_parent();

	String selected_name = "input/" + selected->get_text(0);
	int old_order = ProjectSettings::get_singleton()->get_order(selected_name);
	String target_name = "input/" + target->get_text(0);
	int target_order = ProjectSettings::get_singleton()->get_order(target_name);

	int order = old_order;
	bool is_below = target_order > old_order;
	TreeItem *iterator = is_below ? selected->get_next() : selected->get_prev();

	//undo_redo->create_action("Moved Input Action Event");
	while (iterator != target) {
		String iterator_name = "input/" + iterator->get_text(0);
		int iterator_order = ProjectSettings::get_singleton()->get_order(iterator_name);
		//undo_redo->add_do_method(ProjectSettings::get_singleton(), "set_order", iterator_name, order);
		//undo_redo->add_undo_method(ProjectSettings::get_singleton(), "set_order", iterator_name, iterator_order);

		ProjectSettings::get_singleton()->set_order(iterator_name, order);

		order = iterator_order;
		iterator = is_below ? iterator->get_next() : iterator->get_prev();
	}

	ProjectSettings::get_singleton()->set_order(target_name, order);
	ProjectSettings::get_singleton()->set_order(selected_name, target_order);

	_update_actions();
	_settings_changed();
}

void InputMapEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_item_checked"), &InputMapEditor::_item_checked);
	ClassDB::bind_method(D_METHOD("_save"), &InputMapEditor::_save);
	ClassDB::bind_method(D_METHOD("_action_selected"), &InputMapEditor::_action_selected);
	ClassDB::bind_method(D_METHOD("_action_edited"), &InputMapEditor::_action_edited);
	ClassDB::bind_method(D_METHOD("_action_activated"), &InputMapEditor::_action_activated);
	ClassDB::bind_method(D_METHOD("_action_button_pressed"), &InputMapEditor::_action_button_pressed);
	ClassDB::bind_method(D_METHOD("_update_actions"), &InputMapEditor::_update_actions);
	ClassDB::bind_method(D_METHOD("_wait_for_key"), &InputMapEditor::_wait_for_key);
	ClassDB::bind_method(D_METHOD("_add_item"), &InputMapEditor::_add_item, DEFVAL(Variant()));
	ClassDB::bind_method(D_METHOD("_device_input_add"), &InputMapEditor::_device_input_add);
	ClassDB::bind_method(D_METHOD("_press_a_key_confirm"), &InputMapEditor::_press_a_key_confirm);
	ClassDB::bind_method(D_METHOD("_settings_prop_edited"), &InputMapEditor::_settings_prop_edited);
	ClassDB::bind_method(D_METHOD("_settings_changed"), &InputMapEditor::_settings_changed);

	ClassDB::bind_method(D_METHOD("get_drag_data_fw"), &InputMapEditor::get_drag_data_fw);
	ClassDB::bind_method(D_METHOD("can_drop_data_fw"), &InputMapEditor::can_drop_data_fw);
	ClassDB::bind_method(D_METHOD("drop_data_fw"), &InputMapEditor::drop_data_fw);

	ClassDB::bind_method(D_METHOD("queue_save"), &InputMapEditor::queue_save);

	ClassDB::bind_method(D_METHOD("get_add_texture"), &InputMapEditor::get_add_texture);
	ClassDB::bind_method(D_METHOD("set_add_texture", "tex"), &InputMapEditor::set_add_texture);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "add_texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_add_texture", "get_add_texture");

	ClassDB::bind_method(D_METHOD("get_remove_texture"), &InputMapEditor::get_remove_texture);
	ClassDB::bind_method(D_METHOD("set_remove_texture", "tex"), &InputMapEditor::set_remove_texture);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "remove_texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_remove_texture", "get_remove_texture");

	ClassDB::bind_method(D_METHOD("get_edit_texture"), &InputMapEditor::get_edit_texture);
	ClassDB::bind_method(D_METHOD("set_edit_texture", "tex"), &InputMapEditor::set_edit_texture);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "edit_texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_edit_texture", "get_edit_texture");

	ClassDB::bind_method(D_METHOD("get_keyboard_texture"), &InputMapEditor::get_keyboard_texture);
	ClassDB::bind_method(D_METHOD("set_keyboard_texture", "tex"), &InputMapEditor::set_keyboard_texture);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "keyboard_texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_keyboard_texture", "get_keyboard_texture");

	ClassDB::bind_method(D_METHOD("get_joybutton_texture"), &InputMapEditor::get_joybutton_texture);
	ClassDB::bind_method(D_METHOD("set_joybutton_texture", "tex"), &InputMapEditor::set_joybutton_texture);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "joybutton_texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_joybutton_texture", "get_joybutton_texture");

	ClassDB::bind_method(D_METHOD("get_joyaxis_texture"), &InputMapEditor::get_joyaxis_texture);
	ClassDB::bind_method(D_METHOD("set_joyaxis_texture", "tex"), &InputMapEditor::set_joyaxis_texture);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "joyaxis_texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_joyaxis_texture", "get_joyaxis_texture");

	ClassDB::bind_method(D_METHOD("get_mouse_texture"), &InputMapEditor::get_mouse_texture);
	ClassDB::bind_method(D_METHOD("set_mouse_texture", "tex"), &InputMapEditor::set_mouse_texture);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "mouse_texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_mouse_texture", "get_mouse_texture");
}

InputMapEditor::InputMapEditor() {
	VBoxContainer *vbc = memnew(VBoxContainer);
	add_child(vbc);
	vbc->set_v_size_flags(SIZE_EXPAND_FILL);
	vbc->set_h_size_flags(SIZE_EXPAND_FILL);

	input_editor = memnew(Tree);
	vbc->add_child(input_editor);
	input_editor->set_v_size_flags(SIZE_EXPAND_FILL);
	input_editor->set_h_size_flags(SIZE_EXPAND_FILL);
	input_editor->set_columns(3);
	input_editor->set_column_titles_visible(true);
	input_editor->set_column_title(0, "Action");
	input_editor->set_column_title(1, "Deadzone");
	input_editor->set_column_expand(1, false);
	input_editor->set_column_min_width(1, 80);

	input_editor->set_column_expand(2, false);
	input_editor->set_column_min_width(2, 50);

	input_editor->connect("item_edited", this, "_action_edited");
	input_editor->connect("item_activated", this, "_action_activated");
	input_editor->connect("cell_selected", this, "_action_selected");
	input_editor->connect("button_pressed", this, "_action_button_pressed");
	input_editor->set_drag_forwarding(this);

	popup_add = memnew(PopupMenu);
	add_child(popup_add);
	popup_add->connect("id_pressed", this, "_add_item");

	press_a_key = memnew(ConfirmationDialog);
	press_a_key->set_focus_mode(FOCUS_ALL);

	add_child(press_a_key);

	Label *l = memnew(Label);
	l->set_text("Press a Key...");

	l->set_align(Label::ALIGN_CENTER);

	l->set_anchors_and_margins_preset(Control::PRESET_WIDE);
	l->set_margin(MARGIN_TOP, 20);
	l->set_anchor_and_margin(MARGIN_BOTTOM, ANCHOR_BEGIN, 30);
	press_a_key->get_ok()->set_disabled(true);

	press_a_key_label = l;
	press_a_key->add_child(l);
	press_a_key->connect("gui_input", this, "_wait_for_key");
	press_a_key->connect("confirmed", this, "_press_a_key_confirm");

	device_input = memnew(ConfirmationDialog);
	add_child(device_input);

	device_input->get_ok()->set_text("Add");

	device_input->connect("confirmed", this, "_device_input_add");

	HBoxContainer *hbc = memnew(HBoxContainer);
	device_input->add_child(hbc);

	VBoxContainer *vbc_left = memnew(VBoxContainer);
	hbc->add_child(vbc_left);

	l = memnew(Label);
	l->set_text("Device:");
	vbc_left->add_child(l);

	device_id = memnew(OptionButton);
	for (int i = -1; i < 8; i++)
		device_id->add_item(_get_device_string(i));
	_set_current_device(0);
	vbc_left->add_child(device_id);

	VBoxContainer *vbc_right = memnew(VBoxContainer);
	hbc->add_child(vbc_right);
	vbc_right->set_h_size_flags(SIZE_EXPAND_FILL);

	l = memnew(Label);
	l->set_text("Index:");
	vbc_right->add_child(l);
	device_index_label = l;

	device_index = memnew(OptionButton);
	device_index->set_clip_text(true);

	vbc_right->add_child(device_index);

	setting = false;

	timer = memnew(Timer);
	timer->set_wait_time(1.5);
	timer->connect("timeout", ProjectSettings::get_singleton(), "save");
	timer->set_one_shot(true);
	add_child(timer);
}
