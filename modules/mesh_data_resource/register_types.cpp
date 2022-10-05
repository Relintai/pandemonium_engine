/*
Copyright (c) 2019-2022 Péter Magyar

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

#include "./editor/mdi_ed_plugin.h"
#endif

#ifdef MODULE_PROPS_ENABLED
#include "../props/singleton/prop_utils.h"
#include "props/prop_data_mesh_data.h"
#endif

#ifdef MODULE_PROPS_2D_ENABLED
#include "../props_2d/singleton/prop_2d_utils.h"
#include "props_2d/prop_2d_data_mesh_data.h"
#endif

void register_mesh_data_resource_types() {
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

#ifdef TOOLS_ENABLED
#ifdef MODULE_GLTF_ENABLED
	EditorPlugins::add_by_type<EditorPluginGLTFMdr>();
#endif
	EditorPlugins::add_by_type<MDIEdPlugin>();
#endif
}

void unregister_mesh_data_resource_types() {
}
