/*************************************************************************/
/*  gsai_infinite_proximity.cpp                                          */
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

#include "gsai_infinite_proximity.h"

#include "../gsai_steering_agent.h"
#include "scene/main/scene_tree.h"

int GSAIInfiniteProximity::_find_neighbors(Ref<FuncRef> callback) {
	int neighbor_count = 0;
	int agent_count = agents.size();
	Variant arg;
	const Variant *argptr[1];
	argptr[0] = &arg;
	Variant::CallError err;

	for (int i = 0; i < agent_count; ++i) {
		Ref<GSAISteeringAgent> current_agent = agents[i];

		ERR_CONTINUE(!current_agent.is_valid());

		if (current_agent != agent) {
			if (callback->call_func(argptr, 1, err)) {
				neighbor_count += 1;
			}
		}
	}

	return neighbor_count;
}

GSAIInfiniteProximity::GSAIInfiniteProximity() {
}

GSAIInfiniteProximity::~GSAIInfiniteProximity() {
}

void GSAIInfiniteProximity::_bind_methods() {
}
