/*************************************************************************/
/*  markdown_renderer.cpp                                                */
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

#include "markdown_renderer.h"

#include "core/error/error_macros.h"
#include "core/log/logger.h"
#include "core/object/class_db.h"

#include "./libs/hoedown/html.h"
#include "core/object/object.h"

///// ========  MarkdownRendererCustomRendererCallback  ========

MarkdownRendererCustomRendererCallback::CallbackType MarkdownRendererCustomRendererCallback::get_callback_type() const {
	return _callback_type;
}
void MarkdownRendererCustomRendererCallback::set_callback_type(const CallbackType val) {
	_callback_type = val;
}

String MarkdownRendererCustomRendererCallback::get_text() const {
	return _text;
}
void MarkdownRendererCustomRendererCallback::set_text(const String &val) {
	_text = val;
}

String MarkdownRendererCustomRendererCallback::get_lang() const {
	return _lang;
}
void MarkdownRendererCustomRendererCallback::set_lang(const String &val) {
	_lang = val;
}

String MarkdownRendererCustomRendererCallback::get_content() const {
	return _content;
}
void MarkdownRendererCustomRendererCallback::set_content(const String &val) {
	_content = val;
}

int MarkdownRendererCustomRendererCallback::get_level() const {
	return _level;
}
void MarkdownRendererCustomRendererCallback::set_level(const int val) {
	_level = val;
}

int MarkdownRendererCustomRendererCallback::get_list_flags() const {
	return _list_flags;
}
void MarkdownRendererCustomRendererCallback::set_list_flags(const int val) {
	_list_flags = val;
}

int MarkdownRendererCustomRendererCallback::get_table_flags() const {
	return _table_flags;
}
void MarkdownRendererCustomRendererCallback::set_table_flags(const int val) {
	_table_flags = val;
}

String MarkdownRendererCustomRendererCallback::get_link() const {
	return _link;
}
void MarkdownRendererCustomRendererCallback::set_link(const String &val) {
	_link = val;
}

MarkdownRendererCustomRendererCallback::AutolinkType MarkdownRendererCustomRendererCallback::get_auto_link_type() const {
	return _auto_link_type;
}
void MarkdownRendererCustomRendererCallback::set_auto_link_type(const AutolinkType val) {
	_auto_link_type = val;
}

String MarkdownRendererCustomRendererCallback::get_title() const {
	return _title;
}
void MarkdownRendererCustomRendererCallback::set_title(const String &val) {
	_title = val;
}

String MarkdownRendererCustomRendererCallback::get_alt() const {
	return _alt;
}
void MarkdownRendererCustomRendererCallback::set_alt(const String &val) {
	_alt = val;
}

uint32_t MarkdownRendererCustomRendererCallback::get_num() const {
	return _num;
}
void MarkdownRendererCustomRendererCallback::set_num(const uint32_t val) {
	_num = val;
}

int MarkdownRendererCustomRendererCallback::get_display_mode() const {
	return _display_mode;
}
void MarkdownRendererCustomRendererCallback::set_display_mode(const int val) {
	_display_mode = val;
}

int MarkdownRendererCustomRendererCallback::get_inline_render() const {
	return _inline_render;
}
void MarkdownRendererCustomRendererCallback::set_inline_render(const int val) {
	_inline_render = val;
}

String MarkdownRendererCustomRendererCallback::get_result() const {
	return _result;
}
void MarkdownRendererCustomRendererCallback::set_result(const String &val) {
	_result = val;
}

int MarkdownRendererCustomRendererCallback::get_result_code() const {
	return _result_code;
}
void MarkdownRendererCustomRendererCallback::set_result_code(const int val) {
	_result_code = val;
}

MarkdownRendererCustomRendererCallback::MarkdownRendererCustomRendererCallback() {
	_callback_type = CALLBACK_BLOCKCODE;

	_level = 0;
	_list_flags = 0;
	_table_flags = 0;

	_auto_link_type = AUTOLINK_TYPE_NONE;

	_num = 0;
	_display_mode = 0;
	_inline_render = 0;

	_result_code = 1;
}
MarkdownRendererCustomRendererCallback::~MarkdownRendererCustomRendererCallback() {
}

