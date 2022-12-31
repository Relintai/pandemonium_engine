#ifndef PROP_DATA_DATA_H
#define PROP_DATA_DATA_H
/*
Copyright (c) 2019-2023 Péter Magyar

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

#include "core/math/transform.h"
#include "core/object/resource.h"

#include "modules/modules_enabled.gen.h"

class PropData;
class PropMesher;
class TexturePacker;

class PropDataEntry : public Resource {
	GDCLASS(PropDataEntry, Resource);

public:
	Transform get_transform() const;
	void set_transform(const Transform &value);

#ifdef MODULE_TEXTURE_PACKER_ENABLED
	void add_textures_into(Ref<TexturePacker> texture_packer);
#endif

	bool processor_handles(Node *node);
	void processor_process(Ref<PropData> prop_data, Node *node, const Transform &transform);
	Node *processor_get_node_for(const Transform &transform);
	bool processor_evaluate_children();

	virtual bool _processor_handles(Node *node);
	virtual void _processor_process(Ref<PropData> prop_data, Node *node, const Transform &transform);
	virtual Node *_processor_get_node_for(const Transform &transform);
	virtual bool _processor_evaluate_children();

	PropDataEntry();
	~PropDataEntry();

protected:
	static void _bind_methods();

private:
	Transform _transform;
};

#endif
