#ifndef GSAIEVADE_H
#define GSAIEVADE_H

class GSAIEvade : public GSAIPursue {
	GDCLASS(GSAIEvade, GSAIPursue);

public:
	float _get_modified_acceleration();

	GSAIEvade();
	~GSAIEvade();

protected:
	static void _bind_methods();

	// Calculates acceleration to take an agent away from where a target agent is
	// moving.
	// @category - Individual behaviors
};

#endif
