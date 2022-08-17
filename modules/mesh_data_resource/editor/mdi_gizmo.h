#ifndef MDI_GIZMO_H
#define MDI_GIZMO_H

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

#include "editor/spatial_editor_gizmos.h"

#include "core/math/basis.h"
#include "core/math/transform.h"
#include "core/math/vector3.h"
#include "core/containers/pool_vector.h"
#include "core/reference.h"
#include "core/variant/variant.h"

class Camera;
class MeshDataResource;
class MDREDMeshOutline;
class InputEvent;
class EditorPlugin;
class UndoRedo;

class MDIGizmo : public EditorSpatialGizmo {
	GDCLASS(MDIGizmo, EditorSpatialGizmo);

public:
	enum EditMode {
		EDIT_MODE_NONE = 0,
		EDIT_MODE_TRANSLATE = 1,
		EDIT_MODE_SCALE = 2,
		EDIT_MODE_ROTATE = 3
	};

	enum AxisConstraint {
		AXIS_CONSTRAINT_X = 1 << 0,
		AXIS_CONSTRAINT_Y = 1 << 1,
		AXIS_CONSTRAINT_Z = 1 << 2,
	};

	enum SelectionMode {
		SELECTION_MODE_VERTEX = 0,
		SELECTION_MODE_EDGE = 1,
		SELECTION_MODE_FACE = 2,
	};

	enum PivotTypes {
		PIVOT_TYPE_AVERAGED = 0,
		PIVOT_TYPE_MDI_ORIGIN = 1,
		PIVOT_TYPE_WORLD_ORIGIN = 2,
	};

	enum HandleSelectionType {
		HANDLE_SELECTION_TYPE_FRONT = 0,
		HANDLE_SELECTION_TYPE_BACK = 1,
		HANDLE_SELECTION_TYPE_ALL = 2,
	};

	void setup();
	void set_editor_plugin(EditorPlugin *editor_plugin);

	void set_handle(int index, bool secondary, Camera *camera, const Point2 &point);
	void redraw();
	void apply();

	void select_all();

	bool selection_click(Camera *camera, const Ref<InputEventMouse> &event);
	bool is_point_visible(const Vector3 &point_orig, const Vector3 &camera_pos, const Transform &gt);

	bool selection_click_select_front_or_back(Camera *camera, const Ref<InputEventMouse> &event);
	bool selection_click_select_through(Camera *camera, const Ref<InputEventMouse> &event);
	void selection_drag(Camera *camera, const Ref<InputEventMouse> &event);
	void selection_drag_rect_select_front_back(Camera *camera, const Ref<InputEventMouse> &event);
	void selection_drag_rect_select_through(Camera *camera, const Ref<InputEventMouse> &event);
	EditorPlugin::AfterGUIInput forward_spatial_gui_input(Camera *camera, const Ref<InputEvent> &event);
	void add_to_all_selected(const Vector3 &ofs);

	void mul_all_selected_with_basis(const Basis &b);
	void mul_all_selected_with_transform(const Transform &t);
	void mul_all_selected_with_transform_acc(const Transform &t);

	void set_translate();
	void set_scale();
	void set_rotate();
	void set_edit_mode(const int em);

	void set_axis_x(const bool on);
	void set_axis_y(const bool on);
	void set_axis_z(const bool on);

	void set_selection_mode_vertex();
	void set_selection_mode_edge();
	void set_selection_mode_face();
	
	void recalculate_handle_points();
	void on_mesh_data_resource_changed(Ref<MeshDataResource> mdr);
	void on_mdr_changed();
	void disable_change_event();
	void enable_change_event(const bool update = true);
	void add_triangle();
	void add_quad();

	bool is_verts_equal(const Vector3 &v0, const Vector3 &v1);
	Vector3 find_other_vertex_for_edge(const int edge, const Vector3 &v0);
	Vector<PoolIntArray> split_edge_indices(const int edge);
	bool pool_int_arr_contains(const PoolIntArray &arr, const int val);
	PoolIntArray find_triangles_for_edge(const int edge);
	int find_first_triangle_for_edge(const int edge);
	void add_triangle_to_edge(const int edge);
	void add_quad_to_edge(const int edge);
	void add_triangle_at();
	void add_quad_at();

	void extrude();
	void add_box();
	void split();
	void disconnect_action();
	int get_first_triangle_index_for_vertex(const int indx);

	void create_face();

	Vector<PoolIntArray> split_face_indices(const int face);
	int find_first_triangle_index_for_face(const int face);

	void delete_selected();
	void generate_normals();
	void generate_tangents();
	void remove_doubles();
	void merge_optimize();

	void connect_to_first_selected();
	void connect_to_avg();
	void connect_to_last_selected();

	PoolIntArray get_first_index_pair_for_edge(const int edge);
	PoolIntArray get_all_index_pairs_for_edge(const int edge);

	void mark_seam();
	void unmark_seam();
	void set_seam(Ref<MeshDataResource> mdr, PoolIntArray arr);
	void apply_seam();

	void clean_mesh();

	void uv_unwrap();
	void flip_selected_faces();

	void add_mesh_change_undo_redo(const Array &orig_arr, const Array &new_arr, const String &action_name);
	void add_mesh_seam_change_undo_redo(const Array &orig_arr, const PoolIntArray &orig_seams, const Array &new_arr, const PoolIntArray &new_seams, const String &action_name);

	void apply_mesh_change(Ref<MeshDataResource> mdr, const Array &arr);
	void apply_vertex_array(Ref<MeshDataResource> mdr, const PoolVector3Array &verts);

	Array copy_arrays(const Array &arr);
	PoolIntArray copy_pool_int_array(const PoolIntArray &pia);
	PoolVector3Array copy_mdr_verts_array();

	void setup_op_drag_indices();
	Vector3 get_drag_op_pivot();

	void select_handle_points(const PoolVector3Array &points);

	void set_pivot_averaged();
	void set_pivot_mdi_origin();
	void set_pivot_world_origin();

	void transfer_state_from(const Ref<MDIGizmo> &other);

	void visual_indicator_outline_set(const bool on);
	void visual_indicator_seam_set(const bool on);
	void visual_indicator_handle_set(const bool on);

	void handle_selection_type_front();
	void handle_selection_type_back();
	void handle_selection_type_all();

	MDIGizmo();
	~MDIGizmo();

	float gizmo_size;

	int edit_mode;
	int pivot_type;
	int axis_constraint;
	int selection_mode;
	int handle_selection_type;
	bool visual_indicator_outline;
	bool visual_indicator_seam;
	bool visual_indicator_handle;

	Vector2 previous_point;
	Vector3 _last_known_camera_facing;

	bool _rect_drag;
	Vector2 _rect_drag_start_point;
	float _rect_drag_min_ofset;

	Ref<MeshDataResource> _mdr;

	PoolVector3Array _vertices;
	PoolIntArray _indices;
	PoolVector3Array _handle_points;
	Vector<PoolIntArray> _handle_to_vertex_map;
	PoolIntArray _selected_points;

	Ref<MDREDMeshOutline> _mesh_outline_generator;

	bool _handle_drag_op;
	PoolVector3Array _drag_op_orig_verices;
	PoolIntArray _drag_op_indices;
	Vector3 _drag_op_accumulator;
	Quaternion _drag_op_accumulator_quat;
	Vector3 _drag_op_pivot;

	EditorPlugin *_editor_plugin;
	UndoRedo *_undo_redo;

protected:
	static void _bind_methods();
};

#endif
