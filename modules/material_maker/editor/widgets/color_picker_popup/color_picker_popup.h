#ifndef COLOR_PICKER_POPUP_H
#define COLOR_PICKER_POPUP_H

#include "scene/gui/popup.h"

class ColorPicker;

class ColorPickerPopup : public PopupPanel {
	GDCLASS(ColorPickerPopup, PopupPanel);

public:
	ColorPicker *color_picker;

	ColorPickerPopup();
	~ColorPickerPopup();

protected:
	static void _bind_methods();
};

#endif
