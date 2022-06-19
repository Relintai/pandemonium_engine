
#include "mm_graph_node.h"

#include "../nodes/bases/curve_base.h"
#include "../nodes/bases/gradient_base.h"
#include "../nodes/bases/polygon_base.h"
#include "../nodes/mm_material.h"
#include "../nodes/mm_node.h"
#include "../nodes/mm_node_universal_property.h"
#include "mat_maker_gd_editor.h"
#include "scene/resources/packed_scene.h"

#include "scene/gui/check_box.h"
#include "scene/gui/color_picker.h"
#include "scene/gui/label.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/option_button.h"
#include "scene/gui/spin_box.h"
#include "scene/gui/texture_rect.h"

#include "widgets/curve_edit/curve_edit.h"
#include "widgets/gradient_editor/gradient_editor.h"
#include "widgets/image_picker_button/image_picker_button.h"
#include "widgets/polygon_edit/polygon_edit.h"

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

Ref<MMNode> MMGraphNode::get_mm_node() {
	return _node;
}

void MMGraphNode::set_mm_node(const Ref<MMNode> &val) {
	_node = val;
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
	properties.write[slot_idx].texture = t->get_texture();
	return slot_idx;
}

int MMGraphNode::add_slot_texture_universal(const Ref<MMNodeUniversalProperty> &p_property) {
	Ref<MMNodeUniversalProperty> property = p_property;

	TextureRect *t = memnew(TextureRect);
	t->set_custom_minimum_size(Vector2(128, 128));
	t->set_expand(true);
	t->set_stretch_mode(TextureRect::STRETCH_KEEP_ASPECT_CENTERED);
	int slot_idx = add_slot(property->get_input_slot_type(), property->get_output_slot_type(), "", "", t);
	Ref<Image> img = property->get_active_image();
	Ref<ImageTexture> tex;
	tex.instance();

	if (img.is_valid()) {
		tex->create_from_image(img, 0);
	}

	t->set_texture(tex);
	properties.write[slot_idx].universal_property = property;

	Vector<Variant> bindings;
	bindings.push_back(slot_idx);
	property->connect("changed", this, "on_universal_texture_changed", bindings);
	return slot_idx;
}

int MMGraphNode::add_slot_image_path_universal(const Ref<MMNodeUniversalProperty> &property, const String &getter, const String &setter) {
	ImagePickerButton *t = memnew(ImagePickerButton);
	int slot_idx = add_slot(property->get_input_slot_type(), property->get_output_slot_type(), "", "", t);

	properties.write[slot_idx].universal_property = property;
	properties.write[slot_idx].alt_getter = getter;
	properties.write[slot_idx].alt_setter = setter;

	Vector<Variant> bindings;
	bindings.push_back(slot_idx);

	Ref<MMNodeUniversalProperty> prop = property;

	prop->connect("changed", this, "on_universal_texture_changed_image_picker", bindings);
	t->connect("on_file_selected", this, "on_universal_image_path_changed", bindings);
	t->call_deferred("do_set_image_path", _node->call(getter));
	return slot_idx;
}

int MMGraphNode::add_slot_gradient() {
	ERR_FAIL_COND_V(!_node->is_class("GradientBase"), 0);

	MMGradientEditor *ge = memnew(MMGradientEditor);
	ge->set_graph_node(this);
	ge->set_undo_redo(_undo_redo);
	int slot_idx = add_slot(MMNodeUniversalProperty::SLOT_TYPE_NONE, MMNodeUniversalProperty::SLOT_TYPE_NONE, "", "", ge);
	ge->set_value(_node);
	//ge.texture = _node.call(getter, _material, slot_idx);
	//properties[slot_idx].append(ge.texture);
	return slot_idx;
}

int MMGraphNode::add_slot_polygon() {
	ERR_FAIL_COND_V(!_node->is_class("PolygonBase"), 0);

	PolygonEdit *pe = memnew(PolygonEdit);
	int slot_idx = add_slot(MMNodeUniversalProperty::SLOT_TYPE_NONE, MMNodeUniversalProperty::SLOT_TYPE_NONE, "", "", pe);
	pe->set_value(_node);
	//ge.texture = _node.call(getter, _material, slot_idx);
	//properties[slot_idx].append(ge.texture);
	return slot_idx;
}

