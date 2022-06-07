
#include "mm_graph_node.h"

#include "../nodes/mm_material.h"
#include "../nodes/mm_node.h"
#include "../nodes/mm_node_universal_property.h"
#include "scene/gui/texture_rect.h"

Ref<PackedScene> MMGraphNode::get_gradient_editor_scene() {
	return gradient_editor_scene;
}

void MMGraphNode::set_gradient_editor_scene(const Ref<PackedScene> &val) {
	gradient_editor_scene = val;
}

Ref<PackedScene> MMGraphNode::get_polygon_edit_scene() {
	return polygon_edit_scene;
}

void MMGraphNode::set_polygon_edit_scene(const Ref<PackedScene> &val) {
	polygon_edit_scene = val;
}

Ref<PackedScene> MMGraphNode::get_curve_edit_scene() {
	return curve_edit_scene;
}

void MMGraphNode::set_curve_edit_scene(const Ref<PackedScene> &val) {
	curve_edit_scene = val;
}

Ref<MMMaterial> MMGraphNode::get_mm_material() {
	return _material;
}

void MMGraphNode::set_mm_material(const Ref<MMMaterial> &val) {
	_material = val;
}

Ref<MMNode> MMGraphNode::get_material_node() {
	return _node;
}

Ref<MMNode> MMGraphNode::get_node() {
	return *_node;
}

void MMGraphNode::set_node(const Ref<MMNode> &val) {
	_node = val;
}

Array MMGraphNode::get_properties() {
	return properties;
}

void MMGraphNode::set_properties(const Array &val) {
	properties = val;
}

MatMakerGDEditor *MMGraphNode::get_editor_node() {
	return _editor_node;
}

void MMGraphNode::set_editor_node(MatMakerGDEditor *val) {
	_editor_node = val;
	_undo_redo = _editor_node->get_undo_redo();
}

UndoRedo *MMGraphNode::get_undo_redo() {
	return _undo_redo;
}

void MMGraphNode::set_undo_redo(UndoRedo *val) {
	_undo_redo = val;
}

bool MMGraphNode::get_ignore_change_event() const {
	return _ignore_change_event;
}

void MMGraphNode::set_ignore_change_event(const bool val) {
	_ignore_change_event = val;
}

void MMGraphNode::_init() {
	set_show_close_button(true);

	connect("dragged", this, "on_dragged");
	connect("close_request", this, "on_close_request");
}

void MMGraphNode::ignore_changes(const bool val) {
	_ignore_change_event = val;
	_editor_node->ignore_changes(val);
}

int MMGraphNode::add_slot_texture(const String &getter, const String &setter) {
	TextureRect *t = memnew(TextureRect);
	t->set_custom_minimum_size(Vector2(128, 128));
	t->set_expand(true);
	t->set_stretch_mode(TextureRect::STRETCH_KEEP_ASPECT_CENTERED);
	int slot_idx = add_slot(MMNodeUniversalProperty::SLOT_TYPE_NONE, MMNodeUniversalProperty::SLOT_TYPE_NONE, getter, setter, t);
	t->set_texture(_node->call(getter, _material, slot_idx));
	properties[slot_idx].append(t->get_texture());
	return slot_idx;
}

int MMGraphNode::add_slot_texture_universal(const Ref<MMNodeUniversalProperty> &property) {
	TextureRect *t = memnew(TextureRect);
	t->set_custom_minimum_size(Vector2(128, 128));
	t->set_expand(true);
	t->set_stretch_mode(TextureRect::STRETCH_KEEP_ASPECT_CENTERED);
	int slot_idx = add_slot(property->input_slot_type, property.output_slot_type, "", "", t);
	Ref<Image> img = property->get_active_image();
	Ref<ImageTexture> tex;
	tex.instance();

	if (img) {
		tex->create_from_image(img, 0);
	}

	t->set_texture(tex);
	properties[slot_idx].append(property);
	property.connect("changed", self, "on_universal_texture_changed", [slot_idx]);
	return slot_idx;
}

int MMGraphNode::add_slot_image_path_universal(const Ref<MMNodeUniversalProperty> &property, const String &getter, const String &setter) {
	/*
	  TextureButton *t = load("res://addons/mat_maker_gd/widgets/image_picker_button/image_picker_button.tscn").instance();
	  int slot_idx = add_slot(property.input_slot_type, property.output_slot_type, "", "", t);
	  properties[slot_idx].append(property);
	  properties[slot_idx].append(getter);
	  properties[slot_idx].append(setter);
	  property.connect("changed", self, "on_universal_texture_changed_image_picker", [slot_idx]);
	  t.connect("on_file_selected", self, "on_universal_image_path_changed", [slot_idx]);
	  t.call_deferred("do_set_image_path", _node.call(getter));
	  return slot_idx;
	*/

	return 0;
}

