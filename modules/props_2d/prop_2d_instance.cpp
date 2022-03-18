#include "prop_2d_instance.h"

#include "../mesh_data_resource/nodes/mesh_data_instance.h"

#include "scene/3d/light.h"

#if MESH_DATA_RESOURCE_PRESENT
//define PROPS_PRESENT, so things compile. That module's scsub will define this too while compiling,
//but not when included from here.
#define PROPS_2D_PRESENT 1
#include "../mesh_data_resource/props_2d/prop_2d_data_mesh_data.h"
#endif

#include "./props/prop_2d_data_entry.h"
#include "./props/prop_2d_data_light.h"
#include "./props/prop_2d_data_prop.h"
#include "./props/prop_2d_data_scene.h"
#include "./props/prop_2d_data_sprite.h"
#include "./props/prop_2d_data_tiled_wall_2d.h"

#include "tiled_wall/tiled_wall_2d.h"
#include "tiled_wall/tiled_wall_2d_data.h"

Ref<Prop2DData> Prop2DInstance::get_prop_data() {
	return _prop_data;
}
void Prop2DInstance::set_prop_data(const Ref<Prop2DData> &data) {
	if (_prop_data == data)
		return;

	_prop_data = data;

	if (_building) {
		queue_build();
	} else {
		call_deferred("build");
	}
}

uint32_t Prop2DInstance::get_collision_layer() const {
	return _collision_layer;
}

void Prop2DInstance::set_collision_layer(uint32_t p_layer) {
	_collision_layer = p_layer;

	collision_layer_changed();
}

uint32_t Prop2DInstance::get_collision_mask() const {
	return _collision_mask;
}

void Prop2DInstance::set_collision_mask(uint32_t p_mask) {
	_collision_mask = p_mask;

	collision_mask_changed();
}

void Prop2DInstance::collision_layer_changed() {
}

void Prop2DInstance::collision_mask_changed() {
}

void Prop2DInstance::init_materials() {
	call("_init_materials");
}
void Prop2DInstance::_init_materials() {
}

void Prop2DInstance::build() {
	call("_build");
}

void Prop2DInstance::queue_build() {
	_build_queued = true;
}

void Prop2DInstance::build_finished() {
	call("_build_finished");
}

void Prop2DInstance::_build() {
	_building = true;
	_build_queued = false;

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

	if (!_prop_data.is_valid())
		return;

	prop_preprocess(Transform2D(), _prop_data);
}

void Prop2DInstance::_build_finished() {
	_building = false;

	if (_build_queued) {
		call_deferred("build");
	}
}

void Prop2DInstance::prop_preprocess(Transform2D transform, const Ref<Prop2DData> &prop) {
	call("_prop_preprocess", transform, prop);
}

void Prop2DInstance::_prop_preprocess(Transform2D transform, const Ref<Prop2DData> &prop) {
	//don't set owners, to help working with the editor

	ERR_FAIL_COND(!prop.is_valid());

	int count = prop->get_prop_count();
	for (int i = 0; i < count; ++i) {
		Ref<Prop2DDataEntry> e = prop->get_prop(i);

		if (!e.is_valid())
			continue;

		Transform2D t = transform * e->get_transform_2d();

		Ref<Prop2DDataProp2D> prop_entry_data = e;

		if (prop_entry_data.is_valid()) {
			Ref<Prop2DData> p = prop_entry_data->get_prop();

			if (!p.is_valid())
				continue;

			prop_preprocess(t, p);

			continue;
		}

		Ref<Prop2DDataTiledWall2D> tiled_wall_data = e;

		if (tiled_wall_data.is_valid()) {
			Node *twn = tiled_wall_data->_processor_get_node_for(t);

			add_child(twn);

			continue;
		}

		Ref<Prop2DDataScene> scene_data = e;

		if (scene_data.is_valid()) {
			Ref<PackedScene> sc = scene_data->get_scene();

			if (!sc.is_valid())
				continue;

			Node *n = sc->instance();
			add_child(n);

			Node2D *n2d = Object::cast_to<Node2D>(n);

			if (n2d) {
				n2d->set_transform(t);
			}

			continue;
		}

		Ref<Prop2DDataLight> light_data = e;

		if (light_data.is_valid()) {
			Node *light = light_data->_processor_get_node_for(t);

			add_child(light);

			continue;
		}

		Ref<Prop2DDataSprite> sprite_data = e;

		if (sprite_data.is_valid()) {
			Node *sp = sprite_data->_processor_get_node_for(t);

			add_child(sp);

			continue;
		}

#if MESH_DATA_RESOURCE_PRESENT
//TODO
/*
		Ref<Prop2DDataMeshData> mesh_data = e;

		if (mesh_data.is_valid()) {
			Ref<MeshDataResource> mdr = mesh_data->get_mesh();

			if (!mdr.is_valid())
				continue;

			MeshDataInstance *mdi = memnew(MeshDataInstance);
			add_child(mdi);
			mdi->set_transform(t);

			if (_material.is_valid()) {
				//duplicate the material, so that textures will work
				Ref<Material> mat = _material->duplicate();

				Ref<Texture> texture = mdi->get_texture();
				mdi->set_material(mat);
			}

			mdi->set_mesh_data(mdr);

			continue;
		}
		*/
#endif
	}
}

