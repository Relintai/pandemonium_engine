#ifndef MM_TONES_EDITOR_CURSOR_H
#define MM_TONES_EDITOR_CURSOR_H

#include "scene/gui/texture_rect.h"

class MMTonesEditorCursor : public TextureRect {
	GDCLASS(MMTonesEditorCursor, TextureRect);

public:
	enum {
		CURSOR_WIDTH = 12,
		CURSOR_HEIGHT = 12,
	};

	void set_value(float val);
	void update_value(float val);

	void initialize(const Color &color, float position, bool top);

	void resize();

	void _gui_input(const Ref<InputEvent> &p_event);

	MMTonesEditorCursor();
	~MMTonesEditorCursor();

protected:
	void _notification(int p_what);
	static void _bind_methods();

	Color _color;
	float _position;
	bool _top;
};

#endif
