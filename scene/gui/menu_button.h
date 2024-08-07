#ifndef MENU_BUTTON_H
#define MENU_BUTTON_H

/*************************************************************************/
/*  menu_button.h                                                        */
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

#include "scene/gui/button.h"

class PopupMenu;

class MenuButton : public Button {
	GDCLASS(MenuButton, Button);

	bool clicked;
	bool switch_on_hover;
	bool disable_shortcuts;
	PopupMenu *popup;

	void _shortcut_input(Ref<InputEvent> p_event);
	Array _get_items() const;
	void _set_items(const Array &p_items);

	void _gui_input(Ref<InputEvent> p_event);

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	virtual void pressed();

	PopupMenu *get_popup() const;
	void set_switch_on_hover(bool p_enabled);
	bool is_switch_on_hover();
	void set_disable_shortcuts(bool p_disabled);

	MenuButton();
	~MenuButton();
};

#endif
