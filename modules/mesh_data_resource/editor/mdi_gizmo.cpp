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

#include "mdi_gizmo.h"

#include "../mesh_data_resource.h"
#include "../nodes/mesh_data_instance.h"
#include "./utilities/mdr_ed_mesh_decompose.h"
#include "./utilities/mdr_ed_mesh_outline.h"
#include "./utilities/mdr_ed_mesh_utils.h"
#include "core/math/geometry.h"
#include "editor/editor_node.h"
#include "modules/mesh_utils/mesh_utils.h"
#include "scene/3d/camera.h"

void MDIGizmo::setup() {
	MeshDataInstance *mdi = Object::cast_to<MeshDataInstance>(get_spatial_node());

	ERR_FAIL_COND(!mdi);

	mdi->connect("mesh_data_resource_changed", this, "on_mesh_data_resource_changed");

	on_mesh_data_resource_changed(mdi->get_mesh_data());
}
void MDIGizmo::set_editor_plugin(EditorPlugin *editor_plugin) {
	_editor_plugin = editor_plugin;

	_undo_redo = EditorNode::get_undo_redo();
}

void MDIGizmo::set_handle(int index, Camera *camera, Vector2 point) {
	Vector2 relative = point - previous_point;

	if (!_handle_drag_op) {
		relative = Vector2();
		_handle_drag_op = true;

		if (edit_mode == EditMode::EDIT_MODE_SCALE) {
			_drag_op_accumulator = Vector3(1, 1, 1);
		} else {
			_drag_op_accumulator = Vector3();
		}

		_drag_op_accumulator_quat = Quat();

		_drag_op_orig_verices = copy_mdr_verts_array();
		setup_op_drag_indices();
		_drag_op_pivot = get_drag_op_pivot();
	}

	if (edit_mode == EditMode::EDIT_MODE_NONE) {
		return;
	} else if (edit_mode == EditMode::EDIT_MODE_TRANSLATE) {
		Vector3 ofs;

		ofs = camera->get_global_transform().basis.get_axis(0);

		if ((axis_constraint & AXIS_CONSTRAINT_X) != 0) {
			ofs.x *= relative.x * 0.01;
		} else {
			ofs.x = 0;
		}

		if ((axis_constraint & AXIS_CONSTRAINT_Y) != 0) {
			ofs.y = relative.y * -0.01;
		} else {
			ofs.y = 0;
		}

		if ((axis_constraint & AXIS_CONSTRAINT_Z) != 0) {
			ofs.z *= relative.x * 0.01;
		} else {
			ofs.z = 0;
		}

		_drag_op_accumulator += ofs;

		add_to_all_selected(_drag_op_accumulator);

		apply();
		redraw();
	} else if (edit_mode == EditMode::EDIT_MODE_SCALE) {
		float r = ((relative.x + relative.y) * 0.05);

		Vector3 vs;

		if ((axis_constraint & AXIS_CONSTRAINT_X) != 0) {
			vs.x = r;
		}

		if ((axis_constraint & AXIS_CONSTRAINT_Y) != 0) {
			vs.y = r;
		}

		if ((axis_constraint & AXIS_CONSTRAINT_Z) != 0) {
			vs.z = r;
		}

		_drag_op_accumulator += vs;

		Basis b = Basis().scaled(_drag_op_accumulator);
		Transform t = Transform(Basis(), _drag_op_pivot);
		t *= Transform(b, Vector3());
		t *= Transform(Basis(), _drag_op_pivot).inverse();

		mul_all_selected_with_transform(t);

		apply();
		redraw();
	} else if (edit_mode == EditMode::EDIT_MODE_ROTATE) {
		Quat yrot = Quat(Vector3(0, 1, 0), relative.x * 0.01);
		Quat xrot = Quat(camera->get_global_transform().basis.get_axis(0), relative.y * 0.01);

		_drag_op_accumulator_quat *= yrot;
		_drag_op_accumulator_quat *= xrot;
		_drag_op_accumulator_quat = _drag_op_accumulator_quat.normalized();

		Basis b = Basis(_drag_op_accumulator_quat);
		Transform t = Transform(Basis(), _drag_op_pivot);
		t *= Transform(b, Vector3());
		t *= Transform(Basis(), _drag_op_pivot).inverse();

		mul_all_selected_with_transform(t);

		apply();
		redraw();
	}

	previous_point = point;
}

void MDIGizmo::redraw() {
	clear();

	if (!_mdr.is_valid()) {
		return;
	}

	Array array = _mdr->get_array();

	if (array.size() != ArrayMesh::ARRAY_MAX) {
		return;
	}

	if (!get_plugin().is_valid()) {
		return;
	}

	Ref<SpatialMaterial> handles_material = get_plugin()->get_material("handles", Ref<EditorSpatialGizmo>(this));
	Ref<SpatialMaterial> material = get_plugin()->get_material("main", Ref<EditorSpatialGizmo>(this));
	Ref<SpatialMaterial> seam_material = get_plugin()->get_material("seam", Ref<EditorSpatialGizmo>(this));

	_mesh_outline_generator->setup(_mdr);

	if (selection_mode == SELECTION_MODE_EDGE) {
		_mesh_outline_generator->generate_mark_edges(visual_indicator_outline, visual_indicator_handle);
	} else if (selection_mode == SELECTION_MODE_FACE) {
		_mesh_outline_generator->generate_mark_faces(visual_indicator_outline, visual_indicator_handle);
	} else {
		_mesh_outline_generator->generate(visual_indicator_outline, visual_indicator_handle);
	}

	if (visual_indicator_outline || visual_indicator_handle) {
		add_lines(_mesh_outline_generator->lines, material, false);
	}

	if (visual_indicator_seam) {
		add_lines(_mesh_outline_generator->seam_lines, seam_material, false);
	}

	if (_selected_points.size() > 0) {
		Vector<Vector3> vs;

		for (int i = 0; i < _selected_points.size(); ++i) {
			vs.push_back(_handle_points[_selected_points[i]]);
		}

		add_handles(vs, handles_material);
	}
}
void MDIGizmo::apply() {
	if (!_mdr.is_valid()) {
		return;
	}

	disable_change_event();

	Array arrs = _mdr->get_array();
	arrs[ArrayMesh::ARRAY_VERTEX] = _vertices;
	arrs[ArrayMesh::ARRAY_INDEX] = _indices;
	_mdr->set_array(arrs);

	enable_change_event();
}

void MDIGizmo::select_all() {
	if (_selected_points.size() == _handle_points.size()) {
		return;
	}

	_selected_points.resize(_handle_points.size());

	PoolIntArray::Write w = _selected_points.write();

	for (int i = 0; i < _selected_points.size(); ++i) {
		w[i] = i;
	}

	redraw();
}

bool MDIGizmo::selection_click(int index, Camera *camera, const Ref<InputEventMouse> &event) {
	if (handle_selection_type == HANDLE_SELECTION_TYPE_FRONT) {
		return selection_click_select_front_or_back(index, camera, event);
	} else if (handle_selection_type == HANDLE_SELECTION_TYPE_BACK) {
		return selection_click_select_front_or_back(index, camera, event);
	} else {
		return selection_click_select_through(index, camera, event);
	}

	return false;
}
bool MDIGizmo::is_point_visible(Vector3 point_orig, Vector3 camera_pos, Transform gt) {
	Vector3 point = gt.xform(point_orig);

	// go from the given point to the origin (camera_pos -> camera)
	Vector3 dir = camera_pos - point;
	dir = dir.normalized();
	// Might need to reduce z fighting
	//point += dir * 0.5

	for (int i = 0; i < _indices.size(); i += 3) {
		int i0 = _indices[i];
		int i1 = _indices[i + 1];
		int i2 = _indices[i + 2];

		Vector3 v0 = _vertices[i0];
		Vector3 v1 = _vertices[i1];
		Vector3 v2 = _vertices[i2];

		v0 = gt.xform(v0);
		v1 = gt.xform(v1);
		v2 = gt.xform(v2);

		bool intersects = Geometry::ray_intersects_triangle(point, dir, v0, v1, v2);

		if (intersects) {
			return false;
		}
	}

	return true;
}

