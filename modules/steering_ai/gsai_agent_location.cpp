/*************************************************************************/
/*  gsai_agent_location.cpp                                              */
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

#include "gsai_agent_location.h"

Vector3 GSAIAgentLocation::get_position() {
	return position;
}

void GSAIAgentLocation::set_position(const Vector3 &val) {
	position = val;
}

float GSAIAgentLocation::get_orientation() const {
	return orientation;
}

void GSAIAgentLocation::set_orientation(const float val) {
	orientation = val;
}

GSAIAgentLocation::GSAIAgentLocation() {
	orientation = 0.0;
}

GSAIAgentLocation::~GSAIAgentLocation() {
}

void GSAIAgentLocation::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_position"), &GSAIAgentLocation::get_position);
	ClassDB::bind_method(D_METHOD("set_position", "value"), &GSAIAgentLocation::set_position);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "position"), "set_position", "get_position");

	ClassDB::bind_method(D_METHOD("get_orientation"), &GSAIAgentLocation::get_orientation);
	ClassDB::bind_method(D_METHOD("set_orientation", "value"), &GSAIAgentLocation::set_orientation);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "orientation"), "set_orientation", "get_orientation");
}
