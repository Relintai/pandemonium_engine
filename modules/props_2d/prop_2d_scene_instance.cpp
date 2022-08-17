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