void MarkdownRendererCustomRendererCallback::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_callback_type"), &MarkdownRendererCustomRendererCallback::get_callback_type);
	ClassDB::bind_method(D_METHOD("set_callback_type", "val"), &MarkdownRendererCustomRendererCallback::set_callback_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "callback_type"), "set_callback_type", "get_callback_type");

	ClassDB::bind_method(D_METHOD("get_text"), &MarkdownRendererCustomRendererCallback::get_text);
	ClassDB::bind_method(D_METHOD("set_text", "val"), &MarkdownRendererCustomRendererCallback::set_text);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "text"), "set_text", "get_text");

	ClassDB::bind_method(D_METHOD("get_lang"), &MarkdownRendererCustomRendererCallback::get_lang);
	ClassDB::bind_method(D_METHOD("set_lang", "val"), &MarkdownRendererCustomRendererCallback::set_lang);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "lang"), "set_lang", "get_lang");

	ClassDB::bind_method(D_METHOD("get_content"), &MarkdownRendererCustomRendererCallback::get_content);
	ClassDB::bind_method(D_METHOD("set_content", "val"), &MarkdownRendererCustomRendererCallback::set_content);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "content"), "set_content", "get_content");

	ClassDB::bind_method(D_METHOD("get_level"), &MarkdownRendererCustomRendererCallback::get_level);
	ClassDB::bind_method(D_METHOD("set_level", "val"), &MarkdownRendererCustomRendererCallback::set_level);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "level"), "set_level", "get_level");

	ClassDB::bind_method(D_METHOD("get_list_flags"), &MarkdownRendererCustomRendererCallback::get_list_flags);
	ClassDB::bind_method(D_METHOD("set_list_flags", "val"), &MarkdownRendererCustomRendererCallback::set_list_flags);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "list_flags", PROPERTY_HINT_FLAGS, "Ordered,LI Block"), "set_list_flags", "get_list_flags");

	ClassDB::bind_method(D_METHOD("get_table_flags"), &MarkdownRendererCustomRendererCallback::get_table_flags);
	ClassDB::bind_method(D_METHOD("set_table_flags", "val"), &MarkdownRendererCustomRendererCallback::set_table_flags);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "table_flags", PROPERTY_HINT_FLAGS, "Align Left,Align Right,Align Center"), "set_table_flags", "get_table_flags");

	ClassDB::bind_method(D_METHOD("get_link"), &MarkdownRendererCustomRendererCallback::get_link);
	ClassDB::bind_method(D_METHOD("set_link", "val"), &MarkdownRendererCustomRendererCallback::set_link);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "link"), "set_link", "get_link");

	ClassDB::bind_method(D_METHOD("get_auto_link_type"), &MarkdownRendererCustomRendererCallback::get_auto_link_type);
	ClassDB::bind_method(D_METHOD("set_auto_link_type", "val"), &MarkdownRendererCustomRendererCallback::set_auto_link_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "auto_link_type", PROPERTY_HINT_ENUM, "None,Normal,EMail"), "set_auto_link_type", "get_auto_link_type");

	ClassDB::bind_method(D_METHOD("get_title"), &MarkdownRendererCustomRendererCallback::get_title);
	ClassDB::bind_method(D_METHOD("set_title", "val"), &MarkdownRendererCustomRendererCallback::set_title);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "title"), "set_title", "get_title");

	ClassDB::bind_method(D_METHOD("get_alt"), &MarkdownRendererCustomRendererCallback::get_alt);
	ClassDB::bind_method(D_METHOD("set_alt", "val"), &MarkdownRendererCustomRendererCallback::set_alt);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "alt"), "set_alt", "get_alt");

	ClassDB::bind_method(D_METHOD("get_num"), &MarkdownRendererCustomRendererCallback::get_num);
	ClassDB::bind_method(D_METHOD("set_num", "val"), &MarkdownRendererCustomRendererCallback::set_num);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "num"), "set_num", "get_num");

	ClassDB::bind_method(D_METHOD("get_display_mode"), &MarkdownRendererCustomRendererCallback::get_display_mode);
	ClassDB::bind_method(D_METHOD("set_display_mode", "val"), &MarkdownRendererCustomRendererCallback::set_display_mode);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "display_mode"), "set_display_mode", "get_display_mode");

	ClassDB::bind_method(D_METHOD("get_inline_render"), &MarkdownRendererCustomRendererCallback::get_inline_render);
	ClassDB::bind_method(D_METHOD("set_inline_render", "val"), &MarkdownRendererCustomRendererCallback::set_inline_render);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "inline_render"), "set_inline_render", "get_inline_render");

	ClassDB::bind_method(D_METHOD("get_result"), &MarkdownRendererCustomRendererCallback::get_result);
	ClassDB::bind_method(D_METHOD("set_result", "val"), &MarkdownRendererCustomRendererCallback::set_result);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "result"), "set_result", "get_result");

	ClassDB::bind_method(D_METHOD("get_result_code"), &MarkdownRendererCustomRendererCallback::get_result_code);
	ClassDB::bind_method(D_METHOD("set_result_code", "val"), &MarkdownRendererCustomRendererCallback::set_result_code);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "result_code"), "set_result_code", "get_result_code");

	BIND_ENUM_CONSTANT(CALLBACK_BLOCKCODE);
	BIND_ENUM_CONSTANT(CALLBACK_BLOCKQUOTE);
	BIND_ENUM_CONSTANT(CALLBACK_HEADER);
	BIND_ENUM_CONSTANT(CALLBACK_HRULE);
	BIND_ENUM_CONSTANT(CALLBACK_LIST);
	BIND_ENUM_CONSTANT(CALLBACK_LISTITEM);
	BIND_ENUM_CONSTANT(CALLBACK_PARAGRAPH);
	BIND_ENUM_CONSTANT(CALLBACK_TABLE);
	BIND_ENUM_CONSTANT(CALLBACK_TABLE_HEADER);
	BIND_ENUM_CONSTANT(CALLBACK_TABLE_BODY);
	BIND_ENUM_CONSTANT(CALLBACK_TABLE_ROW);
	BIND_ENUM_CONSTANT(CALLBACK_TABLE_CELL);
	BIND_ENUM_CONSTANT(CALLBACK_FOOTNOTES);
	BIND_ENUM_CONSTANT(CALLBACK_FOOTNOTE_DEF);
	BIND_ENUM_CONSTANT(CALLBACK_BLOCKHTML);

	BIND_ENUM_CONSTANT(CALLBACK_AUTOLINK);
	BIND_ENUM_CONSTANT(CALLBACK_CODESPAN);
	BIND_ENUM_CONSTANT(CALLBACK_DOUBLE_EMPHASIS);
	BIND_ENUM_CONSTANT(CALLBACK_EMPHASIS);
	BIND_ENUM_CONSTANT(CALLBACK_UNDERLINE);
	BIND_ENUM_CONSTANT(CALLBACK_HIGHLIGHT);
	BIND_ENUM_CONSTANT(CALLBACK_QUOTE);
	BIND_ENUM_CONSTANT(CALLBACK_IMAGE);
	BIND_ENUM_CONSTANT(CALLBACK_LINEBREAK);
	BIND_ENUM_CONSTANT(CALLBACK_LINK);

	BIND_ENUM_CONSTANT(CALLBACK_TRIPLE_EMPHASIS);
	BIND_ENUM_CONSTANT(CALLBACK_STRIKETHROUGH);
	BIND_ENUM_CONSTANT(CALLBACK_SUPERSCRIPT);
	BIND_ENUM_CONSTANT(CALLBACK_FOOTNOTE_REF);
	BIND_ENUM_CONSTANT(CALLBACK_MATH);
	BIND_ENUM_CONSTANT(CALLBACK_RAW_HTML);

	BIND_ENUM_CONSTANT(CALLBACK_ENTITY);
	BIND_ENUM_CONSTANT(CALLBACK_NORMAL_TEXT);

	BIND_ENUM_CONSTANT(CALLBACK_DOC_HEADER);
	BIND_ENUM_CONSTANT(CALLBACK_DOC_FOOTER);

	BIND_ENUM_CONSTANT(LIST_FLAG_ORDERED);
	BIND_ENUM_CONSTANT(LIST_FLAG_LI_BLOCK);

	BIND_ENUM_CONSTANT(TABLE_FLAG_ALIGN_LEFT);
	BIND_ENUM_CONSTANT(TABLE_FLAG_ALIGN_RIGHT);
	BIND_ENUM_CONSTANT(TABLE_FLAG_ALIGN_CENTER);
	BIND_ENUM_CONSTANT(TABLE_FLAG_ALIGNMASK);
	BIND_ENUM_CONSTANT(TABLE_FLAG_HEADER);

	BIND_ENUM_CONSTANT(AUTOLINK_TYPE_NONE);
	BIND_ENUM_CONSTANT(AUTOLINK_TYPE_NORMAL);
	BIND_ENUM_CONSTANT(AUTOLINK_TYPE_EMAIL);
}

///// ========  MarkdownRenderer  ========

MarkdownRenderer::RenderType MarkdownRenderer::get_render_type() {
	return _render_type;
}
void MarkdownRenderer::set_render_type(const MarkdownRenderer::RenderType val) {
	_render_type = val;
}

int MarkdownRenderer::get_html_flags() {
	return _html_flags;
}
void MarkdownRenderer::set_html_flags(const int val) {
	_html_flags = val;
}

int MarkdownRenderer::get_markdown_extensions() {
	return _markdown_extensions;
}
void MarkdownRenderer::set_markdown_extensions(const int val) {
	_markdown_extensions = val;
}

int MarkdownRenderer::get_max_nesting() {
	return _max_nesting;
}
void MarkdownRenderer::set_max_nesting(const int val) {
	_max_nesting = val;
}

bool MarkdownRenderer::get_use_smartypants() {
	return _use_smartypants;
}
void MarkdownRenderer::set_use_smartypants(const bool val) {
	_use_smartypants = val;
}

int MarkdownRenderer::get_toc_level() {
	return _toc_level;
}
void MarkdownRenderer::set_toc_level(const int val) {
	_toc_level = val;
}

String MarkdownRenderer::render_to_html(const String &markdown) {
	return render(markdown);
}

