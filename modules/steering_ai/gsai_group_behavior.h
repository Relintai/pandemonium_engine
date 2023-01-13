#ifndef GSAIGROUPBEHAVIOR_H
#define GSAIGROUPBEHAVIOR_H

class GSAIGroupBehavior : public GSAISteeringBehavior {
	GDCLASS(GSAIGroupBehavior, GSAISteeringBehavior);

public:
	GSAIProximity get_ *proximity();
	void set_ *proximity(const GSAIProximity &val);

	Ref<FuncRef> get__callback();
	void set__callback(const Ref<FuncRef> &val);

	FuncRef get_callback();
	bool _report_neighbor(const GSAISteeringAgent &_neighbor);

	GSAIGroupBehavior();
	~GSAIGroupBehavior();

protected:
	static void _bind_methods();

	// Base type for group-based steering behaviors.
	// @category - Base types
	// Container to find neighbors of the agent and calculate group behavior.
	GSAIProximity *proximity;
	Ref<FuncRef> _callback = funcref(self, "_report_neighbor");
	// Internal callback for the behavior to define whether or not a member is
	// relevant
	// @tags - virtual
};

#endif
