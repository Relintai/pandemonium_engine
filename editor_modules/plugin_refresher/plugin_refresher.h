#ifndef PLUGIN_REFRESHER_H
#define PLUGIN_REFRESHER_H

/*************************************************************************/
/*  plugin_refresher.h                                                   */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "core/containers/vector.h"
#include "core/object/reference.h"
#include "core/string/ustring.h"
#include "core/variant/variant.h"

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
	void _on_popup_confirmed();
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
