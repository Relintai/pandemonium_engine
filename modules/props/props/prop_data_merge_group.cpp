/*************************************************************************/
/*  prop_data_merge_group.cpp                                            */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "prop_data_merge_group.h"

#include "../prop_instance.h"
#include "../prop_merge_group.h"
#include "../singleton/prop_utils.h"
#include "prop_data.h"

#include "modules/modules_enabled.gen.h"

bool PropDataMergeGroup::_processor_handles(Node *node) {
	PropMergeGroup *g = Object::cast_to<PropMergeGroup>(node);

	return g;
}

void PropDataMergeGroup::_processor_process(Ref<PropData> prop_data, Node *node, const Transform &transform) {
	PropMergeGroup *g = Object::cast_to<PropMergeGroup>(node);

	if (g) {
		Ref<PropData> d;
		d.instance();

		for (int i = 0; i < g->get_child_count(); ++i) {
			PropUtils::get_singleton()->_convert_tree(d, g->get_child(i), Transform());
		}

		Ref<PropDataMergeGroup> l;
		l.instance();
		l->set_prop(d);
		l->set_transform(transform * g->get_transform());
		l->materials_set(g->materials_get());
		l->set_is_merger(g->get_is_merger());

		prop_data->add_prop(l);
	}
}

bool PropDataMergeGroup::_processor_evaluate_children() {
	return false;
}

PropDataMergeGroup::PropDataMergeGroup() {
}

PropDataMergeGroup::~PropDataMergeGroup() {
}

void PropDataMergeGroup::_bind_methods() {
}
