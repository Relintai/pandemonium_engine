/*************************************************************************/
/*  export.cpp                                                           */
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

#include "export.h"

#include "core/os/file_access.h"
#include "editor/editor_export.h"
#include "platform/frt/logo.gen.h"
#include "scene/resources/texture.h"


class EditorExportPlatformFRTSDL : public EditorExportPlatform {
	GDCLASS(EditorExportPlatformFRTSDL, EditorExportPlatform);

private:
	Ref<ImageTexture> logo;
	String name;
	String os_name;
	RBMap<String, String> extensions;

	String release_file_32;
	String release_file_64;
	String debug_file_32;
	String debug_file_64;
	// For Linux only.
	RBMap<String, String> release_files;
	RBMap<String, String> debug_files;

	RBSet<String> extra_features;

	int chmod_flags;

public:
	virtual void get_preset_features(const Ref<EditorExportPreset> &p_preset, List<String> *r_features);

	virtual void get_export_options(List<ExportOption> *r_options);

	virtual String get_name() const;
	virtual String get_os_name() const;
	virtual Ref<Texture> get_logo() const;

	virtual bool has_valid_export_configuration(const Ref<EditorExportPreset> &p_preset, String &r_error, bool &r_missing_templates) const;
	virtual bool has_valid_project_configuration(const Ref<EditorExportPreset> &p_preset, String &r_error) const;
	virtual List<String> get_binary_extensions(const Ref<EditorExportPreset> &p_preset) const;
	virtual Error export_project(const Ref<EditorExportPreset> &p_preset, bool p_debug, const String &p_path, int p_flags = 0);
	virtual Error sign_shared_object(const Ref<EditorExportPreset> &p_preset, bool p_debug, const String &p_path);

	virtual Error prepare_template(const Ref<EditorExportPreset> &p_preset, bool p_debug, const String &p_path, int p_flags);
	virtual Error modify_template(const Ref<EditorExportPreset> &p_preset, bool p_debug, const String &p_path, int p_flags) { return OK; }
	virtual Error export_project_data(const Ref<EditorExportPreset> &p_preset, bool p_debug, const String &p_path, int p_flags);

	void set_extension(const String &p_extension, const String &p_feature_key = "default");
	void set_name(const String &p_name);
	void set_os_name(const String &p_name);

	void set_logo(const Ref<Texture> &p_logo);

	void set_release_64(const String &p_file);
	void set_release_32(const String &p_file);
	void set_debug_64(const String &p_file);
	void set_debug_32(const String &p_file);

	// For Linux only.
	void set_release_files(const String &p_arch, const String &p_file);
	void set_debug_files(const String &p_arch, const String &p_file);
	String get_preset_arch(const Ref<EditorExportPreset> &p_preset) const;

	void add_platform_feature(const String &p_feature);
	virtual void get_platform_features(List<String> *r_features);
	virtual void resolve_platform_feature_priorities(const Ref<EditorExportPreset> &p_preset, RBSet<String> &p_features);

	int get_chmod_flags() const;
	void set_chmod_flags(int p_flags);

	EditorExportPlatformFRTSDL();
};


//////////

void EditorExportPlatformFRTSDL::get_preset_features(const Ref<EditorExportPreset> &p_preset, List<String> *r_features) {
	if (p_preset->get("texture_format/s3tc")) {
		r_features->push_back("s3tc");
	}
	if (p_preset->get("texture_format/etc")) {
		r_features->push_back("etc");
	}
	if (p_preset->get("texture_format/etc2")) {
		r_features->push_back("etc2");
	}

	const String &arch = get_preset_arch(p_preset);
	r_features->push_back(arch);
	if (arch == "arm64v8") {
		r_features->push_back("64");
	} else {
		r_features->push_back("32");
	}
}

void EditorExportPlatformFRTSDL::get_export_options(List<ExportOption> *r_options) {
	String ext_filter = "";
	r_options->push_back(ExportOption(PropertyInfo(Variant::STRING, "custom_template/debug", PROPERTY_HINT_GLOBAL_FILE, ext_filter), ""));
	r_options->push_back(ExportOption(PropertyInfo(Variant::STRING, "custom_template/release", PROPERTY_HINT_GLOBAL_FILE, ext_filter), ""));

	// Linux support to export to ARM architectures was added in 3.6.
	// Given how late this arrived, we didn't refactor the whole export preset
	// interface to support more per-platform flexibility, like done in 4.0,
	// so instead we hack the few needed changes here with `get_os_name()` checks.

	r_options->push_back(ExportOption(PropertyInfo(Variant::STRING, "binary_format/architecture", PROPERTY_HINT_ENUM, "arm32v6,arm32v7,arm64v8"), "arm32v6"));

	r_options->push_back(ExportOption(PropertyInfo(Variant::BOOL, "texture_format/bptc"), false));
	r_options->push_back(ExportOption(PropertyInfo(Variant::BOOL, "texture_format/s3tc"), true));
	r_options->push_back(ExportOption(PropertyInfo(Variant::BOOL, "texture_format/etc"), false));
	r_options->push_back(ExportOption(PropertyInfo(Variant::BOOL, "texture_format/etc2"), false));
	r_options->push_back(ExportOption(PropertyInfo(Variant::BOOL, "texture_format/no_bptc_fallbacks"), true));
}

