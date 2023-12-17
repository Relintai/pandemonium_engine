#ifndef PROP_SCENE_INSTANCE_H
#define PROP_SCENE_INSTANCE_H


#include "scene/main/spatial.h"

#include "core/math/vector3.h"

#include "props/prop_data.h"
#include "scene/resources/packed_scene.h"

class PropSceneInstance : public Spatial {
	GDCLASS(PropSceneInstance, Spatial);

public:
	Ref<PackedScene> get_scene();
	void set_scene(const Ref<PackedScene> &data);

	bool get_snap_to_mesh() const;
	void set_snap_to_mesh(const bool value);

	Vector3 get_snap_axis() const;
	void set_snap_axis(const Vector3 &value);

	void build();

	PropSceneInstance();
	~PropSceneInstance();

protected:
	void _notification(int p_what);
	static void _bind_methods();

private:
	Ref<PackedScene> _scene;
	bool _snap_to_mesh;
	Vector3 _snap_axis;
};

#endif
