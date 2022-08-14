/*************************************************************************/
/*  PandemoniumLib.java                                                  */
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

import org.pandemoniumengine.pandemonium.io.directory.DirectoryAccessHandler;
import org.pandemoniumengine.pandemonium.io.file.FileAccessHandler;
import org.pandemoniumengine.pandemonium.tts.PandemoniumTTS;
import org.pandemoniumengine.pandemonium.utils.PandemoniumNetUtils;

import android.app.Activity;
import android.content.res.AssetManager;
import android.hardware.SensorEvent;

import javax.microedition.khronos.opengles.GL10;

/**
 * Wrapper for native library
 */
public class PandemoniumLib {
	static {
		System.loadLibrary("pandemonium_android");
	}

	/**
	 * Invoked on the main thread to initialize Pandemonium native layer.
	 */
	public static native void initialize(Activity activity, Pandemonium p_instance, AssetManager p_asset_manager, PandemoniumIO pandemoniumIO, PandemoniumNetUtils netUtils, DirectoryAccessHandler directoryAccessHandler, FileAccessHandler fileAccessHandler, boolean use_apk_expansion, GodotTTS tts);

	/**
	 * Invoked on the main thread to clean up Pandemonium native layer.
	 * @see androidx.fragment.app.Fragment#onDestroy()
	 */
	public static native void ondestroy();

	/**
	 * Invoked on the GL thread to complete setup for the Pandemonium native layer logic.
	 * @param p_cmdline Command line arguments used to configure Pandemonium native layer components.
	 */
	public static native boolean setup(String[] p_cmdline);

	/**
	 * Invoked on the GL thread when the underlying Android surface has changed size.
	 * @param width
	 * @param height
	 * @see android.opengl.GLSurfaceView.Renderer#onSurfaceChanged(GL10, int, int)
	 */
	public static native void resize(int width, int height);

	/**
	 * Invoked on the GL thread when the underlying Android surface is created or recreated.
	 * @see android.opengl.GLSurfaceView.Renderer#onSurfaceCreated(GL10, EGLConfig)
	 */
	public static native void newcontext();

	/**
	 * Forward {@link Activity#onBackPressed()} event.
	 */
	public static native void back();

	/**
	 * Invoked on the GL thread to draw the current frame.
	 * @see android.opengl.GLSurfaceView.Renderer#onDrawFrame(GL10)
	 */
	public static native void step();

	/**
	 * TTS callback.
	 */
	public static native void ttsCallback(int event, int id, int pos);

	/**
	 * Forward touch events
	 */
	public static native void dispatchTouchEvent(int event, int pointer, int pointerCount, float[] positions, boolean doubleTap);

	/**
	 * Dispatch mouse events
	 */
	public static native void dispatchMouseEvent(int event, int buttonMask, float x, float y, float deltaX, float deltaY, boolean doubleClick, boolean sourceMouseRelative);

	public static native void magnify(float x, float y, float factor);

	public static native void pan(float x, float y, float deltaX, float deltaY);

	/**
	 * Forward accelerometer sensor events
	 * @see android.hardware.SensorEventListener#onSensorChanged(SensorEvent)
	 */
	public static native void accelerometer(float x, float y, float z);

	/**
	 * Forward gravity sensor events
	 * @see android.hardware.SensorEventListener#onSensorChanged(SensorEvent)
	 */
	public static native void gravity(float x, float y, float z);

	/**
	 * Forward magnetometer sensor events
	 * @see android.hardware.SensorEventListener#onSensorChanged(SensorEvent)
	 */
	public static native void magnetometer(float x, float y, float z);

	/**
	 * Forward gyroscope sensor events
	 * @see android.hardware.SensorEventListener#onSensorChanged(SensorEvent)
	 */
	public static native void gyroscope(float x, float y, float z);

	/**
	 * Forward regular key events
	 */
	public static native void key(int p_keycode, int p_scancode, int p_unicode_char, boolean p_pressed);

	/**
	 * Forward game device's key events
	 */
	public static native void joybutton(int p_device, int p_but, boolean p_pressed);

	/**
	 * Forward joystick devices axis motion events
	 */
	public static native void joyaxis(int p_device, int p_axis, float p_value);

	/**
	 * Forward joystick devices hat motion events
	 */
	public static native void joyhat(int p_device, int p_hat_x, int p_hat_y);

	/**
	 * Fires when a joystick device is added or removed.
	 */
	public static native void joyconnectionchanged(int p_device, boolean p_connected, String p_name);

	/**
	 * Invoked when the Android app resumes.
	 * @see androidx.fragment.app.Fragment#onResume()
	 */
	public static native void focusin();

	/**
	 * Invoked when the Android app pauses.
	 * @see androidx.fragment.app.Fragment#onPause()
	 */
	public static native void focusout();

	/**
	 * Used to access Pandemonium global properties.
	 * @param p_key Property key
	 * @return String value of the property
	 */
	public static native String getGlobal(String p_key);

	/**
	 * Used to access Pandemonium's editor settings.
	 * @param settingKey Setting key
	 * @return String value of the setting
	 */
	public static native String getEditorSetting(String settingKey);

	/**
	 * Invoke method |p_method| on the Pandemonium object specified by |p_id|
	 * @param p_id Id of the Pandemonium object to invoke
	 * @param p_method Name of the method to invoke
	 * @param p_params Parameters to use for method invocation
	 */
	public static native void callobject(long p_id, String p_method, Object[] p_params);

	/**
	 * Invoke method |p_method| on the Pandemonium object specified by |p_id| during idle time.
	 * @param p_id Id of the Pandemonium object to invoke
	 * @param p_method Name of the method to invoke
	 * @param p_params Parameters to use for method invocation
	 */
	public static native void calldeferred(long p_id, String p_method, Object[] p_params);

	/**
	 * Forward the results from a permission request.
	 * @see Activity#onRequestPermissionsResult(int, String[], int[])
	 * @param p_permission Request permission
	 * @param p_result True if the permission was granted, false otherwise
	 */
	public static native void requestPermissionResult(String p_permission, boolean p_result);

	/**
	 * Invoked on the GL thread to configure the height of the virtual keyboard.
	 */
	public static native void setVirtualKeyboardHeight(int p_height);

	/**
	 * Invoked on the GL thread when the {@link PandemoniumRenderer} has been resumed.
	 * @see PandemoniumRenderer#onActivityResumed()
	 */
	public static native void onRendererResumed();

	/**
	 * Invoked on the GL thread when the {@link PandemoniumRenderer} has been paused.
	 * @see PandemoniumRenderer#onActivityPaused()
	 */
	public static native void onRendererPaused();
}
