#ifndef SPATIAL_EDITOR_GIZMOS_H
#define SPATIAL_EDITOR_GIZMOS_H
/*************************************************************************/
/*  spatial_editor_gizmos.h                                              */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
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

#include "editor/plugins/spatial_editor_plugin.h"

#include "core/color.h"
#include "core/math/basis.h"
#include "core/math/math_defs.h"
#include "core/math/vector2.h"
#include "core/math/vector3.h"
#include "core/object.h"
#include "core/ordered_hash_map.h"
#include "core/pool_vector.h"
#include "core/reference.h"
#include "core/ustring.h"
#include "core/variant.h"
#include "core/vector.h"

#include "scene/resources/mesh.h"

class Camera;
class Spatial;
class Timer;
class Transform;
class Timer;

class EditorSpatialGizmo : public SpatialGizmo {
	GDCLASS(EditorSpatialGizmo, SpatialGizmo);

public:
	struct Instance {
		RID instance;
		Ref<Mesh> mesh;
		Ref<Material> material;
		Ref<SkinReference> skin_reference;
		bool extra_margin;
		Transform xform;

		Instance();
		~Instance();

		void create_instance(Spatial *p_base, bool p_hidden = false);
	};

	bool selected;

	Vector<Vector3> collision_segments;
	Ref<TriangleMesh> collision_mesh;

	Vector<Vector3> handles;
	Vector<int> handle_ids;
	Vector<Vector3> secondary_handles;
	Vector<int> secondary_handle_ids;

	float selectable_icon_size;
	bool billboard_handle;

	bool valid;
	bool hidden;

	Vector<Instance> instances;
	Spatial *spatial_node;

	void _set_spatial_node(Node *p_node) { set_spatial_node(Object::cast_to<Spatial>(p_node)); }

protected:
	static void _bind_methods();

	EditorSpatialGizmoPlugin *gizmo_plugin;

public:
	void add_lines(const Vector<Vector3> &p_lines, const Ref<Material> &p_material, bool p_billboard = false, const Color &p_modulate = Color(1, 1, 1));
	void add_vertices(const Vector<Vector3> &p_vertices, const Ref<Material> &p_material, Mesh::PrimitiveType p_primitive_type, bool p_billboard = false, const Color &p_modulate = Color(1, 1, 1));
	void add_mesh(const Ref<Mesh> &p_mesh, const Ref<Material> &p_material = Ref<Material>(), const Transform &p_xform = Transform(), const Ref<SkinReference> &p_skin_reference = Ref<SkinReference>());
	void add_collision_segments(const Vector<Vector3> &p_lines);
	void add_collision_triangles(const Ref<TriangleMesh> &p_tmesh);
	void add_unscaled_billboard(const Ref<Material> &p_material, float p_scale = 1, const Color &p_modulate = Color(1, 1, 1));
	void add_handles(const Vector<Vector3> &p_handles, const Ref<Material> &p_material, const Vector<int> &p_ids = Vector<int>(), bool p_billboard = false, bool p_secondary = false);
	void add_solid_box(Ref<Material> &p_material, Vector3 p_size, Vector3 p_position = Vector3(), const Transform &p_xform = Transform());

	virtual bool is_handle_highlighted(int p_id) const;
	virtual String get_handle_name(int p_id) const;
	virtual Variant get_handle_value(int p_id);
	virtual void set_handle(int p_id, Camera *p_camera, const Point2 &p_point);
	virtual void commit_handle(int p_id, const Variant &p_restore, bool p_cancel = false);

	virtual int subgizmos_intersect_ray(Camera *p_camera, const Vector2 &p_point) const;
	virtual Vector<int> subgizmos_intersect_frustum(const Camera *p_camera, const Vector<Plane> &p_frustum) const;
	virtual Transform get_subgizmo_transform(int p_id) const;
	virtual void set_subgizmo_transform(int p_id, Transform p_transform) const;
	virtual void commit_subgizmos(const Vector<int> &p_ids, const Vector<Transform> &p_restore, bool p_cancel = false) const;

	void set_selected(bool p_selected) { selected = p_selected; }
	bool is_selected() const { return selected; }

