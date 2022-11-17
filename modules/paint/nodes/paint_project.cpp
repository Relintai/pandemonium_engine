#include "paint_project.h"

#include "core/config/project_settings.h"

Color PaintProject::get_current_color() {
	return _current_color;
}
void PaintProject::set_current_color(const Color &color) {
	_current_color = color;

	emit_signal("current_color_changed", _current_color);
}

PoolColorArray PaintProject::get_color_presets() {
	return _color_presets;
}
void PaintProject::set_color_presets(const PoolColorArray &colors) {
	_color_presets = colors;

	emit_signal("color_presets_changed");
}

void PaintProject::set_colors_as_default() {
	if (ProjectSettings::get_singleton()) {
		ProjectSettings::get_singleton()->set_setting("paint/color_presets/colors", _color_presets);
	}
}

PaintProject::PaintProject() {
	_current_color = Color(1, 1, 1, 1);
}

PaintProject::~PaintProject() {
}

void PaintProject::_notification(int p_what) {
	if (p_what == NOTIFICATION_ENTER_TREE) {
		if (_color_presets.size() == 0) {
			if (ProjectSettings::get_singleton()->has_setting("paint/color_presets/colors")) {
				_color_presets = ProjectSettings::get_singleton()->get_setting("paint/color_presets/colors");
			}
		}
	}
}

void PaintProject::_bind_methods() {
	ADD_SIGNAL(MethodInfo("current_color_changed", PropertyInfo(Variant::COLOR, "color")));
	ADD_SIGNAL(MethodInfo("color_presets_changed"));

	ClassDB::bind_method(D_METHOD("get_current_color"), &PaintProject::get_current_color);
	ClassDB::bind_method(D_METHOD("set_current_color", "size"), &PaintProject::set_current_color);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "current_color"), "set_current_color", "get_current_color");

	ClassDB::bind_method(D_METHOD("get_color_presets"), &PaintProject::get_color_presets);
	ClassDB::bind_method(D_METHOD("set_color_presets", "colors"), &PaintProject::set_color_presets);
	ADD_PROPERTY(PropertyInfo(Variant::POOL_COLOR_ARRAY, "color_presets"), "set_color_presets", "get_color_presets");

	ClassDB::bind_method(D_METHOD("set_colors_as_default"), &PaintProject::set_colors_as_default);
	ADD_PROPERTY(PropertyInfo(Variant::NIL, "set_colors_as_default", PROPERTY_HINT_BUTTON, "set_colors_as_default"), "", "");
}
