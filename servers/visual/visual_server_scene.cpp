/*************************************************************************/
/*  visual_server_scene.cpp                                              */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
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

#include "visual_server_scene.h"

#include "core/math/transform_interpolator.h"
#include "core/os/os.h"
#include "core/project_settings.h"
#include "visual_server_globals.h"
#include "visual_server_raster.h"

#include <new>

/* CAMERA API */

Transform VisualServerScene::Camera::get_transform() const {
	if (!is_currently_interpolated()) {
		return transform;
	}

	Transform final;
	TransformInterpolator::interpolate_transform_via_method(transform_prev, transform, final, Engine::get_singleton()->get_physics_interpolation_fraction(), interpolation_method);
	return final;
}

RID VisualServerScene::camera_create() {
	Camera *camera = memnew(Camera);
	return camera_owner.make_rid(camera);
}

void VisualServerScene::camera_set_scenario(RID p_camera, RID p_scenario) {
	Camera *camera = camera_owner.get(p_camera);
	ERR_FAIL_COND(!camera);

	Scenario *old_scenario = camera->scenario;

	if (p_scenario.is_valid()) {
		camera->scenario = scenario_owner.get(p_scenario);
		ERR_FAIL_COND(!camera->scenario);
	} else {
		camera->scenario = nullptr;
	}

	if (old_scenario && (old_scenario != camera->scenario)) {
		// remove any interpolation data associated with the camera in this scenario
		old_scenario->_interpolation_data.notify_free_camera(p_camera, *camera);
	}
}

void VisualServerScene::camera_set_perspective(RID p_camera, float p_fovy_degrees, float p_z_near, float p_z_far) {
	Camera *camera = camera_owner.get(p_camera);
	ERR_FAIL_COND(!camera);
	camera->type = Camera::PERSPECTIVE;
	camera->fov = p_fovy_degrees;
	camera->znear = p_z_near;
	camera->zfar = p_z_far;
}

void VisualServerScene::camera_set_orthogonal(RID p_camera, float p_size, float p_z_near, float p_z_far) {
	Camera *camera = camera_owner.get(p_camera);
	ERR_FAIL_COND(!camera);
	camera->type = Camera::ORTHOGONAL;
	camera->size = p_size;
	camera->znear = p_z_near;
	camera->zfar = p_z_far;
}

void VisualServerScene::camera_set_frustum(RID p_camera, float p_size, Vector2 p_offset, float p_z_near, float p_z_far) {
	Camera *camera = camera_owner.get(p_camera);
	ERR_FAIL_COND(!camera);
	camera->type = Camera::FRUSTUM;
	camera->size = p_size;
	camera->offset = p_offset;
	camera->znear = p_z_near;
	camera->zfar = p_z_far;
}

void VisualServerScene::camera_reset_physics_interpolation(RID p_camera) {
	Camera *camera = camera_owner.get(p_camera);
	ERR_FAIL_COND(!camera);

	if (camera->is_currently_interpolated()) {
		camera->scenario->_interpolation_data.camera_teleport_list.push_back(p_camera);
	}
}

void VisualServerScene::camera_set_interpolated(RID p_camera, bool p_interpolated) {
	Camera *camera = camera_owner.get(p_camera);
	ERR_FAIL_COND(!camera);
	camera->interpolated = p_interpolated;
}

void VisualServerScene::camera_set_transform(RID p_camera, const Transform &p_transform) {
	Camera *camera = camera_owner.get(p_camera);
	ERR_FAIL_COND(!camera);

	camera->transform = p_transform.orthonormalized();

	if (camera->is_currently_interpolated()) {
		if (!camera->on_interpolate_transform_list) {
			camera->scenario->_interpolation_data.camera_transform_update_list_curr->push_back(p_camera);
			camera->on_interpolate_transform_list = true;
		}

		// decide on the interpolation method .. slerp if possible
		camera->interpolation_method = TransformInterpolator::find_method(camera->transform_prev.basis, camera->transform.basis);
	}
}

void VisualServerScene::camera_set_cull_mask(RID p_camera, uint32_t p_layers) {
	Camera *camera = camera_owner.get(p_camera);
	ERR_FAIL_COND(!camera);

	camera->visible_layers = p_layers;
}

void VisualServerScene::camera_set_environment(RID p_camera, RID p_env) {
	Camera *camera = camera_owner.get(p_camera);
	ERR_FAIL_COND(!camera);
	camera->env = p_env;
}

void VisualServerScene::camera_set_use_vertical_aspect(RID p_camera, bool p_enable) {
	Camera *camera = camera_owner.get(p_camera);
	ERR_FAIL_COND(!camera);
	camera->vaspect = p_enable;
}

/* SPATIAL PARTITIONING */

VisualServerScene::SpatialPartitioningScene_BVH::SpatialPartitioningScene_BVH() {
	_bvh.params_set_thread_safe(GLOBAL_GET("rendering/threads/thread_safe_bvh"));
	_bvh.params_set_pairing_expansion(GLOBAL_GET("rendering/quality/spatial_partitioning/bvh_collision_margin"));

	_dummy_cull_object = memnew(Instance);
}

VisualServerScene::SpatialPartitioningScene_BVH::~SpatialPartitioningScene_BVH() {
	if (_dummy_cull_object) {
		memdelete(_dummy_cull_object);
		_dummy_cull_object = nullptr;
	}
}

VisualServerScene::SpatialPartitionID VisualServerScene::SpatialPartitioningScene_BVH::create(Instance *p_userdata, const AABB &p_aabb, int p_subindex, bool p_pairable, uint32_t p_pairable_type, uint32_t p_pairable_mask) {
#if defined(DEBUG_ENABLED) && defined(TOOLS_ENABLED)
	// we are relying on this instance to be valid in order to pass
	// the visible flag to the bvh.
	DEV_ASSERT(p_userdata);
#endif

	// cache the pairable mask and pairable type on the instance as it is needed for user callbacks from the BVH, and this is
	// too complex to calculate each callback...
	p_userdata->bvh_pairable_mask = p_pairable_mask;
	p_userdata->bvh_pairable_type = p_pairable_type;

	uint32_t tree_id = p_pairable ? 1 : 0;
	uint32_t tree_collision_mask = 3;

	return _bvh.create(p_userdata, p_userdata->visible, tree_id, tree_collision_mask, p_aabb, p_subindex) + 1;
}

void VisualServerScene::SpatialPartitioningScene_BVH::erase(SpatialPartitionID p_handle) {
	_bvh.erase(p_handle - 1);
}

void VisualServerScene::SpatialPartitioningScene_BVH::move(SpatialPartitionID p_handle, const AABB &p_aabb) {
	_bvh.move(p_handle - 1, p_aabb);
}

void VisualServerScene::SpatialPartitioningScene_BVH::activate(SpatialPartitionID p_handle, const AABB &p_aabb) {
	// be very careful here, we are deferring the collision check, expecting a set_pairable to be called
	// immediately after.
	// see the notes in the BVH function.
	_bvh.activate(p_handle - 1, p_aabb, true);
}

void VisualServerScene::SpatialPartitioningScene_BVH::deactivate(SpatialPartitionID p_handle) {
	_bvh.deactivate(p_handle - 1);
}

void VisualServerScene::SpatialPartitioningScene_BVH::force_collision_check(SpatialPartitionID p_handle) {
	_bvh.force_collision_check(p_handle - 1);
}

void VisualServerScene::SpatialPartitioningScene_BVH::update() {
	_bvh.update();
}

void VisualServerScene::SpatialPartitioningScene_BVH::update_collisions() {
	_bvh.update_collisions();
}

void VisualServerScene::SpatialPartitioningScene_BVH::set_pairable(Instance *p_instance, bool p_pairable, uint32_t p_pairable_type, uint32_t p_pairable_mask) {
	SpatialPartitionID handle = p_instance->spatial_partition_id;

	p_instance->bvh_pairable_mask = p_pairable_mask;
	p_instance->bvh_pairable_type = p_pairable_type;

	uint32_t tree_id = p_pairable ? 1 : 0;
	uint32_t tree_collision_mask = 3;

	_bvh.set_tree(handle - 1, tree_id, tree_collision_mask);
}

int VisualServerScene::SpatialPartitioningScene_BVH::cull_convex(const Vector<Plane> &p_convex, Instance **p_result_array, int p_result_max, uint32_t p_mask) {
	_dummy_cull_object->bvh_pairable_mask = p_mask;
	_dummy_cull_object->bvh_pairable_type = 0;
	return _bvh.cull_convex(p_convex, p_result_array, p_result_max, _dummy_cull_object);
}

int VisualServerScene::SpatialPartitioningScene_BVH::cull_aabb(const AABB &p_aabb, Instance **p_result_array, int p_result_max, int *p_subindex_array, uint32_t p_mask) {
	_dummy_cull_object->bvh_pairable_mask = p_mask;
	_dummy_cull_object->bvh_pairable_type = 0;
	return _bvh.cull_aabb(p_aabb, p_result_array, p_result_max, _dummy_cull_object, 0xFFFFFFFF, p_subindex_array);
}

int VisualServerScene::SpatialPartitioningScene_BVH::cull_segment(const Vector3 &p_from, const Vector3 &p_to, Instance **p_result_array, int p_result_max, int *p_subindex_array, uint32_t p_mask) {
	_dummy_cull_object->bvh_pairable_mask = p_mask;
	_dummy_cull_object->bvh_pairable_type = 0;
	return _bvh.cull_segment(p_from, p_to, p_result_array, p_result_max, _dummy_cull_object, 0xFFFFFFFF, p_subindex_array);
}

void VisualServerScene::SpatialPartitioningScene_BVH::set_pair_callback(PairCallback p_callback, void *p_userdata) {
	_bvh.set_pair_callback(p_callback, p_userdata);
}

void VisualServerScene::SpatialPartitioningScene_BVH::set_unpair_callback(UnpairCallback p_callback, void *p_userdata) {
	_bvh.set_unpair_callback(p_callback, p_userdata);
}

///////////////////////

VisualServerScene::SpatialPartitionID VisualServerScene::SpatialPartitioningScene_Octree::create(Instance *p_userdata, const AABB &p_aabb, int p_subindex, bool p_pairable, uint32_t p_pairable_type, uint32_t p_pairable_mask) {
	return _octree.create(p_userdata, p_aabb, p_subindex, p_pairable, p_pairable_type, p_pairable_mask);
}

void VisualServerScene::SpatialPartitioningScene_Octree::erase(SpatialPartitionID p_handle) {
	_octree.erase(p_handle);
}

void VisualServerScene::SpatialPartitioningScene_Octree::move(SpatialPartitionID p_handle, const AABB &p_aabb) {
	_octree.move(p_handle, p_aabb);
}

void VisualServerScene::SpatialPartitioningScene_Octree::set_pairable(Instance *p_instance, bool p_pairable, uint32_t p_pairable_type, uint32_t p_pairable_mask) {
	SpatialPartitionID handle = p_instance->spatial_partition_id;
	_octree.set_pairable(handle, p_pairable, p_pairable_type, p_pairable_mask);
}

int VisualServerScene::SpatialPartitioningScene_Octree::cull_convex(const Vector<Plane> &p_convex, Instance **p_result_array, int p_result_max, uint32_t p_mask) {
	return _octree.cull_convex(p_convex, p_result_array, p_result_max, p_mask);
}

int VisualServerScene::SpatialPartitioningScene_Octree::cull_aabb(const AABB &p_aabb, Instance **p_result_array, int p_result_max, int *p_subindex_array, uint32_t p_mask) {
	return _octree.cull_aabb(p_aabb, p_result_array, p_result_max, p_subindex_array, p_mask);
}

int VisualServerScene::SpatialPartitioningScene_Octree::cull_segment(const Vector3 &p_from, const Vector3 &p_to, Instance **p_result_array, int p_result_max, int *p_subindex_array, uint32_t p_mask) {
	return _octree.cull_segment(p_from, p_to, p_result_array, p_result_max, p_subindex_array, p_mask);
}

void VisualServerScene::SpatialPartitioningScene_Octree::set_pair_callback(PairCallback p_callback, void *p_userdata) {
	_octree.set_pair_callback(p_callback, p_userdata);
}

void VisualServerScene::SpatialPartitioningScene_Octree::set_unpair_callback(UnpairCallback p_callback, void *p_userdata) {
	_octree.set_unpair_callback(p_callback, p_userdata);
}

void VisualServerScene::SpatialPartitioningScene_Octree::set_balance(float p_balance) {
	_octree.set_balance(p_balance);
}

/* SCENARIO API */

VisualServerScene::Scenario::Scenario() {
	debug = VS::SCENARIO_DEBUG_DISABLED;
	_interpolation_data.interpolation_enabled = false;

	bool use_bvh_or_octree = GLOBAL_GET("rendering/quality/spatial_partitioning/use_bvh");

	if (use_bvh_or_octree) {
		sps = memnew(SpatialPartitioningScene_BVH);
	} else {
		sps = memnew(SpatialPartitioningScene_Octree);
	}
}

void *VisualServerScene::_instance_pair(void *p_self, SpatialPartitionID, Instance *p_A, int, SpatialPartitionID, Instance *p_B, int) {
	//VisualServerScene *self = (VisualServerScene*)p_self;
	Instance *A = p_A;
	Instance *B = p_B;

	//instance indices are designed so greater always contains lesser
	if (A->base_type > B->base_type) {
		SWAP(A, B); //lesser always first
	}

	if (B->base_type == VS::INSTANCE_LIGHT && ((1 << A->base_type) & VS::INSTANCE_GEOMETRY_MASK)) {
		InstanceLightData *light = static_cast<InstanceLightData *>(B->base_data);
		InstanceGeometryData *geom = static_cast<InstanceGeometryData *>(A->base_data);

		InstanceLightData::PairInfo pinfo;
		pinfo.geometry = A;
		pinfo.L = geom->lighting.push_back(B);

		List<InstanceLightData::PairInfo>::Element *E = light->geometries.push_back(pinfo);

		if (geom->can_cast_shadows) {
			light->shadow_dirty = true;
		}
		geom->lighting_dirty = true;

		return E; //this element should make freeing faster
	} else if (B->base_type == VS::INSTANCE_REFLECTION_PROBE && ((1 << A->base_type) & VS::INSTANCE_GEOMETRY_MASK)) {
		InstanceReflectionProbeData *reflection_probe = static_cast<InstanceReflectionProbeData *>(B->base_data);
		InstanceGeometryData *geom = static_cast<InstanceGeometryData *>(A->base_data);

		InstanceReflectionProbeData::PairInfo pinfo;
		pinfo.geometry = A;
		pinfo.L = geom->reflection_probes.push_back(B);

		List<InstanceReflectionProbeData::PairInfo>::Element *E = reflection_probe->geometries.push_back(pinfo);

		geom->reflection_dirty = true;

		return E; //this element should make freeing faster
	}

	return nullptr;
}

