#ifndef IMPORT_GDKEYS_H
#define IMPORT_GDKEYS_H

/*************************************************************************/
/*  gdkeys.h                                                             */
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

// imported from godot (core/os/keyboard.h), adding prefixes to avoid
// clashes with the linux input api

enum { GD_SPKEY = (1 << 24) };

enum GdKeyList {
	/* CURSOR/FUNCTION/BROWSER/MULTIMEDIA/MISC KEYS */
	GD_KEY_ESCAPE = GD_SPKEY | 0x01,
	GD_KEY_TAB = GD_SPKEY | 0x02,
	GD_KEY_BACKTAB = GD_SPKEY | 0x03,
	GD_KEY_BACKSPACE = GD_SPKEY | 0x04,
	GD_KEY_RETURN = GD_SPKEY | 0x05,
	GD_KEY_ENTER = GD_SPKEY | 0x06,
	GD_KEY_INSERT = GD_SPKEY | 0x07,
	GD_KEY_DELETE = GD_SPKEY | 0x08,
	GD_KEY_PAUSE = GD_SPKEY | 0x09,
	GD_KEY_PRINT = GD_SPKEY | 0x0A,
	GD_KEY_SYSREQ = GD_SPKEY | 0x0B,
	GD_KEY_CLEAR = GD_SPKEY | 0x0C,
	GD_KEY_HOME = GD_SPKEY | 0x0D,
	GD_KEY_END = GD_SPKEY | 0x0E,
	GD_KEY_LEFT = GD_SPKEY | 0x0F,
	GD_KEY_UP = GD_SPKEY | 0x10,
	GD_KEY_RIGHT = GD_SPKEY | 0x11,
	GD_KEY_DOWN = GD_SPKEY | 0x12,
	GD_KEY_PAGEUP = GD_SPKEY | 0x13,
	GD_KEY_PAGEDOWN = GD_SPKEY | 0x14,
	GD_KEY_SHIFT = GD_SPKEY | 0x15,
	GD_KEY_CONTROL = GD_SPKEY | 0x16,
	GD_KEY_META = GD_SPKEY | 0x17,
	GD_KEY_ALT = GD_SPKEY | 0x18,
	GD_KEY_CAPSLOCK = GD_SPKEY | 0x19,
	GD_KEY_NUMLOCK = GD_SPKEY | 0x1A,
	GD_KEY_SCROLLLOCK = GD_SPKEY | 0x1B,
	GD_KEY_F1 = GD_SPKEY | 0x1C,
	GD_KEY_F2 = GD_SPKEY | 0x1D,
	GD_KEY_F3 = GD_SPKEY | 0x1E,
	GD_KEY_F4 = GD_SPKEY | 0x1F,
	GD_KEY_F5 = GD_SPKEY | 0x20,
	GD_KEY_F6 = GD_SPKEY | 0x21,
	GD_KEY_F7 = GD_SPKEY | 0x22,
	GD_KEY_F8 = GD_SPKEY | 0x23,
	GD_KEY_F9 = GD_SPKEY | 0x24,
	GD_KEY_F10 = GD_SPKEY | 0x25,
	GD_KEY_F11 = GD_SPKEY | 0x26,
	GD_KEY_F12 = GD_SPKEY | 0x27,
	GD_KEY_F13 = GD_SPKEY | 0x28,
	GD_KEY_F14 = GD_SPKEY | 0x29,
	GD_KEY_F15 = GD_SPKEY | 0x2A,
	GD_KEY_F16 = GD_SPKEY | 0x2B,
	GD_KEY_KP_ENTER = GD_SPKEY | 0x80,
	GD_KEY_KP_MULTIPLY = GD_SPKEY | 0x81,
	GD_KEY_KP_DIVIDE = GD_SPKEY | 0x82,
	GD_KEY_KP_SUBTRACT = GD_SPKEY | 0x83,
	GD_KEY_KP_PERIOD = GD_SPKEY | 0x84,
	GD_KEY_KP_ADD = GD_SPKEY | 0x85,
	GD_KEY_KP_0 = GD_SPKEY | 0x86,
	GD_KEY_KP_1 = GD_SPKEY | 0x87,
	GD_KEY_KP_2 = GD_SPKEY | 0x88,
	GD_KEY_KP_3 = GD_SPKEY | 0x89,
	GD_KEY_KP_4 = GD_SPKEY | 0x8A,
	GD_KEY_KP_5 = GD_SPKEY | 0x8B,
	GD_KEY_KP_6 = GD_SPKEY | 0x8C,
	GD_KEY_KP_7 = GD_SPKEY | 0x8D,
	GD_KEY_KP_8 = GD_SPKEY | 0x8E,
	GD_KEY_KP_9 = GD_SPKEY | 0x8F,
	GD_KEY_SUPER_L = GD_SPKEY | 0x2C,
	GD_KEY_SUPER_R = GD_SPKEY | 0x2D,
	GD_KEY_MENU = GD_SPKEY | 0x2E,
	GD_KEY_HYPER_L = GD_SPKEY | 0x2F,
	GD_KEY_HYPER_R = GD_SPKEY | 0x30,
	GD_KEY_HELP = GD_SPKEY | 0x31,
	GD_KEY_DIRECTION_L = GD_SPKEY | 0x32,
	GD_KEY_DIRECTION_R = GD_SPKEY | 0x33,
	GD_KEY_BACK = GD_SPKEY | 0x40,
	GD_KEY_FORWARD = GD_SPKEY | 0x41,
	GD_KEY_STOP = GD_SPKEY | 0x42,
	GD_KEY_REFRESH = GD_SPKEY | 0x43,
	GD_KEY_VOLUMEDOWN = GD_SPKEY | 0x44,
	GD_KEY_VOLUMEMUTE = GD_SPKEY | 0x45,
	GD_KEY_VOLUMEUP = GD_SPKEY | 0x46,
	GD_KEY_BASSBOOST = GD_SPKEY | 0x47,
	GD_KEY_BASSUP = GD_SPKEY | 0x48,
	GD_KEY_BASSDOWN = GD_SPKEY | 0x49,
	GD_KEY_TREBLEUP = GD_SPKEY | 0x4A,
	GD_KEY_TREBLEDOWN = GD_SPKEY | 0x4B,
	GD_KEY_MEDIAPLAY = GD_SPKEY | 0x4C,
	GD_KEY_MEDIASTOP = GD_SPKEY | 0x4D,
	GD_KEY_MEDIAPREVIOUS = GD_SPKEY | 0x4E,
	GD_KEY_MEDIANEXT = GD_SPKEY | 0x4F,
	GD_KEY_MEDIARECORD = GD_SPKEY | 0x50,
	GD_KEY_HOMEPAGE = GD_SPKEY | 0x51,
	GD_KEY_FAVORITES = GD_SPKEY | 0x52,
	GD_KEY_SEARCH = GD_SPKEY | 0x53,
	GD_KEY_STANDBY = GD_SPKEY | 0x54,
	GD_KEY_OPENURL = GD_SPKEY | 0x55,
	GD_KEY_LAUNCHMAIL = GD_SPKEY | 0x56,
	GD_KEY_LAUNCHMEDIA = GD_SPKEY | 0x57,
	GD_KEY_LAUNCH0 = GD_SPKEY | 0x58,
	GD_KEY_LAUNCH1 = GD_SPKEY | 0x59,
	GD_KEY_LAUNCH2 = GD_SPKEY | 0x5A,
	GD_KEY_LAUNCH3 = GD_SPKEY | 0x5B,
	GD_KEY_LAUNCH4 = GD_SPKEY | 0x5C,
	GD_KEY_LAUNCH5 = GD_SPKEY | 0x5D,
	GD_KEY_LAUNCH6 = GD_SPKEY | 0x5E,
	GD_KEY_LAUNCH7 = GD_SPKEY | 0x5F,
	GD_KEY_LAUNCH8 = GD_SPKEY | 0x60,
	GD_KEY_LAUNCH9 = GD_SPKEY | 0x61,
	GD_KEY_LAUNCHA = GD_SPKEY | 0x62,
	GD_KEY_LAUNCHB = GD_SPKEY | 0x63,
	GD_KEY_LAUNCHC = GD_SPKEY | 0x64,
	GD_KEY_LAUNCHD = GD_SPKEY | 0x65,
	GD_KEY_LAUNCHE = GD_SPKEY | 0x66,
	GD_KEY_LAUNCHF = GD_SPKEY | 0x67,

	GD_KEY_UNKNOWN = GD_SPKEY | 0xFFFFFF,
};

#endif // IMPORT_GDKEYS_H
