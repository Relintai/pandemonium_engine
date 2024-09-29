/**************************************************************************/
/*  audio_driver_vita.cpp                                                 */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "audio_driver_vita.h"

#include "core/os/os.h"
#include "core/config/project_settings.h"

#include <errno.h>
#include <malloc.h>

static int portId = -1;

Error AudioDriverVita::init_device() {
	int latency = GLOBAL_GET("audio/output_latency");
	mix_rate = GLOBAL_GET("audio/mix_rate");
	channels = 2;
	portId = -1;
	speaker_mode = SPEAKER_MODE_STEREO;
	buffer_size = closest_power_of_2(latency * mix_rate / 1000);
	samples_in.resize(buffer_size * channels);
	samples_out.resize(buffer_size * channels);

	portId = sceAudioOutOpenPort(SCE_AUDIO_OUT_PORT_TYPE_BGM, buffer_size, mix_rate, SCE_AUDIO_OUT_MODE_STEREO);
	if (portId < 0) {
		return FAILED;
	}

	return OK;
}

Error AudioDriverVita::init() {
	active = false;
	thread_exited = false;
	exit_thread = false;

	Error err = init_device();
	if (err == OK) {
		thread.start(AudioDriverVita::thread_func, this);
	}

	return err;
}

void AudioDriverVita::thread_func(void *p_udata) {
	AudioDriverVita *ad = (AudioDriverVita *)p_udata;

	while (!ad->exit_thread) {
		ad->lock();
		ad->start_counting_ticks();

		if (!ad->active) {
			for (unsigned int i = 0; i < ad->buffer_size * ad->channels; i++) {
				ad->samples_out.write[i] = 0;
			}
		} else {
			ad->audio_server_process(ad->buffer_size, ad->samples_in.ptrw());
			for (unsigned int i = 0; i < ad->buffer_size * ad->channels; i++) {
				ad->samples_out.write[i] = ad->samples_in[i] >> 16;
			}
		}

		sceAudioOutOutput(portId, ad->samples_out.ptr());

		ad->stop_counting_ticks();
		ad->unlock();
		OS::get_singleton()->delay_usec(1000);
	}

	ad->thread_exited = true;
}

void AudioDriverVita::start() {
	active = true;
}

int AudioDriverVita::get_mix_rate() const {
	return mix_rate;
}

AudioDriver::SpeakerMode AudioDriverVita::get_speaker_mode() const {
	return speaker_mode;
}

Array AudioDriverVita::get_device_list() {
	Array list;
	list.push_back("Default");
	return list;
}

String AudioDriverVita::get_device() {
	return device_name;
}

void AudioDriverVita::set_device(String device) {
	lock();
	new_device = device;
	unlock();
}

void AudioDriverVita::lock() {
	mutex.lock();
}

void AudioDriverVita::unlock() {
	mutex.unlock();
}

void AudioDriverVita::finish() {
	exit_thread = true;
	thread.wait_to_finish();

	sceAudioOutOutput(portId, NULL);
	sceAudioOutReleasePort(portId);
}

AudioDriverVita::AudioDriverVita() :
		device_name("Default"),
		new_device("Default") {
}

AudioDriverVita::~AudioDriverVita() {
}