int MMGraphNode::add_slot_gradient() {
	/*
	  Control *ge = gradient_editor_scene.instance();
	  ge.graph_node = self;
	  ge.set_undo_redo(_undo_redo);
	  int slot_idx = add_slot(MMNodeUniversalProperty::SLOT_TYPE_NONE, MMNodeUniversalProperty::SLOT_TYPE_NONE, "", "", ge);
	  ge.set_value(_node);
	  //ge.texture = _node.call(getter, _material, slot_idx);
	  //properties[slot_idx].append(ge.texture);
	  return slot_idx;
	*/

	return 0;
}

int MMGraphNode::add_slot_polygon() {
	/*
	  Control *ge = polygon_edit_scene.instance();
	  int slot_idx = add_slot(MMNodeUniversalProperty::SLOT_TYPE_NONE, MMNodeUniversalProperty::SLOT_TYPE_NONE, "", "", ge);
	  ge.set_value(_node);
	  //ge.texture = _node.call(getter, _material, slot_idx);
	  //properties[slot_idx].append(ge.texture);
	  return slot_idx;
	*/

	return 0;
}

int MMGraphNode::add_slot_curve() {
	/*
	  Control *ge = curve_edit_scene.instance();
	  int slot_idx = add_slot(MMNodeUniversalProperty::SLOT_TYPE_NONE, MMNodeUniversalProperty::SLOT_TYPE_NONE, "", "", ge);
	  ge.set_value(_node);
	  //ge.texture = _node.call(getter, _material, slot_idx);
	  //properties[slot_idx].append(ge.texture);
	  return slot_idx;
	*/

	return 0;
}

int MMGraphNode::add_slot_color(const String &getter, const String &setter) {
	/*
	  ColorPickerButton *cp = ColorPickerButton.new();
	  int slot_idx = add_slot(MMNodeUniversalProperty::SLOT_TYPE_NONE, MMNodeUniversalProperty::SLOT_TYPE_NONE, getter, setter, cp);
	  cp.color = _node.call(getter);
	  cp.connect("color_changed", _node, setter);
	  return slot_idx;
	*/

	return 0;
}

int MMGraphNode::add_slot_color_universal(const Ref<MMNodeUniversalProperty> &property) {
	ColorPickerButton *cp = memnew(ColorPickerButton);
	int slot_idx = add_slot(property.input_slot_type, property.output_slot_type, "", "", cp);
	cp->set_color(property->get_default_value());
	properties[slot_idx].append(property);
	cp->connect("color_changed", this, "on_universal_color_changed", [slot_idx]);
	return slot_idx;
}

int MMGraphNode::add_slot_label(const String &getter, const String &setter, const String &slot_name) {
	Label *l = memnew(Label);
	l->set_text(slot_name);
	return add_slot(MMNodeUniversalProperty::SLOT_TYPE_NONE, MMNodeUniversalProperty::SLOT_TYPE_NONE, getter, setter, l);
}

int MMGraphNode::add_slot_line_edit(const String &getter, const String &setter, const String &slot_name, const String &placeholder) {
	VBoxContainer *bc = memnew(VBoxContainer);
	Label *l = memnew(Label);
	l.set_text(slot_name);
	bc.add_child(l);
	LineEdit *le = LineEdit.new();
	le.set_placeholder_text(placeholder);
	bc.add_child(le);
	int slot_idx = add_slot(MMNodeUniversalProperty::SLOT_TYPE_NONE, MMNodeUniversalProperty::SLOT_TYPE_NONE, getter, setter, bc);
	le->set_text(_node->call(getter));
	le->connect("text_entered", this, "on_slot_line_edit_text_entered", [slot_idx]);
	return slot_idx;
}