	void set_spatial_node(Spatial *p_node);
	Spatial *get_spatial_node() const { return spatial_node; }
	Ref<EditorSpatialGizmoPlugin> get_plugin() const { return gizmo_plugin; }
	bool intersect_frustum(const Camera *p_camera, const Vector<Plane> &p_frustum);
	void handles_intersect_ray(Camera *p_camera, const Vector2 &p_point, bool p_shift_pressed, int &r_id);
	bool intersect_ray(Camera *p_camera, const Point2 &p_point, Vector3 &r_pos, Vector3 &r_normal);
	bool is_subgizmo_selected(int p_id) const;
	Vector<int> get_subgizmo_selection() const;

	virtual void clear();
	virtual void create();
	virtual void transform();
	virtual void redraw();
	virtual void free();

	virtual bool is_editable() const;

	void set_hidden(bool p_hidden);
	void set_plugin(EditorSpatialGizmoPlugin *p_plugin);

	EditorSpatialGizmo();
	~EditorSpatialGizmo();
};

class EditorSpatialGizmoPlugin : public Resource {
	GDCLASS(EditorSpatialGizmoPlugin, Resource);

public:
	static const int VISIBLE = 0;
	static const int HIDDEN = 1;
	static const int ON_TOP = 2;

protected:
	int current_state;
	List<EditorSpatialGizmo *> current_gizmos;
	HashMap<String, Vector<Ref<SpatialMaterial>>> materials;

	static void _bind_methods();
	virtual bool has_gizmo(Spatial *p_spatial);
	virtual Ref<EditorSpatialGizmo> create_gizmo(Spatial *p_spatial);

public:
	void create_material(const String &p_name, const Color &p_color, bool p_billboard = false, bool p_on_top = false, bool p_use_vertex_color = false);
	void create_icon_material(const String &p_name, const Ref<Texture> &p_texture, bool p_on_top = false, const Color &p_albedo = Color(1, 1, 1, 1));
	void create_handle_material(const String &p_name, bool p_billboard = false, const Ref<Texture> &p_icon = nullptr);
	void add_material(const String &p_name, Ref<SpatialMaterial> p_material);

	Ref<SpatialMaterial> get_material(const String &p_name, const Ref<EditorSpatialGizmo> &p_gizmo = Ref<EditorSpatialGizmo>());

	virtual String get_gizmo_name() const;
	virtual int get_priority() const;
	virtual bool can_be_hidden() const;
	virtual bool is_selectable_when_hidden() const;

	virtual void redraw(EditorSpatialGizmo *p_gizmo);
	virtual bool is_handle_highlighted(const EditorSpatialGizmo *p_gizmo, int p_id) const;
	virtual String get_handle_name(const EditorSpatialGizmo *p_gizmo, int p_id) const;
	virtual Variant get_handle_value(EditorSpatialGizmo *p_gizmo, int p_id) const;
	virtual void set_handle(EditorSpatialGizmo *p_gizmo, int p_id, Camera *p_camera, const Point2 &p_point);
	virtual void commit_handle(EditorSpatialGizmo *p_gizmo, int p_id, const Variant &p_restore, bool p_cancel = false);
	
	virtual int subgizmos_intersect_ray(const EditorSpatialGizmo *p_gizmo, Camera *p_camera, const Vector2 &p_point) const;
	virtual Vector<int> subgizmos_intersect_frustum(const EditorSpatialGizmo *p_gizmo, const Camera *p_camera, const Vector<Plane> &p_frustum) const;
	virtual Transform get_subgizmo_transform(const EditorSpatialGizmo *p_gizmo, int p_id) const;
	virtual void set_subgizmo_transform(const EditorSpatialGizmo *p_gizmo, int p_id, Transform p_transform);
	virtual void commit_subgizmos(const EditorSpatialGizmo *p_gizmo, const Vector<int> &p_ids, const Vector<Transform> &p_restore, bool p_cancel = false);


	Ref<EditorSpatialGizmo> get_gizmo(Spatial *p_spatial);
	void set_state(int p_state);
	int get_state() const;
	void unregister_gizmo(EditorSpatialGizmo *p_gizmo);

