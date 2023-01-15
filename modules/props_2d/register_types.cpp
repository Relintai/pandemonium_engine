/*
Copyright (c) 2019-2023 Péter Magyar

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

#include "register_types.h"

#include "core/config/engine.h"

#include "tiled_wall/tiled_wall_2d.h"
#include "tiled_wall/tiled_wall_2d_data.h"

#include "props/prop_2d_data.h"
#include "props/prop_2d_data_entry.h"
#include "props/prop_2d_data_light.h"
#include "props/prop_2d_data_prop.h"
#include "props/prop_2d_data_scene.h"
#include "props/prop_2d_data_sprite.h"
#include "props/prop_2d_data_tiled_wall_2d.h"

#include "props/prop_2d_data_portal.h"

#include "clutter/ground_clutter_2d.h"
#include "clutter/ground_clutter_2d_foliage.h"

#include "prop_2d_ess_entity.h"
#include "prop_2d_instance.h"
#include "prop_2d_instance_merger.h"

#include "prop_2d_instance_job.h"
#include "prop_2d_instance_prop_job.h"

#include "jobs/prop_2d_texture_job.h"

#include "prop_2d_scene_instance.h"

#include "singleton/prop_2d_cache.h"
#include "singleton/prop_2d_utils.h"

#include "lights/prop_2d_light.h"

#include "./editor/prop_2d_editor_plugin.h"

#include "prop_2d_mesher.h"

#include "material_cache/prop_2d_material_cache.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_TEXTURE_PACKER_ENABLED
#include "material_cache/prop_2d_material_cache_pcm.h"
#endif

static Prop2DUtils *prop_utils = NULL;
static Prop2DCache *prop_texture_cache = NULL;

void register_props_2d_types(ModuleRegistrationLevel p_level) {
	if (p_level == MODULE_REGISTRATION_LEVEL_SINGLETON) {
		prop_utils = memnew(Prop2DUtils);
		ClassDB::register_class<Prop2DUtils>();
		Engine::get_singleton()->add_singleton(Engine::Singleton("Prop2DUtils", Prop2DUtils::get_singleton()));

		prop_texture_cache = memnew(Prop2DCache);
		ClassDB::register_class<Prop2DCache>();
		Engine::get_singleton()->add_singleton(Engine::Singleton("Prop2DCache", Prop2DCache::get_singleton()));
	}

	if (p_level == MODULE_REGISTRATION_LEVEL_SCENE) {
		ClassDB::register_class<TiledWall2D>();
		ClassDB::register_class<TiledWall2DData>();

		ClassDB::register_class<Prop2DLight>();

		ClassDB::register_class<Prop2DData>();
		ClassDB::register_class<Prop2DDataEntry>();
		ClassDB::register_class<Prop2DDataScene>();
		ClassDB::register_class<Prop2DDataLight>();
		ClassDB::register_class<Prop2DDataProp2D>();
		ClassDB::register_class<Prop2DDataTiledWall2D>();
		ClassDB::register_class<Prop2DDataSprite>();

		ClassDB::register_class<Prop2DDataPortal>();

		ClassDB::register_class<GroundClutter2D>();
		ClassDB::register_class<GroundClutter2DFoliage>();

		ClassDB::register_class<Prop2DMesher>();

		ClassDB::register_class<Prop2DInstance>();
		ClassDB::register_class<Prop2DInstanceMerger>();

		ClassDB::register_class<Prop2DESSEntity>();

		ClassDB::register_class<Prop2DInstanceJob>();
		ClassDB::register_class<Prop2DInstanceProp2DJob>();

		ClassDB::register_class<Prop2DTextureJob>();

		ClassDB::register_class<Prop2DSceneInstance>();

		ClassDB::register_class<Prop2DMaterialCache>();

#ifdef MODULE_TEXTURE_PACKER_ENABLED
		ClassDB::register_class<Prop2DMaterialCachePCM>();
#endif

		Ref<Prop2DDataLight> light_processor = Ref<Prop2DDataLight>(memnew(Prop2DDataLight));
		Prop2DUtils::add_processor(light_processor);

		Ref<Prop2DDataProp2D> prop_processor = Ref<Prop2DDataProp2D>(memnew(Prop2DDataProp2D));
		Prop2DUtils::add_processor(prop_processor);

		Ref<Prop2DDataScene> scene_processor = Ref<Prop2DDataScene>(memnew(Prop2DDataScene));
		Prop2DUtils::add_processor(scene_processor);

		Ref<Prop2DDataPortal> portal_processor = Ref<Prop2DDataPortal>(memnew(Prop2DDataPortal));
		Prop2DUtils::add_processor(portal_processor);

		Ref<Prop2DDataTiledWall2D> tiled_wall_processor = Ref<Prop2DDataTiledWall2D>(memnew(Prop2DDataTiledWall2D));
		Prop2DUtils::add_processor(tiled_wall_processor);

		Ref<Prop2DDataSprite> sprite_processor = Ref<Prop2DDataSprite>(memnew(Prop2DDataSprite));
		Prop2DUtils::add_processor(sprite_processor);
	}

#ifdef TOOLS_ENABLED
	if (p_level == MODULE_REGISTRATION_LEVEL_EDITOR) {
		EditorPlugins::add_by_type<Prop2DEditorPlugin>();
	}
#endif
}

void unregister_props_2d_types(ModuleRegistrationLevel p_level) {
	if (p_level == MODULE_REGISTRATION_LEVEL_SINGLETON) {
		if (prop_utils) {
			memdelete(prop_utils);
		}

		if (prop_texture_cache) {
			memdelete(prop_texture_cache);
		}
	}
}
