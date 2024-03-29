/*************************************************************************/
/*  java_pandemonium_wrapper.cpp                                         */
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

#include "java_pandemonium_wrapper.h"

// JNIEnv is only valid within the thread it belongs to, in a multi threading environment
// we can't cache it.
// For Pandemonium we call most access methods from our thread and we thus get a valid JNIEnv
// from ThreadAndroid. For one or two we expect to pass the environment

// TODO we could probably create a base class for this...

PandemoniumJavaWrapper::PandemoniumJavaWrapper(JNIEnv *p_env, jobject p_activity, jobject p_pandemonium_instance) {
	pandemonium_instance = p_env->NewGlobalRef(p_pandemonium_instance);
	activity = p_env->NewGlobalRef(p_activity);

	// get info about our Pandemonium class so we can get pointers and stuff...
	pandemonium_class = p_env->FindClass("org/pandemoniumengine/pandemonium/Pandemonium");
	if (pandemonium_class) {
		pandemonium_class = (jclass)p_env->NewGlobalRef(pandemonium_class);
	} else {
		// this is a pretty serious fail.. bail... pointers will stay 0
		return;
	}
	activity_class = p_env->FindClass("android/app/Activity");
	if (activity_class) {
		activity_class = (jclass)p_env->NewGlobalRef(activity_class);
	} else {
		// this is a pretty serious fail.. bail... pointers will stay 0
		return;
	}

	// get some Pandemonium method pointers...
	_on_video_init = p_env->GetMethodID(pandemonium_class, "onVideoInit", "()V");
	_create_offscreen_gl = p_env->GetMethodID(pandemonium_class, "createOffscreenGL", "()Z");
	_destroy_offscreen_gl = p_env->GetMethodID(pandemonium_class, "destroyOffscreenGL", "()V");
	_set_offscreen_gl_current = p_env->GetMethodID(pandemonium_class, "setOffscreenGLCurrent", "(Z)V");
	_restart = p_env->GetMethodID(pandemonium_class, "restart", "()V");
	_finish = p_env->GetMethodID(pandemonium_class, "forceQuit", "(I)Z");
	_set_keep_screen_on = p_env->GetMethodID(pandemonium_class, "setKeepScreenOn", "(Z)V");
	_alert = p_env->GetMethodID(pandemonium_class, "alert", "(Ljava/lang/String;Ljava/lang/String;)V");
	_enable_for_stealing_focus = p_env->GetMethodID(pandemonium_class, "enableForStealingFocus", "(I)V");
	_move_window_to_foreground = p_env->GetMethodID(pandemonium_class, "moveWindowToForeground", "()V");
	_get_GLES_version_code = p_env->GetMethodID(pandemonium_class, "getGLESVersionCode", "()I");
	_get_clipboard = p_env->GetMethodID(pandemonium_class, "getClipboard", "()Ljava/lang/String;");
	_set_clipboard = p_env->GetMethodID(pandemonium_class, "setClipboard", "(Ljava/lang/String;)V");
	_has_clipboard = p_env->GetMethodID(pandemonium_class, "hasClipboard", "()Z");
	_request_permission = p_env->GetMethodID(pandemonium_class, "requestPermission", "(Ljava/lang/String;)Z");
	_request_permissions = p_env->GetMethodID(pandemonium_class, "requestPermissions", "()Z");
	_get_granted_permissions = p_env->GetMethodID(pandemonium_class, "getGrantedPermissions", "()[Ljava/lang/String;");
	_init_input_devices = p_env->GetMethodID(pandemonium_class, "initInputDevices", "()V");
	_get_surface = p_env->GetMethodID(pandemonium_class, "getSurface", "()Landroid/view/Surface;");
	_is_activity_resumed = p_env->GetMethodID(pandemonium_class, "isActivityResumed", "()Z");
	_vibrate = p_env->GetMethodID(pandemonium_class, "vibrate", "(I)V");
	_get_input_fallback_mapping = p_env->GetMethodID(pandemonium_class, "getInputFallbackMapping", "()Ljava/lang/String;");
	_on_pandemonium_setup_completed = p_env->GetMethodID(pandemonium_class, "onPandemoniumSetupCompleted", "()V");
	_on_pandemonium_main_loop_started = p_env->GetMethodID(pandemonium_class, "onPandemoniumMainLoopStarted", "()V");
	_create_new_pandemonium_instance = p_env->GetMethodID(pandemonium_class, "createNewPandemoniumInstance", "([Ljava/lang/String;)I");
	_request_framebuffer_swap = p_env->GetMethodID(pandemonium_class, "requestFramebufferSwap", "()V");
	_get_render_view = p_env->GetMethodID(pandemonium_class, "getRenderView", "()Lorg/pandemoniumengine/pandemonium/PandemoniumView;");
	_begin_benchmark_measure = p_env->GetMethodID(pandemonium_class, "beginBenchmarkMeasure", "(Ljava/lang/String;)V");
	_end_benchmark_measure = p_env->GetMethodID(pandemonium_class, "endBenchmarkMeasure", "(Ljava/lang/String;)V");
	_dump_benchmark = p_env->GetMethodID(pandemonium_class, "dumpBenchmark", "(Ljava/lang/String;)V");

	// get some Activity method pointers...
	_get_class_loader = p_env->GetMethodID(activity_class, "getClassLoader", "()Ljava/lang/ClassLoader;");
}

