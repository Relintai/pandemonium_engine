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

	//If a World has an override these are overridden
	Ref<World2D> find_world_2d() const;
	Ref<World3D> find_world_3d() const;
	Ref<World2D> find_world_2d_no_override() const;
	Ref<World3D> find_world_3d_no_override() const;

	World *get_override_world();
	World *get_override_world_or_this();
	void set_override_world(World *p_world);
	void set_override_world_bind(Node *p_world);

	// If a world has an override the override defers these to the overridden
	// As these are called by the child Nodes of the overriding world

	// Viewport root
	// -- World w1
	// ---- PanelContainer pc

	// w1 is set as override for the root.
	// pc's CanvasItem will use the canvas in w1, and will likely call gui_reset_canvas_sort_index() in w1 at least some point.
	// What we want, is to defer this call to the root viewport, as that is the class that is able to deal with it.
	// This is what makes overrides transparent to nodes like the Camera, as they can just use the world that they are a child of
	// and overrides can be handled for them in the backgorund.

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
	void _add_override_cameras(World *p_from);

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
	World *_overriding_world;

	Camera *camera;
	Set<Camera *> cameras;
	Camera *_own_active_camera;
	Vector<Camera *> _override_cameras;

	//override canvas layers vec remove /add on override set, and unset

	Size2 size;
};

#endif
