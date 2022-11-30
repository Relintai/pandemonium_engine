#ifndef JAVA_PANDEMONIUM_LIB_JNI_H
#define JAVA_PANDEMONIUM_LIB_JNI_H
/*************************************************************************/
/*  java_pandemonium_lib_jni.h                                                 */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           PANDEMONIUM ENGINE                                */
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

#include <android/log.h>
#include <jni.h>

// These functions can be called from within JAVA and are the means by which our JAVA implementation calls back into our C++ code.
// See java/src/net/relintai/pandemonium/pandemonium/PandemoniumLib.java for the JAVA side of this (yes that's why we have the long names)
extern "C" {

JNIEXPORT void JNICALL Java_net_relintai_pandemonium_pandemonium_PandemoniumLib_initialize(JNIEnv *env, jclass clazz, jobject p_activity, jobject p_pandemonium_instance, jobject p_asset_manager, jobject p_pandemonium_io, jobject p_net_utils, jobject p_directory_access_handler, jobject p_file_access_handler, jboolean p_use_apk_expansion);
JNIEXPORT void JNICALL Java_net_relintai_pandemonium_pandemonium_PandemoniumLib_ondestroy(JNIEnv *env, jclass clazz);
JNIEXPORT jboolean JNICALL Java_net_relintai_pandemonium_pandemonium_PandemoniumLib_setup(JNIEnv *env, jclass clazz, jobjectArray p_cmdline);
JNIEXPORT void JNICALL Java_net_relintai_pandemonium_pandemonium_PandemoniumLib_resize(JNIEnv *env, jclass clazz, jint width, jint height);
JNIEXPORT void JNICALL Java_net_relintai_pandemonium_pandemonium_PandemoniumLib_newcontext(JNIEnv *env, jclass clazz);
JNIEXPORT void JNICALL Java_net_relintai_pandemonium_pandemonium_PandemoniumLib_step(JNIEnv *env, jclass clazz);
JNIEXPORT void JNICALL Java_net_relintai_pandemonium_pandemonium_PandemoniumLib_back(JNIEnv *env, jclass clazz);
JNIEXPORT void JNICALL Java_net_relintai_pandemonium_pandemonium_PandemoniumLib_dispatchMouseEvent(JNIEnv *env, jclass clazz, jint p_event_type, jint p_button_mask, jfloat p_x, jfloat p_y, jfloat p_delta_x, jfloat p_delta_y, jboolean p_double_click, jboolean p_source_mouse_relative);
JNIEXPORT void JNICALL Java_net_relintai_pandemonium_pandemonium_PandemoniumLib_dispatchTouchEvent(JNIEnv *env, jclass clazz, jint ev, jint pointer, jint pointer_count, jfloatArray positions, jboolean p_double_tap);
JNIEXPORT void JNICALL Java_net_relintai_pandemonium_pandemonium_PandemoniumLib_magnify(JNIEnv *env, jclass clazz, jfloat p_x, jfloat p_y, jfloat p_factor);
JNIEXPORT void JNICALL Java_net_relintai_pandemonium_pandemonium_PandemoniumLib_pan(JNIEnv *env, jclass clazz, jfloat p_x, jfloat p_y, jfloat p_delta_x, jfloat p_delta_y);
JNIEXPORT void JNICALL Java_net_relintai_pandemonium_pandemonium_PandemoniumLib_key(JNIEnv *env, jclass clazz, jint p_scancode, jint p_physical_scancode, jint p_unicode, jboolean p_pressed);
JNIEXPORT void JNICALL Java_net_relintai_pandemonium_pandemonium_PandemoniumLib_joybutton(JNIEnv *env, jclass clazz, jint p_device, jint p_button, jboolean p_pressed);
JNIEXPORT void JNICALL Java_net_relintai_pandemonium_pandemonium_PandemoniumLib_joyaxis(JNIEnv *env, jclass clazz, jint p_device, jint p_axis, jfloat p_value);
JNIEXPORT void JNICALL Java_net_relintai_pandemonium_pandemonium_PandemoniumLib_joyhat(JNIEnv *env, jclass clazz, jint p_device, jint p_hat_x, jint p_hat_y);
JNIEXPORT void JNICALL Java_net_relintai_pandemonium_pandemonium_PandemoniumLib_joyconnectionchanged(JNIEnv *env, jclass clazz, jint p_device, jboolean p_connected, jstring p_name);
JNIEXPORT void JNICALL Java_net_relintai_pandemonium_pandemonium_PandemoniumLib_accelerometer(JNIEnv *env, jclass clazz, jfloat x, jfloat y, jfloat z);
JNIEXPORT void JNICALL Java_net_relintai_pandemonium_pandemonium_PandemoniumLib_gravity(JNIEnv *env, jclass clazz, jfloat x, jfloat y, jfloat z);
JNIEXPORT void JNICALL Java_net_relintai_pandemonium_pandemonium_PandemoniumLib_magnetometer(JNIEnv *env, jclass clazz, jfloat x, jfloat y, jfloat z);
JNIEXPORT void JNICALL Java_net_relintai_pandemonium_pandemonium_PandemoniumLib_gyroscope(JNIEnv *env, jclass clazz, jfloat x, jfloat y, jfloat z);
JNIEXPORT void JNICALL Java_net_relintai_pandemonium_pandemonium_PandemoniumLib_focusin(JNIEnv *env, jclass clazz);
JNIEXPORT void JNICALL Java_net_relintai_pandemonium_pandemonium_PandemoniumLib_focusout(JNIEnv *env, jclass clazz);
JNIEXPORT jstring JNICALL Java_net_relintai_pandemonium_pandemonium_PandemoniumLib_getGlobal(JNIEnv *env, jclass clazz, jstring path);
JNIEXPORT void JNICALL Java_net_relintai_pandemonium_pandemonium_PandemoniumLib_callobject(JNIEnv *env, jclass clazz, jlong ID, jstring method, jobjectArray params);
JNIEXPORT void JNICALL Java_net_relintai_pandemonium_pandemonium_PandemoniumLib_calldeferred(JNIEnv *env, jclass clazz, jlong ID, jstring method, jobjectArray params);
JNIEXPORT void JNICALL Java_net_relintai_pandemonium_pandemonium_PandemoniumLib_setVirtualKeyboardHeight(JNIEnv *env, jclass clazz, jint p_height);
JNIEXPORT void JNICALL Java_net_relintai_pandemonium_pandemonium_PandemoniumLib_requestPermissionResult(JNIEnv *env, jclass clazz, jstring p_permission, jboolean p_result);
JNIEXPORT void JNICALL Java_net_relintai_pandemonium_pandemonium_PandemoniumLib_onRendererResumed(JNIEnv *env, jclass clazz);
JNIEXPORT void JNICALL Java_net_relintai_pandemonium_pandemonium_PandemoniumLib_onRendererPaused(JNIEnv *env, jclass clazz);
}

#endif /* !JAVA_PANDEMONIUM_LIB_JNI_H */
