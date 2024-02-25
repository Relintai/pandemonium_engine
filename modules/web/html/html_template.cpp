/*************************************************************************/
/*  html_template.cpp                                                    */
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

#include "html_template.h"

#include "html_template_data.h"
#include "../http/web_server_request.h"

// Templates

int HTMLTemplate::get_template_count() const {
	return _templates.size();
}
Ref<HTMLTemplateData> HTMLTemplate::get_template(const int p_index) {
	ERR_FAIL_INDEX_V(p_index, _templates.size(), Ref<HTMLTemplateData>());

	return _templates[p_index];
}
void HTMLTemplate::add_template(const Ref<HTMLTemplateData> &p_template) {
	_templates.push_back(p_template);
}
void HTMLTemplate::remove_template(const int p_index) {
	ERR_FAIL_INDEX(p_index, _templates.size());

	_templates.remove(p_index);
}

void HTMLTemplate::clear_templates() {
	_templates.clear();
}

Vector<Variant> HTMLTemplate::get_templates() {
	Vector<Variant> r;
	for (int i = 0; i < _templates.size(); i++) {
		r.push_back(_templates[i].get_ref_ptr());
	}
	return r;
}
void HTMLTemplate::set_templates(const Vector<Variant> &data) {
	_templates.clear();
	for (int i = 0; i < data.size(); i++) {
		Ref<HTMLTemplateData> e = Ref<HTMLTemplateData>(data[i]);
		_templates.push_back(e);
	}
}

// Overrides

bool HTMLTemplate::has_template_override(const StringName &p_name) const {
	return _template_overrides.has(p_name);
}
String HTMLTemplate::get_template_override(const StringName &p_name) const {
	const String *val = _template_overrides.getptr(p_name);

	if (!val) {
		return String();
	}

	return *val;
}
void HTMLTemplate::set_template_override(const StringName &p_name, const String &p_value) {
	_template_overrides[p_name] = p_value;
}
void HTMLTemplate::remove_template_override(const StringName &p_name) {
	_template_overrides.erase(p_name);
}

void HTMLTemplate::clear_template_overrides() {
	_template_overrides.clear();
}

Dictionary HTMLTemplate::get_template_overrides() const {
	Dictionary ret;

	for (const HashMap<StringName, String>::Element *E = _template_overrides.front(); E; E = E->next) {
		ret[E->key()] = E->value();
	}

	return ret;
}
void HTMLTemplate::set_template_overrides(const Dictionary &p_dict) {
	get_template_overrides();

	Array keys = p_dict.keys();

	for (int i = 0; i < keys.size(); ++i) {
		Variant k = keys[i];
		Variant::Type t = k.get_type();

		if (t != Variant::STRING && t != Variant::STRING_NAME) {
			continue;
		}

		_template_overrides[k] = String(p_dict[k]);
	}
}

HashMap<StringName, String> HTMLTemplate::get_template_overrides_map() const {
	return _template_overrides;
}
void HTMLTemplate::set_template_overrides_map(const HashMap<StringName, String> &p_map) {
	_template_overrides = p_map;
}

// Defaults

bool HTMLTemplate::has_template_default(const StringName &p_name) const {
	return _template_defaults.has(p_name);
}
String HTMLTemplate::get_template_default(const StringName &p_name) const {
	const String *val = _template_defaults.getptr(p_name);

	if (!val) {
		return String();
	}

	return *val;
}
void HTMLTemplate::set_template_default(const StringName &p_name, const String &p_value) {
	_template_defaults[p_name] = p_value;
}
void HTMLTemplate::remove_template_default(const StringName &p_name) {
	_template_defaults.erase(p_name);
}

void HTMLTemplate::clear_template_defaults() {
	_template_defaults.clear();
}

Dictionary HTMLTemplate::get_template_defaults() const {
	Dictionary ret;

	for (const HashMap<StringName, String>::Element *E = _template_defaults.front(); E; E = E->next) {
		ret[E->key()] = E->value();
	}

	return ret;
}
void HTMLTemplate::set_template_defaults(const Dictionary &p_dict) {
	clear_template_defaults();

	Array keys = p_dict.keys();

	for (int i = 0; i < keys.size(); ++i) {
		Variant k = keys[i];
		Variant::Type t = k.get_type();

		if (t != Variant::STRING && t != Variant::STRING_NAME) {
			continue;
		}

		_template_defaults[k] = String(p_dict[k]);
	}
}

HashMap<StringName, String> HTMLTemplate::get_template_defaults_map() const {
	return _template_defaults;
}
void HTMLTemplate::set_template_defaults_map(const HashMap<StringName, String> &p_map) {
	_template_defaults = p_map;
}

// Use

String HTMLTemplate::substitute_data_variables(const String &p_text, const Dictionary &p_data) {
	//TODO

	return p_text;
}
String HTMLTemplate::substitute_request_variables(const String &p_text, const Ref<WebServerRequest> &p_request) {
	if (!p_request.is_valid()) {
		return p_text;
	}

	//TODO

	return p_text;
}

