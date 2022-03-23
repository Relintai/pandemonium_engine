/*************************************************************************/
/*  library_pandemonium_audio.js                                               */
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

const PandemoniumAudio = {
    $PandemoniumAudio__deps: ['$PandemoniumRuntime', '$PandemoniumOS'],
    $PandemoniumAudio: {
        ctx: null,
        input: null,
        driver: null,
        interval: 0,

        init: function(mix_rate, latency, onstatechange, onlatencyupdate) {
            const opts = {};
            // If mix_rate is 0, let the browser choose.
            if (mix_rate) {
                opts['sampleRate'] = mix_rate;
            }
            // Do not specify, leave 'interactive' for good performance.
            // opts['latencyHint'] = latency / 1000;
            const ctx = new(window.AudioContext || window.webkitAudioContext)(opts);
            PandemoniumAudio.ctx = ctx;
            ctx.onstatechange = function() {
                let state = 0;
                switch (ctx.state) {
                    case 'suspended':
                        state = 0;
                        break;
                    case 'running':
                        state = 1;
                        break;
                    case 'closed':
                        state = 2;
                        break;

                        // no default
                }
                onstatechange(state);
            };
            ctx.onstatechange(); // Immediately notify state.
            // Update computed latency
            PandemoniumAudio.interval = setInterval(function() {
                let computed_latency = 0;
                if (ctx.baseLatency) {
                    computed_latency += PandemoniumAudio.ctx.baseLatency;
                }
                if (ctx.outputLatency) {
                    computed_latency += PandemoniumAudio.ctx.outputLatency;
                }
                onlatencyupdate(computed_latency);
            }, 1000);
            PandemoniumOS.atexit(PandemoniumAudio.close_async);
            return ctx.destination.channelCount;
        },

        create_input: function(callback) {
            if (PandemoniumAudio.input) {
                return 0; // Already started.
            }

            function gotMediaInput(stream) {
                try {
                    PandemoniumAudio.input = PandemoniumAudio.ctx.createMediaStreamSource(stream);
                    callback(PandemoniumAudio.input);
                } catch (e) {
                    PandemoniumRuntime.error('Failed creaating input.', e);
                }
            }
            if (navigator.mediaDevices && navigator.mediaDevices.getUserMedia) {
                navigator.mediaDevices.getUserMedia({
                    'audio': true,
                }).then(gotMediaInput, function(e) {
                    PandemoniumRuntime.error('Error getting user media.', e);
                });
            } else {
                if (!navigator.getUserMedia) {
                    navigator.getUserMedia = navigator.webkitGetUserMedia || navigator.mozGetUserMedia;
                }
                if (!navigator.getUserMedia) {
                    PandemoniumRuntime.error('getUserMedia not available.');
                    return 1;
                }
                navigator.getUserMedia({
                    'audio': true,
                }, gotMediaInput, function(e) {
                    PandemoniumRuntime.print(e);
                });
            }
            return 0;
        },

        close_async: function(resolve, reject) {
            const ctx = PandemoniumAudio.ctx;
            PandemoniumAudio.ctx = null;
            // Audio was not initialized.
            if (!ctx) {
                resolve();
                return;
            }
            // Remove latency callback
            if (PandemoniumAudio.interval) {
                clearInterval(PandemoniumAudio.interval);
                PandemoniumAudio.interval = 0;
            }
            // Disconnect input, if it was started.
            if (PandemoniumAudio.input) {
                PandemoniumAudio.input.disconnect();
                PandemoniumAudio.input = null;
            }
            // Disconnect output
            let closed = Promise.resolve();
            if (PandemoniumAudio.driver) {
                closed = PandemoniumAudio.driver.close();
            }
            closed.then(function() {
                return ctx.close();
            }).then(function() {
                ctx.onstatechange = null;
                resolve();
            }).catch(function(e) {
                ctx.onstatechange = null;
                PandemoniumRuntime.error('Error closing AudioContext', e);
                resolve();
            });
        },
    },

    pandemonium_audio_is_available__sig: 'i',
    pandemonium_audio_is_available__proxy: 'sync',
    pandemonium_audio_is_available: function() {
        if (!(window.AudioContext || window.webkitAudioContext)) {
            return 0;
        }
        return 1;
    },

    pandemonium_audio_has_worklet__sig: 'i',
    pandemonium_audio_has_worklet: function() {
        return (PandemoniumAudio.ctx && PandemoniumAudio.ctx.audioWorklet) ? 1 : 0;
    },

    pandemonium_audio_has_script_processor__sig: 'i',
    pandemonium_audio_has_script_processor: function() {
        return (PandemoniumAudio.ctx && PandemoniumAudio.ctx.createScriptProcessor) ? 1 : 0;
    },

    pandemonium_audio_init__sig: 'iiiii',
    pandemonium_audio_init: function(p_mix_rate, p_latency, p_state_change, p_latency_update) {
        const statechange = PandemoniumRuntime.get_func(p_state_change);
        const latencyupdate = PandemoniumRuntime.get_func(p_latency_update);
        const mix_rate = PandemoniumRuntime.getHeapValue(p_mix_rate, 'i32');
        const channels = PandemoniumAudio.init(mix_rate, p_latency, statechange, latencyupdate);
        PandemoniumRuntime.setHeapValue(p_mix_rate, PandemoniumAudio.ctx.sampleRate, 'i32');
        return channels;
    },

    pandemonium_audio_resume__sig: 'v',
    pandemonium_audio_resume: function() {
        if (PandemoniumAudio.ctx && PandemoniumAudio.ctx.state !== 'running') {
            PandemoniumAudio.ctx.resume();
        }
    },

    pandemonium_audio_capture_start__proxy: 'sync',
    pandemonium_audio_capture_start__sig: 'i',
    pandemonium_audio_capture_start: function() {
        return PandemoniumAudio.create_input(function(input) {
            input.connect(PandemoniumAudio.driver.get_node());
        });
    },

    pandemonium_audio_capture_stop__proxy: 'sync',
    pandemonium_audio_capture_stop__sig: 'v',
    pandemonium_audio_capture_stop: function() {
        if (PandemoniumAudio.input) {
            const tracks = PandemoniumAudio.input['mediaStream']['getTracks']();
            for (let i = 0; i < tracks.length; i++) {
                tracks[i]['stop']();
            }
            PandemoniumAudio.input.disconnect();
            PandemoniumAudio.input = null;
        }
    },
};