void VisualServerScene::_instance_unpair(void *p_self, SpatialPartitionID, Instance *p_A, int, SpatialPartitionID, Instance *p_B, int, void *udata) {
	//VisualServerScene *self = (VisualServerScene*)p_self;
	Instance *A = p_A;
	Instance *B = p_B;

	//instance indices are designed so greater always contains lesser
	if (A->base_type > B->base_type) {
		SWAP(A, B); //lesser always first
	}

	if (B->base_type == VS::INSTANCE_LIGHT && ((1 << A->base_type) & VS::INSTANCE_GEOMETRY_MASK)) {
		InstanceLightData *light = static_cast<InstanceLightData *>(B->base_data);
		InstanceGeometryData *geom = static_cast<InstanceGeometryData *>(A->base_data);

		List<InstanceLightData::PairInfo>::Element *E = reinterpret_cast<List<InstanceLightData::PairInfo>::Element *>(udata);

		geom->lighting.erase(E->get().L);
		light->geometries.erase(E);

		if (geom->can_cast_shadows) {
			light->shadow_dirty = true;
		}
		geom->lighting_dirty = true;

	} else if (B->base_type == VS::INSTANCE_REFLECTION_PROBE && ((1 << A->base_type) & VS::INSTANCE_GEOMETRY_MASK)) {
		InstanceReflectionProbeData *reflection_probe = static_cast<InstanceReflectionProbeData *>(B->base_data);
		InstanceGeometryData *geom = static_cast<InstanceGeometryData *>(A->base_data);

		List<InstanceReflectionProbeData::PairInfo>::Element *E = reinterpret_cast<List<InstanceReflectionProbeData::PairInfo>::Element *>(udata);

		geom->reflection_probes.erase(E->get().L);
		reflection_probe->geometries.erase(E);

		geom->reflection_dirty = true;
	}
}

RID VisualServerScene::scenario_create() {
	Scenario *scenario = memnew(Scenario);
	ERR_FAIL_COND_V(!scenario, RID());
	RID scenario_rid = scenario_owner.make_rid(scenario);
	scenario->self = scenario_rid;

	scenario->sps->set_balance(GLOBAL_GET("rendering/quality/spatial_partitioning/render_tree_balance"));
	scenario->sps->set_pair_callback(_instance_pair, this);
	scenario->sps->set_unpair_callback(_instance_unpair, this);

	scenario->reflection_probe_shadow_atlas = VSG::scene_render->shadow_atlas_create();
	VSG::scene_render->shadow_atlas_set_size(scenario->reflection_probe_shadow_atlas, 1024); //make enough shadows for close distance, don't bother with rest
	VSG::scene_render->shadow_atlas_set_quadrant_subdivision(scenario->reflection_probe_shadow_atlas, 0, 4);
	VSG::scene_render->shadow_atlas_set_quadrant_subdivision(scenario->reflection_probe_shadow_atlas, 1, 4);
	VSG::scene_render->shadow_atlas_set_quadrant_subdivision(scenario->reflection_probe_shadow_atlas, 2, 4);
	VSG::scene_render->shadow_atlas_set_quadrant_subdivision(scenario->reflection_probe_shadow_atlas, 3, 8);
	scenario->reflection_atlas = VSG::scene_render->reflection_atlas_create();

	return scenario_rid;
}

void VisualServerScene::scenario_set_physics_interpolation_enabled(RID p_scenario, bool p_enabled) {
	Scenario *scenario = scenario_owner.get(p_scenario);
	ERR_FAIL_COND(!scenario);
	scenario->_interpolation_data.interpolation_enabled = p_enabled;
}

void VisualServerScene::_scenario_tick(RID p_scenario) {
	Scenario *scenario = scenario_owner.get(p_scenario);
	ERR_FAIL_COND(!scenario);

	if (scenario->is_physics_interpolation_enabled()) {
		update_interpolation_tick(scenario->_interpolation_data, true);
	}
}

void VisualServerScene::_scenario_pre_draw(RID p_scenario, bool p_will_draw) {
	Scenario *scenario = scenario_owner.get(p_scenario);
	ERR_FAIL_COND(!scenario);

	// even when running and not drawing scenes, we still need to clear intermediate per frame
	// interpolation data .. hence the p_will_draw flag (so we can reduce the processing if the frame
	// will not be drawn)
	if (scenario->is_physics_interpolation_enabled()) {
		update_interpolation_frame(scenario->_interpolation_data, p_will_draw);
	}
}

void VisualServerScene::scenario_set_debug(RID p_scenario, VS::ScenarioDebugMode p_debug_mode) {
	Scenario *scenario = scenario_owner.get(p_scenario);
	ERR_FAIL_COND(!scenario);
	scenario->debug = p_debug_mode;
}

void VisualServerScene::scenario_set_environment(RID p_scenario, RID p_environment) {
	Scenario *scenario = scenario_owner.get(p_scenario);
	ERR_FAIL_COND(!scenario);
	scenario->environment = p_environment;
}

void VisualServerScene::scenario_set_fallback_environment(RID p_scenario, RID p_environment) {
	Scenario *scenario = scenario_owner.get(p_scenario);
	ERR_FAIL_COND(!scenario);
	scenario->fallback_environment = p_environment;
}

void VisualServerScene::scenario_set_reflection_atlas_size(RID p_scenario, int p_size, int p_subdiv) {
	Scenario *scenario = scenario_owner.get(p_scenario);
	ERR_FAIL_COND(!scenario);
	VSG::scene_render->reflection_atlas_set_size(scenario->reflection_atlas, p_size);
	VSG::scene_render->reflection_atlas_set_subdivision(scenario->reflection_atlas, p_subdiv);
}

/* INSTANCING API */

void VisualServerScene::_instance_queue_update(Instance *p_instance, bool p_update_aabb, bool p_update_materials) {
	if (p_update_aabb) {
		p_instance->update_aabb = true;
	}
	if (p_update_materials) {
		p_instance->update_materials = true;
	}

	if (p_instance->update_item.in_list()) {
		return;
	}

	_instance_update_list.add(&p_instance->update_item);
}

RID VisualServerScene::instance_create() {
	Instance *instance = memnew(Instance);
	ERR_FAIL_COND_V(!instance, RID());

	RID instance_rid = instance_owner.make_rid(instance);
	instance->self = instance_rid;

	return instance_rid;
}

void VisualServerScene::instance_set_base(RID p_instance, RID p_base) {
	Instance *instance = instance_owner.get(p_instance);
	ERR_FAIL_COND(!instance);

	Scenario *scenario = instance->scenario;

	if (instance->base_type != VS::INSTANCE_NONE) {
		//free anything related to that base

		VSG::storage->instance_remove_dependency(instance->base, instance);

		if (scenario && instance->spatial_partition_id) {
			scenario->sps->erase(instance->spatial_partition_id);
			instance->spatial_partition_id = 0;
		}

		switch (instance->base_type) {
			case VS::INSTANCE_LIGHT: {
				InstanceLightData *light = static_cast<InstanceLightData *>(instance->base_data);

				if (instance->scenario && light->D) {
					instance->scenario->directional_lights.erase(light->D);
					light->D = nullptr;
				}
				VSG::scene_render->free(light->instance);
			} break;
			case VS::INSTANCE_REFLECTION_PROBE: {
				InstanceReflectionProbeData *reflection_probe = static_cast<InstanceReflectionProbeData *>(instance->base_data);
				VSG::scene_render->free(reflection_probe->instance);
				if (reflection_probe->update_list.in_list()) {
					reflection_probe_render_list.remove(&reflection_probe->update_list);
				}
			} break;
			default: {
			}
		}

		if (instance->base_data) {
			memdelete(instance->base_data);
			instance->base_data = nullptr;
		}

		instance->blend_values = PoolRealArray();

		for (int i = 0; i < instance->materials.size(); i++) {
			if (instance->materials[i].is_valid()) {
				VSG::storage->material_remove_instance_owner(instance->materials[i], instance);
			}
		}
		instance->materials.clear();
	}

	instance->base_type = VS::INSTANCE_NONE;
	instance->base = RID();

	if (p_base.is_valid()) {
		instance->base_type = VSG::storage->get_base_type(p_base);
		ERR_FAIL_COND(instance->base_type == VS::INSTANCE_NONE);

		switch (instance->base_type) {
			case VS::INSTANCE_LIGHT: {
				InstanceLightData *light = memnew(InstanceLightData);

				if (scenario && VSG::storage->light_get_type(p_base) == VS::LIGHT_DIRECTIONAL) {
					light->D = scenario->directional_lights.push_back(instance);
				}

				light->instance = VSG::scene_render->light_instance_create(p_base);

				instance->base_data = light;
			} break;
			case VS::INSTANCE_MESH:
			case VS::INSTANCE_MULTIMESH:
			case VS::INSTANCE_IMMEDIATE: {
				InstanceGeometryData *geom = memnew(InstanceGeometryData);
				instance->base_data = geom;
				if (instance->base_type == VS::INSTANCE_MESH) {
					instance->blend_values.resize(VSG::storage->mesh_get_blend_shape_count(p_base));
				}
			} break;
			case VS::INSTANCE_REFLECTION_PROBE: {
				InstanceReflectionProbeData *reflection_probe = memnew(InstanceReflectionProbeData);
				reflection_probe->owner = instance;
				instance->base_data = reflection_probe;

				reflection_probe->instance = VSG::scene_render->reflection_probe_instance_create(p_base);
			} break;

			default: {
			}
		}

		VSG::storage->instance_add_dependency(p_base, instance);

		instance->base = p_base;

		if (scenario) {
			_instance_queue_update(instance, true, true);
		}
	}
}
void VisualServerScene::instance_set_scenario(RID p_instance, RID p_scenario) {
	Instance *instance = instance_owner.get(p_instance);
	ERR_FAIL_COND(!instance);

	if (instance->scenario) {
		instance->scenario->instances.remove(&instance->scenario_item);

		if (instance->spatial_partition_id) {
			instance->scenario->sps->erase(instance->spatial_partition_id);
			instance->spatial_partition_id = 0;
		}

		// handle occlusion changes
		if (instance->occlusion_handle) {
			_instance_destroy_occlusion_rep(instance);
		}

		// remove any interpolation data associated with the instance in this scenario
		instance->scenario->_interpolation_data.notify_free_instance(p_instance, *instance);

		switch (instance->base_type) {
			case VS::INSTANCE_LIGHT: {
				InstanceLightData *light = static_cast<InstanceLightData *>(instance->base_data);

				if (light->D) {
					instance->scenario->directional_lights.erase(light->D);
					light->D = nullptr;
				}
			} break;
			case VS::INSTANCE_REFLECTION_PROBE: {
				InstanceReflectionProbeData *reflection_probe = static_cast<InstanceReflectionProbeData *>(instance->base_data);
				VSG::scene_render->reflection_probe_release_atlas_index(reflection_probe->instance);
			} break;
			default: {
			}
		}

		instance->scenario = nullptr;
	}

	if (p_scenario.is_valid()) {
		Scenario *scenario = scenario_owner.get(p_scenario);
		ERR_FAIL_COND(!scenario);

		instance->scenario = scenario;

		scenario->instances.add(&instance->scenario_item);

		switch (instance->base_type) {
			case VS::INSTANCE_LIGHT: {
				InstanceLightData *light = static_cast<InstanceLightData *>(instance->base_data);

				if (VSG::storage->light_get_type(instance->base) == VS::LIGHT_DIRECTIONAL) {
					light->D = scenario->directional_lights.push_back(instance);
				}
			} break;
			default: {
			}
		}

		// handle occlusion changes if necessary
		_instance_create_occlusion_rep(instance);

		_instance_queue_update(instance, true, true);
	}
}
void VisualServerScene::instance_set_layer_mask(RID p_instance, uint32_t p_mask) {
	Instance *instance = instance_owner.get(p_instance);
	ERR_FAIL_COND(!instance);

	instance->layer_mask = p_mask;
}

void VisualServerScene::instance_reset_physics_interpolation(RID p_instance) {
	Instance *instance = instance_owner.get(p_instance);
	ERR_FAIL_COND(!instance);

	if (instance->is_currently_interpolated()) {
		instance->scenario->_interpolation_data.instance_teleport_list.push_back(p_instance);
	}
}

void VisualServerScene::instance_set_interpolated(RID p_instance, bool p_interpolated) {
	Instance *instance = instance_owner.get(p_instance);
	ERR_FAIL_COND(!instance);
	instance->interpolated = p_interpolated;
}

void VisualServerScene::instance_set_transform(RID p_instance, const Transform &p_transform) {
	Instance *instance = instance_owner.get(p_instance);
	ERR_FAIL_COND(!instance);

	if (!instance->is_currently_interpolated() || !instance->scenario) {
		if (instance->transform == p_transform) {
			return; //must be checked to avoid worst evil
		}

#ifdef DEV_ENABLED
		// If we are interpolated but without a scenario, unsure whether
		// this should be supported...
		if (instance->is_currently_interpolated()) {
			WARN_PRINT_ONCE("Instance interpolated without a scenario.");
		}
#endif

#ifdef DEBUG_ENABLED

		for (int i = 0; i < 4; i++) {
			const Vector3 &v = i < 3 ? p_transform.basis.elements[i] : p_transform.origin;
			ERR_FAIL_COND(Math::is_inf(v.x));
			ERR_FAIL_COND(Math::is_nan(v.x));
			ERR_FAIL_COND(Math::is_inf(v.y));
			ERR_FAIL_COND(Math::is_nan(v.y));
			ERR_FAIL_COND(Math::is_inf(v.z));
			ERR_FAIL_COND(Math::is_nan(v.z));
		}

#endif
		instance->transform = p_transform;
		_instance_queue_update(instance, true);
		return;
	}

	float new_checksum = TransformInterpolator::checksum_transform(p_transform);
	bool checksums_match = (instance->transform_checksum_curr == new_checksum) && (instance->transform_checksum_prev == new_checksum);

	// we can't entirely reject no changes because we need the interpolation
	// system to keep on stewing

	// Optimized check. First checks the checksums. If they pass it does the slow check at the end.
	// Alternatively we can do this non-optimized and ignore the checksum...
	// if no change
	if (checksums_match && (instance->transform_curr == p_transform) && (instance->transform_prev == p_transform)) {
		return;
	}

#ifdef DEBUG_ENABLED

	for (int i = 0; i < 4; i++) {
		const Vector3 &v = i < 3 ? p_transform.basis.elements[i] : p_transform.origin;
		ERR_FAIL_COND(Math::is_inf(v.x));
		ERR_FAIL_COND(Math::is_nan(v.x));
		ERR_FAIL_COND(Math::is_inf(v.y));
		ERR_FAIL_COND(Math::is_nan(v.y));
		ERR_FAIL_COND(Math::is_inf(v.z));
		ERR_FAIL_COND(Math::is_nan(v.z));
	}

#endif

	instance->transform_curr = p_transform;

	// keep checksums up to date
	instance->transform_checksum_curr = new_checksum;

	if (!instance->on_interpolate_transform_list) {
		instance->scenario->_interpolation_data.instance_transform_update_list_curr->push_back(p_instance);
		instance->on_interpolate_transform_list = true;
	} else {
		DEV_ASSERT(instance->scenario->_interpolation_data.instance_transform_update_list_curr->size());
	}

	// If the instance is invisible, then we are simply updating the data flow, there is no need to calculate the interpolated
	// transform or anything else.
	// Ideally we would not even call the VisualServer::set_transform() when invisible but that would entail having logic
	// to keep track of the previous transform on the SceneTree side. The "early out" below is less efficient but a lot cleaner codewise.
	if (!instance->visible) {
		return;
	}

	// decide on the interpolation method .. slerp if possible
	instance->interpolation_method = TransformInterpolator::find_method(instance->transform_prev.basis, instance->transform_curr.basis);

	if (!instance->on_interpolate_list) {
		instance->scenario->_interpolation_data.instance_interpolate_update_list.push_back(p_instance);
		instance->on_interpolate_list = true;
	} else {
		DEV_ASSERT(instance->scenario->_interpolation_data.instance_interpolate_update_list.size());
	}

	_instance_queue_update(instance, true);
}

