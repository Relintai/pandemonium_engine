/*************************************************************************/
/*  register_types.cpp                                                   */
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

#include "register_types.h"

#include "core/config/engine.h"

#include "tiled_wall/tiled_wall.h"
#include "tiled_wall/tiled_wall_data.h"

#include "props/prop_data.h"
#include "props/prop_data_collision_object.h"
#include "props/prop_data_entry.h"
#include "props/prop_data_light.h"
#include "props/prop_data_prop.h"
#include "props/prop_data_scene.h"
#include "props/prop_data_static_body.h"
#include "props/prop_data_tiled_wall.h"

#include "props/prop_data_portal.h"

#include "clutter/ground_clutter.h"
#include "clutter/ground_clutter_foliage.h"

#include "prop_ess_entity.h"
#include "prop_instance.h"
#include "prop_instance_merger.h"

#include "prop_instance_job.h"
#include "prop_instance_prop_job.h"

#include "jobs/prop_mesher_job_step.h"
#include "jobs/prop_texture_job.h"

#include "prop_scene_instance.h"

#include "singleton/prop_cache.h"
#include "singleton/prop_utils.h"

#include "lights/prop_light.h"

#include "./editor/prop_editor_plugin.h"

#include "prop_mesher.h"

#include "material_cache/prop_material_cache.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_TEXTURE_PACKER_ENABLED
#include "material_cache/prop_material_cache_pcm.h"
#endif

static PropUtils *prop_utils = NULL;
static PropCache *prop_texture_cache = NULL;

void register_props_types(ModuleRegistrationLevel p_level) {
	if (p_level == MODULE_REGISTRATION_LEVEL_SINGLETON) {
		prop_utils = memnew(PropUtils);
		ClassDB::register_class<PropUtils>();
		Engine::get_singleton()->add_singleton(Engine::Singleton("PropUtils", PropUtils::get_singleton()));

		prop_texture_cache = memnew(PropCache);
		ClassDB::register_class<PropCache>();
		Engine::get_singleton()->add_singleton(Engine::Singleton("PropCache", PropCache::get_singleton()));
	}

	if (p_level == MODULE_REGISTRATION_LEVEL_SCENE) {
		ClassDB::register_class<TiledWall>();
		ClassDB::register_class<TiledWallData>();

		ClassDB::register_class<PropLight>();

		ClassDB::register_class<PropData>();
		ClassDB::register_class<PropDataEntry>();
		ClassDB::register_class<PropDataScene>();
		ClassDB::register_class<PropDataLight>();
		ClassDB::register_class<PropDataProp>();
		ClassDB::register_class<PropDataTiledWall>();
		ClassDB::register_class<PropDataCollisionObject>();
		ClassDB::register_class<PropDataStaticBody>();

		ClassDB::register_class<PropDataPortal>();

		ClassDB::register_class<GroundClutter>();
		ClassDB::register_class<GroundClutterFoliage>();

		ClassDB::register_class<PropMesher>();
		ClassDB::register_class<PropMesherJobStep>();

		ClassDB::register_class<PropInstance>();
		ClassDB::register_class<PropInstanceMerger>();

		ClassDB::register_class<PropESSEntity>();

		ClassDB::register_class<PropInstanceJob>();
		ClassDB::register_class<PropInstancePropJob>();

		ClassDB::register_class<PropTextureJob>();

		ClassDB::register_class<PropSceneInstance>();

		ClassDB::register_class<PropMaterialCache>();

#ifdef MODULE_TEXTURE_PACKER_ENABLED
		ClassDB::register_class<PropMaterialCachePCM>();
#endif

		Ref<PropDataLight> light_processor = Ref<PropDataLight>(memnew(PropDataLight));
		PropUtils::add_processor(light_processor);

		Ref<PropDataProp> prop_processor = Ref<PropDataProp>(memnew(PropDataProp));
		PropUtils::add_processor(prop_processor);

		Ref<PropDataScene> scene_processor = Ref<PropDataScene>(memnew(PropDataScene));
		PropUtils::add_processor(scene_processor);

		Ref<PropDataPortal> portal_processor = Ref<PropDataPortal>(memnew(PropDataPortal));
		PropUtils::add_processor(portal_processor);

		Ref<PropDataTiledWall> tiled_wall_processor = Ref<PropDataTiledWall>(memnew(PropDataTiledWall));
		PropUtils::add_processor(tiled_wall_processor);

		Ref<PropDataStaticBody> static_body_processor = Ref<PropDataStaticBody>(memnew(PropDataStaticBody));
		PropUtils::add_processor(static_body_processor);
	}

#ifdef TOOLS_ENABLED
	if (p_level == MODULE_REGISTRATION_LEVEL_EDITOR) {
		EditorPlugins::add_by_type<PropEditorPlugin>();
	}
#endif
}

void unregister_props_types(ModuleRegistrationLevel p_level) {
	if (p_level == MODULE_REGISTRATION_LEVEL_SINGLETON) {
		if (prop_utils) {
			memdelete(prop_utils);
		}

		if (prop_texture_cache) {
			memdelete(prop_texture_cache);
		}
	}
}
