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

#include "prop_2d_utils.h"

#include "../props/prop_2d_data.h"
#include "../props/prop_2d_data_entry.h"

#include "core/math/quick_hull.h"
#include "scene/3d/portal.h"
#include "scene/3d/room.h"
#include "scene/3d/room_manager.h"

#include "scene/3d/mesh_instance.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
#include "../../mesh_data_resource/nodes/mesh_data_instance.h"
#endif

#include "core/config/engine.h"

#include "scene/2d/node_2d.h"

Prop2DUtils *Prop2DUtils::_instance;
Vector<Ref<Prop2DDataEntry>> Prop2DUtils::_processors;

Prop2DUtils *Prop2DUtils::get_singleton() {
	return _instance;
}

Ref<Prop2DData> Prop2DUtils::convert_tree(Node *root) {
	ERR_FAIL_COND_V(!ObjectDB::instance_validate(root), Ref<Prop2DData>());

	Ref<Prop2DData> data;
	data.instance();
	Transform2D t;

	_convert_tree(data, root, t);

	return data;
}

void Prop2DUtils::_convert_tree(Ref<Prop2DData> prop_data, Node *node, const Transform2D &transform) {
	ERR_FAIL_COND(!ObjectDB::instance_validate(node));

	for (int i = 0; i < Prop2DUtils::_processors.size(); ++i) {
		Ref<Prop2DDataEntry> proc = Prop2DUtils::_processors.get(i);

		ERR_CONTINUE(!proc.is_valid());

		if (proc->processor_handles(node)) {
			proc->processor_process(prop_data, node, transform);

			if (!proc->processor_evaluate_children()) {
				return;
			}

			break;
		}
	}

	Node2D *n2d = Object::cast_to<Node2D>(node);

	if (!n2d) {
		//reset transform
		Transform2D t;

		for (int i = 0; i < node->get_child_count(); ++i) {
			Node *child = node->get_child(i);

			if (Engine::get_singleton()->is_editor_hint()) {
				//Skip it if it's hidden from the tree
				if (child->get_owner() != NULL) {
					_convert_tree(prop_data, node->get_child(i), t);
				}
			} else {
				_convert_tree(prop_data, node->get_child(i), t);
			}
		}
	} else {
		for (int i = 0; i < node->get_child_count(); ++i) {
			Node *child = node->get_child(i);

			if (Engine::get_singleton()->is_editor_hint()) {
				//Skip it if it's hidden from the tree
				if (child->get_owner() != NULL) {
					_convert_tree(prop_data, node->get_child(i), transform * n2d->get_transform());
				}
			} else {
				_convert_tree(prop_data, node->get_child(i), transform * n2d->get_transform());
			}
		}
	}
}

int Prop2DUtils::add_processor(const Ref<Prop2DDataEntry> &processor) {
	ERR_FAIL_COND_V(!processor.is_valid(), 0);

	Prop2DUtils::_processors.push_back(processor);

	return Prop2DUtils::_processors.size() - 1;
}
Ref<Prop2DDataEntry> Prop2DUtils::get_processor(const int index) {
	ERR_FAIL_INDEX_V(index, Prop2DUtils::_processors.size(), Ref<Prop2DDataEntry>());

	return Prop2DUtils::_processors[index];
}
void Prop2DUtils::swap_processors(const int index1, const int index2) {
	ERR_FAIL_INDEX(index1, Prop2DUtils::_processors.size());
	ERR_FAIL_INDEX(index2, Prop2DUtils::_processors.size());

	Ref<Prop2DDataEntry> a = Prop2DUtils::_processors.get(index1);
	Prop2DUtils::_processors.set(index1, Prop2DUtils::_processors.get(index2));
	Prop2DUtils::_processors.set(index2, a);
}
void Prop2DUtils::remove_processor(const int index) {
	ERR_FAIL_INDEX(index, Prop2DUtils::_processors.size());

	Prop2DUtils::_processors.remove(index);
}
int Prop2DUtils::get_processor_count() {
	return Prop2DUtils::_processors.size();
}

Prop2DUtils::Prop2DUtils() {
	_instance = this;
}

Prop2DUtils::~Prop2DUtils() {
	_instance = NULL;

	Prop2DUtils::_processors.clear();
}

void Prop2DUtils::_bind_methods() {
	ClassDB::bind_method(D_METHOD("convert_tree", "root"), &Prop2DUtils::convert_tree);

	ClassDB::bind_method(D_METHOD("add_processor", "processor"), &Prop2DUtils::_add_processor_bind);
	ClassDB::bind_method(D_METHOD("get_processor", "index"), &Prop2DUtils::_get_processor_bind);
	ClassDB::bind_method(D_METHOD("swap_processors", "index1", "index2"), &Prop2DUtils::_swap_processors_bind);
	ClassDB::bind_method(D_METHOD("remove_processor", "index"), &Prop2DUtils::_remove_processor_bind);
	ClassDB::bind_method(D_METHOD("get_processor_count"), &Prop2DUtils::_get_processor_count_bind);
}

int Prop2DUtils::_add_processor_bind(const Ref<Prop2DDataEntry> &processor) {
	return Prop2DUtils::add_processor(processor);
}
Ref<Prop2DDataEntry> Prop2DUtils::_get_processor_bind(const int index) {
	return Prop2DUtils::get_processor(index);
}
void Prop2DUtils::_swap_processors_bind(const int index1, const int index2) {
	Prop2DUtils::swap_processors(index1, index2);
}
void Prop2DUtils::_remove_processor_bind(const int index) {
	Prop2DUtils::remove_processor(index);
}
int Prop2DUtils::_get_processor_count_bind() {
	return Prop2DUtils::get_processor_count();
}
