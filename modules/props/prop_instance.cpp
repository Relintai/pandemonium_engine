/*************************************************************************/
/*  prop_instance.cpp                                                    */
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

#include "prop_instance.h"

#include "../mesh_data_resource/nodes/mesh_data_instance.h"

#include "scene/3d/light.h"
#include "scene/3d/physics_body.h"
#include "scene/resources/material/shader_material.h"
#include "scene/resources/material/spatial_material.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
#include "../mesh_data_resource/props/prop_data_mesh_data.h"
#endif

#include "./props/prop_data_entry.h"
#include "./props/prop_data_light.h"
#include "./props/prop_data_prop.h"
#include "./props/prop_data_scene.h"
#include "./props/prop_data_static_body.h"
#include "./props/prop_data_tiled_wall.h"

#include "tiled_wall/tiled_wall.h"
#include "tiled_wall/tiled_wall_data.h"

Ref<PropData> PropInstance::get_prop_data() {
	return _prop_data;
}
void PropInstance::set_prop_data(const Ref<PropData> &data) {
	if (_prop_data == data) {
		return;
	}

	_prop_data = data;

	if (_building) {
		queue_build();
	} else {
		call_deferred("build");
	}
}

Ref<Material> PropInstance::get_material() {
	return _material;
}
void PropInstance::set_material(const Ref<Material> &material) {
	_material = material;
}

uint32_t PropInstance::get_collision_layer() const {
	return _collision_layer;
}

void PropInstance::set_collision_layer(uint32_t p_layer) {
	_collision_layer = p_layer;

	collision_layer_changed();
}

uint32_t PropInstance::get_collision_mask() const {
	return _collision_mask;
}

void PropInstance::set_collision_mask(uint32_t p_mask) {
	_collision_mask = p_mask;

	collision_mask_changed();
}

void PropInstance::collision_layer_changed() {
}

void PropInstance::collision_mask_changed() {
}

AABB PropInstance::get_aabb() const {
	return _aabb;
}
PoolVector<Face3> PropInstance::get_faces(uint32_t p_usage_flags) const {
	return PoolVector<Face3>();
}

void PropInstance::init_materials() {
	call("_init_materials");
}
void PropInstance::_init_materials() {
}

void PropInstance::build() {
	call("_build");
}

void PropInstance::queue_build() {
	_build_queued = true;
}

void PropInstance::build_finished() {
	call("_build_finished");
}

void PropInstance::_build() {
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

	_aabb = AABB();
	_aabb.position = get_global_translation();

	if (!_prop_data.is_valid()) {
		return;
	}

	prop_preprocess(Transform(), _prop_data);

	update_gizmos();
	_change_notify();
}

void PropInstance::_build_finished() {
	_building = false;

	if (_build_queued) {
		call_deferred("build");
	}
}

void PropInstance::prop_preprocess(Transform transform, const Ref<PropData> &prop) {
	call("_prop_preprocess", transform, prop);
}

void PropInstance::_prop_preprocess(Transform transform, const Ref<PropData> &prop) {
	//don't set owners, to help working with the editor

	ERR_FAIL_COND(!prop.is_valid());

	int count = prop->get_prop_count();
	for (int i = 0; i < count; ++i) {
		Ref<PropDataEntry> e = prop->get_prop(i);

		if (!e.is_valid()) {
			continue;
		}

		Transform t = transform * e->get_transform();

		Ref<PropDataProp> prop_entry_data = e;

		if (prop_entry_data.is_valid()) {
			Ref<PropData> p = prop_entry_data->get_prop();

			if (!p.is_valid()) {
				continue;
			}

			prop_preprocess(t, p);

			continue;
		}

		Ref<PropDataTiledWall> tiled_wall_data = e;

		if (tiled_wall_data.is_valid()) {
			TiledWall *twn = memnew(TiledWall);

			twn->set_width(tiled_wall_data->get_width());
			twn->set_heigth(tiled_wall_data->get_heigth());
			twn->set_data(tiled_wall_data->get_data());
			twn->set_collision(tiled_wall_data->get_collision());

			twn->set_transform(t);

			add_child(twn);

			//_aabb.merge_with(twn->get_aabb());
			_aabb.expand_to(t.get_origin());

			continue;
		}

		Ref<PropDataScene> scene_data = e;

		if (scene_data.is_valid()) {
			Ref<PackedScene> sc = scene_data->get_scene();

			if (!sc.is_valid()) {
				continue;
			}

			Node *n = sc->instance();
			add_child(n);

			Spatial *sp = Object::cast_to<Spatial>(n);

			if (sp) {
				sp->set_transform(t);

				_aabb.expand_to(t.get_origin());
			}

			continue;
		}

		Ref<PropDataLight> light_data = e;

		if (light_data.is_valid()) {
			OmniLight *light = memnew(OmniLight);
			add_child(light);

			light->set_param(Light::PARAM_RANGE, light_data->get_light_range());
			light->set_param(Light::PARAM_ATTENUATION, light_data->get_light_attenuation());
			light->set_color(light_data->get_light_color());
			light->set_param(Light::PARAM_ENERGY, light_data->get_light_energy());
			light->set_param(Light::PARAM_INDIRECT_ENERGY, light_data->get_light_indirect_energy());
			light->set_negative(light_data->get_light_negative());
			light->set_param(Light::PARAM_SPECULAR, light_data->get_light_specular());
			light->set_transform(t);

			_aabb.expand_to(t.get_origin());

			continue;
		}

		Ref<PropDataStaticBody> static_body_data = e;

		if (static_body_data.is_valid()) {
			Node *static_body = static_body_data->processor_get_node_for(t);
			add_child(static_body);

			_aabb.expand_to(t.get_origin());

			continue;
		}

#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
		Ref<PropDataMeshData> mesh_data = e;

		if (mesh_data.is_valid()) {
			Ref<MeshDataResource> mdr = mesh_data->get_mesh();

			if (!mdr.is_valid()) {
				continue;
			}

			_aabb.merge_with(mdr->get_aabb());

			MeshDataInstance *mdi = memnew(MeshDataInstance);
			add_child(mdi);
			mdi->set_transform(t);

			if (_material.is_valid()) {
				//duplicate the material, so that textures will work
				Ref<Material> mat = _material->duplicate();

				Ref<Texture> texture = mdi->get_texture();

				if (texture.is_valid()) {
					//texture is valid, try to set it into the material
					Ref<SpatialMaterial> spmat = mat;

					if (spmat.is_valid()) {
						spmat->set_texture(SpatialMaterial::TEXTURE_ALBEDO, texture);
					} else {
						Ref<ShaderMaterial> shmat = mat;

						if (shmat.is_valid()) {
							shmat->set_shader_param("texture_albedo", texture);
						}
					}
				}

				mdi->set_material(mat);
			}

			mdi->set_mesh_data(mdr);

			//_aabb.merge_with(mdi->get_aabb());
			_aabb.expand_to(t.get_origin());

			continue;
		}
#endif
	}
}

