

#include "register_types.h"

#include "library/terrain_2d_surface.h"
#include "library/terrain_2d_surface_simple.h"

#include "library/terrain_2d_library.h"
#include "library/terrain_2d_library_simple.h"
#include "library/terrain_2d_material_cache.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_TEXTURE_PACKER_ENABLED
#include "library/terrain_2d_library_merger.h"
#include "library/terrain_2d_library_merger_pcm.h"
#include "library/terrain_2d_material_cache_pcm.h"
#include "library/terrain_2d_surface_merger.h"
#endif

#include "data/terrain_2d_light.h"
#include "meshers/terrain_2d_mesher.h"

#include "world/block_terrain_2d_structure.h"
#include "world/terrain_2d_chunk.h"
#include "world/terrain_2d_environment_data.h"
#include "world/terrain_2d_structure.h"
#include "world/terrain_2d_world.h"

#include "world/default/terrain_2d_chunk_default.h"
#include "world/default/terrain_2d_world_default.h"

#include "level_generator/terrain_2d_level_generator.h"
#include "level_generator/terrain_2d_level_generator_flat.h"

#include "areas/terrain_2d_world_area.h"

#include "world/terrain_2d_world_editor.h"

#include "meshers/simple/terrain_2d_mesher_simple.h"
#include "world/simple/terrain_2d_chunk_simple.h"
#include "world/simple/terrain_2d_world_simple.h"

#include "meshers/isometric/terrain_2d_mesher_isometric.h"
#include "world/isometric/terrain_2d_chunk_isometric.h"
#include "world/isometric/terrain_2d_world_isometric.h"

#include "nodes/terrain_2d_light_node.h"

#include "world/jobs/terrain_2d_job.h"
#include "world/jobs/terrain_2d_light_job.h"
#include "world/jobs/terrain_2d_prop_job.h"
#include "world/jobs/terrain_2d_terrain_job.h"

void register_terraman_2d_types(ModuleRegistrationLevel p_level) {
	if (p_level == MODULE_REGISTRATION_LEVEL_SCENE) {
		ClassDB::register_class<Terrain2DMesher>();
		ClassDB::register_class<Terrain2DMesherDefault>();

		ClassDB::register_class<Terrain2DSurface>();
		ClassDB::register_class<Terrain2DSurfaceSimple>();

		ClassDB::register_class<Terrain2DLibrary>();
		ClassDB::register_class<Terrain2DLibrarySimple>();

		ClassDB::register_class<Terrain2DMaterialCache>();

#ifdef MODULE_TEXTURE_PACKER_ENABLED
		ClassDB::register_class<Terrain2DSurfaceMerger>();
		ClassDB::register_class<Terrain2DLibraryMerger>();
		ClassDB::register_class<Terrain2DLibraryMergerPCM>();
		ClassDB::register_class<Terrain2DMaterialCachePCM>();
#endif

		ClassDB::register_class<Terrain2DLight>();
		ClassDB::register_class<Terrain2DLightNode>();

		ClassDB::register_class<Terrain2DWorld>();
		ClassDB::register_class<Terrain2DChunk>();
		ClassDB::register_class<Terrain2DStructure>();
		ClassDB::register_class<BlockTerrain2DStructure>();
		ClassDB::register_class<Terrain2DEnvironmentData>();

		ClassDB::register_class<Terrain2DChunkDefault>();
		ClassDB::register_class<Terrain2DWorldDefault>();

		ClassDB::register_class<Terrain2DMesherSimple>();
		ClassDB::register_class<Terrain2DWorldSimple>();
		ClassDB::register_class<Terrain2DChunkSimple>();

		ClassDB::register_class<Terrain2DMesherIsometric>();
		ClassDB::register_class<Terrain2DWorldIsometric>();
		ClassDB::register_class<Terrain2DChunkIsometric>();

		ClassDB::register_class<Terrain2DLevelGenerator>();
		ClassDB::register_class<Terrain2DLevelGeneratorFlat>();

		ClassDB::register_class<Terrain2DWorldArea>();

		ClassDB::register_class<Terrain2DJob>();
		ClassDB::register_class<Terrain2DTerrain2DJob>();
		ClassDB::register_class<Terrain2DLightJob>();
		ClassDB::register_class<Terrain2DProp2DJob>();
	}

#ifdef TOOLS_ENABLED
	if (p_level == MODULE_REGISTRATION_LEVEL_EDITOR) {
		EditorPlugins::add_by_type<Terrain2DWorldEditorPlugin>();
	}
#endif
}

void unregister_terraman_2d_types(ModuleRegistrationLevel p_level) {
}