int MMGraphNode::add_slot_curve() {
	ERR_FAIL_COND_V(!_node->is_class("CurveBase"), 0);

	CurveEdit *ce = memnew(CurveEdit);
	int slot_idx = add_slot(MMNodeUniversalProperty::SLOT_TYPE_NONE, MMNodeUniversalProperty::SLOT_TYPE_NONE, "", "", ce);
	ce->set_value(_node);
	//ge.texture = _node.call(getter, _material, slot_idx);
	//properties[slot_idx].append(ge.texture);
	return slot_idx;
}

int MMGraphNode::add_slot_color(const String &getter, const String &setter) {
	ColorPickerButton *cp = memnew(ColorPickerButton);
	int slot_idx = add_slot(MMNodeUniversalProperty::SLOT_TYPE_NONE, MMNodeUniversalProperty::SLOT_TYPE_NONE, getter, setter, cp);
	cp->set_pick_color(_node->call(getter));
	Vector<Variant> bindings;
	bindings.push_back(slot_idx);
	cp->connect("popup_closed", this, "on_color_picker_popup_closed", bindings);
	return slot_idx;
}

int MMGraphNode::add_slot_color_universal(const Ref<MMNodeUniversalProperty> &property) {
	ColorPickerButton *cp = memnew(ColorPickerButton);
	int slot_idx = add_slot(property->get_input_slot_type(), property->get_output_slot_type(), "", "", cp);
	cp->set_pick_color(property->get_default_value_const());
	properties.write[slot_idx].universal_property = property;
	Vector<Variant> bindings;
	bindings.push_back(slot_idx);
	cp->connect("color_changed", this, "on_universal_color_changed", bindings);
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
	l->set_text(slot_name);
	bc->add_child(l);
	LineEdit *le = memnew(LineEdit);
	le->set_placeholder(placeholder);
	bc->add_child(le);
	int slot_idx = add_slot(MMNodeUniversalProperty::SLOT_TYPE_NONE, MMNodeUniversalProperty::SLOT_TYPE_NONE, getter, setter, bc);
	le->set_text(_node->call(getter));
	Vector<Variant> bindings;
	bindings.push_back(slot_idx);
	le->connect("text_entered", this, "on_slot_line_edit_text_entered", bindings);
	return slot_idx;
}

int MMGraphNode::add_slot_enum(const String &getter, const String &setter, const String &slot_name, const Array &values) {
	VBoxContainer *bc = memnew(VBoxContainer);

	if (slot_name != "") {
		Label *l = memnew(Label);
		l->set_text(slot_name);
		bc->add_child(l);
	}

	OptionButton *mb = memnew(OptionButton);

	for (int i = 0; i < values.size(); ++i) {
		String v = values[i];
		mb->add_item(v);
	}

	bc->add_child(mb);
	int slot_idx = add_slot(MMNodeUniversalProperty::SLOT_TYPE_NONE, MMNodeUniversalProperty::SLOT_TYPE_NONE, getter, setter, bc);
	mb->select(_node->call(getter));
	Vector<Variant> bindings;
	bindings.push_back(slot_idx);
	mb->connect("item_selected", this, "on_slot_enum_item_selected", bindings);
	return slot_idx;
}

int MMGraphNode::add_slot_int(const String &getter, const String &setter, const String &slot_name, const Vector2 &prange) {
	VBoxContainer *bc = memnew(VBoxContainer);
	Label *l = memnew(Label);
	l->set_text(slot_name);
	bc->add_child(l);
	SpinBox *sb = memnew(SpinBox);
	sb->set_use_rounded_values(true);
	sb->set_min(prange.x);
	sb->set_max(prange.y);
	bc->add_child(sb);
	int slot_idx = add_slot(MMNodeUniversalProperty::SLOT_TYPE_NONE, MMNodeUniversalProperty::SLOT_TYPE_NONE, getter, setter, bc);
	sb->set_value(_node->call(getter));
	Vector<Variant> bindings;
	bindings.push_back(slot_idx);
	sb->connect("value_changed", this, "on_int_spinbox_value_changed", bindings);
	return slot_idx;
}

