#ifndef MARKDOWN_RENDERER_H
#define MARKDOWN_RENDERER_H

/*************************************************************************/
/*  markdown_renderer.h                                                  */
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

#include "core/string/ustring.h"

#include "core/object/reference.h"

#include "libs/hoedown/document.h"

// So there is no need to add ~20 virtual methods
class MarkdownRendererCustomRendererCallback : public Reference {
	GDCLASS(MarkdownRendererCustomRendererCallback, Reference);

public:
	enum CallbackType {
		CALLBACK_BLOCKCODE = 0,
		CALLBACK_BLOCKQUOTE,
		CALLBACK_HEADER,
		CALLBACK_HRULE,
		CALLBACK_LIST,
		CALLBACK_LISTITEM,
		CALLBACK_PARAGRAPH,
		CALLBACK_TABLE,
		CALLBACK_TABLE_HEADER,
		CALLBACK_TABLE_BODY,
		CALLBACK_TABLE_ROW,
		CALLBACK_TABLE_CELL,
		CALLBACK_FOOTNOTES,
		CALLBACK_FOOTNOTE_DEF,
		CALLBACK_BLOCKHTML,

		CALLBACK_AUTOLINK,
		CALLBACK_CODESPAN,
		CALLBACK_DOUBLE_EMPHASIS,
		CALLBACK_EMPHASIS,
		CALLBACK_UNDERLINE,
		CALLBACK_HIGHLIGHT,
		CALLBACK_QUOTE,
		CALLBACK_IMAGE,
		CALLBACK_LINEBREAK,
		CALLBACK_LINK,

		CALLBACK_TRIPLE_EMPHASIS,
		CALLBACK_STRIKETHROUGH,
		CALLBACK_SUPERSCRIPT,
		CALLBACK_FOOTNOTE_REF,
		CALLBACK_MATH,
		CALLBACK_RAW_HTML,

		CALLBACK_ENTITY,
		CALLBACK_NORMAL_TEXT,

		CALLBACK_DOC_HEADER,
		CALLBACK_DOC_FOOTER,
	};

	enum ListFlags {
		LIST_FLAG_ORDERED = (1 << 0),
		LIST_FLAG_LI_BLOCK = (1 << 1)
	};

	enum TableFlags {
		TABLE_FLAG_ALIGN_LEFT = 1,
		TABLE_FLAG_ALIGN_RIGHT = 2,
		TABLE_FLAG_ALIGN_CENTER = 3,
		TABLE_FLAG_ALIGNMASK = 3,
		TABLE_FLAG_HEADER = 4
	};

	enum AutolinkType {
		AUTOLINK_TYPE_NONE = 0, /* used internally when it is not an autolink*/
		AUTOLINK_TYPE_NORMAL, /* normal http/http/ftp/mailto/etc link */
		AUTOLINK_TYPE_EMAIL /* e-mail link without explit mailto: */
	};

	CallbackType get_callback_type() const;
	void set_callback_type(const CallbackType val);

	String get_text() const;
	void set_text(const String &val);

	String get_lang() const;
	void set_lang(const String &val);

	String get_content() const;
	void set_content(const String &val);

	int get_level() const;
	void set_level(const int val);

	int get_list_flags() const;
	void set_list_flags(const int val);

	int get_table_flags() const;
	void set_table_flags(const int val);

	String get_link() const;
	void set_link(const String &val);

	AutolinkType get_auto_link_type() const;
	void set_auto_link_type(const AutolinkType val);

	String get_title() const;
	void set_title(const String &val);

	String get_alt() const;
	void set_alt(const String &val);

	uint32_t get_num() const;
	void set_num(const uint32_t val);

	int get_display_mode() const;
	void set_display_mode(const int val);

	int get_inline_render() const;
	void set_inline_render(const int val);

	String get_result() const;
	void set_result(const String &val);

	int get_result_code() const;
	void set_result_code(const int val);

	MarkdownRendererCustomRendererCallback();
	virtual ~MarkdownRendererCustomRendererCallback();

protected:
	static void _bind_methods();

protected:
	CallbackType _callback_type;

	String _text;
	String _lang;
	String _content;
	int _level;
	int _list_flags;
	int _table_flags;
	String _link;
	AutolinkType _auto_link_type;
	String _title;
	String _alt;
	uint32_t _num;
	int _display_mode;
	int _inline_render;

