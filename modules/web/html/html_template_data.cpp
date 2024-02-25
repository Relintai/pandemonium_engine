/*************************************************************************/
/*  html_template_data.cpp                                               */
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

#include "html_template_data.h"

#include "core/os/file_access.h"

bool HTMLTemplateData::has_template(const StringName &p_name) const {
	return _templates.has(p_name);
}
String HTMLTemplateData::get_template(const StringName &p_name) const {
	const String *val = _templates.getptr(p_name);

	if (!val) {
		return String();
	}

	return *val;
}
void HTMLTemplateData::set_template(const StringName &p_name, const String &p_value) {
	_templates[p_name] = p_value;
}
void HTMLTemplateData::remove_template(const StringName &p_name) {
	_templates.erase(p_name);
}

Dictionary HTMLTemplateData::get_templates() const {
	Dictionary ret;

	for (const HashMap<StringName, String>::Element *E = _templates.front(); E; E = E->next) {
		ret[E->key()] = E->value();
	}

	return ret;
}
void HTMLTemplateData::set_templates(const Dictionary &p_dict) {
}

HashMap<StringName, String> HTMLTemplateData::get_templates_map() const {
	return _templates;
}
void HTMLTemplateData::set_templates_map(const HashMap<StringName, String> &p_map) {
	_templates = p_map;
}

void HTMLTemplateData::clear() {
	_templates.clear();
}

Error HTMLTemplateData::load_from_file(const String &p_file) {
	clear();

	Error err;
	FileAccess *f = FileAccess::open(p_file, FileAccess::READ, &err);

	if (err != OK) {
		if (f) {
			memdelete(f);
		}

		return err;
	}

	String data = f->get_as_utf8_string(true);

	f->close();
	memdelete(f);

	load_from_string(data);

	return OK;
}
Error HTMLTemplateData::save_to_file(const String &p_file) const {
	Error err;
	FileAccess *f = FileAccess::open(p_file, FileAccess::WRITE, &err);

	if (err != OK) {
		if (f) {
			memdelete(f);
		}

		return err;
	}

	String data = save_as_string();

	f->store_string(data);
	f->close();

	memdelete(f);

	return OK;
}
void HTMLTemplateData::load_from_string(const String &p_data) {
	clear();
	
	Vector<String> lines = p_data.split("\n", false);
	
	String current_section_name;
	String current_str;
	for (int i = 0; i < lines.size(); ++i) {
		// Section header: [ Section Name ]
		// Should not have whitespace in front and back
		String l = lines[i];
		
		if (l.begins_with("[") && l.ends_with("]")) {
			if (!current_section_name.empty()) {
				_templates[current_section_name] = current_str;
			}
			
			// Remove [ and ], and strip it.
			current_section_name = l.substr_index(1, l.length() - 1).strip_edges();
			current_str = "";
			continue;
		}
		
		current_str += l + "\n";
	}
	
	if (!current_section_name.empty()) {
		_templates[current_section_name] = current_str;
	}
}
String HTMLTemplateData::save_as_string() const {
	String data;
	
	for (const HashMap<StringName, String>::Element *E = _templates.front(); E; E = E->next) {
		data += "[ " + E->key() + " ]\n\n";
		data +=  E->value();
		data += "\n\n";
	}

	return data;
}

HTMLTemplateData::HTMLTemplateData() {
}

HTMLTemplateData::~HTMLTemplateData() {
}

void HTMLTemplateData::_bind_methods() {
	ClassDB::bind_method(D_METHOD("has_template", "name"), &HTMLTemplateData::has_template);
	ClassDB::bind_method(D_METHOD("get_template", "name"), &HTMLTemplateData::get_template);
	ClassDB::bind_method(D_METHOD("set_template", "name", "value"), &HTMLTemplateData::set_template);
	ClassDB::bind_method(D_METHOD("remove_template", "name"), &HTMLTemplateData::remove_template);

	ClassDB::bind_method(D_METHOD("get_templates"), &HTMLTemplateData::get_templates);
	ClassDB::bind_method(D_METHOD("set_templates", "dict"), &HTMLTemplateData::set_templates);
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "templates"), "set_templates", "get_templates");

	ClassDB::bind_method(D_METHOD("clear"), &HTMLTemplateData::clear);

	ClassDB::bind_method(D_METHOD("load_from_file", "file"), &HTMLTemplateData::load_from_file);
	ClassDB::bind_method(D_METHOD("save_to_file", "file"), &HTMLTemplateData::save_to_file);

	ClassDB::bind_method(D_METHOD("load_from_string", "data"), &HTMLTemplateData::load_from_string);
	ClassDB::bind_method(D_METHOD("save_as_string"), &HTMLTemplateData::save_as_string);
}
