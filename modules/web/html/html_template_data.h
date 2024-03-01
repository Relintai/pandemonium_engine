#ifndef HTML_TEMPLATE_DATA_H
#define HTML_TEMPLATE_DATA_H

/*************************************************************************/
/*  html_template_data.h                                                 */
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

#include "core/containers/hash_map.h"
#include "core/string/string_name.h"
#include "core/string/ustring.h"
#include "core/variant/dictionary.h"

#include "core/object/resource.h"

class HTMLTemplateData : public Resource {
	GDCLASS(HTMLTemplateData, Resource);

public:
	bool has_template(const StringName &p_name) const;
	String get_template(const StringName &p_name) const;
	void set_template(const StringName &p_name, const String &p_value);
	void remove_template(const StringName &p_name);

	Dictionary get_templates() const;
	void set_templates(const Dictionary &p_dict);

	HashMap<StringName, String> get_templates_map() const;
	void set_templates_map(const HashMap<StringName, String> &p_map);

	void clear();

	Error load_from_file(const String &p_file);
	Error save_to_file(const String &p_file) const;

	void load_from_string(const String &p_data);
	String save_as_string() const;

	HTMLTemplateData();
	~HTMLTemplateData();

protected:
	static void _bind_methods();

	HashMap<StringName, String> _templates;
};

#endif
