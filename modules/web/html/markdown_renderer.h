#ifndef MARKDOWN_RENDERER_H
#define MARKDOWN_RENDERER_H

#include "core/ustring.h"

#include "core/reference.h"

class MarkdownRenderer : public Reference {
	GDCLASS(MarkdownRenderer, Reference);

public:
	MarkdownRenderer();
	virtual ~MarkdownRenderer();

protected:
	static void _bind_methods();
};

#endif
