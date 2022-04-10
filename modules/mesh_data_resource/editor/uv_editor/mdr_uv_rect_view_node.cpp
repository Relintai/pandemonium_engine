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

#include "mdr_uv_rect_view_node.h"

void MDRUVRectViewNode::set_edited_resource(Ref<MeshDataResource> mdr, PoolIntArray indices) {
	/*
	_mdr = mdr
	_indices = indices
	_uvs.resize(0)

	var arrays : Array = _mdr.get_array()

	if arrays.size() != ArrayMesh.ARRAY_MAX:
		return

	if arrays[ArrayMesh.ARRAY_TEX_UV] == null:
		return

	# Make sure it gets copied
	_uvs.append_array(arrays[ArrayMesh.ARRAY_TEX_UV])

	set_up_base_rect()

	refresh()
	*/
}

void MDRUVRectViewNode::mirror_horizontal() {
	/*
	var pia : PoolIntArray = PoolIntArray()
	for index in _indices:
		var found : bool = false

		for i in pia:
			if i == index:
				found = true
				break

		if found:
			continue

		pia.append(index)

		var uv : Vector2 = _uvs[index]
		uv.x = 1.0 - uv.x
		_uvs.set(index, uv)

	apply_uv()
	update()
	*/
}
void MDRUVRectViewNode::mirror_vertical() {
	/*
	var pia : PoolIntArray = PoolIntArray()
	for index in _indices:
		var found : bool = false

		for i in pia:
			if i == index:
				found = true
				break

		if found:
			continue

		pia.append(index)

		var uv : Vector2 = _uvs[index]
		uv.y = 1.0 - uv.y
		_uvs.set(index, uv)

	apply_uv()
	update()
	*/
}
void MDRUVRectViewNode::rotate_uvs(float amount) {
	/*
	var t : Transform2D = Transform2D(deg2rad(amount), Vector2())

	var pia : PoolIntArray = PoolIntArray()
	for index in _indices:
		var found : bool = false

		for i in pia:
			if i == index:
				found = true
				break

		if found:
			continue

		pia.append(index)

		var uv : Vector2 = _uvs[index]
		uv = t.xform(uv)
		_uvs.set(index, uv)


	re_normalize_uvs()
	apply_uv()
	update()
	*/
}

void MDRUVRectViewNode::set_selected(bool val) {
	/*
	selected = val
	update()
	*/
}

void MDRUVRectViewNode::set_editor_rect_scale(float rect_scale) {
	/*
	_rect_scale = rect_scale

	refresh()
	*/
}
void MDRUVRectViewNode::set_up_base_rect() {
	/*
	_base_rect = Rect2()

	if !_mdr:
		return

	if _uvs.size() == 0:
		return

	var vmin : Vector2 = _uvs[_indices[0]]
	var vmax : Vector2 = vmin
	for i in range(1, _indices.size()):
		var uv : Vector2 = _uvs[_indices[i]]

		if uv.x < vmin.x:
			vmin.x = uv.x

		if uv.x > vmax.x:
			vmax.x = uv.x

		if uv.y < vmin.y:
			vmin.y = uv.y

		if uv.y > vmax.y:
			vmax.y = uv.y

	_base_rect = Rect2(vmin.x, vmin.y, vmax.x - vmin.x, vmax.y - vmin.y)
	_base_rect.position *= edited_resource_parent_size
	_base_rect.size *= edited_resource_parent_size

	_uv_min = vmin
	_uv_max = vmax

	normalize_uvs()

	*/
}

