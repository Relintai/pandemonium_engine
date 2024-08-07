/*************************************************************************/
/*  java_pandemonium_io_wrapper.cpp                                      */
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

#include "java_pandemonium_io_wrapper.h"
#include "core/error/error_list.h"
#include "core/math/rect2.h"
#include "core/variant/array.h"
#include "core/variant/variant.h"

// JNIEnv is only valid within the thread it belongs to, in a multi threading environment
// we can't cache it.
// For PandemoniumIO we call all access methods from our thread and we thus get a valid JNIEnv
// from ThreadAndroid.

PandemoniumIOJavaWrapper::PandemoniumIOJavaWrapper(JNIEnv *p_env, jobject p_pandemonium_io_instance) {
	pandemonium_io_instance = p_env->NewGlobalRef(p_pandemonium_io_instance);
	if (pandemonium_io_instance) {
		cls = p_env->GetObjectClass(pandemonium_io_instance);
		if (cls) {
			cls = (jclass)p_env->NewGlobalRef(cls);
		} else {
			// this is a pretty serious fail.. bail... pointers will stay 0
			return;
		}

		_open_URI = p_env->GetMethodID(cls, "openURI", "(Ljava/lang/String;)I");
		_get_cache_dir = p_env->GetMethodID(cls, "getCacheDir", "()Ljava/lang/String;");
		_get_data_dir = p_env->GetMethodID(cls, "getDataDir", "()Ljava/lang/String;");
		_get_display_cutouts = p_env->GetMethodID(cls, "getDisplayCutouts", "()[I"),
		_get_locale = p_env->GetMethodID(cls, "getLocale", "()Ljava/lang/String;");
		_get_model = p_env->GetMethodID(cls, "getModel", "()Ljava/lang/String;");
		_get_screen_DPI = p_env->GetMethodID(cls, "getScreenDPI", "()I");
		_get_screen_refresh_rate = p_env->GetMethodID(cls, "getScreenRefreshRate", "(D)D");
		_get_window_safe_area = p_env->GetMethodID(cls, "getWindowSafeArea", "()[I"),
		_get_unique_id = p_env->GetMethodID(cls, "getUniqueID", "()Ljava/lang/String;");
		_show_keyboard = p_env->GetMethodID(cls, "showKeyboard", "(Ljava/lang/String;IIII)V");
		_hide_keyboard = p_env->GetMethodID(cls, "hideKeyboard", "()V");
		_set_screen_orientation = p_env->GetMethodID(cls, "setScreenOrientation", "(I)V");
		_get_screen_orientation = p_env->GetMethodID(cls, "getScreenOrientation", "()I");
		_get_system_dir = p_env->GetMethodID(cls, "getSystemDir", "(IZ)Ljava/lang/String;");
	}
}

PandemoniumIOJavaWrapper::~PandemoniumIOJavaWrapper() {
	// nothing to do here for now
}

jobject PandemoniumIOJavaWrapper::get_instance() {
	return pandemonium_io_instance;
}

Error PandemoniumIOJavaWrapper::open_uri(const String &p_uri) {
	if (_open_URI) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND_V(env == nullptr, ERR_UNAVAILABLE);
		jstring jStr = env->NewStringUTF(p_uri.utf8().get_data());
		return env->CallIntMethod(pandemonium_io_instance, _open_URI, jStr) ? ERR_CANT_OPEN : OK;
	} else {
		return ERR_UNAVAILABLE;
	}
}

String PandemoniumIOJavaWrapper::get_cache_dir() {
	if (_get_cache_dir) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND_V(env == nullptr, String());
		jstring s = (jstring)env->CallObjectMethod(pandemonium_io_instance, _get_cache_dir);
		return jstring_to_string(s, env);
	} else {
		return String();
	}
}

String PandemoniumIOJavaWrapper::get_user_data_dir() {
	if (_get_data_dir) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND_V(env == nullptr, String());
		jstring s = (jstring)env->CallObjectMethod(pandemonium_io_instance, _get_data_dir);
		return jstring_to_string(s, env);
	} else {
		return String();
	}
}

String PandemoniumIOJavaWrapper::get_locale() {
	if (_get_locale) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND_V(env == nullptr, String());
		jstring s = (jstring)env->CallObjectMethod(pandemonium_io_instance, _get_locale);
		return jstring_to_string(s, env);
	} else {
		return String();
	}
}

String PandemoniumIOJavaWrapper::get_model() {
	if (_get_model) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND_V(env == nullptr, String());
		jstring s = (jstring)env->CallObjectMethod(pandemonium_io_instance, _get_model);
		return jstring_to_string(s, env);
	} else {
		return String();
	}
}

int PandemoniumIOJavaWrapper::get_screen_dpi() {
	if (_get_screen_DPI) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND_V(env == nullptr, 160);
		return env->CallIntMethod(pandemonium_io_instance, _get_screen_DPI);
	} else {
		return 160;
	}
}

