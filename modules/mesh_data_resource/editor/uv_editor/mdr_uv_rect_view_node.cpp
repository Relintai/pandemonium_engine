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

#include "../../mesh_data_resource.h"
#include "mdr_uv_rect_view.h"
#include "scene/resources/mesh.h"

void MDRUVRectViewNode::set_edited_resource(Ref<MeshDataResource> mdr, PoolIntArray indices) {
	_mdr = mdr;
	_indices = indices;
	_uvs.resize(0);

	if (!mdr.is_valid()) {
		return;
	}

	Array arrays = _mdr->get_array();

	if (arrays.size() != ArrayMesh::ARRAY_MAX) {
		return;
	}

	if (arrays[ArrayMesh::ARRAY_TEX_UV].is_null()) {
		return;
	}

	PoolVector2Array uvsarr = arrays[ArrayMesh::ARRAY_TEX_UV];

	// Make sure it gets copied
	_uvs.append_array(uvsarr);

	set_up_base_rect();
	refresh();
}

void MDRUVRectViewNode::mirror_horizontal() {
	PoolIntArray pia;

	PoolIntArray::Read r = _indices.read();

	for (int ii = 0; ii < _indices.size(); ++ii) {
		int index = r[ii];
		bool found = false;

		PoolIntArray::Read pr = pia.read();

		for (int i = 0; i < pia.size(); ++i) {
			if (pr[i] == index) {
				found = true;
				break;
			}
		}

		pr.release();

		if (found) {
			continue;
		}

		pia.append(index);

		Vector2 uv = _uvs[index];
		uv.x = 1.0 - uv.x;
		_uvs.set(index, uv);
	}

	r.release();

	apply_uv();
	update();
}

void MDRUVRectViewNode::mirror_vertical() {
	PoolIntArray pia;

	PoolIntArray::Read r = _indices.read();
	for (int ii = 0; ii < _indices.size(); ++ii) {
		int index = r[ii];
		bool found = false;

		PoolIntArray::Read pr = pia.read();

		for (int i = 0; i < pia.size(); ++i) {
			if (pr[i] == index) {
				found = true;
				break;
			}
		}

		pr.release();

		if (found) {
			continue;
		}

		pia.append(index);

		Vector2 uv = _uvs[index];
		uv.y = 1.0 - uv.y;
		_uvs.set(index, uv);
	}

	r.release();

	apply_uv();
	update();
}

void MDRUVRectViewNode::rotate_uvs(float amount) {
	Transform2D t = Transform2D(Math::deg2rad(amount), Vector2());

	PoolIntArray pia;

	PoolIntArray::Read r = _indices.read();
	for (int ii = 0; ii < _indices.size(); ++ii) {
		int index = r[ii];
		bool found = false;

		PoolIntArray::Read pr = pia.read();

		for (int i = 0; i < pia.size(); ++i) {
			if (pr[i] == index) {
				found = true;
				break;
			}
		}

		pr.release();

		if (found) {
			continue;
		}

		pia.append(index);

		Vector2 uv = _uvs[index];
		uv = t.xform(uv);
		_uvs.set(index, uv);
	}

	r.release();

	re_normalize_uvs();
	apply_uv();
	update();
}

void MDRUVRectViewNode::set_selected(bool val) {
	selected = val;
	update();
}

void MDRUVRectViewNode::set_editor_rect_scale(float rect_scale) {
	_rect_scale = rect_scale;

	refresh();
}
void MDRUVRectViewNode::set_up_base_rect() {
	_base_rect = Rect2();

	if (!_mdr.is_valid()) {
		return;
	}

	if (_uvs.size() == 0) {
		return;
	}

	Vector2 vmin = _uvs[_indices[0]];
	Vector2 vmax = vmin;
	for (int i = 1; i < _indices.size(); ++i) {
		Vector2 uv = _uvs[_indices[i]];

		if (uv.x < vmin.x) {
			vmin.x = uv.x;
		}

		if (uv.x > vmax.x) {
			vmax.x = uv.x;
		}

		if (uv.y < vmin.y) {
			vmin.y = uv.y;
		}

		if (uv.y > vmax.y) {
			vmax.y = uv.y;
		}
	}

	_base_rect = Rect2(vmin.x, vmin.y, vmax.x - vmin.x, vmax.y - vmin.y);
	_base_rect.position *= edited_resource_parent_size;
	_base_rect.size *= edited_resource_parent_size;

	_uv_min = vmin;
	_uv_max = vmax;

	normalize_uvs();
}