String MarkdownRenderer::render(const String &markdown) {
	hoedown_renderer *renderer;

	switch (_render_type) {
		case RENDER_TYPE_HTML: {
			renderer = hoedown_html_renderer_new(static_cast<hoedown_html_flags>(_html_flags), _toc_level);
		} break;
		case RENDER_TYPE_HTML_TOC: {
			renderer = hoedown_html_toc_renderer_new(_toc_level);
		} break;
		case RENDERER_TYPE_CUSTOM: {
			renderer = create_custom_hoedown_renderer();
		} break;
		default: {
			renderer = hoedown_html_renderer_new(static_cast<hoedown_html_flags>(_html_flags), _toc_level);
		} break;
	}

	int hoedown_ext_flags = 0;

	if ((_markdown_extensions & EXTENSION_TABLES) != 0) {
		hoedown_ext_flags |= HOEDOWN_EXT_TABLES;
	}
	if ((_markdown_extensions & EXTENSION_FENCED_CODE) != 0) {
		hoedown_ext_flags |= HOEDOWN_EXT_FENCED_CODE;
	}
	if ((_markdown_extensions & EXTENSION_FOOTNOTES) != 0) {
		hoedown_ext_flags |= HOEDOWN_EXT_FOOTNOTES;
	}
	if ((_markdown_extensions & EXTENSION_AUTOLINK) != 0) {
		hoedown_ext_flags |= HOEDOWN_EXT_AUTOLINK;
	}
	if ((_markdown_extensions & EXTENSION_STRKIETHROUGH) != 0) {
		hoedown_ext_flags |= HOEDOWN_EXT_STRIKETHROUGH;
	}
	if ((_markdown_extensions & EXTENSION_UNDERLINE) != 0) {
		hoedown_ext_flags |= HOEDOWN_EXT_UNDERLINE;
	}
	if ((_markdown_extensions & EXTENSION_HIGHLIGHT) != 0) {
		hoedown_ext_flags |= HOEDOWN_EXT_HIGHLIGHT;
	}
	if ((_markdown_extensions & EXTENSION_QUOTE) != 0) {
		hoedown_ext_flags |= HOEDOWN_EXT_QUOTE;
	}
	if ((_markdown_extensions & EXTENSION_SUPERSCRIPT) != 0) {
		hoedown_ext_flags |= HOEDOWN_EXT_SUPERSCRIPT;
	}
	if ((_markdown_extensions & EXTENSION_MATH) != 0) {
		hoedown_ext_flags |= HOEDOWN_EXT_MATH;
	}
	if ((_markdown_extensions & EXTENSION_NO_INTRA_EMPHASIS) != 0) {
		hoedown_ext_flags |= HOEDOWN_EXT_NO_INTRA_EMPHASIS;
	}
	if ((_markdown_extensions & EXTENSION_SPACE_HEADERS) != 0) {
		hoedown_ext_flags |= HOEDOWN_EXT_SPACE_HEADERS;
	}
	if ((_markdown_extensions & EXTENSION_MATH_EXPLICIT) != 0) {
		hoedown_ext_flags |= HOEDOWN_EXT_MATH_EXPLICIT;
	}
	if ((_markdown_extensions & EXTENSION_DISABLE_INDENTED_CODE) != 0) {
		hoedown_ext_flags |= HOEDOWN_EXT_DISABLE_INDENTED_CODE;
	}

	hoedown_document *document = hoedown_document_new(renderer, static_cast<hoedown_extensions>(hoedown_ext_flags), _max_nesting);

	hoedown_buffer *html = hoedown_buffer_new(16);

	CharString csmd = markdown.utf8();

	hoedown_document_render(document, html, reinterpret_cast<const uint8_t *>(csmd.get_data()), csmd.length());

	String html_str = String::utf8(reinterpret_cast<const char *>(html->data), html->size);

	if (_use_smartypants) {
		CharString cshtml = html_str.utf8();

		hoedown_html_smartypants(html, reinterpret_cast<const uint8_t *>(cshtml.get_data()), cshtml.length());

		html_str = String::utf8(reinterpret_cast<const char *>(html->data), html->size);
	}

	hoedown_buffer_free(html);
	hoedown_document_free(document);

	switch (_render_type) {
		case RENDER_TYPE_HTML:
		case RENDER_TYPE_HTML_TOC: {
			hoedown_html_renderer_free(renderer);
		} break;
		case RENDERER_TYPE_CUSTOM: {
			memfree(renderer);
		} break;
		default: {
		} break;
	}

	return html_str;
}

MarkdownRenderer::MarkdownRenderer() {
	_render_type = RENDER_TYPE_HTML;
	_html_flags = 0;
	//https://github.com/hoedown/hoedown/wiki/Popular-presets
	_markdown_extensions = EXTENSION_TABLES | EXTENSION_FENCED_CODE | EXTENSION_AUTOLINK | EXTENSION_STRKIETHROUGH | EXTENSION_NO_INTRA_EMPHASIS;
	_max_nesting = 16;
	_toc_level = 0;
	_use_smartypants = false;
}

MarkdownRenderer::~MarkdownRenderer() {
}

hoedown_renderer *MarkdownRenderer::create_custom_hoedown_renderer() {
	static const hoedown_renderer cb_custom = {
		NULL,

		/* block level callbacks - NULL skips the block */
		MarkdownRenderer::hoedown_cb_blockcode,
		MarkdownRenderer::hoedown_cb_blockquote,
		MarkdownRenderer::hoedown_cb_header,
		MarkdownRenderer::hoedown_cb_hrule,
		MarkdownRenderer::hoedown_cb_list,
		MarkdownRenderer::hoedown_cb_listitem,
		MarkdownRenderer::hoedown_cb_paragraph,
		MarkdownRenderer::hoedown_cb_table,
		MarkdownRenderer::hoedown_cb_table_header,
		MarkdownRenderer::hoedown_cb_table_body,
		MarkdownRenderer::hoedown_cb_table_row,
		MarkdownRenderer::hoedown_cb_table_cell,
		MarkdownRenderer::hoedown_cb_footnotes,
		MarkdownRenderer::hoedown_cb_footnote_def,
		MarkdownRenderer::hoedown_cb_blockhtml,

		/* span level callbacks - NULL or return 0 prints the span verbatim */
		MarkdownRenderer::hoedown_cb_autolink,
		MarkdownRenderer::hoedown_cb_codespan,
		MarkdownRenderer::hoedown_cb_double_emphasis,
		MarkdownRenderer::hoedown_cb_emphasis,
		MarkdownRenderer::hoedown_cb_underline,
		MarkdownRenderer::hoedown_cb_highlight,
		MarkdownRenderer::hoedown_cb_quote,
		MarkdownRenderer::hoedown_cb_image,
		MarkdownRenderer::hoedown_cb_linebreak,
		MarkdownRenderer::hoedown_cb_link,
		MarkdownRenderer::hoedown_cb_triple_emphasis,
		MarkdownRenderer::hoedown_cb_strikethrough,
		MarkdownRenderer::hoedown_cb_superscript,
		MarkdownRenderer::hoedown_cb_footnote_ref,
		MarkdownRenderer::hoedown_cb_math,
		MarkdownRenderer::hoedown_cb_raw_html,

		MarkdownRenderer::hoedown_cb_entity,
		MarkdownRenderer::hoedown_cb_normal_text,

		MarkdownRenderer::hoedown_cb_doc_header,
		MarkdownRenderer::hoedown_cb_doc_footer,
	};

	hoedown_custom_renderer_state *state;
	hoedown_renderer *renderer;

	/* Prepare the state pointer */
	state = memnew(hoedown_custom_renderer_state);
	memset(state, 0x0, sizeof(hoedown_custom_renderer_state));

	state->self = this;

	/* Prepare the renderer */
	renderer = memnew(hoedown_renderer);
	memcpy(renderer, &cb_custom, sizeof(hoedown_renderer));

	renderer->opaque = state;

	return renderer;
}

