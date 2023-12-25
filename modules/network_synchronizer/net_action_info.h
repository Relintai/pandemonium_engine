#ifndef NET_ACTION_INFO_H
#define NET_ACTION_INFO_H

/*************************************************************************/
/*  net_action_info.h                                                    */
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

#include "core/string/string_name.h"
#include "input_network_encoder.h"
#include "net_action_processor.h"

struct NetActionInfo {
	NetActionId id = UINT32_MAX;
	/// The event function
	StringName act_func;
	/// The event function encoding
	StringName act_encoding_func;
	/// If true the client can trigger this action.
	bool can_client_trigger;
	/// If true the client who triggered the event will wait the server validation to execute the event.
	bool wait_server_validation;
	/// The function to validate the event: Only executed on the server.
	StringName server_action_validation_func;
	/// The network_encoder used to encode decode the environment data.
	Ref<InputNetworkEncoder> network_encoder;

	bool operator==(const NetActionInfo &p_other) const;
	bool operator<(const NetActionInfo &p_other) const;
};

#endif