void MDRUVRectViewNode::re_normalize_uvs() {
	/*
	if _uvs.size() == 0:
		return

	var vmin : Vector2 = _uvs[_indices[0]]
	var vmax : Vector2 = vmin
	for i in range(1, _indices.size()):
		var uv : Vector2 = _uvs[_indices[i]]

		if uv.x < vmin.x:
			vmin.x = uv.x

		if uv.x > vmax.x:
			vmax.x = uv.x

		if uv.y < vmin.y:
			vmin.y = uv.y

		if uv.y > vmax.y:
			vmax.y = uv.y

	var xmm : float = vmax.x - vmin.x
	var ymm : float = vmax.y - vmin.y

	if xmm == 0:
		xmm = 0.0000001

	if ymm == 0:
		ymm = 0.0000001

	for i in range(_uvs.size()):
		var uv : Vector2 = _uvs[i]

		uv.x -= vmin.x
		uv.x /= xmm

		uv.y -= vmin.y
		uv.y /= ymm

		_uvs[i] = uv
	*/
}
void MDRUVRectViewNode::normalize_uvs() {
	/*
	var xmm : float = _uv_max.x - _uv_min.x
	var ymm : float = _uv_max.y - _uv_min.y

	if xmm == 0:
		xmm = 0.0000001

	if ymm == 0:
		ymm = 0.0000001

	for i in range(_uvs.size()):
		var uv : Vector2 = _uvs[i]

		uv.x -= _uv_min.x
		uv.x /= xmm

		uv.y -= _uv_min.y
		uv.y /= ymm

		_uvs[i] = uv
	*/
}
void MDRUVRectViewNode::apply_uv() {
	/*
	if !_mdr:
		return

	var rect : Rect2 = get_rect()

	#rect needs to be converted back
	rect.position /= _rect_scale
	rect.size /= _rect_scale
	rect.position /= edited_resource_parent_size
	rect.size /= edited_resource_parent_size

	var arrays : Array = _mdr.get_array()

	if arrays.size() != ArrayMesh.ARRAY_MAX:
		return

	if arrays[ArrayMesh.ARRAY_TEX_UV] == null:
		return

	var uvs : PoolVector2Array = arrays[ArrayMesh.ARRAY_TEX_UV]

	for index in _indices:
		var uv : Vector2 = _uvs[index]

		uv = uv * rect.size + rect.position

		uvs[index] = uv

	_uv_min = rect.position
	_uv_max = rect.position + rect.size

	_base_rect = get_rect()

	arrays[ArrayMesh.ARRAY_TEX_UV] = uvs
	_mdr.array = arrays

	*/
}

void MDRUVRectViewNode::refresh() {
	/*
	if !_mdr:
		return

	var rect : Rect2 = _base_rect
	rect.position *= _rect_scale
	rect.size *= _rect_scale

	rect_position = rect.position
	rect_size = rect.size

	update()
	*/
}

void MDRUVRectViewNode::_draw() {
	/*
	if selected:
		draw_rect(Rect2(Vector2(), get_size()), _edited_resource_rect_selected_color)
		draw_rect(Rect2(Vector2(), get_size()), _edited_resource_rect_selected_border_color, false, _editor_rect_border_size)
	else:
		draw_rect(Rect2(Vector2(), get_size()), _edited_resource_rect_color)
		draw_rect(Rect2(Vector2(), get_size()), _edited_resource_rect_border_color, false, _editor_rect_border_size)

	if _mdr && _uvs.size() > 0:
		var c : Color = _edited_resource_uv_mesh_color

		for i in range(0, len(_indices), 3):
			draw_line(_uvs[_indices[i]] * get_size(), _uvs[_indices[i + 1]] * get_size(), c, 1, false)
			draw_line(_uvs[_indices[i + 1]] * get_size(), _uvs[_indices[i + 2]] * get_size(), c, 1, false)
			draw_line(_uvs[_indices[i + 2]] * get_size(), _uvs[_indices[i]] * get_size(), c, 1, false)

	*/
}

//based on / ported from engine/scene/gui/dialogs.h and .cpp
void MDRUVRectViewNode::_gui_input(Ref<InputEvent> p_event) {
	/*
	if (p_event is InputEventMouseButton) && (p_event.get_button_index() == BUTTON_LEFT):
		var mb : InputEventMouseButton = p_event as InputEventMouseButton

		if (mb.is_pressed()):
			get_parent().set_selected(self)

			# Begin a possible dragging operation.
			drag_type = _drag_hit_test(Vector2(mb.get_position().x, mb.get_position().y))

			if (drag_type != DragType.DRAG_NONE):
				drag_offset = get_global_mouse_position() - get_position()

			drag_offset_far = get_position() + get_size() - get_global_mouse_position()

		elif (drag_type != DragType.DRAG_NONE && !mb.is_pressed()):
			# End a dragging operation.

			apply_uv()

			drag_type = DragType.DRAG_NONE

	if p_event is InputEventMouseMotion:
		var mm : InputEventMouseMotion = p_event as InputEventMouseMotion

		if (drag_type == DragType.DRAG_NONE):
			# Update the cursor while moving along the borders.
			var cursor = CURSOR_ARROW

			var preview_drag_type : int = _drag_hit_test(Vector2(mm.get_position().x, mm.get_position().y))

			var top_left : int = DragType.DRAG_RESIZE_TOP + DragType.DRAG_RESIZE_LEFT
			var bottom_right : int = DragType.DRAG_RESIZE_BOTTOM + DragType.DRAG_RESIZE_RIGHT
			var top_right : int = DragType.DRAG_RESIZE_TOP + DragType.DRAG_RESIZE_RIGHT
			var bottom_left : int = DragType.DRAG_RESIZE_BOTTOM + DragType.DRAG_RESIZE_LEFT

			match (preview_drag_type):
				DragType.DRAG_RESIZE_TOP:
					cursor = CURSOR_VSIZE
				DragType.DRAG_RESIZE_BOTTOM:
					cursor = CURSOR_VSIZE
				DragType.DRAG_RESIZE_LEFT:
					cursor = CURSOR_HSIZE
				DragType.DRAG_RESIZE_RIGHT:
					cursor = CURSOR_HSIZE
				top_left:
					cursor = CURSOR_FDIAGSIZE
				bottom_right:
					cursor = CURSOR_FDIAGSIZE
				top_right:
					cursor = CURSOR_BDIAGSIZE
				bottom_left:
					cursor = CURSOR_BDIAGSIZE

			if (get_cursor_shape() != cursor):
				set_default_cursor_shape(cursor);

		else:
			# Update while in a dragging operation.
			var global_pos : Vector2 = get_global_mouse_position()

			var rect : Rect2 = get_rect()
			var min_size : Vector2 = get_combined_minimum_size()

			if (drag_type == DragType.DRAG_MOVE):
				rect.position = global_pos - drag_offset
			else:
				if (drag_type & DragType.DRAG_RESIZE_TOP):
					var bottom : int = rect.position.y + rect.size.y
					var max_y : int = bottom - min_size.y
					rect.position.y = min(global_pos.y - drag_offset.y, max_y)
					rect.size.y = bottom - rect.position.y
				elif (drag_type & DragType.DRAG_RESIZE_BOTTOM):
					rect.size.y = global_pos.y - rect.position.y + drag_offset_far.y

				if (drag_type & DragType.DRAG_RESIZE_LEFT):
					var right : int = rect.position.x + rect.size.x
					var max_x : int = right - min_size.x
					rect.position.x = min(global_pos.x - drag_offset.x, max_x)
					rect.size.x = right - rect.position.x
				elif (drag_type & DragType.DRAG_RESIZE_RIGHT):
					rect.size.x = global_pos.x - rect.position.x + drag_offset_far.x

			set_size(rect.size)
			set_position(rect.position)
	*/
}

