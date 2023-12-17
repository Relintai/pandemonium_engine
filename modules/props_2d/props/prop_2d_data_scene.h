#ifndef PROP_2D_DATA_SCENE_H
#define PROP_2D_DATA_SCENE_H


#include "core/math/vector3.h"
#include "prop_2d_data_entry.h"

#include "scene/resources/packed_scene.h"

class Prop2DDataScene : public Prop2DDataEntry {
	GDCLASS(Prop2DDataScene, Prop2DDataEntry);

public:
	Ref<PackedScene> get_scene();
	void set_scene(const Ref<PackedScene> &value);

	bool _processor_handles(Node *node);
	void _processor_process(Ref<Prop2DData> prop_data, Node *node, const Transform2D &transform, Ref<Prop2DDataEntry> entry = Ref<Prop2DDataEntry>());
	Node *_processor_get_node_for(const Transform2D &transform, Node *node = nullptr);

	Prop2DDataScene();
	~Prop2DDataScene();

protected:
	static void _bind_methods();

private:
	bool _snap_to_mesh;
	Vector3 _snap_axis;
	Ref<PackedScene> _scene;
};

#endif
