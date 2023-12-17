#ifndef MDR_IMPORT_PLUGIN_BASE
#define MDR_IMPORT_PLUGIN_BASE

/*************************************************************************/
/*  mdr_import_plugin_base.h                                             */
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

#include "core/string/ustring.h"
#include "core/variant/array.h"

#include "../mesh_data_resource_collection.h"
#include "core/io/resource_saver.h"
#include "core/math/basis.h"
#include "core/math/transform.h"
#include "editor/import/editor_import_plugin.h"
#include "scene/main/node.h"
#include "scene/resources/mesh/mesh.h"

#include "../mesh_data_resource.h"

#include "core/math/transform.h"

#include "scene/3d/mesh_instance.h"

#include "modules/modules_enabled.gen.h"

class MDRImportPluginBase : public EditorImportPlugin {
	GDCLASS(MDRImportPluginBase, EditorImportPlugin);

public:
	static const String BINDING_MDR_IMPORT_TYPE;
	static const String BINDING_MDR_SURFACE_HANDLING_TYPE;
	static const String BINDING_MDR_OPTIMIZATION_TYPE;

	enum MDRImportType {
		MDR_IMPORT_TIME_SINGLE = 0,
		//MDR_IMPORT_TIME_SINGLE_MERGED,
		MDR_IMPORT_TIME_MULTIPLE,
		//MDR_IMPORT_TIME_SINGLE_WITH_SEPARATED_BONES,
	};

	enum MDRSurfaceHandlingType {
		MDR_SURFACE_HANDLING_TYPE_ONLY_USE_FIRST = 0,
		MDR_SURFACE_HANDLING_TYPE_SEPARATE_MDRS,
		MDR_SURFACE_HANDLING_TYPE_MERGE,
	};

	enum MDROptimizationType {
		MDR_OPTIMIZATION_OFF = 0,
#ifdef MODULE_MESH_UTILS_ENABLED
		MDR_OPTIMIZATION_REMOVE_DOUBLES,
		MDR_OPTIMIZATION_REMOVE_DOUBLES_INTERPOLATE_NORMALS,
#endif
	};

public:
	virtual void get_import_options(List<ImportOption> *r_options, int p_preset = 0) const;
	virtual bool get_option_visibility(const String &p_option, const RBMap<StringName, Variant> &p_options) const;

	Error process_node(Node *n, const String &p_source_file, const String &p_save_path, const RBMap<StringName, Variant> &p_options, List<String> *r_platform_variants, List<String> *r_gen_files = NULL, Variant *r_metadata = NULL);

	int get_mesh_count(Node *n);
	Error process_node_single(Node *n, const String &p_source_file, const String &p_save_path, const RBMap<StringName, Variant> &p_options, List<String> *r_platform_variants, List<String> *r_gen_files, Variant *r_metadata);
	Error process_node_single_separated_bones(Node *n, const String &p_source_file, const String &p_save_path, const RBMap<StringName, Variant> &p_options, List<String> *r_platform_variants, List<String> *r_gen_files, Variant *r_metadata);
	Error process_node_multi(Node *n, const String &p_source_file, const String &p_save_path, const RBMap<StringName, Variant> &p_options, List<String> *r_platform_variants, List<String> *r_gen_files, Variant *r_metadata, Ref<MeshDataResourceCollection> coll, Ref<MeshDataResourceCollection> copy_coll, int node_count = 0);
	Vector<Ref<MeshDataResource>> get_meshes(MeshInstance *mi, const RBMap<StringName, Variant> &p_options, MeshDataResource::ColliderType collider_type, Vector3 scale);
	Ref<MeshDataResource> get_mesh_arrays(Array &arrs, const RBMap<StringName, Variant> &p_options, MeshDataResource::ColliderType collider_type, Vector3 scale);
	void add_colliders(Ref<MeshDataResource> mdr, Ref<ArrayMesh> mesh, const RBMap<StringName, Variant> &p_options, MeshDataResource::ColliderType collider_type, Vector3 scale);

	Vector<Array> split_mesh_bones(Ref<ArrayMesh> mesh);
	Array slice_mesh_bone(const Array &array, int bone_idx);
	Array apply_transforms(Array &array, const RBMap<StringName, Variant> &p_options);
	Ref<Shape> scale_shape(Ref<Shape> shape, const Vector3 &scale);

	void save_mdr_copy_as_tres(const String &p_source_file, const Ref<MeshDataResource> &res, bool indexed = false, int index = 0);
	void save_mdrcoll_copy_as_tres(const String &p_source_file, const Ref<MeshDataResourceCollection> &res);

	MDRImportPluginBase();
	~MDRImportPluginBase();
};

#endif
