#ifndef PROPERTY_EDITOR_H
#define PROPERTY_EDITOR_H

/*************************************************************************/
/*  property_editor.h                                                    */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
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

#include "core/object/reference.h"
#include "scene/gui/popup.h"

#include "core/containers/list.h"
#include "core/containers/vector.h"
#include "core/math/math_defs.h"
#include "core/string/node_path.h"
#include "core/string/ustring.h"
#include "core/variant/variant.h"

class PropertyValueEvaluator;
class CreateDialog;
class PropertySelector;
class Button;
class CheckBox;
class ColorPicker;
class ConfirmationDialog;
class Control;
class EditorFileDialog;
class GridContainer;
class HScrollBar;
class HSlider;
class InputEvent;
class Label;
class LineEdit;
class MenuButton;
class PopupMenu;
class Resource;
class SceneTreeDialog;
class SpinBox;
class TextEdit;
class TextureRect;
class EditorLocaleDialog;
struct Color;

class EditorResourceConversionPlugin : public Reference {
	GDCLASS(EditorResourceConversionPlugin, Reference);

protected:
	static void _bind_methods();

public:
	virtual String converts_to() const;
	virtual bool handles(const Ref<Resource> &p_resource) const;
	virtual Ref<Resource> convert(const Ref<Resource> &p_resource) const;
};

class CustomPropertyEditor : public Popup {
	GDCLASS(CustomPropertyEditor, Popup);

	enum {
		MAX_VALUE_EDITORS = 12,
		MAX_ACTION_BUTTONS = 5,
		OBJ_MENU_LOAD = 0,
		OBJ_MENU_EDIT = 1,
		OBJ_MENU_CLEAR = 2,
		OBJ_MENU_MAKE_UNIQUE = 3,
		OBJ_MENU_COPY = 4,
		OBJ_MENU_PASTE = 5,
		OBJ_MENU_NEW_SCRIPT = 6,
		OBJ_MENU_EXTEND_SCRIPT = 7,
		OBJ_MENU_SHOW_IN_FILE_SYSTEM = 8,
		TYPE_BASE_ID = 100,
		CONVERT_BASE_ID = 1000
	};

	enum {
		EASING_LINEAR,
		EASING_EASE_IN,
		EASING_EASE_OUT,
		EASING_ZERO,
		EASING_IN_OUT,
		EASING_OUT_IN
	};

	PopupMenu *menu;
	SceneTreeDialog *scene_tree;
	EditorLocaleDialog *locale;
	EditorFileDialog *file;
	ConfirmationDialog *error;
	String name;
	Variant::Type type;
	Variant v;
	List<String> field_names;
	int hint;
	String hint_text;
	LineEdit *value_editor[MAX_VALUE_EDITORS];
	int focused_value_editor;
	Label *value_label[MAX_VALUE_EDITORS];
	HScrollBar *scroll[4];
	Button *action_buttons[MAX_ACTION_BUTTONS];
	MenuButton *type_button;
	Vector<String> inheritors_array;
	TextureRect *texture_preview;
	ColorPicker *color_picker;
	TextEdit *text_edit;
	bool read_only;
	bool picking_viewport;
	GridContainer *checks20gc;
	CheckBox *checks20[20];
	SpinBox *spinbox;
	HSlider *slider;

	Control *easing_draw;
	CreateDialog *create_dialog;
	PropertySelector *property_select;

	Object *owner;

	bool updating;

	PropertyValueEvaluator *evaluator;

	void _text_edit_changed();
	void _locale_selected(String p_locale);
	void _file_selected(String p_file);
	void _modified(String p_string);

	real_t _parse_real_expression(String text);

	void _range_modified(double p_value);
	void _focus_enter();
	void _focus_exit();
	void _action_pressed(int p_which);
	void _type_create_selected(int p_idx);
	void _create_dialog_callback();
	void _create_selected_property(const String &p_prop);

	void _color_changed(const Color &p_color);
	void _draw_easing();
	void _menu_option(int p_which);

	void _drag_easing(const Ref<InputEvent> &p_ev);

	void _node_path_selected(NodePath p_path);
	void config_value_editors(int p_amount, int p_columns, int p_label_w, const List<String> &p_strings);
	void config_action_buttons(const List<String> &p_strings);

	void _emit_changed_whole_or_field();

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	void hide_menu();

	Variant get_variant() const;
	String get_name() const;

	void set_read_only(bool p_read_only) { read_only = p_read_only; }

	bool edit(Object *p_owner, const String &p_name, Variant::Type p_type, const Variant &p_variant, int p_hint, String p_hint_text);

	CustomPropertyEditor();
};

#endif
