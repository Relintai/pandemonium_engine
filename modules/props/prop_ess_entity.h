#ifndef PROP_ESS_ENTITY_H
#define PROP_ESS_ENTITY_H


#include "scene/main/spatial.h"

#include "core/math/vector3.h"

class PropESSEntity : public Spatial {
	GDCLASS(PropESSEntity, Spatial);
	OBJ_CATEGORY("Props");

public:
	PropESSEntity();
	~PropESSEntity();

protected:
	static void _bind_methods();

private:
};

#endif
