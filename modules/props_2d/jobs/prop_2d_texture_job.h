#ifndef PROP_2D_TEXTURE_JOB
#define PROP_2D_TEXTURE_JOB
/*
Copyright (c) 2019-2022 Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

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
