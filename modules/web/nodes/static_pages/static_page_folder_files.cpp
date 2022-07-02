#include "static_page_folder_files.h"

#include "core/os/dir_access.h"
#include "core/os/file_access.h"

String StaticPageFolderFiles::get_dir_path() {
	return _dir_path;
}
void StaticPageFolderFiles::set_dir_path(const String &val) {
	_dir_path = val;
}

bool StaticPageFolderFiles::get_process_if_can() {
	return _process_if_can;
}
void StaticPageFolderFiles::set_process_if_can(const bool &val) {
	_process_if_can = val;
}

void StaticPageFolderFiles::load() {
	if (_dir_path == "") {
		return;
	}

	DirAccessRef da = DirAccess::open(_dir_path);

	ERR_FAIL_COND_MSG(!da, "Dir Path = " + _dir_path);

	if (da) {
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

					//if (process_if_can && d->current_get_extension() == "md") {
					//	Utils::markdown_to_html(&str);
					//}

					append_data(str);
				}
			}
		}

		da->list_dir_end();
	}
}

void StaticPageFolderFiles::append_data(const String &d) {
	_data += d;
}

void StaticPageFolderFiles::_notification(const int what) {
	switch (what) {
		case NOTIFICATION_ENTER_TREE:
			load();
			break;
		default:
			break;
	}
}

StaticPageFolderFiles::StaticPageFolderFiles() {
	_process_if_can = true;
}

StaticPageFolderFiles::~StaticPageFolderFiles() {
}

void StaticPageFolderFiles::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_dir_path"), &StaticPageFolderFiles::get_dir_path);
	ClassDB::bind_method(D_METHOD("set_dir_path", "val"), &StaticPageFolderFiles::set_dir_path);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "dir_path"), "set_dir_path", "get_dir_path");

	ClassDB::bind_method(D_METHOD("get_process_if_can"), &StaticPageFolderFiles::get_process_if_can);
	ClassDB::bind_method(D_METHOD("set_process_if_can", "val"), &StaticPageFolderFiles::set_process_if_can);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "process_if_can"), "set_process_if_can", "get_process_if_can");

	ClassDB::bind_method(D_METHOD("load"), &StaticPageFolderFiles::load);
	ClassDB::bind_method(D_METHOD("append_data", "val"), &StaticPageFolderFiles::append_data);
}
