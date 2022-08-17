#ifndef PROP_2D_UTILS_H
#define PROP_2D_UTILS_H
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

#include "core/object.h"
#include "core/reference.h"
#include "core/containers/vector.h"
#include "scene/3d/spatial.h"

#include "scene/main/node.h"

class Prop2DData;
class Prop2DDataEntry;

class Room;

class Prop2DUtils : public Object {
	GDCLASS(Prop2DUtils, Object);

public:
	static Prop2DUtils *get_singleton();

	Ref<Prop2DData> convert_tree(Node *root);
	void _convert_tree(Ref<Prop2DData> prop_data, Node *node, const Transform2D &transform);

	static int add_processor(const Ref<Prop2DDataEntry> &processor);
	static Ref<Prop2DDataEntry> get_processor(const int index);
	static void swap_processors(const int index1, const int index2);
	static void remove_processor(const int index);
	static int get_processor_count();

	Prop2DUtils();
	~Prop2DUtils();

protected:
	static void _bind_methods();

private:
	int _add_processor_bind(const Ref<Prop2DDataEntry> &processor);
	Ref<Prop2DDataEntry> _get_processor_bind(const int index);
	void _swap_processors_bind(const int index1, const int index2);
	void _remove_processor_bind(const int index);
	int _get_processor_count_bind();

	static Vector<Ref<Prop2DDataEntry>> _processors;
	static Prop2DUtils *_instance;
};

#endif
