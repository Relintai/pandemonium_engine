/*************************************************************************/
/*  input_profile_modifier.cpp                                           */
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

#include "input_profile_modifier.h"

int InputProfileModifier::get_modifier_count() {
	return _modifier_actions->size();
}

void InputProfileModifier::clear_modifiers() {
	_modifier_actions->clear();
}

void InputProfileModifier::add_modifier(String modifier) {
	_modifier_actions->push_back(modifier);
}

String InputProfileModifier::get_modifier(int index) {
	ERR_FAIL_INDEX_V(index, _modifier_actions->size(), "");

	return _modifier_actions->get(index);
}

void InputProfileModifier::set_modifier(int index, String value) {
	ERR_FAIL_INDEX(index, _modifier_actions->size());

	_modifier_actions->set(index, value);
}

void InputProfileModifier::remove_modifier(int index) {
	ERR_FAIL_INDEX(index, _modifier_actions->size());

	_modifier_actions->remove(index);
}

int InputProfileModifier::get_entry_count() {
	return _entries->size();
}

void InputProfileModifier::clear_entries() {
	_entries->clear();
}

void InputProfileModifier::add_entry(Ref<InputProfileModifierEntry> modifier) {
	_entries->push_back(Ref<InputProfileModifierEntry>(modifier));
}

Ref<InputProfileModifierEntry> InputProfileModifier::get_entry(int index) {
	ERR_FAIL_INDEX_V(index, _entries->size(), Ref<InputProfileModifierEntry>());

	return _entries->get(index);
}

void InputProfileModifier::set_entry(int index, Ref<InputProfileModifierEntry> value) {
	ERR_FAIL_INDEX(index, _entries->size());

	_entries->set(index, Ref<InputProfileModifierEntry>(value));
}

void InputProfileModifier::remove_entry(int index) {
	ERR_FAIL_INDEX(index, _entries->size());

	_entries->remove(index);
}

InputProfileModifier::InputProfileModifier() {
	_modifier_actions = memnew(Vector<String>());
	_entries = memnew(Vector<Ref<InputProfileModifierEntry>>());
}

InputProfileModifier::~InputProfileModifier() {
	_modifier_actions->clear();
	_entries->clear();

	memdelete(_modifier_actions);
	memdelete(_entries);
}

void InputProfileModifier::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_modifier_count"), &InputProfileModifier::get_modifier_count);
	ClassDB::bind_method(D_METHOD("clear_modifiers"), &InputProfileModifier::clear_modifiers);
	ClassDB::bind_method(D_METHOD("add_modifier", "modifier"), &InputProfileModifier::add_modifier);
	ClassDB::bind_method(D_METHOD("get_modifier", "index"), &InputProfileModifier::get_modifier);
	ClassDB::bind_method(D_METHOD("set_modifier", "index", "value"), &InputProfileModifier::set_modifier);
	ClassDB::bind_method(D_METHOD("remove_modifier", "index"), &InputProfileModifier::remove_modifier);

	ClassDB::bind_method(D_METHOD("get_entry_count"), &InputProfileModifier::get_entry_count);
	ClassDB::bind_method(D_METHOD("clear_entries"), &InputProfileModifier::clear_entries);
	ClassDB::bind_method(D_METHOD("add_entry", "entry"), &InputProfileModifier::add_entry);
	ClassDB::bind_method(D_METHOD("get_entry", "index"), &InputProfileModifier::get_entry);
	ClassDB::bind_method(D_METHOD("set_entry", "index", "value"), &InputProfileModifier::set_entry);
	ClassDB::bind_method(D_METHOD("remove_entry", "index"), &InputProfileModifier::remove_entry);
}