void MarkdownRenderer::renderer_callback(Ref<MarkdownRendererCustomRendererCallback> data) {
	call("_renderer_callback", data);
}
void MarkdownRenderer::_renderer_callback(Ref<MarkdownRendererCustomRendererCallback> data) {
}

void MarkdownRenderer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_render_type"), &MarkdownRenderer::get_render_type);
	ClassDB::bind_method(D_METHOD("set_render_type", "val"), &MarkdownRenderer::set_render_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "render_type", PROPERTY_HINT_ENUM, "HTML,HTML TOC"), "set_render_type", "get_render_type");

	ClassDB::bind_method(D_METHOD("get_html_flags"), &MarkdownRenderer::get_html_flags);
	ClassDB::bind_method(D_METHOD("set_html_flags", "val"), &MarkdownRenderer::set_html_flags);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "html_flags", PROPERTY_HINT_FLAGS, "Skip HTML,Escape,Hard Wrap,Use XHTML"), "set_html_flags", "get_html_flags");

	ClassDB::bind_method(D_METHOD("get_markdown_extensions"), &MarkdownRenderer::get_markdown_extensions);
	ClassDB::bind_method(D_METHOD("set_markdown_extensions", "val"), &MarkdownRenderer::set_markdown_extensions);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "markdown_extensions", PROPERTY_HINT_FLAGS, "Tables,Fenced Code,Footnotes,Autolink,Strikethrough,Underline,Highlight,Quote,Superscript,Math,No Intra Emphasis,Space Headers,Math Explicit,Disable Indented Code"), "set_markdown_extensions", "get_markdown_extensions");

	ClassDB::bind_method(D_METHOD("get_max_nesting"), &MarkdownRenderer::get_max_nesting);
	ClassDB::bind_method(D_METHOD("set_max_nesting", "val"), &MarkdownRenderer::set_max_nesting);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_nesting"), "set_max_nesting", "get_max_nesting");

	ClassDB::bind_method(D_METHOD("get_use_smartypants"), &MarkdownRenderer::get_use_smartypants);
	ClassDB::bind_method(D_METHOD("set_use_smartypants", "val"), &MarkdownRenderer::set_use_smartypants);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "use_smartypants"), "set_use_smartypants", "get_use_smartypants");

	ClassDB::bind_method(D_METHOD("get_toc_level"), &MarkdownRenderer::get_toc_level);
	ClassDB::bind_method(D_METHOD("set_toc_level", "val"), &MarkdownRenderer::set_toc_level);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "toc_level"), "set_toc_level", "get_toc_level");

	ClassDB::bind_method(D_METHOD("render_to_html", "markdown"), &MarkdownRenderer::render_to_html);
	ClassDB::bind_method(D_METHOD("render", "markdown"), &MarkdownRenderer::render);

	BIND_VMETHOD(MethodInfo("_renderer_callback", PropertyInfo(Variant::OBJECT, "data", PROPERTY_HINT_RESOURCE_TYPE, "MarkdownRendererCustomRendererCallback")));

	//ClassDB::bind_method(D_METHOD("renderer_callback", "data"), &MarkdownRenderer::renderer_callback);
	ClassDB::bind_method(D_METHOD("_renderer_callback", "data"), &MarkdownRenderer::_renderer_callback);

	BIND_ENUM_CONSTANT(RENDER_TYPE_HTML);
	BIND_ENUM_CONSTANT(RENDER_TYPE_HTML_TOC);
	BIND_ENUM_CONSTANT(RENDERER_TYPE_CUSTOM);

	BIND_ENUM_CONSTANT(HTML_FLAG_SKIP_HTML);
	BIND_ENUM_CONSTANT(HTML_FLAG_ESCAPE);
	BIND_ENUM_CONSTANT(HTML_FLAG_HARD_WRAP);
	BIND_ENUM_CONSTANT(HTML_FLAG_USE_XHTML);

	BIND_ENUM_CONSTANT(EXTENSION_TABLES);
	BIND_ENUM_CONSTANT(EXTENSION_FENCED_CODE);
	BIND_ENUM_CONSTANT(EXTENSION_FOOTNOTES);

	BIND_ENUM_CONSTANT(EXTENSION_AUTOLINK);
	BIND_ENUM_CONSTANT(EXTENSION_STRKIETHROUGH);
	BIND_ENUM_CONSTANT(EXTENSION_UNDERLINE);
	BIND_ENUM_CONSTANT(EXTENSION_HIGHLIGHT);
	BIND_ENUM_CONSTANT(EXTENSION_QUOTE);
	BIND_ENUM_CONSTANT(EXTENSION_SUPERSCRIPT);
	BIND_ENUM_CONSTANT(EXTENSION_MATH);

	BIND_ENUM_CONSTANT(EXTENSION_NO_INTRA_EMPHASIS);
	BIND_ENUM_CONSTANT(EXTENSION_SPACE_HEADERS);
	BIND_ENUM_CONSTANT(EXTENSION_MATH_EXPLICIT);

	BIND_ENUM_CONSTANT(EXTENSION_DISABLE_INDENTED_CODE);
};

