
#include "mm_graph_node.h"

Ref<PackedScene> MmGraphNode::get_gradient_editor_scene() {
	return gradient_editor_scene;
}

void MmGraphNode::set_gradient_editor_scene(const Ref<PackedScene> &val) {
	gradient_editor_scene = val;
}

Ref<PackedScene> MmGraphNode::get_polygon_edit_scene() {
	return polygon_edit_scene;
}

void MmGraphNode::set_polygon_edit_scene(const Ref<PackedScene> &val) {
	polygon_edit_scene = val;
}

Ref<PackedScene> MmGraphNode::get_curve_edit_scene() {
	return curve_edit_scene;
}

void MmGraphNode::set_curve_edit_scene(const Ref<PackedScene> &val) {
	curve_edit_scene = val;
}

MMMateial MmGraphNode::get_ *_material() {
	return *_material;
}

void MmGraphNode::set_ *_material(const MMMateial &val) {
	*_material = val;
}

MMNode MmGraphNode::get_ *_node() {
	return *_node;
}

void MmGraphNode::set_ *_node(const MMNode &val) {
	*_node = val;
}

Array MmGraphNode::get_properties() {
	return properties;
}

void MmGraphNode::set_properties(const Array &val) {
	properties = val;
}

Variant MmGraphNode::get_Variant() {
	return Variant;
}

void MmGraphNode::set_Variant(const Variant &val) {
	Variant = val;
}

UndoRedo MmGraphNode::get_ *_undo_redo() {
	return *_undo_redo;
}

void MmGraphNode::set_ *_undo_redo(const UndoRedo &val) {
	*_undo_redo = val;
}

bool MmGraphNode::get__ignore_change_event() const {
	return _ignore_change_event;
}

void MmGraphNode::set__ignore_change_event(const bool val) {
	_ignore_change_event = val;
}

//tool;
//var gradient_editor_scene : PackedScene = preload("res://addons/mat_maker_gd/widgets/gradient_editor/gradient_editor.tscn");
//var polygon_edit_scene : PackedScene = preload("res://addons/mat_maker_gd/widgets/polygon_edit/polygon_edit.tscn");
//var curve_edit_scene : PackedScene = preload("res://addons/mat_maker_gd/widgets/curve_edit/curve_edit.tscn");
MMMateial *_material = null;
MMNode *_node = null;
Array properties = Array();
Variant;
UndoRedo *_undo_redo = null;
bool _ignore_change_event = false;

void MmGraphNode::_init() {
	show_close = true;
	connect("dragged", self, "on_dragged");
	connect("close_request", self, "on_close_request");
}

void MmGraphNode::set_editor(const Variant &editor_node) {
	_editor_node = editor_node;
	_undo_redo = _editor_node.get_undo_redo();
}

void MmGraphNode::ignore_changes(const bool val) {
	_ignore_change_event = val;
	_editor_node.ignore_changes(val);
}

int MmGraphNode::add_slot_texture(const String &getter, const String &setter) {
	TextureRect *t = TextureRect.new();
	t.rect_min_size = Vector2(128, 128);
	t.expand = true;
	t.stretch_mode = TextureRect.STRETCH_KEEP_ASPECT_CENTERED;
	int slot_idx = add_slot(MMNodeUniversalProperty.SlotTypes.SLOT_TYPE_NONE, MMNodeUniversalProperty.SlotTypes.SLOT_TYPE_NONE, getter, setter, t);
	t.texture = _node.call(getter, _material, slot_idx);
	properties[slot_idx].append(t.texture);
	return slot_idx;
}

int MmGraphNode::add_slot_texture_universal(const MMNodeUniversalProperty &property) {
	TextureRect *t = TextureRect.new();
	t.rect_min_size = Vector2(128, 128);
	t.expand = true;
	t.stretch_mode = TextureRect.STRETCH_KEEP_ASPECT_CENTERED;
	int slot_idx = add_slot(property.input_slot_type, property.output_slot_type, "", "", t);
	Ref<Image> img = property.get_active_image();
	Ref<ImageTexture> tex = ImageTexture.new();

	if (img) {
		tex.create_from_image(img, 0);
	}

	t.texture = tex;
	properties[slot_idx].append(property);
	property.connect("changed", self, "on_universal_texture_changed", [slot_idx]);
	return slot_idx;
}

int MmGraphNode::add_slot_image_path_universal(const MMNodeUniversalProperty &property, const String &getter, const String &setter) {
	TextureButton *t = load("res://addons/mat_maker_gd/widgets/image_picker_button/image_picker_button.tscn").instance();
	int slot_idx = add_slot(property.input_slot_type, property.output_slot_type, "", "", t);
	properties[slot_idx].append(property);
	properties[slot_idx].append(getter);
	properties[slot_idx].append(setter);
	property.connect("changed", self, "on_universal_texture_changed_image_picker", [slot_idx]);
	t.connect("on_file_selected", self, "on_universal_image_path_changed", [slot_idx]);
	t.call_deferred("do_set_image_path", _node.call(getter));
	return slot_idx;
}

int MmGraphNode::add_slot_gradient() {
	Control *ge = gradient_editor_scene.instance();
	ge.graph_node = self;
	ge.set_undo_redo(_undo_redo);
	int slot_idx = add_slot(MMNodeUniversalProperty.SlotTypes.SLOT_TYPE_NONE, MMNodeUniversalProperty.SlotTypes.SLOT_TYPE_NONE, "", "", ge);
	ge.set_value(_node);
	//ge.texture = _node.call(getter, _material, slot_idx);
	//properties[slot_idx].append(ge.texture);
	return slot_idx;
}