void VisualServerScene::Scenario::InterpolationData::notify_free_camera(RID p_rid, Camera &r_camera) {
	r_camera.on_interpolate_transform_list = false;

	if (!interpolation_enabled) {
		return;
	}

	// if the camera was on any of the lists, remove
	camera_transform_update_list_curr->erase_multiple_unordered(p_rid);
	camera_transform_update_list_prev->erase_multiple_unordered(p_rid);
	camera_teleport_list.erase_multiple_unordered(p_rid);
}

void VisualServerScene::Scenario::InterpolationData::notify_free_instance(RID p_rid, Instance &r_instance) {
	r_instance.on_interpolate_list = false;
	r_instance.on_interpolate_transform_list = false;

	if (!interpolation_enabled) {
		return;
	}

	// if the instance was on any of the lists, remove
	instance_interpolate_update_list.erase_multiple_unordered(p_rid);
	instance_transform_update_list_curr->erase_multiple_unordered(p_rid);
	instance_transform_update_list_prev->erase_multiple_unordered(p_rid);
	instance_teleport_list.erase_multiple_unordered(p_rid);
}

void VisualServerScene::update_interpolation_tick(Scenario::InterpolationData &r_interpolation_data, bool p_process) {
	// update interpolation in storage
	VSG::storage->update_interpolation_tick(p_process);

	// detect any that were on the previous transform list that are no longer active,
	// we should remove them from the interpolate list

	for (unsigned int n = 0; n < r_interpolation_data.instance_transform_update_list_prev->size(); n++) {
		const RID &rid = (*r_interpolation_data.instance_transform_update_list_prev)[n];
		Instance *instance = instance_owner.getornull(rid);

		bool active = true;

		// no longer active? (either the instance deleted or no longer being transformed)
		if (instance && !instance->on_interpolate_transform_list) {
			active = false;
			instance->on_interpolate_list = false;

			// make sure the most recent transform is set
			instance->transform = instance->transform_curr;

			// and that both prev and current are the same, just in case of any interpolations
			instance->transform_prev = instance->transform_curr;

			// make sure are updated one more time to ensure the AABBs are correct
			_instance_queue_update(instance, true);
		}

		if (!instance) {
			active = false;
		}

		if (!active) {
			r_interpolation_data.instance_interpolate_update_list.erase(rid);
		}
	}

	// and now for any in the transform list (being actively interpolated), keep the previous transform
	// value up to date ready for the next tick
	if (p_process) {
		for (unsigned int n = 0; n < r_interpolation_data.instance_transform_update_list_curr->size(); n++) {
			const RID &rid = (*r_interpolation_data.instance_transform_update_list_curr)[n];
			Instance *instance = instance_owner.getornull(rid);
			if (instance) {
				instance->transform_prev = instance->transform_curr;
				instance->transform_checksum_prev = instance->transform_checksum_curr;
				instance->on_interpolate_transform_list = false;
			}
		}
	}

	// we maintain a mirror list for the transform updates, so we can detect when an instance
	// is no longer being transformed, and remove it from the interpolate list
	SWAP(r_interpolation_data.instance_transform_update_list_curr, r_interpolation_data.instance_transform_update_list_prev);

	// prepare for the next iteration
	r_interpolation_data.instance_transform_update_list_curr->clear();

	// CAMERAS
	// detect any that were on the previous transform list that are no longer active,
	for (unsigned int n = 0; n < r_interpolation_data.camera_transform_update_list_prev->size(); n++) {
		const RID &rid = (*r_interpolation_data.camera_transform_update_list_prev)[n];
		Camera *camera = camera_owner.getornull(rid);

		// no longer active? (either the instance deleted or no longer being transformed)
		if (camera && !camera->on_interpolate_transform_list) {
			camera->transform = camera->transform_prev;
		}
	}

	// cameras , swap any current with previous
	for (unsigned int n = 0; n < r_interpolation_data.camera_transform_update_list_curr->size(); n++) {
		const RID &rid = (*r_interpolation_data.camera_transform_update_list_curr)[n];
		Camera *camera = camera_owner.getornull(rid);
		if (camera) {
			camera->transform_prev = camera->transform;
			camera->on_interpolate_transform_list = false;
		}
	}

	// we maintain a mirror list for the transform updates, so we can detect when an instance
	// is no longer being transformed, and remove it from the interpolate list
	SWAP(r_interpolation_data.camera_transform_update_list_curr, r_interpolation_data.camera_transform_update_list_prev);

	// prepare for the next iteration
	r_interpolation_data.camera_transform_update_list_curr->clear();
}

void VisualServerScene::update_interpolation_frame(Scenario::InterpolationData &r_interpolation_data, bool p_process) {
	// update interpolation in storage
	VSG::storage->update_interpolation_frame(p_process);

	// teleported instances
	for (unsigned int n = 0; n < r_interpolation_data.instance_teleport_list.size(); n++) {
		const RID &rid = r_interpolation_data.instance_teleport_list[n];
		Instance *instance = instance_owner.getornull(rid);
		if (instance) {
			instance->transform_prev = instance->transform_curr;
			instance->transform_checksum_prev = instance->transform_checksum_curr;
		}
	}

	r_interpolation_data.instance_teleport_list.clear();

	// camera teleports
	for (unsigned int n = 0; n < r_interpolation_data.camera_teleport_list.size(); n++) {
		const RID &rid = r_interpolation_data.camera_teleport_list[n];
		Camera *camera = camera_owner.getornull(rid);
		if (camera) {
			camera->transform_prev = camera->transform;
		}
	}

	r_interpolation_data.camera_teleport_list.clear();

	if (p_process) {
		real_t f = Engine::get_singleton()->get_physics_interpolation_fraction();

		for (unsigned int i = 0; i < r_interpolation_data.instance_interpolate_update_list.size(); i++) {
			const RID &rid = r_interpolation_data.instance_interpolate_update_list[i];
			Instance *instance = instance_owner.getornull(rid);
			if (instance) {
				TransformInterpolator::interpolate_transform_via_method(instance->transform_prev, instance->transform_curr, instance->transform, f, instance->interpolation_method);

				// make sure AABBs are constantly up to date through the interpolation
				_instance_queue_update(instance, true);
			}
		} // for n
	}
}

void VisualServerScene::instance_attach_object_instance_id(RID p_instance, ObjectID p_id) {
	Instance *instance = instance_owner.get(p_instance);
	ERR_FAIL_COND(!instance);

	instance->object_id = p_id;
}
void VisualServerScene::instance_set_blend_shape_weight(RID p_instance, int p_shape, float p_weight) {
	Instance *instance = instance_owner.get(p_instance);
	ERR_FAIL_COND(!instance);

	if (instance->update_item.in_list()) {
		_update_dirty_instance(instance);
	}

	ERR_FAIL_INDEX(p_shape, instance->blend_values.size());
	instance->blend_values.write().ptr()[p_shape] = p_weight;
	VSG::storage->mesh_set_blend_shape_values(instance->base, instance->blend_values);
}

void VisualServerScene::instance_set_surface_material(RID p_instance, int p_surface, RID p_material) {
	Instance *instance = instance_owner.get(p_instance);
	ERR_FAIL_COND(!instance);

	if (instance->base_type == VS::INSTANCE_MESH) {
		//may not have been updated yet
		instance->materials.resize(VSG::storage->mesh_get_surface_count(instance->base));
	}

	ERR_FAIL_INDEX(p_surface, instance->materials.size());

	if (instance->materials[p_surface].is_valid()) {
		VSG::storage->material_remove_instance_owner(instance->materials[p_surface], instance);
	}
	instance->materials.write[p_surface] = p_material;
	instance->base_changed(false, true);

	if (instance->materials[p_surface].is_valid()) {
		VSG::storage->material_add_instance_owner(instance->materials[p_surface], instance);
	}
}

void VisualServerScene::instance_set_visible(RID p_instance, bool p_visible) {
	Instance *instance = instance_owner.get(p_instance);
	ERR_FAIL_COND(!instance);

	if (instance->visible == p_visible) {
		return;
	}

	instance->visible = p_visible;

	// Special case for physics interpolation, we want to ensure the interpolated data is up to date
	if (instance->scenario->_interpolation_data.interpolation_enabled && p_visible && instance->interpolated && instance->scenario && !instance->on_interpolate_list) {
		// Do all the extra work we normally do on instance_set_transform(), because this is optimized out for hidden instances.
		// This prevents a glitch of stale interpolation transform data when unhiding before the next physics tick.
		instance->interpolation_method = TransformInterpolator::find_method(instance->transform_prev.basis, instance->transform_curr.basis);
		instance->scenario->_interpolation_data.instance_interpolate_update_list.push_back(p_instance);
		instance->on_interpolate_list = true;
		_instance_queue_update(instance, true);

		// We must also place on the transform update list for a tick, so the system
		// can auto-detect if the instance is no longer moving, and remove from the interpolate lists again.
		// If this step is ignored, an unmoving instance could remain on the interpolate lists indefinitely
		// (or rather until the object is deleted) and cause unnecessary updates and drawcalls.
		if (!instance->on_interpolate_transform_list) {
			instance->scenario->_interpolation_data.instance_transform_update_list_curr->push_back(p_instance);
			instance->on_interpolate_transform_list = true;
		}
	}

	// give the opportunity for the spatial partitioning scene to use a special implementation of visibility
	// for efficiency (supported in BVH but not octree)

	// slightly bug prone optimization here - we want to avoid doing a collision check twice
	// once when activating, and once when calling set_pairable. We do this by deferring the collision check.
	// However, in some cases (notably meshes), set_pairable never gets called. So we want to catch this case
	// and force a collision check (see later in this function).
	// This is only done in two stages to maintain compatibility with the octree.
	if (instance->spatial_partition_id && instance->scenario) {
		if (p_visible) {
			instance->scenario->sps->activate(instance->spatial_partition_id, instance->transformed_aabb);
		} else {
			instance->scenario->sps->deactivate(instance->spatial_partition_id);
		}
	}

	// when showing or hiding geometry, lights must be kept up to date to show / hide shadows
	if ((1 << instance->base_type) & VS::INSTANCE_GEOMETRY_MASK) {
		InstanceGeometryData *geom = static_cast<InstanceGeometryData *>(instance->base_data);

		if (geom->can_cast_shadows) {
			for (List<Instance *>::Element *E = geom->lighting.front(); E; E = E->next()) {
				InstanceLightData *light = static_cast<InstanceLightData *>(E->get()->base_data);
				light->shadow_dirty = true;
			}
		}
	}

	switch (instance->base_type) {
		case VS::INSTANCE_LIGHT: {
			if (VSG::storage->light_get_type(instance->base) != VS::LIGHT_DIRECTIONAL && instance->spatial_partition_id && instance->scenario) {
				instance->scenario->sps->set_pairable(instance, p_visible, 1 << VS::INSTANCE_LIGHT, p_visible ? VS::INSTANCE_GEOMETRY_MASK : 0);
			}

		} break;
		case VS::INSTANCE_REFLECTION_PROBE: {
			if (instance->spatial_partition_id && instance->scenario) {
				instance->scenario->sps->set_pairable(instance, p_visible, 1 << VS::INSTANCE_REFLECTION_PROBE, p_visible ? VS::INSTANCE_GEOMETRY_MASK : 0);
			}

		} break;
		default: {
			// if we haven't called set_pairable, we STILL need to do a collision check
			// for activated items because we deferred it earlier in the call to activate.
			if (instance->spatial_partition_id && instance->scenario && p_visible) {
				instance->scenario->sps->force_collision_check(instance->spatial_partition_id);
			}
		}
	}
}
inline bool is_geometry_instance(VisualServer::InstanceType p_type) {
	return p_type == VS::INSTANCE_MESH || p_type == VS::INSTANCE_MULTIMESH || p_type == VS::INSTANCE_IMMEDIATE;
}

void VisualServerScene::instance_set_custom_aabb(RID p_instance, AABB p_aabb) {
	Instance *instance = instance_owner.get(p_instance);
	ERR_FAIL_COND(!instance);
	ERR_FAIL_COND(!is_geometry_instance(instance->base_type));

	if (p_aabb != AABB()) {
		// Set custom AABB
		if (instance->custom_aabb == nullptr) {
			instance->custom_aabb = memnew(AABB);
		}
		*instance->custom_aabb = p_aabb;

	} else {
		// Clear custom AABB
		if (instance->custom_aabb != nullptr) {
			memdelete(instance->custom_aabb);
			instance->custom_aabb = nullptr;
		}
	}

	if (instance->scenario) {
		_instance_queue_update(instance, true, false);
	}
}

void VisualServerScene::instance_attach_skeleton(RID p_instance, RID p_skeleton) {
	Instance *instance = instance_owner.get(p_instance);
	ERR_FAIL_COND(!instance);

	if (instance->skeleton == p_skeleton) {
		return;
	}

	if (instance->skeleton.is_valid()) {
		VSG::storage->instance_remove_skeleton(instance->skeleton, instance);
	}

	instance->skeleton = p_skeleton;

	if (instance->skeleton.is_valid()) {
		VSG::storage->instance_add_skeleton(instance->skeleton, instance);
	}

	_instance_queue_update(instance, true);
}

