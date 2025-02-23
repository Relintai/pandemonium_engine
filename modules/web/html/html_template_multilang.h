#ifndef HTML_TEMPLATE_MULTILANG_H
#define HTML_TEMPLATE_MULTILANG_H

/*************************************************************************/
/*  html_template_multilang.h                                            */
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

class HTMLTemplateMultilang : public HTMLTemplate {
	GDCLASS(HTMLTemplateMultilang, HTMLTemplate);

public:
	Ref<HTMLTemplate> get_template_for_locale(const StringName &p_locale);
	void set_template_for_locale(const StringName &p_locale, const Ref<HTMLTemplate> &p_template);

	HashMap<StringName, Ref<HTMLTemplate>> get_all_locales() const;

	Dictionary get_locales() const;
	void set_locales(const Dictionary &p_dict);

	virtual String _render(const Ref<WebServerRequest> &p_request, const Dictionary &p_data);

	HTMLTemplateMultilang();
	~HTMLTemplateMultilang();

protected:
	void _on_editor_new_locale_button_pressed(const StringName &p_property);

	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;

	static void _bind_methods();

	HashMap<StringName, Ref<HTMLTemplate>> _locales;

	String _editor_new_locale_key;
};

#endif
