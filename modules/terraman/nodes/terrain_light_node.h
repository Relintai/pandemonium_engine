#ifndef TERRAIN_LIGHT_NODE_H
#define TERRAIN_LIGHT_NODE_H


#include "scene/main/spatial.h"

#include "core/math/vector3.h"

class TerrainLightNode : public Spatial {
	GDCLASS(TerrainLightNode, Spatial);
	OBJ_CATEGORY("Props");

public:
	//make it turn into a normal light if voxelman isn't present?

	TerrainLightNode();
	~TerrainLightNode();

protected:
	static void _bind_methods();

private:
};

#endif