int MmGraphNode::add_slot_polygon() {
	Control *ge = polygon_edit_scene.instance();
	int slot_idx = add_slot(MMNodeUniversalProperty.SlotTypes.SLOT_TYPE_NONE, MMNodeUniversalProperty.SlotTypes.SLOT_TYPE_NONE, "", "", ge);
	ge.set_value(_node);
	//ge.texture = _node.call(getter, _material, slot_idx);
	//properties[slot_idx].append(ge.texture);
	return slot_idx;
}

int MmGraphNode::add_slot_curve() {
	Control *ge = curve_edit_scene.instance();
	int slot_idx = add_slot(MMNodeUniversalProperty.SlotTypes.SLOT_TYPE_NONE, MMNodeUniversalProperty.SlotTypes.SLOT_TYPE_NONE, "", "", ge);
	ge.set_value(_node);
	//ge.texture = _node.call(getter, _material, slot_idx);
	//properties[slot_idx].append(ge.texture);
	return slot_idx;
}

int MmGraphNode::add_slot_color(const String &getter, const String &setter) {
	ColorPickerButton *cp = ColorPickerButton.new();
	int slot_idx = add_slot(MMNodeUniversalProperty.SlotTypes.SLOT_TYPE_NONE, MMNodeUniversalProperty.SlotTypes.SLOT_TYPE_NONE, getter, setter, cp);
	cp.color = _node.call(getter);
	cp.connect("color_changed", _node, setter);
	return slot_idx;
}

int MmGraphNode::add_slot_color_universal(const MMNodeUniversalProperty &property) {
	ColorPickerButton *cp = ColorPickerButton.new();
	int slot_idx = add_slot(property.input_slot_type, property.output_slot_type, "", "", cp);
	cp.color = property.get_default_value();
	properties[slot_idx].append(property);
	cp.connect("color_changed", self, "on_universal_color_changed", [slot_idx]);
	return slot_idx;
}

int MmGraphNode::add_slot_label(const String &getter, const String &setter, const String &slot_name) {
	Label *l = Label.new();
	l.text = slot_name;
	return add_slot(MMNodeUniversalProperty.SlotTypes.SLOT_TYPE_NONE, MMNodeUniversalProperty.SlotTypes.SLOT_TYPE_NONE, getter, setter, l);
}

int MmGraphNode::add_slot_line_edit(const String &getter, const String &setter, const String &slot_name, const String &placeholder) {
	VBoxContainer *bc = VBoxContainer.new();
	Label *l = Label.new();
	l.text = slot_name;
	bc.add_child(l);
	LineEdit *le = LineEdit.new();
	le.placeholder_text = placeholder;
	bc.add_child(le);
	int slot_idx = add_slot(MMNodeUniversalProperty.SlotTypes.SLOT_TYPE_NONE, MMNodeUniversalProperty.SlotTypes.SLOT_TYPE_NONE, getter, setter, bc);
	le.text = _node.call(getter);
	le.connect("text_entered", self, "on_slot_line_edit_text_entered", [slot_idx]);
	return slot_idx;
}