String EditorExportPlatformFRTSDL::get_name() const {
	return name;
}

String EditorExportPlatformFRTSDL::get_os_name() const {
	return os_name;
}
Ref<Texture> EditorExportPlatformFRTSDL::get_logo() const {
	return logo;
}

bool EditorExportPlatformFRTSDL::has_valid_export_configuration(const Ref<EditorExportPreset> &p_preset, String &r_error, bool &r_missing_templates) const {
	String err;
	bool valid = false;
	bool dvalid = false;
	bool rvalid = false;

	// Look for export templates (first official, and if defined custom templates).

	const String &arch = get_preset_arch(p_preset);
	dvalid = exists_export_template(debug_files[arch], &err);
	rvalid = exists_export_template(release_files[arch], &err);

	if (p_preset->get("custom_template/debug") != "") {
		dvalid = FileAccess::exists(p_preset->get("custom_template/debug"));
		if (!dvalid) {
			err += TTR("Custom debug template not found.") + "\n";
		}
	}
	if (p_preset->get("custom_template/release") != "") {
		rvalid = FileAccess::exists(p_preset->get("custom_template/release"));
		if (!rvalid) {
			err += TTR("Custom release template not found.") + "\n";
		}
	}

	valid = dvalid || rvalid;
	r_missing_templates = !valid;

	if (!err.empty()) {
		r_error = err;
	}
	return valid;
}

bool EditorExportPlatformFRTSDL::has_valid_project_configuration(const Ref<EditorExportPreset> &p_preset, String &r_error) const {
	return true;
}

List<String> EditorExportPlatformFRTSDL::get_binary_extensions(const Ref<EditorExportPreset> &p_preset) const {
	List<String> list;

	for (RBMap<String, String>::Element *E = extensions.front(); E; E = E->next()) {
		if (get_preset_arch(p_preset) == E->key()) {
			list.push_back(extensions[E->key()]);
			return list;
		}
	}

	if (extensions.has("default")) {
		list.push_back(extensions["default"]);
		return list;
	}

	return list;
}

Error EditorExportPlatformFRTSDL::export_project(const Ref<EditorExportPreset> &p_preset, bool p_debug, const String &p_path, int p_flags) {
	ExportNotifier notifier(*this, p_preset, p_debug, p_path, p_flags);

	Error err = prepare_template(p_preset, p_debug, p_path, p_flags);
	if (err == OK) {
		err = modify_template(p_preset, p_debug, p_path, p_flags);
	}
	if (err == OK) {
		err = export_project_data(p_preset, p_debug, p_path, p_flags);
	}

	return err;
}

Error EditorExportPlatformFRTSDL::prepare_template(const Ref<EditorExportPreset> &p_preset, bool p_debug, const String &p_path, int p_flags) {
	if (!DirAccess::exists(p_path.get_base_dir())) {
		add_message(EXPORT_MESSAGE_ERROR, TTR("Prepare Template"), TTR("The given export path doesn't exist."));
		return ERR_FILE_BAD_PATH;
	}

	String custom_debug = p_preset->get("custom_template/debug");
	String custom_release = p_preset->get("custom_template/release");

	String template_path = p_debug ? custom_debug : custom_release;

	template_path = template_path.strip_edges();

	if (template_path == String()) {
		if (p_debug) {
			template_path = find_export_template(debug_files[get_preset_arch(p_preset)]);
		} else {
			template_path = find_export_template(release_files[get_preset_arch(p_preset)]);
		}
	}

	if (template_path != String() && !FileAccess::exists(template_path)) {
		add_message(EXPORT_MESSAGE_ERROR, TTR("Prepare Template"), vformat(TTR("Template file not found: \"%s\"."), template_path));
		return ERR_FILE_NOT_FOUND;
	}

	DirAccessRef da = DirAccess::create(DirAccess::ACCESS_FILESYSTEM);
	da->make_dir_recursive(p_path.get_base_dir());
	Error err = da->copy(template_path, p_path, get_chmod_flags());

	if (err != OK) {
		add_message(EXPORT_MESSAGE_ERROR, TTR("Prepare Template"), TTR("Failed to copy export template."));
	}

	return err;
}

Error EditorExportPlatformFRTSDL::export_project_data(const Ref<EditorExportPreset> &p_preset, bool p_debug, const String &p_path, int p_flags) {
	String pck_path;

	pck_path = p_path.get_basename() + ".pck";

	Vector<SharedObject> so_files;

	int64_t embedded_pos;
	int64_t embedded_size;
	Error err = save_pack(p_preset, pck_path, &so_files, false, &embedded_pos, &embedded_size);

	if (err == OK && !so_files.empty()) {
		// If shared object files, copy them.
		DirAccessRef da = DirAccess::create(DirAccess::ACCESS_FILESYSTEM);
		for (int i = 0; i < so_files.size() && err == OK; i++) {
			err = da->copy(so_files[i].path, p_path.get_base_dir().plus_file(so_files[i].path.get_file()));
			if (err == OK) {
				err = sign_shared_object(p_preset, p_debug, p_path.get_base_dir().plus_file(so_files[i].path.get_file()));
			}
		}
	}

	return err;
}

