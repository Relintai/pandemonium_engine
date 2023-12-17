/*************************************************************************/
/*  static_web_page_file.cpp                                             */
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
