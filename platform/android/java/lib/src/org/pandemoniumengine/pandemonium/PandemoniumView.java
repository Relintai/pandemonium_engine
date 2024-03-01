/*************************************************************************/
/*  PandemoniumView.java                                                 */
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

import org.pandemoniumengine.pandemonium.config.RegularConfigChooser;
import org.pandemoniumengine.pandemonium.config.RegularContextFactory;
import org.pandemoniumengine.pandemonium.config.RegularFallbackConfigChooser;
import org.pandemoniumengine.pandemonium.gl.PandemoniumGLSurfaceView;
import org.pandemoniumengine.pandemonium.input.PandemoniumInputHandler;
import org.pandemoniumengine.pandemonium.utils.GLUtils;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.PixelFormat;
import android.os.Build;
import android.text.TextUtils;
import android.util.SparseArray;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.PointerIcon;

import androidx.annotation.Keep;

import java.io.InputStream;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;

/**
 * A simple GLSurfaceView sub-class that demonstrate how to perform
 * OpenGL ES 2.0 rendering into a GL Surface. Note the following important
 * details:
 *
 * - The class must use a custom context factory to enable 2.0 rendering.
 *   See ContextFactory class definition below.
 *
 * - The class must use a custom EGLConfigChooser to be able to select
 *   an EGLConfig that supports 2.0. This is done by providing a config
 *   specification to eglChooseConfig() that has the attribute
 *   EGL10.ELG_RENDERABLE_TYPE containing the EGL_OPENGL_ES2_BIT flag
 *   set. See ConfigChooser class definition below.
 *
 * - The class must select the surface's format, then choose an EGLConfig
 *   that matches it exactly (with regards to red/green/blue/alpha channels
 *   bit depths). Failure to do so would result in an EGL_BAD_MATCH error.
 */
public class PandemoniumView extends PandemoniumGLSurfaceView {
	private static String TAG = PandemoniumView.class.getSimpleName();

	private final Pandemonium pandemonium;
	private final PandemoniumInputHandler inputHandler;
	private final PandemoniumRenderer pandemoniumRenderer;
	private final SparseArray<PointerIcon> customPointerIcons = new SparseArray<>();

	private EGLConfigChooser eglConfigChooser;
	private EGLContextFactory eglContextFactory;
	private EGLContext eglSecondaryContext;

	public PandemoniumView(Context context, Pandemonium pandemonium, boolean p_use_gl3, boolean p_use_debug_opengl, boolean p_translucent) {
		super(context);
		GLUtils.use_gl3 = p_use_gl3;
		GLUtils.use_debug_opengl = p_use_debug_opengl;

		this.pandemonium = pandemonium;
		this.inputHandler = new PandemoniumInputHandler(this);
		this.pandemoniumRenderer = new PandemoniumRenderer();

		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
			setPointerIcon(PointerIcon.getSystemIcon(getContext(), PointerIcon.TYPE_DEFAULT));
		}

