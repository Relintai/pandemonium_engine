#ifndef SLOPE_POINT_H
#define SLOPE_POINT_H

#include "core/math/vector2.h"
#include "core/os/input_event.h"

#include "scene/gui/control.h"

class SlopePoint : public Control {
	GDCLASS(SlopePoint, Control);

public:
	float get_distance() const;
	void set_distance(const float val);

	Variant get_moving();
	void set_moving(const bool val);

	void _on_ControlPoint_gui_input(const Ref<InputEvent> &event);

	SlopePoint();
	~SlopePoint();

protected:
	void _notification(int p_what);

	static void _bind_methods();

	float distance;
	bool moving;
	const Vector2 OFFSET = -Vector2(0, 0);
};

#endif
