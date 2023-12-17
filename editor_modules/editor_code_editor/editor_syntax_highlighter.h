#ifndef EDITOR_SYNTAX_HIGHLIGHTER_H
#define EDITOR_SYNTAX_HIGHLIGHTER_H

/*************************************************************************/
/*  editor_syntax_highlighter.h                                          */
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

#include "editor/editor_plugin.h"

#include "scene/gui/resources/syntax_highlighter.h"

class EditorSyntaxHighlighter : public SyntaxHighlighter {
	GDCLASS(EditorSyntaxHighlighter, SyntaxHighlighter)

public:
	virtual String _get_name() const;
	virtual Array _get_supported_languages() const;

	void _set_edited_resource(const RES &p_res) { edited_resourse = p_res; }
	REF _get_edited_resource() { return edited_resourse; }

	virtual Ref<EditorSyntaxHighlighter> _create() const;

	EditorSyntaxHighlighter();
	~EditorSyntaxHighlighter();

protected:
	static void _bind_methods();

private:
	REF edited_resourse;
};

class EditorStandardSyntaxHighlighter : public EditorSyntaxHighlighter {
	GDCLASS(EditorStandardSyntaxHighlighter, EditorSyntaxHighlighter)

public:
	virtual void _update_cache();
	virtual Dictionary _get_line_syntax_highlighting(int p_line) { return highlighter->get_line_syntax_highlighting(p_line); }

	virtual String _get_name() const { return TTR("Standard"); }

	virtual Ref<EditorSyntaxHighlighter> _create() const;

	EditorStandardSyntaxHighlighter();
	~EditorStandardSyntaxHighlighter();

protected:
	static void _bind_methods();

private:
	Ref<CodeHighlighter> highlighter;
};

class EditorPlainTextSyntaxHighlighter : public EditorSyntaxHighlighter {
	GDCLASS(EditorPlainTextSyntaxHighlighter, EditorSyntaxHighlighter)

public:
	virtual String _get_name() const { return TTR("Plain Text"); }

	virtual Ref<EditorSyntaxHighlighter> _create() const;

	EditorPlainTextSyntaxHighlighter();
	~EditorPlainTextSyntaxHighlighter();

protected:
	static void _bind_methods();
};

#endif // SCRIPT_EDITOR_PLUGIN_H
