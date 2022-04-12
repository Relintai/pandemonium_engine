/*
Copyright (c) 2019-2022 Péter Magyar

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

#include "mdr_uv_rect_view.h"

#include "../../nodes/mesh_data_instance.h"
#include "../utilities/mdr_ed_mesh_decompose.h"
#include "editor/editor_node.h"
#include "editor/editor_plugin.h"
#include "mdr_uv_rect_view_node.h"
#include "scene/gui/control.h"

void MDRUVRectView::set_plugin(EditorPlugin *plugin) {
	_plugin = plugin;

	_undo_redo = EditorNode::get_undo_redo();
}
void MDRUVRectView::set_mesh_data_resource(Ref<MeshDataResource> a) {
	_mdr = a;
}
void MDRUVRectView::set_mesh_data_instance(MeshDataInstance *a) {
	_background_texture.unref();

	if (a) {
		_background_texture = a->get_texture();
	}
}
void MDRUVRectView::set_selected(MDRUVRectViewNode *node) {
	if (selected_rect && ObjectDB::instance_validate(selected_rect)) {
		selected_rect->set_selected(false);
	}

	selected_rect = node;

	if (selected_rect) {
		selected_rect->set_selected(true);
	}
}

void MDRUVRectView::store_uvs() {
	_stored_uvs.resize(0);

	if (!_mdr.is_valid()) {
		return;
	}

	Array arrays = _mdr->get_array();

	if (arrays.size() != ArrayMesh::ARRAY_MAX) {
		return;
	}

	if (arrays[ArrayMesh::ARRAY_TEX_UV].is_null()) {
		return;
	}

	// Make sure it gets copied
	_stored_uvs.append_array(arrays[ArrayMesh::ARRAY_TEX_UV]);
}
PoolVector2Array MDRUVRectView::get_uvs(Ref<MeshDataResource> mdr) {
	if (!_mdr.is_valid()) {
		return PoolVector2Array();
	}

	Array arrays = _mdr->get_array();

	if (arrays.size() != ArrayMesh::ARRAY_MAX) {
		return PoolVector2Array();
	}

	if (arrays[ArrayMesh::ARRAY_TEX_UV].is_null()) {
		return PoolVector2Array();
	}

	return arrays[ArrayMesh::ARRAY_TEX_UV];
}
void MDRUVRectView::apply_uvs(Ref<MeshDataResource> mdr, PoolVector2Array stored_uvs) {
	if (!_mdr.is_valid()) {
		return;
	}

	Array arrays = _mdr->get_array();

	if (arrays.size() != ArrayMesh::ARRAY_MAX) {
		return;
	}

	if (arrays[ArrayMesh::ARRAY_TEX_UV].is_null()) {
		return;
	}

	arrays[ArrayMesh::ARRAY_TEX_UV] = stored_uvs;

	_mdr->set_array(arrays);
}

void MDRUVRectView::refresh() {
	clear();

	Rect2 rect = base_rect;
	edited_resource_current_size = rect.size;
	rect.position = rect.position * _rect_scale;
	rect.size = rect.size * _rect_scale;
	set_custom_minimum_size(rect.size);

	apply_zoom();

	refresh_rects();
}
void MDRUVRectView::clear() {
}
void MDRUVRectView::refresh_rects() {
	clear_rects();

	if (!_mdr.is_valid()) {
		return;
	}

	Vector<PoolIntArray> partitions = MDREDMeshDecompose::partition_mesh(_mdr);

	for (int i = 0; i < partitions.size(); ++i) {
		PoolIntArray p = partitions[i];

		MDRUVRectViewNode *s = memnew(MDRUVRectViewNode);

		add_child(s);
		s->set_editor_rect_scale(_rect_scale);
		s->edited_resource_parent_size = edited_resource_current_size;
		s->set_edited_resource(_mdr, p);
	}
}
void MDRUVRectView::clear_rects() {
	while (get_child_count() > 0) {
		Node *c = get_child(0);
		c->queue_delete();
		remove_child(c);
	}

	selected_rect = nullptr;
}

void MDRUVRectView::_enter_tree() {
}
void MDRUVRectView::_draw() {
	draw_rect(Rect2(Vector2(), get_size()), Color(0.2, 0.2, 0.2, 1));

	if (_background_texture.is_valid()) {
		draw_texture_rect_region(_background_texture, Rect2(Vector2(), get_size()), Rect2(Vector2(), _background_texture->get_size()));
	}
}

void MDRUVRectView::on_mirror_horizontal_button_pressed() {
	if (selected_rect && ObjectDB::instance_validate(selected_rect)) {
		selected_rect->mirror_horizontal();
	}
}
void MDRUVRectView::on_mirror_vertical_button_pressed() {
	if (selected_rect && ObjectDB::instance_validate(selected_rect)) {
		selected_rect->mirror_vertical();
	}
}
void MDRUVRectView::on_rotate_left_button_button_pressed() {
	if (selected_rect && ObjectDB::instance_validate(selected_rect)) {
		selected_rect->rotate_uvs(-rotation_amount);
	}
}

void MDRUVRectView::on_rotate_amount_spinbox_changed(float val) {
	rotation_amount = val;
}
void MDRUVRectView::on_rotate_right_button_button_pressed() {
	if (selected_rect && ObjectDB::instance_validate(selected_rect)) {
		selected_rect->rotate_uvs(rotation_amount);
	}
}
void MDRUVRectView::on_edited_resource_changed() {
	call_deferred("refresh");
}

void MDRUVRectView::on_zoom_changed(float zoom) {
	_rect_scale = zoom;
	apply_zoom();
}

void MDRUVRectView::on_visibility_changed() {
	if (is_visible_in_tree()) {
		store_uvs();
		call_deferred("refresh");
	}
}

void MDRUVRectView::ok_pressed() {
	_undo_redo->create_action("UV Editor Accept");
	_undo_redo->add_do_method(this, "apply_uvs", _mdr, get_uvs(_mdr));
	_undo_redo->add_undo_method(this, "apply_uvs", _mdr, _stored_uvs);
	_undo_redo->commit_action();
}
void MDRUVRectView::cancel_pressed() {
	if (!_mdr.is_valid()) {
		return;
	}

	Array arrays = _mdr->get_array();

	if (arrays.size() != ArrayMesh::ARRAY_MAX) {
		return;
	}

	// Make sure it gets copied
	PoolVector2Array uvs;
	uvs.append_array(_stored_uvs);

	_undo_redo->create_action("UV Editor Cancel");
	_undo_redo->add_do_method(this, "apply_uvs", _mdr, uvs);
	_undo_redo->add_undo_method(this, "apply_uvs", _mdr, get_uvs(_mdr));
	_undo_redo->commit_action();

	_stored_uvs.resize(0);
}

void MDRUVRectView::apply_zoom() {
	Rect2 rect = base_rect;
	edited_resource_current_size = rect.size;
	rect.position = rect.position * _rect_scale;
	rect.size = rect.size * _rect_scale;
	set_custom_minimum_size(rect.size);

	MarginContainer *p = Object::cast_to<MarginContainer>(get_parent());

	p->add_constant_override("margin_left", MIN(rect.size.x / 4.0, 50 * _rect_scale));
	p->add_constant_override("margin_right", MIN(rect.size.x / 4.0, 50 * _rect_scale));
	p->add_constant_override("margin_top", MIN(rect.size.y / 4.0, 50 * _rect_scale));
	p->add_constant_override("margin_bottom", MIN(rect.size.y / 4.0, 50 * _rect_scale));

	for (int i = 0; i < get_child_count(); ++i) {
		MDRUVRectViewNode *c = Object::cast_to<MDRUVRectViewNode>(get_child(i));

		if (c) {
			c->set_editor_rect_scale(_rect_scale);
		}
	}
}

MDRUVRectView::MDRUVRectView() {
	_rect_scale = 1;

	base_rect = Rect2(0, 0, 600, 600);

	_plugin = nullptr;
	_undo_redo = nullptr;

	selected_rect = nullptr;

	rotation_amount = 45;

	/*	var zoom_widget : Node = get_node_or_null(zoom_widget_path)

	if zoom_widget && !zoom_widget.is_connected("zoom_changed", self, "on_zoom_changed"):
		zoom_widget.connect("zoom_changed", self, "on_zoom_changed")

	var mirror_horizontal_button : Button = get_node_or_null(mirror_horizontal_button_path)
	if mirror_horizontal_button && !mirror_horizontal_button.is_connected("pressed", self, "on_mirror_horizontal_button_pressed"):
		mirror_horizontal_button.connect("pressed", self, "on_mirror_horizontal_button_pressed")

	var mirror_vertical_button : Button = get_node_or_null(mirror_vertical_button_path)
	if mirror_vertical_button && !mirror_vertical_button.is_connected("pressed", self, "on_mirror_vertical_button_pressed"):
		mirror_vertical_button.connect("pressed", self, "on_mirror_vertical_button_pressed")

	var rotate_left_button : Button = get_node_or_null(rotate_left_button_path)
	if rotate_left_button && !rotate_left_button.is_connected("pressed", self, "on_rotate_left_button_button_pressed"):
		rotate_left_button.connect("pressed", self, "on_rotate_left_button_button_pressed")

	var rotate_amount_spinbox : SpinBox = get_node_or_null(rotate_amount_spinbox_path)
	if rotate_amount_spinbox:
		rotation_amount = rotate_amount_spinbox.value
		if !rotate_amount_spinbox.is_connected("value_changed", self, "on_rotate_amount_spinbox_changed"):
			rotate_amount_spinbox.connect("value_changed", self, "on_rotate_amount_spinbox_changed")

	var rotate_right_button : Button = get_node_or_null(rotate_right_button_path)
	if rotate_right_button && !rotate_right_button.is_connected("pressed", self, "on_rotate_right_button_button_pressed"):
		rotate_right_button.connect("pressed", self, "on_rotate_right_button_button_pressed")

	if !is_connected("visibility_changed", self, "on_visibility_changed"):
		connect("visibility_changed", self, "on_visibility_changed")

*/
}

MDRUVRectView::~MDRUVRectView() {
}

void MDRUVRectView::_bind_methods() {
	ClassDB::bind_method(D_METHOD("refresh"), &MDRUVRectView::refresh);
}
