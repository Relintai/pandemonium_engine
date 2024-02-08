/*************************************************************************/
/*  window_server.cpp                                                    */
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

#include "window_server.h"

WindowServer *WindowServer::singleton = nullptr;
WindowServer *(*WindowServer::create_func)() = nullptr;

int64_t WindowServer::get_native_handle_bind(HandleType p_handle_type) {
	return (int64_t)get_native_handle(p_handle_type);
}

WindowServer *WindowServer::get_singleton() {
	return singleton;
}

WindowServer *WindowServer::create() {
	ERR_FAIL_COND_V(singleton, nullptr);

	if (create_func) {
		return create_func();
	}

	return nullptr;
}

void WindowServer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_screen_count"), &WindowServer::get_screen_count);
	ClassDB::bind_method(D_METHOD("get_current_screen"), &WindowServer::get_current_screen);
	ClassDB::bind_method(D_METHOD("set_current_screen", "screen"), &WindowServer::set_current_screen);
	ClassDB::bind_method(D_METHOD("get_screen_position", "screen"), &WindowServer::get_screen_position, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("get_screen_size", "screen"), &WindowServer::get_screen_size, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("get_screen_dpi", "screen"), &WindowServer::get_screen_dpi, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("get_screen_scale", "screen"), &WindowServer::get_screen_scale, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("get_screen_max_scale"), &WindowServer::get_screen_max_scale);
	ClassDB::bind_method(D_METHOD("get_screen_refresh_rate", "screen"), &WindowServer::get_screen_refresh_rate, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("get_window_position"), &WindowServer::get_window_position);
	ClassDB::bind_method(D_METHOD("set_window_position", "position"), &WindowServer::set_window_position);
	ClassDB::bind_method(D_METHOD("get_window_size"), &WindowServer::get_window_size);
	ClassDB::bind_method(D_METHOD("get_max_window_size"), &WindowServer::get_max_window_size);
	ClassDB::bind_method(D_METHOD("get_min_window_size"), &WindowServer::get_min_window_size);
	ClassDB::bind_method(D_METHOD("set_max_window_size", "size"), &WindowServer::set_max_window_size);
	ClassDB::bind_method(D_METHOD("set_min_window_size", "size"), &WindowServer::set_min_window_size);
	ClassDB::bind_method(D_METHOD("set_window_size", "size"), &WindowServer::set_window_size);
	ClassDB::bind_method(D_METHOD("set_window_fullscreen", "enabled"), &WindowServer::set_window_fullscreen);
	ClassDB::bind_method(D_METHOD("is_window_fullscreen"), &WindowServer::is_window_fullscreen);
	ClassDB::bind_method(D_METHOD("set_window_resizable", "enabled"), &WindowServer::set_window_resizable);
	ClassDB::bind_method(D_METHOD("is_window_resizable"), &WindowServer::is_window_resizable);
	ClassDB::bind_method(D_METHOD("set_window_minimized", "enabled"), &WindowServer::set_window_minimized);
	ClassDB::bind_method(D_METHOD("is_window_minimized"), &WindowServer::is_window_minimized);
	ClassDB::bind_method(D_METHOD("set_window_maximized", "enabled"), &WindowServer::set_window_maximized);
	ClassDB::bind_method(D_METHOD("is_window_maximized"), &WindowServer::is_window_maximized);
	ClassDB::bind_method(D_METHOD("set_window_always_on_top", "enabled"), &WindowServer::set_window_always_on_top);
	ClassDB::bind_method(D_METHOD("is_window_always_on_top"), &WindowServer::is_window_always_on_top);
	ClassDB::bind_method(D_METHOD("is_window_focused"), &WindowServer::is_window_focused);
	ClassDB::bind_method(D_METHOD("request_attention"), &WindowServer::request_attention);
	ClassDB::bind_method(D_METHOD("get_real_window_size"), &WindowServer::get_real_window_size);
	ClassDB::bind_method(D_METHOD("center_window"), &WindowServer::center_window);
	ClassDB::bind_method(D_METHOD("move_window_to_foreground"), &WindowServer::move_window_to_foreground);

	ClassDB::bind_method(D_METHOD("get_native_handle", "handle_type"), &WindowServer::get_native_handle_bind);

	ClassDB::bind_method(D_METHOD("set_borderless_window", "borderless"), &WindowServer::set_borderless_window);
	ClassDB::bind_method(D_METHOD("get_borderless_window"), &WindowServer::get_borderless_window);

	ClassDB::bind_method(D_METHOD("get_window_per_pixel_transparency_enabled"), &WindowServer::get_window_per_pixel_transparency_enabled);
	ClassDB::bind_method(D_METHOD("set_window_per_pixel_transparency_enabled", "enabled"), &WindowServer::set_window_per_pixel_transparency_enabled);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "current_screen"), "set_current_screen", "get_current_screen");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "min_window_size"), "set_min_window_size", "get_min_window_size");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "max_window_size"), "set_max_window_size", "get_max_window_size");
	//Main Window
	ADD_GROUP("Window", "window_");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "window_borderless"), "set_borderless_window", "get_borderless_window");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "window_per_pixel_transparency_enabled"), "set_window_per_pixel_transparency_enabled", "get_window_per_pixel_transparency_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "window_fullscreen"), "set_window_fullscreen", "is_window_fullscreen");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "window_maximized"), "set_window_maximized", "is_window_maximized");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "window_minimized"), "set_window_minimized", "is_window_minimized");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "window_resizable"), "set_window_resizable", "is_window_resizable");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "window_position"), "set_window_position", "get_window_position");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "window_size"), "set_window_size", "get_window_size");

	// Those default values need to be specified for the docs generator,
	// to avoid using values from the documentation writer's own OS instance.
	ADD_PROPERTY_DEFAULT("clipboard", "");
	ADD_PROPERTY_DEFAULT("current_screen", 0);
	ADD_PROPERTY_DEFAULT("tablet_driver", "");
	ADD_PROPERTY_DEFAULT("exit_code", 0);
	ADD_PROPERTY_DEFAULT("vsync_enabled", true);
	ADD_PROPERTY_DEFAULT("vsync_via_compositor", false);
	ADD_PROPERTY_DEFAULT("low_processor_usage_mode", false);
	ADD_PROPERTY_DEFAULT("low_processor_usage_mode_sleep_usec", 6900);
	ADD_PROPERTY_DEFAULT("keep_screen_on", true);
	ADD_PROPERTY_DEFAULT("min_window_size", Vector2());
	ADD_PROPERTY_DEFAULT("max_window_size", Vector2());
	ADD_PROPERTY_DEFAULT("screen_orientation", 0);
	ADD_PROPERTY_DEFAULT("window_borderless", false);
	ADD_PROPERTY_DEFAULT("window_per_pixel_transparency_enabled", false);
	ADD_PROPERTY_DEFAULT("window_fullscreen", false);
	ADD_PROPERTY_DEFAULT("window_maximized", false);
	ADD_PROPERTY_DEFAULT("window_minimized", false);
	ADD_PROPERTY_DEFAULT("window_resizable", true);
	ADD_PROPERTY_DEFAULT("window_position", Vector2());
	ADD_PROPERTY_DEFAULT("window_size", Vector2());

	BIND_ENUM_CONSTANT(DISPLAY_HANDLE);
	BIND_ENUM_CONSTANT(WINDOW_HANDLE);
	BIND_ENUM_CONSTANT(WINDOW_VIEW);
	BIND_ENUM_CONSTANT(OPENGL_CONTEXT);

	//ClassDB::bind_method(D_METHOD("force_sync"), &WindowServer::sync);

	//ClassDB::bind_method(D_METHOD("is_render_loop_enabled"), &WindowServer::is_render_loop_enabled);
	//ClassDB::bind_method(D_METHOD("set_render_loop_enabled", "enabled"), &WindowServer::set_render_loop_enabled);
	//ADD_PROPERTY(PropertyInfo(Variant::BOOL, "render_loop_enabled"), "set_render_loop_enabled", "is_render_loop_enabled");

	//BIND_CONSTANT(NO_INDEX_ARRAY);

	//BIND_ENUM_CONSTANT(CUBEMAP_LEFT);

	//ADD_SIGNAL(MethodInfo("frame_pre_draw"));
}

WindowServer::WindowServer() {
	//ERR_FAIL_COND(singleton);
	singleton = this;
}

WindowServer::~WindowServer() {
	singleton = nullptr;
}