		init(p_translucent);
	}

	public void initInputDevices() {
		this.inputHandler.initInputDevices();
	}

	@SuppressLint("ClickableViewAccessibility")
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		super.onTouchEvent(event);
		return inputHandler.onTouchEvent(event);
	}

	@Override
	public boolean onKeyUp(final int keyCode, KeyEvent event) {
		return inputHandler.onKeyUp(keyCode, event) || super.onKeyUp(keyCode, event);
	}

	@Override
	public boolean onKeyDown(final int keyCode, KeyEvent event) {
		return inputHandler.onKeyDown(keyCode, event) || super.onKeyDown(keyCode, event);
	}

	@Override
	public boolean onGenericMotionEvent(MotionEvent event) {
		return inputHandler.onGenericMotionEvent(event) || super.onGenericMotionEvent(event);
	}

	@Override
	public boolean onCapturedPointerEvent(MotionEvent event) {
		return inputHandler.onGenericMotionEvent(event);
	}

	@Override
	public void onPointerCaptureChange(boolean hasCapture) {
		if (canCapturePointer()) {
			super.onPointerCaptureChange(hasCapture);
			inputHandler.onPointerCaptureChange(hasCapture);
		}
	}

	@Keep
	private boolean canCapturePointer() {
		return inputHandler.canCapturePointer();
	}

	@Override
	public void requestPointerCapture() {
		super.requestPointerCapture();
		inputHandler.onPointerCaptureChange(true);
	}

	@Override
	public void releasePointerCapture() {
		super.releasePointerCapture();
		inputHandler.onPointerCaptureChange(false);
	}

	/**
	 * Used to configure the PointerIcon for the given type.
	 *
	 * Called from JNI
	 */
	@Keep
	public void configurePointerIcon(int pointerType, String imagePath, float hotSpotX, float hotSpotY) {
		if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.N) {
			try {
				Bitmap bitmap = null;
				if (!TextUtils.isEmpty(imagePath)) {
					if (pandemonium.getDirectoryAccessHandler().filesystemFileExists(imagePath)) {
						// Try to load the bitmap from the file system
						bitmap = BitmapFactory.decodeFile(imagePath);
					} else if (pandemonium.getDirectoryAccessHandler().assetsFileExists(imagePath)) {
						// Try to load the bitmap from the assets directory
						AssetManager am = getContext().getAssets();
						InputStream imageInputStream = am.open(imagePath);
						bitmap = BitmapFactory.decodeStream(imageInputStream);
					}
				}

				PointerIcon customPointerIcon = PointerIcon.create(bitmap, hotSpotX, hotSpotY);
				customPointerIcons.put(pointerType, customPointerIcon);
			} catch (Exception e) {
				// Reset the custom pointer icon
				customPointerIcons.delete(pointerType);
			}
		}
	}

	/**
	 * Called from JNI to change the pointer icon
	 */
	@Keep
	private void setPointerIcon(int pointerType) {
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
			PointerIcon pointerIcon = customPointerIcons.get(pointerType);
			if (pointerIcon == null) {
				pointerIcon = PointerIcon.getSystemIcon(getContext(), pointerType);
			}
			setPointerIcon(pointerIcon);
		}
	}

	@Override
	public PointerIcon onResolvePointerIcon(MotionEvent event, int pointerIndex) {
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
			return getPointerIcon();
		}
		return super.onResolvePointerIcon(event, pointerIndex);
	}

	private void init(boolean translucent) {
		setPreserveEGLContextOnPause(true);
		setFocusableInTouchMode(true);

		/* By default, GLSurfaceView() creates a RGB_565 opaque surface.
		 * If we want a translucent one, we should change the surface's
		 * format here, using PixelFormat.TRANSLUCENT for GL Surfaces
		 * is interpreted as any 32-bit surface with alpha by SurfaceFlinger.
		 */
		if (translucent) {
			this.setZOrderOnTop(true);
			this.getHolder().setFormat(PixelFormat.TRANSLUCENT);
		}

		/* Setup the context factory for 2.0 rendering.
		 * See ContextFactory class definition below
		 */
		eglContextFactory = new RegularContextFactory();

		/* We need to choose an EGLConfig that matches the format of
		 * our surface exactly. This is going to be done in our
		 * custom config chooser. See ConfigChooser class definition
		 * below.
		 */

		eglConfigChooser =
				new RegularFallbackConfigChooser(8, 8, 8, 8, 24, 0,
						new RegularFallbackConfigChooser(8, 8, 8, 8, 16, 0,
								// Let such a desperate fallback be used if under some circumstances that's the best we can get
								// (the translucency flag would be ignored, but that's better than not running at all)
								new RegularConfigChooser(5, 6, 5, 0, 16, 0)));

		setEGLConfigChooser(eglConfigChooser);
		setEGLContextFactory(eglContextFactory);

		/* Set the renderer responsible for frame rendering */
		setRenderer(pandemoniumRenderer);
	}

	public boolean createOffscreenGL() {
		EGL10 egl = (EGL10)EGLContext.getEGL();
		EGLConfig eglConfig = eglConfigChooser.chooseConfig(egl, egl.eglGetDisplay(EGL10.EGL_DEFAULT_DISPLAY));
		eglSecondaryContext = eglContextFactory.createContext(egl, egl.eglGetDisplay(EGL10.EGL_DEFAULT_DISPLAY), eglConfig);
		if (eglSecondaryContext == EGL10.EGL_NO_CONTEXT) {
			eglSecondaryContext = null;
		}
		return eglSecondaryContext != null;
	}

	public void setOffscreenGLCurrent(boolean p_current) {
		EGL10 egl = (EGL10)EGLContext.getEGL();
		egl.eglMakeCurrent(egl.eglGetDisplay(EGL10.EGL_DEFAULT_DISPLAY), EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_SURFACE, p_current ? eglSecondaryContext : EGL10.EGL_NO_CONTEXT);
	}

	public void destroyOffscreenGL() {
		if (eglSecondaryContext != null) {
			EGL10 egl = (EGL10)EGLContext.getEGL();
			eglContextFactory.destroyContext(egl, egl.eglGetCurrentDisplay(), eglSecondaryContext);
			eglSecondaryContext = null;
		}
	}

	public void onBackPressed() {
		pandemonium.onBackPressed();
	}

	public PandemoniumInputHandler getInputHandler() {
		return inputHandler;
	}

	void onActivityStarted() {
		resumeGLThread();
	}

	void onActivityResumed() {
		queueEvent(() -> {
			// Resume the renderer
			pandemoniumRenderer.onActivityResumed();
			PandemoniumLib.focusin();
		});
	}

	void onActivityPaused() {
		queueEvent(() -> {
			PandemoniumLib.focusout();
			// Pause the renderer
			pandemoniumRenderer.onActivityPaused();
		});
	}

	void onActivityStopped() {
		pauseGLThread();
	}
}
