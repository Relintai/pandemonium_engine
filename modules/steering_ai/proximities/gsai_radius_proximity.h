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

	// Determines any agent that is in the specified list as being neighbors with the owner agent if
	// they lie within the specified radius.
	// @category - Proximities
	// The radius around the owning agent to find neighbors in
	float radius;
	int _last_frame;
	// Returns a number of neighbors based on a `callback` function.
	//
	// `_find_neighbors` calls `callback` for each agent in the `agents` array that lie within
	// the radius around the owning agent and adds one to the count if its `callback` returns true.
	// @tags - virtual
};

#endif
