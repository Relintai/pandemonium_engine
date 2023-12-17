#ifndef EDITOR_IMPORT_GLTF_MDR
#define EDITOR_IMPORT_GLTF_MDR


#include "core/variant/array.h"
#include "core/string/ustring.h"

#include "../plugin/mdr_import_plugin_base.h"

#include "core/io/resource_saver.h"
#include "core/math/basis.h"
#include "core/math/transform.h"
#include "scene/main/node.h"
#include "scene/resources/mesh/mesh.h"

#include "../mesh_data_resource.h"

#include "core/math/transform.h"

#include "gltf/editor_scene_importer_gltf.h"
#include "scene/3d/mesh_instance.h"

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

	virtual Error import(const String &p_source_file, const String &p_save_path, const RBMap<StringName, Variant> &p_options, List<String> *r_platform_variants, List<String> *r_gen_files = NULL, Variant *r_metadata = NULL);

	EditorImportGLTFMdr();
	~EditorImportGLTFMdr();

private:
	Ref<EditorSceneImporterGLTF> _importer;
};

#endif
