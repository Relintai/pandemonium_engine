#ifndef EDITOR_INSPECTOR_H
#define EDITOR_INSPECTOR_H

/*************************************************************************/
/*  editor_inspector.h                                                   */
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
#include "scene/gui/container.h"
#include "scene/gui/scroll_container.h"
#include "scene/main/control.h"

#include "core/containers/list.h"
#include "core/containers/rb_map.h"
#include "core/math/color.h"
#include "core/math/rect2.h"
#include "core/math/vector2.h"
#include "core/object/object.h"
#include "core/object/object_id.h"

#include "core/containers/rb_set.h"
#include "core/containers/vector.h"
#include "core/object/resource.h"
#include "core/string/string_name.h"
#include "core/string/ustring.h"
#include "core/variant/variant.h"

#include "scene/resources/texture.h"

class UndoRedo;
class Array;
class InputEvent;
class LineEdit;
class Node;
class PopupMenu;
class VBoxContainer;
class Button;
class AcceptDialog;
class MarginContainer;
class HBoxContainer;
class PanelContainer;
class TextureRect;

class EditorPropertyRevert {
public:
	static bool get_instanced_node_original_property(Node *p_node, const StringName &p_prop, Variant &value, bool p_check_class_default = true);
	static bool is_node_property_different(Node *p_node, const Variant &p_current, const Variant &p_orig);
	static bool is_property_value_different(const Variant &p_a, const Variant &p_b);
	static Variant get_property_revert_value(Object *p_object, const StringName &p_property, bool *r_is_valid);

	static bool can_property_revert(Object *p_object, const StringName &p_property);
};

class EditorProperty : public Container {
	GDCLASS(EditorProperty, Container);

public:
	enum MenuItems {
		MENU_PIN_VALUE,
		MENU_COPY_PROPERTY,
		MENU_PASTE_PROPERTY,
		MENU_COPY_PROPERTY_PATH,
	};

private:
	String label;
	int text_size;
	friend class EditorInspector;
	Object *object;
	StringName property;
	String property_path;

	int property_usage;

	bool read_only;
	bool checkable;
	bool checked;
	bool draw_red;
	bool keying;

	Rect2 right_child_rect;
	Rect2 bottom_child_rect;

	Rect2 keying_rect;
	bool keying_hover;
	Rect2 revert_rect;
	bool revert_hover;
	Rect2 check_rect;
	bool check_hover;

	bool can_revert;
	bool can_pin;
	bool pin_hidden;
	bool is_pinned;

	bool use_folding;
	bool draw_top_bg;

	void _update_popup();
	void _menu_option(int p_option);
	void _focusable_focused(int p_index);

	bool selectable;
	bool selected;
	int selected_focusable;

	float split_ratio;

	Vector<Control *> focusables;
	Control *label_reference;
	Control *bottom_editor;
	PopupMenu *menu;

	void _update_pin_flags();

protected:
	void _notification(int p_what);
	static void _bind_methods();

	void _gui_input(const Ref<InputEvent> &p_event);
	void _shortcut_input(const Ref<InputEvent> &p_event);

public:
	void emit_changed(const StringName &p_property, const Variant &p_value, const StringName &p_field = StringName(), bool p_changing = false);

	virtual Size2 get_minimum_size() const;

	void set_label(const String &p_label);
	String get_label() const;

	void set_read_only(bool p_read_only);
	bool is_read_only() const;

	Object *get_edited_object();
	StringName get_edited_property();

	virtual void update_property();
	void update_revert_and_pin_status();

	virtual bool use_keying_next() const;

	void set_checkable(bool p_checkable);
	bool is_checkable() const;

	void set_checked(bool p_checked);
	bool is_checked() const;

	void set_draw_red(bool p_draw_red);
	bool is_draw_red() const;

	void set_keying(bool p_keying);
	bool is_keying() const;

	void add_focusable(Control *p_control);
	void select(int p_focusable = -1);
	void deselect();
	bool is_selected() const;

	void set_label_reference(Control *p_control);
	void set_bottom_editor(Control *p_control);

	void set_use_folding(bool p_use_folding);
	bool is_using_folding() const;

	virtual void expand_all_folding();
	virtual void collapse_all_folding();

	virtual Variant get_drag_data(const Point2 &p_point);

	void set_selectable(bool p_selectable);
	bool is_selectable() const;

	void set_name_split_ratio(float p_ratio);
	float get_name_split_ratio() const;

	void set_object_and_property(Object *p_object, const StringName &p_property);
	virtual Control *make_custom_tooltip(const String &p_text) const;

	void set_draw_top_bg(bool p_draw) { draw_top_bg = p_draw; }

	bool can_revert_to_default() const { return can_revert; }

	EditorProperty();
};

class EditorInspectorPlugin : public Reference {
	GDCLASS(EditorInspectorPlugin, Reference);

	friend class EditorInspector;
	struct AddedEditor {
		Control *property_editor;
		Vector<String> properties;
		String label;
	};

