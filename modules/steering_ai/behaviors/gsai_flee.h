#ifndef GSAIFLEE_H
#define GSAIFLEE_H

class GSAIFlee : public GSAISeek {
	GDCLASS(GSAIFlee, GSAISeek);

public:
	void _calculate_steering(const GSAITargetAcceleration &acceleration);

	GSAIFlee();
	~GSAIFlee();

protected:
	static void _bind_methods();

	// Calculates acceleration to take an agent directly away from a target agent.
	// @category - Individual behaviors
};

#endif