int MMGraphNode::add_slot_bool(const String &getter, const String &setter, const String &slot_name) {
	CheckBox *cb = memnew(CheckBox);
	cb->set_text(slot_name);
	int slot_idx = add_slot(MMNodeUniversalProperty::SLOT_TYPE_NONE, MMNodeUniversalProperty::SLOT_TYPE_NONE, getter, setter, cb);
	cb->set_pressed(_node->call(getter));
	cb->connect("toggled", *_node, setter);
	return slot_idx;
}

int MMGraphNode::add_slot_label_universal(const Ref<MMNodeUniversalProperty> &property) {
	Label *l = memnew(Label);
	l->set_text(property->get_slot_name());
	int slot_idx = add_slot(property->get_input_slot_type(), property->get_output_slot_type(), "", "", l);
	properties.write[slot_idx].universal_property = property;
	return slot_idx;
}

int MMGraphNode::add_slot_int_universal(const Ref<MMNodeUniversalProperty> &property) {
	VBoxContainer *bc = memnew(VBoxContainer);
	Label *l = memnew(Label);
	l->set_text(property->get_slot_name());
	bc->add_child(l);
	SpinBox *sb = memnew(SpinBox);
	sb->set_use_rounded_values(true);
	sb->set_min(property->get_value_range().x);
	sb->set_max(property->get_value_range().y);
	bc->add_child(sb);
	int slot_idx = add_slot(property->get_input_slot_type(), property->get_output_slot_type(), "", "", bc);
	sb->set_value(property->get_default_value_const());
	Vector<Variant> bindings;
	bindings.push_back(slot_idx);
	sb->connect("value_changed", this, "on_int_universal_spinbox_value_changed", bindings);
	properties.write[slot_idx].universal_property = property;
	return slot_idx;
}

int MMGraphNode::add_slot_float(const String &getter, const String &setter, const String &slot_name, const float step, const Vector2 &prange) {
	VBoxContainer *bc = memnew(VBoxContainer);
	Label *l = memnew(Label);
	l->set_text(slot_name);
	bc->add_child(l);
	SpinBox *sb = memnew(SpinBox);
	bc->add_child(sb);
	int slot_idx = add_slot(MMNodeUniversalProperty::SLOT_TYPE_NONE, MMNodeUniversalProperty::SLOT_TYPE_NONE, getter, setter, bc);
	sb->set_use_rounded_values(false);
	sb->set_step(step);
	sb->set_min(prange.x);
	sb->set_max(prange.y);
	sb->set_value(_node->call(getter));
	Vector<Variant> bindings;
	bindings.push_back(slot_idx);
	sb->connect("value_changed", this, "on_float_spinbox_value_changed", bindings);
	return slot_idx;
}

int MMGraphNode::add_slot_float_universal(const Ref<MMNodeUniversalProperty> &property) {
	VBoxContainer *bc = memnew(VBoxContainer);
	Label *l = memnew(Label);
	l->set_text(property->get_slot_name());
	bc->add_child(l);
	SpinBox *sb = memnew(SpinBox);
	bc->add_child(sb);
	int slot_idx = add_slot(property->get_input_slot_type(), property->get_output_slot_type(), "", "", bc);
	sb->set_use_rounded_values(false);
	sb->set_step(property->get_value_step());
	sb->set_min(property->get_value_range().x);
	sb->set_max(property->get_value_range().y);
	sb->set_value(property->get_default_value_const());
	properties.write[slot_idx].universal_property = property;
	Vector<Variant> bindings;
	bindings.push_back(slot_idx);
	sb->connect("value_changed", this, "on_float_universal_spinbox_value_changed", bindings);
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
	sbx->set_use_rounded_values(false);
	sby->set_use_rounded_values(false);
	sbx->set_step(step);
	sby->set_step(step);
	sbx->set_min(prange.x);
	sbx->set_max(prange.y);
	sby->set_min(prange.x);
	sby->set_max(prange.y);
	Vector2 val = _node->call(getter);
	sbx->set_value(val.x);
	sby->set_value(val.y);
	Vector<Variant> bindings;
	bindings.push_back(slot_idx);
	bindings.push_back(sbx);
	bindings.push_back(sby);
	sbx->connect("value_changed", this, "on_vector2_spinbox_value_changed", bindings);
	sby->connect("value_changed", this, "on_vector2_spinbox_value_changed", bindings);
	return slot_idx;
}

