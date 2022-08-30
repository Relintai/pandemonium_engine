#ifndef WORLD_H
#define WORLD_H

#include "core/containers/vector.h"
#include "core/object/reference.h"

#include "node.h"

class Camera;
class World3D;
class World2D;

class World : public Node {
	GDCLASS(World, Node);

public:
	Camera *get_camera() const;

	Ref<World2D> get_world_2d() const;
	void set_world_2d(const Ref<World2D> &p_world_2d);

	Ref<World3D> get_world_3d() const;
	void set_world_3d(const Ref<World3D> &p_world);

	bool is_using_own_world_3d() const;
	void set_use_own_world_3d(const bool p_use_own_world_3d);

	bool get_override_in_parent_viewport();
	void set_override_in_parent_viewport(const bool value);

	Ref<World2D> find_world_2d() const;
	Ref<World3D> find_world_3d() const;
	Ref<World2D> find_world_2d_no_override() const;
	Ref<World3D> find_world_3d_no_override() const;

	World *get_override_world();
	World *get_override_world_or_this();
	void set_override_world(World *p_world);
	void set_override_world_bind(Node *p_world);

	virtual void gui_reset_canvas_sort_index();
	virtual int gui_get_canvas_sort_index();

	virtual void enable_canvas_transform_override(bool p_enable);
	virtual bool is_canvas_transform_override_enbled() const;

	virtual void set_canvas_transform_override(const Transform2D &p_transform);
	virtual Transform2D get_canvas_transform_override() const;

	virtual void set_canvas_transform(const Transform2D &p_transform);
	virtual Transform2D get_canvas_transform() const;

	virtual void set_global_canvas_transform(const Transform2D &p_transform);
	virtual Transform2D get_global_canvas_transform() const;

	virtual Transform2D get_final_transform() const;

	virtual Rect2 get_visible_rect() const;

	virtual RID get_viewport_rid() const;

	Vector2 get_camera_coords(const Vector2 &p_viewport_coords) const;
	Vector2 get_camera_rect_size() const;

	void update_worlds();

	void _world_3d_register_camera(Camera *p_camera);
	void _world_3d_remove_camera(Camera *p_camera);
	void _world_3d_register_camera_no_override(Camera *p_camera);
	void _world_3d_remove_camera_no_override(Camera *p_camera);
	void _world_3d_register_camera_as_override(Camera *p_camera);
	void _world_3d_remove_camera_as_override(Camera *p_camera);
	void _clear_override_cameras();
	void _add_override_cameras_into(World *p_from);

	World();
	~World();

	//friend class Camera;
	void _camera_transform_changed_notify();
	virtual void _camera_set(Camera *p_camera);
	bool _camera_add(Camera *p_camera); //true if first
	void _camera_remove(Camera *p_camera);
	void _camera_make_next_current(Camera *p_exclude);

	virtual void _update_listener();
	virtual void _update_listener_2d();

protected:
	void _add_overridden_world(World *p_world);
	void _remove_overridden_world(World *p_world);

	virtual void _own_world_3d_changed();
	virtual void _on_set_use_own_world_3d(bool p_use_own_world_3d);
	virtual void _on_set_world_3d(const Ref<World3D> &p_old_world);
	virtual void _on_set_world_2d(const Ref<World2D> &p_old_world_2d);

	virtual void _on_before_world_override_changed();
	virtual void _on_after_world_override_changed();

	void _propagate_enter_world(Node *p_node);
	void _propagate_exit_world(Node *p_node);

	void _notification(int p_what);

	static void _bind_methods();

	Ref<World2D> world_2d;
	Ref<World3D> world_3d;
	Ref<World3D> own_world_3d;

	World *_parent_world;

	bool _override_in_parent_viewport;
	World *_override_world;

	Vector<World *> _overriding_worlds;
	Vector<Camera *> _override_cameras;

	Camera *camera;
	Set<Camera *> cameras;

	Size2 size;
};

#endif