void VisualServerScene::instance_set_exterior(RID p_instance, bool p_enabled) {
}

void VisualServerScene::instance_set_extra_visibility_margin(RID p_instance, real_t p_margin) {
	Instance *instance = instance_owner.get(p_instance);
	ERR_FAIL_COND(!instance);

	instance->extra_margin = p_margin;
	_instance_queue_update(instance, true, false);
}

// Portals
void VisualServerScene::instance_set_portal_mode(RID p_instance, VisualServer::InstancePortalMode p_mode) {
	Instance *instance = instance_owner.get(p_instance);
	ERR_FAIL_COND(!instance);

	// no change?
	if (instance->portal_mode == p_mode) {
		return;
	}

	// should this happen?
	if (!instance->scenario) {
		instance->portal_mode = p_mode;
		return;
	}

	// destroy previous occlusion instance?
	_instance_destroy_occlusion_rep(instance);
	instance->portal_mode = p_mode;
	_instance_create_occlusion_rep(instance);
}

void VisualServerScene::_instance_create_occlusion_rep(Instance *p_instance) {
	ERR_FAIL_COND(!p_instance);
	ERR_FAIL_COND(!p_instance->scenario);

	switch (p_instance->portal_mode) {
		default: {
			p_instance->occlusion_handle = 0;
		} break;
		case VisualServer::InstancePortalMode::INSTANCE_PORTAL_MODE_ROAMING: {
			p_instance->occlusion_handle = p_instance->scenario->_portal_renderer.instance_moving_create(p_instance, p_instance->self, false, p_instance->transformed_aabb);
		} break;
		case VisualServer::InstancePortalMode::INSTANCE_PORTAL_MODE_GLOBAL: {
			p_instance->occlusion_handle = p_instance->scenario->_portal_renderer.instance_moving_create(p_instance, p_instance->self, true, p_instance->transformed_aabb);
		} break;
	}
}

void VisualServerScene::_instance_destroy_occlusion_rep(Instance *p_instance) {
	ERR_FAIL_COND(!p_instance);
	ERR_FAIL_COND(!p_instance->scenario);

	// not an error, can occur
	if (!p_instance->occlusion_handle) {
		return;
	}

	p_instance->scenario->_portal_renderer.instance_moving_destroy(p_instance->occlusion_handle);

	// unset
	p_instance->occlusion_handle = 0;
}

void *VisualServerScene::_instance_get_from_rid(RID p_instance) {
	Instance *instance = instance_owner.get(p_instance);
	return instance;
}

bool VisualServerScene::_instance_get_transformed_aabb(RID p_instance, AABB &r_aabb) {
	Instance *instance = instance_owner.get(p_instance);
	ERR_FAIL_NULL_V(instance, false);

	r_aabb = instance->transformed_aabb;

	return true;
}

// the portal has to be associated with a scenario, this is assumed to be
// the same scenario as the portal node
RID VisualServerScene::portal_create() {
	Portal *portal = memnew(Portal);
	ERR_FAIL_COND_V(!portal, RID());
	RID portal_rid = portal_owner.make_rid(portal);
	return portal_rid;
}

// should not be called multiple times, different scenarios etc, but just in case, we will support this
void VisualServerScene::portal_set_scenario(RID p_portal, RID p_scenario) {
	Portal *portal = portal_owner.getornull(p_portal);
	ERR_FAIL_COND(!portal);
	Scenario *scenario = scenario_owner.getornull(p_scenario);

	// noop?
	if (portal->scenario == scenario) {
		return;
	}

	// if the portal is in a scenario already, remove it
	if (portal->scenario) {
		portal->scenario->_portal_renderer.portal_destroy(portal->scenario_portal_id);
		portal->scenario = nullptr;
		portal->scenario_portal_id = 0;
	}

	// create when entering the world
	if (scenario) {
		portal->scenario = scenario;

		// defer the actual creation to here
		portal->scenario_portal_id = scenario->_portal_renderer.portal_create();
	}
}

void VisualServerScene::portal_set_geometry(RID p_portal, const Vector<Vector3> &p_points, real_t p_margin) {
	Portal *portal = portal_owner.getornull(p_portal);
	ERR_FAIL_COND(!portal);
	ERR_FAIL_COND(!portal->scenario);
	portal->scenario->_portal_renderer.portal_set_geometry(portal->scenario_portal_id, p_points, p_margin);
}

void VisualServerScene::portal_link(RID p_portal, RID p_room_from, RID p_room_to, bool p_two_way) {
	Portal *portal = portal_owner.getornull(p_portal);
	ERR_FAIL_COND(!portal);
	ERR_FAIL_COND(!portal->scenario);

	Room *room_from = room_owner.getornull(p_room_from);
	ERR_FAIL_COND(!room_from);
	Room *room_to = room_owner.getornull(p_room_to);
	ERR_FAIL_COND(!room_to);

	portal->scenario->_portal_renderer.portal_link(portal->scenario_portal_id, room_from->scenario_room_id, room_to->scenario_room_id, p_two_way);
}

void VisualServerScene::portal_set_active(RID p_portal, bool p_active) {
	Portal *portal = portal_owner.getornull(p_portal);
	ERR_FAIL_COND(!portal);
	ERR_FAIL_COND(!portal->scenario);
	portal->scenario->_portal_renderer.portal_set_active(portal->scenario_portal_id, p_active);
}

RID VisualServerScene::ghost_create() {
	Ghost *ci = memnew(Ghost);
	ERR_FAIL_COND_V(!ci, RID());
	RID ci_rid = ghost_owner.make_rid(ci);
	return ci_rid;
}

void VisualServerScene::ghost_set_scenario(RID p_ghost, RID p_scenario, ObjectID p_id, const AABB &p_aabb) {
	Ghost *ci = ghost_owner.getornull(p_ghost);
	ERR_FAIL_COND(!ci);

	ci->aabb = p_aabb;
	ci->object_id = p_id;

	Scenario *scenario = scenario_owner.getornull(p_scenario);

	// noop?
	if (ci->scenario == scenario) {
		return;
	}

	// if the portal is in a scenario already, remove it
	if (ci->scenario) {
		_ghost_destroy_occlusion_rep(ci);
		ci->scenario = nullptr;
	}

	// create when entering the world
	if (scenario) {
		ci->scenario = scenario;

		// defer the actual creation to here
		_ghost_create_occlusion_rep(ci);
	}
}

void VisualServerScene::ghost_update(RID p_ghost, const AABB &p_aabb) {
	Ghost *ci = ghost_owner.getornull(p_ghost);
	ERR_FAIL_COND(!ci);
	ERR_FAIL_COND(!ci->scenario);

	ci->aabb = p_aabb;

	if (ci->rghost_handle) {
		ci->scenario->_portal_renderer.rghost_update(ci->rghost_handle, p_aabb);
	}
}

void VisualServerScene::_ghost_create_occlusion_rep(Ghost *p_ghost) {
	ERR_FAIL_COND(!p_ghost);
	ERR_FAIL_COND(!p_ghost->scenario);

	if (!p_ghost->rghost_handle) {
		p_ghost->rghost_handle = p_ghost->scenario->_portal_renderer.rghost_create(p_ghost->object_id, p_ghost->aabb);
	}
}

void VisualServerScene::_ghost_destroy_occlusion_rep(Ghost *p_ghost) {
	ERR_FAIL_COND(!p_ghost);
	ERR_FAIL_COND(!p_ghost->scenario);

	// not an error, can occur
	if (!p_ghost->rghost_handle) {
		return;
	}

	p_ghost->scenario->_portal_renderer.rghost_destroy(p_ghost->rghost_handle);
	p_ghost->rghost_handle = 0;
}

RID VisualServerScene::roomgroup_create() {
	RoomGroup *rg = memnew(RoomGroup);
	ERR_FAIL_COND_V(!rg, RID());
	RID roomgroup_rid = roomgroup_owner.make_rid(rg);
	return roomgroup_rid;
}

void VisualServerScene::roomgroup_prepare(RID p_roomgroup, ObjectID p_roomgroup_object_id) {
	RoomGroup *roomgroup = roomgroup_owner.getornull(p_roomgroup);
	ERR_FAIL_COND(!roomgroup);
	ERR_FAIL_COND(!roomgroup->scenario);
	roomgroup->scenario->_portal_renderer.roomgroup_prepare(roomgroup->scenario_roomgroup_id, p_roomgroup_object_id);
}

void VisualServerScene::roomgroup_set_scenario(RID p_roomgroup, RID p_scenario) {
	RoomGroup *rg = roomgroup_owner.getornull(p_roomgroup);
	ERR_FAIL_COND(!rg);
	Scenario *scenario = scenario_owner.getornull(p_scenario);

	// noop?
	if (rg->scenario == scenario) {
		return;
	}

	// if the portal is in a scenario already, remove it
	if (rg->scenario) {
		rg->scenario->_portal_renderer.roomgroup_destroy(rg->scenario_roomgroup_id);
		rg->scenario = nullptr;
		rg->scenario_roomgroup_id = 0;
	}

	// create when entering the world
	if (scenario) {
		rg->scenario = scenario;

		// defer the actual creation to here
		rg->scenario_roomgroup_id = scenario->_portal_renderer.roomgroup_create();
	}
}

void VisualServerScene::roomgroup_add_room(RID p_roomgroup, RID p_room) {
	RoomGroup *roomgroup = roomgroup_owner.getornull(p_roomgroup);
	ERR_FAIL_COND(!roomgroup);
	ERR_FAIL_COND(!roomgroup->scenario);

	Room *room = room_owner.getornull(p_room);
	ERR_FAIL_COND(!room);
	ERR_FAIL_COND(!room->scenario);

	ERR_FAIL_COND(roomgroup->scenario != room->scenario);
	roomgroup->scenario->_portal_renderer.roomgroup_add_room(roomgroup->scenario_roomgroup_id, room->scenario_room_id);
}

// Occluders
RID VisualServerScene::occluder_instance_create() {
	OccluderInstance *ro = memnew(OccluderInstance);
	ERR_FAIL_COND_V(!ro, RID());
	RID occluder_rid = occluder_instance_owner.make_rid(ro);
	return occluder_rid;
}

void VisualServerScene::occluder_instance_link_resource(RID p_occluder_instance, RID p_occluder_resource) {
	OccluderInstance *oi = occluder_instance_owner.getornull(p_occluder_instance);
	ERR_FAIL_COND(!oi);
	ERR_FAIL_COND(!oi->scenario);

	OccluderResource *res = occluder_resource_owner.getornull(p_occluder_resource);
	ERR_FAIL_COND(!res);

	oi->scenario->_portal_renderer.occluder_instance_link(oi->scenario_occluder_id, res->occluder_resource_id);
}

void VisualServerScene::occluder_instance_set_scenario(RID p_occluder_instance, RID p_scenario) {
	OccluderInstance *oi = occluder_instance_owner.getornull(p_occluder_instance);
	ERR_FAIL_COND(!oi);
	Scenario *scenario = scenario_owner.getornull(p_scenario);

	// noop?
	if (oi->scenario == scenario) {
		return;
	}

	// if the portal is in a scenario already, remove it
	if (oi->scenario) {
		oi->scenario->_portal_renderer.occluder_instance_destroy(oi->scenario_occluder_id);
		oi->scenario = nullptr;
		oi->scenario_occluder_id = 0;
	}

	// create when entering the world
	if (scenario) {
		oi->scenario = scenario;
		oi->scenario_occluder_id = scenario->_portal_renderer.occluder_instance_create();
	}
}

void VisualServerScene::occluder_instance_set_active(RID p_occluder_instance, bool p_active) {
	OccluderInstance *oi = occluder_instance_owner.getornull(p_occluder_instance);
	ERR_FAIL_COND(!oi);
	ERR_FAIL_COND(!oi->scenario);
	oi->scenario->_portal_renderer.occluder_instance_set_active(oi->scenario_occluder_id, p_active);
}

void VisualServerScene::occluder_instance_set_transform(RID p_occluder_instance, const Transform &p_xform) {
	OccluderInstance *oi = occluder_instance_owner.getornull(p_occluder_instance);
	ERR_FAIL_COND(!oi);
	ERR_FAIL_COND(!oi->scenario);
	oi->scenario->_portal_renderer.occluder_instance_set_transform(oi->scenario_occluder_id, p_xform);
}

RID VisualServerScene::occluder_resource_create() {
	OccluderResource *res = memnew(OccluderResource);
	ERR_FAIL_COND_V(!res, RID());

	res->occluder_resource_id = _portal_resources.occluder_resource_create();

	RID occluder_resource_rid = occluder_resource_owner.make_rid(res);
	return occluder_resource_rid;
}

void VisualServerScene::occluder_resource_prepare(RID p_occluder_resource, VisualServer::OccluderType p_type) {
	OccluderResource *res = occluder_resource_owner.getornull(p_occluder_resource);
	ERR_FAIL_COND(!res);
	_portal_resources.occluder_resource_prepare(res->occluder_resource_id, (VSOccluder_Instance::Type)p_type);
}

void VisualServerScene::occluder_resource_spheres_update(RID p_occluder_resource, const Vector<Plane> &p_spheres) {
	OccluderResource *res = occluder_resource_owner.getornull(p_occluder_resource);
	ERR_FAIL_COND(!res);
	_portal_resources.occluder_resource_update_spheres(res->occluder_resource_id, p_spheres);
}

void VisualServerScene::occluder_resource_mesh_update(RID p_occluder_resource, const Geometry::OccluderMeshData &p_mesh_data) {
	OccluderResource *res = occluder_resource_owner.getornull(p_occluder_resource);
	ERR_FAIL_COND(!res);
	_portal_resources.occluder_resource_update_mesh(res->occluder_resource_id, p_mesh_data);
}

void VisualServerScene::set_use_occlusion_culling(bool p_enable) {
	// this is not scenario specific, and is global
	// (mainly for debugging)
	PortalRenderer::use_occlusion_culling = p_enable;
}

Geometry::MeshData VisualServerScene::occlusion_debug_get_current_polys(RID p_scenario) const {
	Scenario *scenario = scenario_owner.getornull(p_scenario);
	if (!scenario) {
		return Geometry::MeshData();
	}

	return scenario->_portal_renderer.occlusion_debug_get_current_polys();
}

// Rooms
void VisualServerScene::callbacks_register(VisualServerCallbacks *p_callbacks) {
	_visual_server_callbacks = p_callbacks;
}

// the room has to be associated with a scenario, this is assumed to be
// the same scenario as the room node
RID VisualServerScene::room_create() {
	Room *room = memnew(Room);
	ERR_FAIL_COND_V(!room, RID());
	RID room_rid = room_owner.make_rid(room);
	return room_rid;
}

