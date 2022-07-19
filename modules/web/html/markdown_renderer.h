#ifndef MARKDOWN_RENDERER_H
#define MARKDOWN_RENDERER_H

#include "core/ustring.h"

#include "core/reference.h"

class MarkdownRenderer : public Reference {
	GDCLASS(MarkdownRenderer, Reference);

public:
	enum RenderType {
		RENDER_TYPE_HTML = 0,
		RENDER_TYPE_HTML_TOC = 1,
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

	MarkdownRenderer();
	virtual ~MarkdownRenderer();

protected:
	static void _bind_methods();

	RenderType _render_type;
	int _html_flags;
	int _markdown_extensions;
	int _max_nesting;
	int _toc_level;
	bool _use_smartypants;
};

VARIANT_ENUM_CAST(MarkdownRenderer::RenderType);
VARIANT_ENUM_CAST(MarkdownRenderer::HTMLFlags);
VARIANT_ENUM_CAST(MarkdownRenderer::MarkdownExtensions);

#endif
