#ifndef PLUGIN_REFRESHER_H
#define PLUGIN_REFRESHER_H

#include "core/reference.h"
#include "core/ustring.h"
#include "core/variant.h"
#include "core/vector.h"

#include "scene/gui/tool_button.h"

class CheckBox;

class PluginRefresher : public ToolButton {
	GDCLASS(PluginRefresher, ToolButton);

public:
	void set_enabled(const bool p_enabled);

	void set_enabler_check_box(CheckBox *cb);

	PluginRefresher();
	~PluginRefresher();

protected:
	void _set_enabled(const bool p_enabled);

	void pressed();
	void _gui_input(Ref<InputEvent> p_event);

	static void _bind_methods();

	int _button;
	CheckBox *_enabler_check_box;
};

#endif
