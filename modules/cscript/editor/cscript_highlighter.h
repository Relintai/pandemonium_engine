#ifndef CSCRIPT_HIGHLIGHTER_H
#define CSCRIPT_HIGHLIGHTER_H

/*************************************************************************/
/*  cscript_highlighter.h                                                */
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

#include "editor_modules/editor_code_editor/editor_syntax_highlighter.h"
#include "scene/gui/text_edit.h"

class CScriptSyntaxHighlighter : public EditorSyntaxHighlighter {
	GDCLASS(CScriptSyntaxHighlighter, EditorSyntaxHighlighter);

private:
	struct ColorRegion {
		Color color;
		String start_key;
		String end_key;
		bool line_only;
	};
	Vector<ColorRegion> color_regions;
	RBMap<int, int> color_region_cache;

	Dictionary keywords;
	Dictionary member_keywords;

	enum Type {
		NONE,
		REGION,
		NODE_PATH,
		SYMBOL,
		NUMBER,
		FUNCTION,
		KEYWORD,
		MEMBER,
		IDENTIFIER,
		TYPE,
	};

	// colours
	Color font_color;
	Color symbol_color;
	Color function_color;
	Color function_definition_color;
	Color built_in_type_color;
	Color number_color;
	Color member_color;
	Color node_path_color;
	Color type_color;

	void add_color_region(const String &p_start_key, const String &p_end_key, const Color &p_color, bool p_line_only = false);

public:
	virtual Ref<EditorSyntaxHighlighter> _create() const;

	virtual void _update_cache();
	virtual Dictionary _get_line_syntax_highlighting(int p_line);

	virtual String _get_name() const;
	virtual Array _get_supported_languages() const;
};

#endif // CSCRIPT_HIGHLIGHTER_H
