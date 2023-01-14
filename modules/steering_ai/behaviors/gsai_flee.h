#ifndef GSAI_FLEE_H
#define GSAI_FLEE_H

#include "core/object/reference.h"

#include "gsai_seek.h"

class GSAITargetAcceleration;

class GSAIFlee : public GSAISeek {
	GDCLASS(GSAIFlee, GSAISeek);

public:
	void _calculate_steering(Ref<GSAITargetAcceleration> acceleration);

	GSAIFlee();
	~GSAIFlee();

protected:
	static void _bind_methods();
};

#endif
