#ifndef PROP_2D_TEXTURE_JOB
#define PROP_2D_TEXTURE_JOB


#include "scene/resources/texture.h"

#include "core/os/thread_pool_job.h"

#include "core/object/reference.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_TEXTURE_PACKER_ENABLED
class TexturePacker;
#endif

class Prop2DTextureJob : public ThreadPoolJob {
	GDCLASS(Prop2DTextureJob, ThreadPoolJob);

public:
#ifdef MODULE_TEXTURE_PACKER_ENABLED
	Ref<TexturePacker> get_merger();
	void set_merger(const Ref<TexturePacker> &merger);
#endif

	void _execute();

	Prop2DTextureJob();
	~Prop2DTextureJob();

protected:
	static void _bind_methods();

#ifdef MODULE_TEXTURE_PACKER_ENABLED
	Ref<TexturePacker> _merger;
#endif
};

#endif
