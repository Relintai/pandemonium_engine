#ifndef MIDI_DRIVER_ALSAMIDI_H
#define MIDI_DRIVER_ALSAMIDI_H

/*************************************************************************/
/*  midi_driver_alsamidi.h                                               */
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

#ifdef ALSAMIDI_ENABLED

#include "core/containers/vector.h"
#include "core/os/midi_driver.h"
#include "core/os/mutex.h"
#include "core/os/safe_refcount.h"
#include "core/os/thread.h"

#include "../alsa/asound-so_wrap.h"
#include <stdio.h>

class MIDIDriverALSAMidi : public MIDIDriver {
	Thread thread;
	Mutex mutex;

	class InputConnection {
	public:
		InputConnection() = default;
		InputConnection(snd_rawmidi_t *midi_in) :
				rawmidi_ptr{ midi_in } {}

		// Read in and parse available data, forwarding any complete messages through the driver.
		int read_in(MIDIDriverALSAMidi &driver, uint64_t timestamp);

		snd_rawmidi_t *rawmidi_ptr = nullptr;

	private:
		static const size_t MSG_BUFFER_SIZE = 3;
		uint8_t buffer[MSG_BUFFER_SIZE] = { 0 };
		size_t expected_data = 0;
		size_t received_data = 0;
		bool skipping_sys_ex = false;
		void parse_byte(uint8_t byte, MIDIDriverALSAMidi &driver, uint64_t timestamp);
	};

	Vector<InputConnection> connected_inputs;

	SafeFlag exit_thread;

	static void thread_func(void *p_udata);

	enum class MessageCategory {
		Data,
		Voice,
		SysExBegin,
		SystemCommon, // excluding System Exclusive Begin/End
		SysExEnd,
		RealTime,
	};

	// If the passed byte is a status byte, return the associated message category,
	// else return MessageCategory::Data.
	static MessageCategory msg_category(uint8_t msg_part);

	// Return the number of data bytes expected for the provided status byte.
	static size_t msg_expected_data(uint8_t status_byte);

	void lock() const;
	void unlock() const;

public:
	virtual Error open();
	virtual void close();

	virtual PoolStringArray get_connected_inputs();

	MIDIDriverALSAMidi();
	virtual ~MIDIDriverALSAMidi();
};

#endif // ALSAMIDI_ENABLED

#endif // MIDI_DRIVER_ALSAMIDI_H
