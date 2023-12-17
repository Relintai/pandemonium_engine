/*************************************************************************/
/*  prop_2d_scene_instance.cpp                                           */
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

#include "prop_2d_scene_instance.h"

#include "core/config/engine.h"

Ref<PackedScene> Prop2DSceneInstance::get_scene() {
	return _scene;
}
void Prop2DSceneInstance::set_scene(const Ref<PackedScene> &data) {
	if (_scene == data)
		return;

	_scene = data;

	build();
}

void Prop2DSceneInstance::build() {
	if (!is_inside_tree()) {
		return;
	}

	for (int i = 0; i < get_child_count(); ++i) {
		Node *n = get_child(i);

		//this way we won't delete the user's nodes
		if (n->get_owner() == NULL) {
			n->queue_delete();
		}
	}

	if (!_scene.is_valid())
		return;

	Node *n = _scene->instance();

	add_child(n);

	//if (Engine::get_singleton()->is_editor_hint())
	//	n->set_owner(get_tree()->get_edited_scene_root());
}

Prop2DSceneInstance::Prop2DSceneInstance() {
}
Prop2DSceneInstance::~Prop2DSceneInstance() {
	_scene.unref();
}

void Prop2DSceneInstance::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			build();
		}
	}
}

void Prop2DSceneInstance::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_scene"), &Prop2DSceneInstance::get_scene);
	ClassDB::bind_method(D_METHOD("set_scene", "value"), &Prop2DSceneInstance::set_scene);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "scene", PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"), "set_scene", "get_scene");

	ClassDB::bind_method(D_METHOD("build"), &Prop2DSceneInstance::build);
}