// should not be called multiple times, different scenarios etc, but just in case, we will support this
void VisualServerScene::room_set_scenario(RID p_room, RID p_scenario) {
	Room *room = room_owner.getornull(p_room);
	ERR_FAIL_COND(!room);
	Scenario *scenario = scenario_owner.getornull(p_scenario);

	// no change?
	if (room->scenario == scenario) {
		return;
	}

	// if the room has an existing scenario, remove from it
	if (room->scenario) {
		room->scenario->_portal_renderer.room_destroy(room->scenario_room_id);
		room->scenario = nullptr;
		room->scenario_room_id = 0;
	}

	// create when entering the world
	if (scenario) {
		room->scenario = scenario;

		// defer the actual creation to here
		room->scenario_room_id = scenario->_portal_renderer.room_create();
	}
}

void VisualServerScene::room_add_ghost(RID p_room, ObjectID p_object_id, const AABB &p_aabb) {
	Room *room = room_owner.getornull(p_room);
	ERR_FAIL_COND(!room);
	ERR_FAIL_COND(!room->scenario);

	room->scenario->_portal_renderer.room_add_ghost(room->scenario_room_id, p_object_id, p_aabb);
}

void VisualServerScene::room_add_instance(RID p_room, RID p_instance, const AABB &p_aabb, const Vector<Vector3> &p_object_pts) {
	Room *room = room_owner.getornull(p_room);
	ERR_FAIL_COND(!room);
	ERR_FAIL_COND(!room->scenario);

	Instance *instance = instance_owner.getornull(p_instance);
	ERR_FAIL_COND(!instance);

	AABB bb = p_aabb;

	// the aabb passed from the client takes no account of the extra cull margin,
	// so we need to add this manually.
	// It is assumed it is in world space.
	if (instance->extra_margin != 0.0) {
		bb.grow_by(instance->extra_margin);
	}

	bool dynamic = false;

	// don't add if portal mode is not static or dynamic
	switch (instance->portal_mode) {
		default: {
			return; // this should be taken care of by the calling function, but just in case
		} break;
		case VisualServer::InstancePortalMode::INSTANCE_PORTAL_MODE_DYNAMIC: {
			dynamic = true;
		} break;
		case VisualServer::InstancePortalMode::INSTANCE_PORTAL_MODE_STATIC: {
			dynamic = false;
		} break;
	}

	instance->occlusion_handle = room->scenario->_portal_renderer.room_add_instance(room->scenario_room_id, p_instance, bb, dynamic, p_object_pts);
}

void VisualServerScene::room_prepare(RID p_room, int32_t p_priority) {
	Room *room = room_owner.getornull(p_room);
	ERR_FAIL_COND(!room);
	ERR_FAIL_COND(!room->scenario);
	room->scenario->_portal_renderer.room_prepare(room->scenario_room_id, p_priority);
}

void VisualServerScene::room_set_bound(RID p_room, ObjectID p_room_object_id, const Vector<Plane> &p_convex, const AABB &p_aabb, const Vector<Vector3> &p_verts) {
	Room *room = room_owner.getornull(p_room);
	ERR_FAIL_COND(!room);
	ERR_FAIL_COND(!room->scenario);
	room->scenario->_portal_renderer.room_set_bound(room->scenario_room_id, p_room_object_id, p_convex, p_aabb, p_verts);
}

void VisualServerScene::rooms_unload(RID p_scenario, String p_reason) {
	Scenario *scenario = scenario_owner.getornull(p_scenario);
	ERR_FAIL_COND(!scenario);
	scenario->_portal_renderer.rooms_unload(p_reason);
}

void VisualServerScene::rooms_and_portals_clear(RID p_scenario) {
	Scenario *scenario = scenario_owner.getornull(p_scenario);
	ERR_FAIL_COND(!scenario);
	scenario->_portal_renderer.rooms_and_portals_clear();
}

void VisualServerScene::rooms_finalize(RID p_scenario, bool p_generate_pvs, bool p_cull_using_pvs, bool p_use_secondary_pvs, bool p_use_signals, String p_pvs_filename, bool p_use_simple_pvs, bool p_log_pvs_generation) {
	Scenario *scenario = scenario_owner.getornull(p_scenario);
	ERR_FAIL_COND(!scenario);
	scenario->_portal_renderer.rooms_finalize(p_generate_pvs, p_cull_using_pvs, p_use_secondary_pvs, p_use_signals, p_pvs_filename, p_use_simple_pvs, p_log_pvs_generation);
}

void VisualServerScene::rooms_override_camera(RID p_scenario, bool p_override, const Vector3 &p_point, const Vector<Plane> *p_convex) {
	Scenario *scenario = scenario_owner.getornull(p_scenario);
	ERR_FAIL_COND(!scenario);
	scenario->_portal_renderer.rooms_override_camera(p_override, p_point, p_convex);
}

void VisualServerScene::rooms_set_active(RID p_scenario, bool p_active) {
	Scenario *scenario = scenario_owner.getornull(p_scenario);
	ERR_FAIL_COND(!scenario);
	scenario->_portal_renderer.rooms_set_active(p_active);
}

void VisualServerScene::rooms_set_params(RID p_scenario, int p_portal_depth_limit, real_t p_roaming_expansion_margin) {
	Scenario *scenario = scenario_owner.getornull(p_scenario);
	ERR_FAIL_COND(!scenario);
	scenario->_portal_renderer.rooms_set_params(p_portal_depth_limit, p_roaming_expansion_margin);
}

void VisualServerScene::rooms_set_debug_feature(RID p_scenario, VisualServer::RoomsDebugFeature p_feature, bool p_active) {
	Scenario *scenario = scenario_owner.getornull(p_scenario);
	ERR_FAIL_COND(!scenario);
	switch (p_feature) {
		default: {
		} break;
		case VisualServer::ROOMS_DEBUG_SPRAWL: {
			scenario->_portal_renderer.set_debug_sprawl(p_active);
		} break;
	}
}

void VisualServerScene::rooms_update_gameplay_monitor(RID p_scenario, const Vector<Vector3> &p_camera_positions) {
	Scenario *scenario = scenario_owner.getornull(p_scenario);
	ERR_FAIL_COND(!scenario);
	scenario->_portal_renderer.rooms_update_gameplay_monitor(p_camera_positions);
}

bool VisualServerScene::rooms_is_loaded(RID p_scenario) const {
	Scenario *scenario = scenario_owner.getornull(p_scenario);
	ERR_FAIL_COND_V(!scenario, false);
	return scenario->_portal_renderer.rooms_is_loaded();
}

Vector<ObjectID> VisualServerScene::instances_cull_aabb(const AABB &p_aabb, RID p_scenario) const {
	Vector<ObjectID> instances;
	Scenario *scenario = scenario_owner.get(p_scenario);
	ERR_FAIL_COND_V(!scenario, instances);

	const_cast<VisualServerScene *>(this)->update_dirty_instances(); // check dirty instances before culling

	int culled = 0;
	Instance *cull[1024];
	culled = scenario->sps->cull_aabb(p_aabb, cull, 1024);

	for (int i = 0; i < culled; i++) {
		Instance *instance = cull[i];
		ERR_CONTINUE(!instance);
		if (instance->object_id == 0) {
			continue;
		}

		instances.push_back(instance->object_id);
	}

	return instances;
}
Vector<ObjectID> VisualServerScene::instances_cull_ray(const Vector3 &p_from, const Vector3 &p_to, RID p_scenario) const {
	Vector<ObjectID> instances;
	Scenario *scenario = scenario_owner.get(p_scenario);
	ERR_FAIL_COND_V(!scenario, instances);
	const_cast<VisualServerScene *>(this)->update_dirty_instances(); // check dirty instances before culling

	int culled = 0;
	Instance *cull[1024];
	culled = scenario->sps->cull_segment(p_from, p_from + p_to * 10000, cull, 1024);

	for (int i = 0; i < culled; i++) {
		Instance *instance = cull[i];
		ERR_CONTINUE(!instance);
		if (instance->object_id == 0) {
			continue;
		}

		instances.push_back(instance->object_id);
	}

	return instances;
}
Vector<ObjectID> VisualServerScene::instances_cull_convex(const Vector<Plane> &p_convex, RID p_scenario) const {
	Vector<ObjectID> instances;
	Scenario *scenario = scenario_owner.get(p_scenario);
	ERR_FAIL_COND_V(!scenario, instances);
	const_cast<VisualServerScene *>(this)->update_dirty_instances(); // check dirty instances before culling

	int culled = 0;
	Instance *cull[1024];

	culled = scenario->sps->cull_convex(p_convex, cull, 1024);

	for (int i = 0; i < culled; i++) {
		Instance *instance = cull[i];
		ERR_CONTINUE(!instance);
		if (instance->object_id == 0) {
			continue;
		}

		instances.push_back(instance->object_id);
	}

	return instances;
}

// thin wrapper to allow rooms / portals to take over culling if active
int VisualServerScene::_cull_convex_from_point(Scenario *p_scenario, const Transform &p_cam_transform, const CameraMatrix &p_cam_projection, const Vector<Plane> &p_convex, Instance **p_result_array, int p_result_max, int32_t &r_previous_room_id_hint, uint32_t p_mask) {
	int res = -1;
	if (p_scenario->_portal_renderer.is_active()) {
		// Note that the portal renderer ASSUMES that the planes exactly match the convention in
		// CameraMatrix of enum Planes (6 planes, in order, near, far etc)
		// If this is not the case, it should not be used.
		res = p_scenario->_portal_renderer.cull_convex(p_cam_transform, p_cam_projection, p_convex, (VSInstance **)p_result_array, p_result_max, p_mask, r_previous_room_id_hint);
	}

	// fallback to BVH  / octree if portals not active
	if (res == -1) {
		res = p_scenario->sps->cull_convex(p_convex, p_result_array, p_result_max, p_mask);

		// Opportunity for occlusion culling on the main scene. This will be a noop if no occluders.
		if (p_scenario->_portal_renderer.occlusion_is_active()) {
			res = p_scenario->_portal_renderer.occlusion_cull(p_cam_transform, p_cam_projection, p_convex, (VSInstance **)p_result_array, res);
		}
	}
	return res;
}

void VisualServerScene::_rooms_instance_update(Instance *p_instance, const AABB &p_aabb) {
	// magic number for instances in the room / portal system, but not requiring an update
	// (due to being a STATIC or DYNAMIC object within a room)
	// Must match the value in PortalRenderer in VisualServer
	const uint32_t OCCLUSION_HANDLE_ROOM_BIT = 1 << 31;

	// if the instance is a moving object in the room / portal system, update it
	// Note that if rooms and portals is not in use, occlusion_handle should be zero in all cases unless the portal_mode
	// has been set to global or roaming. (which is unlikely as the default is static).
	// The exception is editor user interface elements.
	// These are always set to global and will always keep their aabb up to date in the portal renderer unnecessarily.
	// There is no easy way around this, but it should be very cheap, and have no impact outside the editor.
	if (p_instance->occlusion_handle && (p_instance->occlusion_handle != OCCLUSION_HANDLE_ROOM_BIT)) {
		p_instance->scenario->_portal_renderer.instance_moving_update(p_instance->occlusion_handle, p_aabb);
	}
}

void VisualServerScene::instance_geometry_set_flag(RID p_instance, VS::InstanceFlags p_flags, bool p_enabled) {
	Instance *instance = instance_owner.get(p_instance);
	ERR_FAIL_COND(!instance);

	switch (p_flags) {
		case VS::INSTANCE_FLAG_DRAW_NEXT_FRAME_IF_VISIBLE: {
			instance->redraw_if_visible = p_enabled;

		} break;
		default: {
		}
	}
}
void VisualServerScene::instance_geometry_set_cast_shadows_setting(RID p_instance, VS::ShadowCastingSetting p_shadow_casting_setting) {
	Instance *instance = instance_owner.get(p_instance);
	ERR_FAIL_COND(!instance);

	instance->cast_shadows = p_shadow_casting_setting;
	instance->base_changed(false, true); // to actually compute if shadows are visible or not
}
void VisualServerScene::instance_geometry_set_material_override(RID p_instance, RID p_material) {
	Instance *instance = instance_owner.get(p_instance);
	ERR_FAIL_COND(!instance);

	if (instance->material_override.is_valid()) {
		VSG::storage->material_remove_instance_owner(instance->material_override, instance);
	}
	instance->material_override = p_material;
	instance->base_changed(false, true);

	if (instance->material_override.is_valid()) {
		VSG::storage->material_add_instance_owner(instance->material_override, instance);
	}
}
void VisualServerScene::instance_geometry_set_material_overlay(RID p_instance, RID p_material) {
	Instance *instance = instance_owner.get(p_instance);
	ERR_FAIL_COND(!instance);

	if (instance->material_overlay.is_valid()) {
		VSG::storage->material_remove_instance_owner(instance->material_overlay, instance);
	}
	instance->material_overlay = p_material;
	instance->base_changed(false, true);

	if (instance->material_overlay.is_valid()) {
		VSG::storage->material_add_instance_owner(instance->material_overlay, instance);
	}
}

void VisualServerScene::instance_geometry_set_draw_range(RID p_instance, float p_min, float p_max, float p_min_margin, float p_max_margin) {
}
void VisualServerScene::instance_geometry_set_as_instance_lod(RID p_instance, RID p_as_lod_of_instance) {
}