void MDRUVRectViewNode::re_normalize_uvs() {
	if (_uvs.size() == 0) {
		return;
	}

	Vector2 vmin = _uvs[_indices[0]];
	Vector2 vmax = vmin;
	for (int i = 1; i < _indices.size(); ++i) {
		Vector2 uv = _uvs[_indices[i]];

		if (uv.x < vmin.x) {
			vmin.x = uv.x;
		}

		if (uv.x > vmax.x) {
			vmax.x = uv.x;
		}

		if (uv.y < vmin.y) {
			vmin.y = uv.y;
		}

		if (uv.y > vmax.y) {
			vmax.y = uv.y;
		}
	}

	float xmm = vmax.x - vmin.x;
	float ymm = vmax.y - vmin.y;

	if (xmm == 0) {
		xmm = 0.0000001;
	}

	if (ymm == 0) {
		ymm = 0.0000001;
	}

	for (int i = 0; i < _uvs.size(); ++i) {
		Vector2 uv = _uvs[i];

		uv.x -= vmin.x;
		uv.x /= xmm;

		uv.y -= vmin.y;
		uv.y /= ymm;

		_uvs.set(i, uv);
	}
}
void MDRUVRectViewNode::normalize_uvs() {
	float xmm = _uv_max.x - _uv_min.x;
	float ymm = _uv_max.y - _uv_min.y;

	if (xmm == 0) {
		xmm = 0.0000001;
	}

	if (ymm == 0) {
		ymm = 0.0000001;
	}

	for (int i = 0; i < _uvs.size(); ++i) {
		Vector2 uv = _uvs[i];

		uv.x -= _uv_min.x;
		uv.x /= xmm;

		uv.y -= _uv_min.y;
		uv.y /= ymm;

		_uvs.set(i, uv);
	}
}
void MDRUVRectViewNode::apply_uv() {
	if (!_mdr.is_valid()) {
		return;
	}

	Rect2 rect = get_rect();

	//rect needs to be converted back
	rect.position /= _rect_scale;
	rect.size /= _rect_scale;
	rect.position /= edited_resource_parent_size;
	rect.size /= edited_resource_parent_size;

	Array arrays = _mdr->get_array();

	if (arrays.size() != ArrayMesh::ARRAY_MAX) {
		return;
	}

	if (arrays[ArrayMesh::ARRAY_TEX_UV].is_null()) {
		return;
	}

	PoolVector2Array uvs = arrays[ArrayMesh::ARRAY_TEX_UV];

	for (int i = 0; i < _indices.size(); ++i) {
		int index = _indices[i];

		Vector2 uv = _uvs[index];

		uv = uv * rect.size + rect.position;

		uvs.set(index, uv);
	}

	_uv_min = rect.position;
	_uv_max = rect.position + rect.size;

	_base_rect = get_rect();

	arrays[ArrayMesh::ARRAY_TEX_UV] = uvs;
	_mdr->set_array(arrays);
}

void MDRUVRectViewNode::refresh() {
	if (!_mdr.is_valid()) {
		return;
	}

	Rect2 rect = _base_rect;
	rect.position *= _rect_scale;
	rect.size *= _rect_scale;

	set_position(rect.position);
	set_size(rect.size);

	update();
}

void MDRUVRectViewNode::_draw() {
	if (selected) {
		draw_rect(Rect2(Vector2(), get_size()), _edited_resource_rect_selected_color);
		draw_rect(Rect2(Vector2(), get_size()), _edited_resource_rect_selected_border_color, false, _editor_rect_border_size);
	} else {
		draw_rect(Rect2(Vector2(), get_size()), _edited_resource_rect_color);
		draw_rect(Rect2(Vector2(), get_size()), _edited_resource_rect_border_color, false, _editor_rect_border_size);
	}

	if (_mdr.is_valid() && _uvs.size() > 0) {
		Color c = _edited_resource_uv_mesh_color;

		for (int i = 0; i < _indices.size(); i += 3) {
			draw_line(_uvs[_indices[i]] * get_size(), _uvs[_indices[i + 1]] * get_size(), c, 1, false);
			draw_line(_uvs[_indices[i + 1]] * get_size(), _uvs[_indices[i + 2]] * get_size(), c, 1, false);
			draw_line(_uvs[_indices[i + 2]] * get_size(), _uvs[_indices[i]] * get_size(), c, 1, false);
		}
	}
}

