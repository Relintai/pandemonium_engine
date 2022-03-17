#ifndef EDITOR_IMPORT_GLTF_MDR
#define EDITOR_IMPORT_GLTF_MDR
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




#include "core/version.h"

#if VERSION_MAJOR > 3
#include "core/string/ustring.h"
#include "core/variant/array.h"
#else
#include "core/ustring.h"
#include "core/array.h"
#endif

#include "../plugin/mdr_import_plugin_base.h"

#include "core/io/resource_saver.h"
#include "core/math/basis.h"
#include "core/math/transform.h"
#include "scene/main/node.h"
#include "scene/resources/mesh.h"

#include "../mesh_data_resource.h"

#include "core/math/transform.h"

#include "core/version.h"

#if VERSION_MAJOR < 4

#if VERSION_MINOR < 4
#include "editor/import/editor_scene_importer_gltf.h"
#else
#include "../../gltf/editor_scene_importer_gltf.h"
#endif

#include "scene/3d/mesh_instance.h"
#else
#include "../../gltf/editor_scene_importer_gltf.h"
#include "scene/3d/mesh_instance_3d.h"

#define MeshInstance MeshInstance3D
#endif

class EditorImportGLTFMdr : public MDRImportPluginBase {

	GDCLASS(EditorImportGLTFMdr, MDRImportPluginBase);

public:
	virtual String get_importer_name() const;
	virtual String get_visible_name() const;
	virtual void get_recognized_extensions(List<String> *p_extensions) const;
	virtual String get_save_extension() const;
	virtual String get_resource_type() const;
	virtual float get_priority() const;

	virtual int get_preset_count() const;
	virtual String get_preset_name(int p_idx) const;

	virtual Error import(const String &p_source_file, const String &p_save_path, const Map<StringName, Variant> &p_options, List<String> *r_platform_variants, List<String> *r_gen_files = NULL, Variant *r_metadata = NULL);

	EditorImportGLTFMdr();
	~EditorImportGLTFMdr();

private:
	Ref<EditorSceneImporterGLTF> _importer;
};

#endif
