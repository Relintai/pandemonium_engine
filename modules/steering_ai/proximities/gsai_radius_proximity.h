#ifndef GSAIRADIUSPROXIMITY_H
#define GSAIRADIUSPROXIMITY_H

class GSAIRadiusProximity : public GSAIProximity {
	GDCLASS(GSAIRadiusProximity, GSAIProximity);

public:
	float get_radius() const;
	void set_radius(const float val);

	int get__last_frame() const;
	void set__last_frame(const int val);

	SceneTree get_ *_scene_tree();
	void set_ *_scene_tree(const SceneTree &val);

	void _init();
	int _find_neighbors(const FuncRef &callback);

	GSAIRadiusProximity();
	~GSAIRadiusProximity();

protected:
	static void _bind_methods();

	// Determines any agent that is in the specified list as being neighbors with the owner agent if
	// they lie within the specified radius.
	// @category - Proximities
	// The radius around the owning agent to find neighbors in
	float radius = 0.0;
	int _last_frame = 0;
	SceneTree *_scene_tree;
	// Returns a number of neighbors based on a `callback` function.
	//
	// `_find_neighbors` calls `callback` for each agent in the `agents` array that lie within
	// the radius around the owning agent and adds one to the count if its `callback` returns true.
	// @tags - virtual
};

#endif