/* block level callbacks - NULL skips the block */
void MarkdownRenderer::hoedown_cb_blockcode(hoedown_buffer *ob, const hoedown_buffer *text, const hoedown_buffer *lang, const hoedown_renderer_data *data) {
	hoedown_custom_renderer_state *state = (hoedown_custom_renderer_state *)data->opaque;
	MarkdownRenderer *self = (MarkdownRenderer *)state->self;

	Ref<MarkdownRendererCustomRendererCallback> cb;
	cb.instance();

	cb->set_callback_type(MarkdownRendererCustomRendererCallback::CALLBACK_BLOCKCODE);
	if (text) {
		cb->set_text(String::utf8((const char *)text->data, text->size));
	}
	if (lang) {
		cb->set_lang(String::utf8((const char *)lang->data, lang->size));
	}

	self->renderer_callback(cb);

	CharString cs = cb->get_result().utf8();

	if (cs.length() > 0) {
		hoedown_buffer_put(ob, (const uint8_t *)cs.get_data(), cs.length());
	}
}
void MarkdownRenderer::hoedown_cb_blockquote(hoedown_buffer *ob, const hoedown_buffer *content, const hoedown_renderer_data *data) {
	hoedown_custom_renderer_state *state = (hoedown_custom_renderer_state *)data->opaque;
	MarkdownRenderer *self = (MarkdownRenderer *)state->self;

	Ref<MarkdownRendererCustomRendererCallback> cb;
	cb.instance();

	cb->set_callback_type(MarkdownRendererCustomRendererCallback::CALLBACK_BLOCKQUOTE);
	if (content) {
		cb->set_content(String::utf8((const char *)content->data, content->size));
	}

	self->renderer_callback(cb);

	CharString cs = cb->get_result().utf8();

	if (cs.length() > 0) {
		hoedown_buffer_put(ob, (const uint8_t *)cs.get_data(), cs.length());
	}
}
void MarkdownRenderer::hoedown_cb_header(hoedown_buffer *ob, const hoedown_buffer *content, int level, const hoedown_renderer_data *data) {
	hoedown_custom_renderer_state *state = (hoedown_custom_renderer_state *)data->opaque;
	MarkdownRenderer *self = (MarkdownRenderer *)state->self;

	Ref<MarkdownRendererCustomRendererCallback> cb;
	cb.instance();

	cb->set_callback_type(MarkdownRendererCustomRendererCallback::CALLBACK_HEADER);
	if (content) {
		cb->set_content(String::utf8((const char *)content->data, content->size));
	}
	cb->set_level(level);

	self->renderer_callback(cb);

	CharString cs = cb->get_result().utf8();

	if (cs.length() > 0) {
		hoedown_buffer_put(ob, (const uint8_t *)cs.get_data(), cs.length());
	}
}
void MarkdownRenderer::hoedown_cb_hrule(hoedown_buffer *ob, const hoedown_renderer_data *data) {
	hoedown_custom_renderer_state *state = (hoedown_custom_renderer_state *)data->opaque;
	MarkdownRenderer *self = (MarkdownRenderer *)state->self;

	Ref<MarkdownRendererCustomRendererCallback> cb;
	cb.instance();

	cb->set_callback_type(MarkdownRendererCustomRendererCallback::CALLBACK_HRULE);

	self->renderer_callback(cb);

	CharString cs = cb->get_result().utf8();

	if (cs.length() > 0) {
		hoedown_buffer_put(ob, (const uint8_t *)cs.get_data(), cs.length());
	}
}
void MarkdownRenderer::hoedown_cb_list(hoedown_buffer *ob, const hoedown_buffer *content, hoedown_list_flags flags, const hoedown_renderer_data *data) {
	hoedown_custom_renderer_state *state = (hoedown_custom_renderer_state *)data->opaque;
	MarkdownRenderer *self = (MarkdownRenderer *)state->self;

	Ref<MarkdownRendererCustomRendererCallback> cb;
	cb.instance();

	cb->set_callback_type(MarkdownRendererCustomRendererCallback::CALLBACK_LIST);
	if (content) {
		cb->set_content(String::utf8((const char *)content->data, content->size));
	}
	cb->set_list_flags(flags);

	self->renderer_callback(cb);

	CharString cs = cb->get_result().utf8();

	if (cs.length() > 0) {
		hoedown_buffer_put(ob, (const uint8_t *)cs.get_data(), cs.length());
	}
}
void MarkdownRenderer::hoedown_cb_listitem(hoedown_buffer *ob, const hoedown_buffer *content, hoedown_list_flags flags, const hoedown_renderer_data *data) {
	hoedown_custom_renderer_state *state = (hoedown_custom_renderer_state *)data->opaque;
	MarkdownRenderer *self = (MarkdownRenderer *)state->self;

	Ref<MarkdownRendererCustomRendererCallback> cb;
	cb.instance();

	cb->set_callback_type(MarkdownRendererCustomRendererCallback::CALLBACK_LISTITEM);
	if (content) {
		cb->set_content(String::utf8((const char *)content->data, content->size));
	}
	cb->set_list_flags(flags);

	self->renderer_callback(cb);

	CharString cs = cb->get_result().utf8();

	if (cs.length() > 0) {
		hoedown_buffer_put(ob, (const uint8_t *)cs.get_data(), cs.length());
	}
}
void MarkdownRenderer::hoedown_cb_paragraph(hoedown_buffer *ob, const hoedown_buffer *content, const hoedown_renderer_data *data) {
	hoedown_custom_renderer_state *state = (hoedown_custom_renderer_state *)data->opaque;
	MarkdownRenderer *self = (MarkdownRenderer *)state->self;

	Ref<MarkdownRendererCustomRendererCallback> cb;
	cb.instance();

	cb->set_callback_type(MarkdownRendererCustomRendererCallback::CALLBACK_PARAGRAPH);
	if (content) {
		cb->set_content(String::utf8((const char *)content->data, content->size));
	}

	self->renderer_callback(cb);

	CharString cs = cb->get_result().utf8();

	if (cs.length() > 0) {
		hoedown_buffer_put(ob, (const uint8_t *)cs.get_data(), cs.length());
	}
}
void MarkdownRenderer::hoedown_cb_table(hoedown_buffer *ob, const hoedown_buffer *content, const hoedown_renderer_data *data) {
	hoedown_custom_renderer_state *state = (hoedown_custom_renderer_state *)data->opaque;
	MarkdownRenderer *self = (MarkdownRenderer *)state->self;

	Ref<MarkdownRendererCustomRendererCallback> cb;
	cb.instance();

	cb->set_callback_type(MarkdownRendererCustomRendererCallback::CALLBACK_TABLE);
	if (content) {
		cb->set_content(String::utf8((const char *)content->data, content->size));
	}

	self->renderer_callback(cb);

	CharString cs = cb->get_result().utf8();

	if (cs.length() > 0) {
		hoedown_buffer_put(ob, (const uint8_t *)cs.get_data(), cs.length());
	}
}
void MarkdownRenderer::hoedown_cb_table_header(hoedown_buffer *ob, const hoedown_buffer *content, const hoedown_renderer_data *data) {
	hoedown_custom_renderer_state *state = (hoedown_custom_renderer_state *)data->opaque;
	MarkdownRenderer *self = (MarkdownRenderer *)state->self;

	Ref<MarkdownRendererCustomRendererCallback> cb;
	cb.instance();

	cb->set_callback_type(MarkdownRendererCustomRendererCallback::CALLBACK_TABLE_HEADER);
	if (content) {
		cb->set_content(String::utf8((const char *)content->data, content->size));
	}

	self->renderer_callback(cb);

	CharString cs = cb->get_result().utf8();

	if (cs.length() > 0) {
		hoedown_buffer_put(ob, (const uint8_t *)cs.get_data(), cs.length());
	}
}
void MarkdownRenderer::hoedown_cb_table_body(hoedown_buffer *ob, const hoedown_buffer *content, const hoedown_renderer_data *data) {
	hoedown_custom_renderer_state *state = (hoedown_custom_renderer_state *)data->opaque;
	MarkdownRenderer *self = (MarkdownRenderer *)state->self;

	Ref<MarkdownRendererCustomRendererCallback> cb;
	cb.instance();

	cb->set_callback_type(MarkdownRendererCustomRendererCallback::CALLBACK_TABLE_BODY);
	if (content) {
		cb->set_content(String::utf8((const char *)content->data, content->size));
	}

	self->renderer_callback(cb);

	CharString cs = cb->get_result().utf8();

	if (cs.length() > 0) {
		hoedown_buffer_put(ob, (const uint8_t *)cs.get_data(), cs.length());
	}
}
void MarkdownRenderer::hoedown_cb_table_row(hoedown_buffer *ob, const hoedown_buffer *content, const hoedown_renderer_data *data) {
	hoedown_custom_renderer_state *state = (hoedown_custom_renderer_state *)data->opaque;
	MarkdownRenderer *self = (MarkdownRenderer *)state->self;

	Ref<MarkdownRendererCustomRendererCallback> cb;
	cb.instance();

	cb->set_callback_type(MarkdownRendererCustomRendererCallback::CALLBACK_TABLE_ROW);
	if (content) {
		cb->set_content(String::utf8((const char *)content->data, content->size));
	}

	self->renderer_callback(cb);

	CharString cs = cb->get_result().utf8();

	if (cs.length() > 0) {
		hoedown_buffer_put(ob, (const uint8_t *)cs.get_data(), cs.length());
	}
}
void MarkdownRenderer::hoedown_cb_table_cell(hoedown_buffer *ob, const hoedown_buffer *content, hoedown_table_flags flags, const hoedown_renderer_data *data) {
	hoedown_custom_renderer_state *state = (hoedown_custom_renderer_state *)data->opaque;
	MarkdownRenderer *self = (MarkdownRenderer *)state->self;

	Ref<MarkdownRendererCustomRendererCallback> cb;
	cb.instance();

	cb->set_callback_type(MarkdownRendererCustomRendererCallback::CALLBACK_TABLE_CELL);
	if (content) {
		cb->set_content(String::utf8((const char *)content->data, content->size));
	}
	cb->set_table_flags(flags);

	self->renderer_callback(cb);

	CharString cs = cb->get_result().utf8();

	if (cs.length() > 0) {
		hoedown_buffer_put(ob, (const uint8_t *)cs.get_data(), cs.length());
	}
}
void MarkdownRenderer::hoedown_cb_footnotes(hoedown_buffer *ob, const hoedown_buffer *content, const hoedown_renderer_data *data) {
	hoedown_custom_renderer_state *state = (hoedown_custom_renderer_state *)data->opaque;
	MarkdownRenderer *self = (MarkdownRenderer *)state->self;

	Ref<MarkdownRendererCustomRendererCallback> cb;
	cb.instance();

	cb->set_callback_type(MarkdownRendererCustomRendererCallback::CALLBACK_FOOTNOTES);
	if (content) {
		cb->set_content(String::utf8((const char *)content->data, content->size));
	}

	self->renderer_callback(cb);

	CharString cs = cb->get_result().utf8();

	if (cs.length() > 0) {
		hoedown_buffer_put(ob, (const uint8_t *)cs.get_data(), cs.length());
	}
}
void MarkdownRenderer::hoedown_cb_footnote_def(hoedown_buffer *ob, const hoedown_buffer *content, unsigned int num, const hoedown_renderer_data *data) {
	hoedown_custom_renderer_state *state = (hoedown_custom_renderer_state *)data->opaque;
	MarkdownRenderer *self = (MarkdownRenderer *)state->self;

	Ref<MarkdownRendererCustomRendererCallback> cb;
	cb.instance();

	cb->set_callback_type(MarkdownRendererCustomRendererCallback::CALLBACK_FOOTNOTE_DEF);
	if (content) {
		cb->set_content(String::utf8((const char *)content->data, content->size));
	}
	cb->set_num(num);

	self->renderer_callback(cb);

	CharString cs = cb->get_result().utf8();

	if (cs.length() > 0) {
		hoedown_buffer_put(ob, (const uint8_t *)cs.get_data(), cs.length());
	}
}
void MarkdownRenderer::hoedown_cb_blockhtml(hoedown_buffer *ob, const hoedown_buffer *text, const hoedown_renderer_data *data) {
	hoedown_custom_renderer_state *state = (hoedown_custom_renderer_state *)data->opaque;
	MarkdownRenderer *self = (MarkdownRenderer *)state->self;

	Ref<MarkdownRendererCustomRendererCallback> cb;
	cb.instance();

	cb->set_callback_type(MarkdownRendererCustomRendererCallback::CALLBACK_BLOCKHTML);
	if (text) {
		cb->set_text(String::utf8((const char *)text->data, text->size));
	}

	self->renderer_callback(cb);

	CharString cs = cb->get_result().utf8();

	if (cs.length() > 0) {
		hoedown_buffer_put(ob, (const uint8_t *)cs.get_data(), cs.length());
	}
}