void VisualServerScene::_update_instance(Instance *p_instance) {
	p_instance->version++;

	// when not using interpolation the transform is used straight
	const Transform *instance_xform = &p_instance->transform;

	// Can possibly use the most up to date current transform here when using physics interpolation ..
	// uncomment the next line for this..
	// if (p_instance->is_currently_interpolated()) {
	// instance_xform = &p_instance->transform_curr;
	// }
	// However it does seem that using the interpolated transform (transform) works for keeping AABBs
	// up to date to avoid culling errors.

	if (p_instance->base_type == VS::INSTANCE_LIGHT) {
		InstanceLightData *light = static_cast<InstanceLightData *>(p_instance->base_data);

		VSG::scene_render->light_instance_set_transform(light->instance, *instance_xform);
		light->shadow_dirty = true;
	}

	if (p_instance->base_type == VS::INSTANCE_REFLECTION_PROBE) {
		InstanceReflectionProbeData *reflection_probe = static_cast<InstanceReflectionProbeData *>(p_instance->base_data);

		VSG::scene_render->reflection_probe_instance_set_transform(reflection_probe->instance, *instance_xform);
		reflection_probe->reflection_dirty = true;
	}

	if (p_instance->aabb.has_no_surface()) {
		return;
	}

	if ((1 << p_instance->base_type) & VS::INSTANCE_GEOMETRY_MASK) {
		InstanceGeometryData *geom = static_cast<InstanceGeometryData *>(p_instance->base_data);
		//make sure lights are updated if it casts shadow

		if (geom->can_cast_shadows) {
			for (List<Instance *>::Element *E = geom->lighting.front(); E; E = E->next()) {
				InstanceLightData *light = static_cast<InstanceLightData *>(E->get()->base_data);
				light->shadow_dirty = true;
			}
		}
	}

	p_instance->mirror = instance_xform->basis.determinant() < 0.0;

	AABB new_aabb;

	new_aabb = instance_xform->xform(p_instance->aabb);

	p_instance->transformed_aabb = new_aabb;

	if (!p_instance->scenario) {
		return;
	}

	if (p_instance->spatial_partition_id == 0) {
		uint32_t base_type = 1 << p_instance->base_type;
		uint32_t pairable_mask = 0;
		bool pairable = false;

		if (p_instance->base_type == VS::INSTANCE_LIGHT || p_instance->base_type == VS::INSTANCE_REFLECTION_PROBE) {
			pairable_mask = p_instance->visible ? VS::INSTANCE_GEOMETRY_MASK : 0;
			pairable = true;
		}

		// not inside octree
		p_instance->spatial_partition_id = p_instance->scenario->sps->create(p_instance, new_aabb, 0, pairable, base_type, pairable_mask);

	} else {
		/*
		if (new_aabb==p_instance->data.transformed_aabb)
			return;
		*/

		p_instance->scenario->sps->move(p_instance->spatial_partition_id, new_aabb);
	}

	// keep rooms and portals instance up to date if present
	_rooms_instance_update(p_instance, new_aabb);
}

void VisualServerScene::_update_instance_aabb(Instance *p_instance) {
	AABB new_aabb;

	ERR_FAIL_COND(p_instance->base_type != VS::INSTANCE_NONE && !p_instance->base.is_valid());

	switch (p_instance->base_type) {
		case VisualServer::INSTANCE_NONE: {
			// do nothing
		} break;
		case VisualServer::INSTANCE_MESH: {
			if (p_instance->custom_aabb) {
				new_aabb = *p_instance->custom_aabb;
			} else {
				new_aabb = VSG::storage->mesh_get_aabb(p_instance->base, p_instance->skeleton);
			}

		} break;

		case VisualServer::INSTANCE_MULTIMESH: {
			if (p_instance->custom_aabb) {
				new_aabb = *p_instance->custom_aabb;
			} else {
				new_aabb = VSG::storage->multimesh_get_aabb(p_instance->base);
			}

		} break;
		case VisualServer::INSTANCE_IMMEDIATE: {
			if (p_instance->custom_aabb) {
				new_aabb = *p_instance->custom_aabb;
			} else {
				new_aabb = VSG::storage->immediate_get_aabb(p_instance->base);
			}

		} break;
		case VisualServer::INSTANCE_LIGHT: {
			new_aabb = VSG::storage->light_get_aabb(p_instance->base);

		} break;
		case VisualServer::INSTANCE_REFLECTION_PROBE: {
			new_aabb = VSG::storage->reflection_probe_get_aabb(p_instance->base);

		} break;
		default: {
		}
	}

	// <Zylann> This is why I didn't re-use Instance::aabb to implement custom AABBs
	if (p_instance->extra_margin) {
		new_aabb.grow_by(p_instance->extra_margin);
	}

	p_instance->aabb = new_aabb;
}

void VisualServerScene::_update_dirty_instance(Instance *p_instance) {
	if (p_instance->update_aabb) {
		_update_instance_aabb(p_instance);
	}

	if (p_instance->update_materials) {
		if (p_instance->base_type == VS::INSTANCE_MESH) {
			//remove materials no longer used and un-own them

			int new_mat_count = VSG::storage->mesh_get_surface_count(p_instance->base);
			for (int i = p_instance->materials.size() - 1; i >= new_mat_count; i--) {
				if (p_instance->materials[i].is_valid()) {
					VSG::storage->material_remove_instance_owner(p_instance->materials[i], p_instance);
				}
			}
			p_instance->materials.resize(new_mat_count);

			int new_blend_shape_count = VSG::storage->mesh_get_blend_shape_count(p_instance->base);
			if (new_blend_shape_count != p_instance->blend_values.size()) {
				p_instance->blend_values.resize(new_blend_shape_count);
				for (int i = 0; i < new_blend_shape_count; i++) {
					p_instance->blend_values.write().ptr()[i] = 0;
				}
			}
		}

		if ((1 << p_instance->base_type) & VS::INSTANCE_GEOMETRY_MASK) {
			InstanceGeometryData *geom = static_cast<InstanceGeometryData *>(p_instance->base_data);

			bool can_cast_shadows = true;
			bool is_animated = false;

			if (p_instance->cast_shadows == VS::SHADOW_CASTING_SETTING_OFF) {
				can_cast_shadows = false;
			} else if (p_instance->material_override.is_valid()) {
				can_cast_shadows = VSG::storage->material_casts_shadows(p_instance->material_override);
				is_animated = VSG::storage->material_is_animated(p_instance->material_override);
			} else {
				if (p_instance->base_type == VS::INSTANCE_MESH) {
					RID mesh = p_instance->base;

					if (mesh.is_valid()) {
						bool cast_shadows = false;

						for (int i = 0; i < p_instance->materials.size(); i++) {
							RID mat = p_instance->materials[i].is_valid() ? p_instance->materials[i] : VSG::storage->mesh_surface_get_material(mesh, i);

							if (!mat.is_valid()) {
								cast_shadows = true;
							} else {
								if (VSG::storage->material_casts_shadows(mat)) {
									cast_shadows = true;
								}

								if (VSG::storage->material_is_animated(mat)) {
									is_animated = true;
								}
							}
						}

						if (!cast_shadows) {
							can_cast_shadows = false;
						}
					}

				} else if (p_instance->base_type == VS::INSTANCE_MULTIMESH) {
					RID mesh = VSG::storage->multimesh_get_mesh(p_instance->base);
					if (mesh.is_valid()) {
						bool cast_shadows = false;

						int sc = VSG::storage->mesh_get_surface_count(mesh);
						for (int i = 0; i < sc; i++) {
							RID mat = VSG::storage->mesh_surface_get_material(mesh, i);

							if (!mat.is_valid()) {
								cast_shadows = true;

							} else {
								if (VSG::storage->material_casts_shadows(mat)) {
									cast_shadows = true;
								}
								if (VSG::storage->material_is_animated(mat)) {
									is_animated = true;
								}
							}
						}

						if (!cast_shadows) {
							can_cast_shadows = false;
						}
					}
				} else if (p_instance->base_type == VS::INSTANCE_IMMEDIATE) {
					RID mat = VSG::storage->immediate_get_material(p_instance->base);

					can_cast_shadows = !mat.is_valid() || VSG::storage->material_casts_shadows(mat);

					if (mat.is_valid() && VSG::storage->material_is_animated(mat)) {
						is_animated = true;
					}
				}
			}

			if (p_instance->material_overlay.is_valid()) {
				can_cast_shadows = can_cast_shadows || VSG::storage->material_casts_shadows(p_instance->material_overlay);
				is_animated = is_animated || VSG::storage->material_is_animated(p_instance->material_overlay);
			}

			if (can_cast_shadows != geom->can_cast_shadows) {
				//ability to cast shadows change, let lights now
				for (List<Instance *>::Element *E = geom->lighting.front(); E; E = E->next()) {
					InstanceLightData *light = static_cast<InstanceLightData *>(E->get()->base_data);
					light->shadow_dirty = true;
				}

				geom->can_cast_shadows = can_cast_shadows;
			}

			geom->material_is_animated = is_animated;
		}
	}

	_instance_update_list.remove(&p_instance->update_item);

	_update_instance(p_instance);

	p_instance->update_aabb = false;
	p_instance->update_materials = false;
}

