/*************************************************************************/
/*  library_pandemonium_display.js                                             */
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

const PandemoniumDisplayVK = {

  $PandemoniumDisplayVK__deps: ['$PandemoniumRuntime', '$PandemoniumConfig', '$PandemoniumEventListeners'],
  $PandemoniumDisplayVK__postset: 'PandemoniumOS.atexit(function(resolve, reject) { PandemoniumDisplayVK.clear(); resolve(); });',
  $PandemoniumDisplayVK: {
    textinput: null,
    textarea: null,

    available: function() {
      return PandemoniumConfig.virtual_keyboard && 'ontouchstart' in window;
    },

    init: function(input_cb) {
      function create(what) {
        const elem = document.createElement(what);
        elem.style.display = 'none';
        elem.style.position = 'absolute';
        elem.style.zIndex = '-1';
        elem.style.background = 'transparent';
        elem.style.padding = '0px';
        elem.style.margin = '0px';
        elem.style.overflow = 'hidden';
        elem.style.width = '0px';
        elem.style.height = '0px';
        elem.style.border = '0px';
        elem.style.outline = 'none';
        elem.readonly = true;
        elem.disabled = true;
        PandemoniumEventListeners.add(elem, 'input', function(evt) {
          const c_str = PandemoniumRuntime.allocString(elem.value);
          input_cb(c_str, elem.selectionEnd);
          PandemoniumRuntime.free(c_str);
        }, false);
        PandemoniumEventListeners.add(elem, 'blur', function(evt) {
          elem.style.display = 'none';
          elem.readonly = true;
          elem.disabled = true;
        }, false);
        PandemoniumConfig.canvas.insertAdjacentElement('beforebegin', elem);
        return elem;
      }
      PandemoniumDisplayVK.textinput = create('input');
      PandemoniumDisplayVK.textarea = create('textarea');
      PandemoniumDisplayVK.updateSize();
    },
    show: function(text, multiline, start, end) {
      if (!PandemoniumDisplayVK.textinput || !PandemoniumDisplayVK.textarea) {
        return;
      }
      if (PandemoniumDisplayVK.textinput.style.display !== '' || PandemoniumDisplayVK.textarea.style.display !== '') {
        PandemoniumDisplayVK.hide();
      }
      PandemoniumDisplayVK.updateSize();
      const elem = multiline ? PandemoniumDisplayVK.textarea : PandemoniumDisplayVK.textinput;
      elem.readonly = false;
      elem.disabled = false;
      elem.value = text;
      elem.style.display = 'block';
      elem.focus();
      elem.setSelectionRange(start, end);
    },
    hide: function() {
      if (!PandemoniumDisplayVK.textinput || !PandemoniumDisplayVK.textarea) {
        return;
      }
      [PandemoniumDisplayVK.textinput, PandemoniumDisplayVK.textarea].forEach(function(elem) {
        elem.blur();
        elem.style.display = 'none';
        elem.value = '';
      });
    },
    updateSize: function() {
      if (!PandemoniumDisplayVK.textinput || !PandemoniumDisplayVK.textarea) {
        return;
      }
      const rect = PandemoniumConfig.canvas.getBoundingClientRect();

      function update(elem) {
        elem.style.left = `${rect.left}px`;
        elem.style.top = `${rect.top}px`;
        elem.style.width = `${rect.width}px`;
        elem.style.height = `${rect.height}px`;
      }
      update(PandemoniumDisplayVK.textinput);
      update(PandemoniumDisplayVK.textarea);
    },
    clear: function() {
      if (PandemoniumDisplayVK.textinput) {
        PandemoniumDisplayVK.textinput.remove();
        PandemoniumDisplayVK.textinput = null;
      }
      if (PandemoniumDisplayVK.textarea) {
        PandemoniumDisplayVK.textarea.remove();
        PandemoniumDisplayVK.textarea = null;
      }
    },
  },
};
mergeInto(LibraryManager.library, PandemoniumDisplayVK);

/*
 * Display server cursor helper.
 * Keeps track of cursor status and custom shapes.
 */
