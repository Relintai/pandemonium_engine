#ifndef GSAI_LOOK_WHERE_YOU_GO_H
#define GSAI_LOOK_WHERE_YOU_GO_H

class GSAILookWhereYouGo : public GSAIMatchOrientation {
	GDCLASS(GSAILookWhereYouGo, GSAIMatchOrientation);

public:
	void _calculate_steering(const GSAITargetAcceleration &accel);

	GSAILookWhereYouGo();
	~GSAILookWhereYouGo();

protected:
	static void _bind_methods();

	// Calculates an angular acceleration to match an agent's orientation to its
	// direction of travel.
	// @category - Individual behaviors
};

#endif
