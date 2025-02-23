#ifndef RESOURCE_IMPORTER_H
#define RESOURCE_IMPORTER_H

/*************************************************************************/
/*  resource_importer.h                                                  */
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

#include "core/io/resource_loader.h"

class ResourceImporter;

class ResourceFormatImporter : public ResourceFormatLoader {
	struct PathAndType {
		String path;
		String type;
		String importer;
		String group_file;
		Variant metadata;
	};

	Error _get_path_and_type(const String &p_path, PathAndType &r_path_and_type, bool *r_valid = nullptr) const;

	static ResourceFormatImporter *singleton;

	//need them to stay in order to compute the settings hash
	struct SortImporterByName {
		bool operator()(const Ref<ResourceImporter> &p_a, const Ref<ResourceImporter> &p_b) const;
	};

	Vector<Ref<ResourceImporter>> importers;

public:
	static ResourceFormatImporter *get_singleton() { return singleton; }
	virtual RES load(const String &p_path, const String &p_original_path = "", Error *r_error = NULL, bool p_no_subresource_cache = false);
	virtual void get_recognized_extensions(List<String> *p_extensions) const;
	virtual void get_recognized_extensions_for_type(const String &p_type, List<String> *p_extensions) const;
	virtual bool recognize_path(const String &p_path, const String &p_for_type = String()) const;
	virtual bool handles_type(const String &p_type) const;
	virtual String get_resource_type(const String &p_path) const;
	virtual Variant get_resource_metadata(const String &p_path) const;
	virtual bool is_import_valid(const String &p_path) const;
	virtual void get_dependencies(const String &p_path, List<String> *p_dependencies, bool p_add_types = false);
	virtual bool is_imported(const String &p_path) const { return recognize_path(p_path); }
	virtual String get_import_group_file(const String &p_path) const;
	virtual bool exists(const String &p_path) const;

	virtual bool can_be_imported(const String &p_path) const;
	virtual int get_import_order(const String &p_path) const;

	String get_internal_resource_path(const String &p_path) const;
	void get_internal_resource_path_list(const String &p_path, List<String> *r_paths);

	void add_importer(const Ref<ResourceImporter> &p_importer) {
		importers.push_back(p_importer);
	}
	void remove_importer(const Ref<ResourceImporter> &p_importer) { importers.erase(p_importer); }
	Ref<ResourceImporter> get_importer_by_name(const String &p_name) const;
	Ref<ResourceImporter> get_importer_by_extension(const String &p_extension) const;
	void get_importers_for_extension(const String &p_extension, List<Ref<ResourceImporter>> *r_importers);
	void get_importers(List<Ref<ResourceImporter>> *r_importers);

	bool are_import_settings_valid(const String &p_path) const;
	String get_import_settings_hash() const;

	String get_import_base_path(const String &p_for_file) const;
	ResourceFormatImporter();
};

class ResourceImporter : public Reference {
	GDCLASS(ResourceImporter, Reference);

protected:
	static void _bind_methods();

public:
	virtual String get_importer_name() const = 0;
	virtual String get_visible_name() const = 0;
	virtual void get_recognized_extensions(List<String> *p_extensions) const = 0;
	virtual String get_save_extension() const = 0;
	virtual String get_resource_type() const = 0;
	virtual float get_priority() const { return 1.0; }
	virtual int get_import_order() const { return IMPORT_ORDER_DEFAULT; }
	virtual bool use_on_initial_import() const { return true; }

	struct ImportOption {
		PropertyInfo option;
		Variant default_value;

		ImportOption(const PropertyInfo &p_info, const Variant &p_default) :
				option(p_info),
				default_value(p_default) {
		}
		ImportOption() {}
	};

	enum ImportOrder {
		IMPORT_ORDER_DEFAULT = 0,
		IMPORT_ORDER_SCENE = 100,
	};

	virtual int get_preset_count() const { return 0; }
	virtual String get_preset_name(int p_idx) const { return String(); }

	virtual void get_import_options(List<ImportOption> *r_options, int p_preset = 0) const = 0;
	virtual bool get_option_visibility(const String &p_option, const RBMap<StringName, Variant> &p_options) const = 0;
	virtual String get_option_group_file() const { return String(); }

	virtual Error import(const String &p_source_file, const String &p_save_path, const RBMap<StringName, Variant> &p_options, List<String> *r_platform_variants, List<String> *r_gen_files = nullptr, Variant *r_metadata = nullptr) = 0;

	virtual Error import_group_file(const String &p_group_file, const RBMap<String, RBMap<StringName, Variant>> &p_source_file_options, const RBMap<String, String> &p_base_paths) { return ERR_UNAVAILABLE; }
	virtual bool are_import_settings_valid(const String &p_path) const { return true; }
	virtual String get_import_settings_string() const { return String(); }
};

VARIANT_ENUM_CAST(ResourceImporter::ImportOrder);

#endif // RESOURCE_IMPORTER_H
