/*************************************************************************/
/*  folder_serve_web_page.cpp                                            */
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

#include "folder_serve_web_page.h"

#include "../../file_cache.h"
#include "../../http/web_permission.h"
#include "../../http/web_server_request.h"

String FolderServeWebPage::get_serve_folder() {
	return _serve_folder;
}
void FolderServeWebPage::set_serve_folder(const String &val) {
	_serve_folder = val;
}

Ref<FileCache> FolderServeWebPage::get_file_cache() const {
	return _file_cache;
}

void FolderServeWebPage::_handle_request_main(Ref<WebServerRequest> request) {
	if (_web_permission.is_valid()) {
		if (_web_permission->activate(request)) {
			return;
		}
	}

	const String &rp = request->get_current_path_segment();

	if (rp == "") {
		if (!try_route_request_to_children(request)) {
			handle_request(request);
		}
		return;
	}

	String file_name = request->get_path(true, false);

	String file_path = _file_cache->wwwroot_get_file_abspath(file_name);

	if (!file_path.empty()) {
		request->send_file(file_path);
		return;
	}

	if (!try_route_request_to_children(request)) {
		handle_request(request);
	}
}

void FolderServeWebPage::load() {
	_file_cache->clear();

	_file_cache->set_wwwroot(_serve_folder);
}

void FolderServeWebPage::_notification(const int what) {
	switch (what) {
		case NOTIFICATION_ENTER_TREE:
			load();
			break;
		default:
			break;
	}
}

FolderServeWebPage::FolderServeWebPage() {
	_file_cache.instance();
}

FolderServeWebPage::~FolderServeWebPage() {
}

void FolderServeWebPage::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_serve_folder"), &FolderServeWebPage::get_serve_folder);
	ClassDB::bind_method(D_METHOD("set_serve_folder", "val"), &FolderServeWebPage::set_serve_folder);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "serve_folder"), "set_serve_folder", "get_serve_folder");

	ClassDB::bind_method(D_METHOD("get_file_cache"), &FolderServeWebPage::get_file_cache);

	ClassDB::bind_method(D_METHOD("load"), &FolderServeWebPage::load);
}
