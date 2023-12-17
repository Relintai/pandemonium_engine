/*************************************************************************/
/*  editor_syntax_highlighter.cpp                                        */
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

#include "editor_syntax_highlighter.h"

#include "core/config/project_settings.h"
#include "editor/editor_settings.h"

#include "editor_script_editor.h"
/*** SYNTAX HIGHLIGHTER ****/

String EditorSyntaxHighlighter::_get_name() const {
	ScriptInstance *si = get_script_instance();
	if (si && si->has_method("_get_name")) {
		return si->call("_get_name");
	}
	return "Unnamed";
}

Array EditorSyntaxHighlighter::_get_supported_languages() const {
	ScriptInstance *si = get_script_instance();
	if (si && si->has_method("_get_supported_languages")) {
		return si->call("_get_supported_languages");
	}
	return Array();
}

Ref<EditorSyntaxHighlighter> EditorSyntaxHighlighter::_create() const {
	Ref<EditorSyntaxHighlighter> syntax_highlighter;
	syntax_highlighter.instance();
	if (get_script_instance()) {
		syntax_highlighter->set_script(get_script_instance()->get_script().get_ref_ptr());
	}
	return syntax_highlighter;
}

EditorSyntaxHighlighter::EditorSyntaxHighlighter() {
}
EditorSyntaxHighlighter::~EditorSyntaxHighlighter() {
}

void EditorSyntaxHighlighter::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_get_edited_resource"), &EditorSyntaxHighlighter::_get_edited_resource);

	BIND_VMETHOD(MethodInfo(Variant::STRING, "_get_name"));
	BIND_VMETHOD(MethodInfo(Variant::ARRAY, "_get_supported_languages"));
	BIND_VMETHOD(MethodInfo(Variant::ARRAY, "_get_supported_extentions"));
}

////

