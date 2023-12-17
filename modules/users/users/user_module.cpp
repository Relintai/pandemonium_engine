/*************************************************************************/
/*  user_module.cpp                                                      */
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

#include "user_module.h"
#include "core/object/class_db.h"

#include "core/object/object.h"
#include "user.h"

int UserModule::get_module_index() const {
	return _module_index;
}
void UserModule::set_module_index(const int val) {
	_module_index = val;
}

String UserModule::get_module_name() const {
	return _module_name;
}
void UserModule::set_module_name(const String &val) {
	_module_name = val;
}

Ref<User> UserModule::get_user() {
	return Ref<User>(_user);
}
void UserModule::set_user(User *user) {
	_user = user;
}

Dictionary UserModule::to_dict() {
	return call("_to_dict");
}
void UserModule::from_dict(const Dictionary &dict) {
	call("_from_dict", dict);
}

Dictionary UserModule::_to_dict() {
	Dictionary dict;

	dict["module_name"] = _module_name;

	return dict;
}
void UserModule::_from_dict(const Dictionary &dict) {
	_module_name = dict["module_name"];
}

void UserModule::read_lock() {
	_rw_lock.read_lock();
}
void UserModule::read_unlock() {
	_rw_lock.read_unlock();
}
void UserModule::write_lock() {
	_rw_lock.write_lock();
}
void UserModule::write_unlock() {
	_rw_lock.write_unlock();
}

UserModule::UserModule() {
	_module_index = -1;
	_user = nullptr;
}

UserModule::~UserModule() {
}

void UserModule::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_module_index"), &UserModule::get_module_index);

	ClassDB::bind_method(D_METHOD("get_module_name"), &UserModule::get_module_name);
	ClassDB::bind_method(D_METHOD("set_module_name", "val"), &UserModule::set_module_name);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "module_name"), "set_module_name", "get_module_name");

	ClassDB::bind_method(D_METHOD("get_user"), &UserModule::get_user);

	BIND_VMETHOD(MethodInfo("_from_dict", PropertyInfo(Variant::DICTIONARY, "dict")));
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::DICTIONARY, "dict"), "_to_dict"));
	ClassDB::bind_method(D_METHOD("from_dict", "dict"), &UserModule::from_dict);
	ClassDB::bind_method(D_METHOD("to_dict"), &UserModule::to_dict);
	ClassDB::bind_method(D_METHOD("_from_dict", "dict"), &UserModule::_from_dict);
	ClassDB::bind_method(D_METHOD("_to_dict"), &UserModule::_to_dict);

	ClassDB::bind_method(D_METHOD("read_lock"), &UserModule::read_lock);
	ClassDB::bind_method(D_METHOD("read_unlock"), &UserModule::read_unlock);
	ClassDB::bind_method(D_METHOD("write_lock"), &UserModule::write_lock);
	ClassDB::bind_method(D_METHOD("write_unlock"), &UserModule::write_unlock);
}