int MMGraphNode::add_slot_vector3(const String &getter, const String &setter, const String &slot_name, const float step, const Vector2 &prange) {
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
	sbx->set_use_rounded_values(false);
	sby->set_use_rounded_values(false);
	sbz->set_use_rounded_values(false);
	sbx->set_step(step);
	sby->set_step(step);
	sbz->set_step(step);
	sbx->set_min(prange.x);
	sbx->set_max(prange.y);
	sby->set_min(prange.x);
	sby->set_max(prange.y);
	sbz->set_min(prange.x);
	sbz->set_max(prange.y);
	Vector3 val = _node->call(getter);
	sbx->set_value(val.x);
	sby->set_value(val.y);
	sbz->set_value(val.z);

	Vector<Variant> bindings;
	bindings.push_back(slot_idx);
	bindings.push_back(sbx);
	bindings.push_back(sby);
	bindings.push_back(sbz);
	sbx->connect("value_changed", this, "on_vector3_spinbox_value_changed", bindings);
	sby->connect("value_changed", this, "on_vector3_spinbox_value_changed", bindings);
	sbz->connect("value_changed", this, "on_vector3_spinbox_value_changed", bindings);
	return slot_idx;
}

int MMGraphNode::add_slot_vector2_universal(const Ref<MMNodeUniversalProperty> &property) {
	VBoxContainer *bc = memnew(VBoxContainer);
	Label *l = memnew(Label);
	l->set_text(property->get_slot_name());
	bc->add_child(l);
	SpinBox *sbx = memnew(SpinBox);
	bc->add_child(sbx);
	SpinBox *sby = memnew(SpinBox);
	bc->add_child(sby);
	int slot_idx = add_slot(property->get_input_slot_type(), property->get_output_slot_type(), "", "", bc);
	sbx->set_use_rounded_values(false);
	sby->set_use_rounded_values(false);
	sbx->set_step(property->get_value_step());
	sby->set_step(property->get_value_step());
	sbx->set_min(property->get_value_range().x);
	sbx->set_max(property->get_value_range().y);
	sby->set_min(property->get_value_range().x);
	sby->set_max(property->get_value_range().y);
	Vector2 val = property->get_default_value_const();
	sbx->set_value(val.x);
	sby->set_value(val.y);
	properties.write[slot_idx].universal_property = property;

	Vector<Variant> bindings;
	bindings.push_back(slot_idx);
	bindings.push_back(sbx);
	bindings.push_back(sby);
	sbx->connect("value_changed", this, "on_vector2_universal_spinbox_value_changed", bindings);
	sby->connect("value_changed", this, "on_vector2_universal_spinbox_value_changed", bindings);
	return slot_idx;
}