bool MDIGizmo::selection_click_select_front_or_back(int index, Camera *camera, const Ref<InputEventMouse> &event) {
	Transform gt = get_spatial_node()->get_global_transform();
	Vector3 ray_from = camera->get_global_transform().origin;
	Vector2 gpoint = event->get_position();
	float grab_threshold = 8;

	// select vertex
	int closest_idx = -1;
	float closest_dist = 1e10;

	for (int i = 0; i < _handle_points.size(); ++i) {
		Vector3 vert_pos_3d = gt.xform(_handle_points[i]);
		Vector2 vert_pos_2d = camera->unproject_position(vert_pos_3d);
		float dist_3d = ray_from.distance_to(vert_pos_3d);
		float dist_2d = gpoint.distance_to(vert_pos_2d);

		if (dist_2d < grab_threshold && dist_3d < closest_dist) {
			bool point_visible = is_point_visible(_handle_points[i], ray_from, gt);

			if (handle_selection_type == HANDLE_SELECTION_TYPE_FRONT) {
				if (!point_visible) {
					continue;
				}
			} else if (handle_selection_type == HANDLE_SELECTION_TYPE_BACK) {
				if (point_visible) {
					continue;
				}
			}

			closest_dist = dist_3d;
			closest_idx = i;
		}
	}

	if (closest_idx >= 0) {
		for (int si = 0; si < _selected_points.size(); ++si) {
			if (_selected_points[si] == closest_idx) {
				if (event->get_alt() || event->get_control()) {
					_selected_points.remove(si);
					return true;
				}

				return false;
			}
		}

		if (event->get_alt() || event->get_control()) {
			return false;
		}

		if (event->get_shift()) {
			_selected_points.append(closest_idx);
		} else {
			// Select new point only
			_selected_points.resize(0);
			_selected_points.append(closest_idx);
		}

		apply();
		redraw();
	} else {
		// Don't unselect all if either control or shift is held down
		if (event->get_shift() || event->get_control() || event->get_alt()) {
			return false;
		}

		if (_selected_points.size() == 0) {
			return false;
		}

		//Unselect all
		_selected_points.resize(0);

		redraw();
	}
}
bool MDIGizmo::selection_click_select_through(int index, Camera *camera, const Ref<InputEventMouse> &event) {
	Transform gt = get_spatial_node()->get_global_transform();
	Vector3 ray_from = camera->get_global_transform().origin;
	Vector2 gpoint = event->get_position();
	float grab_threshold = 8;

	// select vertex
	int closest_idx = -1;
	float closest_dist = 1e10;

	for (int i = 0; i < _handle_points.size(); ++i) {
		Vector3 vert_pos_3d = gt.xform(_handle_points[i]);
		Vector2 vert_pos_2d = camera->unproject_position(vert_pos_3d);
		float dist_3d = ray_from.distance_to(vert_pos_3d);
		float dist_2d = gpoint.distance_to(vert_pos_2d);

		if (dist_2d < grab_threshold && dist_3d < closest_dist) {
			closest_dist = dist_3d;
			closest_idx = i;
		}
	}

	if (closest_idx >= 0) {
		for (int si = 0; si < _selected_points.size(); ++si) {
			if (_selected_points[si] == closest_idx) {
				if (event->get_alt() || event->get_control()) {
					_selected_points.remove(si);
					return true;
				}

				return false;
			}
		}

		if (event->get_alt() || event->get_control()) {
			return false;
		}

		if (event->get_shift()) {
			_selected_points.append(closest_idx);
		} else {
			// Select new point only
			_selected_points.resize(0);
			_selected_points.append(closest_idx);
		}

		apply();
		redraw();
	} else {
		// Don't unselect all if either control or shift is held down
		if (event->get_shift() || event->get_control() || event->get_alt()) {
			return false;
		}

		if (_selected_points.size() == 0) {
			return false;
		}

		//Unselect all
		_selected_points.resize(0);

		redraw();
	}
}
void MDIGizmo::selection_drag(int index, Camera *camera, const Ref<InputEventMouse> &event) {
	if (handle_selection_type == HANDLE_SELECTION_TYPE_FRONT) {
		selection_drag_rect_select_front_back(index, camera, event);
	} else if (handle_selection_type == HANDLE_SELECTION_TYPE_BACK) {
		selection_drag_rect_select_front_back(index, camera, event);
	} else {
		selection_drag_rect_select_through(index, camera, event);
	}
}
void MDIGizmo::selection_drag_rect_select_front_back(int index, Camera *camera, const Ref<InputEventMouse> &event) {
	Transform gt = get_spatial_node()->get_global_transform();
	Vector3 ray_from = camera->get_global_transform().origin;

	Vector2 mouse_pos = event->get_position();
	Vector2 rect_size = _rect_drag_start_point - mouse_pos;
	rect_size.x = ABS(rect_size.x);
	rect_size.y = ABS(rect_size.y);

	Rect2 rect = Rect2(_rect_drag_start_point, rect_size);

	// This is needed so selection works even when you drag from bottom to top, and from right to left
	Vector2 rect_ofs = _rect_drag_start_point - mouse_pos;

	if (rect_ofs.x > 0) {
		rect.position.x -= rect_ofs.x;
	}

	if (rect_ofs.y > 0) {
		rect.position.y -= rect_ofs.y;
	}

	PoolIntArray selected;

	for (int i = 0; i < _handle_points.size(); ++i) {
		Vector3 vert_pos_3d = gt.xform(_handle_points[i]);
		Vector2 vert_pos_2d = camera->unproject_position(vert_pos_3d);

		if (rect.has_point(vert_pos_2d)) {
			bool point_visible = is_point_visible(_handle_points[i], ray_from, gt);

			if (handle_selection_type == HANDLE_SELECTION_TYPE_FRONT) {
				if (!point_visible) {
					continue;
				}
			} else if (handle_selection_type == HANDLE_SELECTION_TYPE_BACK) {
				if (point_visible) {
					continue;
				}
			}

			selected.push_back(i);
		}
	}

	if (event->get_alt() || event->get_control()) {
		PoolIntArray::Read r = selected.read();

		for (int is = 0; is < selected.size(); ++is) {
			int isel = r[is];

			for (int i = 0; i < _selected_points.size(); ++i) {
				if (_selected_points[i] == isel) {
					_selected_points.remove(i);
					break;
				}
			}
		}

		r.release();

		redraw();

		return;
	}

	if (event->get_shift()) {
		PoolIntArray::Read r = selected.read();

		for (int is = 0; is < selected.size(); ++is) {
			int isel = r[is];

			if (!pool_int_arr_contains(_selected_points, isel)) {
				_selected_points.push_back(isel);
			}
		}

		r.release();

		redraw();
		return;
	}

	_selected_points.resize(0);
	_selected_points.append_array(selected);

	redraw();
}
void MDIGizmo::selection_drag_rect_select_through(int index, Camera *camera, const Ref<InputEventMouse> &event) {
	/*
	var gt : Transform = get_spatial_node().global_transform

	var mouse_pos : Vector2 = event.get_position()
	var rect_size : Vector2 = _rect_drag_start_point - mouse_pos
	rect_size.x = abs(rect_size.x)
	rect_size.y = abs(rect_size.y)

	var rect : Rect2 = Rect2(_rect_drag_start_point, rect_size)

	# This is needed so selection works even when you drag from bottom to top, and from right to left
	var rect_ofs : Vector2 = _rect_drag_start_point - mouse_pos

	if rect_ofs.x > 0:
		rect.position.x -= rect_ofs.x

	if rect_ofs.y > 0:
		rect.position.y -= rect_ofs.y

	var selected : PoolIntArray = PoolIntArray()

	for i in range(_handle_points.size()):
		var vert_pos_3d : Vector3 = gt.xform(_handle_points[i])
		var vert_pos_2d : Vector2 = camera.unproject_position(vert_pos_3d)

		if rect.has_point(vert_pos_2d):
			selected.push_back(i)

	if event.alt || event.control:
		for isel in selected:
			for i in range(_selected_points.size()):
				if _selected_points[i] == isel:
					_selected_points.remove(i)
					break
		redraw()

		return

	if event.shift:
		for isel in selected:
			if !pool_int_arr_contains(_selected_points, isel):
				_selected_points.push_back(isel)

		redraw()
		return

	_selected_points.resize(0)
	_selected_points.append_array(selected)

	redraw()
	*/
}
bool MDIGizmo::forward_spatial_gui_input(int index, Camera *camera, const Ref<InputEvent> &event) {
	/*
	_last_known_camera_facing = camera.transform.basis.xform(Vector3(0, 0, -1))

	if event is InputEventMouseButton:
		if event.get_button_index() == BUTTON_LEFT:
			if _handle_drag_op:
				if !event.is_pressed():
					_handle_drag_op = false

					# If a handle was being dragged only run these
					if _mdr && _mdr.array.size() == ArrayMesh::ARRAY_MAX && _mdr.array[ArrayMesh::ARRAY_VERTEX] != null && _mdr.array[ArrayMesh::ARRAY_VERTEX].size() == _drag_op_orig_verices.size():
						_undo_redo.create_action("Drag")
						_undo_redo.add_do_method(self, "apply_vertex_array", _mdr, _mdr.array[ArrayMesh::ARRAY_VERTEX])
						_undo_redo.add_undo_method(self, "apply_vertex_array", _mdr, _drag_op_orig_verices)
						_undo_redo.commit_action()

				# Dont consume the event here, because the handles will get stuck
				# to the mouse pointer if we return true
				return false

			if !event.is_pressed():
				# See whether we should check for a click or a selection box
				var mouse_pos : Vector2 = event.get_position()
				var rect_size : Vector2 = _rect_drag_start_point - mouse_pos
				rect_size.x = abs(rect_size.x)
				rect_size.y = abs(rect_size.y)
				var had_rect_drag : bool = false

				if rect_size.x > _rect_drag_min_ofset || rect_size.y > _rect_drag_min_ofset:
					had_rect_drag = true

				if !had_rect_drag:
					return selection_click(index, camera, event)
				else:
					selection_drag(index, camera, event)
					# Always return false here, so the drag rect thing disappears in the editor
					return false
			else:
				# event is pressed
				_rect_drag = true
				_rect_drag_start_point = event.get_position()

	return false

	*/
}
void MDIGizmo::add_to_all_selected(Vector3 ofs) {
	/*
	for i in _selected_points:
		var v : Vector3 = _handle_points[i]
		v += ofs
		_handle_points.set(i, v)

	for indx in _drag_op_indices:
		var v : Vector3 = _drag_op_orig_verices[indx]
		v += ofs
		_vertices.set(indx, v)
	*/
}

