#ifndef GSAI_SEPARATION_H
#define GSAI_SEPARATION_H

#include "core/object/reference.h"

#include "../gsai_group_behavior.h"

class GSAITargetAcceleration;

class GSAISeparation : public GSAIGroupBehavior {
	GDCLASS(GSAISeparation, GSAIGroupBehavior);

public:
	float get_decay_coefficient() const;
	void set_decay_coefficient(const float val);

	Ref<GSAITargetAcceleration> get_acceleration();
	void set_acceleration(const Ref<GSAITargetAcceleration> &val);

	void _calculate_steering(Ref<GSAITargetAcceleration> p_acceleration);
	bool _report_neighbor(Ref<GSAISteeringAgent> p_neighbor);

	GSAISeparation();
	~GSAISeparation();

protected:
	static void _bind_methods();

	float decay_coefficient;
	Ref<GSAITargetAcceleration> acceleration;
};

#endif