	EditorSpatialGizmoPlugin();
	virtual ~EditorSpatialGizmoPlugin();
};

class LightSpatialGizmoPlugin : public EditorSpatialGizmoPlugin {
	GDCLASS(LightSpatialGizmoPlugin, EditorSpatialGizmoPlugin);

public:
	bool has_gizmo(Spatial *p_spatial);
	String get_gizmo_name() const;
	int get_priority() const;

	String get_handle_name(const EditorSpatialGizmo *p_gizmo, int p_id) const;
	Variant get_handle_value(EditorSpatialGizmo *p_gizmo, int p_id) const;
	void set_handle(EditorSpatialGizmo *p_gizmo, int p_id, Camera *p_camera, const Point2 &p_point);
	void commit_handle(EditorSpatialGizmo *p_gizmo, int p_id, const Variant &p_restore, bool p_cancel = false);
	void redraw(EditorSpatialGizmo *p_gizmo);

	LightSpatialGizmoPlugin();
};

class AudioStreamPlayer3DSpatialGizmoPlugin : public EditorSpatialGizmoPlugin {
	GDCLASS(AudioStreamPlayer3DSpatialGizmoPlugin, EditorSpatialGizmoPlugin);

public:
	bool has_gizmo(Spatial *p_spatial);
	String get_gizmo_name() const;
	int get_priority() const;

	String get_handle_name(const EditorSpatialGizmo *p_gizmo, int p_id) const;
	Variant get_handle_value(EditorSpatialGizmo *p_gizmo, int p_id) const;
	void set_handle(EditorSpatialGizmo *p_gizmo, int p_id, Camera *p_camera, const Point2 &p_point);
	void commit_handle(EditorSpatialGizmo *p_gizmo, int p_id, const Variant &p_restore, bool p_cancel = false);
	void redraw(EditorSpatialGizmo *p_gizmo);

	AudioStreamPlayer3DSpatialGizmoPlugin();
};

class ListenerSpatialGizmoPlugin : public EditorSpatialGizmoPlugin {
	GDCLASS(ListenerSpatialGizmoPlugin, EditorSpatialGizmoPlugin);

public:
	bool has_gizmo(Spatial *p_spatial);
	String get_gizmo_name() const;
	int get_priority() const;

	void redraw(EditorSpatialGizmo *p_gizmo);

	ListenerSpatialGizmoPlugin();
};

class CameraSpatialGizmoPlugin : public EditorSpatialGizmoPlugin {
	GDCLASS(CameraSpatialGizmoPlugin, EditorSpatialGizmoPlugin);

public:
	bool has_gizmo(Spatial *p_spatial);
	String get_gizmo_name() const;
	int get_priority() const;

	String get_handle_name(const EditorSpatialGizmo *p_gizmo, int p_id) const;
	Variant get_handle_value(EditorSpatialGizmo *p_gizmo, int p_id) const;
	void set_handle(EditorSpatialGizmo *p_gizmo, int p_id, Camera *p_camera, const Point2 &p_point);
	void commit_handle(EditorSpatialGizmo *p_gizmo, int p_id, const Variant &p_restore, bool p_cancel = false);
	void redraw(EditorSpatialGizmo *p_gizmo);

	CameraSpatialGizmoPlugin();
};

class MeshInstanceSpatialGizmoPlugin : public EditorSpatialGizmoPlugin {
	GDCLASS(MeshInstanceSpatialGizmoPlugin, EditorSpatialGizmoPlugin);

public:
	bool has_gizmo(Spatial *p_spatial);
	String get_gizmo_name() const;
	int get_priority() const;
	bool can_be_hidden() const;
	void redraw(EditorSpatialGizmo *p_gizmo);

	MeshInstanceSpatialGizmoPlugin();
};

class Sprite3DSpatialGizmoPlugin : public EditorSpatialGizmoPlugin {
	GDCLASS(Sprite3DSpatialGizmoPlugin, EditorSpatialGizmoPlugin);

public:
	bool has_gizmo(Spatial *p_spatial);
	String get_gizmo_name() const;
	int get_priority() const;
	bool can_be_hidden() const;
	void redraw(EditorSpatialGizmo *p_gizmo);