autoAddDeps(PandemoniumAudio, '$PandemoniumAudio');
mergeInto(LibraryManager.library, PandemoniumAudio);

/**
 * The AudioWorklet API driver, used when threads are available.
 */
const PandemoniumAudioWorklet = {
    $PandemoniumAudioWorklet__deps: ['$PandemoniumAudio', '$PandemoniumConfig'],
    $PandemoniumAudioWorklet: {
        promise: null,
        worklet: null,
        ring_buffer: null,

        create: function(channels) {
            const path = PandemoniumConfig.locate_file('pandemonium.audio.worklet.js');
            PandemoniumAudioWorklet.promise = PandemoniumAudio.ctx.audioWorklet.addModule(path).then(function() {
                PandemoniumAudioWorklet.worklet = new AudioWorkletNode(
                    PandemoniumAudio.ctx,
                    'pandemonium-processor', {
                        'outputChannelCount': [channels],
                    }
                );
                return Promise.resolve();
            });
            PandemoniumAudio.driver = PandemoniumAudioWorklet;
        },

        start: function(in_buf, out_buf, state) {
            PandemoniumAudioWorklet.promise.then(function() {
                const node = PandemoniumAudioWorklet.worklet;
                node.connect(PandemoniumAudio.ctx.destination);
                node.port.postMessage({
                    'cmd': 'start',
                    'data': [state, in_buf, out_buf],
                });
                node.port.onmessage = function(event) {
                    PandemoniumRuntime.error(event.data);
                };
            });
        },

        start_no_threads: function(p_out_buf, p_out_size, out_callback, p_in_buf, p_in_size, in_callback) {
            function RingBuffer() {
                let wpos = 0;
                let rpos = 0;
                let pending_samples = 0;
                const wbuf = new Float32Array(p_out_size);

                function send(port) {
                    if (pending_samples === 0) {
                        return;
                    }
                    const buffer = PandemoniumRuntime.heapSub(HEAPF32, p_out_buf, p_out_size);
                    const size = buffer.length;
                    const tot_sent = pending_samples;
                    out_callback(wpos, pending_samples);
                    if (wpos + pending_samples >= size) {
                        const high = size - wpos;
                        wbuf.set(buffer.subarray(wpos, size));
                        pending_samples -= high;
                        wpos = 0;
                    }
                    if (pending_samples > 0) {
                        wbuf.set(buffer.subarray(wpos, wpos + pending_samples), tot_sent - pending_samples);
                    }
                    port.postMessage({
                        'cmd': 'chunk',
                        'data': wbuf.subarray(0, tot_sent)
                    });
                    wpos += pending_samples;
                    pending_samples = 0;
                }
                this.receive = function(recv_buf) {
                    const buffer = PandemoniumRuntime.heapSub(HEAPF32, p_in_buf, p_in_size);
                    const from = rpos;
                    let to_write = recv_buf.length;
                    let high = 0;
                    if (rpos + to_write >= p_in_size) {
                        high = p_in_size - rpos;
                        buffer.set(recv_buf.subarray(0, high), rpos);
                        to_write -= high;
                        rpos = 0;
                    }
                    if (to_write) {
                        buffer.set(recv_buf.subarray(high, to_write), rpos);
                    }
                    in_callback(from, recv_buf.length);
                    rpos += to_write;
                };
                this.consumed = function(size, port) {
                    pending_samples += size;
                    send(port);
                };
            }
            PandemoniumAudioWorklet.ring_buffer = new RingBuffer();
            PandemoniumAudioWorklet.promise.then(function() {
                const node = PandemoniumAudioWorklet.worklet;
                const buffer = PandemoniumRuntime.heapSlice(HEAPF32, p_out_buf, p_out_size);
                node.connect(PandemoniumAudio.ctx.destination);
                node.port.postMessage({
                    'cmd': 'start_nothreads',
                    'data': [buffer, p_in_size],
                });
                node.port.onmessage = function(event) {
                    if (!PandemoniumAudioWorklet.worklet) {
                        return;
                    }
                    if (event.data['cmd'] === 'read') {
                        const read = event.data['data'];
                        PandemoniumAudioWorklet.ring_buffer.consumed(read, PandemoniumAudioWorklet.worklet.port);
                    } else if (event.data['cmd'] === 'input') {
                        const buf = event.data['data'];
                        if (buf.length > p_in_size) {
                            PandemoniumRuntime.error('Input chunk is too big');
                            return;
                        }
                        PandemoniumAudioWorklet.ring_buffer.receive(buf);
                    } else {
                        PandemoniumRuntime.error(event.data);
                    }
                };
            });
        },

        get_node: function() {
            return PandemoniumAudioWorklet.worklet;
        },

        close: function() {
            return new Promise(function(resolve, reject) {
                if (PandemoniumAudioWorklet.promise === null) {
                    return;
                }
                PandemoniumAudioWorklet.promise.then(function() {
                    PandemoniumAudioWorklet.worklet.port.postMessage({
                        'cmd': 'stop',
                        'data': null,
                    });
                    PandemoniumAudioWorklet.worklet.disconnect();
                    PandemoniumAudioWorklet.worklet = null;
                    PandemoniumAudioWorklet.promise = null;
                    resolve();
                }).catch(function(err) {
                    /* aborted? */
                });
            });
        },
    },

    pandemonium_audio_worklet_create__sig: 'ii',
    pandemonium_audio_worklet_create: function(channels) {
        try {
            PandemoniumAudioWorklet.create(channels);
        } catch (e) {
            PandemoniumRuntime.error('Error starting AudioDriverWorklet', e);
            return 1;
        }
        return 0;
    },

    pandemonium_audio_worklet_start__sig: 'viiiii',
    pandemonium_audio_worklet_start: function(p_in_buf, p_in_size, p_out_buf, p_out_size, p_state) {
        const out_buffer = PandemoniumRuntime.heapSub(HEAPF32, p_out_buf, p_out_size);
        const in_buffer = PandemoniumRuntime.heapSub(HEAPF32, p_in_buf, p_in_size);
        const state = PandemoniumRuntime.heapSub(HEAP32, p_state, 4);
        PandemoniumAudioWorklet.start(in_buffer, out_buffer, state);
    },

    pandemonium_audio_worklet_start_no_threads__sig: 'viiiiii',
    pandemonium_audio_worklet_start_no_threads: function(p_out_buf, p_out_size, p_out_callback, p_in_buf, p_in_size, p_in_callback) {
        const out_callback = PandemoniumRuntime.get_func(p_out_callback);
        const in_callback = PandemoniumRuntime.get_func(p_in_callback);
        PandemoniumAudioWorklet.start_no_threads(p_out_buf, p_out_size, out_callback, p_in_buf, p_in_size, in_callback);
    },

    pandemonium_audio_worklet_state_wait__sig: 'iiii',
    pandemonium_audio_worklet_state_wait: function(p_state, p_idx, p_expected, p_timeout) {
        Atomics.wait(HEAP32, (p_state >> 2) + p_idx, p_expected, p_timeout);
        return Atomics.load(HEAP32, (p_state >> 2) + p_idx);
    },

    pandemonium_audio_worklet_state_add__sig: 'iiii',
    pandemonium_audio_worklet_state_add: function(p_state, p_idx, p_value) {
        return Atomics.add(HEAP32, (p_state >> 2) + p_idx, p_value);
    },

    pandemonium_audio_worklet_state_get__sig: 'iii',
    pandemonium_audio_worklet_state_get: function(p_state, p_idx) {
        return Atomics.load(HEAP32, (p_state >> 2) + p_idx);
    },
};

