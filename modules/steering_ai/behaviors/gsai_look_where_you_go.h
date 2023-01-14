#ifndef GSAI_LOOK_WHERE_YOU_GO_H
#define GSAI_LOOK_WHERE_YOU_GO_H

#include "core/object/reference.h"

#include "gsai_match_orientation.h"

class GSAITargetAcceleration;

class GSAILookWhereYouGo : public GSAIMatchOrientation {
	GDCLASS(GSAILookWhereYouGo, GSAIMatchOrientation);

public:
	void _calculate_steering(Ref<GSAITargetAcceleration> accel);

	GSAILookWhereYouGo();
	~GSAILookWhereYouGo();

protected:
	static void _bind_methods();
};

#endif