void MDIGizmo::mul_all_selected_with_basis(Basis b) {
	/*
	for i in _selected_points:
		var v : Vector3 = _handle_points[i]
		v = b * v
		_handle_points.set(i, v)

	for indx in _drag_op_indices:
		var v : Vector3 = _drag_op_orig_verices[indx]
		v = b * v
		_vertices.set(indx, v)
	*/
}
void MDIGizmo::mul_all_selected_with_transform(Transform t) {
	/*
	for i in _selected_points:
		var v : Vector3 = _handle_points[i]
		v = t * v
		_handle_points.set(i, v)

	for indx in _drag_op_indices:
		var v : Vector3 = _drag_op_orig_verices[indx]
		v = t * v
		_vertices.set(indx, v)
	*/
}
void MDIGizmo::mul_all_selected_with_transform_acc(Transform t) {
	/*
	for i in _selected_points:
		var v : Vector3 = _handle_points[i]
		v = t * v
		_handle_points.set(i, v)

	for indx in _drag_op_indices:
		var v : Vector3 = _vertices[indx]
		v = t * v
		_vertices.set(indx, v)
	*/
}

void MDIGizmo::set_translate() {
	edit_mode = EDIT_MODE_TRANSLATE;
}
void MDIGizmo::set_scale() {
	edit_mode = EDIT_MODE_SCALE;
}
void MDIGizmo::set_rotate() {
	edit_mode = EDIT_MODE_ROTATE;
}
void MDIGizmo::set_edit_mode(int em) {
	edit_mode = em;
}

void MDIGizmo::set_axis_x(bool on) {
	if (on) {
		axis_constraint |= AXIS_CONSTRAINT_X;
	} else {
		if ((axis_constraint & AXIS_CONSTRAINT_X) != 0) {
			axis_constraint ^= AXIS_CONSTRAINT_X;
		}
	}
}
void MDIGizmo::set_axis_y(bool on) {
	if (on) {
		axis_constraint |= AXIS_CONSTRAINT_Y;
	} else {
		if ((axis_constraint & AXIS_CONSTRAINT_Y) != 0) {
			axis_constraint ^= AXIS_CONSTRAINT_Y;
		}
	}
}
void MDIGizmo::set_axis_z(bool on) {
	if (on) {
		axis_constraint |= AXIS_CONSTRAINT_Z;
	} else {
		if ((axis_constraint & AXIS_CONSTRAINT_Z) != 0) {
			axis_constraint ^= AXIS_CONSTRAINT_Z;
		}
	}
}

void MDIGizmo::set_selection_mode_vertex() {
	if (selection_mode == SELECTION_MODE_VERTEX) {
		return;
	}

	selection_mode = SELECTION_MODE_VERTEX;
	_selected_points.resize(0);
	recalculate_handle_points();
	redraw();
}
void MDIGizmo::set_selection_mode_edge() {
	if (selection_mode == SELECTION_MODE_EDGE) {
		return;
	}
	selection_mode = SELECTION_MODE_EDGE;
	_selected_points.resize(0);
	recalculate_handle_points();
	redraw();
}
void MDIGizmo::set_selection_mode_face() {
	if (selection_mode == SELECTION_MODE_FACE) {
		return;
	}
	selection_mode = SELECTION_MODE_FACE;
	_selected_points.resize(0);
	recalculate_handle_points();
	redraw();
}

void MDIGizmo::_notification(int what) {
	/*
	if (what == NOTIFICATION_PREDELETE) {
		if (this != nullptr && get_plugin().is_valid()) {
			get_plugin()->unregister_gizmo(this);
		}
	}*/
}

void MDIGizmo::recalculate_handle_points() {
	/*
	if !_mdr:
		_handle_points.resize(0)
		_handle_to_vertex_map.resize(0)
		return

	var mdr_arr : Array = _mdr.array

	if mdr_arr.size() != ArrayMesh::ARRAY_MAX || mdr_arr[ArrayMesh::ARRAY_VERTEX] == null || mdr_arr[ArrayMesh::ARRAY_VERTEX].size() == 0:
		_handle_points.resize(0)
		_handle_to_vertex_map.resize(0)
		return

	var arr : Array = Array()
	arr.resize(ArrayMesh::ARRAY_MAX)
	arr[ArrayMesh::ARRAY_VERTEX] = mdr_arr[ArrayMesh::ARRAY_VERTEX]
	arr[ArrayMesh::ARRAY_INDEX] = mdr_arr[ArrayMesh::ARRAY_INDEX]

	if selection_mode == SELECTION_MODE_VERTEX:
		var merged_arrays : Array = MeshUtils::merge_mesh_array(arr)
		_handle_points = merged_arrays[ArrayMesh::ARRAY_VERTEX]
		_handle_to_vertex_map = MeshDecompose.get_handle_vertex_to_vertex_map(mdr_arr, _handle_points)
	elif selection_mode == SELECTION_MODE_EDGE:
		var result : Array = MeshDecompose.get_handle_edge_to_vertex_map(arr)

		_handle_points = result[0]
		_handle_to_vertex_map = result[1]
	elif selection_mode == SELECTION_MODE_FACE:
		var result : Array = MeshDecompose.get_handle_face_to_vertex_map(arr)

		_handle_points = result[0]
		_handle_to_vertex_map = result[1]
	*/
}
void MDIGizmo::on_mesh_data_resource_changed(Ref<MeshDataResource> mdr) {
	/*
	if _mdr:
		_mdr.disconnect("changed", self, "on_mdr_changed")

	_mdr = mdr

	if _mdr && _mdr.array.size() == ArrayMesh::ARRAY_MAX && _mdr.array[ArrayMesh::ARRAY_VERTEX] != null:
		_vertices = _mdr.array[ArrayMesh::ARRAY_VERTEX]
		_indices = _mdr.array[ArrayMesh::ARRAY_INDEX]
	else:
		_vertices.resize(0)
		_indices.resize(0)

	if _mdr:
		_mdr.connect("changed", self, "on_mdr_changed")

	recalculate_handle_points()
	redraw()
	*/
}
void MDIGizmo::on_mdr_changed() {
	if (!_mdr.is_valid()) {
		_vertices.resize(0);
		_indices.resize(0);
		recalculate_handle_points();
		redraw();
	}

	Array arr = _mdr->get_array();

	if (arr.size() == ArrayMesh::ARRAY_MAX && !arr[ArrayMesh::ARRAY_VERTEX].is_null()) {
		_vertices = arr[ArrayMesh::ARRAY_VERTEX];
		_indices = arr[ArrayMesh::ARRAY_INDEX];
	} else {
		_vertices.resize(0);
		_indices.resize(0);
	}

	recalculate_handle_points();
	redraw();
}
void MDIGizmo::disable_change_event() {
	_mdr->disconnect("changed", this, "on_mdr_changed");
}
void MDIGizmo::enable_change_event(bool update = true) {
	_mdr->connect("changed", this, "on_mdr_changed");

	if (update) {
		on_mdr_changed();
	}
}
void MDIGizmo::add_triangle() {
	if (_mdr.is_valid()) {
		Array orig_arr = copy_arrays(_mdr->get_array());
		MDREDMeshUtils::add_triangle(_mdr);
		add_mesh_change_undo_redo(orig_arr, _mdr->get_array(), "Add Triangle");
	}
}
void MDIGizmo::add_quad() {
	if (_mdr.is_valid()) {
		Array orig_arr = copy_arrays(_mdr->get_array());
		MDREDMeshUtils::add_quad(_mdr);
		add_mesh_change_undo_redo(orig_arr, _mdr->get_array(), "Add Quad");
	}
}

