/*************************************************************************/
/*  library_pandemonium_os.js                                                  */
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

const IDHandler = {
    $IDHandler: {
        _last_id: 0,
        _references: {},

        get: function(p_id) {
            return IDHandler._references[p_id];
        },

        add: function(p_data) {
            const id = ++IDHandler._last_id;
            IDHandler._references[id] = p_data;
            return id;
        },

        remove: function(p_id) {
            delete IDHandler._references[p_id];
        },
    },
};

autoAddDeps(IDHandler, '$IDHandler');
mergeInto(LibraryManager.library, IDHandler);

const PandemoniumConfig = {
    $PandemoniumConfig__postset: 'Module["initConfig"] = PandemoniumConfig.init_config;',
    $PandemoniumConfig__deps: ['$PandemoniumRuntime'],
    $PandemoniumConfig: {
        canvas: null,
        locale: 'en',
        canvas_resize_policy: 2, // Adaptive
        virtual_keyboard: false,
        persistent_drops: false,
        on_execute: null,
        on_exit: null,

        init_config: function(p_opts) {
            PandemoniumConfig.canvas_resize_policy = p_opts['canvasResizePolicy'];
            PandemoniumConfig.canvas = p_opts['canvas'];
            PandemoniumConfig.locale = p_opts['locale'] || PandemoniumConfig.locale;
            PandemoniumConfig.virtual_keyboard = p_opts['virtualKeyboard'];
            PandemoniumConfig.persistent_drops = !!p_opts['persistentDrops'];
            PandemoniumConfig.on_execute = p_opts['onExecute'];
            PandemoniumConfig.on_exit = p_opts['onExit'];
            if (p_opts['focusCanvas']) {
                PandemoniumConfig.canvas.focus();
            }
        },

        locate_file: function(file) {
            return Module['locateFile'](file); // eslint-disable-line no-undef
        },
        clear: function() {
            PandemoniumConfig.canvas = null;
            PandemoniumConfig.locale = 'en';
            PandemoniumConfig.canvas_resize_policy = 2;
            PandemoniumConfig.virtual_keyboard = false;
            PandemoniumConfig.persistent_drops = false;
            PandemoniumConfig.on_execute = null;
            PandemoniumConfig.on_exit = null;
        },
    },

    pandemonium_js_config_canvas_id_get__sig: 'vii',
    pandemonium_js_config_canvas_id_get: function(p_ptr, p_ptr_max) {
        PandemoniumRuntime.stringToHeap(`#${PandemoniumConfig.canvas.id}`, p_ptr, p_ptr_max);
    },

    pandemonium_js_config_locale_get__sig: 'vii',
    pandemonium_js_config_locale_get: function(p_ptr, p_ptr_max) {
        PandemoniumRuntime.stringToHeap(PandemoniumConfig.locale, p_ptr, p_ptr_max);
    },
};

autoAddDeps(PandemoniumConfig, '$PandemoniumConfig');
mergeInto(LibraryManager.library, PandemoniumConfig);

