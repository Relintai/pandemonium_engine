#ifndef HTML_TEMPLATE_H
#define HTML_TEMPLATE_H

/*************************************************************************/
/*  html_template.h                                                      */
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

#include "core/object/resource.h"

#include "core/containers/hash_map.h"
#include "core/containers/vector.h"
#include "core/string/string_name.h"
#include "core/string/ustring.h"

class HTMLTemplateData;
class WebServerRequest;
class HTMLTemplateRenderer;

class HTMLTemplate : public Resource {
	GDCLASS(HTMLTemplate, Resource);

public:
	// Templates
	int get_template_count() const;
	Ref<HTMLTemplateData> get_template(const int p_index);
	void add_template(const Ref<HTMLTemplateData> &p_template);
	void remove_template(const int p_index);

	void clear_templates();

	Vector<Variant> get_templates();
	void set_templates(const Vector<Variant> &data);

	// Overrides
	bool has_template_override(const StringName &p_name) const;
	String get_template_override(const StringName &p_name) const;
	void set_template_override(const StringName &p_name, const String &p_value);
	void remove_template_override(const StringName &p_name);

	Ref<HTMLTemplateRenderer> get_template_override_renderer(const StringName &p_name) const;

	void clear_template_overrides();

	Dictionary get_template_overrides() const;
	void set_template_overrides(const Dictionary &p_dict);

	// Defaults
	bool has_template_default(const StringName &p_name) const;
	String get_template_default(const StringName &p_name) const;
	void set_template_default(const StringName &p_name, const String &p_value);
	void remove_template_default(const StringName &p_name);

	Ref<HTMLTemplateRenderer> get_template_default_renderer(const StringName &p_name) const;

	void clear_template_defaults();

	Dictionary get_template_defaults() const;
	void set_template_defaults(const Dictionary &p_dict);

	// Use

	String get_template_text(const StringName &p_name);
	Ref<HTMLTemplateRenderer> get_template_renderer(const StringName &p_name);

	String render_template(const String &p_text, const Dictionary &p_data);
	String renderer_render_template(Ref<HTMLTemplateRenderer> p_renderer, const Dictionary &p_data);

	String get_and_render_template(const StringName &p_name, const Dictionary &p_data);

	String render(const Ref<WebServerRequest> &p_request, const Dictionary &p_data);
	virtual String _render(const Ref<WebServerRequest> &p_request, const Dictionary &p_data);

	HTMLTemplate();
	~HTMLTemplate();

protected:
	void _on_editor_template_button_pressed(const StringName &p_property);

	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;

	static void _bind_methods();

	Vector<Ref<HTMLTemplateData>> _templates;

	HashMap<StringName, String> _template_overrides;
	HashMap<StringName, String> _template_defaults;

	HashMap<StringName, Ref<HTMLTemplateRenderer>> _template_override_renderers;
	HashMap<StringName, Ref<HTMLTemplateRenderer>> _template_default_renderers;

	String _editor_new_template_override_key;
	String _editor_new_template_default_key;
};

#endif
