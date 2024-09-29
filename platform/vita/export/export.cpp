/**************************************************************************/
/*  export.cpp                                                            */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "export.h"
#include "core/io/zip_io.h"
#include "core/version.h"

#include "core/config/project_settings.h"
#include "editor/editor_settings.h"

#define TEMPLATE_RELEASE "vita_release.zip"

class ExportPluginVita : public EditorExportPlugin {
public:
	Vector<uint8_t> editor_id_vec;

protected:
	virtual void _export_begin(const RBSet<String> &p_features, bool p_debug, const String &p_path, int p_flags) {
		if (editor_id_vec.size() != 0) {
			add_file("custom_editor_id", editor_id_vec, false);
		}
	}
};

class EditorExportPlatformVita : public EditorExportPlatform {
	GDCLASS(EditorExportPlatformVita, EditorExportPlatform)

	Ref<ImageTexture> logo;

	ExportPluginVita *export_plugin;

public:
	virtual void get_preset_features(const Ref<EditorExportPreset> &p_preset, List<String> *r_features) {
		String driver = ProjectSettings::get_singleton()->get("rendering/quality/driver/driver_name");
		if ((driver == "GLES2") || (driver == "GLES3" && ProjectSettings::get_singleton()->get("rendering/quality/driver/fallback_to_gles2"))) {
			if (p_preset->get("texture_format/etc")) {
				r_features->push_back("etc");
			}
			if (p_preset->get("texture_format/pvrtc")) {
				r_features->push_back("pvrtc");
			}
		}
	}

	virtual void get_export_options(List<ExportOption> *r_options) {
		String title = ProjectSettings::get_singleton()->get("application/config/name");
		r_options->push_back(ExportOption(PropertyInfo(Variant::BOOL, "texture_format/etc"), false));
		r_options->push_back(ExportOption(PropertyInfo(Variant::BOOL, "texture_format/pvrtc"), false));
		r_options->push_back(ExportOption(PropertyInfo(Variant::STRING, "param_sfo/title", PROPERTY_HINT_PLACEHOLDER_TEXT, title), title));
		r_options->push_back(ExportOption(PropertyInfo(Variant::STRING, "param_sfo/title_id", PROPERTY_HINT_PLACEHOLDER_TEXT, "GDOT00001 (Make sure it's CAPITALIZED and 9 characters MAX"), "GDOT00001"));
		r_options->push_back(ExportOption(PropertyInfo(Variant::INT, "param_sfo/parental_level", PROPERTY_HINT_MAX, "11"), 1));
		r_options->push_back(ExportOption(PropertyInfo(Variant::STRING, "param_sfo/version", PROPERTY_HINT_PLACEHOLDER_TEXT, "Game Version XX.YY"), "01.00"));
		r_options->push_back(ExportOption(PropertyInfo(Variant::STRING, "assets/bubble_icon_128x128", PROPERTY_HINT_GLOBAL_FILE, "*.png"), ""));
		r_options->push_back(ExportOption(PropertyInfo(Variant::STRING, "assets/app_splash_960x544", PROPERTY_HINT_GLOBAL_FILE, "*.png"), ""));
		r_options->push_back(ExportOption(PropertyInfo(Variant::STRING, "assets/livearea_bg_840x500", PROPERTY_HINT_GLOBAL_FILE, "*.png"), ""));
		r_options->push_back(ExportOption(PropertyInfo(Variant::STRING, "assets/livearea_startup_button_280x158", PROPERTY_HINT_GLOBAL_FILE, "*.png"), ""));
	}

	virtual String get_name() const {
		return "PlayStation Vita";
	}

	virtual String get_os_name() const {
		return "Vita";
	}

	virtual Ref<Texture> get_logo() const {
		return logo;
	}

	virtual Ref<Texture> get_run_icon() const {
		return logo;
	}

	virtual bool poll_devices() {
		return false;
	}

	virtual int get_device_count() const {
		return 2;
	}

	virtual Error run(const Ref<EditorExportPreset> &p_preset, int p_device, int p_debug_flags) {
		return OK;
	}

	virtual bool has_valid_export_configuration(const Ref<EditorExportPreset> &p_preset, String &r_error, bool &r_missing_templates) const {
		String err;
		r_missing_templates =
				find_export_template(TEMPLATE_RELEASE) == String();

		bool valid = !r_missing_templates;

		if (!err.empty()) {
			r_error = err;
		}

		return valid;
	}

	virtual bool has_valid_project_configuration(const Ref<EditorExportPreset> &p_preset, String &r_error) const {
		return true;
	}

	virtual List<String> get_binary_extensions(const Ref<EditorExportPreset> &p_preset) const {
		List<String> list;
		list.push_back("vpk");
		return list;
	}