bool VisualServerScene::_light_instance_update_shadow(Instance *p_instance, const Transform p_cam_transform, const CameraMatrix &p_cam_projection, bool p_cam_orthogonal, RID p_shadow_atlas, Scenario *p_scenario) {
	InstanceLightData *light = static_cast<InstanceLightData *>(p_instance->base_data);

	Transform light_transform = p_instance->transform;
	light_transform.orthonormalize(); //scale does not count on lights

	bool animated_material_found = false;

	switch (VSG::storage->light_get_type(p_instance->base)) {
		case VS::LIGHT_DIRECTIONAL: {
			float max_distance = p_cam_projection.get_z_far();
			float shadow_max = VSG::storage->light_get_param(p_instance->base, VS::LIGHT_PARAM_SHADOW_MAX_DISTANCE);
			if (shadow_max > 0 && !p_cam_orthogonal) { //its impractical (and leads to unwanted behaviors) to set max distance in orthogonal camera
				max_distance = MIN(shadow_max, max_distance);
			}
			max_distance = MAX(max_distance, p_cam_projection.get_z_near() + 0.001);
			float min_distance = MIN(p_cam_projection.get_z_near(), max_distance);

			VS::LightDirectionalShadowDepthRangeMode depth_range_mode = VSG::storage->light_directional_get_shadow_depth_range_mode(p_instance->base);

			if (depth_range_mode == VS::LIGHT_DIRECTIONAL_SHADOW_DEPTH_RANGE_OPTIMIZED) {
				//optimize min/max
				Vector<Plane> planes = p_cam_projection.get_projection_planes(p_cam_transform);
				int cull_count = p_scenario->sps->cull_convex(planes, instance_shadow_cull_result, MAX_INSTANCE_CULL, VS::INSTANCE_GEOMETRY_MASK);
				Plane base(p_cam_transform.origin, -p_cam_transform.basis.get_axis(2));
				//check distance max and min

				bool found_items = false;
				float z_max = -1e20;
				float z_min = 1e20;

				for (int i = 0; i < cull_count; i++) {
					Instance *instance = instance_shadow_cull_result[i];
					if (!instance->visible || !((1 << instance->base_type) & VS::INSTANCE_GEOMETRY_MASK) || !static_cast<InstanceGeometryData *>(instance->base_data)->can_cast_shadows) {
						continue;
					}

					if (static_cast<InstanceGeometryData *>(instance->base_data)->material_is_animated) {
						animated_material_found = true;
					}

					float max, min;
					instance->transformed_aabb.project_range_in_plane(base, min, max);

					if (max > z_max) {
						z_max = max;
					}

					if (min < z_min) {
						z_min = min;
					}

					found_items = true;
				}

				if (found_items) {
					min_distance = MAX(min_distance, z_min);
					max_distance = MIN(max_distance, z_max);
				}
			}

			float range = max_distance - min_distance;

			int splits = 0;
			switch (VSG::storage->light_directional_get_shadow_mode(p_instance->base)) {
				case VS::LIGHT_DIRECTIONAL_SHADOW_ORTHOGONAL:
					splits = 1;
					break;
				case VS::LIGHT_DIRECTIONAL_SHADOW_PARALLEL_2_SPLITS:
					splits = 2;
					break;
				case VS::LIGHT_DIRECTIONAL_SHADOW_PARALLEL_4_SPLITS:
					splits = 4;
					break;
			}

			float distances[5];

			distances[0] = min_distance;
			for (int i = 0; i < splits; i++) {
				distances[i + 1] = min_distance + VSG::storage->light_get_param(p_instance->base, VS::LightParam(VS::LIGHT_PARAM_SHADOW_SPLIT_1_OFFSET + i)) * range;
			};

			distances[splits] = max_distance;

			float texture_size = VSG::scene_render->get_directional_light_shadow_size(light->instance);

			bool overlap = VSG::storage->light_directional_get_blend_splits(p_instance->base);

			float first_radius = 0.0;

			for (int i = 0; i < splits; i++) {
				// setup a camera matrix for that range!
				CameraMatrix camera_matrix;

				float aspect = p_cam_projection.get_aspect();

				if (p_cam_orthogonal) {
					Vector2 vp_he = p_cam_projection.get_viewport_half_extents();

					camera_matrix.set_orthogonal(vp_he.y * 2.0, aspect, distances[(i == 0 || !overlap) ? i : i - 1], distances[i + 1], false);
				} else {
					float fov = p_cam_projection.get_fov();
					camera_matrix.set_perspective(fov, aspect, distances[(i == 0 || !overlap) ? i : i - 1], distances[i + 1], false);
				}

				//obtain the frustum endpoints

				Vector3 endpoints[8]; // frustum plane endpoints
				bool res = camera_matrix.get_endpoints(p_cam_transform, endpoints);
				ERR_CONTINUE(!res);

				// obtain the light frustm ranges (given endpoints)

				Transform transform = light_transform; //discard scale and stabilize light

				Vector3 x_vec = transform.basis.get_axis(Vector3::AXIS_X).normalized();
				Vector3 y_vec = transform.basis.get_axis(Vector3::AXIS_Y).normalized();
				Vector3 z_vec = transform.basis.get_axis(Vector3::AXIS_Z).normalized();
				//z_vec points agsint the camera, like in default opengl

				float x_min = 0.f, x_max = 0.f;
				float y_min = 0.f, y_max = 0.f;
				float z_min = 0.f, z_max = 0.f;

				// FIXME: z_max_cam is defined, computed, but not used below when setting up
				// ortho_camera. Commented out for now to fix warnings but should be investigated.
				float x_min_cam = 0.f, x_max_cam = 0.f;
				float y_min_cam = 0.f, y_max_cam = 0.f;
				float z_min_cam = 0.f;
				//float z_max_cam = 0.f;

				float bias_scale = 1.0;

				//used for culling

				for (int j = 0; j < 8; j++) {
					float d_x = x_vec.dot(endpoints[j]);
					float d_y = y_vec.dot(endpoints[j]);
					float d_z = z_vec.dot(endpoints[j]);

					if (j == 0 || d_x < x_min) {
						x_min = d_x;
					}
					if (j == 0 || d_x > x_max) {
						x_max = d_x;
					}

					if (j == 0 || d_y < y_min) {
						y_min = d_y;
					}
					if (j == 0 || d_y > y_max) {
						y_max = d_y;
					}

					if (j == 0 || d_z < z_min) {
						z_min = d_z;
					}
					if (j == 0 || d_z > z_max) {
						z_max = d_z;
					}
				}

				{
					//camera viewport stuff

					Vector3 center;

					for (int j = 0; j < 8; j++) {
						center += endpoints[j];
					}
					center /= 8.0;

					//center=x_vec*(x_max-x_min)*0.5 + y_vec*(y_max-y_min)*0.5 + z_vec*(z_max-z_min)*0.5;

					float radius = 0;

					for (int j = 0; j < 8; j++) {
						float d = center.distance_to(endpoints[j]);
						if (d > radius) {
							radius = d;
						}
					}

					radius *= texture_size / (texture_size - 2.0); //add a texel by each side

					if (i == 0) {
						first_radius = radius;
					} else {
						bias_scale = radius / first_radius;
					}

					x_max_cam = x_vec.dot(center) + radius;
					x_min_cam = x_vec.dot(center) - radius;
					y_max_cam = y_vec.dot(center) + radius;
					y_min_cam = y_vec.dot(center) - radius;
					//z_max_cam = z_vec.dot(center) + radius;
					z_min_cam = z_vec.dot(center) - radius;

					if (depth_range_mode == VS::LIGHT_DIRECTIONAL_SHADOW_DEPTH_RANGE_STABLE) {
						//this trick here is what stabilizes the shadow (make potential jaggies to not move)
						//at the cost of some wasted resolution. Still the quality increase is very well worth it

						float unit = radius * 2.0 / texture_size;

						x_max_cam = Math::stepify(x_max_cam, unit);
						x_min_cam = Math::stepify(x_min_cam, unit);
						y_max_cam = Math::stepify(y_max_cam, unit);
						y_min_cam = Math::stepify(y_min_cam, unit);
					}
				}

				//now that we now all ranges, we can proceed to make the light frustum planes, for culling octree

				Vector<Plane> light_frustum_planes;
				light_frustum_planes.resize(6);

				//right/left
				light_frustum_planes.write[0] = Plane(x_vec, x_max);
				light_frustum_planes.write[1] = Plane(-x_vec, -x_min);
				//top/bottom
				light_frustum_planes.write[2] = Plane(y_vec, y_max);
				light_frustum_planes.write[3] = Plane(-y_vec, -y_min);
				//near/far
				light_frustum_planes.write[4] = Plane(z_vec, z_max + 1e6);
				light_frustum_planes.write[5] = Plane(-z_vec, -z_min); // z_min is ok, since casters further than far-light plane are not needed

				int cull_count = p_scenario->sps->cull_convex(light_frustum_planes, instance_shadow_cull_result, MAX_INSTANCE_CULL, VS::INSTANCE_GEOMETRY_MASK);

				// a pre pass will need to be needed to determine the actual z-near to be used

				Plane near_plane(light_transform.origin, -light_transform.basis.get_axis(2));

				for (int j = 0; j < cull_count; j++) {
					float min, max;
					Instance *instance = instance_shadow_cull_result[j];
					if (!instance->visible || !((1 << instance->base_type) & VS::INSTANCE_GEOMETRY_MASK) || !static_cast<InstanceGeometryData *>(instance->base_data)->can_cast_shadows) {
						cull_count--;
						SWAP(instance_shadow_cull_result[j], instance_shadow_cull_result[cull_count]);
						j--;
						continue;
					}

					instance->transformed_aabb.project_range_in_plane(Plane(z_vec, 0), min, max);
					instance->depth = near_plane.distance_to(instance->transform.origin);
					instance->depth_layer = 0;
					if (max > z_max) {
						z_max = max;
					}
				}

				{
					CameraMatrix ortho_camera;
					real_t half_x = (x_max_cam - x_min_cam) * 0.5;
					real_t half_y = (y_max_cam - y_min_cam) * 0.5;

					ortho_camera.set_orthogonal(-half_x, half_x, -half_y, half_y, 0, (z_max - z_min_cam));

					Transform ortho_transform;
					ortho_transform.basis = transform.basis;
					ortho_transform.origin = x_vec * (x_min_cam + half_x) + y_vec * (y_min_cam + half_y) + z_vec * z_max;

					VSG::scene_render->light_instance_set_shadow_transform(light->instance, ortho_camera, ortho_transform, 0, distances[i + 1], i, bias_scale);
				}

				VSG::scene_render->render_shadow(light->instance, p_shadow_atlas, i, (RasterizerScene::InstanceBase **)instance_shadow_cull_result, cull_count);
			}

		} break;
		case VS::LIGHT_OMNI: {
			VS::LightOmniShadowMode shadow_mode = VSG::storage->light_omni_get_shadow_mode(p_instance->base);

			if (shadow_mode == VS::LIGHT_OMNI_SHADOW_DUAL_PARABOLOID || !VSG::scene_render->light_instances_can_render_shadow_cube()) {
				for (int i = 0; i < 2; i++) {
					//using this one ensures that raster deferred will have it

					float radius = VSG::storage->light_get_param(p_instance->base, VS::LIGHT_PARAM_RANGE);

					float z = i == 0 ? -1 : 1;
					Vector<Plane> planes;
					planes.resize(6);
					planes.write[0] = light_transform.xform(Plane(Vector3(0, 0, z), radius));
					planes.write[1] = light_transform.xform(Plane(Vector3(1, 0, z).normalized(), radius));
					planes.write[2] = light_transform.xform(Plane(Vector3(-1, 0, z).normalized(), radius));
					planes.write[3] = light_transform.xform(Plane(Vector3(0, 1, z).normalized(), radius));
					planes.write[4] = light_transform.xform(Plane(Vector3(0, -1, z).normalized(), radius));
					planes.write[5] = light_transform.xform(Plane(Vector3(0, 0, -z), 0));

					int cull_count = p_scenario->sps->cull_convex(planes, instance_shadow_cull_result, MAX_INSTANCE_CULL, VS::INSTANCE_GEOMETRY_MASK);
					Plane near_plane(light_transform.origin, light_transform.basis.get_axis(2) * z);

					for (int j = 0; j < cull_count; j++) {
						Instance *instance = instance_shadow_cull_result[j];
						if (!instance->visible || !((1 << instance->base_type) & VS::INSTANCE_GEOMETRY_MASK) || !static_cast<InstanceGeometryData *>(instance->base_data)->can_cast_shadows) {
							cull_count--;
							SWAP(instance_shadow_cull_result[j], instance_shadow_cull_result[cull_count]);
							j--;
						} else {
							if (static_cast<InstanceGeometryData *>(instance->base_data)->material_is_animated) {
								animated_material_found = true;
							}

							instance->depth = near_plane.distance_to(instance->transform.origin);
							instance->depth_layer = 0;
						}
					}

					VSG::scene_render->light_instance_set_shadow_transform(light->instance, CameraMatrix(), light_transform, radius, 0, i);
					VSG::scene_render->render_shadow(light->instance, p_shadow_atlas, i, (RasterizerScene::InstanceBase **)instance_shadow_cull_result, cull_count);
				}
			} else { //shadow cube

				float radius = VSG::storage->light_get_param(p_instance->base, VS::LIGHT_PARAM_RANGE);
				CameraMatrix cm;
				cm.set_perspective(90, 1, 0.01, radius);

				for (int i = 0; i < 6; i++) {
					//using this one ensures that raster deferred will have it

					static const Vector3 view_normals[6] = {
						Vector3(-1, 0, 0),
						Vector3(+1, 0, 0),
						Vector3(0, -1, 0),
						Vector3(0, +1, 0),
						Vector3(0, 0, -1),
						Vector3(0, 0, +1)
					};
					static const Vector3 view_up[6] = {
						Vector3(0, -1, 0),
						Vector3(0, -1, 0),
						Vector3(0, 0, -1),
						Vector3(0, 0, +1),
						Vector3(0, -1, 0),
						Vector3(0, -1, 0)
					};

					Transform xform = light_transform * Transform().looking_at(view_normals[i], view_up[i]);

					Vector<Plane> planes = cm.get_projection_planes(xform);

					int cull_count = _cull_convex_from_point(p_scenario, light_transform, cm, planes, instance_shadow_cull_result, MAX_INSTANCE_CULL, light->previous_room_id_hint, VS::INSTANCE_GEOMETRY_MASK);

					Plane near_plane(xform.origin, -xform.basis.get_axis(2));
					for (int j = 0; j < cull_count; j++) {
						Instance *instance = instance_shadow_cull_result[j];
						if (!instance->visible || !((1 << instance->base_type) & VS::INSTANCE_GEOMETRY_MASK) || !static_cast<InstanceGeometryData *>(instance->base_data)->can_cast_shadows) {
							cull_count--;
							SWAP(instance_shadow_cull_result[j], instance_shadow_cull_result[cull_count]);
							j--;
						} else {
							if (static_cast<InstanceGeometryData *>(instance->base_data)->material_is_animated) {
								animated_material_found = true;
							}
							instance->depth = near_plane.distance_to(instance->transform.origin);
							instance->depth_layer = 0;
						}
					}

					VSG::scene_render->light_instance_set_shadow_transform(light->instance, cm, xform, radius, 0, i);
					VSG::scene_render->render_shadow(light->instance, p_shadow_atlas, i, (RasterizerScene::InstanceBase **)instance_shadow_cull_result, cull_count);
				}

				//restore the regular DP matrix
				VSG::scene_render->light_instance_set_shadow_transform(light->instance, CameraMatrix(), light_transform, radius, 0, 0);
			}

		} break;
		case VS::LIGHT_SPOT: {
			float radius = VSG::storage->light_get_param(p_instance->base, VS::LIGHT_PARAM_RANGE);
			float angle = VSG::storage->light_get_param(p_instance->base, VS::LIGHT_PARAM_SPOT_ANGLE);

			CameraMatrix cm;
			cm.set_perspective(angle * 2.0, 1.0, 0.01, radius);

			Vector<Plane> planes = cm.get_projection_planes(light_transform);
			int cull_count = _cull_convex_from_point(p_scenario, light_transform, cm, planes, instance_shadow_cull_result, MAX_INSTANCE_CULL, light->previous_room_id_hint, VS::INSTANCE_GEOMETRY_MASK);

			Plane near_plane(light_transform.origin, -light_transform.basis.get_axis(2));
			for (int j = 0; j < cull_count; j++) {
				Instance *instance = instance_shadow_cull_result[j];
				if (!instance->visible || !((1 << instance->base_type) & VS::INSTANCE_GEOMETRY_MASK) || !static_cast<InstanceGeometryData *>(instance->base_data)->can_cast_shadows) {
					cull_count--;
					SWAP(instance_shadow_cull_result[j], instance_shadow_cull_result[cull_count]);
					j--;
				} else {
					if (static_cast<InstanceGeometryData *>(instance->base_data)->material_is_animated) {
						animated_material_found = true;
					}
					instance->depth = near_plane.distance_to(instance->transform.origin);
					instance->depth_layer = 0;
				}
			}

			VSG::scene_render->light_instance_set_shadow_transform(light->instance, cm, light_transform, radius, 0, 0);
			VSG::scene_render->render_shadow(light->instance, p_shadow_atlas, 0, (RasterizerScene::InstanceBase **)instance_shadow_cull_result, cull_count);

		} break;
	}

	return animated_material_found;
}

void VisualServerScene::render_camera(RID p_camera, RID p_scenario, Size2 p_viewport_size, RID p_shadow_atlas) {
// render to mono camera
#ifndef _3D_DISABLED

	Camera *camera = camera_owner.getornull(p_camera);
	ERR_FAIL_COND(!camera);

	/* STEP 1 - SETUP CAMERA */
	CameraMatrix camera_matrix;
	bool ortho = false;

	switch (camera->type) {
		case Camera::ORTHOGONAL: {
			camera_matrix.set_orthogonal(
					camera->size,
					p_viewport_size.width / (float)p_viewport_size.height,
					camera->znear,
					camera->zfar,
					camera->vaspect);
			ortho = true;
		} break;
		case Camera::PERSPECTIVE: {
			camera_matrix.set_perspective(
					camera->fov,
					p_viewport_size.width / (float)p_viewport_size.height,
					camera->znear,
					camera->zfar,
					camera->vaspect);
			ortho = false;

		} break;
		case Camera::FRUSTUM: {
			camera_matrix.set_frustum(
					camera->size,
					p_viewport_size.width / (float)p_viewport_size.height,
					camera->offset,
					camera->znear,
					camera->zfar,
					camera->vaspect);
			ortho = false;
		} break;
	}

	// This getter allows optional fixed timestep interpolation for the camera.
	Transform camera_transform = camera->get_transform();

	_prepare_scene(camera_transform, camera_matrix, ortho, camera->env, camera->visible_layers, p_scenario, p_shadow_atlas, RID(), camera->previous_room_id_hint);
	_render_scene(camera_transform, camera_matrix, 0, ortho, camera->env, p_scenario, p_shadow_atlas, RID(), -1);
#endif
}