int MMGraphNode::add_slot_enum(const String &getter, const String &setter, const String &slot_name, const Array &values) {
	VBoxContainer *bc = memnew(VBoxContainer);

	if (slot_name) {
		Label *l = memnew(Label);
		l->set_text(slot_name);
		bc->add_child(l);
	}

	OptionButton *mb = memnew(OptionButton);

	for (v in values) {
		mb.add_item(v);
	}

	bc->add_child(mb);
	int slot_idx = add_slot(MMNodeUniversalProperty::SLOT_TYPE_NONE, MMNodeUniversalProperty::SLOT_TYPE_NONE, getter, setter, bc);
	mb->set_selected(_node->call(getter));
	mb->connect("item_selected", this, "on_slot_enum_item_selected", [slot_idx]);
	return slot_idx;
}

 int MMGraphNode::add_slot_int(const String &getter, const String &setter, const String &slot_name, const Vector2 &prange, const Variant & 1000)) {
	 VBoxContainer *bc = memnew(VBoxContainer);
	 Label *l = memnew(Label);
	 l->set_text(slot_name);
	 bc->add_child(l);
	 SpinBox *sb = memnew(SpinBox);
	 sb->set_rounded(true);
	 sb->set_min_value(prange.x);
	 sb->set_max_value(prange.y);
	 bc->add_child(sb);
	 int slot_idx = add_slot(MMNodeUniversalProperty::SLOT_TYPE_NONE, MMNodeUniversalProperty::SLOT_TYPE_NONE, getter, setter, bc);
	 sb->set_value(_node->call(getter));
	 sb->connect("value_changed", this, "on_int_spinbox_value_changed", [slot_idx]);
	 return slot_idx;
 }

 int MMGraphNode::add_slot_bool(const String &getter, const String &setter, const String &slot_name) {
	 CheckBox *cb = memnew(CheckBox);
	 cb->set_text(slot_name);
	 int slot_idx = add_slot(MMNodeUniversalProperty::SLOT_TYPE_NONE, MMNodeUniversalProperty::SLOT_TYPE_NONE, getter, setter, cb);
	 cb->set_pressed(_node->call(getter));
	 cb->connect("toggled", _node, setter);
	 return slot_idx;
 }

 int MMGraphNode::add_slot_label_universal(const Ref<MMNodeUniversalProperty> &property) {
	 Label *l = memnew(Label);
	 l->set_text(property.slot_name);
	 int slot_idx = add_slot(property->input_slot_type, property->output_slot_type, "", "", l);
	 properties[slot_idx].append(property);
	 return slot_idx;
 }

 int MMGraphNode::add_slot_int_universal(const Ref<MMNodeUniversalProperty> &property) {
	 VBoxContainer *bc = memnew(VBoxContainer);
	 Label *l = memnew(Label);
	 l->set_text(property->slot_name);
	 bc->add_child(l);
	 SpinBox *sb = memnew(SpinBox);
	 sb->set_rounded(true);
	 sb->set_min_value(property.value_range.x);
	 sb->set_max_value(property.value_range.y);
	 bc->add_child(sb);
	 int slot_idx = add_slot(property->input_slot_type, property->output_slot_type, "", "", bc);
	 sb->set_value(property->get_default_value());
	 sb->connect("value_changed", this, "on_int_universal_spinbox_value_changed", [slot_idx]);
	 properties[slot_idx].append(property);
	 return slot_idx;
 }

 int MMGraphNode::add_slot_float(const String &getter, const String &setter, const String &slot_name, const float step, const Vector2 &prange, const Variant & 1000)) {
	 VBoxContainer *bc = memnew(VBoxContainer);
	 Label *l = memnew(Label);
	 l->set_text(slot_name);
	 bc->add_child(l);
	 SpinBox *sb = memnew(SpinBox);
	 bc->add_child(sb);
	 int slot_idx = add_slot(MMNodeUniversalProperty::SLOT_TYPE_NONE, MMNodeUniversalProperty::SLOT_TYPE_NONE, getter, setter, bc);
	 sb->set_rounded(false);
	 sb->set_step(step);
	 sb->set_min_value(prange.x);
	 sb->set_max_value(prange.y);
	 sb->set_value(_node->call(getter));
	 sb->connect("value_changed", this, "on_float_spinbox_value_changed", [slot_idx]);
	 return slot_idx;
 }

 int MMGraphNode::add_slot_float_universal(const Ref<MMNodeUniversalProperty> &property) {
	 VBoxContainer *bc = memnew(VBoxContainer);
	 Label *l = memnew(Label);
	 l->set_text(property->slot_name);
	 bc->add_child(l);
	 SpinBox *sb = memnew(SpinBox);
	 bc->add_child(sb);
	 int slot_idx = add_slot(property->input_slot_type, property->output_slot_type, "", "", bc);
	 sb->set_rounded(false);
	 sb->set_step(property->value_step);
	 sb->set_min_value(property->value_range.x);
	 sb->set_max_value(property->value_range.y);
	 sb->set_value(property.get_default_value());
	 properties[slot_idx].append(property);
	 sb->connect("value_changed", this, "on_float_universal_spinbox_value_changed", [slot_idx]);
	 return slot_idx;
 }

 int MMGraphNode::add_slot_vector2(const String &getter, const String &setter, const String &slot_name, const float step, const Vector2 &prange) {
	 VBoxContainer *bc = memnew(VBoxContainer);
	 Label *l = memnew(Label);
	 l->set_text(slot_name);
	 bc->add_child(l);
	 SpinBox *sbx = memnew(SpinBox);
	 bc->add_child(sbx);
	 SpinBox *sby = memnew(SpinBox);
	 bc->add_child(sby);
	 int slot_idx = add_slot(MMNodeUniversalProperty::SLOT_TYPE_NONE, MMNodeUniversalProperty::SLOT_TYPE_NONE, getter, setter, bc);
	 sbx->set_rounded(false);
	 sby->set_rounded(false);
	 sbx->set_step(step);
	 sby->set_step(step);
	 sbx->set_min_value(prange.x);
	 sbx->set_max_value(prange.y);
	 sby->set_min_value(prange.x);
	 sby->set_max_value(prange.y);
	 Vector2 val = _node->call(getter);
	 sbx->set_value(val.x);
	 sby->set_value(val.y);
	 sbx->connect("value_changed", this, "on_vector2_spinbox_value_changed", [ slot_idx, sbx, sby ]);
	 sby->connect("value_changed", this, "on_vector2_spinbox_value_changed", [ slot_idx, sbx, sby ]);
	 return slot_idx;
 }

 int MMGraphNode::add_slot_vector3(const String &getter, const String &setter, const String &slot_name, const float step, const Vector2 &prange, const Variant & 1000)) {
	 VBoxContainer *bc = memnew(VBoxContainer);
	 Label *l = memnew(Label);
	 l->set_text(slot_name);
	 bc->add_child(l);
	 SpinBox *sbx = memnew(SpinBox);
	 bc->add_child(sbx);
	 SpinBox *sby = memnew(SpinBox);
	 bc->add_child(sby);
	 SpinBox *sbz = memnew(SpinBox);
	 bc->add_child(sbz);
	 int slot_idx = add_slot(MMNodeUniversalProperty::SLOT_TYPE_NONE, MMNodeUniversalProperty::SLOT_TYPE_NONE, getter, setter, bc);
	 sbx->set_rounded(false);
	 sby->set_rounded(false);
	 sbz->set_rounded(false);
	 sbx->set_step(step);
	 sby->set_step(step);
	 sbz->set_step(step);
	 sbx->set_min_value(prange.x);
	 sbx->set_max_value(prange.y);
	 sby->set_min_value(prange.x);
	 sby->set_max_value(prange.y);
	 sbz->set_min_value(prange.x);
	 sbz->set_max_value(prange.y);
	 Vector3 val = _node->call(getter);
	 sbx->set_value(val.x);
	 sby->set_value(val.y);
	 sbz->set_value(val.z);
	 sbx->connect("value_changed", this, "on_vector3_spinbox_value_changed", [ slot_idx, sbx, sby, sbz ]);
	 sby->connect("value_changed", this, "on_vector3_spinbox_value_changed", [ slot_idx, sbx, sby, sbz ]);
	 sbz->connect("value_changed", this, "on_vector3_spinbox_value_changed", [ slot_idx, sbx, sby, sbz ]);
	 return slot_idx;
 }

 int MMGraphNode::add_slot_vector2_universal(const Ref<MMNodeUniversalProperty> &property) {
	 VBoxContainer *bc = memnew(VBoxContainer);
	 Label *l = memnew(Label);
	 l.text = property->slot_name;
	 bc.add_child(l);
	 SpinBox *sbx = memnew(SpinBox);
	 bc.add_child(sbx);
	 SpinBox *sby = memnew(SpinBox);
	 bc.add_child(sby);
	 int slot_idx = add_slot(property.input_slot_type, property.output_slot_type, "", "", bc);
	 sbx.set_rounded(false);
	 sby.set_rounded(false);
	 sbx.set_step(property->value_step);
	 sby.set_step(property->value_step);
	 sbx.set_min_value(property->value_range.x);
	 sbx.set_max_value(property->value_range.y);
	 sby.set_min_value(property->value_range.x);
	 sby.set_max_value(property->value_range.y);
	 Vector2 val = property->get_default_value();
	 sbx.set_value(val.x);
	 sby.set_value(val.y);
	 properties[slot_idx].append(property);
	 sbx.connect("value_changed", self, "on_vector2_universal_spinbox_value_changed", [ slot_idx, sbx, sby ]);
	 sby.connect("value_changed", self, "on_vector2_universal_spinbox_value_changed", [ slot_idx, sbx, sby ]);
	 return slot_idx;
 }

 int MMGraphNode::add_slot(const int input_type, const int output_type, const String &getter, const String &setter, Control *control) {
	 add_child(control);
	 int slot_idx = get_child_count() - 1;
	 Array arr;
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

 bool MMGraphNode::connect_slot(const int slot_idx, const Node *to_node, const int to_slot_idx) {
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

	 _undo_redo->create_action("MMGD: connect_slot");
	 _undo_redo->add_do_method(to_node->properties[to_property_index][6], "set_input_property", properties[from_property_index][6]);
	 _undo_redo->add_undo_method(to_node->properties[to_property_index][6], "set_input_property", to_node->properties[to_property_index][6].input_property);
	 _undo_redo->commit_action();
	 return true;
 }

 bool MMGraphNode::disconnect_slot(const int slot_idx, const Node *to_node, const int to_slot_idx) {
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

	 _undo_redo->create_action("MMGD: disconnect_slot");
	 _undo_redo->add_do_method(to_node->properties[to_property_index][6], "unset_input_property");
	 _undo_redo->add_undo_method(to_node->properties[to_property_index][6], "set_input_property", to_node->properties[to_property_index][6].input_property);
	 _undo_redo->commit_action();
	 return true;
 }

 int MMGraphNode::get_input_property_graph_node_slot_index(const Variant &property) {
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

 int MMGraphNode::get_output_property_graph_node_slot_index(const Variant &property) {
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

 Node MMGraphNode::get_property_control(const int slot_idx) {
	 return properties[slot_idx][5];
 }

 void MMGraphNode::set_node(const Ref<MMMaterial> &material, const Ref<MMNode> &node) {
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

	 //_node.connect("changed", self, "on_node_changed");
 }

 void MMGraphNode::propagate_node_change() {
 }

 void MMGraphNode::on_dragged(const Vector2 &from, const Vector2 &to) {
	 if (_node) {
		 ignore_changes(true);
		 //_node.set_graph_position(offset);
		 _undo_redo->create_action("MMGD: value changed");
		 _undo_redo->add_do_method(_node, "set_graph_position", to);
		 _undo_redo->add_undo_method(_node, "set_graph_position", from);
		 _undo_redo->commit_action();
		 ignore_changes(false);
	 }
 }

 //func on_node_changed():;
 //	if _ignore_change_event:;
 //		return;
 //;
 //	_ignore_change_event = true;
 //	propagate_node_change();
 //	_ignore_change_event = false;

 void MMGraphNode::on_int_spinbox_value_changed(const float val, const Variant &slot_idx) {
	 //_node.call(properties[slot_idx][4], int(val));
	 ignore_changes(true);
	 _undo_redo->create_action("MMGD: value changed");
	 _undo_redo->add_do_method(_node, properties[slot_idx][4], int(val));
	 _undo_redo->add_undo_method(_node, properties[slot_idx][4], _node.call(properties[slot_idx][3]));
	 _undo_redo->commit_action();
	 ignore_changes(false);
 }

 void MMGraphNode::on_float_spinbox_value_changed(const float val, const Variant &slot_idx) {
	 //_node.call(properties[slot_idx][4], val);
	 ignore_changes(true);
	 _undo_redo->create_action("MMGD: value changed");
	 _undo_redo->add_do_method(_node, properties[slot_idx][4], val);
	 _undo_redo->add_undo_method(_node, properties[slot_idx][4], _node.call(properties[slot_idx][3]));
	 _undo_redo->commit_action();
	 ignore_changes(false);
 }

 void MMGraphNode::on_vector2_spinbox_value_changed(const float val, const Variant &slot_idx, const Variant &spinbox_x, const Variant &spinbox_y) {
	 Vector2 vv = Vector2(spinbox_x.value, spinbox_y.value);
	 //_node.call(properties[slot_idx][4], vv);
	 ignore_changes(true);
	 _undo_redo->create_action("MMGD: value changed");
	 _undo_redo->add_do_method(_node, properties[slot_idx][4], vv);
	 _undo_redo->add_undo_method(_node, properties[slot_idx][4], _node.call(properties[slot_idx][3]));
	 _undo_redo->commit_action();
	 ignore_changes(false);
 }

 void MMGraphNode::on_vector3_spinbox_value_changed(const float val, const Variant &slot_idx, const Variant &spinbox_x, const Variant &spinbox_y, const Variant &spinbox_z) {
	 Vector3 vv = Vector3(spinbox_x.value, spinbox_y.value, spinbox_z.value);
	 //_node.call(properties[slot_idx][4], vv);
	 ignore_changes(true);
	 _undo_redo->create_action("MMGD: value changed");
	 _undo_redo->add_do_method(_node, properties[slot_idx][4], vv);
	 _undo_redo->add_undo_method(_node, properties[slot_idx][4], _node.call(properties[slot_idx][3]));
	 _undo_redo->commit_action();
	 ignore_changes(false);
 }

 void MMGraphNode::on_int_universal_spinbox_value_changed(const float val, const Variant &slot_idx) {
	 //properties[slot_idx][6].set_default_value(int(val));
	 ignore_changes(true);
	 _undo_redo->create_action("MMGD: value changed");
	 _undo_redo->add_do_method(properties[slot_idx][6], "set_default_value", int(val));
	 _undo_redo->add_undo_method(properties[slot_idx][6], "set_default_value", properties[slot_idx][6].get_default_value());
	 _undo_redo->commit_action();
	 ignore_changes(false);
 }

 void MMGraphNode::on_float_universal_spinbox_value_changed(const float val, const Variant &slot_idx) {
	 //properties[slot_idx][6].set_default_value(val);
	 ignore_changes(true);
	 _undo_redo->create_action("MMGD: value changed");
	 _undo_redo->add_do_method(properties[slot_idx][6], "set_default_value", val);
	 _undo_redo->add_undo_method(properties[slot_idx][6], "set_default_value", properties[slot_idx][6].get_default_value());
	 _undo_redo->commit_action();
	 ignore_changes(false);
 }

 void MMGraphNode::on_vector2_universal_spinbox_value_changed(const float val, const Variant &slot_idx, const Variant &spinbox_x, const Variant &spinbox_y) {
	 Vector2 vv = Vector2(spinbox_x.value, spinbox_y.value);
	 //properties[slot_idx][6].set_default_value(vv);
	 ignore_changes(true);
	 _undo_redo->create_action("MMGD: value changed");
	 _undo_redo->add_do_method(properties[slot_idx][6], "set_default_value", vv);
	 _undo_redo->add_undo_method(properties[slot_idx][6], "set_default_value", properties[slot_idx][6].get_default_value());
	 _undo_redo->commit_action();
	 ignore_changes(false);
 }

 void MMGraphNode::on_slot_enum_item_selected(const int val, const int slot_idx) {
	 //_node.call(properties[slot_idx][4], val);
	 ignore_changes(true);
	 _undo_redo->create_action("MMGD: value changed");
	 _undo_redo->add_do_method(_node, properties[slot_idx][4], val);
	 _undo_redo->add_undo_method(_node, properties[slot_idx][4], _node.call(properties[slot_idx][3]));
	 _undo_redo->commit_action();
	 ignore_changes(false);
 }

 void MMGraphNode::on_universal_texture_changed(const int slot_idx) {
	 ignore_changes(true);
	 Ref<Image> img = properties[slot_idx][6].get_active_image();
	 Ref<ImageTexture> tex = properties[slot_idx][5].texture;

	 if (img) {
		 properties[slot_idx][5].texture.create_from_image(img, 0);
	 }
	 properties[slot_idx][5].texture = ImageTexture.new();
 }

 ignore_changes(false);
 }

 void MMGraphNode::on_universal_texture_changed_image_picker(const int slot_idx) {
	 ignore_changes(true);
	 Ref<Image> img = properties[slot_idx][6].get_active_image();
	 Ref<ImageTexture> tex = properties[slot_idx][5].texture_normal;

	 if (img.is_valid()) {
		 properties[slot_idx][5].texture_normal.create_from_image(img, 0);
	 } else {
	 }

	 ignore_changes(false);
 }

 void MMGraphNode::on_slot_line_edit_text_entered(const String &text, const int slot_idx) {
	 //_node.call(properties[slot_idx][4], text);
	 ignore_changes(true);
	 _undo_redo->create_action("MMGD: value changed");
	 _undo_redo->add_do_method(_node, properties[slot_idx][4], text);
	 _undo_redo->add_undo_method(_node, properties[slot_idx][4], _node.call(properties[slot_idx][3]));
	 _undo_redo->commit_action();
	 ignore_changes(false);
 }

 void MMGraphNode::on_universal_color_changed(const Color &c, const int slot_idx) {
	 //properties[slot_idx][6].set_default_value(c);
	 ignore_changes(true);
	 _undo_redo->create_action("MMGD: value changed");
	 _undo_redo->add_do_method(properties[slot_idx][6], "set_default_value", c);
	 _undo_redo->add_undo_method(properties[slot_idx][6], "set_default_value", properties[slot_idx][6].get_default_value());
	 _undo_redo->commit_action();
	 ignore_changes(false);
 }

 void MMGraphNode::on_universal_image_path_changed(const String &f, const int slot_idx) {
	 _node.call(properties[slot_idx][8], f);
	 ignore_changes(true);
	 _undo_redo->create_action("MMGD: value changed");
	 _undo_redo->add_do_method(properties[slot_idx][6], "set_default_value", f);
	 _undo_redo->add_undo_method(properties[slot_idx][6], "set_default_value", properties[slot_idx][6].get_default_value());
	 _undo_redo->commit_action();
	 ignore_changes(false);
 }

 void MMGraphNode::on_close_request() {
	 Node *n = get_parent();

	 while (n) {
		 if (n.has_method("on_graph_node_close_request")) {
			 n.call_deferred("on_graph_node_close_request", self);
			 return;
		 }

		 n = n.get_parent();
	 }
 }

 Color MMGraphNode::get_slot_color(const int slot_type) {
	 return _get_slot_color(slot_type);
 }

 Color MMGraphNode::_get_slot_color(const int slot_type) {
	 if (slot_type == 0) {
		 return Color(0.91, 0.06, 0.06);
	 } else if (slot_type == 1) {
		 return Color(0.43, 0.04, 0.04);
	 } else if (slot_type == 2) {
		 return Color(0.83, 0.38, 0.38);
	 } else if (slot_type == 3) {
		 return Color(0.04, 0.48, 0.43);
	 } else if (slot_type == 4) {
		 return Color(0.35, 0.04, 0.34);
	 } else if (slot_type == 5) {
		 return Color(0.04, 0.05, 1);
	 } else if (slot_type == 6) {
		 return Color(0.37, 0.37, 0.37);
	 }

	 return Color(1, 1, 1, 1);
 }

 MMGraphNode::MMGraphNode() {
	 //var gradient_editor_scene : PackedScene = preload("res://addons/mat_maker_gd/widgets/gradient_editor/gradient_editor.tscn");
	 //var polygon_edit_scene : PackedScene = preload("res://addons/mat_maker_gd/widgets/polygon_edit/polygon_edit.tscn");
	 //var curve_edit_scene : PackedScene = preload("res://addons/mat_maker_gd/widgets/curve_edit/curve_edit.tscn");

	 _editor_node = nullptr;
	 _undo_redo = nullptr;
	 _ignore_change_event = false;
 }

 MMGraphNode::~MMGraphNode() {
 }

 void MMGraphNode::_bind_methods() {
	 ClassDB::bind_method(D_METHOD("get_gradient_editor_scene"), &MMGraphNode::get_gradient_editor_scene);
	 ClassDB::bind_method(D_METHOD("set_gradient_editor_scene", "value"), &MMGraphNode::set_gradient_editor_scene);
	 ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "gradient_editor_scene", PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"), "set_gradient_editor_scene", "get_gradient_editor_scene");

	 ClassDB::bind_method(D_METHOD("get_polygon_edit_scene"), &MMGraphNode::get_polygon_edit_scene);
	 ClassDB::bind_method(D_METHOD("set_polygon_edit_scene", "value"), &MMGraphNode::set_polygon_edit_scene);
	 ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "polygon_edit_scene", PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"), "set_polygon_edit_scene", "get_polygon_edit_scene");

	 ClassDB::bind_method(D_METHOD("get_curve_edit_scene"), &MMGraphNode::get_curve_edit_scene);
	 ClassDB::bind_method(D_METHOD("set_curve_edit_scene", "value"), &MMGraphNode::set_curve_edit_scene);
	 ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "curve_edit_scene", PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"), "set_curve_edit_scene", "get_curve_edit_scene");

	 ClassDB::bind_method(D_METHOD("get_material"), &MMGraphNode::get_material);
	 ClassDB::bind_method(D_METHOD("set_material", "value"), &MMGraphNode::set_material);
	 ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "material", PROPERTY_HINT_RESOURCE_TYPE, "MMMaterial"), "set_material", "get_material");

	 ClassDB::bind_method(D_METHOD("get_material_node"), &MMGraphNode::get_material_node);

	 ClassDB::bind_method(D_METHOD("get_node"), &MMGraphNode::get_node);
	 ClassDB::bind_method(D_METHOD("set_node", "value"), &MMGraphNode::set_node);
	 ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*_node", PROPERTY_HINT_RESOURCE_TYPE, "MMNode"), "set_node", "get_node");

	 ClassDB::bind_method(D_METHOD("get_properties"), &MMGraphNode::get_properties);
	 ClassDB::bind_method(D_METHOD("set_properties", "value"), &MMGraphNode::set_properties);
	 ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "properties"), "set_properties", "get_properties");

	 ClassDB::bind_method(D_METHOD("get_Variant"), &MMGraphNode::get_Variant);
	 ClassDB::bind_method(D_METHOD("set_Variant", "value"), &MMGraphNode::set_Variant);
	 ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "Variant", PROPERTY_HINT_RESOURCE_TYPE, "Variant"), "set_Variant", "get_Variant");

	 ClassDB::bind_method(D_METHOD("get_ignore_change_event"), &MMGraphNode::get_ignore_change_event);
	 ClassDB::bind_method(D_METHOD("set_ignore_change_event", "value"), &MMGraphNode::set_ignore_change_event);
	 ADD_PROPERTY(PropertyInfo(Variant::BOOL, "_ignore_change_event"), "set_ignore_change_event", "get_ignore_change_event");

	 ClassDB::bind_method(D_METHOD("_init"), &MMGraphNode::_init);

	 ClassDB::bind_method(D_METHOD("set_editor", "editor_node"), &MMGraphNode::set_editor);
	 ClassDB::bind_method(D_METHOD("ignore_changes", "val"), &MMGraphNode::ignore_changes);

	 ClassDB::bind_method(D_METHOD("add_slot_texture", "getter", "setter"), &MMGraphNode::add_slot_texture);
	 ClassDB::bind_method(D_METHOD("add_slot_texture_universal", "property"), &MMGraphNode::add_slot_texture_universal);
	 ClassDB::bind_method(D_METHOD("add_slot_image_path_universal", "property", "getter", "setter"), &MMGraphNode::add_slot_image_path_universal);
	 ClassDB::bind_method(D_METHOD("add_slot_gradient"), &MMGraphNode::add_slot_gradient);
	 ClassDB::bind_method(D_METHOD("add_slot_polygon"), &MMGraphNode::add_slot_polygon);
	 ClassDB::bind_method(D_METHOD("add_slot_curve"), &MMGraphNode::add_slot_curve);
	 ClassDB::bind_method(D_METHOD("add_slot_color", "getter", "setter"), &MMGraphNode::add_slot_color);
	 ClassDB::bind_method(D_METHOD("add_slot_color_universal", "property"), &MMGraphNode::add_slot_color_universal);
	 ClassDB::bind_method(D_METHOD("add_slot_label", "getter", "setter", "slot_name"), &MMGraphNode::add_slot_label);
	 ClassDB::bind_method(D_METHOD("add_slot_line_edit", "getter", "setter", "slot_name", "placeholder"), &MMGraphNode::add_slot_line_edit, String());
	 ClassDB::bind_method(D_METHOD("add_slot_enum", "getter", "setter", "slot_name", "values"), &MMGraphNode::add_slot_enum);
	 ClassDB::bind_method(D_METHOD("add_slot_int", "getter", "setter", "slot_name", "prange"), &MMGraphNode::add_slot_int, Vector2(-1000, -1000));
	 ClassDB::bind_method(D_METHOD("add_slot_bool", "getter", "setter", "slot_name"), &MMGraphNode::add_slot_bool);
	 ClassDB::bind_method(D_METHOD("add_slot_label_universal", "property"), &MMGraphNode::add_slot_label_universal);
	 ClassDB::bind_method(D_METHOD("add_slot_int_universal", "property"), &MMGraphNode::add_slot_int_universal);
	 ClassDB::bind_method(D_METHOD("add_slot_float", "getter", "setter", "slot_name", "step", "prange", " 1000)"), &MMGraphNode::add_slot_float, 0.1, Vector2(-1000));
	 ClassDB::bind_method(D_METHOD("add_slot_float_universal", "property"), &MMGraphNode::add_slot_float_universal);
	 ClassDB::bind_method(D_METHOD("add_slot_vector2", "getter", "setter", "slot_name", "step", "prange", " 1000)"), &MMGraphNode::add_slot_vector2, 0.1, Vector2(-1000, -1000));
	 ClassDB::bind_method(D_METHOD("add_slot_vector3", "getter", "setter", "slot_name", "step", "prange", " 1000)"), &MMGraphNode::add_slot_vector3, 0.1, Vector2(-1000, -1000));
	 ClassDB::bind_method(D_METHOD("add_slot_vector2_universal", "property"), &MMGraphNode::add_slot_vector2_universal);
	 ClassDB::bind_method(D_METHOD("add_slot", "input_type", "output_type", "getter", "setter", "control"), &MMGraphNode::add_slot);

	 ClassDB::bind_method(D_METHOD("connect_slot", "slot_idx", "to_node", "to_slot_idx"), &MMGraphNode::connect_slot);
	 ClassDB::bind_method(D_METHOD("disconnect_slot", "slot_idx", "to_node", "to_slot_idx"), &MMGraphNode::disconnect_slot);

	 ClassDB::bind_method(D_METHOD("get_input_property_graph_node_slot_index", "property"), &MMGraphNode::get_input_property_graph_node_slot_index);
	 ClassDB::bind_method(D_METHOD("get_output_property_graph_node_slot_index", "property"), &MMGraphNode::get_output_property_graph_node_slot_index);

	 ClassDB::bind_method(D_METHOD("get_property_control", "slot_idx"), &MMGraphNode::get_property_control);

	 ClassDB::bind_method(D_METHOD("set_node", "material", "node"), &MMGraphNode::set_node);

	 ClassDB::bind_method(D_METHOD("propagate_node_change"), &MMGraphNode::propagate_node_change);

	 ClassDB::bind_method(D_METHOD("on_dragged", "from", "to"), &MMGraphNode::on_dragged);
	 ClassDB::bind_method(D_METHOD("on_int_spinbox_value_changed", "val", " slot_idx"), &MMGraphNode::on_int_spinbox_value_changed);
	 ClassDB::bind_method(D_METHOD("on_float_spinbox_value_changed", "val", " slot_idx"), &MMGraphNode::on_float_spinbox_value_changed);
	 ClassDB::bind_method(D_METHOD("on_vector2_spinbox_value_changed", "val", " slot_idx", " spinbox_x", " spinbox_y"), &MMGraphNode::on_vector2_spinbox_value_changed);
	 ClassDB::bind_method(D_METHOD("on_vector3_spinbox_value_changed", "val", " slot_idx", " spinbox_x", " spinbox_y", " spinbox_z"), &MMGraphNode::on_vector3_spinbox_value_changed);
	 ClassDB::bind_method(D_METHOD("on_int_universal_spinbox_value_changed", "val", " slot_idx"), &MMGraphNode::on_int_universal_spinbox_value_changed);
	 ClassDB::bind_method(D_METHOD("on_float_universal_spinbox_value_changed", "val", " slot_idx"), &MMGraphNode::on_float_universal_spinbox_value_changed);
	 ClassDB::bind_method(D_METHOD("on_vector2_universal_spinbox_value_changed", "val", " slot_idx", " spinbox_x", " spinbox_y"), &MMGraphNode::on_vector2_universal_spinbox_value_changed);
	 ClassDB::bind_method(D_METHOD("on_slot_enum_item_selected", "val", "slot_idx"), &MMGraphNode::on_slot_enum_item_selected);
	 ClassDB::bind_method(D_METHOD("on_universal_texture_changed", "slot_idx"), &MMGraphNode::on_universal_texture_changed);
	 ClassDB::bind_method(D_METHOD("on_universal_texture_changed_image_picker", "slot_idx"), &MMGraphNode::on_universal_texture_changed_image_picker);
	 ClassDB::bind_method(D_METHOD("on_slot_line_edit_text_entered", "text", "slot_idx"), &MMGraphNode::on_slot_line_edit_text_entered);
	 ClassDB::bind_method(D_METHOD("on_universal_color_changed", "c", "slot_idx"), &MMGraphNode::on_universal_color_changed);
	 ClassDB::bind_method(D_METHOD("on_universal_image_path_changed", "f", "slot_idx"), &MMGraphNode::on_universal_image_path_changed);
	 ClassDB::bind_method(D_METHOD("on_close_request"), &MMGraphNode::on_close_request);

	 ClassDB::bind_method(D_METHOD("get_slot_color", "slot_type"), &MMGraphNode::get_slot_color);
	 ClassDB::bind_method(D_METHOD("_get_slot_color", "slot_type"), &MMGraphNode::_get_slot_color);
 }
