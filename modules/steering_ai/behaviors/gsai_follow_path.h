#ifndef GSAI_FOLLOW_PATH_H
#define GSAI_FOLLOW_PATH_H

#include "core/object/reference.h"

#include "gsai_arrive.h"

class GSAIPath;

class GSAIFollowPath : public GSAIArrive {
	GDCLASS(GSAIFollowPath, GSAIArrive);

public:
	Ref<GSAIPath> get_path();
	void set_path(const Ref<GSAIPath> &val);

	float get_path_offset() const;
	void set_path_offset(const float val);

	bool get_is_arrive_enabled() const;
	void set_is_arrive_enabled(const bool val);

	float get_prediction_time() const;
	void set_prediction_time(const float val);

	void _calculate_steering(Ref<GSAITargetAcceleration> acceleration);

	GSAIFollowPath();
	~GSAIFollowPath();

protected:
	static void _bind_methods();

	// Produces a linear acceleration that moves the agent along the specified path.
	// @category - Individual behaviors
	// The path to follow and travel along.
	Ref<GSAIPath> path;
	// The distance along the path to generate the next target position.
	float path_offset;
	// Whether to use `GSAIArrive` behavior on an open path.
	bool is_arrive_enabled;
	// The amount of time in the future to predict the owning agent's position along
	// the path. Setting it to 0.0 will force non-predictive path following.
	float prediction_time;
};

#endif
