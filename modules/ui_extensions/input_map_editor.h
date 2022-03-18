#ifndef INPUT_MAP_EDITOR_H
#define INPUT_MAP_EDITOR_H
/*************************************************************************/
/*  originally project_settings_editor.h                                 */
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

#include "bs_input_event_key.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/margin_container.h"
#include "scene/gui/menu_button.h"
#include "scene/gui/option_button.h"
#include "scene/gui/panel_container.h"
#include "scene/gui/popup_menu.h"
#include "scene/gui/tree.h"

class InputMapEditor : public MarginContainer {
	GDCLASS(InputMapEditor, MarginContainer);

	enum InputType {
		INPUT_KEY,
		INPUT_JOY_BUTTON,
		INPUT_JOY_MOTION,
		INPUT_MOUSE_BUTTON
	};

	enum LocaleFilter {
		SHOW_ALL_LOCALES,
		SHOW_ONLY_SELECTED_LOCALES,
	};

	Timer *timer;
	InputType add_type;
	String add_at;
	int edit_idx;

	AcceptDialog *message;
	LineEdit *category;
	LineEdit *property;
	OptionButton *type;
	PopupMenu *popup_add;
	ConfirmationDialog *press_a_key;
	Label *press_a_key_label;
	ConfirmationDialog *device_input;
	OptionButton *device_id;
	OptionButton *device_index;
	Label *device_index_label;

	Tree *input_editor;
	bool setting;

	Ref<InputEventKey> last_wait_for_key;

	Ref<Texture> _add_texture;
	Ref<Texture> _remove_texture;
	Ref<Texture> _edit_texture;
	Ref<Texture> _keyboard_texture;
	Ref<Texture> _joybutton_texture;
	Ref<Texture> _joyaxis_texture;
	Ref<Texture> _mouse_texture;

	void _update_actions();
	void _save();
	void _add_item(int p_item, Ref<InputEvent> p_exiting_event = NULL);
	void _edit_item(Ref<InputEvent> p_exiting_event);

	void _device_input_add();

	void _item_checked(const String &p_item, bool p_check);
	void _action_selected();
	void _action_edited();
	void _action_activated();
	void _action_button_pressed(Object *p_obj, int p_column, int p_id);
	void _wait_for_key(const Ref<InputEvent> &p_event);
	void _press_a_key_confirm();
	void _show_last_added(const Ref<InputEvent> &p_event, const String &p_name);

	void _settings_prop_edited(const String &p_name);
	void _settings_changed();

	Variant get_drag_data_fw(const Point2 &p_point, Control *p_from);
	bool can_drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from) const;
	void drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from);

protected:
	void _notification(int p_what);
	static void _bind_methods();

	int _get_current_device();
	void _set_current_device(int i_device);
	String _get_device_string(int i_device);

public:
	Ref<Texture> get_add_texture();
	void set_add_texture(const Ref<Texture> &tex);

	Ref<Texture> get_remove_texture();
	void set_remove_texture(const Ref<Texture> &tex);

	Ref<Texture> get_edit_texture();
	void set_edit_texture(const Ref<Texture> &tex);

	Ref<Texture> get_keyboard_texture();
	void set_keyboard_texture(const Ref<Texture> &tex);

	Ref<Texture> get_joybutton_texture();
	void set_joybutton_texture(const Ref<Texture> &tex);

	Ref<Texture> get_joyaxis_texture();
	void set_joyaxis_texture(const Ref<Texture> &tex);

	Ref<Texture> get_mouse_texture();
	void set_mouse_texture(const Ref<Texture> &tex);

	void queue_save();

	InputMapEditor();
};

#endif // PROJECT_SETTINGS_H
