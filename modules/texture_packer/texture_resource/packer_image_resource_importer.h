#ifndef PACKER_IMAGE_RESOURCE_H
#define PACKER_IMAGE_RESOURCE_H


#include "core/io/image.h"
#include "core/string/ustring.h"

#include "core/io/resource_saver.h"
#include "editor/import/editor_import_plugin.h"

#include "core/io/image_loader.h"

#include "packer_image_resource.h"

class PackerImageResourceImporter : public EditorImportPlugin {
	GDCLASS(PackerImageResourceImporter, EditorImportPlugin);

public:
	virtual String get_importer_name() const;
	virtual String get_visible_name() const;
	virtual void get_recognized_extensions(List<String> *p_extensions) const;
	virtual String get_save_extension() const;
	virtual String get_resource_type() const;
	virtual float get_priority() const;

	virtual int get_preset_count() const;
	virtual String get_preset_name(int p_idx) const;

	virtual void get_import_options(List<ImportOption> *r_options, int p_preset = 0) const;
	virtual bool get_option_visibility(const String &p_option, const RBMap<StringName, Variant> &p_options) const;

	virtual Error import(const String &p_source_file, const String &p_save_path, const RBMap<StringName, Variant> &p_options, List<String> *r_platform_variants, List<String> *r_gen_files = NULL, Variant *r_metadata = NULL);

	PackerImageResourceImporter();
	~PackerImageResourceImporter();

private:
};

#endif
