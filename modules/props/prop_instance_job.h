#ifndef PROP_INSTANCE_JOB
#define PROP_INSTANCE_JOB


#include "scene/resources/texture.h"

#include "core/os/thread_pool_job.h"

class PropData;
class PropInstance;

class PropInstanceJob : public ThreadPoolJob {
	GDCLASS(PropInstanceJob, ThreadPoolJob);
public:
	static const String BINDING_STRING_ACTIVE_BUILD_PHASE_TYPE;

	enum ActiveBuildPhaseType {
		BUILD_PHASE_TYPE_NORMAL = 0,
		BUILD_PHASE_TYPE_PROCESS,
		BUILD_PHASE_TYPE_PHYSICS_PROCESS,
	};

public:
	ActiveBuildPhaseType get_build_phase_type();
	void set_build_phase_type(PropInstanceJob::ActiveBuildPhaseType build_phase_type);

	void set_prop(const Ref<PropData> &prop);
	void set_prop_instance(PropInstance *instance);
	void set_prop_instance_bind(Node *instance);

	int get_phase();
	void set_phase(const int phase);
	void next_phase();

	bool get_build_done();
	void set_build_done(const bool val);

	void finished();

	void reset();
	virtual void _reset();

	void _execute();

	void execute_phase();
	virtual void _execute_phase();

	void process(const float delta);
	void physics_process(const float delta);

	void prop_instance_enter_tree();
	void prop_instance_exit_tree();

	PropInstanceJob();
	~PropInstanceJob();

protected:
	static void _bind_methods();

	ActiveBuildPhaseType _build_phase_type;
	bool _build_done;
	int _phase;
	bool _in_tree;
	Ref<PropData> _prop;
	PropInstance *_instance;
};

VARIANT_ENUM_CAST(PropInstanceJob::ActiveBuildPhaseType);

#endif
