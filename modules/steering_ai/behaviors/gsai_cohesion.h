#ifndef GSAI_COHESION_H
#define GSAI_COHESION_H

class GSAICohesion : public GSAIGroupBehavior {
	GDCLASS(GSAICohesion, GSAIGroupBehavior);

public:
	Vector3 get__center_of_mass();
	void set__center_of_mass(const Vector3 &val);

	void _calculate_steering(const GSAITargetAcceleration &acceleration);
	bool _report_neighbor(const GSAISteeringAgent &neighbor);

	GSAICohesion();
	~GSAICohesion();

protected:
	static void _bind_methods();

	// Calculates an acceleration that attempts to move the agent towards the center
	// of mass of the agents in the area defined by the `GSAIProximity`.
	// @category - Group behaviors
	Vector3 _center_of_mass = ;
};

#endif
