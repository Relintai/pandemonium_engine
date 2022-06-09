#ifndef MM_DND_COLOR_PICKER_BUTTON_H
#define MM_DND_COLOR_PICKER_BUTTON_H

#include "scene/gui/color_picker.h"

class MMDNDColorPickerButton : public ColorPickerButton {
	GDCLASS(MMDNDColorPickerButton, ColorPickerButton);

public:
	Variant get_drag_data_fw(const Point2 &p_point, Control *p_from);
	bool can_drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from) const;
	void drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from);

	MMDNDColorPickerButton();
	~MMDNDColorPickerButton();

protected:
	static void _bind_methods();
};

#endif
