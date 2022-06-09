
#include "color_picker_popup.h"

#include "scene/gui/color_picker.h"

ColorPickerPopup::ColorPickerPopup() {
	color_picker = memnew(ColorPicker);
	add_child(color_picker);
}

ColorPickerPopup::~ColorPickerPopup() {
	color_picker = nullptr;
}

void ColorPickerPopup::_bind_methods() {
}