void VisualServerScene::_prepare_scene(const Transform p_cam_transform, const CameraMatrix &p_cam_projection, bool p_cam_orthogonal, RID p_force_environment, uint32_t p_visible_layers, RID p_scenario, RID p_shadow_atlas, RID p_reflection_probe, int32_t &r_previous_room_id_hint) {
	// Note, in stereo rendering:
	// - p_cam_transform will be a transform in the middle of our two eyes
	// - p_cam_projection is a wider frustrum that encompasses both eyes

	Scenario *scenario = scenario_owner.getornull(p_scenario);

	render_pass++;
	uint32_t camera_layer_mask = p_visible_layers;

	VSG::scene_render->set_scene_pass(render_pass);

	//rasterizer->set_camera(camera->transform, camera_matrix,ortho);

	Vector<Plane> planes = p_cam_projection.get_projection_planes(p_cam_transform);

	Plane near_plane(p_cam_transform.origin, -p_cam_transform.basis.get_axis(2).normalized());
	float z_far = p_cam_projection.get_z_far();

	/* STEP 2 - CULL */
	instance_cull_count = _cull_convex_from_point(scenario, p_cam_transform, p_cam_projection, planes, instance_cull_result, MAX_INSTANCE_CULL, r_previous_room_id_hint);
	light_cull_count = 0;

	reflection_probe_cull_count = 0;

	//light_samplers_culled=0;

	/*
	print_line("OT: "+rtos( (OS::get_singleton()->get_ticks_usec()-t)/1000.0));
	print_line("OTO: "+itos(p_scenario->octree.get_octant_count()));
	print_line("OTE: "+itos(p_scenario->octree.get_elem_count()));
	print_line("OTP: "+itos(p_scenario->octree.get_pair_count()));
	*/

	/* STEP 3 - PROCESS PORTALS, VALIDATE ROOMS */
	//removed, will replace with culling

	/* STEP 4 - REMOVE FURTHER CULLED OBJECTS, ADD LIGHTS */

	for (int i = 0; i < instance_cull_count; i++) {
		Instance *ins = instance_cull_result[i];

		bool keep = false;

		if ((camera_layer_mask & ins->layer_mask) == 0) {
			//failure
		} else if (ins->base_type == VS::INSTANCE_LIGHT && ins->visible) {
			if (light_cull_count < MAX_LIGHTS_CULLED) {
				InstanceLightData *light = static_cast<InstanceLightData *>(ins->base_data);

				if (!light->geometries.empty()) {
					//do not add this light if no geometry is affected by it..
					light_cull_result[light_cull_count] = ins;
					light_instance_cull_result[light_cull_count] = light->instance;
					if (p_shadow_atlas.is_valid() && VSG::storage->light_has_shadow(ins->base)) {
						VSG::scene_render->light_instance_mark_visible(light->instance); //mark it visible for shadow allocation later
					}

					light_cull_count++;
				}
			}
		} else if (ins->base_type == VS::INSTANCE_REFLECTION_PROBE && ins->visible) {
			if (reflection_probe_cull_count < MAX_REFLECTION_PROBES_CULLED) {
				InstanceReflectionProbeData *reflection_probe = static_cast<InstanceReflectionProbeData *>(ins->base_data);

				if (p_reflection_probe != reflection_probe->instance) {
					//avoid entering The Matrix

					if (!reflection_probe->geometries.empty()) {
						//do not add this light if no geometry is affected by it..

						if (reflection_probe->reflection_dirty || VSG::scene_render->reflection_probe_instance_needs_redraw(reflection_probe->instance)) {
							if (!reflection_probe->update_list.in_list()) {
								reflection_probe->render_step = 0;
								reflection_probe_render_list.add_last(&reflection_probe->update_list);
							}

							reflection_probe->reflection_dirty = false;
						}

						if (VSG::scene_render->reflection_probe_instance_has_reflection(reflection_probe->instance)) {
							reflection_probe_instance_cull_result[reflection_probe_cull_count] = reflection_probe->instance;
							reflection_probe_cull_count++;
						}
					}
				}
			}

		} else if (((1 << ins->base_type) & VS::INSTANCE_GEOMETRY_MASK) && ins->visible && ins->cast_shadows != VS::SHADOW_CASTING_SETTING_SHADOWS_ONLY) {
			keep = true;

			InstanceGeometryData *geom = static_cast<InstanceGeometryData *>(ins->base_data);

			if (ins->redraw_if_visible) {
				VisualServerRaster::redraw_request(false);
			}

			if (geom->lighting_dirty) {
				int l = 0;
				//only called when lights AABB enter/exit this geometry
				ins->light_instances.resize(geom->lighting.size());

				for (List<Instance *>::Element *E = geom->lighting.front(); E; E = E->next()) {
					InstanceLightData *light = static_cast<InstanceLightData *>(E->get()->base_data);

					ins->light_instances.write[l++] = light->instance;
				}

				geom->lighting_dirty = false;
			}

			if (geom->reflection_dirty) {
				int l = 0;
				//only called when reflection probe AABB enter/exit this geometry
				ins->reflection_probe_instances.resize(geom->reflection_probes.size());

				for (List<Instance *>::Element *E = geom->reflection_probes.front(); E; E = E->next()) {
					InstanceReflectionProbeData *reflection_probe = static_cast<InstanceReflectionProbeData *>(E->get()->base_data);

					ins->reflection_probe_instances.write[l++] = reflection_probe->instance;
				}

				geom->reflection_dirty = false;
			}
		}

		if (!keep) {
			// remove, no reason to keep
			instance_cull_count--;
			SWAP(instance_cull_result[i], instance_cull_result[instance_cull_count]);
			i--;
			ins->last_render_pass = 0; // make invalid
		} else {
			ins->last_render_pass = render_pass;
		}
	}

	/* STEP 5 - PROCESS LIGHTS */

	RID *directional_light_ptr = &light_instance_cull_result[light_cull_count];
	directional_light_count = 0;

	// directional lights
	{
		Instance **lights_with_shadow = (Instance **)alloca(sizeof(Instance *) * scenario->directional_lights.size());
		int directional_shadow_count = 0;

		for (List<Instance *>::Element *E = scenario->directional_lights.front(); E; E = E->next()) {
			if (light_cull_count + directional_light_count >= MAX_LIGHTS_CULLED) {
				break;
			}

			if (!E->get()->visible) {
				continue;
			}

			InstanceLightData *light = static_cast<InstanceLightData *>(E->get()->base_data);

			//check shadow..

			if (light) {
				if (p_shadow_atlas.is_valid() && VSG::storage->light_has_shadow(E->get()->base)) {
					lights_with_shadow[directional_shadow_count++] = E->get();
				}
				//add to list
				directional_light_ptr[directional_light_count++] = light->instance;
			}
		}

		VSG::scene_render->set_directional_shadow_count(directional_shadow_count);

		for (int i = 0; i < directional_shadow_count; i++) {
			_light_instance_update_shadow(lights_with_shadow[i], p_cam_transform, p_cam_projection, p_cam_orthogonal, p_shadow_atlas, scenario);
		}
	}

	{ //setup shadow maps

		//SortArray<Instance*,_InstanceLightsort> sorter;
		//sorter.sort(light_cull_result,light_cull_count);
		for (int i = 0; i < light_cull_count; i++) {
			Instance *ins = light_cull_result[i];

			if (!p_shadow_atlas.is_valid() || !VSG::storage->light_has_shadow(ins->base)) {
				continue;
			}

			InstanceLightData *light = static_cast<InstanceLightData *>(ins->base_data);

			float coverage = 0.f;

			{ //compute coverage

				Transform cam_xf = p_cam_transform;
				float zn = p_cam_projection.get_z_near();
				Plane p(cam_xf.origin + cam_xf.basis.get_axis(2) * -zn, -cam_xf.basis.get_axis(2)); //camera near plane

				// near plane half width and height
				Vector2 vp_half_extents = p_cam_projection.get_viewport_half_extents();

				switch (VSG::storage->light_get_type(ins->base)) {
					case VS::LIGHT_OMNI: {
						float radius = VSG::storage->light_get_param(ins->base, VS::LIGHT_PARAM_RANGE);

						//get two points parallel to near plane
						Vector3 points[2] = {
							ins->transform.origin,
							ins->transform.origin + cam_xf.basis.get_axis(0) * radius
						};

						if (!p_cam_orthogonal) {
							//if using perspetive, map them to near plane
							for (int j = 0; j < 2; j++) {
								if (p.distance_to(points[j]) < 0) {
									points[j].z = -zn; //small hack to keep size constant when hitting the screen
								}

								p.intersects_segment(cam_xf.origin, points[j], &points[j]); //map to plane
							}
						}

						float screen_diameter = points[0].distance_to(points[1]) * 2;
						coverage = screen_diameter / (vp_half_extents.x + vp_half_extents.y);
					} break;
					case VS::LIGHT_SPOT: {
						float radius = VSG::storage->light_get_param(ins->base, VS::LIGHT_PARAM_RANGE);
						float angle = VSG::storage->light_get_param(ins->base, VS::LIGHT_PARAM_SPOT_ANGLE);

						float w = radius * Math::sin(Math::deg2rad(angle));
						float d = radius * Math::cos(Math::deg2rad(angle));

						Vector3 base = ins->transform.origin - ins->transform.basis.get_axis(2).normalized() * d;

						Vector3 points[2] = {
							base,
							base + cam_xf.basis.get_axis(0) * w
						};

						if (!p_cam_orthogonal) {
							//if using perspetive, map them to near plane
							for (int j = 0; j < 2; j++) {
								if (p.distance_to(points[j]) < 0) {
									points[j].z = -zn; //small hack to keep size constant when hitting the screen
								}

								p.intersects_segment(cam_xf.origin, points[j], &points[j]); //map to plane
							}
						}

						float screen_diameter = points[0].distance_to(points[1]) * 2;
						coverage = screen_diameter / (vp_half_extents.x + vp_half_extents.y);

					} break;
					default: {
						ERR_PRINT("Invalid Light Type");
					}
				}
			}

			if (light->shadow_dirty) {
				light->last_version++;
				light->shadow_dirty = false;
			}

			bool redraw = VSG::scene_render->shadow_atlas_update_light(p_shadow_atlas, light->instance, coverage, light->last_version);

			if (redraw) {
				//must redraw!
				light->shadow_dirty = _light_instance_update_shadow(ins, p_cam_transform, p_cam_projection, p_cam_orthogonal, p_shadow_atlas, scenario);
			}
		}
	}

	// Calculate instance->depth from the camera, after shadow calculation has stopped overwriting instance->depth
	for (int i = 0; i < instance_cull_count; i++) {
		Instance *ins = instance_cull_result[i];

		if (((1 << ins->base_type) & VS::INSTANCE_GEOMETRY_MASK) && ins->visible && ins->cast_shadows != VS::SHADOW_CASTING_SETTING_SHADOWS_ONLY) {
			Vector3 aabb_center = ins->transformed_aabb.position + (ins->transformed_aabb.size * 0.5);
			if (p_cam_orthogonal) {
				ins->depth = near_plane.distance_to(aabb_center);
			} else {
				ins->depth = p_cam_transform.origin.distance_to(aabb_center);
			}
			ins->depth_layer = CLAMP(int(ins->depth * 16 / z_far), 0, 15);
		}
	}
}

void VisualServerScene::_render_scene(const Transform p_cam_transform, const CameraMatrix &p_cam_projection, const int p_eye, bool p_cam_orthogonal, RID p_force_environment, RID p_scenario, RID p_shadow_atlas, RID p_reflection_probe, int p_reflection_probe_pass) {
	Scenario *scenario = scenario_owner.getornull(p_scenario);

	/* ENVIRONMENT */

	RID environment;
	if (p_force_environment.is_valid()) { //camera has more environment priority
		environment = p_force_environment;
	} else if (scenario->environment.is_valid()) {
		environment = scenario->environment;
	} else {
		environment = scenario->fallback_environment;
	}

	/* PROCESS GEOMETRY AND DRAW SCENE */

	VSG::scene_render->render_scene(p_cam_transform, p_cam_projection, p_eye, p_cam_orthogonal, (RasterizerScene::InstanceBase **)instance_cull_result, instance_cull_count, light_instance_cull_result, light_cull_count + directional_light_count, reflection_probe_instance_cull_result, reflection_probe_cull_count, environment, p_shadow_atlas, scenario->reflection_atlas, p_reflection_probe, p_reflection_probe_pass);
}

void VisualServerScene::render_empty_scene(RID p_scenario, RID p_shadow_atlas) {
#ifndef _3D_DISABLED

	Scenario *scenario = scenario_owner.getornull(p_scenario);

	RID environment;
	if (scenario->environment.is_valid()) {
		environment = scenario->environment;
	} else {
		environment = scenario->fallback_environment;
	}
	VSG::scene_render->render_scene(Transform(), CameraMatrix(), 0, true, nullptr, 0, nullptr, 0, nullptr, 0, environment, p_shadow_atlas, scenario->reflection_atlas, RID(), 0);
#endif
}

void VisualServerScene::update_dirty_instances() {
	VSG::storage->update_dirty_resources();

	// this is just to get access to scenario so we can update the spatial partitioning scheme
	Scenario *scenario = nullptr;
	if (_instance_update_list.first()) {
		scenario = _instance_update_list.first()->self()->scenario;
	}

	while (_instance_update_list.first()) {
		_update_dirty_instance(_instance_update_list.first()->self());
	}

	if (scenario) {
		scenario->sps->update();
	}
}

bool VisualServerScene::free(RID p_rid) {
	if (camera_owner.owns(p_rid)) {
		Camera *camera = camera_owner.get(p_rid);

		if (camera->scenario) {
			camera->scenario->_interpolation_data.notify_free_camera(p_rid, *camera);
		}

		camera_owner.free(p_rid);
		memdelete(camera);
	} else if (scenario_owner.owns(p_rid)) {
		Scenario *scenario = scenario_owner.get(p_rid);

		while (scenario->instances.first()) {
			instance_set_scenario(scenario->instances.first()->self()->self, RID());
		}
		VSG::scene_render->free(scenario->reflection_probe_shadow_atlas);
		VSG::scene_render->free(scenario->reflection_atlas);
		scenario_owner.free(p_rid);
		memdelete(scenario);

	} else if (instance_owner.owns(p_rid)) {
		// delete the instance

		update_dirty_instances();

		Instance *instance = instance_owner.get(p_rid);

		if (instance->scenario) {
			instance->scenario->_interpolation_data.notify_free_instance(p_rid, *instance);
		} else {
			if (instance->on_interpolate_list || instance->on_interpolate_transform_list) {
				// These flags should be set to false when removing the scenario.
				WARN_PRINT_ONCE("Instance delete without scenario and on interpolate lists.");
			}
		}

		instance_set_scenario(p_rid, RID());
		instance_set_base(p_rid, RID());
		instance_geometry_set_material_override(p_rid, RID());
		instance_geometry_set_material_overlay(p_rid, RID());
		instance_attach_skeleton(p_rid, RID());

		update_dirty_instances(); //in case something changed this

		instance_owner.free(p_rid);
		memdelete(instance);

	} else if (room_owner.owns(p_rid)) {
		Room *room = room_owner.get(p_rid);
		room_owner.free(p_rid);
		memdelete(room);
	} else if (portal_owner.owns(p_rid)) {
		Portal *portal = portal_owner.get(p_rid);
		portal_owner.free(p_rid);
		memdelete(portal);
	} else if (ghost_owner.owns(p_rid)) {
		Ghost *ghost = ghost_owner.get(p_rid);
		ghost_owner.free(p_rid);
		memdelete(ghost);
	} else if (roomgroup_owner.owns(p_rid)) {
		RoomGroup *roomgroup = roomgroup_owner.get(p_rid);
		roomgroup_owner.free(p_rid);
		memdelete(roomgroup);
	} else if (occluder_instance_owner.owns(p_rid)) {
		OccluderInstance *occ_inst = occluder_instance_owner.get(p_rid);
		occluder_instance_owner.free(p_rid);
		memdelete(occ_inst);
	} else if (occluder_resource_owner.owns(p_rid)) {
		OccluderResource *occ_res = occluder_resource_owner.get(p_rid);
		occ_res->destroy(_portal_resources);
		occluder_resource_owner.free(p_rid);
		memdelete(occ_res);
	} else {
		return false;
	}

	return true;
}

VisualServerScene *VisualServerScene::singleton = nullptr;

VisualServerScene::VisualServerScene() {
	render_pass = 1;
	singleton = this;
	_use_bvh = GLOBAL_DEF("rendering/quality/spatial_partitioning/use_bvh", true);
	GLOBAL_DEF("rendering/quality/spatial_partitioning/bvh_collision_margin", 0.1);
	ProjectSettings::get_singleton()->set_custom_property_info("rendering/quality/spatial_partitioning/bvh_collision_margin", PropertyInfo(Variant::REAL, "rendering/quality/spatial_partitioning/bvh_collision_margin", PROPERTY_HINT_RANGE, "0.0,2.0,0.01"));

	_visual_server_callbacks = nullptr;
}

VisualServerScene::~VisualServerScene() {
}