String HTMLTemplate::get_template_text(const StringName &p_name) {
	// First try overrides
	String *sptr = _template_overrides.getptr(p_name);

	if (sptr) {
		return *sptr;
	}

	// Go thourgh templates

	for (int i = 0; i < _templates.size(); ++i) {
		Ref<HTMLTemplateData> d = _templates[i];

		if (!d.is_valid()) {
			continue;
		}

		String r = d->get_template(p_name);

		if (!r.empty()) {
			return r;
		}
	}

	// At last try default

	sptr = _template_defaults.getptr(p_name);

	if (sptr) {
		return *sptr;
	}

	return String();
}

String HTMLTemplate::render_template(const String &p_text, const Ref<WebServerRequest> &p_request, const Dictionary &p_data) {
	String res = substitute_data_variables(p_text, p_data);

	res = substitute_request_variables(p_text, p_request);

	return res;
}

String HTMLTemplate::get_and_render_template(const StringName &p_name, const Ref<WebServerRequest> &p_request, const Dictionary &p_data) {
	String text = get_template_text(p_name);

	return render_template(text, p_request, p_data);
}

String HTMLTemplate::render(const Ref<WebServerRequest> &p_request, const Dictionary &p_data) {
	return call("_render", p_request, p_data);
}

String HTMLTemplate::_render(const Ref<WebServerRequest> &p_request, const Dictionary &p_data) {
	return "";
}

HTMLTemplate::HTMLTemplate() {
}

HTMLTemplate::~HTMLTemplate() {
}

void HTMLTemplate::_bind_methods() {
	// Templates
	ClassDB::bind_method(D_METHOD("get_template_count"), &HTMLTemplate::get_template_count);
	ClassDB::bind_method(D_METHOD("get_template", "index"), &HTMLTemplate::get_template);
	ClassDB::bind_method(D_METHOD("add_template", "template"), &HTMLTemplate::add_template);
	ClassDB::bind_method(D_METHOD("remove_template", "index"), &HTMLTemplate::remove_template);

	ClassDB::bind_method(D_METHOD("clear_templates"), &HTMLTemplate::clear_templates);

	ClassDB::bind_method(D_METHOD("get_templates"), &HTMLTemplate::get_templates);
	ClassDB::bind_method(D_METHOD("set_templates", "data"), &HTMLTemplate::set_templates);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "templates", PROPERTY_HINT_NONE, "23/20:HTMLTemplateData", PROPERTY_USAGE_DEFAULT, "HTMLTemplateData"), "set_templates", "get_templates");

	// Overrides
	ClassDB::bind_method(D_METHOD("has_template_override", "name"), &HTMLTemplate::has_template_override);
	ClassDB::bind_method(D_METHOD("get_template_override", "name"), &HTMLTemplate::get_template_override);
	ClassDB::bind_method(D_METHOD("set_template_override", "name", "value"), &HTMLTemplate::set_template_override);
	ClassDB::bind_method(D_METHOD("remove_template_override", "name"), &HTMLTemplate::remove_template_override);

	ClassDB::bind_method(D_METHOD("clear_template_overrides"), &HTMLTemplate::clear_template_overrides);

	ClassDB::bind_method(D_METHOD("get_template_overrides"), &HTMLTemplate::get_template_overrides);
	ClassDB::bind_method(D_METHOD("set_template_overrides", "dict"), &HTMLTemplate::set_template_overrides);
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "template_overrides"), "set_template_overrides", "get_template_overrides");

	// Defaults
	ClassDB::bind_method(D_METHOD("has_template_default", "name"), &HTMLTemplate::has_template_default);
	ClassDB::bind_method(D_METHOD("get_template_default", "name"), &HTMLTemplate::get_template_default);
	ClassDB::bind_method(D_METHOD("set_template_default", "name", "value"), &HTMLTemplate::set_template_default);
	ClassDB::bind_method(D_METHOD("remove_template_default", "name"), &HTMLTemplate::remove_template_default);

	ClassDB::bind_method(D_METHOD("clear_template_defaults"), &HTMLTemplate::clear_template_defaults);

	ClassDB::bind_method(D_METHOD("get_template_defaults"), &HTMLTemplate::get_template_defaults);
	ClassDB::bind_method(D_METHOD("set_template_defaults", "dict"), &HTMLTemplate::set_template_defaults);
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "template_defaults"), "set_template_defaults", "get_template_defaults");

	// Use

	ClassDB::bind_method(D_METHOD("substitute_data_variables", "text", "data"), &HTMLTemplate::substitute_data_variables);
	ClassDB::bind_method(D_METHOD("substitute_request_variables", "text", "request"), &HTMLTemplate::substitute_request_variables);

	ClassDB::bind_method(D_METHOD("get_template_text", "name"), &HTMLTemplate::get_template_text);

	ClassDB::bind_method(D_METHOD("render_template", "text", "request", "data"), &HTMLTemplate::render_template);

	ClassDB::bind_method(D_METHOD("get_and_render_template", "name", "request", "data"), &HTMLTemplate::get_and_render_template);

	BIND_VMETHOD(MethodInfo("_render",
			PropertyInfo(Variant::OBJECT, "request", PROPERTY_HINT_RESOURCE_TYPE, "WebServerRequest"),
			PropertyInfo(Variant::DICTIONARY, "data")));

	ClassDB::bind_method(D_METHOD("render", "request", "data"), &HTMLTemplate::render);
	ClassDB::bind_method(D_METHOD("_render", "request", "data"), &HTMLTemplate::_render);
}