/* span level callbacks - NULL or return 0 prints the span verbatim */
int MarkdownRenderer::hoedown_cb_autolink(hoedown_buffer *ob, const hoedown_buffer *link, hoedown_autolink_type type, const hoedown_renderer_data *data) {
	hoedown_custom_renderer_state *state = (hoedown_custom_renderer_state *)data->opaque;
	MarkdownRenderer *self = (MarkdownRenderer *)state->self;

	Ref<MarkdownRendererCustomRendererCallback> cb;
	cb.instance();

	cb->set_callback_type(MarkdownRendererCustomRendererCallback::CALLBACK_AUTOLINK);
	if (link) {
		cb->set_link(String::utf8((const char *)link->data, link->size));
	}
	cb->set_auto_link_type((MarkdownRendererCustomRendererCallback::AutolinkType)(int)type);

	self->renderer_callback(cb);

	CharString cs = cb->get_result().utf8();

	if (cs.length() > 0) {
		hoedown_buffer_put(ob, (const uint8_t *)cs.get_data(), cs.length());
	}

	return cb->get_result_code();
}
int MarkdownRenderer::hoedown_cb_codespan(hoedown_buffer *ob, const hoedown_buffer *text, const hoedown_renderer_data *data) {
	hoedown_custom_renderer_state *state = (hoedown_custom_renderer_state *)data->opaque;
	MarkdownRenderer *self = (MarkdownRenderer *)state->self;

	Ref<MarkdownRendererCustomRendererCallback> cb;
	cb.instance();

	cb->set_callback_type(MarkdownRendererCustomRendererCallback::CALLBACK_CODESPAN);
	if (text) {
		cb->set_text(String::utf8((const char *)text->data, text->size));
	}

	self->renderer_callback(cb);

	CharString cs = cb->get_result().utf8();

	if (cs.length() > 0) {
		hoedown_buffer_put(ob, (const uint8_t *)cs.get_data(), cs.length());
	}

	return cb->get_result_code();
}
int MarkdownRenderer::hoedown_cb_double_emphasis(hoedown_buffer *ob, const hoedown_buffer *content, const hoedown_renderer_data *data) {
	hoedown_custom_renderer_state *state = (hoedown_custom_renderer_state *)data->opaque;
	MarkdownRenderer *self = (MarkdownRenderer *)state->self;

	Ref<MarkdownRendererCustomRendererCallback> cb;
	cb.instance();

	cb->set_callback_type(MarkdownRendererCustomRendererCallback::CALLBACK_DOUBLE_EMPHASIS);
	if (content) {
		cb->set_content(String::utf8((const char *)content->data, content->size));
	}

	self->renderer_callback(cb);

	CharString cs = cb->get_result().utf8();

	if (cs.length() > 0) {
		hoedown_buffer_put(ob, (const uint8_t *)cs.get_data(), cs.length());
	}

	return cb->get_result_code();
}
int MarkdownRenderer::hoedown_cb_emphasis(hoedown_buffer *ob, const hoedown_buffer *content, const hoedown_renderer_data *data) {
	hoedown_custom_renderer_state *state = (hoedown_custom_renderer_state *)data->opaque;
	MarkdownRenderer *self = (MarkdownRenderer *)state->self;

	Ref<MarkdownRendererCustomRendererCallback> cb;
	cb.instance();

	cb->set_callback_type(MarkdownRendererCustomRendererCallback::CALLBACK_EMPHASIS);
	if (content) {
		cb->set_content(String::utf8((const char *)content->data, content->size));
	}

	self->renderer_callback(cb);

	CharString cs = cb->get_result().utf8();

	if (cs.length() > 0) {
		hoedown_buffer_put(ob, (const uint8_t *)cs.get_data(), cs.length());
	}

	return cb->get_result_code();
}
int MarkdownRenderer::hoedown_cb_underline(hoedown_buffer *ob, const hoedown_buffer *content, const hoedown_renderer_data *data) {
	hoedown_custom_renderer_state *state = (hoedown_custom_renderer_state *)data->opaque;
	MarkdownRenderer *self = (MarkdownRenderer *)state->self;

	Ref<MarkdownRendererCustomRendererCallback> cb;
	cb.instance();

	cb->set_callback_type(MarkdownRendererCustomRendererCallback::CALLBACK_UNDERLINE);
	if (content) {
		cb->set_content(String::utf8((const char *)content->data, content->size));
	}

	self->renderer_callback(cb);

	CharString cs = cb->get_result().utf8();

	if (cs.length() > 0) {
		hoedown_buffer_put(ob, (const uint8_t *)cs.get_data(), cs.length());
	}

	return cb->get_result_code();
}
int MarkdownRenderer::hoedown_cb_highlight(hoedown_buffer *ob, const hoedown_buffer *content, const hoedown_renderer_data *data) {
	hoedown_custom_renderer_state *state = (hoedown_custom_renderer_state *)data->opaque;
	MarkdownRenderer *self = (MarkdownRenderer *)state->self;

	Ref<MarkdownRendererCustomRendererCallback> cb;
	cb.instance();

	cb->set_callback_type(MarkdownRendererCustomRendererCallback::CALLBACK_HIGHLIGHT);
	if (content) {
		cb->set_content(String::utf8((const char *)content->data, content->size));
	}

	self->renderer_callback(cb);

	CharString cs = cb->get_result().utf8();

	if (cs.length() > 0) {
		hoedown_buffer_put(ob, (const uint8_t *)cs.get_data(), cs.length());
	}

	return cb->get_result_code();
}
int MarkdownRenderer::hoedown_cb_quote(hoedown_buffer *ob, const hoedown_buffer *content, const hoedown_renderer_data *data) {
	hoedown_custom_renderer_state *state = (hoedown_custom_renderer_state *)data->opaque;
	MarkdownRenderer *self = (MarkdownRenderer *)state->self;

	Ref<MarkdownRendererCustomRendererCallback> cb;
	cb.instance();

	cb->set_callback_type(MarkdownRendererCustomRendererCallback::CALLBACK_QUOTE);
	if (content) {
		cb->set_content(String::utf8((const char *)content->data, content->size));
	}

	self->renderer_callback(cb);

	CharString cs = cb->get_result().utf8();

	if (cs.length() > 0) {
		hoedown_buffer_put(ob, (const uint8_t *)cs.get_data(), cs.length());
	}

	return cb->get_result_code();
}
int MarkdownRenderer::hoedown_cb_image(hoedown_buffer *ob, const hoedown_buffer *link, const hoedown_buffer *title, const hoedown_buffer *alt, const hoedown_renderer_data *data) {
	hoedown_custom_renderer_state *state = (hoedown_custom_renderer_state *)data->opaque;
	MarkdownRenderer *self = (MarkdownRenderer *)state->self;

	Ref<MarkdownRendererCustomRendererCallback> cb;
	cb.instance();

	cb->set_callback_type(MarkdownRendererCustomRendererCallback::CALLBACK_IMAGE);
	if (link) {
		cb->set_link(String::utf8((const char *)link->data, link->size));
	}
	if (title) {
		cb->set_title(String::utf8((const char *)title->data, title->size));
	}

	self->renderer_callback(cb);

	CharString cs = cb->get_result().utf8();

	if (cs.length() > 0) {
		hoedown_buffer_put(ob, (const uint8_t *)cs.get_data(), cs.length());
	}

	return cb->get_result_code();
}
int MarkdownRenderer::hoedown_cb_linebreak(hoedown_buffer *ob, const hoedown_renderer_data *data) {
	hoedown_custom_renderer_state *state = (hoedown_custom_renderer_state *)data->opaque;
	MarkdownRenderer *self = (MarkdownRenderer *)state->self;

	Ref<MarkdownRendererCustomRendererCallback> cb;
	cb.instance();

	cb->set_callback_type(MarkdownRendererCustomRendererCallback::CALLBACK_LINEBREAK);

	self->renderer_callback(cb);

	CharString cs = cb->get_result().utf8();

	if (cs.length() > 0) {
		hoedown_buffer_put(ob, (const uint8_t *)cs.get_data(), cs.length());
	}

	return cb->get_result_code();
}
int MarkdownRenderer::hoedown_cb_link(hoedown_buffer *ob, const hoedown_buffer *content, const hoedown_buffer *link, const hoedown_buffer *title, const hoedown_renderer_data *data) {
	hoedown_custom_renderer_state *state = (hoedown_custom_renderer_state *)data->opaque;
	MarkdownRenderer *self = (MarkdownRenderer *)state->self;

	Ref<MarkdownRendererCustomRendererCallback> cb;
	cb.instance();

	cb->set_callback_type(MarkdownRendererCustomRendererCallback::CALLBACK_LINK);
	if (content) {
		cb->set_content(String::utf8((const char *)content->data, content->size));
	}
	if (link) {
		cb->set_link(String::utf8((const char *)link->data, link->size));
	}
	if (title) {
		cb->set_title(String::utf8((const char *)title->data, title->size));
	}

	self->renderer_callback(cb);

	CharString cs = cb->get_result().utf8();

	if (cs.length() > 0) {
		hoedown_buffer_put(ob, (const uint8_t *)cs.get_data(), cs.length());
	}

	return cb->get_result_code();
}
int MarkdownRenderer::hoedown_cb_triple_emphasis(hoedown_buffer *ob, const hoedown_buffer *content, const hoedown_renderer_data *data) {
	hoedown_custom_renderer_state *state = (hoedown_custom_renderer_state *)data->opaque;
	MarkdownRenderer *self = (MarkdownRenderer *)state->self;

	Ref<MarkdownRendererCustomRendererCallback> cb;
	cb.instance();

	cb->set_callback_type(MarkdownRendererCustomRendererCallback::CALLBACK_TRIPLE_EMPHASIS);
	if (content) {
		cb->set_content(String::utf8((const char *)content->data, content->size));
	}

	self->renderer_callback(cb);

	CharString cs = cb->get_result().utf8();

	if (cs.length() > 0) {
		hoedown_buffer_put(ob, (const uint8_t *)cs.get_data(), cs.length());
	}

	return cb->get_result_code();
}
int MarkdownRenderer::hoedown_cb_strikethrough(hoedown_buffer *ob, const hoedown_buffer *content, const hoedown_renderer_data *data) {
	hoedown_custom_renderer_state *state = (hoedown_custom_renderer_state *)data->opaque;
	MarkdownRenderer *self = (MarkdownRenderer *)state->self;

	Ref<MarkdownRendererCustomRendererCallback> cb;
	cb.instance();

	cb->set_callback_type(MarkdownRendererCustomRendererCallback::CALLBACK_STRIKETHROUGH);
	if (content) {
		cb->set_content(String::utf8((const char *)content->data, content->size));
	}

	self->renderer_callback(cb);

	CharString cs = cb->get_result().utf8();

	if (cs.length() > 0) {
		hoedown_buffer_put(ob, (const uint8_t *)cs.get_data(), cs.length());
	}

	return cb->get_result_code();
}
int MarkdownRenderer::hoedown_cb_superscript(hoedown_buffer *ob, const hoedown_buffer *content, const hoedown_renderer_data *data) {
	hoedown_custom_renderer_state *state = (hoedown_custom_renderer_state *)data->opaque;
	MarkdownRenderer *self = (MarkdownRenderer *)state->self;

	Ref<MarkdownRendererCustomRendererCallback> cb;
	cb.instance();

	cb->set_callback_type(MarkdownRendererCustomRendererCallback::CALLBACK_SUPERSCRIPT);
	if (content) {
		cb->set_content(String::utf8((const char *)content->data, content->size));
	}

	self->renderer_callback(cb);

	CharString cs = cb->get_result().utf8();

	if (cs.length() > 0) {
		hoedown_buffer_put(ob, (const uint8_t *)cs.get_data(), cs.length());
	}

	return cb->get_result_code();
}
int MarkdownRenderer::hoedown_cb_footnote_ref(hoedown_buffer *ob, unsigned int num, const hoedown_renderer_data *data) {
	hoedown_custom_renderer_state *state = (hoedown_custom_renderer_state *)data->opaque;
	MarkdownRenderer *self = (MarkdownRenderer *)state->self;

	Ref<MarkdownRendererCustomRendererCallback> cb;
	cb.instance();

	cb->set_callback_type(MarkdownRendererCustomRendererCallback::CALLBACK_FOOTNOTE_REF);
	cb->set_num(num);

	self->renderer_callback(cb);

	CharString cs = cb->get_result().utf8();

	if (cs.length() > 0) {
		hoedown_buffer_put(ob, (const uint8_t *)cs.get_data(), cs.length());
	}

	return cb->get_result_code();
}
int MarkdownRenderer::hoedown_cb_math(hoedown_buffer *ob, const hoedown_buffer *text, int displaymode, const hoedown_renderer_data *data) {
	hoedown_custom_renderer_state *state = (hoedown_custom_renderer_state *)data->opaque;
	MarkdownRenderer *self = (MarkdownRenderer *)state->self;

	Ref<MarkdownRendererCustomRendererCallback> cb;
	cb.instance();

	cb->set_callback_type(MarkdownRendererCustomRendererCallback::CALLBACK_MATH);
	if (text) {
		cb->set_text(String::utf8((const char *)text->data, text->size));
	}
	cb->set_display_mode(displaymode);

	self->renderer_callback(cb);

	CharString cs = cb->get_result().utf8();

	if (cs.length() > 0) {
		hoedown_buffer_put(ob, (const uint8_t *)cs.get_data(), cs.length());
	}

	return cb->get_result_code();
}
int MarkdownRenderer::hoedown_cb_raw_html(hoedown_buffer *ob, const hoedown_buffer *text, const hoedown_renderer_data *data) {
	hoedown_custom_renderer_state *state = (hoedown_custom_renderer_state *)data->opaque;
	MarkdownRenderer *self = (MarkdownRenderer *)state->self;

	Ref<MarkdownRendererCustomRendererCallback> cb;
	cb.instance();

	cb->set_callback_type(MarkdownRendererCustomRendererCallback::CALLBACK_RAW_HTML);
	if (text) {
		cb->set_text(String::utf8((const char *)text->data, text->size));
	}

	self->renderer_callback(cb);

	CharString cs = cb->get_result().utf8();

	if (cs.length() > 0) {
		hoedown_buffer_put(ob, (const uint8_t *)cs.get_data(), cs.length());
	}

	return cb->get_result_code();
}