Error EditorExportPlatformFRTSDL::sign_shared_object(const Ref<EditorExportPreset> &p_preset, bool p_debug, const String &p_path) {
	return OK;
}

void EditorExportPlatformFRTSDL::set_extension(const String &p_extension, const String &p_feature_key) {
	extensions[p_feature_key] = p_extension;
}

void EditorExportPlatformFRTSDL::set_name(const String &p_name) {
	name = p_name;
}

void EditorExportPlatformFRTSDL::set_os_name(const String &p_name) {
	os_name = p_name;
}

void EditorExportPlatformFRTSDL::set_logo(const Ref<Texture> &p_logo) {
	logo = p_logo;
}

void EditorExportPlatformFRTSDL::set_release_64(const String &p_file) {
	release_file_64 = p_file;
}

void EditorExportPlatformFRTSDL::set_release_32(const String &p_file) {
	release_file_32 = p_file;
}

void EditorExportPlatformFRTSDL::set_debug_64(const String &p_file) {
	debug_file_64 = p_file;
}

void EditorExportPlatformFRTSDL::set_debug_32(const String &p_file) {
	debug_file_32 = p_file;
}

// For Linux only.
void EditorExportPlatformFRTSDL::set_release_files(const String &p_arch, const String &p_file) {
	release_files[p_arch] = p_file;
}

void EditorExportPlatformFRTSDL::set_debug_files(const String &p_arch, const String &p_file) {
	debug_files[p_arch] = p_file;
}

String EditorExportPlatformFRTSDL::get_preset_arch(const Ref<EditorExportPreset> &p_preset) const {
	String arch = p_preset->get("binary_format/architecture");

	if (arch != "arm32v6" && arch != "arm32v7" && arch != "arm64v8") {
		ERR_PRINT(vformat("Invalid value \"%s\" for \"binary_format/architecture\" in export preset \"%s\". Defaulting to \"arm32v6\".",
				arch, 
				p_preset->get_name()));

		arch = "arm32v6";
	}

	return arch;
}

void EditorExportPlatformFRTSDL::add_platform_feature(const String &p_feature) {
	extra_features.insert(p_feature);
}

void EditorExportPlatformFRTSDL::get_platform_features(List<String> *r_features) {
	r_features->push_back("pc"); //all pcs support "pc"
	r_features->push_back("s3tc"); //all pcs support "s3tc" compression
	r_features->push_back(get_os_name()); //OS name is a feature

	for (RBSet<String>::Element *E = extra_features.front(); E; E = E->next()) {
		r_features->push_back(E->get());
	}
}

void EditorExportPlatformFRTSDL::resolve_platform_feature_priorities(const Ref<EditorExportPreset> &p_preset, RBSet<String> &p_features) {
	if (p_features.has("bptc")) {
		if (p_preset->has("texture_format/no_bptc_fallbacks")) {
			p_features.erase("s3tc");
		}
	}
}

int EditorExportPlatformFRTSDL::get_chmod_flags() const {
	return chmod_flags;
}

void EditorExportPlatformFRTSDL::set_chmod_flags(int p_flags) {
	chmod_flags = p_flags;
}

EditorExportPlatformFRTSDL::EditorExportPlatformFRTSDL() {
	chmod_flags = -1;
}

void register_frt_sdl_exporter() {
	Ref<EditorExportPlatformFRTSDL> platform;
	platform.instance();

	Ref<Image> img = memnew(Image(_frt_logo));
	Ref<ImageTexture> logo;
	logo.instance();
	logo->create_from_image(img);
	platform->set_logo(logo);
	platform->set_name("Linux/FRT SDL");
	platform->set_extension("arm32v6", "arm32v6");
	platform->set_extension("arm32v7", "arm32v7");
	platform->set_extension("arm64v8", "arm64v8");
	platform->set_release_files("arm32v6", "linux_frt_sdl_arm32v6_release");
	platform->set_release_files("arm32v7", "linux_frt_sdl_arm32v7_release");
	platform->set_release_files("arm64v8", "linux_frt_sdl_arm64v8_release");
	platform->set_debug_files("arm32v6", "linux_frt_sdl_arm32v6_debug");
	platform->set_debug_files("arm32v7", "linux_frt_sdl_arm32v7_debug");
	platform->set_debug_files("arm64v8", "linux_frt_sdl_arm64v8_debug");
	platform->set_os_name("FRTSDL");
	platform->set_chmod_flags(0755);

	EditorExport::get_singleton()->add_export_platform(platform);
}
