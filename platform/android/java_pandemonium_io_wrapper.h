#ifndef JAVA_PANDEMONIUM_IO_WRAPPER_H
#define JAVA_PANDEMONIUM_IO_WRAPPER_H

/*************************************************************************/
/*  java_pandemonium_io_wrapper.h                                        */
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

// note, swapped java and pandemonium around in the file name so all the java
// wrappers are together

#include <android/log.h>
#include <jni.h>

#include "string_android.h"

// Class that makes functions in java/src/org/pandemoniumengine/pandemonium/PandemoniumIO.java callable from C++
class PandemoniumIOJavaWrapper {
private:
	jobject pandemonium_io_instance;
	jclass cls;

	jmethodID _open_URI = 0;
	jmethodID _get_cache_dir = 0;
	jmethodID _get_data_dir = 0;
	jmethodID _get_display_cutouts = 0;
	jmethodID _get_locale = 0;
	jmethodID _get_model = 0;
	jmethodID _get_screen_DPI = 0;
	jmethodID _get_scaled_density = 0;
	jmethodID _get_screen_refresh_rate = 0;
	jmethodID _get_window_safe_area = 0;
	jmethodID _get_unique_id = 0;
	jmethodID _show_keyboard = 0;
	jmethodID _hide_keyboard = 0;
	jmethodID _set_screen_orientation = 0;
	jmethodID _get_screen_orientation = 0;
	jmethodID _get_system_dir = 0;

public:
	PandemoniumIOJavaWrapper(JNIEnv *p_env, jobject p_pandemonium_io_instance);
	~PandemoniumIOJavaWrapper();

	jobject get_instance();

	Error open_uri(const String &p_uri);
	String get_cache_dir();
	String get_user_data_dir();
	String get_locale();
	String get_model();
	int get_screen_dpi();
	float get_scaled_density();
	void get_window_safe_area(int (&p_rect_xywh)[4]);
	Array get_display_cutouts();
	float get_screen_refresh_rate(float p_fallback);
	String get_unique_id();
	bool has_vk();
	void show_vk(const String &p_existing, int p_type, int p_max_input_length, int p_cursor_start, int p_cursor_end);
	void hide_vk();
	int get_vk_height();
	void set_vk_height(int p_height);
	void set_screen_orientation(int p_orient);
	int get_screen_orientation() const;
	String get_system_dir(int p_dir, bool p_shared_storage);
};

#endif /* !JAVA_PANDEMONIUM_IO_WRAPPER_H */