const PandemoniumFS = {
    $PandemoniumFS__deps: ['$ERRNO_CODES', '$FS', '$IDBFS', '$PandemoniumRuntime'],
    $PandemoniumFS__postset: [
        'Module["initFS"] = PandemoniumFS.init;',
        'Module["copyToFS"] = PandemoniumFS.copy_to_fs;',
    ].join(''),
    $PandemoniumFS: {
        _idbfs: false,
        _syncing: false,
        _mount_points: [],

        is_persistent: function() {
            return PandemoniumFS._idbfs ? 1 : 0;
        },

        // Initialize pandemonium file system, setting up persistent paths.
        // Returns a promise that resolves when the FS is ready.
        // We keep track of mount_points, so that we can properly close the IDBFS
        // since emscripten is not doing it by itself. (emscripten GH#12516).
        init: function(persistentPaths) {
            PandemoniumFS._idbfs = false;
            if (!Array.isArray(persistentPaths)) {
                return Promise.reject(new Error('Persistent paths must be an array'));
            }
            if (!persistentPaths.length) {
                return Promise.resolve();
            }
            PandemoniumFS._mount_points = persistentPaths.slice();

            function createRecursive(dir) {
                try {
                    FS.stat(dir);
                } catch (e) {
                    if (e.errno !== ERRNO_CODES.ENOENT) {
                        throw e;
                    }
                    FS.mkdirTree(dir);
                }
            }

            PandemoniumFS._mount_points.forEach(function(path) {
                createRecursive(path);
                FS.mount(IDBFS, {}, path);
            });
            return new Promise(function(resolve, reject) {
                FS.syncfs(true, function(err) {
                    if (err) {
                        PandemoniumFS._mount_points = [];
                        PandemoniumFS._idbfs = false;
                        PandemoniumRuntime.print(`IndexedDB not available: ${err.message}`);
                    } else {
                        PandemoniumFS._idbfs = true;
                    }
                    resolve(err);
                });
            });
        },

        // Deinit pandemonium file system, making sure to unmount file systems, and close IDBFS(s).
        deinit: function() {
            PandemoniumFS._mount_points.forEach(function(path) {
                try {
                    FS.unmount(path);
                } catch (e) {
                    PandemoniumRuntime.print('Already unmounted', e);
                }
                if (PandemoniumFS._idbfs && IDBFS.dbs[path]) {
                    IDBFS.dbs[path].close();
                    delete IDBFS.dbs[path];
                }
            });
            PandemoniumFS._mount_points = [];
            PandemoniumFS._idbfs = false;
            PandemoniumFS._syncing = false;
        },

        sync: function() {
            if (PandemoniumFS._syncing) {
                PandemoniumRuntime.error('Already syncing!');
                return Promise.resolve();
            }
            PandemoniumFS._syncing = true;
            return new Promise(function(resolve, reject) {
                FS.syncfs(false, function(error) {
                    if (error) {
                        PandemoniumRuntime.error(`Failed to save IDB file system: ${error.message}`);
                    }
                    PandemoniumFS._syncing = false;
                    resolve(error);
                });
            });
        },

        // Copies a buffer to the internal file system. Creating directories recursively.
        copy_to_fs: function(path, buffer) {
            const idx = path.lastIndexOf('/');
            let dir = '/';
            if (idx > 0) {
                dir = path.slice(0, idx);
            }
            try {
                FS.stat(dir);
            } catch (e) {
                if (e.errno !== ERRNO_CODES.ENOENT) {
                    throw e;
                }
                FS.mkdirTree(dir);
            }
            FS.writeFile(path, new Uint8Array(buffer));
        },
    },
};
mergeInto(LibraryManager.library, PandemoniumFS);

const PandemoniumOS = {
    $PandemoniumOS__deps: ['$PandemoniumRuntime', '$PandemoniumConfig', '$PandemoniumFS'],
    $PandemoniumOS__postset: [
        'Module["request_quit"] = function() { PandemoniumOS.request_quit() };',
        'Module["onExit"] = PandemoniumOS.cleanup;',
        'PandemoniumOS._fs_sync_promise = Promise.resolve();',
    ].join(''),
    $PandemoniumOS: {
        request_quit: function() {},
        _async_cbs: [],
        _fs_sync_promise: null,

        atexit: function(p_promise_cb) {
            PandemoniumOS._async_cbs.push(p_promise_cb);
        },

        cleanup: function(exit_code) {
            const cb = PandemoniumConfig.on_exit;
            PandemoniumFS.deinit();
            PandemoniumConfig.clear();
            if (cb) {
                cb(exit_code);
            }
        },

        finish_async: function(callback) {
            PandemoniumOS._fs_sync_promise.then(function(err) {
                const promises = [];
                PandemoniumOS._async_cbs.forEach(function(cb) {
                    promises.push(new Promise(cb));
                });
                return Promise.all(promises);
            }).then(function() {
                return PandemoniumFS.sync(); // Final FS sync.
            }).then(function(err) {
                // Always deferred.
                setTimeout(function() {
                    callback();
                }, 0);
            });
        },
    },

    pandemonium_js_os_finish_async__sig: 'vi',
    pandemonium_js_os_finish_async: function(p_callback) {
        const func = PandemoniumRuntime.get_func(p_callback);
        PandemoniumOS.finish_async(func);
    },

    pandemonium_js_os_request_quit_cb__sig: 'vi',
    pandemonium_js_os_request_quit_cb: function(p_callback) {
        PandemoniumOS.request_quit = PandemoniumRuntime.get_func(p_callback);
    },

    pandemonium_js_os_fs_is_persistent__sig: 'i',
    pandemonium_js_os_fs_is_persistent: function() {
        return PandemoniumFS.is_persistent();
    },

    pandemonium_js_os_fs_sync__sig: 'vi',
    pandemonium_js_os_fs_sync: function(callback) {
        const func = PandemoniumRuntime.get_func(callback);
        PandemoniumOS._fs_sync_promise = PandemoniumFS.sync();
        PandemoniumOS._fs_sync_promise.then(function(err) {
            func();
        });
    },

    pandemonium_js_os_execute__sig: 'ii',
    pandemonium_js_os_execute: function(p_json) {
        const json_args = PandemoniumRuntime.parseString(p_json);
        const args = JSON.parse(json_args);
        if (PandemoniumConfig.on_execute) {
            PandemoniumConfig.on_execute(args);
            return 0;
        }
        return 1;
    },

    pandemonium_js_os_shell_open__sig: 'vi',
    pandemonium_js_os_shell_open: function(p_uri) {
        window.open(PandemoniumRuntime.parseString(p_uri), '_blank');
    },

    pandemonium_js_os_hw_concurrency_get__sig: 'i',
    pandemonium_js_os_hw_concurrency_get: function() {
        return navigator.hardwareConcurrency || 1;
    },

    pandemonium_js_os_download_buffer__sig: 'viiii',
    pandemonium_js_os_download_buffer: function(p_ptr, p_size, p_name, p_mime) {
        const buf = PandemoniumRuntime.heapSlice(HEAP8, p_ptr, p_size);
        const name = PandemoniumRuntime.parseString(p_name);
        const mime = PandemoniumRuntime.parseString(p_mime);
        const blob = new Blob([buf], {
            type: mime
        });
        const url = window.URL.createObjectURL(blob);
        const a = document.createElement('a');
        a.href = url;
        a.download = name;
        a.style.display = 'none';
        document.body.appendChild(a);
        a.click();
        a.remove();
        window.URL.revokeObjectURL(url);
    },
};

