#ifndef GSAI_EVADE_H
#define GSAI_EVADE_H

#include "core/object/reference.h"

#include "gsai_pursue.h"

class GSAIEvade : public GSAIPursue {
	GDCLASS(GSAIEvade, GSAIPursue);

public:
	float _get_modified_acceleration();

	GSAIEvade();
	~GSAIEvade();

protected:
	static void _bind_methods();
};

#endif