PandemoniumJavaWrapper::~PandemoniumJavaWrapper() {
	if (pandemonium_view) {
		delete pandemonium_view;
	}

	JNIEnv *env = get_jni_env();
	ERR_FAIL_NULL(env);
	env->DeleteGlobalRef(pandemonium_instance);
	env->DeleteGlobalRef(pandemonium_class);
	env->DeleteGlobalRef(activity);
	env->DeleteGlobalRef(activity_class);
}

jobject PandemoniumJavaWrapper::get_activity() {
	return activity;
}

jobject PandemoniumJavaWrapper::get_member_object(const char *p_name, const char *p_class, JNIEnv *p_env) {
	if (pandemonium_class) {
		if (p_env == NULL)
			p_env = get_jni_env();

		ERR_FAIL_COND_V(p_env == nullptr, nullptr);

		jfieldID fid = p_env->GetStaticFieldID(pandemonium_class, p_name, p_class);
		return p_env->GetStaticObjectField(pandemonium_class, fid);
	} else {
		return NULL;
	}
}

jobject PandemoniumJavaWrapper::get_class_loader() {
	if (_get_class_loader) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND_V(env == nullptr, nullptr);

		return env->CallObjectMethod(activity, _get_class_loader);
	} else {
		return NULL;
	}
}

void PandemoniumJavaWrapper::gfx_init(bool gl2) {
	// beats me what this once did, there was no code,
	// but we're getting false if our GLES3 driver is initialised
	// and true for our GLES2 driver
	// Maybe we're supposed to communicate this back or store it?
}

PandemoniumJavaViewWrapper *PandemoniumJavaWrapper::get_pandemonium_view() {
	if (pandemonium_view != nullptr) {
		return pandemonium_view;
	}
	if (_get_render_view) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_NULL_V(env, nullptr);
		jobject pandemonium_render_view = env->CallObjectMethod(pandemonium_instance, _get_render_view);
		if (!env->IsSameObject(pandemonium_render_view, nullptr)) {
			pandemonium_view = new PandemoniumJavaViewWrapper(pandemonium_render_view);
		}
	}
	return pandemonium_view;
}

void PandemoniumJavaWrapper::on_video_init(JNIEnv *p_env) {
	if (_on_video_init) {
		if (p_env == NULL)
			p_env = get_jni_env();
		ERR_FAIL_COND(p_env == nullptr);

		p_env->CallVoidMethod(pandemonium_instance, _on_video_init);
	}
}

bool PandemoniumJavaWrapper::create_offscreen_gl(JNIEnv *p_env) {
	if (_create_offscreen_gl) {
		return p_env->CallBooleanMethod(pandemonium_instance, _create_offscreen_gl);
	} else {
		return false;
	}
}

void PandemoniumJavaWrapper::destroy_offscreen_gl(JNIEnv *p_env) {
	if (_destroy_offscreen_gl) {
		p_env->CallVoidMethod(pandemonium_instance, _destroy_offscreen_gl);
	}
}

void PandemoniumJavaWrapper::set_offscreen_gl_current(JNIEnv *p_env, bool p_current) {
	if (_set_offscreen_gl_current) {
		if (p_env == NULL)
			p_env = get_jni_env();
		ERR_FAIL_COND(p_env == nullptr);
		p_env->CallVoidMethod(pandemonium_instance, _set_offscreen_gl_current, p_current);
	}
}

