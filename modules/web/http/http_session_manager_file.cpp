/*************************************************************************/
/*  http_session_manager_file.cpp                                        */
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

#include "http_session_manager_file.h"

#include "http_session.h"

#include "web_server.h"
#include "web_server_request.h"

#include "core/os/dir_access.h"
#include "core/os/file_access.h"

#include "core/bind/core_bind.h"

String HTTPSessionManagerFile::get_save_folder_path() {
	return _save_folder_path;
}
void HTTPSessionManagerFile::set_save_folder_path(const String &val) {
	_save_folder_path = val;
}

Ref<HTTPSession> HTTPSessionManagerFile::delete_session(const String &session_id) {
	Ref<HTTPSession> s = HTTPSessionManager::delete_session(session_id);

	if (!s.is_valid()) {
		return s;
	}

	int id = s->get_id();

	if (id <= 0) {
		return s;
	}

	String file = _save_folder_path.plus_file(itos(id) + ".json");

	DirAccess *da = DirAccess::create_for_path(_save_folder_path);
	ERR_FAIL_COND_V(!da, s);
	if (da->file_exists(file)) {
		if (da->remove(file) != OK) {
			ERR_FAIL_V_MSG(s, "Cannot remove file: " + file);
		}
	}
	memdelete(da);

	return s;
}

void HTTPSessionManagerFile::save_session(Ref<HTTPSession> session) {
	ERR_FAIL_COND(!session.is_valid());

	if (!DirAccess::exists(_save_folder_path)) {
		DirAccess *diru = DirAccess::create_for_path(_save_folder_path);

		ERR_FAIL_COND(!diru);

		diru->make_dir_recursive(_save_folder_path);
		memdelete(diru);
	}

	int id = session->get_id();

	if (id <= 0) {
		id = _next_id++;
		session->set_id(id);
	}

	String file = _save_folder_path.plus_file(itos(id) + ".json");
	String data = session->to_json();

	FileAccess *f = FileAccess::open(file, FileAccess::WRITE);

	ERR_FAIL_COND(!f);

	f->store_string(data);
	f->close();

	memdelete(f);
}

void HTTPSessionManagerFile::load_sessions() {
	DirAccess *dir = DirAccess::open(_save_folder_path);

	if (!dir) {
		return;
	}

	clear();
	_next_id = 1;

	dir->list_dir_begin();

	String file = dir->get_next();

	while (file != "") {
		if (!dir->current_is_dir()) {
			if (!file.ends_with(".json")) {
				file = dir->get_next();
				continue;
			}

			FileAccess *f = FileAccess::open(_save_folder_path.plus_file(file), FileAccess::READ);
			ERR_CONTINUE(!f);
			String data = f->get_as_utf8_string();
			f->close();
			memdelete(f);

			Ref<HTTPSession> session;
			session.instance();
			session->from_json(data);

			int id = session->get_id();

			if (id >= _next_id) {
				_next_id = id + 1;
			}

			add_session(session);
		}

		file = dir->get_next();
	}

	dir->list_dir_end();

	memdelete(dir);
}

HTTPSessionManagerFile::HTTPSessionManagerFile() {
	_save_folder_path = "user://sessions/";
	_next_id = 1;
}

HTTPSessionManagerFile::~HTTPSessionManagerFile() {
}

void HTTPSessionManagerFile::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_save_folder_path"), &HTTPSessionManagerFile::get_save_folder_path);
	ClassDB::bind_method(D_METHOD("set_save_folder_path", "val"), &HTTPSessionManagerFile::set_save_folder_path);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "save_folder_path"), "set_save_folder_path", "get_save_folder_path");
}