const PandemoniumDisplayCursor = {
  $PandemoniumDisplayCursor__deps: ['$PandemoniumOS', '$PandemoniumConfig'],
  $PandemoniumDisplayCursor__postset: 'PandemoniumOS.atexit(function(resolve, reject) { PandemoniumDisplayCursor.clear(); resolve(); });',
  $PandemoniumDisplayCursor: {
    shape: 'auto',
    visible: true,
    cursors: {},
    set_style: function(style) {
      PandemoniumConfig.canvas.style.cursor = style;
    },
    set_shape: function(shape) {
      PandemoniumDisplayCursor.shape = shape;
      let css = shape;
      if (shape in PandemoniumDisplayCursor.cursors) {
        const c = PandemoniumDisplayCursor.cursors[shape];
        css = `url("${c.url}") ${c.x} ${c.y}, auto`;
      }
      if (PandemoniumDisplayCursor.visible) {
        PandemoniumDisplayCursor.set_style(css);
      }
    },
    clear: function() {
      PandemoniumDisplayCursor.set_style('');
      PandemoniumDisplayCursor.shape = 'auto';
      PandemoniumDisplayCursor.visible = true;
      Object.keys(PandemoniumDisplayCursor.cursors).forEach(function(key) {
        URL.revokeObjectURL(PandemoniumDisplayCursor.cursors[key]);
        delete PandemoniumDisplayCursor.cursors[key];
      });
    },
    lockPointer: function() {
      const canvas = PandemoniumConfig.canvas;
      if (canvas.requestPointerLock) {
        canvas.requestPointerLock();
      }
    },
    releasePointer: function() {
      if (document.exitPointerLock) {
        document.exitPointerLock();
      }
    },
    isPointerLocked: function() {
      return document.pointerLockElement === PandemoniumConfig.canvas;
    },
  },
};
mergeInto(LibraryManager.library, PandemoniumDisplayCursor);

const PandemoniumDisplayScreen = {
  $PandemoniumDisplayScreen__deps: ['$PandemoniumConfig', '$PandemoniumOS', '$GL', 'emscripten_webgl_get_current_context'],
  $PandemoniumDisplayScreen: {
    desired_size: [0, 0],
    hidpi: true,
    getPixelRatio: function() {
      return PandemoniumDisplayScreen.hidpi ? window.devicePixelRatio || 1 : 1;
    },
    isFullscreen: function() {
      const elem = document.fullscreenElement || document.mozFullscreenElement ||
        document.webkitFullscreenElement || document.msFullscreenElement;
      if (elem) {
        return elem === PandemoniumConfig.canvas;
      }
      // But maybe knowing the element is not supported.
      return document.fullscreen || document.mozFullScreen ||
        document.webkitIsFullscreen;
    },
    hasFullscreen: function() {
      return document.fullscreenEnabled || document.mozFullScreenEnabled ||
        document.webkitFullscreenEnabled;
    },
    requestFullscreen: function() {
      if (!PandemoniumDisplayScreen.hasFullscreen()) {
        return 1;
      }
      const canvas = PandemoniumConfig.canvas;
      try {
        const promise = (canvas.requestFullscreen || canvas.msRequestFullscreen ||
          canvas.mozRequestFullScreen || canvas.mozRequestFullscreen ||
          canvas.webkitRequestFullscreen
        ).call(canvas);
        // Some browsers (Safari) return undefined.
        // For the standard ones, we need to catch it.
        if (promise) {
          promise.catch(function() {
            // nothing to do.
          });
        }
      } catch (e) {
        return 1;
      }
      return 0;
    },
    exitFullscreen: function() {
      if (!PandemoniumDisplayScreen.isFullscreen()) {
        return 0;
      }
      try {
        const promise = document.exitFullscreen();
        if (promise) {
          promise.catch(function() {
            // nothing to do.
          });
        }
      } catch (e) {
        return 1;
      }
      return 0;
    },
    _updateGL: function() {
      const gl_context_handle = _emscripten_webgl_get_current_context(); // eslint-disable-line no-undef
      const gl = GL.getContext(gl_context_handle);
      if (gl) {
        GL.resizeOffscreenFramebuffer(gl);
      }
    },
    updateSize: function() {
      const isFullscreen = PandemoniumDisplayScreen.isFullscreen();
      const wantsFullWindow = PandemoniumConfig.canvas_resize_policy === 2;
      const noResize = PandemoniumConfig.canvas_resize_policy === 0;
      const wwidth = PandemoniumDisplayScreen.desired_size[0];
      const wheight = PandemoniumDisplayScreen.desired_size[1];
      const canvas = PandemoniumConfig.canvas;
      let width = wwidth;
      let height = wheight;
      if (noResize) {
        // Don't resize canvas, just update GL if needed.
        if (canvas.width !== width || canvas.height !== height) {
          PandemoniumDisplayScreen.desired_size = [canvas.width, canvas.height];
          PandemoniumDisplayScreen._updateGL();
          return 1;
        }
        return 0;
      }
      const scale = PandemoniumDisplayScreen.getPixelRatio();
      if (isFullscreen || wantsFullWindow) {
        // We need to match screen size.
        width = window.innerWidth * scale;
        height = window.innerHeight * scale;
      }
      const csw = `${width / scale}px`;
      const csh = `${height / scale}px`;
      if (canvas.style.width !== csw || canvas.style.height !== csh || canvas.width !== width || canvas.height !== height) {
        // Size doesn't match.
        // Resize canvas, set correct CSS pixel size, update GL.
        canvas.width = width;
        canvas.height = height;
        canvas.style.width = csw;
        canvas.style.height = csh;
        PandemoniumDisplayScreen._updateGL();
        return 1;
      }
      return 0;
    },
  },
};
mergeInto(LibraryManager.library, PandemoniumDisplayScreen);

