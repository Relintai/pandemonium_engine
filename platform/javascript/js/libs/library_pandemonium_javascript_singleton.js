/*************************************************************************/
/*  library_pandemonium_eval.js                                                */
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

const PandemoniumJSWrapper = {

    $PandemoniumJSWrapper__deps: ['$PandemoniumRuntime', '$IDHandler'],
    $PandemoniumJSWrapper__postset: 'PandemoniumJSWrapper.proxies = new Map();',
    $PandemoniumJSWrapper: {
        proxies: null,

        MyProxy: function(val) {
            const id = IDHandler.add(this);
            PandemoniumJSWrapper.proxies.set(val, id);
            let refs = 1;
            this.ref = function() {
                refs++;
            };
            this.unref = function() {
                refs--;
                if (refs === 0) {
                    IDHandler.remove(id);
                    PandemoniumJSWrapper.proxies.delete(val);
                }
            };
            this.get_val = function() {
                return val;
            };
            this.get_id = function() {
                return id;
            };
        },

        get_proxied: function(val) {
            const id = PandemoniumJSWrapper.proxies.get(val);
            if (id === undefined) {
                const proxy = new PandemoniumJSWrapper.MyProxy(val);
                return proxy.get_id();
            }
            IDHandler.get(id).ref();
            return id;
        },

        get_proxied_value: function(id) {
            const proxy = IDHandler.get(id);
            if (proxy === undefined) {
                return undefined;
            }
            return proxy.get_val();
        },

        variant2js: function(type, val) {
            switch (type) {
                case 0:
                    return null;
                case 1:
                    return !!PandemoniumRuntime.getHeapValue(val, 'i64');
                case 2:
                    return PandemoniumRuntime.getHeapValue(val, 'i64');
                case 3:
                    return PandemoniumRuntime.getHeapValue(val, 'double');
                case 4:
                    return PandemoniumRuntime.parseString(PandemoniumRuntime.getHeapValue(val, '*'));
                case 17: // OBJECT
                    return PandemoniumJSWrapper.get_proxied_value(PandemoniumRuntime.getHeapValue(val, 'i64'));
                default:
                    return undefined;
            }
        },

        js2variant: function(p_val, p_exchange) {
            if (p_val === undefined || p_val === null) {
                return 0; // NIL
            }
            const type = typeof(p_val);
            if (type === 'boolean') {
                PandemoniumRuntime.setHeapValue(p_exchange, p_val, 'i64');
                return 1; // BOOL
            } else if (type === 'number') {
                if (Number.isInteger(p_val)) {
                    PandemoniumRuntime.setHeapValue(p_exchange, p_val, 'i64');
                    return 2; // INT
                }
                PandemoniumRuntime.setHeapValue(p_exchange, p_val, 'double');
                return 3; // REAL
            } else if (type === 'string') {
                const c_str = PandemoniumRuntime.allocString(p_val);
                PandemoniumRuntime.setHeapValue(p_exchange, c_str, '*');
                return 4; // STRING
            }
            const id = PandemoniumJSWrapper.get_proxied(p_val);
            PandemoniumRuntime.setHeapValue(p_exchange, id, 'i64');
            return 17;
        },
    },

    pandemonium_js_wrapper_interface_get__sig: 'ii',
    pandemonium_js_wrapper_interface_get: function(p_name) {
        const name = PandemoniumRuntime.parseString(p_name);
        if (typeof(window[name]) !== 'undefined') {
            return PandemoniumJSWrapper.get_proxied(window[name]);
        }
        return 0;
    },

    pandemonium_js_wrapper_object_get__sig: 'iiii',
    pandemonium_js_wrapper_object_get: function(p_id, p_exchange, p_prop) {
        const obj = PandemoniumJSWrapper.get_proxied_value(p_id);
        if (obj === undefined) {
            return 0;
        }
        if (p_prop) {
            const prop = PandemoniumRuntime.parseString(p_prop);
            try {
                return PandemoniumJSWrapper.js2variant(obj[prop], p_exchange);
            } catch (e) {
                PandemoniumRuntime.error(`Error getting variable ${prop} on object`, obj);
                return 0; // NIL
            }
        }
        return PandemoniumJSWrapper.js2variant(obj, p_exchange);
    },

    pandemonium_js_wrapper_object_set__sig: 'viiii',
    pandemonium_js_wrapper_object_set: function(p_id, p_name, p_type, p_exchange) {
        const obj = PandemoniumJSWrapper.get_proxied_value(p_id);
        if (obj === undefined) {
            return;
        }
        const name = PandemoniumRuntime.parseString(p_name);
        try {
            obj[name] = PandemoniumJSWrapper.variant2js(p_type, p_exchange);
        } catch (e) {
            PandemoniumRuntime.error(`Error setting variable ${name} on object`, obj);
        }
    },

    pandemonium_js_wrapper_object_call__sig: 'iiiiiiiii',
    pandemonium_js_wrapper_object_call: function(p_id, p_method, p_args, p_argc, p_convert_callback, p_exchange, p_lock, p_free_lock_callback) {
        const obj = PandemoniumJSWrapper.get_proxied_value(p_id);
        if (obj === undefined) {
            return -1;
        }
        const method = PandemoniumRuntime.parseString(p_method);
        const convert = PandemoniumRuntime.get_func(p_convert_callback);
        const freeLock = PandemoniumRuntime.get_func(p_free_lock_callback);
        const args = new Array(p_argc);
        for (let i = 0; i < p_argc; i++) {
            const type = convert(p_args, i, p_exchange, p_lock);
            const lock = PandemoniumRuntime.getHeapValue(p_lock, '*');
            args[i] = PandemoniumJSWrapper.variant2js(type, p_exchange);
            if (lock) {
                freeLock(p_lock, type);
            }
        }
        try {
            const res = obj[method](...args);
            return PandemoniumJSWrapper.js2variant(res, p_exchange);
        } catch (e) {
            PandemoniumRuntime.error(`Error calling method ${method} on:`, obj, 'error:', e);
            return -1;
        }
    },

    pandemonium_js_wrapper_object_unref__sig: 'vi',
    pandemonium_js_wrapper_object_unref: function(p_id) {
        const proxy = IDHandler.get(p_id);
        if (proxy !== undefined) {
            proxy.unref();
        }
    },

    pandemonium_js_wrapper_create_cb__sig: 'iii',
    pandemonium_js_wrapper_create_cb: function(p_ref, p_func) {
        const func = PandemoniumRuntime.get_func(p_func);
        let id = 0;
        const cb = function() {
            if (!PandemoniumJSWrapper.get_proxied_value(id)) {
                return;
            }
            const args = Array.from(arguments);
            const argsProxy = new PandemoniumJSWrapper.MyProxy(args);
            func(p_ref, argsProxy.get_id(), args.length);
            argsProxy.unref();
        };
        id = PandemoniumJSWrapper.get_proxied(cb);
        return id;
    },

    pandemonium_js_wrapper_object_getvar__sig: 'iiii',
    pandemonium_js_wrapper_object_getvar: function(p_id, p_type, p_exchange) {
        const obj = PandemoniumJSWrapper.get_proxied_value(p_id);
        if (obj === undefined) {
            return -1;
        }
        const prop = PandemoniumJSWrapper.variant2js(p_type, p_exchange);
        if (prop === undefined || prop === null) {
            return -1;
        }
        try {
            return PandemoniumJSWrapper.js2variant(obj[prop], p_exchange);
        } catch (e) {
            PandemoniumRuntime.error(`Error getting variable ${prop} on object`, obj, e);
            return -1;
        }
    },

    pandemonium_js_wrapper_object_setvar__sig: 'iiiiii',
    pandemonium_js_wrapper_object_setvar: function(p_id, p_key_type, p_key_ex, p_val_type, p_val_ex) {
        const obj = PandemoniumJSWrapper.get_proxied_value(p_id);
        if (obj === undefined) {
            return -1;
        }
        const key = PandemoniumJSWrapper.variant2js(p_key_type, p_key_ex);
        try {
            obj[key] = PandemoniumJSWrapper.variant2js(p_val_type, p_val_ex);
            return 0;
        } catch (e) {
            PandemoniumRuntime.error(`Error setting variable ${key} on object`, obj);
            return -1;
        }
    },

    pandemonium_js_wrapper_create_object__sig: 'iiiiiiii',
    pandemonium_js_wrapper_create_object: function(p_object, p_args, p_argc, p_convert_callback, p_exchange, p_lock, p_free_lock_callback) {
        const name = PandemoniumRuntime.parseString(p_object);
        if (typeof(window[name]) === 'undefined') {
            return -1;
        }
        const convert = PandemoniumRuntime.get_func(p_convert_callback);
        const freeLock = PandemoniumRuntime.get_func(p_free_lock_callback);
        const args = new Array(p_argc);
        for (let i = 0; i < p_argc; i++) {
            const type = convert(p_args, i, p_exchange, p_lock);
            const lock = PandemoniumRuntime.getHeapValue(p_lock, '*');
            args[i] = PandemoniumJSWrapper.variant2js(type, p_exchange);
            if (lock) {
                freeLock(p_lock, type);
            }
        }
        try {
            const res = new window[name](...args);
            return PandemoniumJSWrapper.js2variant(res, p_exchange);
        } catch (e) {
            PandemoniumRuntime.error(`Error calling constructor ${name} with args:`, args, 'error:', e);
            return -1;
        }
    },
};

