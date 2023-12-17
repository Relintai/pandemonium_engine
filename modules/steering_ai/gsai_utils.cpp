/*************************************************************************/
/*  gsai_utils.cpp                                                       */
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

#include "gsai_utils.h"

GSAIUtils *GSAIUtils::_singleton = NULL;

GSAIUtils::GSAIUtils() {
	_singleton = this;
}

GSAIUtils::~GSAIUtils() {
	_singleton = NULL;
}

void GSAIUtils::_bind_methods() {
	ClassDB::bind_method(D_METHOD("clampedv3", "vector", "limit"), &GSAIUtils::bclampedv3);
	ClassDB::bind_method(D_METHOD("vector3_to_angle", "vector"), &GSAIUtils::bvector3_to_angle);
	ClassDB::bind_method(D_METHOD("vector2_to_angle", "vector"), &GSAIUtils::bvector2_to_angle);
	ClassDB::bind_method(D_METHOD("angle_to_vector2", "angle"), &GSAIUtils::bangle_to_vector2);
	ClassDB::bind_method(D_METHOD("to_vector2", "vector"), &GSAIUtils::bto_vector2);
	ClassDB::bind_method(D_METHOD("to_vector3", "vector"), &GSAIUtils::bto_vector3);
}
