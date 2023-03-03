#ifndef MM_TONES_EDITOR_CURSOR_H
#define MM_TONES_EDITOR_CURSOR_H

#include "scene/gui/texture_rect.h"

class MMTonesEditorCursor : public TextureRect {
	GDCLASS(MMTonesEditorCursor, TextureRect);

public:
	MMTonesEditorCursor();
	~MMTonesEditorCursor();

protected:
	static void _bind_methods();
};

#endif
