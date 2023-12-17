#ifndef WEB_NODE_EDITOR_H
#define WEB_NODE_EDITOR_H

/*************************************************************************/
/*  web_node_editor.h                                                    */
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

#include "core/object/reference.h"

#include "scene/gui/box_container.h"

class WebNode;
class WebNodeEditorWebServer;
class RichTextLabel;
class TextEdit;
class MarginContainer;
class Button;
class ButtonGroup;

class WebNodeEditor : public VBoxContainer {
	GDCLASS(WebNodeEditor, VBoxContainer);

public:
	void add_control_to_tool_bar(Control *control);
	void remove_control_from_tool_bar(Control *control);

	void add_main_screen_tab(Control *control);
	void remove_main_screen_tab(Control *control);
	void switch_to_main_screen_tab(Control *control);

	Ref<ButtonGroup> get_main_button_group();

	void edit(WebNode *web_node);
	void refresh_html_preview();
	void clear();

	static WebNodeEditor *get_singleton() { return _singleton; }

	WebNodeEditor();
	~WebNodeEditor();

protected:
	void _on_html_previewer_tool_button_toggled(bool on);
	void _on_html_previewer_visibility_changed();

	void _notification(int p_what);

	static void _bind_methods();
	bool _prettify_html;

	WebNode *_edited_node;
	WebNodeEditorWebServer *_web_server;
	HBoxContainer *_toolbar;
	MarginContainer *_main_container;

	Ref<ButtonGroup> _main_button_group;
	Button *_html_previewer_tool_button;
	VBoxContainer *_html_previewer;
	RichTextLabel *_result_info_label;
	TextEdit *_results_label;

private:
	static WebNodeEditor *_singleton;
};

#endif
