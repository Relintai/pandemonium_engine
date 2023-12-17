#ifndef PROP_2D_ESS_ENTITY_H
#define PROP_2D_ESS_ENTITY_H


#include "scene/main/spatial.h"

#include "core/math/vector3.h"

class Prop2DESSEntity : public Spatial {
	GDCLASS(Prop2DESSEntity, Spatial);
	OBJ_CATEGORY("Prop2Ds");

public:
	Prop2DESSEntity();
	~Prop2DESSEntity();

protected:
	static void _bind_methods();

private:
};

#endif
