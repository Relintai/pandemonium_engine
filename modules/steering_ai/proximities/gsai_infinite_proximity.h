#ifndef GSAI_INFINITE_PROXIMITY_H
#define GSAI_INFINITE_PROXIMITY_H

#include "core/object/func_ref.h"
#include "core/object/reference.h"

#include "gsai_proximity.h"

class GSAIInfiniteProximity : public GSAIProximity {
	GDCLASS(GSAIInfiniteProximity, GSAIProximity);

public:
	int _find_neighbors(Ref<FuncRef> callback);

	GSAIInfiniteProximity();
	~GSAIInfiniteProximity();

protected:
	static void _bind_methods();
};

#endif