bool MDIGizmo::is_verts_equal(Vector3 v0, Vector3 v1) {
	return Math::is_equal_approx(v0.x, v1.x) && Math::is_equal_approx(v0.y, v1.y) && Math::is_equal_approx(v0.z, v1.z);
}
Vector3 MDIGizmo::find_other_vertex_for_edge(int edge, Vector3 v0) {
	PoolIntArray ps = _handle_to_vertex_map[edge];

	Vector3 vert;

	for (int i = 0; i < ps.size(); ++i) {
		vert = _vertices[ps[i]];

		if (!is_verts_equal(v0, vert)) {
			return vert;
		}
	}

	return v0;
}
Array MDIGizmo::split_edge_indices(int edge) {
	PoolIntArray ps = _handle_to_vertex_map[edge];

	if (ps.size() == 0) {
		return Array();
	}

	Vector3 v0 = _vertices[ps[0]];

	PoolIntArray v0ei;
	v0ei.append(ps[0]);
	PoolIntArray v1ei;

	for (int i = 1; i < ps.size(); ++i) {
		Vector3 vert = _vertices[ps[i]];

		if (is_verts_equal(v0, vert)) {
			v0ei.append(ps[i]);
		} else {
			v1ei.append(ps[i]);
		}
	}

	Array arr;
	arr.push_back(v0ei);
	arr.push_back(v1ei);

	return arr;
}
bool MDIGizmo::pool_int_arr_contains(PoolIntArray arr, int val) {
	PoolIntArray::Read r = arr.read();

	for (int i = 0; i < arr.size(); ++i) {
		if (r[i] == val) {
			return true;
		}
	}

	return false;
}
PoolIntArray MDIGizmo::find_triangles_for_edge(int edge) {
	/*
	var eisarr : Array = split_edge_indices(edge)

	if eisarr.size() == 0:
		return PoolIntArray()

	# these should have the same size
	var v0ei : PoolIntArray = eisarr[0]
	var v1ei : PoolIntArray = eisarr[1]

	var res : PoolIntArray = PoolIntArray()

	for i in range(0, _indices.size(), 3):
		var i0 : int = _indices[i]
		var i1 : int = _indices[i + 1]
		var i2 : int = _indices[i + 2]

		if pool_int_arr_contains(v0ei, i0) || pool_int_arr_contains(v0ei, i1) || pool_int_arr_contains(v0ei, i2):
			if pool_int_arr_contains(v1ei, i0) || pool_int_arr_contains(v1ei, i1) || pool_int_arr_contains(v1ei, i2):
				res.append(i / 3)

	return res
	*/
}
int MDIGizmo::find_first_triangle_for_edge(int edge) {
	/*
	var eisarr : Array = split_edge_indices(edge)

	if eisarr.size() == 0:
		return -1

	# these should have the same size
	var v0ei : PoolIntArray = eisarr[0]
	var v1ei : PoolIntArray = eisarr[1]

	var res : PoolIntArray = PoolIntArray()

	for i in range(0, _indices.size(), 3):
		var i0 : int = _indices[i]
		var i1 : int = _indices[i + 1]
		var i2 : int = _indices[i + 2]

		if pool_int_arr_contains(v0ei, i0) || pool_int_arr_contains(v0ei, i1) || pool_int_arr_contains(v0ei, i2):
			if pool_int_arr_contains(v1ei, i0) || pool_int_arr_contains(v1ei, i1) || pool_int_arr_contains(v1ei, i2):
				return i / 3

	return -1

	*/
}
void MDIGizmo::add_triangle_to_edge(int edge) {
	/*
	var triangle_index : int = find_first_triangle_for_edge(edge)

	var inds : int = triangle_index * 3

	var ti0 : int = _indices[inds]
	var ti1 : int = _indices[inds + 1]
	var ti2 : int = _indices[inds + 2]

	var ps : PoolIntArray = _handle_to_vertex_map[edge]

	if ps.size() == 0:
		return

	var ei0 : int = 0
	var ei1 : int = 0
	var erefind : int = 0

	if !pool_int_arr_contains(ps, ti0):
		ei0 = ti1
		ei1 = ti2
		erefind = ti0
	elif !pool_int_arr_contains(ps, ti1):
		ei0 = ti0
		ei1 = ti2
		erefind = ti1
	elif !pool_int_arr_contains(ps, ti2):
		ei0 = ti0
		ei1 = ti1
		erefind = ti2

	var fo : Vector3 = MDREDMeshUtils::get_face_normal(_vertices[ti0], _vertices[ti1], _vertices[ti2])
	var fn : Vector3 = MDREDMeshUtils::get_face_normal(_vertices[ei0], _vertices[ei1], _vertices[erefind])

	if fo.dot(fn) < 0:
		var t : int = ei0
		ei0 = ei1
		ei1 = t

	MDREDMeshUtils::append_triangle_to_tri_edge(_mdr, _vertices[ei0], _vertices[ei1], _vertices[erefind])

	*/
}
void MDIGizmo::add_quad_to_edge(int edge) {
	/*
	var triangle_index : int = find_first_triangle_for_edge(edge)

	var inds : int = triangle_index * 3

	var ti0 : int = _indices[inds]
	var ti1 : int = _indices[inds + 1]
	var ti2 : int = _indices[inds + 2]

	var ps : PoolIntArray = _handle_to_vertex_map[edge]

	if ps.size() == 0:
		return

	var ei0 : int = 0
	var ei1 : int = 0
	var erefind : int = 0

	if !pool_int_arr_contains(ps, ti0):
		ei0 = ti1
		ei1 = ti2
		erefind = ti0
	elif !pool_int_arr_contains(ps, ti1):
		ei0 = ti0
		ei1 = ti2
		erefind = ti1
	elif !pool_int_arr_contains(ps, ti2):
		ei0 = ti0
		ei1 = ti1
		erefind = ti2

	var fo : Vector3 = MDREDMeshUtils::get_face_normal(_vertices[ti0], _vertices[ti1], _vertices[ti2])
	var fn : Vector3 = MDREDMeshUtils::get_face_normal(_vertices[ei0], _vertices[ei1], _vertices[erefind])

	if fo.dot(fn) < 0:
		var t : int = ei0
		ei0 = ei1
		ei1 = t

	MDREDMeshUtils::append_quad_to_tri_edge(_mdr, _vertices[ei0], _vertices[ei1], _vertices[erefind])
	*/
}
void MDIGizmo::add_triangle_at() {
	/*
	if !_mdr:
		return

	if selection_mode == SELECTION_MODE_VERTEX:
		#todo
		pass
	elif selection_mode == SELECTION_MODE_EDGE:
		disable_change_event()
		var orig_arr = copy_arrays(_mdr.array)

		for sp in _selected_points:
			add_triangle_to_edge(sp)

		_selected_points.resize(0)
		add_mesh_change_undo_redo(orig_arr, _mdr.array, "Add Triangle At")
		enable_change_event()
	else:
		add_triangle()
	*/
}
void MDIGizmo::add_quad_at() {
	/*
	if !_mdr:
		return

	if selection_mode == SELECTION_MODE_VERTEX:
		#todo
		pass
	elif selection_mode == SELECTION_MODE_EDGE:
		disable_change_event()
		var orig_arr = copy_arrays(_mdr.array)

		for sp in _selected_points:
			add_quad_to_edge(sp)

		_selected_points.resize(0)
		add_mesh_change_undo_redo(orig_arr, _mdr.array, "Add Quad At")
		enable_change_event()
	else:
		add_quad()
	*/
}

