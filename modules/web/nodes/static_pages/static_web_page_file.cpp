#include "static_web_page_file.h"

String StaticWebPageFile::get_file_path() {
	return _file_path;
}
void StaticWebPageFile::set_file_path(const String &val) {
	_file_path = val;
}

bool StaticWebPageFile::get_process_if_can() {
	return _process_if_can;
}
void StaticWebPageFile::set_process_if_can(const bool &val) {
	_process_if_can = val;
}

void StaticWebPageFile::load() {
	if (_file_path == "") {
		return;
	}

	if (_process_if_can) {
		load_and_process_file(_file_path);
	} else {
		load_file(_file_path);
	}
}

void StaticWebPageFile::_notification(const int what) {
	switch (what) {
		case NOTIFICATION_ENTER_TREE:
			load();
			break;
		default:
			break;
	}
}

StaticWebPageFile::StaticWebPageFile() {
	_process_if_can = true;
}

StaticWebPageFile::~StaticWebPageFile() {
}

void StaticWebPageFile::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_file_path"), &StaticWebPageFile::get_file_path);
	ClassDB::bind_method(D_METHOD("set_file_path", "val"), &StaticWebPageFile::set_file_path);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "file_path"), "set_file_path", "get_file_path");

	ClassDB::bind_method(D_METHOD("get_process_if_can"), &StaticWebPageFile::get_process_if_can);
	ClassDB::bind_method(D_METHOD("set_process_if_can", "val"), &StaticWebPageFile::set_process_if_can);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "process_if_can"), "set_process_if_can", "get_process_if_can");

	ClassDB::bind_method(D_METHOD("load"), &StaticWebPageFile::load);
}
