
#include "plugin_refresher.h"
#include "core/os/input_event.h"

#include "editor/editor_settings.h"
#include "scene/gui/check_box.h"

void PluginRefresher::set_enabled(const bool p_enabled) {
	if (_enabler_check_box) {
		_enabler_check_box->set_pressed(p_enabled);
	}

	_set_enabled(p_enabled);
}

void PluginRefresher::set_enabler_check_box(CheckBox *cb) {
	_enabler_check_box = cb;

	if (_enabler_check_box) {
		_enabler_check_box->connect("toggled", this, "_set_enabled");
	}
}

void PluginRefresher::_set_enabled(const bool p_enabled) {
	set_visible(p_enabled);

	bool sett_enabled = EditorSettings::get_singleton()->get_project_metadata("plugin_refresher", "enabled", false);

	if (sett_enabled != p_enabled) {
		EditorSettings::get_singleton()->set_project_metadata("plugin_refresher", "enabled", p_enabled);
	}
}

void PluginRefresher::pressed() {
	if (_button == BUTTON_LEFT) {
	} else if (_button == BUTTON_RIGHT) {
	}
}
void PluginRefresher::_gui_input(Ref<InputEvent> p_event) {
	Ref<InputEventMouseButton> mouse_button = p_event;

	if (mouse_button.is_valid()) {
		_button = mouse_button->get_button_index();
	}

	BaseButton::_gui_input(p_event);
}

PluginRefresher::PluginRefresher() {
	_button = 0;
	set_button_mask(BUTTON_MASK_LEFT | BUTTON_MASK_RIGHT);
	set_enabled_focus_mode(FOCUS_NONE);
	set_tooltip("Left click: Refresh selected plugin.\nRight click: Settings.");
}

PluginRefresher::~PluginRefresher() {
}

void PluginRefresher::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_set_enabled", "enabled"), &PluginRefresher::_set_enabled);
}
