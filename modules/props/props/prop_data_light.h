#ifndef PROP_DATA_LIGHT_H
#define PROP_DATA_LIGHT_H
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

#include "core/color.h"

#include "prop_data_entry.h"

class PropDataLight : public PropDataEntry {
	GDCLASS(PropDataLight, PropDataEntry);

public:
	Color get_light_color() const;
	void set_light_color(const Color value);

	int get_light_size() const;
	void set_light_size(const int value);

	bool _processor_handles(Node *node);
	void _processor_process(Ref<PropData> prop_data, Node *node, const Transform &transform);
	Node *_processor_get_node_for(const Transform &transform);

	PropDataLight();
	~PropDataLight();

protected:
	static void _bind_methods();

private:
	Color _light_color;
	int _light_size;
};

#endif
