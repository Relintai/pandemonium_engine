#ifndef GSAI_BLEND_H
#define GSAI_BLEND_H

#include "core/object/reference.h"

#include "../gsai_steering_behavior.h"

class GSAISteeringBehavior;
class GSAITargetAcceleration;

class GSAIBlend : public GSAISteeringBehavior {
	GDCLASS(GSAIBlend, GSAISteeringBehavior);

public:
	void add_behavior(const Ref<GSAISteeringBehavior> &behavior, const float weight);
	Ref<GSAISteeringBehavior> get_behavior(const int index);
	float get_behavior_weight(const int index);
	void remove_behavior(const int index);
	int get_behaviour_count();

	Ref<GSAITargetAcceleration> get_accel();

	void _calculate_steering(Ref<GSAITargetAcceleration> blended_accel);

	GSAIBlend();
	~GSAIBlend();

protected:
	struct GSAIBlendBehaviorEntry {
		Ref<GSAISteeringBehavior> behavior;
		float weight;
	};

protected:
	static void _bind_methods();

	Vector<GSAIBlendBehaviorEntry> _behaviors;
	Ref<GSAITargetAcceleration> _accel;
};

#endif
