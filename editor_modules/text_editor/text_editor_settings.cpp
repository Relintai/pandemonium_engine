/*************************************************************************/
/*  text_editor_settings.cpp                                             */
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

#include "text_editor_settings.h"

#include "editor/editor_settings.h"

#include "core/variant/variant.h"
#include "scene/gui/item_list.h"
#include "text_editor_vanilla_editor.h"

void TextEditorSettings::store_opened_files(ItemList *filecontainer) {
	ERR_FAIL_COND(!filecontainer);

	Array arr;

	for (int child = 0; child < filecontainer->get_item_count(); ++child) {
		Array metaarr = filecontainer->get_item_metadata(child);
		ObjectID editor = metaarr[0];
		TextEditorVanillaEditor *ed = Object::cast_to<TextEditorVanillaEditor>(ObjectDB::get_instance(editor));

		ERR_CONTINUE(!ed);

		String filepath = ed->get_current_path();
		Array a = Array();
		a.push_back(filepath.get_file());
		a.push_back(filepath);
		arr.push_back(a);
	}

	EditorSettings::get_singleton()->set_project_metadata("file_editor", "files", arr);
}

void TextEditorSettings::remove_opened_file(const int index, ItemList *filecontainer) {
	Array metaarr = filecontainer->get_item_metadata(index);
	ObjectID editor = metaarr[0];
	TextEditorVanillaEditor *ed = Object::cast_to<TextEditorVanillaEditor>(ObjectDB::get_instance(editor));

	ERR_FAIL_COND(!ed);

	String filepath = ed->get_current_path();
	String f = filepath.get_file();
	Array arr = EditorSettings::get_singleton()->get_project_metadata("file_editor", "files", Array());

	for (int i = 0; i < arr.size(); ++i) {
		Array a = arr[i];

		if (a.size() < 2) {
			continue;
		}

		if (a[0] == f) {
			arr.remove(i);
			break;
		}
	}

	EditorSettings::get_singleton()->set_project_metadata("file_editor", "files", arr);

	/*
	Dictionary fonts_dict = EditorSettings::get_singleton()->get_project_metadata("file_editor", "file_fonts", Dictionary());
	
	if (fonts_dict.has(f)) {
		fonts_dict.erase(f);
		EditorSettings::get_singleton()->set_project_metadata("file_editor", "file_fonts", fonts_dict);
	}
	*/
}

Array TextEditorSettings::load_opened_files() {
	Array arr = EditorSettings::get_singleton()->get_project_metadata("file_editor", "files", Array());
	Dictionary fonts_dict = EditorSettings::get_singleton()->get_project_metadata("file_editor", "file_fonts", Dictionary());
	Array keys;

	for (int i = 0; i < arr.size(); ++i) {
		Array a = arr[i];

		if (a.size() < 2) {
			continue;
		}

		// creating and returning an Array with this format [1:file name, 2:file path, 3:file font];
		Array k;
		k.push_back(a[0]);
		k.push_back(a[1]);

		/*
		if (fonts_dict.has(a[2])) {
			k.push_back(fonts_dict[a[2]]);
		} else {
			k.push_back("null");
		}
		*/

		keys.append(k);
	}

	return keys;
}

void TextEditorSettings::store_editor_fonts(const String &file_name, const String &font_path) {
	Dictionary fonts_dict = EditorSettings::get_singleton()->get_project_metadata("file_editor", "file_fonts", Dictionary());
	fonts_dict[file_name] = font_path;
	EditorSettings::get_singleton()->set_project_metadata("file_editor", "file_fonts", fonts_dict);
}

String TextEditorSettings::get_editor_font() {
	return EditorSettings::get_singleton()->get_setting("interface/editor/code_font");
}

TextEditorSettings::TextEditorSettings() {
}

TextEditorSettings::~TextEditorSettings() {
}

void TextEditorSettings::_bind_methods() {
	//ClassDB::bind_method(D_METHOD("store_opened_files", "filecontainer"), &TextEditorSettings::store_opened_files);
	//ClassDB::bind_method(D_METHOD("remove_opened_file", "index", "filecontainer"), &TextEditorSettings::remove_opened_file);
	//ClassDB::bind_method(D_METHOD("load_opened_files"), &TextEditorSettings::load_opened_files);
	//ClassDB::bind_method(D_METHOD("store_editor_fonts", "file_name", "font_path"), &TextEditorSettings::store_editor_fonts);
	//ClassDB::bind_method(D_METHOD("get_editor_font"), &TextEditorSettings::get_editor_font);
}