void MDIGizmo::extrude() {
	/*
	if !_mdr:
		return

	if _mdr.array.size() != ArrayMesh::ARRAY_MAX || _mdr.array[ArrayMesh::ARRAY_VERTEX] == null:
		return

	if selection_mode == SELECTION_MODE_VERTEX:
		pass
	elif selection_mode == SELECTION_MODE_EDGE:
		disable_change_event()
		var orig_arr = copy_arrays(_mdr.array)
		var original_size : int = orig_arr[ArrayMesh::ARRAY_VERTEX].size()

		for sp in _selected_points:
			add_quad_to_edge(sp)

		var arr : Array = _mdr.array

		# Note: This algorithm depends heavily depends on the inner workings of add_quad_to_edge!
		var new_verts : PoolVector3Array = arr[ArrayMesh::ARRAY_VERTEX]

		# every 4 vertex is a quad
		# 1 ---- 2
		# |      |
		# |      |
		# 0 ---- 3
		# vertex 1, and 2 are the created new ones, 0, and 3 are duplicated from the original edge

		# Don't reallocate it every time
		var found_verts : PoolIntArray = PoolIntArray()

		# Go through every new created 0th vertex
		for i in range(original_size, new_verts.size(), 4):
			var v0 : Vector3 = new_verts[i]

			found_verts.resize(0)

			# Find a pair for it (has to be the 3th).
			for j in range(original_size, new_verts.size(), 4):
				if i == j:
					continue

				# +3 offset to 3rd vert
				var v3 : Vector3 = new_verts[j + 3]

				if is_verts_equal(v0, v3):
					# +2 offset to 2rd vert
					found_verts.append(j + 2)

			if found_verts.size() == 0:
				continue

			# Also append the first vertex index to simplify logic
			found_verts.append(i + 1)

			# Calculate avg
			var vavg : Vector3 = Vector3()
			for ind in found_verts:
				vavg += new_verts[ind]

			vavg /= found_verts.size()

			# set back
			for ind in found_verts:
				new_verts[ind] = vavg

		arr[ArrayMesh::ARRAY_VERTEX] = new_verts
		_mdr.array = arr

		_selected_points.resize(0)
		add_mesh_change_undo_redo(orig_arr, _mdr.array, "Extrude")
		enable_change_event()

		# The selection alo will take care of the duplicates
		var new_handle_points : PoolVector3Array = PoolVector3Array()
		for i in range(original_size, new_verts.size(), 4):
			var vavg : Vector3 = new_verts[i + 1]
			vavg += new_verts[i + 2]
			vavg /= 2

			new_handle_points.append(vavg)

		select_handle_points(new_handle_points)
	else:
		add_quad()
	*/
}
void MDIGizmo::add_box() {
	/*
	if _mdr:
		var orig_arr = copy_arrays(_mdr.array)
		MDREDMeshUtils::add_box(_mdr)
		add_mesh_change_undo_redo(orig_arr, _mdr.array, "Add Box")
	*/
}
void MDIGizmo::split() {
}
void MDIGizmo::disconnect_action() {
}
int MDIGizmo::get_first_triangle_index_for_vertex(int indx) {
	/*
	for i in range(_indices.size()):
		if _indices[i] == indx:
			return i / 3

	return -1
	*/
}

void MDIGizmo::create_face() {
	/*
	if !_mdr:
		return

	if _selected_points.size() <= 2:
		return

	if selection_mode == SELECTION_MODE_VERTEX:
		disable_change_event()

		var orig_arr = copy_arrays(_mdr.array)

		var points : PoolVector3Array = PoolVector3Array()

		for sp in _selected_points:
			points.push_back(_handle_points[sp])

		if points.size() == 3:
			var i0 : int = _handle_to_vertex_map[_selected_points[0]][0]
			var i1 : int = _handle_to_vertex_map[_selected_points[1]][0]
			var i2 : int = _handle_to_vertex_map[_selected_points[2]][0]

			var v0 : Vector3 = points[0]
			var v1 : Vector3 = points[1]
			var v2 : Vector3 = points[2]

			var tfn : Vector3 = Vector3()

			if orig_arr[ArrayMesh::ARRAY_NORMAL] != null && orig_arr[ArrayMesh::ARRAY_NORMAL].size() == orig_arr[ArrayMesh::ARRAY_VERTEX].size():
				var normals : PoolVector3Array = orig_arr[ArrayMesh::ARRAY_NORMAL]

				tfn += normals[i0]
				tfn += normals[i1]
				tfn += normals[i2]
				tfn /= 3
				tfn = tfn.normalized()
			else:
				tfn = MDREDMeshUtils::get_face_normal(_vertices[i0], _vertices[i1], _vertices[i2])

			var flip : bool = !MDREDMeshUtils::should_triangle_flip(v0, v1, v2, tfn)

			MDREDMeshUtils::add_triangle_at(_mdr, v0, v1, v2, flip)
			add_mesh_change_undo_redo(orig_arr, _mdr.array, "Create Face")
			enable_change_event()
			return

		if !MDREDMeshUtils::add_triangulated_mesh_from_points_delaunay(_mdr, points, _last_known_camera_facing):
			enable_change_event()
			return

		add_mesh_change_undo_redo(orig_arr, _mdr.array, "Create Face")

		#_selected_points.resize(0)
		enable_change_event()
	elif selection_mode == SELECTION_MODE_EDGE:
		pass
	elif selection_mode == SELECTION_MODE_FACE:
		pass
	*/
}

Array MDIGizmo::split_face_indices(int face) {
	/*
	var ps : PoolIntArray = _handle_to_vertex_map[face]

	if ps.size() == 0:
		return [  ]

	var v0 : Vector3 = _vertices[ps[0]]
	var v1 : Vector3 = Vector3()
	var v1found : bool = false

	var v0ei : PoolIntArray = PoolIntArray()
	v0ei.append(ps[0])
	var v1ei : PoolIntArray = PoolIntArray()
	var v2ei : PoolIntArray = PoolIntArray()

	for i in range(1, ps.size()):
		var vert : Vector3 = _vertices[ps[i]]

		if is_verts_equal(v0, vert):
			v0ei.append(ps[i])
		else:
			if v1found:
				if is_verts_equal(v1, vert):
					v1ei.append(ps[i])
				else:
					v2ei.append(ps[i])
			else:
				v1found = true
				v1 = _vertices[ps[i]]
				v1ei.append(ps[i])

	return [ v0ei, v1ei, v2ei ]
	*/
}
int MDIGizmo::find_first_triangle_index_for_face(int face) {
	/*
	var split_indices_arr : Array = split_face_indices(face)

	if split_indices_arr.size() == 0:
		return -1

	var v0ei : PoolIntArray = split_indices_arr[0]
	var v1ei : PoolIntArray = split_indices_arr[1]
	var v2ei : PoolIntArray = split_indices_arr[2]
	var tri_index : int = -1

	for i in range(0, _indices.size(), 3):
		var i0 : int = _indices[i]
		var i1 : int = _indices[i + 1]
		var i2 : int = _indices[i + 2]

		if pool_int_arr_contains(v0ei, i0) || pool_int_arr_contains(v0ei, i1) || pool_int_arr_contains(v0ei, i2):
			if pool_int_arr_contains(v1ei, i0) || pool_int_arr_contains(v1ei, i1) || pool_int_arr_contains(v1ei, i2):
				if pool_int_arr_contains(v2ei, i0) || pool_int_arr_contains(v2ei, i1) || pool_int_arr_contains(v2ei, i2):
					return i / 3

	return -1
	*/
}

