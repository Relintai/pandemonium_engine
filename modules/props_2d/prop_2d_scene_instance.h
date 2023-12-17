#ifndef PROP_2D_SCENE_INSTANCE_H
#define PROP_2D_SCENE_INSTANCE_H


#include "scene/main/node_2d.h"

#include "props/prop_2d_data.h"
#include "scene/resources/packed_scene.h"

class Prop2DSceneInstance : public Node2D {
	GDCLASS(Prop2DSceneInstance, Node2D);

public:
	Ref<PackedScene> get_scene();
	void set_scene(const Ref<PackedScene> &data);

	void build();

	Prop2DSceneInstance();
	~Prop2DSceneInstance();

protected:
	void _notification(int p_what);
	static void _bind_methods();

private:
	Ref<PackedScene> _scene;
};

#endif
