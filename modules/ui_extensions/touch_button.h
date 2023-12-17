#ifndef TOUCH_BUTTON_H
#define TOUCH_BUTTON_H


#include "core/config/engine.h"

#include "core/os/os.h"
#include "scene/gui/button.h"
#include "scene/main/viewport.h"

class TouchButton : public Button {
	GDCLASS(TouchButton, Button);

public:
	TouchButton() {}

	bool _is_point_inside(const Point2 &p_point);

protected:
	void _notification(int p_what);
	virtual void _gui_input(Ref<InputEvent> p_event);
	virtual void _shortcut_input(Ref<InputEvent> p_event);
	void _input(Ref<InputEvent> p_event);
	static void _bind_methods();
};

#endif