float PandemoniumIOJavaWrapper::get_scaled_density() {
	if (_get_scaled_density) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND_V(env == nullptr, 1.0f);
		return env->CallFloatMethod(pandemonium_io_instance, _get_scaled_density);
	} else {
		return 1.0f;
	}
}

float PandemoniumIOJavaWrapper::get_screen_refresh_rate(float p_fallback) {
	if (_get_screen_refresh_rate) {
		JNIEnv *env = get_jni_env();
		if (env == nullptr) {
			ERR_PRINT("An error occurred while trying to get screen refresh rate.");
			return p_fallback;
		}
		return (float)env->CallDoubleMethod(pandemonium_io_instance, _get_screen_refresh_rate, (double)p_fallback);
	}
	ERR_PRINT("An error occurred while trying to get the screen refresh rate.");
	return p_fallback;
}

void PandemoniumIOJavaWrapper::get_window_safe_area(int (&p_rect_xywh)[4]) {
	if (_get_window_safe_area) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND(env == nullptr);
		jintArray returnArray = (jintArray)env->CallObjectMethod(pandemonium_io_instance, _get_window_safe_area);
		ERR_FAIL_COND(env->GetArrayLength(returnArray) != 4);
		jint *arrayBody = env->GetIntArrayElements(returnArray, JNI_FALSE);
		for (int i = 0; i < 4; i++) {
			p_rect_xywh[i] = arrayBody[i];
		}
		env->ReleaseIntArrayElements(returnArray, arrayBody, 0);
	}
}

Array PandemoniumIOJavaWrapper::get_display_cutouts() {
	Array result;
	ERR_FAIL_NULL_V(_get_display_cutouts, result);
	JNIEnv *env = get_jni_env();
	ERR_FAIL_NULL_V(env, result);
	jintArray returnArray = (jintArray)env->CallObjectMethod(pandemonium_io_instance, _get_display_cutouts);
	jint arrayLength = env->GetArrayLength(returnArray);
	jint *arrayBody = env->GetIntArrayElements(returnArray, JNI_FALSE);
	int cutouts = arrayLength / 4;
	for (int i = 0; i < cutouts; i++) {
		int x = arrayBody[i * 4];
		int y = arrayBody[i * 4 + 1];
		int width = arrayBody[i * 4 + 2];
		int height = arrayBody[i * 4 + 3];
		Rect2 cutout(x, y, width, height);
		result.append(cutout);
	}
	env->ReleaseIntArrayElements(returnArray, arrayBody, 0);
	return result;
}

String PandemoniumIOJavaWrapper::get_unique_id() {
	if (_get_unique_id) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND_V(env == nullptr, String());
		jstring s = (jstring)env->CallObjectMethod(pandemonium_io_instance, _get_unique_id);
		return jstring_to_string(s, env);
	} else {
		return String();
	}
}

bool PandemoniumIOJavaWrapper::has_vk() {
	return (_show_keyboard != 0) && (_hide_keyboard != 0);
}

void PandemoniumIOJavaWrapper::show_vk(const String &p_existing, int p_type, int p_max_input_length, int p_cursor_start, int p_cursor_end) {
	if (_show_keyboard) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND(env == nullptr);
		jstring jStr = env->NewStringUTF(p_existing.utf8().get_data());
		env->CallVoidMethod(pandemonium_io_instance, _show_keyboard, jStr, p_type, p_max_input_length, p_cursor_start, p_cursor_end);
	}
}

void PandemoniumIOJavaWrapper::hide_vk() {
	if (_hide_keyboard) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND(env == nullptr);
		env->CallVoidMethod(pandemonium_io_instance, _hide_keyboard);
	}
}

void PandemoniumIOJavaWrapper::set_screen_orientation(int p_orient) {
	if (_set_screen_orientation) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND(env == nullptr);
		env->CallVoidMethod(pandemonium_io_instance, _set_screen_orientation, p_orient);
	}
}

int PandemoniumIOJavaWrapper::get_screen_orientation() const {
	if (_get_screen_orientation) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND_V(env == nullptr, 0);
		return env->CallIntMethod(pandemonium_io_instance, _get_screen_orientation);
	} else {
		return 0;
	}
}

String PandemoniumIOJavaWrapper::get_system_dir(int p_dir, bool p_shared_storage) {
	if (_get_system_dir) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND_V(env == nullptr, String("."));
		jstring s = (jstring)env->CallObjectMethod(pandemonium_io_instance, _get_system_dir, p_dir, p_shared_storage);
		return jstring_to_string(s, env);
	} else {
		return String(".");
	}
}

// SafeNumeric because it can be changed from non-main thread and we need to
// ensure the change is immediately visible to other threads.
static SafeNumeric<int> virtual_keyboard_height;

int PandemoniumIOJavaWrapper::get_vk_height() {
	return virtual_keyboard_height.get();
}

void PandemoniumIOJavaWrapper::set_vk_height(int p_height) {
	virtual_keyboard_height.set(p_height);
}