autoAddDeps(PandemoniumAudioWorklet, '$PandemoniumAudioWorklet');
mergeInto(LibraryManager.library, PandemoniumAudioWorklet);

/*
 * The deprecated ScriptProcessorNode API, used when threads are disabled.
 */
const PandemoniumAudioScript = {
    $PandemoniumAudioScript__deps: ['$PandemoniumAudio'],
    $PandemoniumAudioScript: {
        script: null,

        create: function(buffer_length, channel_count) {
            PandemoniumAudioScript.script = PandemoniumAudio.ctx.createScriptProcessor(buffer_length, 2, channel_count);
            PandemoniumAudio.driver = PandemoniumAudioScript;
            return PandemoniumAudioScript.script.bufferSize;
        },

        start: function(p_in_buf, p_in_size, p_out_buf, p_out_size, onprocess) {
            PandemoniumAudioScript.script.onaudioprocess = function(event) {
                // Read input
                const inb = PandemoniumRuntime.heapSub(HEAPF32, p_in_buf, p_in_size);
                const input = event.inputBuffer;
                if (PandemoniumAudio.input) {
                    const inlen = input.getChannelData(0).length;
                    for (let ch = 0; ch < 2; ch++) {
                        const data = input.getChannelData(ch);
                        for (let s = 0; s < inlen; s++) {
                            inb[s * 2 + ch] = data[s];
                        }
                    }
                }

                // Let Pandemonium process the input/output.
                onprocess();

                // Write the output.
                const outb = PandemoniumRuntime.heapSub(HEAPF32, p_out_buf, p_out_size);
                const output = event.outputBuffer;
                const channels = output.numberOfChannels;
                for (let ch = 0; ch < channels; ch++) {
                    const data = output.getChannelData(ch);
                    // Loop through samples and assign computed values.
                    for (let sample = 0; sample < data.length; sample++) {
                        data[sample] = outb[sample * channels + ch];
                    }
                }
            };
            PandemoniumAudioScript.script.connect(PandemoniumAudio.ctx.destination);
        },

        get_node: function() {
            return PandemoniumAudioScript.script;
        },

        close: function() {
            return new Promise(function(resolve, reject) {
                PandemoniumAudioScript.script.disconnect();
                PandemoniumAudioScript.script.onaudioprocess = null;
                PandemoniumAudioScript.script = null;
                resolve();
            });
        },
    },

    pandemonium_audio_script_create__sig: 'iii',
    pandemonium_audio_script_create: function(buffer_length, channel_count) {
        const buf_len = PandemoniumRuntime.getHeapValue(buffer_length, 'i32');
        try {
            const out_len = PandemoniumAudioScript.create(buf_len, channel_count);
            PandemoniumRuntime.setHeapValue(buffer_length, out_len, 'i32');
        } catch (e) {
            PandemoniumRuntime.error('Error starting AudioDriverScriptProcessor', e);
            return 1;
        }
        return 0;
    },

    pandemonium_audio_script_start__sig: 'viiiii',
    pandemonium_audio_script_start: function(p_in_buf, p_in_size, p_out_buf, p_out_size, p_cb) {
        const onprocess = PandemoniumRuntime.get_func(p_cb);
        PandemoniumAudioScript.start(p_in_buf, p_in_size, p_out_buf, p_out_size, onprocess);
    },
};

autoAddDeps(PandemoniumAudioScript, '$PandemoniumAudioScript');
mergeInto(LibraryManager.library, PandemoniumAudioScript);