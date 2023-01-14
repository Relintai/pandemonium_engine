#ifndef GSAI_PRIORITY_H
#define GSAI_PRIORITY_H

#include "core/object/reference.h"

#include "../gsai_steering_behavior.h"

class GSAISteeringBehavior;
class GSAITargetAcceleration;

class GSAIPriority : public GSAISteeringBehavior {
	GDCLASS(GSAIPriority, GSAISteeringBehavior);

public:
	float get_zero_threshold() const;
	void set_zero_threshold(const float val);

	void add_behavior(const Ref<GSAISteeringBehavior> &behavior);
	Ref<GSAISteeringBehavior> get_behavior(const int index);
	void remove_behavior(const int index);
	int get_behaviour_count();

	void _calculate_steering(Ref<GSAITargetAcceleration> accel);

	GSAIPriority();
	~GSAIPriority();

protected:
	static void _bind_methods();

	float zero_threshold;
	int _last_selected_index;
	Vector<Ref<GSAISteeringBehavior>> _behaviors;
};

#endif
