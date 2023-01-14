#ifndef GSAI_RADIUS_PROXIMITY_H
#define GSAI_RADIUS_PROXIMITY_H

#include "core/object/func_ref.h"
#include "core/object/reference.h"

#include "gsai_proximity.h"

class SceneTree;

class GSAIRadiusProximity : public GSAIProximity {
	GDCLASS(GSAIRadiusProximity, GSAIProximity);

public:
	float get_radius() const;
	void set_radius(const float val);

	int _find_neighbors(Ref<FuncRef> callback);

	GSAIRadiusProximity();
	~GSAIRadiusProximity();

protected:
	static void _bind_methods();

	float radius;
	int _last_frame;
};

#endif