void EditorStandardSyntaxHighlighter::_update_cache() {
	highlighter->set_text_edit(text_edit);
	highlighter->clear_keyword_colors();
	highlighter->clear_member_keyword_colors();
	highlighter->clear_color_regions();

	highlighter->set_symbol_color(EDITOR_GET("text_editor/highlighting/symbol_color"));
	highlighter->set_function_color(EDITOR_GET("text_editor/highlighting/function_color"));
	highlighter->set_number_color(EDITOR_GET("text_editor/highlighting/number_color"));
	highlighter->set_member_variable_color(EDITOR_GET("text_editor/highlighting/member_variable_color"));

	/* Engine types. */
	const Color type_color = EDITOR_GET("text_editor/highlighting/engine_type_color");
	List<StringName> types;
	ClassDB::get_class_list(&types);
	for (List<StringName>::Element *E = types.front(); E; E = E->next()) {
		String n = E->get();
		if (n.begins_with("_")) {
			n = n.substr(1, n.length());
		}
		highlighter->add_keyword_color(n, type_color);
	}

	/* User types. */
	const Color usertype_color = EDITOR_GET("text_editor/highlighting/user_type_color");
	List<StringName> global_classes;
	ScriptServer::get_global_class_list(&global_classes);
	for (List<StringName>::Element *E = global_classes.front(); E; E = E->next()) {
		highlighter->add_keyword_color(E->get(), usertype_color);
	}

	/* Autoloads. */
	List<PropertyInfo> props;
	ProjectSettings::get_singleton()->get_property_list(&props);
	for (List<PropertyInfo>::Element *E = props.front(); E; E = E->next()) {
		const PropertyInfo &pi = E->get();

		if (!pi.name.begins_with("autoload/")) {
			continue;
		}

		String name = pi.name.get_slice("/", 1);
		String path = ProjectSettings::get_singleton()->get(pi.name);

		if (name.empty()) {
			continue;
		}

		bool is_singleton = path.begins_with("*");

		if (is_singleton) {
			highlighter->add_keyword_color(name, usertype_color);
		}
	}

	const Ref<Script> script = _get_edited_resource();
	if (script.is_valid()) {
		/* Core types. */
		const Color basetype_color = EDITOR_GET("text_editor/highlighting/base_type_color");
		List<String> core_types;
		script->get_language()->get_core_type_words(&core_types);
		for (List<String>::Element *E = core_types.front(); E; E = E->next()) {
			highlighter->add_keyword_color(E->get(), basetype_color);
		}

		/* Reserved words. */
		const Color keyword_color = EDITOR_GET("text_editor/highlighting/keyword_color");
		const Color control_flow_keyword_color = EDITOR_GET("text_editor/highlighting/control_flow_keyword_color");
		List<String> keywords;
		script->get_language()->get_reserved_words(&keywords);
		for (List<String>::Element *E = keywords.front(); E; E = E->next()) {
			if (script->get_language()->is_control_flow_keyword(E->get())) {
				highlighter->add_keyword_color(E->get(), control_flow_keyword_color);
			} else {
				highlighter->add_keyword_color(E->get(), keyword_color);
			}
		}

		/* Member types. */
		const Color member_variable_color = EDITOR_GET("text_editor/highlighting/member_variable_color");
		StringName instance_base = script->get_instance_base_type();
		if (instance_base != StringName()) {
			List<PropertyInfo> plist;
			ClassDB::get_property_list(instance_base, &plist);
			for (List<PropertyInfo>::Element *E = plist.front(); E; E = E->next()) {
				String name = E->get().name;
				if (E->get().usage & PROPERTY_USAGE_CATEGORY || E->get().usage & PROPERTY_USAGE_GROUP) {
					continue;
				}
				if (name.find("/") != -1) {
					continue;
				}
				highlighter->add_member_keyword_color(name, member_variable_color);
			}

			List<String> clist;
			ClassDB::get_integer_constant_list(instance_base, &clist);
			for (List<String>::Element *E = clist.front(); E; E = E->next()) {
				highlighter->add_member_keyword_color(E->get(), member_variable_color);
			}
		}

		/* Comments */
		const Color comment_color = EDITOR_GET("text_editor/highlighting/comment_color");
		List<String> comments;
		script->get_language()->get_comment_delimiters(&comments);
		for (List<String>::Element *E = comments.front(); E; E = E->next()) {
			String comment = E->get();
			String beg = comment.get_slice(" ", 0);
			String end = comment.get_slice_count(" ") > 1 ? comment.get_slice(" ", 1) : String();
			highlighter->add_color_region(beg, end, comment_color, end == "");
		}

		/* Strings */
		const Color string_color = EDITOR_GET("text_editor/highlighting/string_color");
		List<String> strings;
		script->get_language()->get_string_delimiters(&strings);
		for (List<String>::Element *E = strings.front(); E; E = E->next()) {
			String string = E->get();
			String beg = string.get_slice(" ", 0);
			String end = string.get_slice_count(" ") > 1 ? string.get_slice(" ", 1) : String();
			highlighter->add_color_region(beg, end, string_color, end == "");
		}
	}
}

Ref<EditorSyntaxHighlighter> EditorStandardSyntaxHighlighter::_create() const {
	Ref<EditorStandardSyntaxHighlighter> syntax_highlighter;
	syntax_highlighter.instance();
	return syntax_highlighter;
}

EditorStandardSyntaxHighlighter::EditorStandardSyntaxHighlighter() {
	highlighter.instance();
}
EditorStandardSyntaxHighlighter::~EditorStandardSyntaxHighlighter() {
}

void EditorStandardSyntaxHighlighter::_bind_methods() {
}

////

Ref<EditorSyntaxHighlighter> EditorPlainTextSyntaxHighlighter::_create() const {
	Ref<EditorPlainTextSyntaxHighlighter> syntax_highlighter;
	syntax_highlighter.instance();
	return syntax_highlighter;
}

EditorPlainTextSyntaxHighlighter::EditorPlainTextSyntaxHighlighter() {
}
EditorPlainTextSyntaxHighlighter::~EditorPlainTextSyntaxHighlighter() {
}

void EditorPlainTextSyntaxHighlighter::_bind_methods() {
}
