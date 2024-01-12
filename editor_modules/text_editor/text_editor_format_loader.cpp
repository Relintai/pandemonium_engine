/*************************************************************************/
/*  text_editor_format_loader.cpp                                        */
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

#include "text_editor_format_loader.h"

#include "text_editor_file.h"

RES TextEditorTextLoader::load(const String &p_path, const String &p_original_path, Error *r_error, bool p_no_subresource_cache) {
	if (r_error) {
		*r_error = ERR_FILE_CANT_OPEN;
	}

	Ref<TextEditorFile> file;
	file.instance();
	file->set_file_path(p_original_path);

	if (r_error) {
		*r_error = OK;
	}

	return file;
}

void TextEditorTextLoader::get_recognized_extensions(List<String> *p_extensions) const {
	p_extensions->push_back("txt");
	p_extensions->push_back("md");
	p_extensions->push_back("xml");
	p_extensions->push_back("sql");
	p_extensions->push_back("csv");
	p_extensions->push_back("cfg");
	p_extensions->push_back("ini");
	p_extensions->push_back("log");
	p_extensions->push_back("json");
	p_extensions->push_back("yml");
	p_extensions->push_back("yaml");
	p_extensions->push_back("toml");
	p_extensions->push_back("html");
	p_extensions->push_back("htm");
	p_extensions->push_back("js");
	p_extensions->push_back("css");
	p_extensions->push_back("htmpl");
	p_extensions->push_back("sh");
	p_extensions->push_back("bat");
	p_extensions->push_back("ps1");
	p_extensions->push_back("pxml"); // can be used as a normal easily editable non-translation xml
}

bool TextEditorTextLoader::handles_type(const String &p_type) const {
	return p_type == "TextEditorFile";
}

String TextEditorTextLoader::get_resource_type(const String &p_path) const {
	return "TextEditorFile";
}

void TextEditorTextLoader::get_dependencies(const String &p_path, List<String> *p_dependencies, bool p_add_types) {
}