void MDIGizmo::delete_selected() {
	/*
	if !_mdr:
		return

	if _selected_points.size() == 0:
		return

	if selection_mode == SELECTION_MODE_VERTEX:
		#todo
		pass
	elif selection_mode == SELECTION_MODE_EDGE:
		#todo
		pass
	elif selection_mode == SELECTION_MODE_FACE:
		disable_change_event()

		var orig_arr = copy_arrays(_mdr.array)

		var triangle_indexes : Array = Array()
		for sp in _selected_points:
			var triangle_index : int = find_first_triangle_index_for_face(sp)
			triangle_indexes.append(triangle_index)

		#delete in reverse triangle index order
		triangle_indexes.sort()

		for i in range(triangle_indexes.size() - 1, -1, -1):
			var triangle_index : int = triangle_indexes[i]
			MDREDMeshUtils::remove_triangle(_mdr, triangle_index)

		add_mesh_change_undo_redo(orig_arr, _mdr.array, "Delete")

		_selected_points.resize(0)
		enable_change_event()
	*/
}
void MDIGizmo::generate_normals() {
	/*
	if !_mdr:
		return

	var mdr_arr : Array = _mdr.array

	if mdr_arr.size() != ArrayMesh::ARRAY_MAX || mdr_arr[ArrayMesh::ARRAY_VERTEX] == null || mdr_arr[ArrayMesh::ARRAY_VERTEX].size() == 0:
		return

	disable_change_event()
	var orig_arr = copy_arrays(_mdr.array)
	var orig_seams = copy_pool_int_array(_mdr.seams)

	var seam_points : PoolVector3Array = MDREDMeshUtils::seams_to_points(_mdr)
	MDREDMeshUtils::generate_normals_mdr(_mdr)
	MDREDMeshUtils::points_to_seams(_mdr, seam_points)

	add_mesh_seam_change_undo_redo(orig_arr, orig_seams, _mdr.array, _mdr.seams, "Generate Normals")
	enable_change_event()
	*/
}
void MDIGizmo::generate_tangents() {
	/*
	if !_mdr:
		return

	var mdr_arr : Array = _mdr.array

	if mdr_arr.size() != ArrayMesh::ARRAY_MAX || mdr_arr[ArrayMesh::ARRAY_VERTEX] == null || mdr_arr[ArrayMesh::ARRAY_VERTEX].size() == 0:
		return

	disable_change_event()
	var orig_arr = copy_arrays(_mdr.array)
	var orig_seams = copy_pool_int_array(_mdr.seams)

	var seam_points : PoolVector3Array = MDREDMeshUtils::seams_to_points(_mdr)
	MDREDMeshUtils::generate_tangents(_mdr)
	MDREDMeshUtils::points_to_seams(_mdr, seam_points)

	add_mesh_seam_change_undo_redo(orig_arr, orig_seams, _mdr.array, _mdr.seams, "Generate Tangents")
	enable_change_event()
	*/
}
void MDIGizmo::remove_doubles() {
	/*
	if !_mdr:
		return

	var mdr_arr : Array = _mdr.array

	if mdr_arr.size() != ArrayMesh::ARRAY_MAX || mdr_arr[ArrayMesh::ARRAY_VERTEX] == null || mdr_arr[ArrayMesh::ARRAY_VERTEX].size() == 0:
		return

	disable_change_event()
	var orig_arr = copy_arrays(_mdr.array)
	var orig_seams = copy_pool_int_array(_mdr.seams)

	var seam_points : PoolVector3Array = MDREDMeshUtils::seams_to_points(_mdr)

	var merged_arrays : Array = MeshUtils::remove_doubles(mdr_arr)
	_mdr.array = merged_arrays
	MDREDMeshUtils::points_to_seams(_mdr, seam_points)

	add_mesh_seam_change_undo_redo(orig_arr, orig_seams, _mdr.array, _mdr.seams, "Remove Doubles")
	enable_change_event()
	*/
}
void MDIGizmo::merge_optimize() {
	/*
	if !_mdr:
		return

	var mdr_arr : Array = _mdr.array

	if mdr_arr.size() != ArrayMesh::ARRAY_MAX || mdr_arr[ArrayMesh::ARRAY_VERTEX] == null || mdr_arr[ArrayMesh::ARRAY_VERTEX].size() == 0:
		return

	disable_change_event()
	var orig_arr = copy_arrays(_mdr.array)
	var orig_seams = copy_pool_int_array(_mdr.seams)

	var seam_points : PoolVector3Array = MDREDMeshUtils::seams_to_points(_mdr)

	var merged_arrays : Array = MeshUtils::merge_mesh_array(mdr_arr)
	_mdr.array = merged_arrays
	MDREDMeshUtils::points_to_seams(_mdr, seam_points)

	add_mesh_seam_change_undo_redo(orig_arr, orig_seams, _mdr.array, _mdr.seams, "Merge Optimize")
	enable_change_event()

	*/
}

void MDIGizmo::connect_to_first_selected() {
	/*
	if !_mdr:
		return

	if _selected_points.size() < 2:
		return

	var mdr_arr : Array = _mdr.array

	if mdr_arr.size() != ArrayMesh::ARRAY_MAX || mdr_arr[ArrayMesh::ARRAY_VERTEX] == null || mdr_arr[ArrayMesh::ARRAY_VERTEX].size() == 0:
		return

	disable_change_event()

	var orig_arr = copy_arrays(_mdr.array)

	var vertices : PoolVector3Array = mdr_arr[ArrayMesh::ARRAY_VERTEX]

	if selection_mode == SELECTION_MODE_VERTEX:
		var mpos : Vector3 = _handle_points[_selected_points[0]]

		for i in range(1, _selected_points.size()):
			var ps : PoolIntArray = _handle_to_vertex_map[_selected_points[i]]

			for indx in ps:
				vertices[indx] = mpos

		_selected_points.resize(0)

		mdr_arr[ArrayMesh::ARRAY_VERTEX] = vertices
		_mdr.array = mdr_arr

		add_mesh_change_undo_redo(orig_arr, _mdr.array, "Connect to first selected")
		enable_change_event()
	elif selection_mode == SELECTION_MODE_EDGE:
		pass
	elif selection_mode == SELECTION_MODE_FACE:
		pass
	*/
}
void MDIGizmo::connect_to_avg() {
	/*
	if !_mdr:
		return

	if _selected_points.size() < 2:
		return

	var mdr_arr : Array = _mdr.array

	if mdr_arr.size() != ArrayMesh::ARRAY_MAX || mdr_arr[ArrayMesh::ARRAY_VERTEX] == null || mdr_arr[ArrayMesh::ARRAY_VERTEX].size() == 0:
		return

	disable_change_event()
	var orig_arr = copy_arrays(_mdr.array)

	var vertices : PoolVector3Array = mdr_arr[ArrayMesh::ARRAY_VERTEX]

	if selection_mode == SELECTION_MODE_VERTEX:
		var mpos : Vector3 = Vector3()

		for sp in _selected_points:
			mpos += _handle_points[sp]

		mpos /= _selected_points.size()

		for i in range(_selected_points.size()):
			var ps : PoolIntArray = _handle_to_vertex_map[_selected_points[i]]

			for indx in ps:
				vertices[indx] = mpos

		_selected_points.resize(0)

		mdr_arr[ArrayMesh::ARRAY_VERTEX] = vertices
		_mdr.array = mdr_arr

		add_mesh_change_undo_redo(orig_arr, _mdr.array, "Connect to average")
		enable_change_event()

	elif selection_mode == SELECTION_MODE_EDGE:
		pass
	elif selection_mode == SELECTION_MODE_FACE:
		pass
	*/
}
void MDIGizmo::connect_to_last_selected() {
	/*
	if !_mdr:
		return

	if _selected_points.size() < 2:
		return

	var orig_arr = copy_arrays(_mdr.array)

	var mdr_arr : Array = _mdr.array

	if mdr_arr.size() != ArrayMesh::ARRAY_MAX || mdr_arr[ArrayMesh::ARRAY_VERTEX] == null || mdr_arr[ArrayMesh::ARRAY_VERTEX].size() == 0:
		return

	disable_change_event()

	var vertices : PoolVector3Array = mdr_arr[ArrayMesh::ARRAY_VERTEX]

	if selection_mode == SELECTION_MODE_VERTEX:
		var mpos : Vector3 = _handle_points[_selected_points[_selected_points.size() - 1]]

		for i in range(0, _selected_points.size() - 1):
			var ps : PoolIntArray = _handle_to_vertex_map[_selected_points[i]]

			for indx in ps:
				vertices[indx] = mpos

		_selected_points.resize(0)

		mdr_arr[ArrayMesh::ARRAY_VERTEX] = vertices
		_mdr.array = mdr_arr

		add_mesh_change_undo_redo(orig_arr, _mdr.array, "Connect to last selected")
		enable_change_event()
	elif selection_mode == SELECTION_MODE_EDGE:
		pass
	elif selection_mode == SELECTION_MODE_FACE:
		pass
	*/
}

