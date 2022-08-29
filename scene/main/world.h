#ifndef WORLD_H
#define WORLD_H

#include "core/containers/vector.h"
#include "core/object/reference.h"

#include "node.h"

class World3D;
class World2D;

class World : public Node {
	GDCLASS(World, Node);

public:
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

	World *get_override_world();
	World *get_override_world_or_this();
	void set_override_world(World *p_world);
	void set_override_world_bind(Node *p_world);

	World();
	~World();

protected:
	void _add_overridden_world(World *p_world);
	void _remove_overridden_world(World *p_world);

	virtual void _own_world_3d_changed();
	virtual void _on_set_use_own_world_3d(bool p_use_own_world_3d);
	virtual void _on_set_world_3d(const Ref<World3D> &p_old_world);
	virtual void _on_set_world_2d(const Ref<World2D> &p_old_world_2d);
	virtual void _on_world_override_changed(World *p_old_world);

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
};

#endif
