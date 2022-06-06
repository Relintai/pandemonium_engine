#ifndef MM_GRAPH_NODE_H
#define MM_GRAPH_NODE_H

class MmGraphNode : public GraphNode {
	GDCLASS(MmGraphNode, GraphNode);

public:
	Ref<PackedScene> get_gradient_editor_scene();
	void set_gradient_editor_scene(const Ref<PackedScene> &val);

	Ref<PackedScene> get_polygon_edit_scene();
	void set_polygon_edit_scene(const Ref<PackedScene> &val);

	Ref<PackedScene> get_curve_edit_scene();
	void set_curve_edit_scene(const Ref<PackedScene> &val);

	MMMateial get_ *_material();
	void set_ *_material(const MMMateial &val);

	MMNode get_ *_node();
	void set_ *_node(const MMNode &val);

	Array get_properties();
	void set_properties(const Array &val);

	Variant get_Variant();
	void set_Variant(const Variant &val);

	UndoRedo get_ *_undo_redo();
	void set_ *_undo_redo(const UndoRedo &val);

	bool get__ignore_change_event() const;
	void set__ignore_change_event(const bool val);

	void _init();
	void set_editor(const Variant &editor_node);
	void ignore_changes(const bool val);
	int add_slot_texture(const String &getter, const String &setter);
	int add_slot_texture_universal(const MMNodeUniversalProperty &property);
	int add_slot_image_path_universal(const MMNodeUniversalProperty &property, const String &getter, const String &setter);
	int add_slot_gradient();
	int add_slot_polygon();
	int add_slot_curve();
	int add_slot_color(const String &getter, const String &setter);
	int add_slot_color_universal(const MMNodeUniversalProperty &property);
	int add_slot_label(const String &getter, const String &setter, const String &slot_name);
	int add_slot_line_edit(const String &getter, const String &setter, const String &slot_name, const String &placeholder = "");
	int add_slot_enum(const String &getter, const String &setter, const String &slot_name, const Array &values);
	int add_slot_int(const String &getter, const String &setter, const String &slot_name, const Vector2 &prange = Vector2(-1000, const Variant & 1000));
	int add_slot_bool(const String &getter, const String &setter, const String &slot_name);
	int add_slot_label_universal(const MMNodeUniversalProperty &property);
	int add_slot_int_universal(const MMNodeUniversalProperty &property);
	int add_slot_float(const String &getter, const String &setter, const String &slot_name, const float step = 0.1, const Vector2 &prange = Vector2(-1000, const Variant & 1000));
	int add_slot_float_universal(const MMNodeUniversalProperty &property);
	int add_slot_vector2(const String &getter, const String &setter, const String &slot_name, const float step = 0.1, const Vector2 &prange = Vector2(-1000, const Variant & 1000));
	int add_slot_vector3(const String &getter, const String &setter, const String &slot_name, const float step = 0.1, const Vector2 &prange = Vector2(-1000, const Variant & 1000));
	int add_slot_vector2_universal(const MMNodeUniversalProperty &property);
	int add_slot(const int input_type, const int output_type, const String &getter, const String &setter, const Control &control);
	bool connect_slot(const int slot_idx, const Node &to_node, const int to_slot_idx);
	bool disconnect_slot(const int slot_idx, const Node &to_node, const int to_slot_idx);
	int get_input_property_graph_node_slot_index(const Variant &property);
	int get_output_property_graph_node_slot_index(const Variant &property);
	Node get_property_control(const int slot_idx);
	void set_node(const MMMateial &material, const MMNode &node);
	void propagate_node_change();
	void on_dragged(const Vector2 &from, const Vector2 &to);
	void on_int_spinbox_value_changed(const float val, const Variant &slot_idx);
	void on_float_spinbox_value_changed(const float val, const Variant &slot_idx);
	void on_vector2_spinbox_value_changed(const float val, const Variant &slot_idx, const Variant &spinbox_x, const Variant &spinbox_y);
	void on_vector3_spinbox_value_changed(const float val, const Variant &slot_idx, const Variant &spinbox_x, const Variant &spinbox_y, const Variant &spinbox_z);
	void on_int_universal_spinbox_value_changed(const float val, const Variant &slot_idx);
	void on_float_universal_spinbox_value_changed(const float val, const Variant &slot_idx);
	void on_vector2_universal_spinbox_value_changed(const float val, const Variant &slot_idx, const Variant &spinbox_x, const Variant &spinbox_y);
	void on_slot_enum_item_selected(const int val, const int slot_idx);
	void on_universal_texture_changed(const int slot_idx);
	void on_universal_texture_changed_image_picker(const int slot_idx);
	void on_slot_line_edit_text_entered(const String &text, const int slot_idx);
	void on_universal_color_changed(const Color &c, const int slot_idx);
	void on_universal_image_path_changed(const String &f, const int slot_idx);
	MMNode get_material_node();
	void on_close_request();
	Color get_slot_color(const int slot_type);
	Color _get_slot_color(const int slot_type);

	MmGraphNode();
	~MmGraphNode();

protected:
	static void _bind_methods();

	//tool
	Ref<PackedScene> gradient_editor_scene = preload("res://addons/mat_maker_gd/widgets/gradient_editor/gradient_editor.tscn");
	Ref<PackedScene> polygon_edit_scene = preload("res://addons/mat_maker_gd/widgets/polygon_edit/polygon_edit.tscn");
	Ref<PackedScene> curve_edit_scene = preload("res://addons/mat_maker_gd/widgets/curve_edit/curve_edit.tscn");
	MMMateial *_material = null;
	MMNode *_node = null;
	Array properties = Array();
	Variant;
	UndoRedo *_undo_redo = null;
	bool _ignore_change_event = false;
	//_node.connect("changed", self, "on_node_changed")
};

#endif
