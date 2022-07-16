#ifndef PLUGIN_REFRESHER_H
#define PLUGIN_REFRESHER_H

#include "core/reference.h"
#include "core/ustring.h"
#include "core/variant.h"
#include "core/vector.h"

#include "scene/gui/tool_button.h"

class CheckBox;
class AcceptDialog;
class OptionButton;

class PluginRefresher : public ToolButton {
	GDCLASS(PluginRefresher, ToolButton);

public:
	void set_enabled(const bool p_enabled);

	void set_enabler_check_box(CheckBox *cb);

	PluginRefresher();
	~PluginRefresher();

protected:
	void _popup();
	void _refresh();
	void _set_enabled(const bool p_enabled);
	void _on_plugins_button_pressed();
	void _on_disable_button_pressed();
	void _on_option_selected(const int id);
	void _refresh_selected();

	void pressed();
	void _gui_input(Ref<InputEvent> p_event);

	static void _bind_methods();

	int _button;
	CheckBox *_enabler_check_box;
	AcceptDialog *_selector_dialog;
	OptionButton *_option_button;
	int _currently_selected_addon;

	struct PluginEntry {
		String name;
		String folder;
	};

	Vector<PluginEntry> _plugins;
};

#endif
