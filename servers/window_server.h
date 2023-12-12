#ifndef DISPLAY_SERVER_H
#define DISPLAY_SERVER_H

/*************************************************************************/
/*  display_server.h                                                     */
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

#include "core/containers/rid.h"
#include "core/object/object.h"

class WindowServer : public Object {
	GDCLASS(WindowServer, Object);

	static WindowServer *singleton;

public:
	const float SCREEN_REFRESH_RATE_FALLBACK = -1.0;
	struct VideoMode {
		int width, height;
		bool fullscreen;
		bool resizable;
		bool borderless_window;
		bool maximized;
		bool always_on_top;
		bool use_vsync;
		bool vsync_via_compositor;
		bool layered;

		float get_aspect() const { return (float)width / (float)height; }

		VideoMode(int p_width = 1024, int p_height = 600, bool p_fullscreen = false, bool p_resizable = true, bool p_borderless_window = false, bool p_maximized = false, bool p_always_on_top = false, bool p_use_vsync = false, bool p_vsync_via_compositor = false) {
			width = p_width;
			height = p_height;
			fullscreen = p_fullscreen;
			resizable = p_resizable;
			borderless_window = p_borderless_window;
			maximized = p_maximized;
			always_on_top = p_always_on_top;
			use_vsync = p_use_vsync;
			vsync_via_compositor = p_vsync_via_compositor;
			layered = false;
		}
	};

	virtual int get_screen_count() const { return 1; }
	virtual int get_current_screen() const { return 0; }
	virtual void set_current_screen(int p_screen) {}
	virtual Point2 get_screen_position(int p_screen = -1) const { return Point2(); }
	virtual Size2 get_screen_size(int p_screen = -1) const { return get_window_size(); }
	virtual int get_screen_dpi(int p_screen = -1) const { return 72; }
	virtual float get_screen_scale(int p_screen = -1) const { return 1.0; }
	virtual float get_screen_max_scale() const { return 1.0; };
	virtual float get_screen_refresh_rate(int p_screen = -1) const { return SCREEN_REFRESH_RATE_FALLBACK; };
	virtual Point2 get_window_position() const { return Vector2(); }
	virtual void set_window_position(const Point2 &p_position) {}
	virtual Size2 get_max_window_size() const { return Size2(); };
	virtual Size2 get_min_window_size() const { return Size2(); };
	virtual Size2 get_window_size() const { return Size2(); };
	virtual Size2 get_real_window_size() const { return get_window_size(); }
	virtual void set_min_window_size(const Size2 p_size) {}
	virtual void set_max_window_size(const Size2 p_size) {}
	virtual void set_window_size(const Size2 p_size) {}
	virtual void set_window_fullscreen(bool p_enabled) {}
	virtual bool is_window_fullscreen() const { return true; }
	virtual void set_window_resizable(bool p_enabled) {}
	virtual bool is_window_resizable() const { return false; }
	virtual void set_window_minimized(bool p_enabled) {}
	virtual bool is_window_minimized() const { return false; }
	virtual void set_window_maximized(bool p_enabled) {}
	virtual bool is_window_maximized() const { return true; }
	virtual void set_window_always_on_top(bool p_enabled) {}
	virtual bool is_window_always_on_top() const { return false; }
	virtual bool is_window_focused() const { return true; }
	virtual void request_attention() {}
	virtual void center_window() {}

	//WindowServer
	enum HandleType {
		DISPLAY_HANDLE, // X11::Display* ...
		WINDOW_HANDLE, // HWND, X11::Window*, NSWindow*, UIWindow*, Android activity ...
		WINDOW_VIEW, // HDC, NSView*, UIView*, Android surface ...
		OPENGL_CONTEXT, // HGLRC, X11::GLXContext, NSOpenGLContext*, EGLContext* ...
	};

	virtual void *get_native_handle(int p_handle_type) { return nullptr; };
	virtual int64_t get_native_handle_bind(HandleType p_handle_type);

	virtual void set_borderless_window(bool p_borderless) {}
	virtual bool get_borderless_window() { return false; }

	virtual bool get_window_per_pixel_transparency_enabled() const { return false; }
	virtual void set_window_per_pixel_transparency_enabled(bool p_enabled) {}

	virtual void set_no_window_mode(bool p_enable) {}
	virtual bool is_no_window_mode_enabled() const { return false; }

	virtual void move_window_to_foreground() {}

	virtual void swap_buffers() {}

protected:
	static WindowServer *(*create_func)();

	static void _bind_methods();

public:
	static WindowServer *get_singleton();
	static WindowServer *create();

	WindowServer();
	virtual ~WindowServer();
};

VARIANT_ENUM_CAST(WindowServer::HandleType);

#endif
