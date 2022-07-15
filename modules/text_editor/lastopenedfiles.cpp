
#include "lastopenedfiles.h"

#include "editor/editor_settings.h"

#include "core/variant.h"
#include "scene/gui/item_list.h"
#include "vanillaeditor.h"

void LastOpenedFiles::store_opened_files(ItemList *filecontainer) {
	Array arr;

	for (int child = 0; child < filecontainer->get_item_count(); ++child) {
		Array metaarr = filecontainer->get_item_metadata(child);
		Variant e = metaarr[0];
		Control *c = e;
		VanillaEditor *ed = Object::cast_to<VanillaEditor>(c);

		String filepath = ed->get_current_path();
		Array a = Array();
		a.push_back(filepath.get_file());
		a.push_back(filepath);
		arr.push_back(a);
	}

	EditorSettings::get_singleton()->set_project_metadata("file_editor", "files", arr);
}

void LastOpenedFiles::remove_opened_file(const int index, ItemList *filecontainer) {
	Array metaarr = filecontainer->get_item_metadata(index);
	Variant e = metaarr[0];
	Control *c = e;
	VanillaEditor *ed = Object::cast_to<VanillaEditor>(c);

	String filepath = ed->get_current_path();
	String f = filepath.get_file();
	Array arr = EditorSettings::get_singleton()->get_project_metadata("file_editor", "files", Array());

	for (int i = 0; i < arr.size(); ++i) {
		Array a = arr[i];

		if (a[0] == f) {
			arr.remove(i);
			break;
		}
	}

	EditorSettings::get_singleton()->set_project_metadata("file_editor", "files", arr);
	Dictionary fonts_dict = EditorSettings::get_singleton()->get_project_metadata("file_editor", "file_fonts", Dictionary());

	if (fonts_dict.has(f)) {
		fonts_dict.erase(f);
		EditorSettings::get_singleton()->set_project_metadata("file_editor", "file_fonts", fonts_dict);
	}
}

Array LastOpenedFiles::load_opened_files() {
	Array arr = EditorSettings::get_singleton()->get_project_metadata("file_editor", "files", Array());
	Dictionary fonts_dict = EditorSettings::get_singleton()->get_project_metadata("file_editor", "file_fonts", Dictionary());
	Array keys;

	for (int i = 0; i < arr.size(); ++i) { //i in range(arr.size())
		Array a = arr[i];
		// creating and returning an Array with this format [1:file name, 2:file path, 3:file font];
		Array k;
		k.push_back(a[0]);
		k.push_back(a[1]);

		if (fonts_dict.has(a[0])) {
			k.push_back(fonts_dict[a[0]]);
		}

		else {
			k.push_back("null");
		}

		keys.append(k);
	}

	return keys;
}

void LastOpenedFiles::store_editor_fonts(const String &file_name, const String &font_path) {
	Dictionary fonts_dict = EditorSettings::get_singleton()->get_project_metadata("file_editor", "file_fonts", Dictionary());
	fonts_dict[file_name] = font_path;
	EditorSettings::get_singleton()->set_project_metadata("file_editor", "file_fonts", fonts_dict);
}

String LastOpenedFiles::get_editor_font() {
	return EditorSettings::get_singleton()->get_setting("interface/editor/code_font");
}

LastOpenedFiles::LastOpenedFiles() {
}

LastOpenedFiles::~LastOpenedFiles() {
}

void LastOpenedFiles::_bind_methods() {
	//ClassDB::bind_method(D_METHOD("store_opened_files", "filecontainer"), &LastOpenedFiles::store_opened_files);
	//ClassDB::bind_method(D_METHOD("remove_opened_file", "index", "filecontainer"), &LastOpenedFiles::remove_opened_file);
	//ClassDB::bind_method(D_METHOD("load_opened_files"), &LastOpenedFiles::load_opened_files);
	//ClassDB::bind_method(D_METHOD("store_editor_fonts", "file_name", "font_path"), &LastOpenedFiles::store_editor_fonts);
	//ClassDB::bind_method(D_METHOD("get_editor_font"), &LastOpenedFiles::get_editor_font);
}
