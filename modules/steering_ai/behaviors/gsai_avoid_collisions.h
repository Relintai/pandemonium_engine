#ifndef GSAIAVOIDCOLLISIONS_H
#define GSAIAVOIDCOLLISIONS_H

class GSAIAvoidCollisions : public GSAIGroupBehavior {
	GDCLASS(GSAIAvoidCollisions, GSAIGroupBehavior);

public:
	GSAISteeringAgent get_ *_first_neighbor();
	void set_ *_first_neighbor(const GSAISteeringAgent &val);

	float get__shortest_time() const;
	void set__shortest_time(const float val);

	float get__first_minimum_separation() const;
	void set__first_minimum_separation(const float val);

	float get__first_distance() const;
	void set__first_distance(const float val);

	Vector3 get__first_relative_position();
	void set__first_relative_position(const Vector3 &val);

	Vector3 get__first_relative_velocity();
	void set__first_relative_velocity(const Vector3 &val);

	void _calculate_steering(const GSAITargetAcceleration &acceleration);
	bool _report_neighbor(const GSAISteeringAgent &neighbor);

	GSAIAvoidCollisions();
	~GSAIAvoidCollisions();

protected:
	static void _bind_methods();

	// Steers the agent to avoid obstacles in its path. Approximates obstacles as
	// spheres.
	// @category - Group behaviors
	GSAISteeringAgent *_first_neighbor;
	float _shortest_time = 0.0;
	float _first_minimum_separation = 0.0;
	float _first_distance = 0.0;
	Vector3 _first_relative_position = ;
	Vector3 _first_relative_velocity = ;
	// Callback for the proximity to call when finding neighbors. Keeps track of every `neighbor`
	// that was found but only keeps the one the owning agent will most likely collide with.
	// @tags - virtual
};

#endif
