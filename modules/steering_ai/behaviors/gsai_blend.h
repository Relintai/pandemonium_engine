#ifndef GSAI_BLEND_H
#define GSAI_BLEND_H

class GSAIBlend : public GSAISteeringBehavior {
	GDCLASS(GSAIBlend, GSAISteeringBehavior);

public:
	Array get__behaviors();
	void set__behaviors(const Array &val);

	GSAITargetAcceleration get_ *_accel();
	void set_ *_accel(const GSAITargetAcceleration &val);

	void add_behavior(const GSAISteeringBehavior &behavior, const float weight);
	Dictionary get_behavior(const int index);
	void remove_behavior(const int index);
	int get_behaviour_count();
	GSAITargetAcceleration get_accel();
	void _calculate_steering(const GSAITargetAcceleration &blended_accel);

	GSAIBlend();
	~GSAIBlend();

protected:
	static void _bind_methods();

	// Blends multiple steering behaviors into one, and returns a weighted
	// acceleration from their calculations.
	//
	// Stores the behaviors internally as dictionaries of the form
	// {
	// 	behavior : GSAISteeringBehavior,
	// 	weight : float
	// }
	// @category - Combination behaviors
	Array _behaviors = Array();
	GSAITargetAcceleration *_accel = GSAITargetAcceleration.new();
	// Appends a behavior to the internal array along with its `weight`.
	// Returns the behavior at the specified `index`, or an empty `Dictionary` if
	// none was found.
};

#endif