	String _result;
	int _result_code;
};

class MarkdownRenderer : public Reference {
	GDCLASS(MarkdownRenderer, Reference);

public:
	enum RenderType {
		RENDER_TYPE_HTML = 0,
		RENDER_TYPE_HTML_TOC = 1,
		RENDERER_TYPE_CUSTOM = 2,
	};

	enum HTMLFlags {
		HTML_FLAG_SKIP_HTML = 1 << 0,
		HTML_FLAG_ESCAPE = 1 << 1,
		HTML_FLAG_HARD_WRAP = 1 << 2,
		HTML_FLAG_USE_XHTML = 1 << 3,
	};

	enum MarkdownExtensions {
		EXTENSION_TABLES = 1 << 0,
		EXTENSION_FENCED_CODE = 1 << 1,
		EXTENSION_FOOTNOTES = 1 << 2,

		EXTENSION_AUTOLINK = 1 << 3,
		EXTENSION_STRKIETHROUGH = 1 << 4,
		EXTENSION_UNDERLINE = 1 << 5,
		EXTENSION_HIGHLIGHT = 1 << 6,
		EXTENSION_QUOTE = 1 << 7,
		EXTENSION_SUPERSCRIPT = 1 << 8,
		EXTENSION_MATH = 1 << 9,

		EXTENSION_NO_INTRA_EMPHASIS = 1 << 10,
		EXTENSION_SPACE_HEADERS = 1 << 11,
		EXTENSION_MATH_EXPLICIT = 1 << 12,

		EXTENSION_DISABLE_INDENTED_CODE = 1 << 13,
	};

public:
	RenderType get_render_type();
	void set_render_type(const RenderType val);

	int get_html_flags();
	void set_html_flags(const int val);

	int get_markdown_extensions();
	void set_markdown_extensions(const int val);

	int get_max_nesting();
	void set_max_nesting(const int val);

	bool get_use_smartypants();
	void set_use_smartypants(const bool val);

	int get_toc_level();
	void set_toc_level(const int val);

	String render_to_html(const String &markdown);
	String render(const String &markdown);

	MarkdownRenderer();
	virtual ~MarkdownRenderer();

protected:
	void renderer_callback(Ref<MarkdownRendererCustomRendererCallback> data);
	virtual void _renderer_callback(Ref<MarkdownRendererCustomRendererCallback> data);

	static void _bind_methods();

	RenderType _render_type;
	int _html_flags;
	int _markdown_extensions;
	int _max_nesting;
	int _toc_level;
	bool _use_smartypants;

protected:
	struct hoedown_custom_renderer_state {
		void *self;
	};

	/* block level callbacks - NULL skips the block */
	static void hoedown_cb_blockcode(hoedown_buffer *ob, const hoedown_buffer *text, const hoedown_buffer *lang, const hoedown_renderer_data *data);
	static void hoedown_cb_blockquote(hoedown_buffer *ob, const hoedown_buffer *content, const hoedown_renderer_data *data);
	static void hoedown_cb_header(hoedown_buffer *ob, const hoedown_buffer *content, int level, const hoedown_renderer_data *data);
	static void hoedown_cb_hrule(hoedown_buffer *ob, const hoedown_renderer_data *data);
	static void hoedown_cb_list(hoedown_buffer *ob, const hoedown_buffer *content, hoedown_list_flags flags, const hoedown_renderer_data *data);
	static void hoedown_cb_listitem(hoedown_buffer *ob, const hoedown_buffer *content, hoedown_list_flags flags, const hoedown_renderer_data *data);
	static void hoedown_cb_paragraph(hoedown_buffer *ob, const hoedown_buffer *content, const hoedown_renderer_data *data);
	static void hoedown_cb_table(hoedown_buffer *ob, const hoedown_buffer *content, const hoedown_renderer_data *data);
	static void hoedown_cb_table_header(hoedown_buffer *ob, const hoedown_buffer *content, const hoedown_renderer_data *data);
	static void hoedown_cb_table_body(hoedown_buffer *ob, const hoedown_buffer *content, const hoedown_renderer_data *data);
	static void hoedown_cb_table_row(hoedown_buffer *ob, const hoedown_buffer *content, const hoedown_renderer_data *data);
	static void hoedown_cb_table_cell(hoedown_buffer *ob, const hoedown_buffer *content, hoedown_table_flags flags, const hoedown_renderer_data *data);
	static void hoedown_cb_footnotes(hoedown_buffer *ob, const hoedown_buffer *content, const hoedown_renderer_data *data);
	static void hoedown_cb_footnote_def(hoedown_buffer *ob, const hoedown_buffer *content, unsigned int num, const hoedown_renderer_data *data);
	static void hoedown_cb_blockhtml(hoedown_buffer *ob, const hoedown_buffer *text, const hoedown_renderer_data *data);

