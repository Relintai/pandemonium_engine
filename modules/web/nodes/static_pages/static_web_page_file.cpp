#include "static_web_page_file.h"

String StaticPageFile::get_file_path() {
	return _file_path;
}
void StaticPageFile::set_file_path(const String &val) {
	_file_path = val;
}

bool StaticPageFile::get_process_if_can() {
	return _process_if_can;
}
void StaticPageFile::set_process_if_can(const bool &val) {
	_process_if_can = val;
}

void StaticPageFile::load() {
	if (_file_path == "") {
		return;
	}

	if (_process_if_can) {
		load_and_process_file(_file_path);
	} else {
		load_file(_file_path);
	}
}

void StaticPageFile::_notification(const int what) {
	switch (what) {
		case NOTIFICATION_ENTER_TREE:
			load();
			break;
		default:
			break;
	}
}

StaticPageFile::StaticPageFile() {
	_process_if_can = true;
}

StaticPageFile::~StaticPageFile() {
}

void StaticPageFile::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_file_path"), &StaticPageFile::get_file_path);
	ClassDB::bind_method(D_METHOD("set_file_path", "val"), &StaticPageFile::set_file_path);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "file_path"), "set_file_path", "get_file_path");

	ClassDB::bind_method(D_METHOD("get_process_if_can"), &StaticPageFile::get_process_if_can);
	ClassDB::bind_method(D_METHOD("set_process_if_can", "val"), &StaticPageFile::set_process_if_can);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "process_if_can"), "set_process_if_can", "get_process_if_can");

	ClassDB::bind_method(D_METHOD("load"), &StaticPageFile::load);
}
