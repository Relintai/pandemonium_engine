/*************************************************************************/
/*  tts_windows.h                                                        */
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

#ifndef TTS_WINDOWS_H
#define TTS_WINDOWS_H

#include "core/array.h"
#include "core/list.h"
#include "core/map.h"
#include "core/os/os.h"
#include "core/ustring.h"

#include <objbase.h>
#include <sapi.h>
#include <wchar.h>
#include <winnls.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class TTS_Windows {
	List<OS::TTSUtterance> queue;
	ISpVoice *synth = nullptr;
	bool paused = false;
	struct UTData {
		String string;
		int offset;
		int id;
	};
	Map<ULONG, UTData> ids;

	static void __stdcall speech_event_callback(WPARAM wParam, LPARAM lParam);
	void _update_tts();

	static TTS_Windows *singleton;

public:
	static TTS_Windows *get_singleton();

	bool is_speaking() const;
	bool is_paused() const;
	Array get_voices() const;

	void speak(const String &p_text, const String &p_voice, int p_volume = 50, float p_pitch = 1.f, float p_rate = 1.f, int p_utterance_id = 0, bool p_interrupt = false);
	void pause();
	void resume();
	void stop();

	TTS_Windows();
	~TTS_Windows();
};

#endif // TTS_WINDOWS_H
