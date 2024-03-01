/*************************************************************************/
/*  PandemoniumRenderer.java                                             */
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

package org.pandemoniumengine.pandemonium;

import org.pandemoniumengine.pandemonium.gl.PandemoniumGLSurfaceView;
import org.pandemoniumengine.pandemonium.plugin.PandemoniumPlugin;
import org.pandemoniumengine.pandemonium.plugin.PandemoniumPluginRegistry;
import org.pandemoniumengine.pandemonium.utils.GLUtils;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Pandemonium's renderer implementation.
 */
class PandemoniumRenderer implements PandemoniumGLSurfaceView.Renderer {
	private final PandemoniumPluginRegistry pluginRegistry;
	private boolean activityJustResumed = false;

	PandemoniumRenderer() {
		this.pluginRegistry = PandemoniumPluginRegistry.getPluginRegistry();
	}

	public void onDrawFrame(GL10 gl) {
		if (activityJustResumed) {
			PandemoniumLib.onRendererResumed();
			activityJustResumed = false;
		}

		PandemoniumLib.step();
		for (int i = 0; i < Pandemonium.singleton_count; i++) {
			Pandemonium.singletons[i].onGLDrawFrame(gl);
		}
		for (PandemoniumPlugin plugin : pluginRegistry.getAllPlugins()) {
			plugin.onGLDrawFrame(gl);
		}
	}

	public void onSurfaceChanged(GL10 gl, int width, int height) {
		PandemoniumLib.resize(width, height);
		for (int i = 0; i < Pandemonium.singleton_count; i++) {
			Pandemonium.singletons[i].onGLSurfaceChanged(gl, width, height);
		}
		for (PandemoniumPlugin plugin : pluginRegistry.getAllPlugins()) {
			plugin.onGLSurfaceChanged(gl, width, height);
		}
	}

	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		PandemoniumLib.newcontext();
		for (PandemoniumPlugin plugin : pluginRegistry.getAllPlugins()) {
			plugin.onGLSurfaceCreated(gl, config);
		}
	}

	void onActivityResumed() {
		// We defer invoking PandemoniumLib.onRendererResumed() until the first draw frame call.
		// This ensures we have a valid GL context and surface when we do so.
		activityJustResumed = true;
	}

	void onActivityPaused() {
		PandemoniumLib.onRendererPaused();
	}
}
