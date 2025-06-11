/*************************************************************************/
/*  browsable_folder_serve_web_page.cpp                                  */
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

#include "browsable_folder_serve_web_page.h"

#include "core/os/dir_access.h"

#include "../../html/html_builder.h"

#include "../../file_cache.h"
#include "../../http/web_permission.h"
#include "../../http/web_server_request.h"

#include "../../http/http_server_enums.h"

bool BrowsableFolderServeWebPage::get_should_render_menu() {
	return _should_render_menu;
}
void BrowsableFolderServeWebPage::set_should_render_menu(const bool &val) {
	_should_render_menu = val;
}

void BrowsableFolderServeWebPage::_handle_request(Ref<WebServerRequest> request) {
	String file_name = request->get_path(true, false);

	String *data = _folder_indexes.getptr(file_name);

	if (!data) {
		request->send_error(HTTPServerEnums::HTTP_STATUS_CODE_404_NOT_FOUND);
		return;
	}

	if (_should_render_menu) {
		render_menu(request);
	}

	request->body += *data;
	request->compile_and_send_body();
}

void BrowsableFolderServeWebPage::_render_index(Ref<WebServerRequest> request) {
	request->body += _index;
}
void BrowsableFolderServeWebPage::_render_preview(Ref<WebServerRequest> request) {
}

void BrowsableFolderServeWebPage::load() {
	FolderServeWebPage::load();

	if (_serve_folder == "") {
		return;
	}

	evaluate_dir(_file_cache->get_wwwroot_abs(), true);
}

void BrowsableFolderServeWebPage::evaluate_dir(const String &path, const bool top_level) {
	DirAccess *dir = DirAccess::open(path);

	ERR_FAIL_COND_MSG(!dir, "Error opening folde!r: " + String(path));

	String serve_folder = _file_cache->get_wwwroot_abs();

	String dir_uri;

	if (!top_level) {
		dir_uri = path.substr(serve_folder.length(), path.length() - serve_folder.length());
	} else {
		dir_uri = "/";
	}

	Vector<String> folders;
	Vector<String> files;

	dir->list_dir_begin();

	String file = dir->get_next();

	while (!file.empty()) {
		String np = path.append_path(file);
		String nnp = np.substr(serve_folder.length(), np.length() - serve_folder.length());

		if (dir->current_is_dir()) {
			if (file == "." || file == "..") {
				file = dir->get_next();
				continue;
			}

			folders.push_back(nnp);
			evaluate_dir(np);
		} else {
			files.push_back(nnp);
		}

		file = dir->get_next();
	}

	dir->list_dir_end();

	memdelete(dir);

	folders.sort();
	files.sort();

	render_dir_page(dir_uri, folders, files, top_level);
}

void BrowsableFolderServeWebPage::render_dir_page(const String &dir_uri, const Vector<String> &folders, const Vector<String> &files, const bool top_level) {
	HTMLBuilder b;

	String uri = get_full_uri(false);

	b.div("file_list");
	{
		if (!top_level) {
			b.div("file_list_entry");
			{
				b.a(uri + dir_uri.path_get_prev_dir())->w("..")->ca();
			}
			b.cdiv();
		}

		for (int i = 0; i < folders.size(); ++i) {
			b.div("file_list_entry");
			{
				b.a(uri + folders[i])->w("(Folder) ")->w(folders[i].get_file())->ca();
			}
			b.cdiv();
		}

		for (int i = 0; i < files.size(); ++i) {
			b.div("file_list_entry");
			{
				b.a(uri + files[i])->w("(File) ")->w(files[i].get_file())->ca();
			}
			b.cdiv();
		}
	}
	b.cdiv();

	_folder_indexes[dir_uri] = b.get_result();

	if (dir_uri == "/") {
		_index = b.get_result();
	}
}

BrowsableFolderServeWebPage::BrowsableFolderServeWebPage() {
	_should_render_menu = true;
}

BrowsableFolderServeWebPage::~BrowsableFolderServeWebPage() {
	_folder_indexes.clear();
}
void BrowsableFolderServeWebPage::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_should_render_menu"), &BrowsableFolderServeWebPage::get_should_render_menu);
	ClassDB::bind_method(D_METHOD("set_should_render_menu", "val"), &BrowsableFolderServeWebPage::set_should_render_menu);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "should_render_menu"), "set_should_render_menu", "get_should_render_menu");
}