int MMGraphNode::add_slot(const int input_type, const int output_type, const String &getter, const String &setter, Control *control) {
	add_child(control);
	int slot_idx = get_child_count() - 1;

	MMGraphNodeEntry e;
	e.slot_idx = slot_idx;
	e.input_type = input_type;
	e.output_type = output_type;
	e.getter = getter;
	e.setter = setter;
	e.control = control;
	properties.push_back(e);

	/*
	  Array arr;
	  arr.append(slot_idx);
	  arr.append(input_type);
	  arr.append(output_type);
	  arr.append(getter);
	  arr.append(setter);
	  arr.append(control);
	  properties.append(arr);
	*/

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

bool MMGraphNode::connect_slot(const int slot_idx, Node *p_to_node, const int to_slot_idx) {
	MMGraphNode *to_node = Object::cast_to<MMGraphNode>(p_to_node);
	ERR_FAIL_COND_V(!to_node, false);

	int from_property_index = -1;
	int to_property_index = -1;

	for (int i = 0; i < properties.size(); ++i) { //i in range(properties.size())
		if (properties[i].output_type != -1) {
			from_property_index += 1;

			if (from_property_index == slot_idx) {
				from_property_index = i;
				break;
			}
		}
	}

	for (int i = 0; i < to_node->properties.size(); ++i) { //i in range(to_node.properties.size())
		if (to_node->properties[i].input_type != -1) {
			to_property_index += 1;

			if (to_property_index == to_slot_idx) {
				to_property_index = i;
				break;
			}

			//to_node.properties[to_property_index][6].set_input_property(properties[from_property_index][6]);
		}
	}

	_undo_redo->create_action("MMGD: connect_slot");
	_undo_redo->add_do_method(*(to_node->properties.write[to_property_index].universal_property), "set_input_property", *(properties.write[from_property_index].universal_property));
	_undo_redo->add_undo_method(*(to_node->properties.write[to_property_index].universal_property), "set_input_property", *(to_node->properties.write[to_property_index].universal_property->get_input_property()));
	_undo_redo->commit_action();
	return true;
}

bool MMGraphNode::disconnect_slot(const int slot_idx, Node *p_to_node, const int to_slot_idx) {
	MMGraphNode *to_node = Object::cast_to<MMGraphNode>(p_to_node);
	ERR_FAIL_COND_V(!to_node, false);

	int from_property_index = -1;
	int to_property_index = -1;

	for (int i = 0; i < properties.size(); ++i) { //i in range(properties.size())

		if (properties[i].output_type != -1) {
			from_property_index += 1;

			if (from_property_index == slot_idx) {
				from_property_index = i;
				break;
			}
		}
	}

	for (int i = 0; i < to_node->properties.size(); ++i) { //i in range(to_node.properties.size())

		if (to_node->properties[i].input_type != -1) {
			to_property_index += 1;

			if (to_property_index == to_slot_idx) {
				to_property_index = i;
				break;
			}

			//to_node.properties[to_property_index][6].set_input_property(null);
		}
	}

	_undo_redo->create_action("MMGD: disconnect_slot");
	_undo_redo->add_do_method(*(to_node->properties.write[to_property_index].universal_property), "unset_input_property");
	_undo_redo->add_undo_method(*(to_node->properties.write[to_property_index].universal_property), "set_input_property", *(to_node->properties.write[to_property_index].universal_property->get_input_property()));
	_undo_redo->commit_action();
	return true;
}

int MMGraphNode::get_input_property_graph_node_slot_index(const Variant &property) {
	int property_index = -1;

	for (int i = 0; i < properties.size(); ++i) { //i in range(properties.size())

		if (properties[i].input_type != -1) {
			property_index += 1;

			if (properties[i].universal_property == property) {
				break;
			}
		}
	}

	return property_index;
}

int MMGraphNode::get_output_property_graph_node_slot_index(const Variant &property) {
	int property_index = -1;

	for (int i = 0; i < properties.size(); ++i) { //i in range(properties.size())

		if (properties[i].output_type != -1) {
			property_index += 1;

			if (properties[i].universal_property == property) {
				break;
			}
		}
	}

	return property_index;
}

Control *MMGraphNode::get_property_control(const int slot_idx) {
	return properties[slot_idx].control;
}

void MMGraphNode::set_node(const Ref<MMMaterial> &material, const Ref<MMNode> &node) {
	_node = node;
	_material = material;

	if (!_node.is_valid()) {
		return;
	}

	set_title(_node->get_class());

	Ref<Script> script = _node->get_script();
	if (script.is_valid()) {
		set_title(script->get_path().get_file().get_basename());
	}

	_node->register_methods(this);
	set_offset(_node->get_graph_position());

	//_node.connect("changed", self, "on_node_changed");
}

void MMGraphNode::propagate_node_change() {
}

void MMGraphNode::on_dragged(const Vector2 &from, const Vector2 &to) {
	if (_node.is_valid()) {
		ignore_changes(true);
		//_node.set_graph_position(offset);
		_undo_redo->create_action("MMGD: value changed");
		_undo_redo->add_do_method(_node.ptr(), "set_graph_position", to);
		_undo_redo->add_undo_method(_node.ptr(), "set_graph_position", from);
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

void MMGraphNode::on_int_spinbox_value_changed(const float val, const int slot_idx) {
	//_node.call(properties[slot_idx][4], int(val));
	ignore_changes(true);
	_undo_redo->create_action("MMGD: value changed");
	_undo_redo->add_do_method(*_node, properties[slot_idx].setter, int(val));
	_undo_redo->add_undo_method(*_node, properties[slot_idx].setter, _node->call(properties[slot_idx].getter));
	_undo_redo->commit_action();
	ignore_changes(false);
}

void MMGraphNode::on_float_spinbox_value_changed(const float val, const int slot_idx) {
	//_node.call(properties[slot_idx][4], val);
	ignore_changes(true);
	_undo_redo->create_action("MMGD: value changed");
	_undo_redo->add_do_method(*_node, properties[slot_idx].setter, val);
	_undo_redo->add_undo_method(*_node, properties[slot_idx].setter, _node->call(properties[slot_idx].getter));
	_undo_redo->commit_action();
	ignore_changes(false);
}

void MMGraphNode::on_vector2_spinbox_value_changed(const float val, const int slot_idx, Node *p_spinbox_x, Node *p_spinbox_y) {
	SpinBox *spinbox_x = Object::cast_to<SpinBox>(p_spinbox_x);
	SpinBox *spinbox_y = Object::cast_to<SpinBox>(p_spinbox_y);

	Vector2 vv = Vector2(spinbox_x->get_value(), spinbox_y->get_value());
	//_node.call(properties[slot_idx][4], vv);
	ignore_changes(true);
	_undo_redo->create_action("MMGD: value changed");
	_undo_redo->add_do_method(*_node, properties[slot_idx].setter, vv);
	_undo_redo->add_undo_method(*_node, properties[slot_idx].setter, _node->call(properties[slot_idx].getter));
	_undo_redo->commit_action();
	ignore_changes(false);
}

void MMGraphNode::on_vector3_spinbox_value_changed(const float val, const int slot_idx, Node *p_spinbox_x, Node *p_spinbox_y, Node *p_spinbox_z) {
	SpinBox *spinbox_x = Object::cast_to<SpinBox>(p_spinbox_x);
	SpinBox *spinbox_y = Object::cast_to<SpinBox>(p_spinbox_y);
	SpinBox *spinbox_z = Object::cast_to<SpinBox>(p_spinbox_z);

	Vector3 vv = Vector3(spinbox_x->get_value(), spinbox_y->get_value(), spinbox_z->get_value());
	//_node.call(properties[slot_idx][4], vv);
	ignore_changes(true);
	_undo_redo->create_action("MMGD: value changed");
	_undo_redo->add_do_method(*_node, properties[slot_idx].setter, vv);
	_undo_redo->add_undo_method(*_node, properties[slot_idx].setter, _node->call(properties[slot_idx].getter));
	_undo_redo->commit_action();
	ignore_changes(false);
}

void MMGraphNode::on_int_universal_spinbox_value_changed(const float val, const int slot_idx) {
	//properties[slot_idx][6].set_default_value(int(val));
	ignore_changes(true);
	_undo_redo->create_action("MMGD: value changed");
	_undo_redo->add_do_method(*(properties.write[slot_idx].universal_property), "set_default_value", int(val));
	_undo_redo->add_undo_method(*(properties.write[slot_idx].universal_property), "set_default_value", properties.write[slot_idx].universal_property->get_default_value());
	_undo_redo->commit_action();
	ignore_changes(false);
}

void MMGraphNode::on_float_universal_spinbox_value_changed(const float val, const int slot_idx) {
	//properties[slot_idx][6].set_default_value(val);
	ignore_changes(true);
	_undo_redo->create_action("MMGD: value changed");
	_undo_redo->add_do_method(*(properties.write[slot_idx].universal_property), "set_default_value", val);
	_undo_redo->add_undo_method(*(properties.write[slot_idx].universal_property), "set_default_value", properties.write[slot_idx].universal_property->get_default_value());
	_undo_redo->commit_action();
	ignore_changes(false);
}

void MMGraphNode::on_vector2_universal_spinbox_value_changed(const float val, const int slot_idx, Node *p_spinbox_x, Node *p_spinbox_y) {
	SpinBox *spinbox_x = Object::cast_to<SpinBox>(p_spinbox_x);
	SpinBox *spinbox_y = Object::cast_to<SpinBox>(p_spinbox_y);

	Vector2 vv = Vector2(spinbox_x->get_value(), spinbox_y->get_value());
	//properties[slot_idx][6].set_default_value(vv);
	ignore_changes(true);
	_undo_redo->create_action("MMGD: value changed");
	_undo_redo->add_do_method(*(properties.write[slot_idx].universal_property), "set_default_value", vv);
	_undo_redo->add_undo_method(*(properties.write[slot_idx].universal_property), "set_default_value", properties.write[slot_idx].universal_property->get_default_value());
	_undo_redo->commit_action();
	ignore_changes(false);
}

void MMGraphNode::on_slot_enum_item_selected(const int val, const int slot_idx) {
	//_node.call(properties[slot_idx][4], val);
	ignore_changes(true);
	_undo_redo->create_action("MMGD: value changed");
	_undo_redo->add_do_method(*_node, properties[slot_idx].setter, val);
	_undo_redo->add_undo_method(*_node, properties[slot_idx].setter, _node->call(properties[slot_idx].getter));
	_undo_redo->commit_action();
	ignore_changes(false);
}

void MMGraphNode::on_universal_texture_changed(const int slot_idx) {
	ignore_changes(true);
	Ref<Image> img = properties.write[slot_idx].universal_property->get_active_image();

	if (img.is_valid()) {
		Ref<ImageTexture> tex = properties.write[slot_idx].control->call("get_texture");
		tex->create_from_image(img, 0);
	} else {
		Ref<ImageTexture> it;
		it.instance();

		properties.write[slot_idx].control->call("set_texture", it);
	}

	ignore_changes(false);
}

void MMGraphNode::on_universal_texture_changed_image_picker(const int slot_idx) {
	ignore_changes(true);
	Ref<Image> img = properties.write[slot_idx].universal_property->get_active_image();

	if (img.is_valid()) {
		Ref<ImageTexture> tex = properties.write[slot_idx].control->call("get_texture_normal");
		tex->create_from_image(img, 0);
	} else {
		Ref<ImageTexture> it;
		it.instance();
		properties.write[slot_idx].control->call("set_texture", it);
	}

	ignore_changes(false);
}

void MMGraphNode::on_slot_line_edit_text_entered(const String &text, const int slot_idx) {
	//_node.call(properties[slot_idx][4], text);
	ignore_changes(true);
	_undo_redo->create_action("MMGD: value changed");
	_undo_redo->add_do_method(*_node, properties[slot_idx].setter, text);
	_undo_redo->add_undo_method(*_node, properties[slot_idx].setter, _node->call(properties[slot_idx].getter));
	_undo_redo->commit_action();
	ignore_changes(false);
}

void MMGraphNode::on_color_picker_popup_closed(const int slot_idx) {
	//properties[slot_idx][6].set_default_value(c);
	ignore_changes(true);
	_undo_redo->create_action("MMGD: value changed");

	ColorPickerButton *btn = Object::cast_to<ColorPickerButton>(properties[slot_idx].control);
	ERR_FAIL_COND(!btn);

	_undo_redo->add_do_method(*_node, properties[slot_idx].setter, btn->get_pick_color());
	_undo_redo->add_undo_method(*_node, properties[slot_idx].setter, _node->call(properties[slot_idx].getter));
	_undo_redo->commit_action();
	ignore_changes(false);
}

void MMGraphNode::on_universal_color_changed(const Color &c, const int slot_idx) {
	//properties[slot_idx][6].set_default_value(c);
	ignore_changes(true);
	_undo_redo->create_action("MMGD: value changed");
	_undo_redo->add_do_method(*(properties.write[slot_idx].universal_property), "set_default_value", c);
	_undo_redo->add_undo_method(*(properties.write[slot_idx].universal_property), "set_default_value", properties.write[slot_idx].universal_property->get_default_value());
	_undo_redo->commit_action();
	ignore_changes(false);
}

void MMGraphNode::on_universal_image_path_changed(const String &f, const int slot_idx) {
	_node->call(properties[slot_idx].alt_setter, f);
	ignore_changes(true);
	_undo_redo->create_action("MMGD: value changed");
	_undo_redo->add_do_method(*(properties.write[slot_idx].universal_property), "set_default_value", f);
	_undo_redo->add_undo_method(*(properties.write[slot_idx].universal_property), "set_default_value", properties.write[slot_idx].universal_property->get_default_value());
	_undo_redo->commit_action();
	ignore_changes(false);
}

void MMGraphNode::on_close_request() {
	Node *n = get_parent();

	while (n) {
		if (n->has_method("on_graph_node_close_request")) {
			n->call_deferred("on_graph_node_close_request", this);
			return;
		}

		n = n->get_parent();
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
void MMGraphNode::_notification(int p_what) {
	if (p_what == NOTIFICATION_POSTINITIALIZE) {
		set_show_close_button(true);

		connect("dragged", this, "on_dragged");
		connect("close_request", this, "on_close_request");
	}
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

	ClassDB::bind_method(D_METHOD("get_mm_material"), &MMGraphNode::get_mm_material);
	ClassDB::bind_method(D_METHOD("set_mm_material", "value"), &MMGraphNode::set_mm_material);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "mm_material", PROPERTY_HINT_RESOURCE_TYPE, "MMMaterial"), "set_mm_material", "get_mm_material");

	ClassDB::bind_method(D_METHOD("get_material_node"), &MMGraphNode::get_material_node);

	ClassDB::bind_method(D_METHOD("get_mm_node"), &MMGraphNode::get_mm_node);
	ClassDB::bind_method(D_METHOD("set_mm_node", "value"), &MMGraphNode::set_mm_node);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "mm_node", PROPERTY_HINT_RESOURCE_TYPE, "MMNode"), "set_mm_node", "get_mm_node");

	ClassDB::bind_method(D_METHOD("get_ignore_change_event"), &MMGraphNode::get_ignore_change_event);
	ClassDB::bind_method(D_METHOD("set_ignore_change_event", "value"), &MMGraphNode::set_ignore_change_event);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "_ignore_change_event"), "set_ignore_change_event", "get_ignore_change_event");

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
	ClassDB::bind_method(D_METHOD("add_slot_int", "getter", "setter", "slot_name", "prange"), &MMGraphNode::add_slot_int, Vector2(-1000, 1000));
	ClassDB::bind_method(D_METHOD("add_slot_bool", "getter", "setter", "slot_name"), &MMGraphNode::add_slot_bool);
	ClassDB::bind_method(D_METHOD("add_slot_label_universal", "property"), &MMGraphNode::add_slot_label_universal);
	ClassDB::bind_method(D_METHOD("add_slot_int_universal", "property"), &MMGraphNode::add_slot_int_universal);
	ClassDB::bind_method(D_METHOD("add_slot_float", "getter", "setter", "slot_name", "step", "prange"), &MMGraphNode::add_slot_float, 0.1, Vector2(-1000, 1000));
	ClassDB::bind_method(D_METHOD("add_slot_float_universal", "property"), &MMGraphNode::add_slot_float_universal);
	ClassDB::bind_method(D_METHOD("add_slot_vector2", "getter", "setter", "slot_name", "step", "prange"), &MMGraphNode::add_slot_vector2, 0.1, Vector2(-1000, 1000));
	ClassDB::bind_method(D_METHOD("add_slot_vector3", "getter", "setter", "slot_name", "step", "prange"), &MMGraphNode::add_slot_vector3, 0.1, Vector2(-1000, 1000));
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
	ClassDB::bind_method(D_METHOD("on_color_picker_popup_closed", "slot_idx"), &MMGraphNode::on_color_picker_popup_closed);
	ClassDB::bind_method(D_METHOD("on_universal_color_changed", "c", "slot_idx"), &MMGraphNode::on_universal_color_changed);
	ClassDB::bind_method(D_METHOD("on_universal_image_path_changed", "f", "slot_idx"), &MMGraphNode::on_universal_image_path_changed);
	ClassDB::bind_method(D_METHOD("on_close_request"), &MMGraphNode::on_close_request);

	ClassDB::bind_method(D_METHOD("get_slot_color", "slot_type"), &MMGraphNode::get_slot_color);
	ClassDB::bind_method(D_METHOD("_get_slot_color", "slot_type"), &MMGraphNode::_get_slot_color);
}