	Sprite3DSpatialGizmoPlugin();
};

class Label3DSpatialGizmoPlugin : public EditorSpatialGizmoPlugin {
	GDCLASS(Label3DSpatialGizmoPlugin, EditorSpatialGizmoPlugin);

public:
	bool has_gizmo(Spatial *p_spatial);
	String get_gizmo_name() const;
	int get_priority() const;
	bool can_be_hidden() const;
	void redraw(EditorSpatialGizmo *p_gizmo);

	Label3DSpatialGizmoPlugin();
};

class Position3DSpatialGizmoPlugin : public EditorSpatialGizmoPlugin {
	GDCLASS(Position3DSpatialGizmoPlugin, EditorSpatialGizmoPlugin);

	Ref<ArrayMesh> pos3d_mesh;
	Vector<Vector3> cursor_points;

public:
	bool has_gizmo(Spatial *p_spatial);
	String get_gizmo_name() const;
	int get_priority() const;
	void redraw(EditorSpatialGizmo *p_gizmo);

	Position3DSpatialGizmoPlugin();
};

class PhysicalBoneSpatialGizmoPlugin : public EditorSpatialGizmoPlugin {
	GDCLASS(PhysicalBoneSpatialGizmoPlugin, EditorSpatialGizmoPlugin);

public:
	bool has_gizmo(Spatial *p_spatial);
	String get_gizmo_name() const;
	int get_priority() const;
	void redraw(EditorSpatialGizmo *p_gizmo);

	PhysicalBoneSpatialGizmoPlugin();
};

class RayCastSpatialGizmoPlugin : public EditorSpatialGizmoPlugin {
	GDCLASS(RayCastSpatialGizmoPlugin, EditorSpatialGizmoPlugin);

public:
	bool has_gizmo(Spatial *p_spatial);
	String get_gizmo_name() const;
	int get_priority() const;
	void redraw(EditorSpatialGizmo *p_gizmo);

	RayCastSpatialGizmoPlugin();
};

class SpringArmSpatialGizmoPlugin : public EditorSpatialGizmoPlugin {
	GDCLASS(SpringArmSpatialGizmoPlugin, EditorSpatialGizmoPlugin);

public:
	bool has_gizmo(Spatial *p_spatial);
	String get_gizmo_name() const;
	int get_priority() const;
	void redraw(EditorSpatialGizmo *p_gizmo);

	SpringArmSpatialGizmoPlugin();
};

class VehicleWheelSpatialGizmoPlugin : public EditorSpatialGizmoPlugin {
	GDCLASS(VehicleWheelSpatialGizmoPlugin, EditorSpatialGizmoPlugin);

public:
	bool has_gizmo(Spatial *p_spatial);
	String get_gizmo_name() const;
	int get_priority() const;
	void redraw(EditorSpatialGizmo *p_gizmo);

	VehicleWheelSpatialGizmoPlugin();
};

class SoftBodySpatialGizmoPlugin : public EditorSpatialGizmoPlugin {
	GDCLASS(SoftBodySpatialGizmoPlugin, EditorSpatialGizmoPlugin);

public:
	bool has_gizmo(Spatial *p_spatial);
	String get_gizmo_name() const;
	int get_priority() const;
	bool is_selectable_when_hidden() const;
	void redraw(EditorSpatialGizmo *p_gizmo);

	String get_handle_name(const EditorSpatialGizmo *p_gizmo, int p_id) const;
	Variant get_handle_value(EditorSpatialGizmo *p_gizmo, int p_id) const;
	void commit_handle(EditorSpatialGizmo *p_gizmo, int p_id, const Variant &p_restore, bool p_cancel);
	bool is_handle_highlighted(const EditorSpatialGizmo *p_gizmo, int idx) const;

	SoftBodySpatialGizmoPlugin();
};

class VisibilityNotifierGizmoPlugin : public EditorSpatialGizmoPlugin {
	GDCLASS(VisibilityNotifierGizmoPlugin, EditorSpatialGizmoPlugin);

public:
	bool has_gizmo(Spatial *p_spatial);
	String get_gizmo_name() const;
	int get_priority() const;
	void redraw(EditorSpatialGizmo *p_gizmo);