	List<AddedEditor> added_editors;

protected:
	static void _bind_methods();

public:
	void add_custom_control(Control *control);
	void add_property_editor(const String &p_for_property, Control *p_prop);
	void add_property_editor_for_multiple_properties(const String &p_label, const Vector<String> &p_properties, Control *p_prop);

	virtual bool can_handle(Object *p_object);
	virtual void parse_begin(Object *p_object);
	virtual void parse_category(Object *p_object, const String &p_parse_category);
	virtual bool parse_property(Object *p_object, Variant::Type p_type, const String &p_path, PropertyHint p_hint, const String &p_hint_text, int p_usage);
	virtual void parse_end();
};

class EditorInspectorCategory : public Control {
	GDCLASS(EditorInspectorCategory, Control);

	friend class EditorInspector;
	Ref<Texture> icon;
	String label;
	Color bg_color;

protected:
	void _notification(int p_what);

public:
	virtual Size2 get_minimum_size() const;
	virtual Control *make_custom_tooltip(const String &p_text) const;

	EditorInspectorCategory();
};

class EditorInspectorSection : public Container {
	GDCLASS(EditorInspectorSection, Container);

	String label;
	String section;

	bool vbox_added; // Optimization
	Color bg_color;
	bool foldable;

	void _test_unfold();
	int _get_header_height();
	Ref<Texture> _get_arrow();

protected:
	Object *object;
	VBoxContainer *vbox;

	void _notification(int p_what);
	static void _bind_methods();
	void _gui_input(const Ref<InputEvent> &p_event);

public:
	virtual Size2 get_minimum_size() const;

	void setup(const String &p_section, const String &p_label, Object *p_object, const Color &p_bg_color, bool p_foldable);
	VBoxContainer *get_vbox();
	void unfold();
	void fold();

	EditorInspectorSection();
	~EditorInspectorSection();
};

class EditorInspectorArray : public EditorInspectorSection {
	GDCLASS(EditorInspectorArray, EditorInspectorSection);

	UndoRedo *undo_redo;

	enum Mode {
		MODE_NONE,
		MODE_USE_COUNT_PROPERTY,
		MODE_USE_MOVE_ARRAY_ELEMENT_FUNCTION,
	} mode;

	StringName count_property;
	StringName array_element_prefix;
	String swap_method;

	int count = 0;

	VBoxContainer *elements_vbox;

	Control *control_dropping;
	bool dropping = false;

	Button *add_button;

	AcceptDialog *resize_dialog;
	int new_size = 0;
	LineEdit *new_size_line_edit;

	// Pagination
	int page_length = 5;
	int page = 0;
	int max_page = 0;
	int begin_array_index = 0;
	int end_array_index = 0;

	bool movable = true;
	bool numbered = false;

	HBoxContainer *hbox_pagination;
	Button *first_page_button;
	Button *prev_page_button;
	LineEdit *page_line_edit;
	Label *page_count_label;
	Button *next_page_button;
	Button *last_page_button;

	enum MenuOptions {
		OPTION_MOVE_UP = 0,
		OPTION_MOVE_DOWN,
		OPTION_NEW_BEFORE,
		OPTION_NEW_AFTER,
		OPTION_REMOVE,
		OPTION_CLEAR_ARRAY,
		OPTION_RESIZE_ARRAY,
	};

	int popup_array_index_pressed = -1;
	PopupMenu *rmb_popup;

	struct ArrayElement {
		PanelContainer *panel;
		MarginContainer *margin;
		HBoxContainer *hbox;
		TextureRect *move_texture_rect;
		Label *number;
		VBoxContainer *vbox;
		Button *erase;

		ArrayElement() {
			panel = NULL;
			margin = NULL;
			hbox = NULL;
			move_texture_rect = NULL;
			number = NULL;
			vbox = NULL;
			erase = NULL;
		}
	};

	LocalVector<ArrayElement> array_elements;

	Ref<StyleBoxFlat> odd_style;
	Ref<StyleBoxFlat> even_style;

	int _get_array_count();
	void _add_button_pressed();

	void _first_page_button_pressed();
	void _prev_page_button_pressed();
	void _page_line_edit_text_submitted(String p_text);
	void _next_page_button_pressed();
	void _last_page_button_pressed();

	void _rmb_popup_id_pressed(int p_id);

	void _control_dropping_draw();

	void _vbox_visibility_changed();

	void _panel_draw(int p_index);
	void _panel_gui_input(Ref<InputEvent> p_event, int p_index);
	void _move_element(int p_element_index, int p_to_pos);
	void _clear_array();
	void _resize_array(int p_size);
	Array _extract_properties_as_array(const List<PropertyInfo> &p_list);
	int _drop_position() const;

	void _new_size_line_edit_text_changed(String p_text);
	void _new_size_line_edit_text_submitted(String p_text);
	void _resize_dialog_confirmed();

	void _update_elements_visibility();
	void _setup();