Prop2DInstance::Prop2DInstance() {
	_build_queued = false;
	_building = false;

	_collision_layer = 1;
	_collision_mask = 1;
}

Prop2DInstance::~Prop2DInstance() {
	_prop_data.unref();
}

void Prop2DInstance::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			if (_prop_data.is_valid()) {
				build();
			}
		}
		case NOTIFICATION_EXIT_TREE: {
		}
	}
}

void Prop2DInstance::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_prop_data"), &Prop2DInstance::get_prop_data);
	ClassDB::bind_method(D_METHOD("set_prop_data", "value"), &Prop2DInstance::set_prop_data);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "prop_data", PROPERTY_HINT_RESOURCE_TYPE, "Prop2DData"), "set_prop_data", "get_prop_data");

	ClassDB::bind_method(D_METHOD("get_collision_layer"), &Prop2DInstance::get_collision_layer);
	ClassDB::bind_method(D_METHOD("set_collision_layer", "layer"), &Prop2DInstance::set_collision_layer);

	ClassDB::bind_method(D_METHOD("get_collision_mask"), &Prop2DInstance::get_collision_mask);
	ClassDB::bind_method(D_METHOD("set_collision_mask", "layer"), &Prop2DInstance::set_collision_mask);

	ADD_GROUP("Collision", "collision_");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "collision_layer", PROPERTY_HINT_LAYERS_3D_PHYSICS), "set_collision_layer", "get_collision_layer");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "collision_mask", PROPERTY_HINT_LAYERS_3D_PHYSICS), "set_collision_mask", "get_collision_mask");

	BIND_VMETHOD(MethodInfo("_prop_preprocess",
			PropertyInfo(Variant::TRANSFORM, "tarnsform"),
			PropertyInfo(Variant::OBJECT, "prop_data", PROPERTY_HINT_RESOURCE_TYPE, "Prop2DData")));

	ClassDB::bind_method(D_METHOD("prop_preprocess", "tarnsform", "prop"), &Prop2DInstance::prop_preprocess);
	ClassDB::bind_method(D_METHOD("_prop_preprocess", "tarnsform", "prop"), &Prop2DInstance::_prop_preprocess);

	//---
	BIND_VMETHOD(MethodInfo("_init_materials"));

	ClassDB::bind_method(D_METHOD("init_materials"), &Prop2DInstance::init_materials);
	ClassDB::bind_method(D_METHOD("_init_materials"), &Prop2DInstance::_init_materials);

	//---
	ClassDB::bind_method(D_METHOD("build"), &Prop2DInstance::build);
	ClassDB::bind_method(D_METHOD("queue_build"), &Prop2DInstance::queue_build);
	ClassDB::bind_method(D_METHOD("build_finished"), &Prop2DInstance::build_finished);

	BIND_VMETHOD(MethodInfo("_build"));
	BIND_VMETHOD(MethodInfo("_build_finished"));

	ClassDB::bind_method(D_METHOD("_build"), &Prop2DInstance::_build);
	ClassDB::bind_method(D_METHOD("_build_finished"), &Prop2DInstance::_build_finished);
}