//based on / ported from engine/scene/gui/dialogs.h and .cpp
int MDRUVRectViewNode::_drag_hit_test(Vector2 pos) {
	/*
	var drag_type : int = DragType.DRAG_NONE

	var scaleborder_size : int = 5 #get_constant("scaleborder_size", "WindowDialog")

	var rect : Rect2 = get_rect()

	if (pos.y < (scaleborder_size)):
		drag_type = DragType.DRAG_RESIZE_TOP
	elif (pos.y >= (rect.size.y - scaleborder_size)):
		drag_type = DragType.DRAG_RESIZE_BOTTOM

	if (pos.x < scaleborder_size):
		drag_type |= DragType.DRAG_RESIZE_LEFT
	elif (pos.x >= (rect.size.x - scaleborder_size)):
		drag_type |= DragType.DRAG_RESIZE_RIGHT

	if (drag_type == DragType.DRAG_NONE):
		drag_type = DragType.DRAG_MOVE

	return drag_type
	*/
}

//based on / ported from engine/scene/gui/dialogs.h and .cpp
void MDRUVRectViewNode::_notification(int p_what) {
	/*
		if (p_what == NOTIFICATION_MOUSE_EXIT):
				# Reset the mouse cursor when leaving the resizable window border.
				if (_mdr && !drag_type):
					if (get_default_cursor_shape() != CURSOR_ARROW):
						set_default_cursor_shape(CURSOR_ARROW)
	*/
}

MDRUVRectViewNode::MDRUVRectViewNode() {
	selected = false;
	_base_rect = Rect2(0, 0, 100, 100);

	_edited_resource_rect_border_color = Color(0.8, 0.8, 0.8, 0.5);
	_edited_resource_rect_color = Color(0.5, 0.5, 0.5, 0.2);
	_edited_resource_rect_selected_border_color = Color(0.9, 0.9, 0.9, 0.8);
	_edited_resource_rect_selected_color = Color(0.5, 0.5, 0.5, 0.4);
	_edited_resource_uv_mesh_color = Color(1, 1, 1, 1);
	_editor_rect_border_size = 2;
	_edited_resource_font_color = Color(0, 0, 0, 1);

	drag_type = 0;
	_rect_scale = 1;

	/*
	[gd_scene load_steps=2 format=2]

	[ext_resource path="res://addons/mesh_data_resource_editor/uv_editor/RectViewNode.gd" type="Script" id=1]

	[node name="RectViewNode" type="MarginContainer"]
	margin_right = 1024.0
	margin_bottom = 600.0
	script = ExtResource( 1 )
	__meta__ = {
	"_edit_use_anchors_": false
	}
	*/
}

MDRUVRectViewNode::~MDRUVRectViewNode() {
}

void MDRUVRectViewNode::_bind_methods() {
}