	String get_handle_name(const EditorSpatialGizmo *p_gizmo, int p_id) const;
	Variant get_handle_value(EditorSpatialGizmo *p_gizmo, int p_id) const;
	void set_handle(EditorSpatialGizmo *p_gizmo, int p_id, Camera *p_camera, const Point2 &p_point);
	void commit_handle(EditorSpatialGizmo *p_gizmo, int p_id, const Variant &p_restore, bool p_cancel = false);

	VisibilityNotifierGizmoPlugin();
};

class CPUParticlesGizmoPlugin : public EditorSpatialGizmoPlugin {
	GDCLASS(CPUParticlesGizmoPlugin, EditorSpatialGizmoPlugin);

public:
	bool has_gizmo(Spatial *p_spatial);
	String get_gizmo_name() const;
	int get_priority() const;
	bool is_selectable_when_hidden() const;
	void redraw(EditorSpatialGizmo *p_gizmo);
	CPUParticlesGizmoPlugin();
};

class ReflectionProbeGizmoPlugin : public EditorSpatialGizmoPlugin {
	GDCLASS(ReflectionProbeGizmoPlugin, EditorSpatialGizmoPlugin);

public:
	bool has_gizmo(Spatial *p_spatial);
	String get_gizmo_name() const;
	int get_priority() const;
	void redraw(EditorSpatialGizmo *p_gizmo);

	String get_handle_name(const EditorSpatialGizmo *p_gizmo, int p_id) const;
	Variant get_handle_value(EditorSpatialGizmo *p_gizmo, int p_id) const;
	void set_handle(EditorSpatialGizmo *p_gizmo, int p_id, Camera *p_camera, const Point2 &p_point);
	void commit_handle(EditorSpatialGizmo *p_gizmo, int p_id, const Variant &p_restore, bool p_cancel = false);

	ReflectionProbeGizmoPlugin();
};

class CollisionObjectGizmoPlugin : public EditorSpatialGizmoPlugin {
	GDCLASS(CollisionObjectGizmoPlugin, EditorSpatialGizmoPlugin);

public:
	bool has_gizmo(Spatial *p_spatial);
	String get_gizmo_name() const;
	int get_priority() const;
	void redraw(EditorSpatialGizmo *p_gizmo);

	CollisionObjectGizmoPlugin();
};

class CollisionShapeSpatialGizmoPlugin : public EditorSpatialGizmoPlugin {
	GDCLASS(CollisionShapeSpatialGizmoPlugin, EditorSpatialGizmoPlugin);

public:
	bool has_gizmo(Spatial *p_spatial);
	String get_gizmo_name() const;
	int get_priority() const;
	void redraw(EditorSpatialGizmo *p_gizmo);

	String get_handle_name(const EditorSpatialGizmo *p_gizmo, int p_id) const;
	Variant get_handle_value(EditorSpatialGizmo *p_gizmo, int p_id) const;
	void set_handle(EditorSpatialGizmo *p_gizmo, int p_id, Camera *p_camera, const Point2 &p_point);
	void commit_handle(EditorSpatialGizmo *p_gizmo, int p_id, const Variant &p_restore, bool p_cancel = false);

	CollisionShapeSpatialGizmoPlugin();
};

class CollisionPolygonSpatialGizmoPlugin : public EditorSpatialGizmoPlugin {
	GDCLASS(CollisionPolygonSpatialGizmoPlugin, EditorSpatialGizmoPlugin);

public:
	bool has_gizmo(Spatial *p_spatial);
	String get_gizmo_name() const;
	int get_priority() const;
	void redraw(EditorSpatialGizmo *p_gizmo);
	CollisionPolygonSpatialGizmoPlugin();
};

class NavigationMeshSpatialGizmoPlugin : public EditorSpatialGizmoPlugin {
	GDCLASS(NavigationMeshSpatialGizmoPlugin, EditorSpatialGizmoPlugin);

	struct _EdgeKey {
		Vector3 from;
		Vector3 to;