autoAddDeps(PandemoniumJSWrapper, '$PandemoniumJSWrapper');
mergeInto(LibraryManager.library, PandemoniumJSWrapper);

const PandemoniumEval = {
    pandemonium_js_eval__deps: ['$PandemoniumRuntime'],
    pandemonium_js_eval__sig: 'iiiiiii',
    pandemonium_js_eval: function(p_js, p_use_global_ctx, p_union_ptr, p_byte_arr, p_byte_arr_write, p_callback) {
        const js_code = PandemoniumRuntime.parseString(p_js);
        let eval_ret = null;
        try {
            if (p_use_global_ctx) {
                // indirect eval call grants global execution context
                const global_eval = eval; // eslint-disable-line no-eval
                eval_ret = global_eval(js_code);
            } else {
                eval_ret = eval(js_code); // eslint-disable-line no-eval
            }
        } catch (e) {
            PandemoniumRuntime.error(e);
        }

        switch (typeof eval_ret) {
            case 'boolean':
                PandemoniumRuntime.setHeapValue(p_union_ptr, eval_ret, 'i32');
                return 1; // BOOL

            case 'number':
                PandemoniumRuntime.setHeapValue(p_union_ptr, eval_ret, 'double');
                return 3; // REAL

            case 'string':
                PandemoniumRuntime.setHeapValue(p_union_ptr, PandemoniumRuntime.allocString(eval_ret), '*');
                return 4; // STRING

            case 'object':
                if (eval_ret === null) {
                    break;
                }

                if (ArrayBuffer.isView(eval_ret) && !(eval_ret instanceof Uint8Array)) {
                    eval_ret = new Uint8Array(eval_ret.buffer);
                } else if (eval_ret instanceof ArrayBuffer) {
                    eval_ret = new Uint8Array(eval_ret);
                }
                if (eval_ret instanceof Uint8Array) {
                    const func = PandemoniumRuntime.get_func(p_callback);
                    const bytes_ptr = func(p_byte_arr, p_byte_arr_write, eval_ret.length);
                    HEAPU8.set(eval_ret, bytes_ptr);
                    return 20; // POOL_BYTE_ARRAY
                }
                break;

                // no default
        }
        return 0; // NIL
    },
};

mergeInto(LibraryManager.library, PandemoniumEval);