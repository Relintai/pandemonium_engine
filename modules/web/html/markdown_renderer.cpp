#include "markdown_renderer.h"

#include "core/object/class_db.h"
#include "core/error_macros.h"
#include "core/log/logger.h"

#include "./libs/hoedown/html.h"
#include "core/object/object.h"

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
	hoedown_renderer *renderer;

	switch (_render_type) {
		case RENDER_TYPE_HTML: {
			renderer = hoedown_html_renderer_new(static_cast<hoedown_html_flags>(_html_flags), _toc_level);
		} break;
		case RENDER_TYPE_HTML_TOC: {
			renderer = hoedown_html_toc_renderer_new(_toc_level);
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

	String html_str = reinterpret_cast<const char *>(html->data);

	if (_use_smartypants) {
		CharString cshtml = html_str.utf8();

		hoedown_html_smartypants(html, reinterpret_cast<const uint8_t *>(cshtml.get_data()), cshtml.length());

		html_str = reinterpret_cast<const char *>(html->data);
	}

	hoedown_buffer_free(html);
	hoedown_document_free(document);
	hoedown_html_renderer_free(renderer);

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

	BIND_ENUM_CONSTANT(RENDER_TYPE_HTML);
	BIND_ENUM_CONSTANT(RENDER_TYPE_HTML_TOC);

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
