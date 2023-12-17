#ifndef MM_GRAPH_NODE_H
#define MM_GRAPH_NODE_H

/*************************************************************************/
/*  mm_graph_node.h                                                      */
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

#include "core/containers/vector.h"
#include "core/math/vector2.h"
#include "core/object/reference.h"
#include "core/object/undo_redo.h"
#include "core/string/ustring.h"

#include "scene/gui/graph_node.h"

class MMMaterial;
class MMNode;
class PackedScene;
class MatMakerGDEditor;
class MMNodeUniversalProperty;

class MMGraphNode : public GraphNode {
	GDCLASS(MMGraphNode, GraphNode);

public:
	Ref<PackedScene> get_gradient_editor_scene();
	void set_gradient_editor_scene(const Ref<PackedScene> &val);

	Ref<PackedScene> get_polygon_edit_scene();
	void set_polygon_edit_scene(const Ref<PackedScene> &val);

	Ref<PackedScene> get_curve_edit_scene();
	void set_curve_edit_scene(const Ref<PackedScene> &val);

	Ref<MMMaterial> get_mm_material();
	void set_mm_material(const Ref<MMMaterial> &val);

	Ref<MMNode> get_material_node();

	Ref<MMNode> get_mm_node();
	void set_mm_node(const Ref<MMNode> &val);

	MatMakerGDEditor *get_editor_node();
	void set_editor_node(MatMakerGDEditor *val);

	UndoRedo *get_undo_redo();
	void set_undo_redo(UndoRedo *val);

	bool get_ignore_change_event() const;
	void set_ignore_change_event(const bool val);

	void ignore_changes(const bool val);

	int add_slot_texture(const String &getter, const String &setter);
	int add_slot_texture_universal(const Ref<MMNodeUniversalProperty> &property);
	int add_slot_image_path_universal(const Ref<MMNodeUniversalProperty> &property, const String &getter, const String &setter);
	int add_slot_gradient();
	int add_slot_polygon();
	int add_slot_curve();
	int add_slot_tones();
	int add_slot_color(const String &getter, const String &setter);
	int add_slot_color_universal(const Ref<MMNodeUniversalProperty> &property);
	int add_slot_label(const String &getter, const String &setter, const String &slot_name);
	int add_slot_line_edit(const String &getter, const String &setter, const String &slot_name, const String &placeholder = "");
	int add_slot_enum(const String &getter, const String &setter, const String &slot_name, const Array &values);
	int add_slot_int(const String &getter, const String &setter, const String &slot_name, const Vector2 &prange = Vector2(-1000, 1000));
	int add_slot_bool(const String &getter, const String &setter, const String &slot_name);
	int add_slot_button(const String &method, const String &slot_name);
	int add_slot_label_universal(const Ref<MMNodeUniversalProperty> &property);
	int add_slot_int_universal(const Ref<MMNodeUniversalProperty> &property);
	int add_slot_float(const String &getter, const String &setter, const String &slot_name, const float step = 0.1, const Vector2 &prange = Vector2(-1000, 1000));
	int add_slot_float_universal(const Ref<MMNodeUniversalProperty> &property);
	int add_slot_vector2(const String &getter, const String &setter, const String &slot_name, const float step = 0.1, const Vector2 &prange = Vector2(-1000, 1000));
	int add_slot_vector3(const String &getter, const String &setter, const String &slot_name, const float step = 0.1, const Vector2 &prange = Vector2(-1000, 1000));
	int add_slot_vector2_universal(const Ref<MMNodeUniversalProperty> &property);
	int add_slot(const int input_type, const int output_type, const String &getter, const String &setter, Control *control);

	bool connect_slot(const int slot_idx, Node *to_node, const int to_slot_idx);
	bool disconnect_slot(const int slot_idx, Node *to_node, const int to_slot_idx);

	int get_input_property_graph_node_slot_index(const Variant &property);
	int get_output_property_graph_node_slot_index(const Variant &property);

	Control *get_property_control(const int slot_idx);
	void set_node(const Ref<MMMaterial> &material, const Ref<MMNode> &node);

	void propagate_node_change();

	void on_dragged(const Vector2 &from, const Vector2 &to);
	void on_int_spinbox_value_changed(const float val, const int slot_idx);
	void on_float_spinbox_value_changed(const float val, const int slot_idx);
	void on_vector2_spinbox_value_changed(const float val, const int slot_idx, Node *spinbox_x, Node *spinbox_y);
	void on_vector3_spinbox_value_changed(const float val, const int slot_idx, Node *spinbox_x, Node *spinbox_y, Node *spinbox_z);
	void on_int_universal_spinbox_value_changed(const float val, const int slot_idx);
	void on_float_universal_spinbox_value_changed(const float val, const int slot_idx);
	void on_vector2_universal_spinbox_value_changed(const float val, const int slot_idx, Node *spinbox_x, Node *spinbox_y);
	void on_slot_enum_item_selected(const int val, const int slot_idx);
	void on_universal_texture_changed(const int slot_idx);
	void on_universal_texture_changed_image_picker(const int slot_idx);
	void on_slot_line_edit_text_entered(const String &text, const int slot_idx);
	void on_color_picker_popup_closed(const int slot_idx);
	void on_universal_color_changed(const Color &c, const int slot_idx);
	void on_universal_image_path_changed(const String &f, const int slot_idx);

	void on_close_request();

	Color get_slot_color(const int slot_type);
	Color _get_slot_color(const int slot_type);

	MMGraphNode();
	~MMGraphNode();

protected:
	void _notification(int p_what);

	static void _bind_methods();

	struct MMGraphNodeEntry {
		int slot_idx;
		int input_type;
		int output_type;
		String getter;
		String setter;
		Control *control;
		Ref<MMNodeUniversalProperty> universal_property;
		Ref<Texture> texture;
		String alt_getter;
		String alt_setter;

		MMGraphNodeEntry() {
			slot_idx = 0;
			input_type = 0;
			output_type = 0;
			control = nullptr;
		}
	};

	Ref<PackedScene> gradient_editor_scene; // = preload("res://addons/mat_maker_gd/widgets/gradient_editor/gradient_editor.tscn");
	Ref<PackedScene> polygon_edit_scene; // = preload("res://addons/mat_maker_gd/widgets/polygon_edit/polygon_edit.tscn");
	Ref<PackedScene> curve_edit_scene; // = preload("res://addons/mat_maker_gd/widgets/curve_edit/curve_edit.tscn");
	Ref<MMMaterial> _material;
	Ref<MMNode> _node;
	Vector<MMGraphNodeEntry> properties;
	MatMakerGDEditor *_editor_node;
	UndoRedo *_undo_redo;
	bool _ignore_change_event;
};

#endif
