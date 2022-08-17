
#ifndef GRADIENT_CURSOR_H
#define GRADIENT_CURSOR_H

#include "core/math/color.h"
#include "core/input/input_event.h"
#include "core/variant/variant.h"

#include "scene/gui/control.h"

class Label;

class GradientCursor : public Control {
	GDCLASS(GradientCursor, Control);

public:
	Color get_cursor_color();
	void set_cursor_color(const Color &val);

	void set_cursor_color_notify(const Color &val);

	bool get_sliding() const;
	void set_sliding(const bool val);

	void set_label(Label *label);

	void _draw();
	void _gui_input(const Ref<InputEvent> &ev);

	float get_cursor_position();

	bool operator<(const GradientCursor &b) const;

	bool can_drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from) const;
	void drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from);

	GradientCursor();
	~GradientCursor();

	static const int WIDTH;

protected:
	void _notification(int p_what);

	static void _bind_methods();

	Color color;
	bool sliding;
	Label *label;
};

#endif
