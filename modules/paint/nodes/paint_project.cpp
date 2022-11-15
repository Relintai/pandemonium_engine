#include "paint_project.h"

Color PaintProject::get_current_color() {
	return _current_color;
}
void PaintProject::set_current_color(const Color &color) {
	_current_color = color;

	emit_signal("current_color_changed", _current_color);
}

PaintProject::PaintProject() {
	_current_color = Color(1, 1, 1, 1);
}

PaintProject::~PaintProject() {
}

void PaintProject::_bind_methods() {
    ADD_SIGNAL(MethodInfo("current_color_changed", PropertyInfo(Variant::COLOR, "color")));

	ClassDB::bind_method(D_METHOD("get_current_color"), &PaintProject::get_current_color);
	ClassDB::bind_method(D_METHOD("set_current_color", "size"), &PaintProject::set_current_color);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "current_color"), "set_current_color", "get_current_color");
}