/**
 * Display server interface.
 *
 * Exposes all the functions needed by DisplayServer implementation.
 */
const PandemoniumDisplay = {
  $PandemoniumDisplay__deps: ['$PandemoniumConfig', '$PandemoniumRuntime', '$PandemoniumDisplayCursor', '$PandemoniumEventListeners', '$PandemoniumDisplayScreen', '$PandemoniumDisplayVK'],
  $PandemoniumDisplay: {
    window_icon: '',
    findDPI: function() {
      function testDPI(dpi) {
        return window.matchMedia(`(max-resolution: ${dpi}dpi)`).matches;
      }

      function bisect(low, high, func) {
        const mid = parseInt(((high - low) / 2) + low, 10);
        if (high - low <= 1) {
          return func(high) ? high : low;
        }
        if (func(mid)) {
          return bisect(low, mid, func);
        }
        return bisect(mid, high, func);
      }
      try {
        const dpi = bisect(0, 800, testDPI);
        return dpi >= 96 ? dpi : 96;
      } catch (e) {
        return 96;
      }
    },
  },

  // This is implemented as "glGetBufferSubData" in new emscripten versions.
  // Since we have to support older (pre 2.0.17) emscripten versions, we add this wrapper function instead.
  pandemonium_js_display_glGetBufferSubData__sig: 'viiii',
  pandemonium_js_display_glGetBufferSubData__deps: ['$GL', 'emscripten_webgl_get_current_context'],
  pandemonium_js_display_glGetBufferSubData: function(target, offset, size, data) {
    const gl_context_handle = _emscripten_webgl_get_current_context(); // eslint-disable-line no-undef
    const gl = GL.getContext(gl_context_handle);
    if (gl) {
      gl.GLctx['getBufferSubData'](target, offset, HEAPU8, data, size);
    }
  },

  pandemonium_js_display_is_swap_ok_cancel__sig: 'i',
  pandemonium_js_display_is_swap_ok_cancel: function() {
    const win = (['Windows', 'Win64', 'Win32', 'WinCE']);
    const plat = navigator.platform || '';
    if (win.indexOf(plat) !== -1) {
      return 1;
    }
    return 0;
  },

  pandemonium_js_display_alert__sig: 'vi',
  pandemonium_js_display_alert: function(p_text) {
    window.alert(PandemoniumRuntime.parseString(p_text)); // eslint-disable-line no-alert
  },

  pandemonium_js_display_screen_dpi_get__sig: 'i',
  pandemonium_js_display_screen_dpi_get: function() {
    return PandemoniumDisplay.findDPI();
  },

  pandemonium_js_display_pixel_ratio_get__sig: 'f',
  pandemonium_js_display_pixel_ratio_get: function() {
    return PandemoniumDisplayScreen.getPixelRatio();
  },

  pandemonium_js_display_fullscreen_request__sig: 'i',
  pandemonium_js_display_fullscreen_request: function() {
    return PandemoniumDisplayScreen.requestFullscreen();
  },

  pandemonium_js_display_fullscreen_exit__sig: 'i',
  pandemonium_js_display_fullscreen_exit: function() {
    return PandemoniumDisplayScreen.exitFullscreen();
  },

  pandemonium_js_display_desired_size_set__sig: 'vii',
  pandemonium_js_display_desired_size_set: function(width, height) {
    PandemoniumDisplayScreen.desired_size = [width, height];
    PandemoniumDisplayScreen.updateSize();
  },

  pandemonium_js_display_size_update__sig: 'i',
  pandemonium_js_display_size_update: function() {
    const updated = PandemoniumDisplayScreen.updateSize();
    if (updated) {
      PandemoniumDisplayVK.updateSize();
    }
    return updated;
  },

  pandemonium_js_display_screen_size_get__sig: 'vii',
  pandemonium_js_display_screen_size_get: function(width, height) {
    const scale = PandemoniumDisplayScreen.getPixelRatio();
    PandemoniumRuntime.setHeapValue(width, window.screen.width * scale, 'i32');
    PandemoniumRuntime.setHeapValue(height, window.screen.height * scale, 'i32');
  },

  pandemonium_js_display_window_size_get__sig: 'vii',
  pandemonium_js_display_window_size_get: function(p_width, p_height) {
    PandemoniumRuntime.setHeapValue(p_width, PandemoniumConfig.canvas.width, 'i32');
    PandemoniumRuntime.setHeapValue(p_height, PandemoniumConfig.canvas.height, 'i32');
  },

  pandemonium_js_display_has_webgl__sig: 'ii',
  pandemonium_js_display_has_webgl: function(p_version) {
    if (p_version !== 1 && p_version !== 2) {
      return false;
    }
    try {
      return !!document.createElement('canvas').getContext(p_version === 2 ? 'webgl2' : 'webgl');
    } catch (e) {
      /* Not available */
    }
    return false;
  },

  /*
   * Canvas
   */
  pandemonium_js_display_canvas_focus__sig: 'v',
  pandemonium_js_display_canvas_focus: function() {
    PandemoniumConfig.canvas.focus();
  },

  pandemonium_js_display_canvas_is_focused__sig: 'i',
  pandemonium_js_display_canvas_is_focused: function() {
    return document.activeElement === PandemoniumConfig.canvas;
  },

  /*
   * Touchscreen
   */
  pandemonium_js_display_touchscreen_is_available__sig: 'i',
  pandemonium_js_display_touchscreen_is_available: function() {
    return 'ontouchstart' in window;
  },

  /*
   * Clipboard
   */
  pandemonium_js_display_clipboard_set__sig: 'ii',
  pandemonium_js_display_clipboard_set: function(p_text) {
    const text = PandemoniumRuntime.parseString(p_text);
    if (!navigator.clipboard || !navigator.clipboard.writeText) {
      return 1;
    }
    navigator.clipboard.writeText(text).catch(function(e) {
      // Setting OS clipboard is only possible from an input callback.
      PandemoniumRuntime.error('Setting OS clipboard is only possible from an input callback for the HTML5 plafrom. Exception:', e);
    });
    return 0;
  },

  pandemonium_js_display_clipboard_get__sig: 'ii',
  pandemonium_js_display_clipboard_get: function(callback) {
    const func = PandemoniumRuntime.get_func(callback);
    try {
      navigator.clipboard.readText().then(function(result) {
        const ptr = PandemoniumRuntime.allocString(result);
        func(ptr);
        PandemoniumRuntime.free(ptr);
      }).catch(function(e) {
        // Fail graciously.
      });
    } catch (e) {
      // Fail graciously.
    }
  },

  /*
   * Window
   */
  pandemonium_js_display_window_title_set__sig: 'vi',
  pandemonium_js_display_window_title_set: function(p_data) {
    document.title = PandemoniumRuntime.parseString(p_data);
  },

  pandemonium_js_display_window_icon_set__sig: 'vii',
  pandemonium_js_display_window_icon_set: function(p_ptr, p_len) {
    let link = document.getElementById('-gd-engine-icon');
    if (link === null) {
      link = document.createElement('link');
      link.rel = 'icon';
      link.id = '-gd-engine-icon';
      document.head.appendChild(link);
    }
    const old_icon = PandemoniumDisplay.window_icon;
    const png = new Blob([PandemoniumRuntime.heapSlice(HEAPU8, p_ptr, p_len)], {
      type: 'image/png'
    });
    PandemoniumDisplay.window_icon = URL.createObjectURL(png);
    link.href = PandemoniumDisplay.window_icon;
    if (old_icon) {
      URL.revokeObjectURL(old_icon);
    }
  },

  /*
   * Cursor
   */
  pandemonium_js_display_cursor_set_visible__sig: 'vi',
  pandemonium_js_display_cursor_set_visible: function(p_visible) {
    const visible = p_visible !== 0;
    if (visible === PandemoniumDisplayCursor.visible) {
      return;
    }
    PandemoniumDisplayCursor.visible = visible;
    if (visible) {
      PandemoniumDisplayCursor.set_shape(PandemoniumDisplayCursor.shape);
    } else {
      PandemoniumDisplayCursor.set_style('none');
    }
  },

  pandemonium_js_display_cursor_is_hidden__sig: 'i',
  pandemonium_js_display_cursor_is_hidden: function() {
    return !PandemoniumDisplayCursor.visible;
  },

  pandemonium_js_display_cursor_set_shape__sig: 'vi',
  pandemonium_js_display_cursor_set_shape: function(p_string) {
    PandemoniumDisplayCursor.set_shape(PandemoniumRuntime.parseString(p_string));
  },

  pandemonium_js_display_cursor_set_custom_shape__sig: 'viiiii',
  pandemonium_js_display_cursor_set_custom_shape: function(p_shape, p_ptr, p_len, p_hotspot_x, p_hotspot_y) {
    const shape = PandemoniumRuntime.parseString(p_shape);
    const old_shape = PandemoniumDisplayCursor.cursors[shape];
    if (p_len > 0) {
      const png = new Blob([PandemoniumRuntime.heapSlice(HEAPU8, p_ptr, p_len)], {
        type: 'image/png'
      });
      const url = URL.createObjectURL(png);
      PandemoniumDisplayCursor.cursors[shape] = {
        url: url,
        x: p_hotspot_x,
        y: p_hotspot_y,
      };
    } else {
      delete PandemoniumDisplayCursor.cursors[shape];
    }
    if (shape === PandemoniumDisplayCursor.shape) {
      PandemoniumDisplayCursor.set_shape(PandemoniumDisplayCursor.shape);
    }
    if (old_shape) {
      URL.revokeObjectURL(old_shape.url);
    }
  },

  pandemonium_js_display_cursor_lock_set__sig: 'vi',
  pandemonium_js_display_cursor_lock_set: function(p_lock) {
    if (p_lock) {
      PandemoniumDisplayCursor.lockPointer();
    } else {
      PandemoniumDisplayCursor.releasePointer();
    }
  },

  pandemonium_js_display_cursor_is_locked__sig: 'i',
  pandemonium_js_display_cursor_is_locked: function() {
    return PandemoniumDisplayCursor.isPointerLocked() ? 1 : 0;
  },

  /*
   * Listeners
   */
  pandemonium_js_display_fullscreen_cb__sig: 'vi',
  pandemonium_js_display_fullscreen_cb: function(callback) {
    const canvas = PandemoniumConfig.canvas;
    const func = PandemoniumRuntime.get_func(callback);

    function change_cb(evt) {
      if (evt.target === canvas) {
        func(PandemoniumDisplayScreen.isFullscreen());
      }
    }
    PandemoniumEventListeners.add(document, 'fullscreenchange', change_cb, false);
    PandemoniumEventListeners.add(document, 'mozfullscreenchange', change_cb, false);
    PandemoniumEventListeners.add(document, 'webkitfullscreenchange', change_cb, false);
  },

  pandemonium_js_display_window_blur_cb__sig: 'vi',
  pandemonium_js_display_window_blur_cb: function(callback) {
    const func = PandemoniumRuntime.get_func(callback);
    PandemoniumEventListeners.add(window, 'blur', function() {
      func();
    }, false);
  },

  pandemonium_js_display_notification_cb__sig: 'viiiii',
  pandemonium_js_display_notification_cb: function(callback, p_enter, p_exit, p_in, p_out) {
    const canvas = PandemoniumConfig.canvas;
    const func = PandemoniumRuntime.get_func(callback);
    const notif = [p_enter, p_exit, p_in, p_out];
    ['mouseover', 'mouseleave', 'focus', 'blur'].forEach(function(evt_name, idx) {
      PandemoniumEventListeners.add(canvas, evt_name, function() {
        func(notif[idx]);
      }, true);
    });
  },

  pandemonium_js_display_setup_canvas__sig: 'viiii',
  pandemonium_js_display_setup_canvas: function(p_width, p_height, p_fullscreen, p_hidpi) {
    const canvas = PandemoniumConfig.canvas;
    PandemoniumEventListeners.add(canvas, 'contextmenu', function(ev) {
      ev.preventDefault();
    }, false);
    PandemoniumEventListeners.add(canvas, 'webglcontextlost', function(ev) {
      alert('WebGL context lost, please reload the page'); // eslint-disable-line no-alert
      ev.preventDefault();
    }, false);
    PandemoniumDisplayScreen.hidpi = !!p_hidpi;
    switch (PandemoniumConfig.canvas_resize_policy) {
      case 0: // None
        PandemoniumDisplayScreen.desired_size = [canvas.width, canvas.height];
        break;
      case 1: // Project
        PandemoniumDisplayScreen.desired_size = [p_width, p_height];
        break;
      default: // Full window
        // Ensure we display in the right place, the size will be handled by updateSize
        canvas.style.position = 'absolute';
        canvas.style.top = 0;
        canvas.style.left = 0;
        break;
    }
    PandemoniumDisplayScreen.updateSize();
    if (p_fullscreen) {
      PandemoniumDisplayScreen.requestFullscreen();
    }
  },

  /*
   * Virtual Keyboard
   */
  pandemonium_js_display_vk_show__sig: 'viiii',
  pandemonium_js_display_vk_show: function(p_text, p_multiline, p_start, p_end) {
    const text = PandemoniumRuntime.parseString(p_text);
    const start = p_start > 0 ? p_start : 0;
    const end = p_end > 0 ? p_end : start;
    PandemoniumDisplayVK.show(text, p_multiline, start, end);
  },

  pandemonium_js_display_vk_hide__sig: 'v',
  pandemonium_js_display_vk_hide: function() {
    PandemoniumDisplayVK.hide();
  },

  pandemonium_js_display_vk_available__sig: 'i',
  pandemonium_js_display_vk_available: function() {
    return PandemoniumDisplayVK.available();
  },

  pandemonium_js_display_vk_cb__sig: 'vi',
  pandemonium_js_display_vk_cb: function(p_input_cb) {
    const input_cb = PandemoniumRuntime.get_func(p_input_cb);
    if (PandemoniumDisplayVK.available()) {
      PandemoniumDisplayVK.init(input_cb);
    }
  },
};

autoAddDeps(PandemoniumDisplay, '$PandemoniumDisplay');
mergeInto(LibraryManager.library, PandemoniumDisplay);