void PandemoniumJavaWrapper::on_pandemonium_setup_completed(JNIEnv *p_env) {
	if (_on_pandemonium_setup_completed) {
		if (p_env == NULL) {
			p_env = get_jni_env();
		}
		ERR_FAIL_COND(p_env == nullptr);
		p_env->CallVoidMethod(pandemonium_instance, _on_pandemonium_setup_completed);
	}
}

void PandemoniumJavaWrapper::on_pandemonium_main_loop_started(JNIEnv *p_env) {
	if (_on_pandemonium_main_loop_started) {
		if (p_env == NULL) {
			p_env = get_jni_env();
		}
		ERR_FAIL_COND(p_env == nullptr);
		p_env->CallVoidMethod(pandemonium_instance, _on_pandemonium_main_loop_started);
	}
}

void PandemoniumJavaWrapper::restart(JNIEnv *p_env) {
	if (_restart) {
		if (p_env == NULL)
			p_env = get_jni_env();
		ERR_FAIL_COND(p_env == nullptr);

		p_env->CallVoidMethod(pandemonium_instance, _restart);
	}
}

bool PandemoniumJavaWrapper::force_quit(JNIEnv *p_env, int p_instance_id) {
	if (_finish) {
		if (p_env == NULL) {
			p_env = get_jni_env();
		}

		ERR_FAIL_NULL_V(p_env, false);

		return p_env->CallBooleanMethod(pandemonium_instance, _finish, p_instance_id);
	}

	return false;
}

void PandemoniumJavaWrapper::set_keep_screen_on(bool p_enabled) {
	if (_set_keep_screen_on) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND(env == nullptr);

		env->CallVoidMethod(pandemonium_instance, _set_keep_screen_on, p_enabled);
	}
}

void PandemoniumJavaWrapper::alert(const String &p_message, const String &p_title) {
	if (_alert) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND(env == nullptr);

		jstring jStrMessage = env->NewStringUTF(p_message.utf8().get_data());
		jstring jStrTitle = env->NewStringUTF(p_title.utf8().get_data());
		env->CallVoidMethod(pandemonium_instance, _alert, jStrMessage, jStrTitle);
	}
}

void PandemoniumJavaWrapper::enable_for_stealing_focus(int pid) {
	JNIEnv *env = get_jni_env();
	ERR_FAIL_COND(env == nullptr);

	if (_enable_for_stealing_focus) {
		env->CallVoidMethod(pandemonium_instance, _enable_for_stealing_focus, pid);
	}
}
void PandemoniumJavaWrapper::move_window_to_foreground() {
	JNIEnv *env = get_jni_env();
	ERR_FAIL_COND(env == nullptr);

	if (_move_window_to_foreground) {
		env->CallVoidMethod(pandemonium_instance, _move_window_to_foreground);
	}
}

int PandemoniumJavaWrapper::get_gles_version_code() {
	JNIEnv *env = get_jni_env();
	ERR_FAIL_COND_V(env == nullptr, 0);

	if (_get_GLES_version_code) {
		return env->CallIntMethod(pandemonium_instance, _get_GLES_version_code);
	}

	return 0;
}

bool PandemoniumJavaWrapper::has_get_clipboard() {
	return _get_clipboard != 0;
}

String PandemoniumJavaWrapper::get_clipboard() {
	if (_get_clipboard) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND_V(env == nullptr, String());

		jstring s = (jstring)env->CallObjectMethod(pandemonium_instance, _get_clipboard);
		return jstring_to_string(s, env);
	} else {
		return String();
	}
}

String PandemoniumJavaWrapper::get_input_fallback_mapping() {
	if (_get_input_fallback_mapping) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND_V(env == nullptr, String());

		jstring fallback_mapping = (jstring)env->CallObjectMethod(pandemonium_instance, _get_input_fallback_mapping);
		return jstring_to_string(fallback_mapping, env);
	} else {
		return String();
	}
}

bool PandemoniumJavaWrapper::has_set_clipboard() {
	return _set_clipboard != 0;
}

void PandemoniumJavaWrapper::set_clipboard(const String &p_text) {
	if (_set_clipboard) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND(env == nullptr);

		jstring jStr = env->NewStringUTF(p_text.utf8().get_data());
		env->CallVoidMethod(pandemonium_instance, _set_clipboard, jStr);
	}
}

bool PandemoniumJavaWrapper::has_has_clipboard() {
	return _has_clipboard != 0;
}