PropInstance::PropInstance() {
	_build_queued = false;
	_building = false;

	_collision_layer = 1;
	_collision_mask = 1;
}

PropInstance::~PropInstance() {
	_prop_data.unref();
}

void PropInstance::_notification(int p_what) {
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

void PropInstance::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_prop_data"), &PropInstance::get_prop_data);
	ClassDB::bind_method(D_METHOD("set_prop_data", "value"), &PropInstance::set_prop_data);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "prop_data", PROPERTY_HINT_RESOURCE_TYPE, "PropData"), "set_prop_data", "get_prop_data");

	ClassDB::bind_method(D_METHOD("get_material"), &PropInstance::get_material);
	ClassDB::bind_method(D_METHOD("set_material", "material"), &PropInstance::set_material);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "material", PROPERTY_HINT_RESOURCE_TYPE, "Material"), "set_material", "get_material");

	ClassDB::bind_method(D_METHOD("get_collision_layer"), &PropInstance::get_collision_layer);
	ClassDB::bind_method(D_METHOD("set_collision_layer", "layer"), &PropInstance::set_collision_layer);

	ClassDB::bind_method(D_METHOD("get_collision_mask"), &PropInstance::get_collision_mask);
	ClassDB::bind_method(D_METHOD("set_collision_mask", "layer"), &PropInstance::set_collision_mask);

	ADD_GROUP("Collision", "collision_");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "collision_layer", PROPERTY_HINT_LAYERS_3D_PHYSICS), "set_collision_layer", "get_collision_layer");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "collision_mask", PROPERTY_HINT_LAYERS_3D_PHYSICS), "set_collision_mask", "get_collision_mask");

	BIND_VMETHOD(MethodInfo("_prop_preprocess",
			PropertyInfo(Variant::TRANSFORM, "transform"),
			PropertyInfo(Variant::OBJECT, "prop_data", PROPERTY_HINT_RESOURCE_TYPE, "PropData")));

	ClassDB::bind_method(D_METHOD("prop_preprocess", "transform", "prop"), &PropInstance::prop_preprocess);
	ClassDB::bind_method(D_METHOD("_prop_preprocess", "transform", "prop"), &PropInstance::_prop_preprocess);

	//---
	BIND_VMETHOD(MethodInfo("_init_materials"));

	ClassDB::bind_method(D_METHOD("init_materials"), &PropInstance::init_materials);
	ClassDB::bind_method(D_METHOD("_init_materials"), &PropInstance::_init_materials);

	//---
	ClassDB::bind_method(D_METHOD("build"), &PropInstance::build);
	ClassDB::bind_method(D_METHOD("queue_build"), &PropInstance::queue_build);
	ClassDB::bind_method(D_METHOD("build_finished"), &PropInstance::build_finished);

	BIND_VMETHOD(MethodInfo("_build"));
	BIND_VMETHOD(MethodInfo("_build_finished"));

	ClassDB::bind_method(D_METHOD("_build"), &PropInstance::_build);
	ClassDB::bind_method(D_METHOD("_build_finished"), &PropInstance::_build_finished);
}