		bool operator<(const _EdgeKey &p_with) const { return from == p_with.from ? to < p_with.to : from < p_with.from; }
	};

public:
	bool has_gizmo(Spatial *p_spatial);
	String get_gizmo_name() const;
	int get_priority() const;
	void redraw(EditorSpatialGizmo *p_gizmo);

	NavigationMeshSpatialGizmoPlugin();
};

class JointGizmosDrawer {
public:
	static Basis look_body(const Transform &p_joint_transform, const Transform &p_body_transform);
	static Basis look_body_toward(Vector3::Axis p_axis, const Transform &joint_transform, const Transform &body_transform);
	static Basis look_body_toward_x(const Transform &p_joint_transform, const Transform &p_body_transform);
	static Basis look_body_toward_y(const Transform &p_joint_transform, const Transform &p_body_transform);
	/// Special function just used for physics joints, it returns a basis constrained toward Joint Z axis
	/// with axis X and Y that are looking toward the body and oriented toward up
	static Basis look_body_toward_z(const Transform &p_joint_transform, const Transform &p_body_transform);

	// Draw circle around p_axis
	static void draw_circle(Vector3::Axis p_axis, real_t p_radius, const Transform &p_offset, const Basis &p_base, real_t p_limit_lower, real_t p_limit_upper, Vector<Vector3> &r_points, bool p_inverse = false);
	static void draw_cone(const Transform &p_offset, const Basis &p_base, real_t p_swing, real_t p_twist, Vector<Vector3> &r_points);
};

class JointSpatialGizmoPlugin : public EditorSpatialGizmoPlugin {
	GDCLASS(JointSpatialGizmoPlugin, EditorSpatialGizmoPlugin);

	Timer *update_timer;
	uint64_t update_idx = 0;

	void incremental_update_gizmos();

protected:
	static void _bind_methods();

public:
	bool has_gizmo(Spatial *p_spatial);
	String get_gizmo_name() const;
	int get_priority() const;
	void redraw(EditorSpatialGizmo *p_gizmo);

	static void CreatePinJointGizmo(const Transform &p_offset, Vector<Vector3> &r_cursor_points);
	static void CreateHingeJointGizmo(const Transform &p_offset, const Transform &p_trs_joint, const Transform &p_trs_body_a, const Transform &p_trs_body_b, real_t p_limit_lower, real_t p_limit_upper, bool p_use_limit, Vector<Vector3> &r_common_points, Vector<Vector3> *r_body_a_points, Vector<Vector3> *r_body_b_points);
	static void CreateSliderJointGizmo(const Transform &p_offset, const Transform &p_trs_joint, const Transform &p_trs_body_a, const Transform &p_trs_body_b, real_t p_angular_limit_lower, real_t p_angular_limit_upper, real_t p_linear_limit_lower, real_t p_linear_limit_upper, Vector<Vector3> &r_points, Vector<Vector3> *r_body_a_points, Vector<Vector3> *r_body_b_points);
	static void CreateConeTwistJointGizmo(const Transform &p_offset, const Transform &p_trs_joint, const Transform &p_trs_body_a, const Transform &p_trs_body_b, real_t p_swing, real_t p_twist, Vector<Vector3> *r_body_a_points, Vector<Vector3> *r_body_b_points);
	static void CreateGeneric6DOFJointGizmo(
			const Transform &p_offset,
			const Transform &p_trs_joint,
			const Transform &p_trs_body_a,
			const Transform &p_trs_body_b,
			real_t p_angular_limit_lower_x,
			real_t p_angular_limit_upper_x,
			real_t p_linear_limit_lower_x,
			real_t p_linear_limit_upper_x,
			bool p_enable_angular_limit_x,
			bool p_enable_linear_limit_x,
			real_t p_angular_limit_lower_y,
			real_t p_angular_limit_upper_y,
			real_t p_linear_limit_lower_y,
			real_t p_linear_limit_upper_y,
			bool p_enable_angular_limit_y,
			bool p_enable_linear_limit_y,
			real_t p_angular_limit_lower_z,
			real_t p_angular_limit_upper_z,
			real_t p_linear_limit_lower_z,
			real_t p_linear_limit_upper_z,
			bool p_enable_angular_limit_z,
			bool p_enable_linear_limit_z,
			Vector<Vector3> &r_points,
			Vector<Vector3> *r_body_a_points,
			Vector<Vector3> *r_body_b_points);

