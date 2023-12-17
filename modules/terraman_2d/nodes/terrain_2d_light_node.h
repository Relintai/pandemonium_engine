#ifndef TERRAIN_2D_LIGHT_NODE_H
#define TERRAIN_2D_LIGHT_NODE_H


#include "scene/main/spatial.h"

#include "core/math/vector3.h"

class Terrain2DLightNode : public Spatial {
	GDCLASS(Terrain2DLightNode, Spatial);
	OBJ_CATEGORY("Prop2Ds");

public:
	//make it turn into a normal light if voxelman isn't present?

	Terrain2DLightNode();
	~Terrain2DLightNode();

protected:
	static void _bind_methods();

private:
};

#endif