PoolIntArray MDIGizmo::get_first_index_pair_for_edge(int edge) {
	/*
	var ret : PoolIntArray = PoolIntArray()

	var eisarr : Array = split_edge_indices(edge)

	if eisarr.size() == 0:
		return ret

	# these should have the same size
	var v0ei : PoolIntArray = eisarr[0]
	var v1ei : PoolIntArray = eisarr[1]

	var res : PoolIntArray = PoolIntArray()

	for i in range(0, _indices.size(), 3):
		var i0 : int = _indices[i]
		var i1 : int = _indices[i + 1]
		var i2 : int = _indices[i + 2]

		if pool_int_arr_contains(v0ei, i0) || pool_int_arr_contains(v0ei, i1) || pool_int_arr_contains(v0ei, i2):
			if pool_int_arr_contains(v1ei, i0) || pool_int_arr_contains(v1ei, i1) || pool_int_arr_contains(v1ei, i2):

				if pool_int_arr_contains(v0ei, i0):
					ret.push_back(i0)
				elif pool_int_arr_contains(v0ei, i1):
					ret.push_back(i1)
				elif pool_int_arr_contains(v0ei, i2):
					ret.push_back(i2)

				if pool_int_arr_contains(v1ei, i0):
					ret.push_back(i0)
				elif pool_int_arr_contains(v1ei, i1):
					ret.push_back(i1)
				elif pool_int_arr_contains(v1ei, i2):
					ret.push_back(i2)

				return ret

	return ret
	*/
}
PoolIntArray MDIGizmo::get_all_index_pairs_for_edge(int edge) {
	/*
	var ret : PoolIntArray = PoolIntArray()

	var eisarr : Array = split_edge_indices(edge)

	if eisarr.size() == 0:
		return ret

	# these should have the same size
	var v0ei : PoolIntArray = eisarr[0]
	var v1ei : PoolIntArray = eisarr[1]

	var res : PoolIntArray = PoolIntArray()

	for i in range(0, _indices.size(), 3):
		var i0 : int = _indices[i]
		var i1 : int = _indices[i + 1]
		var i2 : int = _indices[i + 2]

		if pool_int_arr_contains(v0ei, i0) || pool_int_arr_contains(v0ei, i1) || pool_int_arr_contains(v0ei, i2):
			if pool_int_arr_contains(v1ei, i0) || pool_int_arr_contains(v1ei, i1) || pool_int_arr_contains(v1ei, i2):

				if pool_int_arr_contains(v0ei, i0):
					ret.push_back(i0)
				elif pool_int_arr_contains(v0ei, i1):
					ret.push_back(i1)
				elif pool_int_arr_contains(v0ei, i2):
					ret.push_back(i2)

				if pool_int_arr_contains(v1ei, i0):
					ret.push_back(i0)
				elif pool_int_arr_contains(v1ei, i1):
					ret.push_back(i1)
				elif pool_int_arr_contains(v1ei, i2):
					ret.push_back(i2)

	return ret
	*/
}

void MDIGizmo::mark_seam() {
	/*
	if !_mdr:
		return

	if _selected_points.size() == 0:
		return

	if selection_mode == SELECTION_MODE_VERTEX:
		pass
	elif selection_mode == SELECTION_MODE_EDGE:
		disable_change_event()

		var prev_seams : PoolIntArray = copy_pool_int_array(_mdr.seams)

		for se in _selected_points:
			var eis : PoolIntArray = MDREDMeshUtils::order_seam_indices(get_first_index_pair_for_edge(se))

			if eis.size() == 0:
				continue

			MDREDMeshUtils::add_seam(_mdr, eis[0], eis[1])

		_undo_redo.create_action("mark_seam")
		_undo_redo.add_do_method(self, "set_seam", _mdr, copy_pool_int_array(_mdr.seams))
		_undo_redo.add_undo_method(self, "set_seam", _mdr, prev_seams)
		_undo_redo.commit_action()

		enable_change_event()
	elif selection_mode == SELECTION_MODE_FACE:
		pass
	*/
}
void MDIGizmo::unmark_seam() {
	/*
	if !_mdr:
		return

	if _selected_points.size() == 0:
		return

	if selection_mode == SELECTION_MODE_VERTEX:
		pass
	elif selection_mode == SELECTION_MODE_EDGE:
		disable_change_event()

		var prev_seams : PoolIntArray = copy_pool_int_array(_mdr.seams)

		for se in _selected_points:
			var eis : PoolIntArray = MDREDMeshUtils::order_seam_indices(get_all_index_pairs_for_edge(se))

			if eis.size() == 0:
				continue

			MDREDMeshUtils::remove_seam(_mdr, eis[0], eis[1])

		_undo_redo.create_action("unmark_seam")
		_undo_redo.add_do_method(self, "set_seam", _mdr, copy_pool_int_array(_mdr.seams))
		_undo_redo.add_undo_method(self, "set_seam", _mdr, prev_seams)
		_undo_redo.commit_action()

		enable_change_event()
	elif selection_mode == SELECTION_MODE_FACE:
		pass
	*/
}
void MDIGizmo::set_seam(Ref<MeshDataResource> mdr, PoolIntArray arr) {
	/*
	mdr.seams = arr
	*/
}
void MDIGizmo::apply_seam() {
	/*
	if !_mdr:
		return

	disable_change_event()

	var orig_arr : Array = copy_arrays(_mdr.array)
	MDREDMeshUtils::apply_seam(_mdr)
	add_mesh_change_undo_redo(orig_arr, _mdr.array, "apply_seam")

	enable_change_event()
	*/
}

void MDIGizmo::clean_mesh() {
	/*
	if !_mdr:
		return

	var arrays : Array = _mdr.array

	if arrays.size() != ArrayMesh::ARRAY_MAX:
		return arrays

	if arrays[ArrayMesh::ARRAY_VERTEX] == null || arrays[ArrayMesh::ARRAY_INDEX] == null:
		return arrays

	var old_vert_size : int = arrays[ArrayMesh::ARRAY_VERTEX].size()

	disable_change_event()

	var orig_arr : Array = copy_arrays(arrays)
	arrays = MDREDMeshUtils::remove_used_vertices(arrays)
	var new_vert_size : int = arrays[ArrayMesh::ARRAY_VERTEX].size()
	add_mesh_change_undo_redo(orig_arr, arrays, "clean_mesh")

	enable_change_event()

	var d : int = old_vert_size - new_vert_size

	print("MDRED: Removed " + str(d) + " unused vertices.")
	*/
}

void MDIGizmo::uv_unwrap() {
	if (!_mdr.is_valid()) {
		return;
	}

	Array mdr_arr = _mdr->get_array();

	if (mdr_arr.size() != ArrayMesh::ARRAY_MAX || mdr_arr[ArrayMesh::ARRAY_VERTEX].is_null()) {
		return;
	}

	PoolVector3Array verts = mdr_arr[ArrayMesh::ARRAY_VERTEX];

	if (verts.size() == 0) {
		return;
	}

	disable_change_event();

	PoolVector2Array uvs = MeshUtils::get_singleton()->uv_unwrap(mdr_arr);

	if (uvs.size() != verts.size()) {
		ERR_PRINT("Error: Could not unwrap mesh!");
		enable_change_event(false);
		return;
	}

	Array orig_arr = copy_arrays(mdr_arr);

	mdr_arr[ArrayMesh::ARRAY_TEX_UV] = uvs;

	add_mesh_change_undo_redo(orig_arr, mdr_arr, "uv_unwrap");
	enable_change_event();
}
void MDIGizmo::flip_selected_faces() {
	if (!_mdr.is_valid()) {
		return;
	}

	if (_selected_points.size() == 0) {
		return;
	}

	if (selection_mode == SELECTION_MODE_VERTEX) {
	} else if (selection_mode == SELECTION_MODE_EDGE) {
	} else if (selection_mode == SELECTION_MODE_FACE) {
		disable_change_event();

		Array orig_arr = copy_arrays(_mdr->get_array());

		PoolIntArray::Read r = _selected_points.read();

		for (int i = 0; i < _selected_points.size(); ++i) {
			int sp = r[i];

			int triangle_index = find_first_triangle_index_for_face(sp);

			MDREDMeshUtils::flip_triangle_ti(_mdr, triangle_index);
		}

		add_mesh_change_undo_redo(orig_arr, _mdr->get_array(), "Flip Faces");

		enable_change_event();
	}
}