void MarkdownRenderer::hoedown_cb_entity(hoedown_buffer *ob, const hoedown_buffer *text, const hoedown_renderer_data *data) {
	hoedown_custom_renderer_state *state = (hoedown_custom_renderer_state *)data->opaque;
	MarkdownRenderer *self = (MarkdownRenderer *)state->self;

	Ref<MarkdownRendererCustomRendererCallback> cb;
	cb.instance();

	cb->set_callback_type(MarkdownRendererCustomRendererCallback::CALLBACK_ENTITY);
	if (text) {
		cb->set_text(String::utf8((const char *)text->data, text->size));
	}

	self->renderer_callback(cb);

	CharString cs = cb->get_result().utf8();

	if (cs.length() > 0) {
		hoedown_buffer_put(ob, (const uint8_t *)cs.get_data(), cs.length());
	}
}
void MarkdownRenderer::hoedown_cb_normal_text(hoedown_buffer *ob, const hoedown_buffer *text, const hoedown_renderer_data *data) {
	hoedown_custom_renderer_state *state = (hoedown_custom_renderer_state *)data->opaque;
	MarkdownRenderer *self = (MarkdownRenderer *)state->self;

	Ref<MarkdownRendererCustomRendererCallback> cb;
	cb.instance();

	cb->set_callback_type(MarkdownRendererCustomRendererCallback::CALLBACK_NORMAL_TEXT);
	if (text) {
		cb->set_text(String::utf8((const char *)text->data, text->size));
	}

	self->renderer_callback(cb);

	CharString cs = cb->get_result().utf8();

	if (cs.length() > 0) {
		hoedown_buffer_put(ob, (const uint8_t *)cs.get_data(), cs.length());
	}
}