autoAddDeps(PandemoniumOS, '$PandemoniumOS');
mergeInto(LibraryManager.library, PandemoniumOS);

/*
 * Pandemonium event listeners.
 * Keeps track of registered event listeners so it can remove them on shutdown.
 */
const PandemoniumEventListeners = {
    $PandemoniumEventListeners__deps: ['$PandemoniumOS'],
    $PandemoniumEventListeners__postset: 'PandemoniumOS.atexit(function(resolve, reject) { PandemoniumEventListeners.clear(); resolve(); });',
    $PandemoniumEventListeners: {
        handlers: [],

        has: function(target, event, method, capture) {
            return PandemoniumEventListeners.handlers.findIndex(function(e) {
                return e.target === target && e.event === event && e.method === method && e.capture === capture;
            }) !== -1;
        },

        add: function(target, event, method, capture) {
            if (PandemoniumEventListeners.has(target, event, method, capture)) {
                return;
            }

            function Handler(p_target, p_event, p_method, p_capture) {
                this.target = p_target;
                this.event = p_event;
                this.method = p_method;
                this.capture = p_capture;
            }
            PandemoniumEventListeners.handlers.push(new Handler(target, event, method, capture));
            target.addEventListener(event, method, capture);
        },

        clear: function() {
            PandemoniumEventListeners.handlers.forEach(function(h) {
                h.target.removeEventListener(h.event, h.method, h.capture);
            });
            PandemoniumEventListeners.handlers.length = 0;
        },
    },
};
mergeInto(LibraryManager.library, PandemoniumEventListeners);

const PandemoniumPWA = {

    $PandemoniumPWA__deps: ['$PandemoniumRuntime', '$PandemoniumEventListeners'],
    $PandemoniumPWA: {
        hasUpdate: false,

        updateState: function(cb, reg) {
            if (!reg) {
                return;
            }
            if (!reg.active) {
                return;
            }
            if (reg.waiting) {
                PandemoniumPWA.hasUpdate = true;
                cb();
            }
            PandemoniumEventListeners.add(reg, 'updatefound', function() {
                const installing = reg.installing;
                PandemoniumEventListeners.add(installing, 'statechange', function() {
                    if (installing.state === 'installed') {
                        PandemoniumPWA.hasUpdate = true;
                        cb();
                    }
                });
            });
        },
    },

    pandemonium_js_pwa_cb__sig: 'vi',
    pandemonium_js_pwa_cb: function(p_update_cb) {
        if ('serviceWorker' in navigator) {
            const cb = PandemoniumRuntime.get_func(p_update_cb);
            navigator.serviceWorker.getRegistration().then(PandemoniumPWA.updateState.bind(null, cb));
        }
    },

    pandemonium_js_pwa_update__sig: 'i',
    pandemonium_js_pwa_update: function() {
        if ('serviceWorker' in navigator && PandemoniumPWA.hasUpdate) {
            navigator.serviceWorker.getRegistration().then(function(reg) {
                if (!reg || !reg.waiting) {
                    return;
                }
                reg.waiting.postMessage('update');
            });
            return 0;
        }
        return 1;
    },
};

autoAddDeps(PandemoniumPWA, '$PandemoniumPWA');
mergeInto(LibraryManager.library, PandemoniumPWA);