//based on / ported from engine/scene/gui/dialogs.h and .cpp
void MDRUVRectViewNode::_gui_input(const Ref<InputEvent> &p_event) {
	Ref<InputEventMouseButton> mb = p_event;

	if ((mb.is_valid()) && (mb->get_button_index() == BUTTON_LEFT)) {
		if (mb->is_pressed()) {
			MDRUVRectView *rvp = Object::cast_to<MDRUVRectView>(get_parent());

			if (rvp) {
				rvp->set_selected(this);
			}

			// Begin a possible dragging operation.
			drag_type = _drag_hit_test(Vector2(mb->get_position().x, mb->get_position().y));

			if (drag_type != DragType::DRAG_NONE) {
				drag_offset = get_global_mouse_position() - get_position();
			}

			drag_offset_far = get_position() + get_size() - get_global_mouse_position();

		} else if (drag_type != DragType::DRAG_NONE && !mb->is_pressed()) {
			// End a dragging operation.
			apply_uv();

			drag_type = DragType::DRAG_NONE;
		}

		return;
	}

	Ref<InputEventMouseMotion> mm = p_event;

	if (mm.is_valid()) {
		if (drag_type == DragType::DRAG_NONE) {
			// Update the cursor while moving along the borders.
			Control::CursorShape cursor = CURSOR_ARROW;

			int preview_drag_type = _drag_hit_test(Vector2(mm->get_position().x, mm->get_position().y));

			switch (preview_drag_type) {
				case (DragType::DRAG_RESIZE_TOP):
					cursor = CURSOR_VSIZE;
					break;
				case (DragType::DRAG_RESIZE_BOTTOM):
					cursor = CURSOR_VSIZE;
					break;
				case (DragType::DRAG_RESIZE_LEFT):
					cursor = CURSOR_HSIZE;
					break;
				case (DragType::DRAG_RESIZE_RIGHT):
					cursor = CURSOR_HSIZE;
					break;
				case (DragType::DRAG_RESIZE_TOP + DragType::DRAG_RESIZE_LEFT):
					cursor = CURSOR_FDIAGSIZE;
					break;
				case (DragType::DRAG_RESIZE_BOTTOM + DragType::DRAG_RESIZE_RIGHT):
					cursor = CURSOR_FDIAGSIZE;
					break;
				case (DragType::DRAG_RESIZE_TOP + DragType::DRAG_RESIZE_RIGHT):
					cursor = CURSOR_BDIAGSIZE;
					break;
				case (DragType::DRAG_RESIZE_BOTTOM + DragType::DRAG_RESIZE_LEFT):
					cursor = CURSOR_BDIAGSIZE;
					break;
				default:
					break;
			}

			if (get_cursor_shape() != cursor) {
				set_default_cursor_shape(cursor);
			}

		} else {
			// Update while in a dragging operation.
			Vector2 global_pos = get_global_mouse_position();

			Rect2 rect = get_rect();
			Vector2 min_size = get_combined_minimum_size();

			if (drag_type == DragType::DRAG_MOVE) {
				rect.position = global_pos - drag_offset;
			} else {
				if (drag_type & DragType::DRAG_RESIZE_TOP) {
					int bottom = rect.position.y + rect.size.y;
					int max_y = bottom - min_size.y;
					rect.position.y = MIN(global_pos.y - drag_offset.y, max_y);
					rect.size.y = bottom - rect.position.y;
				} else if (drag_type & DragType::DRAG_RESIZE_BOTTOM) {
					rect.size.y = global_pos.y - rect.position.y + drag_offset_far.y;
				}

				if (drag_type & DragType::DRAG_RESIZE_LEFT) {
					int right = rect.position.x + rect.size.x;
					int max_x = right - min_size.x;
					rect.position.x = MIN(global_pos.x - drag_offset.x, max_x);
					rect.size.x = right - rect.position.x;
				} else if (drag_type & DragType::DRAG_RESIZE_RIGHT) {
					rect.size.x = global_pos.x - rect.position.x + drag_offset_far.x;
				}
			}

			set_size(rect.size);
			set_position(rect.position);
		}
	}
}

//based on / ported from engine/scene/gui/dialogs.h and .cpp
int MDRUVRectViewNode::_drag_hit_test(Vector2 pos) {
	int drag_type = DragType::DRAG_NONE;

	int scaleborder_size = get_constant("scaleborder_size", "WindowDialog");

	Rect2 rect = get_rect();

	if (pos.y < (scaleborder_size)) {
		drag_type = DragType::DRAG_RESIZE_TOP;
	} else if (pos.y >= (rect.size.y - scaleborder_size)) {
		drag_type = DragType::DRAG_RESIZE_BOTTOM;
	}

	if (pos.x < scaleborder_size) {
		drag_type |= DragType::DRAG_RESIZE_LEFT;
	} else if (pos.x >= (rect.size.x - scaleborder_size)) {
		drag_type |= DragType::DRAG_RESIZE_RIGHT;
	}

	if (drag_type == DragType::DRAG_NONE) {
		drag_type = DragType::DRAG_MOVE;
	}

	return drag_type;
}

//based on / ported from engine/scene/gui/dialogs.h and .cpp
void MDRUVRectViewNode::_notification(int p_what) {
	switch(p_what) {
		case NOTIFICATION_MOUSE_EXIT: {
			// Reset the mouse cursor when leaving the resizable window border.
			if (_mdr.is_valid() && !drag_type) {
				if (get_default_cursor_shape() != CURSOR_ARROW) {
					set_default_cursor_shape(CURSOR_ARROW);
				}
			}
		} break;
		case NOTIFICATION_DRAW: {
			_draw();
		} break;
	}
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

	set_margin(MARGIN_RIGHT, 1024);
	set_margin(MARGIN_BOTTOM, 600);
}

MDRUVRectViewNode::~MDRUVRectViewNode() {
}

void MDRUVRectViewNode::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_gui_input"), &MDRUVRectViewNode::_gui_input);
}
