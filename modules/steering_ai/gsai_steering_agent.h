#ifndef GSAI_STEERING_AGENT_H
#define GSAI_STEERING_AGENT_H

#include "core/int_types.h"
#include "core/math/vector3.h"

#include "core/object/reference.h"

#include "gsai_agent_location.h"

class GSAISteeringAgent : public GSAIAgentLocation {
	GDCLASS(GSAISteeringAgent, GSAIAgentLocation);

public:
	float get_zero_linear_speed_threshold() const;
	void set_zero_linear_speed_threshold(const float val);

	float get_linear_speed_max() const;
	void set_linear_speed_max(const float val);

	float get_linear_acceleration_max() const;
	void set_linear_acceleration_max(const float val);

	float get_angular_speed_max() const;
	void set_angular_speed_max(const float val);

	float get_angular_acceleration_max() const;
	void set_angular_acceleration_max(const float val);

	Vector3 get_linear_velocity();
	void set_linear_velocity(const Vector3 &val);

	float get_angular_velocity() const;
	void set_angular_velocity(const float val);

	float get_bounding_radius() const;
	void set_bounding_radius(const float val);

	bool get_is_tagged() const;
	void set_is_tagged(const bool val);

	GSAISteeringAgent();
	~GSAISteeringAgent();

protected:
	static void _bind_methods();

	float zero_linear_speed_threshold;
	float linear_speed_max;
	float linear_acceleration_max;
	float angular_speed_max;
	float angular_acceleration_max;
	Vector3 linear_velocity;
	float angular_velocity;
	float bounding_radius;
	bool is_tagged;
};

#endif