int MmGraphNode::add_slot_enum(const String &getter, const String &setter, const String &slot_name, const Array &values) {
	VBoxContainer *bc = VBoxContainer.new();

	if (slot_name) {
		Label *l = Label.new();
		l.text = slot_name;
		bc.add_child(l);
	}

	OptionButton *mb = OptionButton.new();

	for (v in values) {
		mb.add_item(v);
	}

	bc.add_child(mb);
	int slot_idx = add_slot(MMNodeUniversalProperty.SlotTypes.SLOT_TYPE_NONE, MMNodeUniversalProperty.SlotTypes.SLOT_TYPE_NONE, getter, setter, bc);
	mb.selected = _node.call(getter);
	mb.connect("item_selected", self, "on_slot_enum_item_selected", [slot_idx]);
	return slot_idx;
}

 int MmGraphNode::add_slot_int(const String &getter, const String &setter, const String &slot_name, const Vector2 &prange, const Variant & 1000)) {
	 VBoxContainer *bc = VBoxContainer.new();
	 Label *l = Label.new();
	 l.text = slot_name;
	 bc.add_child(l);
	 SpinBox *sb = SpinBox.new();
	 sb.rounded = true;
	 sb.min_value = prange.x;
	 sb.max_value = prange.y;
	 bc.add_child(sb);
	 int slot_idx = add_slot(MMNodeUniversalProperty.SlotTypes.SLOT_TYPE_NONE, MMNodeUniversalProperty.SlotTypes.SLOT_TYPE_NONE, getter, setter, bc);
	 sb.value = _node.call(getter);
	 sb.connect("value_changed", self, "on_int_spinbox_value_changed", [slot_idx]);
	 return slot_idx;
 }

 int MmGraphNode::add_slot_bool(const String &getter, const String &setter, const String &slot_name) {
	 CheckBox *cb = CheckBox.new();
	 cb.text = slot_name;
	 int slot_idx = add_slot(MMNodeUniversalProperty.SlotTypes.SLOT_TYPE_NONE, MMNodeUniversalProperty.SlotTypes.SLOT_TYPE_NONE, getter, setter, cb);
	 cb.pressed = _node.call(getter);
	 cb.connect("toggled", _node, setter);
	 return slot_idx;
 }

 int MmGraphNode::add_slot_label_universal(const MMNodeUniversalProperty &property) {
	 Label *l = Label.new();
	 l.text = property.slot_name;
	 int slot_idx = add_slot(property.input_slot_type, property.output_slot_type, "", "", l);
	 properties[slot_idx].append(property);
	 return slot_idx;
 }

 int MmGraphNode::add_slot_int_universal(const MMNodeUniversalProperty &property) {
	 VBoxContainer *bc = VBoxContainer.new();
	 Label *l = Label.new();
	 l.text = property.slot_name;
	 bc.add_child(l);
	 SpinBox *sb = SpinBox.new();
	 sb.rounded = true;
	 sb.min_value = property.value_range.x;
	 sb.max_value = property.value_range.y;
	 bc.add_child(sb);
	 int slot_idx = add_slot(property.input_slot_type, property.output_slot_type, "", "", bc);
	 sb.value = property.get_default_value();
	 sb.connect("value_changed", self, "on_int_universal_spinbox_value_changed", [slot_idx]);
	 properties[slot_idx].append(property);
	 return slot_idx;
 }

 int MmGraphNode::add_slot_float(const String &getter, const String &setter, const String &slot_name, const float step, const Vector2 &prange, const Variant & 1000)) {
	 VBoxContainer *bc = VBoxContainer.new();
	 Label *l = Label.new();
	 l.text = slot_name;
	 bc.add_child(l);
	 SpinBox *sb = SpinBox.new();
	 bc.add_child(sb);
	 int slot_idx = add_slot(MMNodeUniversalProperty.SlotTypes.SLOT_TYPE_NONE, MMNodeUniversalProperty.SlotTypes.SLOT_TYPE_NONE, getter, setter, bc);
	 sb.rounded = false;
	 sb.step = step;
	 sb.min_value = prange.x;
	 sb.max_value = prange.y;
	 sb.value = _node.call(getter);
	 sb.connect("value_changed", self, "on_float_spinbox_value_changed", [slot_idx]);
	 return slot_idx;
 }

 int MmGraphNode::add_slot_float_universal(const MMNodeUniversalProperty &property) {
	 VBoxContainer *bc = VBoxContainer.new();
	 Label *l = Label.new();
	 l.text = property.slot_name;
	 bc.add_child(l);
	 SpinBox *sb = SpinBox.new();
	 bc.add_child(sb);
	 int slot_idx = add_slot(property.input_slot_type, property.output_slot_type, "", "", bc);
	 sb.rounded = false;
	 sb.step = property.value_step;
	 sb.min_value = property.value_range.x;
	 sb.max_value = property.value_range.y;
	 sb.value = property.get_default_value();
	 properties[slot_idx].append(property);
	 sb.connect("value_changed", self, "on_float_universal_spinbox_value_changed", [slot_idx]);
	 return slot_idx;
 }

 int MmGraphNode::add_slot_vector2(const String &getter, const String &setter, const String &slot_name, const float step, const Vector2 &prange, const Variant & 1000)) {
	 VBoxContainer *bc = VBoxContainer.new();
	 Label *l = Label.new();
	 l.text = slot_name;
	 bc.add_child(l);
	 SpinBox *sbx = SpinBox.new();
	 bc.add_child(sbx);
	 SpinBox *sby = SpinBox.new();
	 bc.add_child(sby);
	 int slot_idx = add_slot(MMNodeUniversalProperty.SlotTypes.SLOT_TYPE_NONE, MMNodeUniversalProperty.SlotTypes.SLOT_TYPE_NONE, getter, setter, bc);
	 sbx.rounded = false;
	 sby.rounded = false;
	 sbx.step = step;
	 sby.step = step;
	 sbx.min_value = prange.x;
	 sbx.max_value = prange.y;
	 sby.min_value = prange.x;
	 sby.max_value = prange.y;
	 Vector2 val = _node.call(getter);
	 sbx.value = val.x;
	 sby.value = val.y;
	 sbx.connect("value_changed", self, "on_vector2_spinbox_value_changed", [ slot_idx, sbx, sby ]);
	 sby.connect("value_changed", self, "on_vector2_spinbox_value_changed", [ slot_idx, sbx, sby ]);
	 return slot_idx;
 }

 int MmGraphNode::add_slot_vector3(const String &getter, const String &setter, const String &slot_name, const float step, const Vector2 &prange, const Variant & 1000)) {
	 VBoxContainer *bc = VBoxContainer.new();
	 Label *l = Label.new();
	 l.text = slot_name;
	 bc.add_child(l);
	 SpinBox *sbx = SpinBox.new();
	 bc.add_child(sbx);
	 SpinBox *sby = SpinBox.new();
	 bc.add_child(sby);
	 SpinBox *sbz = SpinBox.new();
	 bc.add_child(sbz);
	 int slot_idx = add_slot(MMNodeUniversalProperty.SlotTypes.SLOT_TYPE_NONE, MMNodeUniversalProperty.SlotTypes.SLOT_TYPE_NONE, getter, setter, bc);
	 sbx.rounded = false;
	 sby.rounded = false;
	 sbz.rounded = false;
	 sbx.step = step;
	 sby.step = step;
	 sbz.step = step;
	 sbx.min_value = prange.x;
	 sbx.max_value = prange.y;
	 sby.min_value = prange.x;
	 sby.max_value = prange.y;
	 sbz.min_value = prange.x;
	 sbz.max_value = prange.y;
	 Vector3 val = _node.call(getter);
	 sbx.value = val.x;
	 sby.value = val.y;
	 sbz.value = val.z;
	 sbx.connect("value_changed", self, "on_vector3_spinbox_value_changed", [ slot_idx, sbx, sby, sbz ]);
	 sby.connect("value_changed", self, "on_vector3_spinbox_value_changed", [ slot_idx, sbx, sby, sbz ]);
	 sbz.connect("value_changed", self, "on_vector3_spinbox_value_changed", [ slot_idx, sbx, sby, sbz ]);
	 return slot_idx;
 }

 int MmGraphNode::add_slot_vector2_universal(const MMNodeUniversalProperty &property) {
	 VBoxContainer *bc = VBoxContainer.new();
	 Label *l = Label.new();
	 l.text = property.slot_name;
	 bc.add_child(l);
	 SpinBox *sbx = SpinBox.new();
	 bc.add_child(sbx);
	 SpinBox *sby = SpinBox.new();
	 bc.add_child(sby);
	 int slot_idx = add_slot(property.input_slot_type, property.output_slot_type, "", "", bc);
	 sbx.rounded = false;
	 sby.rounded = false;
	 sbx.step = property.value_step;
	 sby.step = property.value_step;
	 sbx.min_value = property.value_range.x;
	 sbx.max_value = property.value_range.y;
	 sby.min_value = property.value_range.x;
	 sby.max_value = property.value_range.y;
	 Vector2 val = property.get_default_value();
	 sbx.value = val.x;
	 sby.value = val.y;
	 properties[slot_idx].append(property);
	 sbx.connect("value_changed", self, "on_vector2_universal_spinbox_value_changed", [ slot_idx, sbx, sby ]);
	 sby.connect("value_changed", self, "on_vector2_universal_spinbox_value_changed", [ slot_idx, sbx, sby ]);
	 return slot_idx;
 }

 int MmGraphNode::add_slot(const int input_type, const int output_type, const String &getter, const String &setter, const Control &control) {
	 add_child(control);
	 int slot_idx = get_child_count() - 1;
	 Array arr = Array();
	 arr.append(slot_idx);
	 arr.append(input_type);
	 arr.append(output_type);
	 arr.append(getter);
	 arr.append(setter);
	 arr.append(control);
	 properties.append(arr);
	 set_slot_enabled_left(slot_idx, input_type != -1);
	 set_slot_enabled_right(slot_idx, output_type != -1);

	 if (input_type != -1) {
		 set_slot_type_left(slot_idx, input_type);
		 set_slot_color_left(slot_idx, get_slot_color(input_type));
	 }

	 if (output_type != -1) {
		 set_slot_type_left(slot_idx, output_type);
		 set_slot_color_right(slot_idx, get_slot_color(output_type));
	 }

	 return slot_idx;
 }

 bool MmGraphNode::connect_slot(const int slot_idx, const Node &to_node, const int to_slot_idx) {
	 int from_property_index = -1;
	 int to_property_index = -1;

	 for (int i = 0; i < properties.size(); ++i) { //i in range(properties.size())

		 if (properties[i][2] != -1) {
			 from_property_index += 1;

			 if (from_property_index == slot_idx) {
				 from_property_index = i;
				 break;
			 }
		 }
	 }

	 for (int i = 0; i < to_node.properties.size(); ++i) { //i in range(to_node.properties.size())

		 if (to_node.properties[i][1] != -1) {
			 to_property_index += 1;

			 if (to_property_index == to_slot_idx) {
				 to_property_index = i;
				 break;
			 }

			 //to_node.properties[to_property_index][6].set_input_property(properties[from_property_index][6]);
		 }
	 }

	 _undo_redo.create_action("MMGD: connect_slot");
	 _undo_redo.add_do_method(to_node.properties[to_property_index][6], "set_input_property", properties[from_property_index][6]);
	 _undo_redo.add_undo_method(to_node.properties[to_property_index][6], "set_input_property", to_node.properties[to_property_index][6].input_property);
	 _undo_redo.commit_action();
	 return true;
 }

 bool MmGraphNode::disconnect_slot(const int slot_idx, const Node &to_node, const int to_slot_idx) {
	 int from_property_index = -1;
	 int to_property_index = -1;

	 for (int i = 0; i < properties.size(); ++i) { //i in range(properties.size())

		 if (properties[i][2] != -1) {
			 from_property_index += 1;

			 if (from_property_index == slot_idx) {
				 from_property_index = i;
				 break;
			 }
		 }
	 }

	 for (int i = 0; i < to_node.properties.size(); ++i) { //i in range(to_node.properties.size())

		 if (to_node.properties[i][1] != -1) {
			 to_property_index += 1;

			 if (to_property_index == to_slot_idx) {
				 to_property_index = i;
				 break;
			 }

			 //to_node.properties[to_property_index][6].set_input_property(null);
		 }
	 }

	 _undo_redo.create_action("MMGD: disconnect_slot");
	 _undo_redo.add_do_method(to_node.properties[to_property_index][6], "unset_input_property");
	 _undo_redo.add_undo_method(to_node.properties[to_property_index][6], "set_input_property", to_node.properties[to_property_index][6].input_property);
	 _undo_redo.commit_action();
	 return true;
 }

 int MmGraphNode::get_input_property_graph_node_slot_index(const Variant &property) {
	 int property_index = -1;

	 for (int i = 0; i < properties.size(); ++i) { //i in range(properties.size())

		 if (properties[i][1] != -1) {
			 property_index += 1;

			 if (properties[i][6] == property) {
				 break;
			 }
		 }
	 }

	 return property_index;
 }

 int MmGraphNode::get_output_property_graph_node_slot_index(const Variant &property) {
	 int property_index = -1;

	 for (int i = 0; i < properties.size(); ++i) { //i in range(properties.size())

		 if (properties[i][2] != -1) {
			 property_index += 1;

			 if (properties[i][6] == property) {
				 break;
			 }
		 }
	 }

	 return property_index;
 }

 Node MmGraphNode::get_property_control(const int slot_idx) {
	 return properties[slot_idx][5];
 }

 void MmGraphNode::set_node(const MMMateial &material, const MMNode &node) {
	 _node = node;
	 _material = material;

	 if (!_node) {
		 return;
	 }

	 title = _node.get_class();

	 if (_node.get_script()) {
		 title = _node.get_script().resource_path.get_file().get_basename();
	 }

	 _node.register_methods(self);
	 offset = _node.get_graph_position();
 }

 //_node.connect("changed", self, "on_node_changed");

 void MmGraphNode::propagate_node_change() {
	 pass;
 }

 void MmGraphNode::on_dragged(const Vector2 &from, const Vector2 &to) {
	 if (_node) {
		 ignore_changes(true);
		 //_node.set_graph_position(offset);
		 _undo_redo.create_action("MMGD: value changed");
		 _undo_redo.add_do_method(_node, "set_graph_position", to);
		 _undo_redo.add_undo_method(_node, "set_graph_position", from);
		 _undo_redo.commit_action();
		 ignore_changes(false);
	 }

	 //func on_node_changed():;
	 //	if _ignore_change_event:;
	 //		return;
	 //;
	 //	_ignore_change_event = true;
	 //	propagate_node_change();
	 //	_ignore_change_event = false;
 }

 void MmGraphNode::on_int_spinbox_value_changed(const float val, const Variant &slot_idx) {
	 //_node.call(properties[slot_idx][4], int(val));
	 ignore_changes(true);
	 _undo_redo.create_action("MMGD: value changed");
	 _undo_redo.add_do_method(_node, properties[slot_idx][4], int(val));
	 _undo_redo.add_undo_method(_node, properties[slot_idx][4], _node.call(properties[slot_idx][3]));
	 _undo_redo.commit_action();
	 ignore_changes(false);
 }

 void MmGraphNode::on_float_spinbox_value_changed(const float val, const Variant &slot_idx) {
	 //_node.call(properties[slot_idx][4], val);
	 ignore_changes(true);
	 _undo_redo.create_action("MMGD: value changed");
	 _undo_redo.add_do_method(_node, properties[slot_idx][4], val);
	 _undo_redo.add_undo_method(_node, properties[slot_idx][4], _node.call(properties[slot_idx][3]));
	 _undo_redo.commit_action();
	 ignore_changes(false);
 }

 void MmGraphNode::on_vector2_spinbox_value_changed(const float val, const Variant &slot_idx, const Variant &spinbox_x, const Variant &spinbox_y) {
	 Vector2 vv = Vector2(spinbox_x.value, spinbox_y.value);
	 //_node.call(properties[slot_idx][4], vv);
	 ignore_changes(true);
	 _undo_redo.create_action("MMGD: value changed");
	 _undo_redo.add_do_method(_node, properties[slot_idx][4], vv);
	 _undo_redo.add_undo_method(_node, properties[slot_idx][4], _node.call(properties[slot_idx][3]));
	 _undo_redo.commit_action();
	 ignore_changes(false);
 }

 void MmGraphNode::on_vector3_spinbox_value_changed(const float val, const Variant &slot_idx, const Variant &spinbox_x, const Variant &spinbox_y, const Variant &spinbox_z) {
	 Vector3 vv = Vector3(spinbox_x.value, spinbox_y.value, spinbox_z.value);
	 //_node.call(properties[slot_idx][4], vv);
	 ignore_changes(true);
	 _undo_redo.create_action("MMGD: value changed");
	 _undo_redo.add_do_method(_node, properties[slot_idx][4], vv);
	 _undo_redo.add_undo_method(_node, properties[slot_idx][4], _node.call(properties[slot_idx][3]));
	 _undo_redo.commit_action();
	 ignore_changes(false);
 }

 void MmGraphNode::on_int_universal_spinbox_value_changed(const float val, const Variant &slot_idx) {
	 //properties[slot_idx][6].set_default_value(int(val));
	 ignore_changes(true);
	 _undo_redo.create_action("MMGD: value changed");
	 _undo_redo.add_do_method(properties[slot_idx][6], "set_default_value", int(val));
	 _undo_redo.add_undo_method(properties[slot_idx][6], "set_default_value", properties[slot_idx][6].get_default_value());
	 _undo_redo.commit_action();
	 ignore_changes(false);
 }

 void MmGraphNode::on_float_universal_spinbox_value_changed(const float val, const Variant &slot_idx) {
	 //properties[slot_idx][6].set_default_value(val);
	 ignore_changes(true);
	 _undo_redo.create_action("MMGD: value changed");
	 _undo_redo.add_do_method(properties[slot_idx][6], "set_default_value", val);
	 _undo_redo.add_undo_method(properties[slot_idx][6], "set_default_value", properties[slot_idx][6].get_default_value());
	 _undo_redo.commit_action();
	 ignore_changes(false);
 }

 void MmGraphNode::on_vector2_universal_spinbox_value_changed(const float val, const Variant &slot_idx, const Variant &spinbox_x, const Variant &spinbox_y) {
	 Vector2 vv = Vector2(spinbox_x.value, spinbox_y.value);
	 //properties[slot_idx][6].set_default_value(vv);
	 ignore_changes(true);
	 _undo_redo.create_action("MMGD: value changed");
	 _undo_redo.add_do_method(properties[slot_idx][6], "set_default_value", vv);
	 _undo_redo.add_undo_method(properties[slot_idx][6], "set_default_value", properties[slot_idx][6].get_default_value());
	 _undo_redo.commit_action();
	 ignore_changes(false);
 }

 void MmGraphNode::on_slot_enum_item_selected(const int val, const int slot_idx) {
	 //_node.call(properties[slot_idx][4], val);
	 ignore_changes(true);
	 _undo_redo.create_action("MMGD: value changed");
	 _undo_redo.add_do_method(_node, properties[slot_idx][4], val);
	 _undo_redo.add_undo_method(_node, properties[slot_idx][4], _node.call(properties[slot_idx][3]));
	 _undo_redo.commit_action();
	 ignore_changes(false);
 }

 void MmGraphNode::on_universal_texture_changed(const int slot_idx) {
	 ignore_changes(true);
	 Ref<Image> img = properties[slot_idx][6].get_active_image();
	 Ref<ImageTexture> tex = properties[slot_idx][5].texture;

	 if (img) {
		 properties[slot_idx][5].texture.create_from_image(img, 0);
	 }

	 else {
		 properties[slot_idx][5].texture = ImageTexture.new();
	 }

	 ignore_changes(false);
 }

 void MmGraphNode::on_universal_texture_changed_image_picker(const int slot_idx) {
	 ignore_changes(true);
	 Ref<Image> img = properties[slot_idx][6].get_active_image();
	 Ref<ImageTexture> tex = properties[slot_idx][5].texture_normal;

	 if (img) {
		 properties[slot_idx][5].texture_normal.create_from_image(img, 0);
	 }

	 else {
		 properties[slot_idx][5].texture_normal = ImageTexture.new();
	 }

	 ignore_changes(false);
 }

 void MmGraphNode::on_slot_line_edit_text_entered(const String &text, const int slot_idx) {
	 //_node.call(properties[slot_idx][4], text);
	 ignore_changes(true);
	 _undo_redo.create_action("MMGD: value changed");
	 _undo_redo.add_do_method(_node, properties[slot_idx][4], text);
	 _undo_redo.add_undo_method(_node, properties[slot_idx][4], _node.call(properties[slot_idx][3]));
	 _undo_redo.commit_action();
	 ignore_changes(false);
 }

 void MmGraphNode::on_universal_color_changed(const Color &c, const int slot_idx) {
	 //properties[slot_idx][6].set_default_value(c);
	 ignore_changes(true);
	 _undo_redo.create_action("MMGD: value changed");
	 _undo_redo.add_do_method(properties[slot_idx][6], "set_default_value", c);
	 _undo_redo.add_undo_method(properties[slot_idx][6], "set_default_value", properties[slot_idx][6].get_default_value());
	 _undo_redo.commit_action();
	 ignore_changes(false);
 }

 void MmGraphNode::on_universal_image_path_changed(const String &f, const int slot_idx) {
	 _node.call(properties[slot_idx][8], f);
	 ignore_changes(true);
	 _undo_redo.create_action("MMGD: value changed");
	 _undo_redo.add_do_method(properties[slot_idx][6], "set_default_value", f);
	 _undo_redo.add_undo_method(properties[slot_idx][6], "set_default_value", properties[slot_idx][6].get_default_value());
	 _undo_redo.commit_action();
	 ignore_changes(false);
 }

 MMNode MmGraphNode::get_material_node() {
	 return _node;
 }

 void MmGraphNode::on_close_request() {
	 Node *n = get_parent();

	 while (n) {
		 if (n.has_method("on_graph_node_close_request")) {
			 n.call_deferred("on_graph_node_close_request", self);
			 return;
		 }

		 n = n.get_parent();
	 }
 }

 Color MmGraphNode::get_slot_color(const int slot_type) {
	 return _get_slot_color(slot_type);
 }

 Color MmGraphNode::_get_slot_color(const int slot_type) {
	 if (slot_type == 0) {
		 return Color(0.91, 0.06, 0.06);
	 }

	 else if (slot_type == 1) {
		 return Color(0.43, 0.04, 0.04);
	 }

	 else if (slot_type == 2) {
		 return Color(0.83, 0.38, 0.38);
	 }

	 else if (slot_type == 3) {
		 return Color(0.04, 0.48, 0.43);
	 }

	 else if (slot_type == 4) {
		 return Color(0.35, 0.04, 0.34);
	 }

	 else if (slot_type == 5) {
		 return Color(0.04, 0.05, 1);
	 }

	 else if (slot_type == 6) {
		 return Color(0.37, 0.37, 0.37);
	 }

	 return Color(1, 1, 1, 1);
 }
 }

 MmGraphNode::MmGraphNode() {
	 //var gradient_editor_scene : PackedScene = preload("res://addons/mat_maker_gd/widgets/gradient_editor/gradient_editor.tscn");
	 //var polygon_edit_scene : PackedScene = preload("res://addons/mat_maker_gd/widgets/polygon_edit/polygon_edit.tscn");
	 //var curve_edit_scene : PackedScene = preload("res://addons/mat_maker_gd/widgets/curve_edit/curve_edit.tscn");
	 *_material = null;
	 *_node = null;
	 properties = Array();
	 ;
	 *_undo_redo = null;
	 _ignore_change_event = false;
 }

 MmGraphNode::~MmGraphNode() {
 }

 static void MmGraphNode::_bind_methods() {
	 ClassDB::bind_method(D_METHOD("get_gradient_editor_scene"), &MmGraphNode::get_gradient_editor_scene);
	 ClassDB::bind_method(D_METHOD("set_gradient_editor_scene", "value"), &MmGraphNode::set_gradient_editor_scene);
	 ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "gradient_editor_scene", PROPERTY_HINT_RESOURCE_TYPE, "Ref<PackedScene>"), "set_gradient_editor_scene", "get_gradient_editor_scene");

	 ClassDB::bind_method(D_METHOD("get_polygon_edit_scene"), &MmGraphNode::get_polygon_edit_scene);
	 ClassDB::bind_method(D_METHOD("set_polygon_edit_scene", "value"), &MmGraphNode::set_polygon_edit_scene);
	 ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "polygon_edit_scene", PROPERTY_HINT_RESOURCE_TYPE, "Ref<PackedScene>"), "set_polygon_edit_scene", "get_polygon_edit_scene");

	 ClassDB::bind_method(D_METHOD("get_curve_edit_scene"), &MmGraphNode::get_curve_edit_scene);
	 ClassDB::bind_method(D_METHOD("set_curve_edit_scene", "value"), &MmGraphNode::set_curve_edit_scene);
	 ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "curve_edit_scene", PROPERTY_HINT_RESOURCE_TYPE, "Ref<PackedScene>"), "set_curve_edit_scene", "get_curve_edit_scene");

	 ClassDB::bind_method(D_METHOD("get_*_material"), &MmGraphNode::get_ * _material);
	 ClassDB::bind_method(D_METHOD("set_*_material", "value"), &MmGraphNode::set_ * _material);
	 ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*_material", PROPERTY_HINT_RESOURCE_TYPE, "MMMateial"), "set_*_material", "get_*_material");

	 ClassDB::bind_method(D_METHOD("get_*_node"), &MmGraphNode::get_ * _node);
	 ClassDB::bind_method(D_METHOD("set_*_node", "value"), &MmGraphNode::set_ * _node);
	 ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*_node", PROPERTY_HINT_RESOURCE_TYPE, "MMNode"), "set_*_node", "get_*_node");

	 ClassDB::bind_method(D_METHOD("get_properties"), &MmGraphNode::get_properties);
	 ClassDB::bind_method(D_METHOD("set_properties", "value"), &MmGraphNode::set_properties);
	 ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "properties"), "set_properties", "get_properties");

	 ClassDB::bind_method(D_METHOD("get_Variant"), &MmGraphNode::get_Variant);
	 ClassDB::bind_method(D_METHOD("set_Variant", "value"), &MmGraphNode::set_Variant);
	 ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "Variant", PROPERTY_HINT_RESOURCE_TYPE, "Variant"), "set_Variant", "get_Variant");

	 ClassDB::bind_method(D_METHOD("get_*_undo_redo"), &MmGraphNode::get_ * _undo_redo);
	 ClassDB::bind_method(D_METHOD("set_*_undo_redo", "value"), &MmGraphNode::set_ * _undo_redo);
	 ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*_undo_redo", PROPERTY_HINT_RESOURCE_TYPE, "UndoRedo"), "set_*_undo_redo", "get_*_undo_redo");

	 ClassDB::bind_method(D_METHOD("get__ignore_change_event"), &MmGraphNode::get__ignore_change_event);
	 ClassDB::bind_method(D_METHOD("set__ignore_change_event", "value"), &MmGraphNode::set__ignore_change_event);
	 ADD_PROPERTY(PropertyInfo(Variant::BOOL, "_ignore_change_event"), "set__ignore_change_event", "get__ignore_change_event");

	 ClassDB::bind_method(D_METHOD("_init"), &MmGraphNode::_init);
	 ClassDB::bind_method(D_METHOD("set_editor", "editor_node"), &MmGraphNode::set_editor);
	 ClassDB::bind_method(D_METHOD("ignore_changes", "val"), &MmGraphNode::ignore_changes);
	 ClassDB::bind_method(D_METHOD("add_slot_texture", "getter", "setter"), &MmGraphNode::add_slot_texture);
	 ClassDB::bind_method(D_METHOD("add_slot_texture_universal", "property"), &MmGraphNode::add_slot_texture_universal);
	 ClassDB::bind_method(D_METHOD("add_slot_image_path_universal", "property", "getter", "setter"), &MmGraphNode::add_slot_image_path_universal);
	 ClassDB::bind_method(D_METHOD("add_slot_gradient"), &MmGraphNode::add_slot_gradient);
	 ClassDB::bind_method(D_METHOD("add_slot_polygon"), &MmGraphNode::add_slot_polygon);
	 ClassDB::bind_method(D_METHOD("add_slot_curve"), &MmGraphNode::add_slot_curve);
	 ClassDB::bind_method(D_METHOD("add_slot_color", "getter", "setter"), &MmGraphNode::add_slot_color);
	 ClassDB::bind_method(D_METHOD("add_slot_color_universal", "property"), &MmGraphNode::add_slot_color_universal);
	 ClassDB::bind_method(D_METHOD("add_slot_label", "getter", "setter", "slot_name"), &MmGraphNode::add_slot_label);
	 ClassDB::bind_method(D_METHOD("add_slot_line_edit", "getter", "setter", "slot_name", "placeholder"), &MmGraphNode::add_slot_line_edit, "");
	 ClassDB::bind_method(D_METHOD("add_slot_enum", "getter", "setter", "slot_name", "values"), &MmGraphNode::add_slot_enum);
  ClassDB::bind_method(D_METHOD("add_slot_int", "getter", "setter", "slot_name", "prange", " 1000)"), &MmGraphNode::add_slot_int, Vector2(-1000);
  ClassDB::bind_method(D_METHOD("add_slot_bool", "getter", "setter", "slot_name"), &MmGraphNode::add_slot_bool);
  ClassDB::bind_method(D_METHOD("add_slot_label_universal", "property"), &MmGraphNode::add_slot_label_universal);
  ClassDB::bind_method(D_METHOD("add_slot_int_universal", "property"), &MmGraphNode::add_slot_int_universal);
  ClassDB::bind_method(D_METHOD("add_slot_float", "getter", "setter", "slot_name", "step", "prange", " 1000)"), &MmGraphNode::add_slot_float, 0.1, Vector2(-1000);
  ClassDB::bind_method(D_METHOD("add_slot_float_universal", "property"), &MmGraphNode::add_slot_float_universal);
  ClassDB::bind_method(D_METHOD("add_slot_vector2", "getter", "setter", "slot_name", "step", "prange", " 1000)"), &MmGraphNode::add_slot_vector2, 0.1, Vector2(-1000);
  ClassDB::bind_method(D_METHOD("add_slot_vector3", "getter", "setter", "slot_name", "step", "prange", " 1000)"), &MmGraphNode::add_slot_vector3, 0.1, Vector2(-1000);
  ClassDB::bind_method(D_METHOD("add_slot_vector2_universal", "property"), &MmGraphNode::add_slot_vector2_universal);
  ClassDB::bind_method(D_METHOD("add_slot", "input_type", "output_type", "getter", "setter", "control"), &MmGraphNode::add_slot);
  ClassDB::bind_method(D_METHOD("connect_slot", "slot_idx", "to_node", "to_slot_idx"), &MmGraphNode::connect_slot);
  ClassDB::bind_method(D_METHOD("disconnect_slot", "slot_idx", "to_node", "to_slot_idx"), &MmGraphNode::disconnect_slot);
  ClassDB::bind_method(D_METHOD("get_input_property_graph_node_slot_index", "property"), &MmGraphNode::get_input_property_graph_node_slot_index);
  ClassDB::bind_method(D_METHOD("get_output_property_graph_node_slot_index", "property"), &MmGraphNode::get_output_property_graph_node_slot_index);
  ClassDB::bind_method(D_METHOD("get_property_control", "slot_idx"), &MmGraphNode::get_property_control);
  ClassDB::bind_method(D_METHOD("set_node", "material", "node"), &MmGraphNode::set_node);
  ClassDB::bind_method(D_METHOD("propagate_node_change"), &MmGraphNode::propagate_node_change);
  ClassDB::bind_method(D_METHOD("on_dragged", "from", "to"), &MmGraphNode::on_dragged);
  ClassDB::bind_method(D_METHOD("on_int_spinbox_value_changed", "val", " slot_idx"), &MmGraphNode::on_int_spinbox_value_changed);
  ClassDB::bind_method(D_METHOD("on_float_spinbox_value_changed", "val", " slot_idx"), &MmGraphNode::on_float_spinbox_value_changed);
  ClassDB::bind_method(D_METHOD("on_vector2_spinbox_value_changed", "val", " slot_idx", " spinbox_x", " spinbox_y"), &MmGraphNode::on_vector2_spinbox_value_changed);
  ClassDB::bind_method(D_METHOD("on_vector3_spinbox_value_changed", "val", " slot_idx", " spinbox_x", " spinbox_y", " spinbox_z"), &MmGraphNode::on_vector3_spinbox_value_changed);
  ClassDB::bind_method(D_METHOD("on_int_universal_spinbox_value_changed", "val", " slot_idx"), &MmGraphNode::on_int_universal_spinbox_value_changed);
  ClassDB::bind_method(D_METHOD("on_float_universal_spinbox_value_changed", "val", " slot_idx"), &MmGraphNode::on_float_universal_spinbox_value_changed);
  ClassDB::bind_method(D_METHOD("on_vector2_universal_spinbox_value_changed", "val", " slot_idx", " spinbox_x", " spinbox_y"), &MmGraphNode::on_vector2_universal_spinbox_value_changed);
  ClassDB::bind_method(D_METHOD("on_slot_enum_item_selected", "val", "slot_idx"), &MmGraphNode::on_slot_enum_item_selected);
  ClassDB::bind_method(D_METHOD("on_universal_texture_changed", "slot_idx"), &MmGraphNode::on_universal_texture_changed);
  ClassDB::bind_method(D_METHOD("on_universal_texture_changed_image_picker", "slot_idx"), &MmGraphNode::on_universal_texture_changed_image_picker);
  ClassDB::bind_method(D_METHOD("on_slot_line_edit_text_entered", "text", "slot_idx"), &MmGraphNode::on_slot_line_edit_text_entered);
  ClassDB::bind_method(D_METHOD("on_universal_color_changed", "c", "slot_idx"), &MmGraphNode::on_universal_color_changed);
  ClassDB::bind_method(D_METHOD("on_universal_image_path_changed", "f", "slot_idx"), &MmGraphNode::on_universal_image_path_changed);
  ClassDB::bind_method(D_METHOD("get_material_node"), &MmGraphNode::get_material_node);
  ClassDB::bind_method(D_METHOD("on_close_request"), &MmGraphNode::on_close_request);
  ClassDB::bind_method(D_METHOD("get_slot_color", "slot_type"), &MmGraphNode::get_slot_color);
  ClassDB::bind_method(D_METHOD("_get_slot_color", "slot_type"), &MmGraphNode::_get_slot_color);
 }
