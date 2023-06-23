/**************************************************************************/
/*  pluginscript_instance.cpp                                             */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             PANDEMONIUM ENGINE                               */
/*                        https://pandemoniumengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "pluginscript_instance.h"

// Pandemonium imports
#include "core/os/os.h"
#include "core/variant/variant.h"

// PluginScript imports
#include "pluginscript_language.h"
#include "pluginscript_script.h"

bool PluginScriptInstance::set(const StringName &p_name, const Variant &p_value) {
	String name = String(p_name);
	return _desc->set_prop(_data, (const pandemonium_string *)&name, (const pandemonium_variant *)&p_value);
}

bool PluginScriptInstance::get(const StringName &p_name, Variant &r_ret) const {
	String name = String(p_name);
	return _desc->get_prop(_data, (const pandemonium_string *)&name, (pandemonium_variant *)&r_ret);
}

Ref<Script> PluginScriptInstance::get_script() const {
	return _script;
}

ScriptLanguage *PluginScriptInstance::get_language() {
	return _script->get_language();
}

Variant::Type PluginScriptInstance::get_property_type(const StringName &p_name, bool *r_is_valid) const {
	if (!_script->has_property(p_name)) {
		if (r_is_valid) {
			*r_is_valid = false;
		}
		return Variant::NIL;
	}
	if (r_is_valid) {
		*r_is_valid = true;
	}
	return _script->get_property_info(p_name).type;
}

void PluginScriptInstance::get_property_list(List<PropertyInfo> *p_properties) const {
	_script->get_script_property_list(p_properties);
}

void PluginScriptInstance::get_method_list(List<MethodInfo> *p_list) const {
	_script->get_script_method_list(p_list);
}

bool PluginScriptInstance::has_method(const StringName &p_method) const {
	return _script->has_method(p_method);
}

Variant PluginScriptInstance::call(const StringName &p_method, const Variant **p_args, int p_argcount, Variant::CallError &r_error) {
	// TODO: optimize when calling a Pandemonium method from Pandemonium to avoid param conversion ?
	pandemonium_variant ret = _desc->call_method(
			_data, (pandemonium_string_name *)&p_method, (const pandemonium_variant **)p_args,
			p_argcount, (pandemonium_variant_call_error *)&r_error);
	Variant var_ret = *(Variant *)&ret;
	pandemonium_variant_destroy(&ret);
	return var_ret;
}

void PluginScriptInstance::notification(int p_notification) {
	_desc->notification(_data, p_notification);
}

void PluginScriptInstance::refcount_incremented() {
	if (_desc->refcount_decremented) {
		_desc->refcount_incremented(_data);
	}
}

bool PluginScriptInstance::refcount_decremented() {
	// Return true if it can die
	if (_desc->refcount_decremented) {
		return _desc->refcount_decremented(_data);
	}
	return true;
}

PluginScriptInstance::PluginScriptInstance() {
}

bool PluginScriptInstance::init(PluginScript *p_script, Object *p_owner) {
	_owner = p_owner;
	_owner_variant = Variant(p_owner);
	_script = Ref<PluginScript>(p_script);
	_desc = &p_script->_desc->instance_desc;
	_data = _desc->init(p_script->_data, (pandemonium_object *)p_owner);
	ERR_FAIL_COND_V(_data == nullptr, false);
	p_owner->set_script_instance(this);
	return true;
}

PluginScriptInstance::~PluginScriptInstance() {
	_desc->finish(_data);
	_script->_language->lock();
	_script->_instances.erase(_owner);
	_script->_language->unlock();
}
