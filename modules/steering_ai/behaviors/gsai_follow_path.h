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

	Ref<GSAIPath> path;
	float path_offset;
	bool is_arrive_enabled;
	float prediction_time;
};

#endif
