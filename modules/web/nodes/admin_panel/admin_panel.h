#ifndef ADMIN_PANEL_H
#define ADMIN_PANEL_H

/*************************************************************************/
/*  admin_panel.h                                                        */
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

#include "web/http/web_node.h"

#include "core/containers/vector.h"
#include "core/string.h"

class Request;
class FormValidator;
class AdminNode;

class AdminPanel : public WebNode {
	RCPP_OBJECT(AdminPanel, WebNode);

public:
	void handle_request_main(Request *request);

	virtual void render_admin_panel_list(Request *request);
	virtual void render_controller_panel(Request *request, AdminNode *controller);

	void register_admin_controller(const String &section, AdminNode *controller);
	void clear();

	virtual void render_headers(Request *request);
	virtual void render_main_body_top(Request *request);
	virtual void render_segment_body_top(Request *request);
	virtual void render_footer(Request *request);

	void set_default_header(const String &val);
	void set_default_main_body_top(const String &val);
	void set_default_segment_body_top(const String &val);
	void set_default_footer(const String &val);

	static AdminPanel *get_singleton();

	AdminPanel();
	~AdminPanel();

protected:
	struct AdminPanelSection {
		String section_url;
		String name;
		AdminNode *controller;
	};

	static AdminPanel *_self;

	Vector<AdminPanelSection> _controllers;

	String _default_headers;
	String _default_main_body_top;
	String _default_segment_body_top;
	String _default_footer;
};

#endif