	void _zip_folder_recursive(zipFile &p_zip, const String &p_root_path, const String &p_folder) {
		String dir = p_root_path.plus_file(p_folder);

		DirAccess *da = DirAccess::open(dir);
		da->list_dir_begin();
		String f = da->get_next();
		while (!f.empty()) {
			if (f == "." || f == "..") {
				f = da->get_next();
				continue;
			}
			if (da->is_link(f)) {
				OS::Time time = OS::get_singleton()->get_time();
				OS::Date date = OS::get_singleton()->get_date();

				zip_fileinfo zipfi;
				zipfi.tmz_date.tm_hour = time.hour;
				zipfi.tmz_date.tm_mday = date.day;
				zipfi.tmz_date.tm_min = time.min;
				zipfi.tmz_date.tm_mon = date.month - 1; // Note: "tm" month range - 0..11, Pandemonium month range - 1..12, http://www.cplusplus.com/reference/ctime/tm/
				zipfi.tmz_date.tm_sec = time.sec;
				zipfi.tmz_date.tm_year = date.year;
				zipfi.dosDate = 0;
				// 0120000: symbolic link type
				// 0000755: permissions rwxr-xr-x
				// 0000644: permissions rw-r--r--
				uint32_t _mode = 0100777;
				zipfi.external_fa = (_mode << 16L) | !(_mode & 0200);
				zipfi.internal_fa = 0;

				zipOpenNewFileInZip4(p_zip,
						p_folder.plus_file(f).utf8().get_data(),
						&zipfi,
						nullptr,
						0,
						nullptr,
						0,
						nullptr,
						Z_DEFLATED,
						Z_DEFAULT_COMPRESSION,
						0,
						-MAX_WBITS,
						DEF_MEM_LEVEL,
						Z_DEFAULT_STRATEGY,
						nullptr,
						0,
						0x0314, // "version made by", 0x03 - Unix, 0x14 - ZIP specification version 2.0, required to store Unix file permissions
						0);

				String target = da->read_link(f);
				zipWriteInFileInZip(p_zip, target.utf8().get_data(), target.utf8().size());
				zipCloseFileInZip(p_zip);
			} else if (da->current_is_dir()) {
				_zip_folder_recursive(p_zip, p_root_path, p_folder.plus_file(f));
			} else {
				OS::Time time = OS::get_singleton()->get_time();
				OS::Date date = OS::get_singleton()->get_date();

				zip_fileinfo zipfi;
				zipfi.tmz_date.tm_hour = time.hour;
				zipfi.tmz_date.tm_mday = date.day;
				zipfi.tmz_date.tm_min = time.min;
				zipfi.tmz_date.tm_mon = date.month - 1; // Note: "tm" month range - 0..11, Godot month range - 1..12, http://www.cplusplus.com/reference/ctime/tm/
				zipfi.tmz_date.tm_sec = time.sec;
				zipfi.tmz_date.tm_year = date.year;
				zipfi.dosDate = 0;
				// 0100000: regular file type
				// 0000755: permissions rwxr-xr-x
				// 0000644: permissions rw-r--r--
				uint32_t _mode = 0100777;
				zipfi.external_fa = (_mode << 16L) | !(_mode & 0200);
				zipfi.internal_fa = 0;

				zipOpenNewFileInZip4(p_zip,
						p_folder.plus_file(f).utf8().get_data(),
						&zipfi,
						nullptr,
						0,
						nullptr,
						0,
						nullptr,
						Z_DEFLATED,
						Z_DEFAULT_COMPRESSION,
						0,
						-MAX_WBITS,
						DEF_MEM_LEVEL,
						Z_DEFAULT_STRATEGY,
						nullptr,
						0,
						0x0314, // "version made by", 0x03 - Unix, 0x14 - ZIP specification version 2.0, required to store Unix file permissions
						0);

				FileAccessRef fa = FileAccess::open(dir.plus_file(f), FileAccess::READ);
				if (!fa) {
					add_message(EXPORT_MESSAGE_ERROR, TTR("ZIP Creation"), vformat(TTR("Could not open file to read from path \"%s\"."), dir.plus_file(f)));
					return;
				}
				const int bufsize = 16384;
				uint8_t buf[bufsize];

				while (true) {
					uint64_t got = fa->get_buffer(buf, bufsize);
					if (got == 0) {
						break;
					}
					zipWriteInFileInZip(p_zip, buf, got);					
				}

				zipCloseFileInZip(p_zip);
			}

			f = da->get_next();
		}

		da->list_dir_end();
		memdelete(da);
	}

	void create_vpk(String outVpk, String dir) {
		FileAccess *dst_f = nullptr;
		zlib_filefunc_def io_dst = zipio_create_io_from_file(&dst_f);
		zipFile zip = zipOpen2(outVpk.utf8().get_data(), APPEND_STATUS_CREATE, nullptr, &io_dst);
		_zip_folder_recursive(zip, dir, "");
		zipClose(zip, NULL);
	}

