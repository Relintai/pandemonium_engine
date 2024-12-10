/*************************************************************************/
/*  html_template_multilang.cpp                                          */
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

#include "html_template_multilang.h"
#include "core/object/object.h"

Ref<HTMLTemplate> HTMLTemplateMultilang::get_template_for_locale(const StringName &p_locale) {
	Ref<HTMLTemplate> *t = _locales.getptr(p_locale);
	Ref<HTMLTemplate> ht;

	if (t) {
		ht = *t;
	}

	return ht;
}
void HTMLTemplateMultilang::set_template_for_locale(const StringName &p_locale, const Ref<HTMLTemplate> &p_template) {
	if (!p_template.is_valid()) {
		_locales.erase(p_locale);
		return;
	}

	_locales[p_locale] = p_template;
}

HashMap<StringName, Ref<HTMLTemplate>> HTMLTemplateMultilang::get_all_locales() const {
	return _locales;
}

Dictionary HTMLTemplateMultilang::get_locales() const {
	Dictionary d;

	for (const HashMap<StringName, Ref<HTMLTemplate>>::Element *E = _locales.front(); E; E = E->next) {
		d[E->key()] = E->value();
	}

	return d;
}
void HTMLTemplateMultilang::set_locales(const Dictionary &p_dict) {
	_locales.clear();

	const Variant *next = p_dict.next();

	while (next) {
		Variant n = *next;

		_locales[n] = p_dict[n];

		next = p_dict.next(next);
	}
}

String HTMLTemplateMultilang::_render(const Ref<WebServerRequest> &p_request, const Dictionary &p_data) {
	const Variant *pp = p_data.getptr("locale");
	if (pp) {
		StringName locale = *pp;

		Ref<HTMLTemplate> *tp = _locales.getptr(locale);

		if (tp) {
			return (*tp)->render(p_request, p_data);
		}
	}

	return HTMLTemplate::_render(p_request, p_data);
}

HTMLTemplateMultilang::HTMLTemplateMultilang() {
}

HTMLTemplateMultilang::~HTMLTemplateMultilang() {
}

void HTMLTemplateMultilang::_on_editor_new_locale_button_pressed(const StringName &p_property) {
	String name = p_property;

	if (name.begins_with("locales/")) {
		int scount = name.get_slice_count("/");

		String key = name.get_slicec('/', 1);

		if (scount == 2) {
			// This way add_key can also be used as a key
			if (key == "add_key_button") {
				if (!_editor_new_locale_key.empty()) {
					_locales[_editor_new_locale_key] = Ref<HTMLTemplate>();

					_editor_new_locale_key = "";

					property_list_changed_notify();
				}
			}

			return;
		}

		String property = name.get_slicec('/', 2);

		if (property == "delete_key_button") {
			_locales.erase(key);
			property_list_changed_notify();
		}

		return;
	}
}

bool HTMLTemplateMultilang::_set(const StringName &p_name, const Variant &p_value) {
	String name = p_name;

	if (name.begins_with("locales/")) {
		int scount = name.get_slice_count("/");

		String key = name.get_slicec('/', 1);

		if (scount == 2) {
			// This way add_key can also be used as a key
			if (key == "add_key") {
				_editor_new_locale_key = p_value;
				return true;
			} else if (key == "add_key_button") {
				return true;
			}
		}

		String property = name.get_slicec('/', 2);

		if (property == "value") {
			_locales[key] = p_value;
		}

		return true;
	}

	return false;
}

bool HTMLTemplateMultilang::_get(const StringName &p_name, Variant &r_ret) const {
	String name = p_name;

	if (name.begins_with("locales/")) {
		int scount = name.get_slice_count("/");

		String key = name.get_slicec('/', 1);

		if (scount == 2) {
			// This way add_key can also be used as a key
			if (key == "add_key") {
				r_ret = _editor_new_locale_key;
				return true;
			}
		}

		String property = name.get_slicec('/', 2);

		if (property == "value") {
			if (!_locales.has(key)) {
				return false;
			}

			r_ret = _locales[key];
			return true;
		}

		return true;
	}

	return false;
}

void HTMLTemplateMultilang::_get_property_list(List<PropertyInfo> *p_list) const {
	for (const HashMap<StringName, Ref<HTMLTemplate>>::Element *E = _locales.front(); E; E = E->next) {
		p_list->push_back(PropertyInfo(Variant::OBJECT, "locales/" + E->key() + "/value", PROPERTY_HINT_RESOURCE_TYPE, "HTMLTemplate"));
		p_list->push_back(PropertyInfo(Variant::NIL, "locales/" + E->key() + "/delete_key_button", PROPERTY_HINT_BUTTON, "_on_editor_new_locale_button_pressed:Close/EditorIcons"));
	}

	p_list->push_back(PropertyInfo(Variant::STRING, "locales/add_key"));
	p_list->push_back(PropertyInfo(Variant::NIL, "locales/add_key_button", PROPERTY_HINT_BUTTON, "_on_editor_new_locale_button_pressed:Add/EditorIcons"));
}

void HTMLTemplateMultilang::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_on_editor_new_locale_button_pressed"), &HTMLTemplateMultilang::_on_editor_new_locale_button_pressed);
}
