
#ifndef GRADIENT_CURSOR_H
#define GRADIENT_CURSOR_H

#include "core/color.h"
#include "core/os/input_event.h"
#include "core/variant.h"

#include "scene/gui/control.h"

class Label;

class GradientCursor : public Control {
	GDCLASS(GradientCursor, Control);

public:
	Color get_color();
	void set_color(const Color &val);

	bool get_sliding() const;
	void set_sliding(const bool val);

	void set_label(Label *label);

	void _draw();
	void _gui_input(const Ref<InputEvent> &ev);

	float get_cursor_position();

	static bool sort(GradientCursor *a, GradientCursor *b);

	bool can_drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from) const;
	void drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from);

	GradientCursor();
	~GradientCursor();

protected:
	void _notification(int p_what);

	static void _bind_methods();

	Color color;
	bool sliding;
	Label *label;

	const int WIDTH = 10;
};

#endif