	JointSpatialGizmoPlugin();
};

class Room;

class RoomSpatialGizmo : public EditorSpatialGizmo {
	GDCLASS(RoomSpatialGizmo, EditorSpatialGizmo);

	Room *_room = nullptr;

public:
	virtual String get_handle_name(int p_id) const;
	virtual Variant get_handle_value(int p_id);
	virtual void set_handle(int p_id, Camera *p_camera, const Point2 &p_point);
	virtual void commit_handle(int p_id, const Variant &p_restore, bool p_cancel = false);
	virtual void redraw();

	RoomSpatialGizmo(Room *p_room = nullptr);
};

class RoomGizmoPlugin : public EditorSpatialGizmoPlugin {
	GDCLASS(RoomGizmoPlugin, EditorSpatialGizmoPlugin);

protected:
	virtual bool has_gizmo(Spatial *p_spatial);
	String get_gizmo_name() const;
	int get_priority() const;
	Ref<EditorSpatialGizmo> create_gizmo(Spatial *p_spatial);

public:
	RoomGizmoPlugin();
};

class Portal;

class PortalSpatialGizmo : public EditorSpatialGizmo {
	GDCLASS(PortalSpatialGizmo, EditorSpatialGizmo);

	Portal *_portal = nullptr;
	Color _color_portal_front;
	Color _color_portal_back;

public:
	virtual String get_handle_name(int p_id) const;
	virtual Variant get_handle_value(int p_id);
	virtual void set_handle(int p_id, Camera *p_camera, const Point2 &p_point);
	virtual void commit_handle(int p_id, const Variant &p_restore, bool p_cancel = false);
	virtual void redraw();

	PortalSpatialGizmo(Portal *p_portal = nullptr);
};

class PortalGizmoPlugin : public EditorSpatialGizmoPlugin {
	GDCLASS(PortalGizmoPlugin, EditorSpatialGizmoPlugin);

protected:
	virtual bool has_gizmo(Spatial *p_spatial);
	String get_gizmo_name() const;
	int get_priority() const;
	Ref<EditorSpatialGizmo> create_gizmo(Spatial *p_spatial);

public:
	PortalGizmoPlugin();
};

class Occluder;
class OccluderShape;
class OccluderShapeSphere;
class OccluderShapePolygon;

class OccluderSpatialGizmo : public EditorSpatialGizmo {
	GDCLASS(OccluderSpatialGizmo, EditorSpatialGizmo);

	Occluder *_occluder = nullptr;

	const OccluderShape *get_occluder_shape() const;
	const OccluderShapeSphere *get_occluder_shape_sphere() const;
	const OccluderShapePolygon *get_occluder_shape_poly() const;
	OccluderShape *get_occluder_shape();
	OccluderShapeSphere *get_occluder_shape_sphere();
	OccluderShapePolygon *get_occluder_shape_poly();

	Color _color_poly_front;
	Color _color_poly_back;
	Color _color_hole;

	void _redraw_poly(bool p_hole, const Vector<Vector2> &p_pts, const PoolVector<Vector2> &p_pts_raw);

public:
	virtual String get_handle_name(int p_id) const;
	virtual Variant get_handle_value(int p_id);
	virtual void set_handle(int p_id, Camera *p_camera, const Point2 &p_point);
	virtual void commit_handle(int p_id, const Variant &p_restore, bool p_cancel = false);
	virtual void redraw();

	OccluderSpatialGizmo(Occluder *p_occluder = nullptr);
};

class OccluderGizmoPlugin : public EditorSpatialGizmoPlugin {
	GDCLASS(OccluderGizmoPlugin, EditorSpatialGizmoPlugin);

protected:
	virtual bool has_gizmo(Spatial *p_spatial);
	String get_gizmo_name() const;
	int get_priority() const;
	Ref<EditorSpatialGizmo> create_gizmo(Spatial *p_spatial);

public:
	OccluderGizmoPlugin();
};

#endif // SPATIAL_EDITOR_GIZMOS_H