	virtual Error export_project(const Ref<EditorExportPreset> &p_preset, bool p_debug, const String &p_path, int p_flags = 0) {
		ExportNotifier notifier(*this, p_preset, p_debug, p_path, p_flags);

		const String base_dir = p_path.get_base_dir();
		const String base_path = p_path.get_basename();
		const String base_name = p_path.get_file().get_basename();

		if (!DirAccess::exists(base_dir)) {
			return ERR_FILE_BAD_PATH;
		}

		String template_path = find_export_template(TEMPLATE_RELEASE);
		if (template_path != String() && !FileAccess::exists(template_path)) {
			add_message(EXPORT_MESSAGE_ERROR, TTR("Prepare Templates"), vformat(TTR("Template file not found: \"%s\"."), template_path));
			return ERR_FILE_NOT_FOUND;
		}

		DirAccess *da = DirAccess::create(DirAccess::ACCESS_FILESYSTEM);

		Error err;
		// update nro icon/title/author/version
		ParamSFOStruct *sfo = memnew(ParamSFOStruct);

		sfo->title = p_preset->get("param_sfo/title");
		sfo->title_id = p_preset->get("param_sfo/title_id");
		sfo->version = p_preset->get("param_sfo/version");
		sfo->parental_level = p_preset->get("param_sfo/parental_level");

		String icon = p_preset->get("assets/bubble_icon_128x128");
		String splash = p_preset->get("assets/app_splash_960x544");
		String livearea_bg = p_preset->get("assets/livearea_bg_840x500");
		String livearea_startup_button = p_preset->get("assets/livearea_startup_button_280x158");

		String cache = EditorSettings::get_singleton()->get_cache_dir();
		String app_dir = cache.plus_file("app");
		da->make_dir(app_dir);
		String game_data_dir = app_dir.plus_file("game_data");
		da->make_dir(game_data_dir);
		da->make_dir(app_dir.plus_file("module"));
		da->make_dir_recursive(app_dir.plus_file("sce_sys/livearea/contents"));

		FileAccess *src_f = nullptr;
		zlib_filefunc_def io = zipio_create_io_from_file(&src_f);

		unzFile pkg = unzOpen2(template_path.utf8().get_data(), &io);

		if (!pkg) {
			EditorNode::add_io_error("Could not find vita template for exporting:\n" + template_path);
			return ERR_FILE_NOT_FOUND;
		}

		int ret = unzGoToFirstFile(pkg);

		while (ret == UNZ_OK) {
			// get file name
			unz_file_info info;
			char fname[16384];
			ret = unzGetCurrentFileInfo(pkg, &info, fname, 16384, nullptr, 0, nullptr, 0);

			String path = String::utf8(fname);

			if (path.ends_with("/")) {
				// Ignore directories
				ret = unzGoToNextFile(pkg);
				continue;
			}

			Vector<uint8_t> data;
			bool do_read = true;

			//read
			if (do_read) {
				data.resize(info.uncompressed_size);
				unzOpenCurrentFile(pkg);
				unzReadCurrentFile(pkg, data.ptrw(), data.size());
				unzCloseCurrentFile(pkg);
			}

			print_line("ADDING: " + path);

			FileAccess *fa = FileAccess::open(app_dir.plus_file(path), FileAccess::WRITE);
			fa->store_buffer(data.ptr(), data.size());
			fa->flush();
			fa->close();

			ret = unzGoToNextFile(pkg);
		}

		unzClose(pkg);

		//String current_version = VERSION_FULL_CONFIG;
		//String template_path = EditorSettings::get_singleton()->get_templates_dir().plus_file(current_version);
		Vector<SharedObject> shared_objects;

		err = save_pack(p_preset, game_data_dir.plus_file("game.pck"), &shared_objects);
		mksfoex(sfo, app_dir.plus_file("sce_sys"));
		if (err == OK) {
			if (icon != String() && FileAccess::exists(icon)) {
				da->copy(icon, app_dir.plus_file("sce_sys/icon0.png"));
			}
			if (splash != String() && FileAccess::exists(splash)) {
				da->copy(splash, app_dir.plus_file("sce_sys/pic0.png"));
			}
			if (livearea_bg != String() && FileAccess::exists(livearea_bg)) {
				da->copy(livearea_bg, app_dir.plus_file("sce_sys/livearea/contents/bg.png"));
			}
			if (livearea_startup_button != String() && FileAccess::exists(livearea_startup_button)) {
				da->copy(livearea_startup_button, app_dir.plus_file("sce_sys/livearea/contents/startup.png"));
			}
		}

		create_vpk(base_path + ".vpk", app_dir);
		memdelete(sfo);
		memdelete(da);
		da = nullptr;

		return OK;
	}

	virtual void get_platform_features(List<String> *r_features) {
		r_features->push_back("mobile");
		r_features->push_back(get_os_name());
	}

	virtual void resolve_platform_feature_priorities(const Ref<EditorExportPreset> &p_preset, RBSet<String> &p_features) {
	}

	EditorExportPlatformVita() {
		Ref<Image> img = memnew(Image(_vita_logo));
		logo.instance();
		logo->create_from_image(img);

		export_plugin = memnew(ExportPluginVita);
		EditorExport::get_singleton()->add_export_plugin(export_plugin);
	}

	~EditorExportPlatformVita() {
	}
};

void register_vita_exporter() {
	Ref<EditorExportPlatformVita> exporter;
	exporter.instance();
	EditorExport::get_singleton()->add_export_platform(exporter);
}