void MarkdownRenderer::hoedown_cb_doc_header(hoedown_buffer *ob, int inline_render, const hoedown_renderer_data *data) {
	hoedown_custom_renderer_state *state = (hoedown_custom_renderer_state *)data->opaque;
	MarkdownRenderer *self = (MarkdownRenderer *)state->self;

	Ref<MarkdownRendererCustomRendererCallback> cb;
	cb.instance();

	cb->set_callback_type(MarkdownRendererCustomRendererCallback::CALLBACK_DOC_HEADER);
	cb->set_inline_render(inline_render);

	self->renderer_callback(cb);

	CharString cs = cb->get_result().utf8();

	if (cs.length() > 0) {
		hoedown_buffer_put(ob, (const uint8_t *)cs.get_data(), cs.length());
	}
}
void MarkdownRenderer::hoedown_cb_doc_footer(hoedown_buffer *ob, int inline_render, const hoedown_renderer_data *data) {
	hoedown_custom_renderer_state *state = (hoedown_custom_renderer_state *)data->opaque;
	MarkdownRenderer *self = (MarkdownRenderer *)state->self;

	Ref<MarkdownRendererCustomRendererCallback> cb;
	cb.instance();

	cb->set_callback_type(MarkdownRendererCustomRendererCallback::CALLBACK_DOC_FOOTER);
	cb->set_inline_render(inline_render);

	self->renderer_callback(cb);

	CharString cs = cb->get_result().utf8();

	if (cs.length() > 0) {
		hoedown_buffer_put(ob, (const uint8_t *)cs.get_data(), cs.length());
	}
}