	Variant get_drag_data_fw(const Point2 &p_point, Control *p_from);
	void drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from);
	bool can_drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from) const;

	void _remove_item(int p_index);

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	void set_undo_redo(UndoRedo *p_undo_redo);

	void setup_with_move_element_function(Object *p_object, String p_label, const StringName &p_array_element_prefix, int p_page, const Color &p_bg_color, bool p_foldable, bool p_movable = true, bool p_numbered = false, int p_page_length = 5, const String &p_add_item_text = "");
	void setup_with_count_property(Object *p_object, String p_label, const StringName &p_count_property, const StringName &p_array_element_prefix, int p_page, const Color &p_bg_color, bool p_foldable, bool p_movable = true, bool p_numbered = false, int p_page_length = 5, const String &p_add_item_text = "", const String &p_swap_method = "");
	VBoxContainer *get_vbox(int p_index);

	EditorInspectorArray();
};

class EditorInspector : public ScrollContainer {
	GDCLASS(EditorInspector, ScrollContainer);

	UndoRedo *undo_redo;
	enum {
		MAX_PLUGINS = 1024
	};
	static Ref<EditorInspectorPlugin> inspector_plugins[MAX_PLUGINS];
	static int inspector_plugin_count;

	VBoxContainer *main_vbox;

	//map use to cache the instanced editors
	RBMap<StringName, List<EditorProperty *>> editor_property_map;
	List<EditorInspectorSection *> sections;
	RBSet<StringName> pending;

	void _clear();
	Object *object;

	//

	LineEdit *search_box;
	bool show_categories;
	bool hide_script;
	bool use_doc_hints;
	bool capitalize_paths;
	bool use_filter;
	bool autoclear;
	bool use_folding;
	int changing;
	bool update_all_pending;
	bool read_only;
	bool keying;
	bool sub_inspector;

	float refresh_countdown;
	bool update_tree_pending;
	StringName _prop_edited;
	StringName property_selected;
	int property_focusable;
	int update_scroll_request;

	RBMap<StringName, RBMap<StringName, String>> descr_cache;
	RBMap<StringName, String> class_descr_cache;
	RBSet<StringName> restart_request_props;

	RBMap<ObjectID, int> scroll_cache;

	String property_prefix; //used for sectioned inspector
	String object_class;
	Variant property_clipboard;

	void _edit_set(const String &p_name, const Variant &p_value, bool p_refresh_all, const String &p_changed_field);

	void _property_changed(const String &p_path, const Variant &p_value, const String &p_name = "", bool changing = false);
	void _property_changed_update_all(const String &p_path, const Variant &p_value, const String &p_name = "", bool p_changing = false);
	void _multiple_properties_changed(Vector<String> p_paths, Array p_values, bool p_changing = false);
	void _property_keyed(const String &p_path, bool p_advance);
	void _property_keyed_with_value(const String &p_path, const Variant &p_value, bool p_advance);
	void _property_checked(const String &p_path, bool p_checked);
	void _property_pinned(const String &p_path, bool p_pinned);

	void _resource_selected(const String &p_path, RES p_resource);
	void _property_selected(const String &p_path, int p_focusable);
	void _object_id_selected(const String &p_path, ObjectID p_id);

	void _node_removed(Node *p_node);

	RBMap<StringName, int> per_array_page;
	void _page_change_request(int p_new_page, const StringName &p_array_prefix);

	void _changed_callback(Object *p_changed, const char *p_prop);
	void _edit_request_change(Object *p_object, const String &p_prop);

	void _filter_changed(const String &p_text);
	void _parse_added_editors(VBoxContainer *current_vbox, Ref<EditorInspectorPlugin> ped);

	void _vscroll_changed(double);

	void _update_inspector_bg();

	void _update_tree();

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	static void add_inspector_plugin(const Ref<EditorInspectorPlugin> &p_plugin);
	static void remove_inspector_plugin(const Ref<EditorInspectorPlugin> &p_plugin);
	static void cleanup_plugins();

	static EditorProperty *instantiate_property_editor(Object *p_object, Variant::Type p_type, const String &p_path, PropertyHint p_hint, const String &p_hint_text, int p_usage);

	void set_undo_redo(UndoRedo *p_undo_redo);

	String get_selected_path() const;

	void update_tree();
	void update_property(const String &p_prop);

	void refresh();

	void edit(Object *p_object);
	Object *get_edited_object();

	void set_keying(bool p_active);
	void set_read_only(bool p_read_only);

	bool is_capitalize_paths_enabled() const;
	void set_enable_capitalize_paths(bool p_capitalize);
	void set_autoclear(bool p_enable);

	void set_show_categories(bool p_show);
	void set_use_doc_hints(bool p_enable);
	void set_hide_script(bool p_hide);

	void set_use_filter(bool p_use);
	void register_text_enter(Node *p_line_edit);

	void set_use_folding(bool p_enable);
	bool is_using_folding();

	void collapse_all_folding();
	void expand_all_folding();

	void set_scroll_offset(int p_offset);
	int get_scroll_offset() const;

	void set_property_prefix(const String &p_prefix);
	String get_property_prefix() const;

	void set_object_class(const String &p_class);
	String get_object_class() const;

	void set_sub_inspector(bool p_enable);
	bool is_sub_inspector() const { return sub_inspector; }

	void set_property_clipboard(const Variant &p_value);
	Variant get_property_clipboard() const;

	EditorInspector();
};

#endif // INSPECTOR_H
