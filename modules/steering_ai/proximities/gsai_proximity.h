#ifndef GSAI_PROXIMITY_H
#define GSAI_PROXIMITY_H

#include "core/int_types.h"
#include "core/math/vector3.h"

#include "core/object/func_ref.h"
#include "core/object/reference.h"

class GSAISteeringAgent;

class GSAIProximity : public Reference {
	GDCLASS(GSAIProximity, Reference);

public:
	Ref<GSAISteeringAgent> get_agent();
	void set_agent(const Ref<GSAISteeringAgent> &val);

	Vector<Ref<GSAISteeringAgent>> get_agents();
	void set_agents(const Vector<Ref<GSAISteeringAgent>> &arr);

	Array get_agents_arr();
	void set_agents_arr(const Array &arr);

	int find_neighbors(const Ref<FuncRef> &callback);
	virtual int _find_neighbors(Ref<FuncRef> callback);

	GSAIProximity();
	~GSAIProximity();

protected:
	static void _bind_methods();

	Ref<GSAISteeringAgent> agent;
	Vector<Ref<GSAISteeringAgent>> agents;
};

#endif
