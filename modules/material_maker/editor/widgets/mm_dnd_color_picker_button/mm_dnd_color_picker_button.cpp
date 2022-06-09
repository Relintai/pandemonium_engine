
#include "mm_dnd_color_picker_button.h"

#include "scene/gui/color_rect.h"

Variant MMDNDColorPickerButton::get_drag_data_fw(const Point2 &p_point, Control *p_from) {
	ColorRect *preview = memnew(ColorRect);
	preview->set_frame_color(get_pick_color());
	preview->set_custom_minimum_size(Vector2(32, 32));
	set_drag_preview(preview);
	return get_pick_color();
}

bool MMDNDColorPickerButton::can_drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from) const {
	return p_data.get_type() == Variant::COLOR;
}

void MMDNDColorPickerButton::drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from) {
	set_pick_color(p_data);
	emit_signal("color_changed", get_pick_color());
}

MMDNDColorPickerButton::MMDNDColorPickerButton() {
}

MMDNDColorPickerButton::~MMDNDColorPickerButton() {
}

void MMDNDColorPickerButton::_bind_methods() {
	ADD_SIGNAL(MethodInfo("color_changed", PropertyInfo(Variant::COLOR, "color")));

	ClassDB::bind_method(D_METHOD("get_drag_data_fw", "point", "from"), &MMDNDColorPickerButton::get_drag_data_fw);
	ClassDB::bind_method(D_METHOD("can_drop_data_fw", "point", "data", "from"), &MMDNDColorPickerButton::can_drop_data_fw);
	ClassDB::bind_method(D_METHOD("drop_data_fw", "point", "data", "from"), &MMDNDColorPickerButton::drop_data_fw);
}
