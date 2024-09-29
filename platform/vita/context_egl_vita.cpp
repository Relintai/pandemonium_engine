/**************************************************************************/
/*  context_egl_vita.cpp                                                  */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "context_egl_vita.h"

void ContextEGL_Vita::release_current() {
	eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, context);
}

void ContextEGL_Vita::make_current() {
	eglMakeCurrent(display, surface, surface, context);
}

int ContextEGL_Vita::get_window_width() {
	return width;
}

int ContextEGL_Vita::get_window_height() {
	return height;
}

void ContextEGL_Vita::reset() {
	cleanup();
	initialize();
}

void ContextEGL_Vita::swap_buffers() {
	if (eglSwapBuffers(display, surface) != EGL_TRUE) {
		cleanup();
		initialize();
	}
};

Error ContextEGL_Vita::initialize() {
	// Get an appropriate EGL framebuffer configuration
	static const EGLint attributeList[] = {
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_ALPHA_SIZE, 8,
		EGL_DEPTH_SIZE, 8,
		EGL_STENCIL_SIZE, 8,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_NONE
	};

	static const EGLint contextAttributeList[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};

	EGLConfig config = nullptr;
	EGLint numConfigs = 0;

	// Initialize PVR_PSP2
	PVRSRV_PSP2_APPHINT hint;

	sceKernelLoadStartModule("app0:module/libgpu_es4_ext.suprx", 0, NULL, 0, NULL, NULL);
	sceKernelLoadStartModule("app0:module/libIMGEGL.suprx", 0, NULL, 0, NULL, NULL);

	PVRSRVInitializeAppHint(&hint);
	hint.ui32SwTexOpCleanupDelay = 16000;
	PVRSRVCreateVirtualAppHint(&hint);

	// Connect to the EGL default display
	display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if (!display) {
		goto _fail0;
	}

	// Initialize the display
	eglInitialize(display, NULL, NULL);

	// Get avaiable EGL framebuffer configurations
	eglChooseConfig(display, attributeList, &config, 1, &numConfigs);
	if (numConfigs == 0) {
		goto _fail1;
	}

	// Create an EGL window surface
	window.type = PSP2_DRAWABLE_TYPE_WINDOW;
	window.numFlipBuffers = 2;
	window.flipChainThrdAffinity = 0x20000;
	window.windowSize = PSP2_WINDOW_960X544;

	// Create a window surface
	surface = eglCreateWindowSurface(display, config, &window, NULL);
	if (!surface) {
		goto _fail1;
	}

	// Create an EGL rendering context
	context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttributeList);
	if (!context) {
		goto _fail2;
	}

	// Connect the context to the surface
	eglMakeCurrent(display, surface, surface, context);

	eglQuerySurface(display, surface, EGL_WIDTH, &width);
	eglQuerySurface(display, surface, EGL_HEIGHT, &height);

	return OK;

_fail2:
	eglDestroySurface(display, surface);
	surface = NULL;
_fail1:
	eglTerminate(display);
	display = NULL;
_fail0:
	return ERR_UNCONFIGURED;
}

void ContextEGL_Vita::cleanup() {
	if (display != EGL_NO_DISPLAY && surface != EGL_NO_SURFACE) {
		eglDestroySurface(display, surface);
		surface = EGL_NO_SURFACE;
	}

	if (display != EGL_NO_DISPLAY && context != EGL_NO_CONTEXT) {
		eglDestroyContext(display, context);
		context = EGL_NO_CONTEXT;
	}

	if (display != EGL_NO_DISPLAY) {
		eglTerminate(display);
		display = EGL_NO_DISPLAY;
	}
}

ContextEGL_Vita::ContextEGL_Vita(bool gles) :
		gles2_context(gles) {}

ContextEGL_Vita::~ContextEGL_Vita() {
	cleanup();
}
