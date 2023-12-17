

#include "register_types.h"

#include "modules/modules_enabled.gen.h"

#include "mesh_data_resource.h"
#include "mesh_data_resource_collection.h"
#include "nodes/mesh_data_instance.h"
#include "nodes/mesh_data_instance_2d.h"

#ifdef TOOLS_ENABLED
#include "editor/editor_plugin.h"

#ifdef MODULE_GLTF_ENABLED
#include "plugin_gltf/editor_plugin_gltf_mdr.h"
#endif

#ifdef MODULE_MESH_UTILS_ENABLED
#include "./editor/mdi_ed_plugin.h"
#endif

#endif

#ifdef MODULE_PROPS_ENABLED
#include "../props/singleton/prop_utils.h"
#include "props/prop_data_mesh_data.h"
#endif

#ifdef MODULE_PROPS_2D_ENABLED
#include "../props_2d/singleton/prop_2d_utils.h"
#include "props_2d/prop_2d_data_mesh_data.h"
#endif

void register_mesh_data_resource_types(ModuleRegistrationLevel p_level) {
	if (p_level == MODULE_REGISTRATION_LEVEL_SCENE) {
		ClassDB::register_class<MeshDataResource>();
		ClassDB::register_class<MeshDataResourceCollection>();

		ClassDB::register_class<MeshDataInstance>();
		ClassDB::register_class<MeshDataInstance2D>();

#ifdef MODULE_PROPS_ENABLED
		ClassDB::register_class<PropDataMeshData>();
		Ref<PropDataMeshData> processor = Ref<PropDataMeshData>(memnew(PropDataMeshData));
		PropUtils::add_processor(processor);
#endif

#ifdef MODULE_PROPS_2D_ENABLED
		ClassDB::register_class<Prop2DDataMeshData>();
		Ref<Prop2DDataMeshData> prop_2d_processor = Ref<Prop2DDataMeshData>(memnew(Prop2DDataMeshData));
		Prop2DUtils::add_processor(prop_2d_processor);
#endif
	}

#ifdef TOOLS_ENABLED
	if (p_level == MODULE_REGISTRATION_LEVEL_EDITOR) {
#ifdef MODULE_GLTF_ENABLED
		EditorPlugins::add_by_type<EditorPluginGLTFMdr>();
#endif

#ifdef MODULE_MESH_UTILS_ENABLED
		EditorPlugins::add_by_type<MDIEdPlugin>();
#endif
	}
#endif
}

void unregister_mesh_data_resource_types(ModuleRegistrationLevel p_level) {
}
