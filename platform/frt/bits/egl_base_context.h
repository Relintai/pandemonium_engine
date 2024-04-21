/*************************************************************************/
/*  egl_base_context.h                                                   */
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

// egl_base_context.h
/*
 * FRT - A Godot platform targeting single board computers
 * Copyright (c) 2017-2019  Emanuele Fornara
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "dl/egl.gen.h"

#include "../frt.h"

namespace frt {

class EGLBaseContext {
private:
	void build_attr_list(EGLint *attr_list) {
		App *app = App::instance();
		int n = 0;
		const int color_size = app->get_int_param("color_size");
		attr_list[n++] = EGL_RED_SIZE;
		attr_list[n++] = color_size;
		attr_list[n++] = EGL_GREEN_SIZE;
		attr_list[n++] = color_size;
		attr_list[n++] = EGL_BLUE_SIZE;
		attr_list[n++] = color_size;
		const int alpha_size = app->get_int_param("alpha_size");
		attr_list[n++] = EGL_ALPHA_SIZE;
		attr_list[n++] = alpha_size >= 0 ? alpha_size : EGL_DONT_CARE;
		const int depth_size = app->get_int_param("depth_size");
		attr_list[n++] = EGL_DEPTH_SIZE;
		attr_list[n++] = depth_size >= 0 ? depth_size : EGL_DONT_CARE;
		if (app->get_bool_param("multisample")) {
			attr_list[n++] = EGL_SAMPLE_BUFFERS;
			attr_list[n++] = 1;
		}
		attr_list[n++] = EGL_SURFACE_TYPE;
		attr_list[n++] = EGL_WINDOW_BIT;
		attr_list[n++] = EGL_NONE;
	}

protected:
	EGLDisplay display;
	EGLContext context;
	EGLSurface surface;
	EGLConfig config;

public:
	void init(int version, EGLNativeDisplayType display_id = EGL_DEFAULT_DISPLAY) {
		static EGLint attr_list[32];
		static const EGLint ctx_attrs[] = {
			EGL_CONTEXT_CLIENT_VERSION, version,
			EGL_NONE
		};
		EGLBoolean result;
		EGLint num_config;
		display = eglGetDisplay(display_id);
		if (display == EGL_NO_DISPLAY)
			fatal("eglGetDisplay failed.");
		result = eglInitialize(display, 0, 0);
		if (result == EGL_FALSE)
			fatal("eglInitialize failed.");
		build_attr_list(attr_list);
		result = eglChooseConfig(display, attr_list, &config, 1, &num_config);
		if (result == EGL_FALSE)
			fatal("eglChooseConfig failed.");
		result = eglBindAPI(EGL_OPENGL_ES_API);
		if (result == EGL_FALSE)
			fatal("eglBindAPI failed.");
		context = eglCreateContext(display, config, EGL_NO_CONTEXT, ctx_attrs);
		if (context == EGL_NO_CONTEXT)
			fatal("eglCreateContext failed.");
	};
	void create_simple_surface(EGLNativeWindowType window_id) {
		surface = eglCreateWindowSurface(display, config, window_id, 0);
		if (surface == EGL_NO_SURFACE)
			fatal("eglCreateWindowSurface failed.");
	}
	void cleanup() {
		eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		eglDestroyContext(display, context);
		eglTerminate(display);
	}
	void destroy_surface() {
		eglDestroySurface(display, surface);
	}
	void make_current() {
		eglMakeCurrent(display, surface, surface, context);
	}
	void release_current() {
		eglMakeCurrent(display, 0, 0, 0);
	}
	void swap_buffers() {
		eglSwapBuffers(display, surface);
	}
	void swap_interval(int interval) {
		eglSwapInterval(display, interval);
	}
};

} // namespace frt
