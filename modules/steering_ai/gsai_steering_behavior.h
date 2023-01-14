#ifndef GSAI_STEERING_BEHAVIOR_H
#define GSAI_STEERING_BEHAVIOR_H

#include "core/int_types.h"
#include "core/math/vector3.h"

#include "core/object/reference.h"

class GSAISteeringAgent;
class GSAITargetAcceleration;

class GSAISteeringBehavior : public Reference {
	GDCLASS(GSAISteeringBehavior, Reference);

public:
	bool get_is_enabled() const;
	void set_is_enabled(const bool val);

	Ref<GSAISteeringAgent> get_agent();
	void set_agent(const Ref<GSAISteeringAgent> &val);

	void calculate_steering(Ref<GSAITargetAcceleration> acceleration);
	virtual void _calculate_steering(Ref<GSAITargetAcceleration> acceleration);

	GSAISteeringBehavior();
	~GSAISteeringBehavior();

protected:
	static void _bind_methods();

	bool is_enabled;

	Ref<GSAISteeringAgent> agent;
};

#endif
