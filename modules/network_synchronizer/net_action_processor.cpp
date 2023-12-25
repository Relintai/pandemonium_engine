/*************************************************************************/
/*  net_action_processor.cpp                                             */
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

#include "net_action_processor.h"

#include "input_network_encoder.h"
#include "net_action_info.h"
#include "net_utilities.h"
#include "scene/main/node.h"

void NetActionProcessor::execute() {
	const NetActionInfo &info = nd->net_actions[action_id];
	nd->node->callv(info.act_func, vars);
}

bool NetActionProcessor::server_validate() const {
	const NetActionInfo &info = nd->net_actions[action_id];

	if (info.server_action_validation_func == StringName()) {
		// Always valid when the func is not set!
		return true;
	}

	const Variant is_valid = nd->node->callv(info.server_action_validation_func, vars);

	ERR_FAIL_COND_V_MSG(is_valid.get_type() != Variant::BOOL, false, "[FATAL] The function `" + nd->node->get_path() + "::" + info.server_action_validation_func + "` MUST return a bool.");

	return is_valid.operator bool();
}

NetActionProcessor::operator String() const {
	const NetActionInfo &info = nd->net_actions[action_id];
	String v = Variant(vars);
	// Strip `[]` from the Array string.
	v = v.substr(1, v.size() - 3);
	return String(nd->node->get_path()) + "::" + info.act_func + "(" + v + ")";
}