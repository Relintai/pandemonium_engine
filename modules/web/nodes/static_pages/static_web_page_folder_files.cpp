/*************************************************************************/
/*  static_web_page_folder_files.cpp                                     */
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

#include "static_web_page_folder_files.h"

#include "../../html/markdown_renderer.h"
#include "core/os/dir_access.h"
#include "core/os/file_access.h"

String StaticWebPageFolderFiles::get_dir_path() {
	return _dir_path;
}
void StaticWebPageFolderFiles::set_dir_path(const String &val) {
	_dir_path = val;
}

bool StaticWebPageFolderFiles::get_process_if_can() {
	return _process_if_can;
}
void StaticWebPageFolderFiles::set_process_if_can(const bool &val) {
	_process_if_can = val;
}

void StaticWebPageFolderFiles::load() {
	if (_dir_path == "") {
		return;
	}

	DirAccessRef da = DirAccess::open(_dir_path);

	ERR_FAIL_COND_MSG(!da, "Dir Path = " + _dir_path);

	if (da) {
		Ref<MarkdownRenderer> r;
		r.instance();

		da->list_dir_begin();

		while (true) {
			String file = da->get_next();
			if (file == "") {
				break;
			}

			if (!da->current_is_dir() && !da->current_is_hidden()) {
				String fn = _dir_path;
				fn = fn.append_path(file);

				FileAccess *f = FileAccess::open(fn, FileAccess::READ);

				if (f) {
					String str;
					str = f->get_as_utf8_string();
					f->close();
					memdelete(f);

					if (_process_if_can && (fn.get_extension()) == "md") {
						str = r->render_to_html(str);
					}

					append_data(str);
				}
			}
		}

		da->list_dir_end();
	}
}

void StaticWebPageFolderFiles::append_data(const String &d) {
	_data += d;
}

void StaticWebPageFolderFiles::_notification(const int what) {
	switch (what) {
		case NOTIFICATION_ENTER_TREE:
			load();
			break;
		default:
			break;
	}
}

StaticWebPageFolderFiles::StaticWebPageFolderFiles() {
	_process_if_can = true;
}

StaticWebPageFolderFiles::~StaticWebPageFolderFiles() {
}

void StaticWebPageFolderFiles::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_dir_path"), &StaticWebPageFolderFiles::get_dir_path);
	ClassDB::bind_method(D_METHOD("set_dir_path", "val"), &StaticWebPageFolderFiles::set_dir_path);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "dir_path"), "set_dir_path", "get_dir_path");

	ClassDB::bind_method(D_METHOD("get_process_if_can"), &StaticWebPageFolderFiles::get_process_if_can);
	ClassDB::bind_method(D_METHOD("set_process_if_can", "val"), &StaticWebPageFolderFiles::set_process_if_can);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "process_if_can"), "set_process_if_can", "get_process_if_can");

	ClassDB::bind_method(D_METHOD("load"), &StaticWebPageFolderFiles::load);
	ClassDB::bind_method(D_METHOD("append_data", "val"), &StaticWebPageFolderFiles::append_data);
}
