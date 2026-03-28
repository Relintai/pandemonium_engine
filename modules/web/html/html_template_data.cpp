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

#include "html_template_renderer.h"

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

	Ref<HTMLTemplateRenderer> renderer;
	renderer.instance();
	renderer->compile(p_value);
	_template_renderers[p_name] = renderer;

	emit_changed();
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

Ref<HTMLTemplateRenderer> HTMLTemplateData::get_template_renderer(const StringName &p_name) const {
	const Ref<HTMLTemplateRenderer> *val = _template_renderers.getptr(p_name);

	if (!val) {
		return Ref<HTMLTemplateRenderer>();
	}

	return *val;
}

void HTMLTemplateData::set_templates(const Dictionary &p_dict) {
	_templates.clear();
	_template_renderers.clear();

	Array keys = p_dict.keys();

	for (int i = 0; i < keys.size(); ++i) {
		Variant k = keys[i];
		Variant::Type t = k.get_type();

		if (t != Variant::STRING && t != Variant::STRING_NAME) {
			continue;
		}

		String data = String(p_dict[k]);

		_templates[k] = data;

		Ref<HTMLTemplateRenderer> renderer;
		renderer.instance();
		renderer->compile(data);
		_template_renderers[k] = renderer;
	}

	emit_changed();
}

void HTMLTemplateData::clear() {
	_templates.clear();

	emit_changed();
}

Error HTMLTemplateData::load_from_file(const String &p_file) {
	_templates.clear();

	Error err;
	FileAccess *f = FileAccess::open(p_file, FileAccess::READ, &err);

	if (err != OK) {
		if (f) {
			memdelete(f);
		}

		emit_changed();

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
	_templates.clear();
	_template_renderers.clear();

	Vector<String> lines = p_data.split("\n", false);

	String current_section_name;
	String current_str;
	int section_line_start = 0;
	for (int i = 0; i < lines.size(); ++i) {
		// Section header: [ Section Name ]
		// Should not have whitespace in front and back
		String l = lines[i];

		if (l.begins_with("[") && l.ends_with("]")) {
			if (!current_section_name.empty()) {
				StringName csnsn = current_section_name;

				_templates[csnsn] = current_str;

				Ref<HTMLTemplateRenderer> renderer;
				renderer.instance();
				renderer->compile(current_str, section_line_start);
				_template_renderers[csnsn] = renderer;
			}

			// Remove [ and ], and strip it.
			current_section_name = l.substr_index(1, l.length() - 2).strip_edges();
			current_str = "";
			// We need to start after the header, with 1 based indexing
			section_line_start = i + 2;
			continue;
		}

		current_str += l + "\n";
	}

	if (!current_section_name.empty()) {
		_templates[current_section_name] = current_str;
	}

	emit_changed();
}
String HTMLTemplateData::save_as_string() const {
	String data;

	for (const HashMap<StringName, String>::Element *E = _templates.front(); E; E = E->next) {
		data += "[ " + E->key() + " ]\n\n";
		data += E->value();
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

	ClassDB::bind_method(D_METHOD("get_template_renderer", "name"), &HTMLTemplateData::get_template_renderer);

	ClassDB::bind_method(D_METHOD("get_templates"), &HTMLTemplateData::get_templates);
	ClassDB::bind_method(D_METHOD("set_templates", "dict"), &HTMLTemplateData::set_templates);
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "templates"), "set_templates", "get_templates");

	ClassDB::bind_method(D_METHOD("clear"), &HTMLTemplateData::clear);

	ClassDB::bind_method(D_METHOD("load_from_file", "file"), &HTMLTemplateData::load_from_file);
	ClassDB::bind_method(D_METHOD("save_to_file", "file"), &HTMLTemplateData::save_to_file);

	ClassDB::bind_method(D_METHOD("load_from_string", "data"), &HTMLTemplateData::load_from_string);
	ClassDB::bind_method(D_METHOD("save_as_string"), &HTMLTemplateData::save_as_string);
}
