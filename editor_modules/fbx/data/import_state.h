#ifndef IMPORT_STATE_H
#define IMPORT_STATE_H

/*************************************************************************/
/*  import_state.h                                                       */
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

#include "fbx_mesh_data.h"
#include "../tools/import_utils.h"
#include "../tools/validation_tools.h"

#include "pivot_transform.h"

#include "core/bind/core_bind.h"
#include "core/io/resource_importer.h"
#include "core/containers/vector.h"
#include "editor/import/resource_importer_scene.h"
#include "editor/project_settings_editor.h"
#include "scene/3d/mesh_instance.h"
#include "scene/main/spatial.h"
#include "scene/animation/animation_player.h"
#include "scene/animation/animation.h"
#include "scene/resources/material/spatial_material.h"
#include "scene/resources/mesh/surface_tool.h"

#include "../fbx_parser/FBXDocument.h"
#include "../fbx_parser/FBXImportSettings.h"
#include "../fbx_parser/FBXMeshGeometry.h"
#include "../fbx_parser/FBXParser.h"
#include "../fbx_parser/FBXTokenizer.h"
#include "../fbx_parser/FBXUtil.h"

#include "modules/skeleton_3d/nodes/skeleton.h"

struct FBXBone;
struct FBXMeshData;
struct FBXNode;
struct FBXSkeleton;

struct ImportState {
	bool enable_material_import = true;
	bool enable_animation_import = true;
	bool is_blender_fbx = false;

	RBMap<StringName, Ref<Texture>> cached_image_searches;
	RBMap<uint64_t, Ref<SpatialMaterial>> cached_materials;

	String path = String();
	Spatial *root_owner = nullptr;
	Spatial *root = nullptr;
	real_t scale = 0.01;
	Ref<FBXNode> fbx_root_node = Ref<FBXNode>();
	// skeleton map - merged automatically when they are on the same x node in the tree so we can merge them automatically.
	RBMap<uint64_t, Ref<FBXSkeleton>> skeleton_map = RBMap<uint64_t, Ref<FBXSkeleton>>();

	// nodes on the same level get merged automatically.
	//RBMap<uint64_t, Skeleton *> armature_map;
	AnimationPlayer *animation_player = nullptr;

	// Generation 4 - Raw document accessing for bone/skin/joint/kLocators
	// joints are not necessarily bones but must be merged into the skeleton
	// (bone id), bone
	RBMap<uint64_t, Ref<FBXBone>> fbx_bone_map = RBMap<uint64_t, Ref<FBXBone>>(); // this is the bone name and setup information required for joints
	// this will never contain joints only bones attached to a mesh.

	// Generation 4 - Raw document for creating the nodes transforms in the scene
	// this is a list of the nodes in the scene
	// (id, node)
	List<Ref<FBXNode>> fbx_node_list = List<Ref<FBXNode>>();

	// All nodes which have been created in the scene
	// this will not contain the root node of the scene
	RBMap<uint64_t, Ref<FBXNode>> fbx_target_map = RBMap<uint64_t, Ref<FBXNode>>();

	// mesh nodes which are created in node / mesh step - used for populating skin poses in MeshSkins
	RBMap<uint64_t, Ref<FBXNode>> MeshNodes = RBMap<uint64_t, Ref<FBXNode>>();
	// mesh skin map
	RBMap<uint64_t, Ref<Skin>> MeshSkins = RBMap<uint64_t, Ref<Skin>>();

	// this is the container for the mesh weight information and eventually
	// any mesh data
	// but not the skin, just stuff important for rendering
	// skin is applied to mesh instance so not really required to be in here yet.
	// maybe later
	// fbx mesh id, FBXMeshData
	RBMap<uint64_t, Ref<FBXMeshData>> renderer_mesh_data = RBMap<uint64_t, Ref<FBXMeshData>>();
};

#endif // IMPORT_STATE_H
