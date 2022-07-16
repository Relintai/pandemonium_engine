
#include "plugin_refresher.h"
#include "core/os/input_event.h"

#include "editor/project_settings_editor.h"

#include "editor/editor_settings.h"
#include "scene/gui/box_container.h"
#include "scene/gui/check_box.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/label.h"
#include "scene/gui/option_button.h"

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

void PluginRefresher::_popup() {
	_refresh();
	_selector_dialog->popup_centered();
}
void PluginRefresher::_refresh() {
}

void PluginRefresher::_set_enabled(const bool p_enabled) {
	set_visible(p_enabled);

	bool sett_enabled = EditorSettings::get_singleton()->get_project_metadata("plugin_refresher", "enabled", false);

	if (sett_enabled != p_enabled) {
		EditorSettings::get_singleton()->set_project_metadata("plugin_refresher", "enabled", p_enabled);
	}
}

void PluginRefresher::_on_plugins_button_pressed() {
	_selector_dialog->hide();
	ProjectSettingsEditor::get_singleton()->set_plugins_page();
	ProjectSettingsEditor::get_singleton()->popup_project_settings();
}
void PluginRefresher::_on_disable_button_pressed() {
	_selector_dialog->hide();
	set_enabled(false);
}

void PluginRefresher::_on_popup_confirmed() {
}

void PluginRefresher::pressed() {
	if (_button == BUTTON_LEFT) {
	} else if (_button == BUTTON_RIGHT) {
		_popup();
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

	_selector_dialog = memnew(AcceptDialog);
	add_child(_selector_dialog);
	_selector_dialog->set_title("Plugin refresher");
	_selector_dialog->set_custom_minimum_size(Size2(300, 0));
	_selector_dialog->connect("confirmed", this, "_on_popup_confirmed");

	VBoxContainer *main_container = memnew(VBoxContainer);
	_selector_dialog->add_child(main_container);

	HBoxContainer *top_container = memnew(HBoxContainer);
	main_container->add_child(top_container);
	top_container->set_alignment(BoxContainer::ALIGN_END);

	Button *plugins_button = memnew(Button);
	top_container->add_child(plugins_button);
	plugins_button->set_text("Plugins");
	plugins_button->set_tooltip("Open the plugins tab.");
	plugins_button->connect("pressed", this, "_on_plugins_button_pressed");

	Button *disable_button = memnew(Button);
	top_container->add_child(disable_button);
	disable_button->set_text("Disable");
	disable_button->set_tooltip("Disables the refresher widget.");
	disable_button->connect("pressed", this, "_on_disable_button_pressed");

	Label *ml = memnew(Label);
	main_container->add_child(ml);
	ml->set_text("Select an addon:");

	_option_button = memnew(OptionButton);
	main_container->add_child(_option_button);
}

PluginRefresher::~PluginRefresher() {
}

void PluginRefresher::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_set_enabled", "enabled"), &PluginRefresher::_set_enabled);
	ClassDB::bind_method(D_METHOD("_on_plugins_button_pressed"), &PluginRefresher::_on_plugins_button_pressed);
	ClassDB::bind_method(D_METHOD("_on_disable_button_pressed"), &PluginRefresher::_on_disable_button_pressed);
	ClassDB::bind_method(D_METHOD("_on_popup_confirmed"), &PluginRefresher::_on_popup_confirmed);
}
