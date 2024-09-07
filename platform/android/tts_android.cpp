/*************************************************************************/
/*  tts_android.cpp                                                      */
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

#include "tts_android.h"

#include "java_godot_wrapper.h"
#include "os_android.h"
#include "string_android.h"
#include "thread_jandroid.h"

jobject TTS_Android::tts = 0;
jclass TTS_Android::cls = 0;

jmethodID TTS_Android::_is_speaking = 0;
jmethodID TTS_Android::_is_paused = 0;
jmethodID TTS_Android::_get_voices = 0;
jmethodID TTS_Android::_speak = 0;
jmethodID TTS_Android::_pause_speaking = 0;
jmethodID TTS_Android::_resume_speaking = 0;
jmethodID TTS_Android::_stop_speaking = 0;

HashMap<int, Vector<char16_t>> TTS_Android::ids;

Vector<char16_t> TTS_Android::str_to_utf16(const String &p_string) {
	int l = p_string.length();
	if (!l) {
		return Vector<char16_t>();
	}

	const CharType *d = &p_string[0];
	int fl = 0;
	for (int i = 0; i < l; i++) {
		uint32_t c = d[i];
		if (c <= 0xffff) { // 16 bits.
			fl += 1;
		} else if (c <= 0x10ffff) { // 32 bits.
			fl += 2;
		} else {
			print_error("Unicode parsing error: Invalid unicode codepoint " + String::num_int64(c, 16) + ".");
			return Vector<char16_t>();
		}
		if (c >= 0xd800 && c <= 0xdfff) {
			print_error("Unicode parsing error: Invalid unicode codepoint " + String::num_int64(c, 16) + ".");
			return Vector<char16_t>();
		}
	}

	Vector<char16_t> utf16s;
	if (fl == 0) {
		return utf16s;
	}

	utf16s.resize(fl + 1);
	uint16_t *cdst = (uint16_t *)utf16s.ptrw();

#define APPEND_CHAR(m_c) *(cdst++) = m_c

	for (int i = 0; i < l; i++) {
		uint32_t c = d[i];

		if (c <= 0xffff) { // 16 bits.
			APPEND_CHAR(c);
		} else { // 32 bits.
			APPEND_CHAR(uint32_t((c >> 10) + 0xd7c0)); // lead surrogate.
			APPEND_CHAR(uint32_t((c & 0x3ff) | 0xdc00)); // trail surrogate.
		}
	}
#undef APPEND_CHAR
	*cdst = 0; //trailing zero

	return utf16s;
}

void TTS_Android::setup(jobject p_tts) {
	JNIEnv *env = get_jni_env();

	tts = env->NewGlobalRef(p_tts);

	jclass c = env->GetObjectClass(tts);
	cls = (jclass)env->NewGlobalRef(c);

	_is_speaking = env->GetMethodID(cls, "isSpeaking", "()Z");
	_is_paused = env->GetMethodID(cls, "isPaused", "()Z");
	_get_voices = env->GetMethodID(cls, "getVoices", "()[Ljava/lang/String;");
	_speak = env->GetMethodID(cls, "speak", "(Ljava/lang/String;Ljava/lang/String;IFFIZ)V");
	_pause_speaking = env->GetMethodID(cls, "pauseSpeaking", "()V");
	_resume_speaking = env->GetMethodID(cls, "resumeSpeaking", "()V");
	_stop_speaking = env->GetMethodID(cls, "stopSpeaking", "()V");
}

void TTS_Android::_java_utterance_callback(int p_event, int p_id, int p_pos) {
	if (ids.has(p_id)) {
		int pos = 0;
		if ((OS::TTSUtteranceEvent)p_event == OS::TTS_UTTERANCE_BOUNDARY) {
			// Convert position from UTF-16 to UTF-32.
			const Vector<char16_t> &string = ids[p_id];
			for (int i = 0; i < MIN(p_pos, string.size() - 1); i++) {
				char16_t c = string[i];
				if ((c & 0xfffffc00) == 0xd800) {
					i++;
				}
				pos++;
			}
		} else if ((OS::TTSUtteranceEvent)p_event != OS::TTS_UTTERANCE_STARTED) {
			ids.erase(p_id);
		}
		OS::get_singleton()->tts_post_utterance_event((OS::TTSUtteranceEvent)p_event, p_id, pos);
	}
}

bool TTS_Android::is_speaking() {
	if (_is_speaking) {
		JNIEnv *env = get_jni_env();

		ERR_FAIL_COND_V(env == nullptr, false);
		return env->CallBooleanMethod(tts, _is_speaking);
	} else {
		return false;
	}
}

bool TTS_Android::is_paused() {
	if (_is_paused) {
		JNIEnv *env = get_jni_env();

		ERR_FAIL_COND_V(env == nullptr, false);
		return env->CallBooleanMethod(tts, _is_paused);
	} else {
		return false;
	}
}

Array TTS_Android::get_voices() {
	Array list;
	if (_get_voices) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND_V(env == nullptr, list);

		jobject voices_object = env->CallObjectMethod(tts, _get_voices);
		jobjectArray *arr = reinterpret_cast<jobjectArray *>(&voices_object);

		jsize len = env->GetArrayLength(*arr);
		for (int i = 0; i < len; i++) {
			jstring jStr = (jstring)env->GetObjectArrayElement(*arr, i);
			String str = jstring_to_string(jStr, env);
			Vector<String> tokens = str.split(";", true, 2);
			if (tokens.size() == 2) {
				Dictionary voice_d;
				voice_d["name"] = tokens[1];
				voice_d["id"] = tokens[1];
				voice_d["language"] = tokens[0];
				list.push_back(voice_d);
			}
			env->DeleteLocalRef(jStr);
		}
	}
	return list;
}

void TTS_Android::speak(const String &p_text, const String &p_voice, int p_volume, float p_pitch, float p_rate, int p_utterance_id, bool p_interrupt) {
	if (p_interrupt) {
		stop();
	}

	if (p_text.empty()) {
		OS::get_singleton()->tts_post_utterance_event(OS::TTS_UTTERANCE_CANCELED, p_utterance_id);
		return;
	}

	ids[p_utterance_id] = str_to_utf16(p_text);

	if (_speak) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND(env == nullptr);

		jstring jStrT = env->NewStringUTF(p_text.utf8().get_data());
		jstring jStrV = env->NewStringUTF(p_voice.utf8().get_data());
		env->CallVoidMethod(tts, _speak, jStrT, jStrV, CLAMP(p_volume, 0, 100), CLAMP(p_pitch, 0.f, 2.f), CLAMP(p_rate, 0.1f, 10.f), p_utterance_id, p_interrupt);
	}
}

void TTS_Android::pause() {
	if (_pause_speaking) {
		JNIEnv *env = get_jni_env();

		ERR_FAIL_COND(env == nullptr);
		env->CallVoidMethod(tts, _pause_speaking);
	}
}

void TTS_Android::resume() {
	if (_resume_speaking) {
		JNIEnv *env = get_jni_env();

		ERR_FAIL_COND(env == nullptr);
		env->CallVoidMethod(tts, _resume_speaking);
	}
}

void TTS_Android::stop() {
	const int *k = NULL;
	while ((k = ids.next(k))) {
		OS::get_singleton()->tts_post_utterance_event(OS::TTS_UTTERANCE_CANCELED, *k);
	}
	ids.clear();

	if (_stop_speaking) {
		JNIEnv *env = get_jni_env();

		ERR_FAIL_COND(env == nullptr);
		env->CallVoidMethod(tts, _stop_speaking);
	}
}
