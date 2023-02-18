#ifndef EDITOR_SCRIPT_EDITOR_BASE_H
#define EDITOR_SCRIPT_EDITOR_BASE_H

/*************************************************************************/
/*  script_editor_plugin.h                                               */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
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

#include "scene/gui/box_container.h"

class SyntaxHighlighter;
class Texture;

class EditorScriptEditorBase : public VBoxContainer {
	GDCLASS(EditorScriptEditorBase, VBoxContainer);

protected:
	static void _bind_methods();

public:
	virtual void add_syntax_highlighter(SyntaxHighlighter *p_highlighter) = 0;
	virtual void set_syntax_highlighter(SyntaxHighlighter *p_highlighter) = 0;

	virtual void apply_code() = 0;
	virtual RES get_edited_resource() const = 0;
	virtual Vector<String> get_functions() = 0;
	virtual void set_edited_resource(const RES &p_res) = 0;
	virtual void enable_editor() = 0;
	virtual void reload_text() = 0;
	virtual String get_name() = 0;
	virtual Ref<Texture> get_icon() = 0;
	virtual bool is_unsaved() = 0;
	virtual Variant get_edit_state() = 0;
	virtual void set_edit_state(const Variant &p_state) = 0;
	virtual void goto_line(int p_line, bool p_with_error = false) = 0;
	virtual void set_executing_line(int p_line) = 0;
	virtual void clear_executing_line() = 0;
	virtual void trim_trailing_whitespace() = 0;
	virtual void insert_final_newline() = 0;
	virtual void convert_indent_to_spaces() = 0;
	virtual void convert_indent_to_tabs() = 0;
	virtual void ensure_focus() = 0;
	virtual void tag_saved_version() = 0;
	virtual void reload(bool p_soft) {}
	virtual void get_breakpoints(List<int> *p_breakpoints) = 0;
	virtual void add_callback(const String &p_function, PoolStringArray p_args) = 0;
	virtual void update_settings() = 0;
	virtual void set_debugger_active(bool p_active) = 0;
	virtual bool can_lose_focus_on_node_selection() { return true; }

	virtual bool show_members_overview() = 0;

	virtual void set_tooltip_request_func(String p_method, Object *p_obj) = 0;
	virtual Control *get_edit_menu() = 0;
	virtual void clear_edit_menu() = 0;

	virtual void validate() = 0;

	EditorScriptEditorBase() {}
};

#endif // SCRIPT_EDITOR_PLUGIN_H
