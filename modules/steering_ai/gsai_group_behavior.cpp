/*************************************************************************/
/*  gsai_group_behavior.cpp                                              */
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

#include "gsai_group_behavior.h"

#include "gsai_steering_agent.h"
#include "proximities/gsai_proximity.h"

Ref<GSAIProximity> GSAIGroupBehavior::get_proximity() {
	return proximity;
}

void GSAIGroupBehavior::set_proximity(const Ref<GSAIProximity> &val) {
	proximity = val;
}

Ref<FuncRef> GSAIGroupBehavior::get_callback() {
	return _callback;
}

void GSAIGroupBehavior::set_callback(const Ref<FuncRef> &val) {
	_callback = val;
}

bool GSAIGroupBehavior::_report_neighbor(Ref<GSAISteeringAgent> neighbor) {
	return false;
}

GSAIGroupBehavior::GSAIGroupBehavior() {
	_callback.instance();
	_callback->set_instance(this);
	_callback->set_function("_report_neighbor");
}

GSAIGroupBehavior::~GSAIGroupBehavior() {
}

void GSAIGroupBehavior::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_proximity"), &GSAIGroupBehavior::get_proximity);
	ClassDB::bind_method(D_METHOD("set_proximity", "value"), &GSAIGroupBehavior::set_proximity);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "proximity", PROPERTY_HINT_RESOURCE_TYPE, "GSAIProximity"), "set_proximity", "get_proximity");

	ClassDB::bind_method(D_METHOD("get_callback"), &GSAIGroupBehavior::get_callback);
	ClassDB::bind_method(D_METHOD("set_callback", "value"), &GSAIGroupBehavior::set_callback);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "_callback", PROPERTY_HINT_RESOURCE_TYPE, "FuncRef"), "set_callback", "get_callback");

	BIND_VMETHOD(MethodInfo(Variant::BOOL, "_report_neighbor", PropertyInfo(Variant::OBJECT, "neighbor", PROPERTY_HINT_RESOURCE_TYPE, "GSAISteeringAgent")));
	ClassDB::bind_method(D_METHOD("_report_neighbor", "neighbor"), &GSAIGroupBehavior::_report_neighbor);
}