bool PandemoniumJavaWrapper::has_clipboard() {
	if (_has_clipboard) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND_V(env == nullptr, false);

		return env->CallBooleanMethod(pandemonium_instance, _has_clipboard);
	} else {
		return false;
	}
}

bool PandemoniumJavaWrapper::request_permission(const String &p_name) {
	if (_request_permission) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND_V(env == nullptr, false);

		jstring jStrName = env->NewStringUTF(p_name.utf8().get_data());
		return env->CallBooleanMethod(pandemonium_instance, _request_permission, jStrName);
	} else {
		return false;
	}
}

bool PandemoniumJavaWrapper::request_permissions() {
	if (_request_permissions) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND_V(env == nullptr, false);

		return env->CallBooleanMethod(pandemonium_instance, _request_permissions);
	} else {
		return false;
	}
}

Vector<String> PandemoniumJavaWrapper::get_granted_permissions() const {
	Vector<String> permissions_list;
	if (_get_granted_permissions) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND_V(env == nullptr, permissions_list);

		jobject permissions_object = env->CallObjectMethod(pandemonium_instance, _get_granted_permissions);
		jobjectArray *arr = reinterpret_cast<jobjectArray *>(&permissions_object);

		int i = 0;
		jsize len = env->GetArrayLength(*arr);
		for (i = 0; i < len; i++) {
			jstring jstr = (jstring)env->GetObjectArrayElement(*arr, i);
			String str = jstring_to_string(jstr, env);
			permissions_list.push_back(str);
			env->DeleteLocalRef(jstr);
		}
	}
	return permissions_list;
}

void PandemoniumJavaWrapper::init_input_devices() {
	if (_init_input_devices) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND(env == nullptr);

		env->CallVoidMethod(pandemonium_instance, _init_input_devices);
	}
}

jobject PandemoniumJavaWrapper::get_surface() {
	if (_get_surface) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND_V(env == nullptr, nullptr);

		return env->CallObjectMethod(pandemonium_instance, _get_surface);
	} else {
		return NULL;
	}
}

bool PandemoniumJavaWrapper::is_activity_resumed() {
	if (_is_activity_resumed) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND_V(env == nullptr, false);

		return env->CallBooleanMethod(pandemonium_instance, _is_activity_resumed);
	} else {
		return false;
	}
}

void PandemoniumJavaWrapper::vibrate(int p_duration_ms) {
	if (_vibrate) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND(env == nullptr);

		env->CallVoidMethod(pandemonium_instance, _vibrate, p_duration_ms);
	}
}

int PandemoniumJavaWrapper::create_new_pandemonium_instance(List<String> args) {
	if (_create_new_pandemonium_instance) {
		JNIEnv *env = get_jni_env();

		ERR_FAIL_NULL_V(env, 0);

		jobjectArray jargs = env->NewObjectArray(args.size(), env->FindClass("java/lang/String"), env->NewStringUTF(""));
		for (int i = 0; i < args.size(); i++) {
			env->SetObjectArrayElement(jargs, i, env->NewStringUTF(args[i].utf8().get_data()));
		}

		return env->CallIntMethod(pandemonium_instance, _create_new_pandemonium_instance, jargs);
	} else {
		return 0;
	}
}

void PandemoniumJavaWrapper::request_framebuffer_swap() {
	if (_request_framebuffer_swap) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND(env == nullptr);

		env->CallVoidMethod(pandemonium_instance, _request_framebuffer_swap);
	}
}

void PandemoniumJavaWrapper::begin_benchmark_measure(const String &p_label) {
	if (_begin_benchmark_measure) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_NULL(env);
		jstring j_label = env->NewStringUTF(p_label.utf8().get_data());
		env->CallVoidMethod(pandemonium_instance, _begin_benchmark_measure, j_label);
	}
}

void PandemoniumJavaWrapper::end_benchmark_measure(const String &p_label) {
	if (_end_benchmark_measure) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_NULL(env);
		jstring j_label = env->NewStringUTF(p_label.utf8().get_data());
		env->CallVoidMethod(pandemonium_instance, _end_benchmark_measure, j_label);
	}
}

void PandemoniumJavaWrapper::dump_benchmark(const String &benchmark_file) {
	if (_dump_benchmark) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_NULL(env);
		jstring j_benchmark_file = env->NewStringUTF(benchmark_file.utf8().get_data());
		env->CallVoidMethod(pandemonium_instance, _dump_benchmark, j_benchmark_file);
	}
}