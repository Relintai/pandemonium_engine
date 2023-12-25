#ifndef NET_ACTION_PROCESSOR_H
#define NET_ACTION_PROCESSOR_H

/*************************************************************************/
/*  net_action_processor.h                                               */
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

#include "core/string/ustring.h"
#include "core/variant/array.h"

namespace NetUtility {
struct NodeData;
};

typedef uint32_t NetActionId;

struct NetActionProcessor {
	NetUtility::NodeData *nd;
	NetActionId action_id;
	Array vars;

	NetActionProcessor() = default;
	NetActionProcessor(
			NetUtility::NodeData *p_nd,
			NetActionId p_action_id,
			const Array &p_vars) :
			nd(p_nd),
			action_id(p_action_id),
			vars(p_vars) {}

	void execute();
	bool server_validate() const;
	operator String() const;
};

struct TokenizedNetActionProcessor {
	uint32_t action_token;
	NetActionProcessor processor;

	bool operator==(const TokenizedNetActionProcessor &p_other) const { return action_token == p_other.action_token; }

	TokenizedNetActionProcessor() = default;
	TokenizedNetActionProcessor(uint32_t p_at, NetActionProcessor p_p) :
			action_token(p_at), processor(p_p) {}
};

#endif