void MDIGizmo::add_mesh_change_undo_redo(Array orig_arr, Array new_arr, String action_name) {
	_undo_redo->create_action(action_name);
	Array nac = copy_arrays(new_arr);
	_undo_redo->add_do_method(this, "apply_mesh_change", _mdr, nac);
	_undo_redo->add_undo_method(this, "apply_mesh_change", _mdr, orig_arr);
	_undo_redo->commit_action();
}
void MDIGizmo::add_mesh_seam_change_undo_redo(Array orig_arr, PoolIntArray orig_seams, Array new_arr, PoolIntArray new_seams, String action_name) {
	_undo_redo->create_action(action_name);
	Array nac = copy_arrays(new_arr);

	_undo_redo->add_do_method(this, "apply_mesh_change", _mdr, nac);
	_undo_redo->add_undo_method(this, "apply_mesh_change", _mdr, orig_arr);

	_undo_redo->add_do_method(this, "set_seam", _mdr, copy_pool_int_array(new_seams));
	_undo_redo->add_undo_method(this, "set_seam", _mdr, orig_seams);

	_undo_redo->commit_action();
}

void MDIGizmo::apply_mesh_change(Ref<MeshDataResource> mdr, Array arr) {
	if (!mdr.is_valid()) {
		return;
	}

	mdr->set_array(copy_arrays(arr));
}
void MDIGizmo::apply_vertex_array(Ref<MeshDataResource> mdr, PoolVector3Array verts) {
	if (!mdr.is_valid()) {
		return;
	}

	Array mdr_arr = mdr->get_array();

	if (mdr_arr.size() != ArrayMesh::ARRAY_MAX) {
		return;
	}

	mdr_arr[ArrayMesh::ARRAY_VERTEX] = verts;
	mdr->set_array(mdr_arr);
}

Array MDIGizmo::copy_arrays(Array arr) {
	return arr.duplicate(true);
}
PoolIntArray MDIGizmo::copy_pool_int_array(PoolIntArray pia) {
	PoolIntArray ret;
	ret.resize(pia.size());

	PoolIntArray::Read r = pia.read();
	PoolIntArray::Write w = ret.write();

	for (int i = 0; i < pia.size(); ++i) {
		w[i] = r[i];
	}

	r.release();
	w.release();

	return ret;
}
PoolVector3Array MDIGizmo::copy_mdr_verts_array() {
	PoolVector3Array ret;

	if (!_mdr.is_valid()) {
		return ret;
	}

	Array mdr_arr = _mdr->get_array();

	if (mdr_arr.size() != ArrayMesh::ARRAY_MAX || mdr_arr[ArrayMesh::ARRAY_VERTEX].is_null()) {
		return ret;
	}

	PoolVector3Array vertices = mdr_arr[ArrayMesh::ARRAY_VERTEX];
	ret.append_array(vertices);

	return ret;
}

void MDIGizmo::setup_op_drag_indices() {
	_drag_op_indices.resize(0);

	PoolIntArray::Read r = _selected_points.read();

	for (int i = 0; i < _selected_points.size(); ++i) {
		int sp = r[i];
		PoolIntArray pi = _handle_to_vertex_map[sp];

		PoolIntArray::Read pir = pi.read();

		for (int j = 0; j < pi.size(); ++j) {
			int indx = pir[j];
			if (!pool_int_arr_contains(_drag_op_indices, indx)) {
				_drag_op_indices.append(indx);
			}
		}

		pir.release();
	}
}
Vector3 MDIGizmo::get_drag_op_pivot() {
	if (pivot_type == PIVOT_TYPE_AVERAGED) {
		Vector3 avg = Vector3();

		PoolIntArray::Read r = _drag_op_indices.read();

		for (int i = 0; i < _drag_op_indices.size(); ++i) {
			avg += _vertices[r[i]];
		}

		r.release();

		avg /= _drag_op_indices.size();

		return avg;
	} else if (pivot_type == PIVOT_TYPE_MDI_ORIGIN) {
		return Vector3();
	} else if (pivot_type == PIVOT_TYPE_WORLD_ORIGIN) {
		return get_spatial_node()->to_local(Vector3());
	}

	return Vector3();
}

void MDIGizmo::select_handle_points(PoolVector3Array points) {
	_selected_points.resize(0);

	PoolVector3Array::Read r = points.read();

	for (int ip = 0; ip < points.size(); ++ip) {
		Vector3 p = r[ip];

		PoolVector3Array::Read hpr = _handle_points.read();

		for (int i = 0; i < _handle_points.size(); ++i) {
			if (is_verts_equal(p, hpr[i])) {
				if (!pool_int_arr_contains(_selected_points, i)) {
					_selected_points.push_back(i);
				}
			}
		}

		hpr.release();
	}

	redraw();
}

void MDIGizmo::set_pivot_averaged() {
	pivot_type = PIVOT_TYPE_AVERAGED;
}
void MDIGizmo::set_pivot_mdi_origin() {
	pivot_type = PIVOT_TYPE_MDI_ORIGIN;
}
void MDIGizmo::set_pivot_world_origin() {
	pivot_type = PIVOT_TYPE_WORLD_ORIGIN;
}

void MDIGizmo::transfer_state_from(const Ref<MDIGizmo> &other) {
	edit_mode = other->edit_mode;
	pivot_type = other->pivot_type;
	axis_constraint = other->axis_constraint;
	selection_mode = other->selection_mode;
	handle_selection_type = other->handle_selection_type;

	visual_indicator_outline = other->visual_indicator_outline;
	visual_indicator_seam = other->visual_indicator_seam;
	visual_indicator_handle = other->visual_indicator_handle;
}

void MDIGizmo::visual_indicator_outline_set(bool on) {
	visual_indicator_outline = on;
	redraw();
}
void MDIGizmo::visual_indicator_seam_set(bool on) {
	visual_indicator_seam = on;
	redraw();
}
void MDIGizmo::visual_indicator_handle_set(bool on) {
	visual_indicator_handle = on;
	redraw();
}

void MDIGizmo::handle_selection_type_front() {
	handle_selection_type = HANDLE_SELECTION_TYPE_FRONT;
}
void MDIGizmo::handle_selection_type_back() {
	handle_selection_type = HANDLE_SELECTION_TYPE_BACK;
}
void MDIGizmo::handle_selection_type_all() {
	handle_selection_type = HANDLE_SELECTION_TYPE_ALL;
}

MDIGizmo::MDIGizmo() {
	gizmo_size = 3.0;

	edit_mode = EDIT_MODE_TRANSLATE;
	pivot_type = PIVOT_TYPE_AVERAGED;
	axis_constraint = AXIS_CONSTRAINT_X | AXIS_CONSTRAINT_Y | AXIS_CONSTRAINT_Z;
	selection_mode = SELECTION_MODE_VERTEX;
	handle_selection_type = HANDLE_SELECTION_TYPE_FRONT;
	visual_indicator_outline = true;
	visual_indicator_seam = true;
	visual_indicator_handle = true;

	_last_known_camera_facing = Vector3(0, 0, -1);

	_rect_drag = false;
	_rect_drag_min_ofset = 10;

	_mesh_outline_generator.instance();

	_handle_drag_op = false;

	_editor_plugin = nullptr;
	_undo_redo = nullptr;
}

MDIGizmo::~MDIGizmo() {
}

void MDIGizmo::_bind_methods() {
}
