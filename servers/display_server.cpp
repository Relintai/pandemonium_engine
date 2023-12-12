/*************************************************************************/
/*  DisplayServer.cpp                                                    */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
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

#include "display_server.h"

DisplayServer *DisplayServer::singleton = nullptr;
DisplayServer *(*DisplayServer::create_func)() = nullptr;

DisplayServer *DisplayServer::get_singleton() {
	return singleton;
}

DisplayServer *DisplayServer::create() {
	ERR_FAIL_COND_V(singleton, nullptr);

	if (create_func) {
		return create_func();
	}

	return nullptr;
}

void DisplayServer::_bind_methods() {
	//ClassDB::bind_method(D_METHOD("force_sync"), &DisplayServer::sync);

	//ClassDB::bind_method(D_METHOD("is_render_loop_enabled"), &DisplayServer::is_render_loop_enabled);
	//ClassDB::bind_method(D_METHOD("set_render_loop_enabled", "enabled"), &DisplayServer::set_render_loop_enabled);
	//ADD_PROPERTY(PropertyInfo(Variant::BOOL, "render_loop_enabled"), "set_render_loop_enabled", "is_render_loop_enabled");

	//BIND_CONSTANT(NO_INDEX_ARRAY);

	//BIND_ENUM_CONSTANT(CUBEMAP_LEFT);

	//ADD_SIGNAL(MethodInfo("frame_pre_draw"));
}

DisplayServer::DisplayServer() {
	//ERR_FAIL_COND(singleton);
	singleton = this;
}

DisplayServer::~DisplayServer() {
	singleton = nullptr;
}