	/* span level callbacks - NULL or return 0 prints the span verbatim */
	static int hoedown_cb_autolink(hoedown_buffer *ob, const hoedown_buffer *link, hoedown_autolink_type type, const hoedown_renderer_data *data);
	static int hoedown_cb_codespan(hoedown_buffer *ob, const hoedown_buffer *text, const hoedown_renderer_data *data);
	static int hoedown_cb_double_emphasis(hoedown_buffer *ob, const hoedown_buffer *content, const hoedown_renderer_data *data);
	static int hoedown_cb_emphasis(hoedown_buffer *ob, const hoedown_buffer *content, const hoedown_renderer_data *data);
	static int hoedown_cb_underline(hoedown_buffer *ob, const hoedown_buffer *content, const hoedown_renderer_data *data);
	static int hoedown_cb_highlight(hoedown_buffer *ob, const hoedown_buffer *content, const hoedown_renderer_data *data);
	static int hoedown_cb_quote(hoedown_buffer *ob, const hoedown_buffer *content, const hoedown_renderer_data *data);
	static int hoedown_cb_image(hoedown_buffer *ob, const hoedown_buffer *link, const hoedown_buffer *title, const hoedown_buffer *alt, const hoedown_renderer_data *data);
	static int hoedown_cb_linebreak(hoedown_buffer *ob, const hoedown_renderer_data *data);
	static int hoedown_cb_link(hoedown_buffer *ob, const hoedown_buffer *content, const hoedown_buffer *link, const hoedown_buffer *title, const hoedown_renderer_data *data);
	static int hoedown_cb_triple_emphasis(hoedown_buffer *ob, const hoedown_buffer *content, const hoedown_renderer_data *data);
	static int hoedown_cb_strikethrough(hoedown_buffer *ob, const hoedown_buffer *content, const hoedown_renderer_data *data);
	static int hoedown_cb_superscript(hoedown_buffer *ob, const hoedown_buffer *content, const hoedown_renderer_data *data);
	static int hoedown_cb_footnote_ref(hoedown_buffer *ob, unsigned int num, const hoedown_renderer_data *data);
	static int hoedown_cb_math(hoedown_buffer *ob, const hoedown_buffer *text, int displaymode, const hoedown_renderer_data *data);
	static int hoedown_cb_raw_html(hoedown_buffer *ob, const hoedown_buffer *text, const hoedown_renderer_data *data);

	static void hoedown_cb_entity(hoedown_buffer *ob, const hoedown_buffer *text, const hoedown_renderer_data *data);
	static void hoedown_cb_normal_text(hoedown_buffer *ob, const hoedown_buffer *text, const hoedown_renderer_data *data);

	static void hoedown_cb_doc_header(hoedown_buffer *ob, int inline_render, const hoedown_renderer_data *data);
	static void hoedown_cb_doc_footer(hoedown_buffer *ob, int inline_render, const hoedown_renderer_data *data);

	hoedown_renderer *create_custom_hoedown_renderer();
};

VARIANT_ENUM_CAST(MarkdownRenderer::RenderType);
VARIANT_ENUM_CAST(MarkdownRenderer::HTMLFlags);
VARIANT_ENUM_CAST(MarkdownRenderer::MarkdownExtensions);

VARIANT_ENUM_CAST(MarkdownRendererCustomRendererCallback::CallbackType);
VARIANT_ENUM_CAST(MarkdownRendererCustomRendererCallback::ListFlags);
VARIANT_ENUM_CAST(MarkdownRendererCustomRendererCallback::TableFlags);
VARIANT_ENUM_CAST(MarkdownRendererCustomRendererCallback::AutolinkType);

#endif
