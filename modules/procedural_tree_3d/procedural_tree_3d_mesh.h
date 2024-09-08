#ifndef PROCEDURAL_TREE_3D
#define PROCEDURAL_TREE_3D

#include "scene/main/spatial.h"

class ProceduralTree3D : public Spatial {
	GDCLASS(ProceduralTree3D, Spatial);

public:

	ProceduralTree3D();
	~ProceduralTree3D();

protected:
	void _notification(int p_what);
	static void _bind_methods();
};

#endif
