#ifndef POLYGON_CONTROL_POINT_H
#define POLYGON_CONTROL_POINT_H

#include "scene/gui/control.h"

#include "core/os/input_event.h"

class PolygonControlPoint : public Control {
	GDCLASS(PolygonControlPoint, Control);

public:
	bool get_moving() const;
	void set_moving(const bool val);

	void _draw();
	void initialize(const Vector2 &p);

	void _on_ControlPoint_gui_input(const Ref<InputEvent> &event);

	PolygonControlPoint();
	~PolygonControlPoint();

protected:
	void _notification(int p_what);

	static void _bind_methods();

	bool moving = false;
	const Vector2 OFFSET = Vector2(3, 3);
};

#endif
