#ifndef CONTROL_POINT_H
#define CONTROL_POINT_H

#include "scene/gui/control.h"

#include "../../../nodes/bases/curve_base.h"

class SlopePoint;

class ControlPoint : public Control {
	GDCLASS(ControlPoint, Control);

public:
	bool get_moving() const;
	void set_moving(const bool val);

	float get_min_x() const;
	void set_min_x(const float val);

	float get_max_x() const;
	void set_max_x(const float val);

	float get_min_y() const;
	void set_min_y(const float val);

	float get_max_y() const;
	void set_max_y(const float val);

	void _draw();

	void initialize(const CurveBase::Point &p);
	void set_constraint(const float x, const float X, const float y, const float Y);
	void update_tangents();

	void _on_ControlPoint_gui_input(const Ref<InputEvent> &event);

	ControlPoint();
	~ControlPoint();

	const Vector2 OFFSET = Vector2(3, 3);

	SlopePoint *left_slope_point;
	SlopePoint *right_slope_point;

protected:
	void _notification(int p_what);

	static void _bind_methods();

	bool moving;
	float min_x;
	float max_x;
	float min_y;
	float max_y;
};

#endif
