/*************************************************************************/
/*  static_web_page.cpp                                                  */
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

#include "static_web_page.h"

#include "../../html/markdown_renderer.h"
#include "../../http/web_server_request.h"
#include "core/os/file_access.h"

String StaticWebPage::get_data() {
	return _data;
}
void StaticWebPage::set_data(const String &val) {
	_data = val;
}

String StaticWebPage::get_preview_data() {
	return _preview_data;
}
void StaticWebPage::set_preview_data(const String &val) {
	_preview_data = val;
}

bool StaticWebPage::get_should_render_menu() {
	return _should_render_menu;
}
void StaticWebPage::set_should_render_menu(const bool &val) {
	_should_render_menu = val;
}

void StaticWebPage::_handle_request(Ref<WebServerRequest> request) {
	if (_should_render_menu) {
		render_menu(request);
	}

	render_index(request);
	request->compile_and_send_body();
}

void StaticWebPage::_render_index(Ref<WebServerRequest> request) {
	request->body += _data;
}

void StaticWebPage::_render_preview(Ref<WebServerRequest> request) {
	request->body += _preview_data;
}

void StaticWebPage::load_file(const String &path) {
	FileAccess *f = FileAccess::open(path, FileAccess::READ);

	if (f) {
		_data = f->get_as_utf8_string();
		f->close();
		memdelete(f);
	} else {
		_data = "";
	}
}

void StaticWebPage::load_and_process_file(const String &path) {
	FileAccess *f = FileAccess::open(path, FileAccess::READ);

	if (f) {
		_data = f->get_as_utf8_string();
		f->close();
		memdelete(f);
	} else {
		_data = "";
	}

	if (path.get_extension() == "md") {
		Ref<MarkdownRenderer> r;
		r.instance();
		_data = r->render_to_html(_data);
	}
}

void StaticWebPage::load_md_file(const String &path) {
	FileAccess *f = FileAccess::open(path, FileAccess::READ);

	if (f) {
		_data = f->get_as_utf8_string();
		f->close();
		memdelete(f);

		Ref<MarkdownRenderer> r;
		r.instance();
		_data = r->render_to_html(_data);
	} else {
		_data = "";
	}
}
void StaticWebPage::set_data_md(const String &data) {
	Ref<MarkdownRenderer> r;
	r.instance();
	_data = r->render_to_html(data);
}

StaticWebPage::StaticWebPage() {
	_should_render_menu = true;
}

StaticWebPage::~StaticWebPage() {
}

void StaticWebPage::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_data"), &StaticWebPage::get_data);
	ClassDB::bind_method(D_METHOD("set_data", "val"), &StaticWebPage::set_data);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "data", PROPERTY_HINT_MULTILINE_TEXT), "set_data", "get_data");

	ClassDB::bind_method(D_METHOD("get_preview_data"), &StaticWebPage::get_preview_data);
	ClassDB::bind_method(D_METHOD("set_preview_data", "val"), &StaticWebPage::set_preview_data);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "preview_data", PROPERTY_HINT_MULTILINE_TEXT), "set_preview_data", "get_preview_data");

	ClassDB::bind_method(D_METHOD("get_should_render_menu"), &StaticWebPage::get_should_render_menu);
	ClassDB::bind_method(D_METHOD("set_should_render_menu", "val"), &StaticWebPage::set_should_render_menu);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "should_render_menu"), "set_should_render_menu", "get_should_render_menu");

	ClassDB::bind_method(D_METHOD("load_file", "path"), &StaticWebPage::load_file);
	ClassDB::bind_method(D_METHOD("load_and_process_file", "path"), &StaticWebPage::load_and_process_file);

	ClassDB::bind_method(D_METHOD("load_md_file", "path"), &StaticWebPage::load_md_file);
	ClassDB::bind_method(D_METHOD("set_data_md", "path"), &StaticWebPage::set_data_md);
}
