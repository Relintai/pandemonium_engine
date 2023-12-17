/*************************************************************************/
/*  dom_keys.inc                                                         */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           PANDEMONIUM ENGINE                                */
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

#include "core/os/keyboard.h"

// See https://w3c.github.io/uievents-code/#code-value-tables
int dom_code2pandemonium_scancode(EM_UTF8 const p_code[32], EM_UTF8 const p_key[32], bool p_physical) {
#define DOM2PANDEMONIUM(p_str, p_pandemonium_code)                             \
	if (memcmp((const void *)p_str, (void *)p_code, strlen(p_str) + 1) == 0) { \
		return KEY_##p_pandemonium_code;                                       \
	}

	// Numpad section.
	DOM2PANDEMONIUM("NumLock", NUMLOCK);
	DOM2PANDEMONIUM("Numpad0", KP_0);
	DOM2PANDEMONIUM("Numpad1", KP_1);
	DOM2PANDEMONIUM("Numpad2", KP_2);
	DOM2PANDEMONIUM("Numpad3", KP_3);
	DOM2PANDEMONIUM("Numpad4", KP_4);
	DOM2PANDEMONIUM("Numpad5", KP_5);
	DOM2PANDEMONIUM("Numpad6", KP_6);
	DOM2PANDEMONIUM("Numpad7", KP_7);
	DOM2PANDEMONIUM("Numpad8", KP_8);
	DOM2PANDEMONIUM("Numpad9", KP_9);
	DOM2PANDEMONIUM("NumpadAdd", KP_ADD);
	DOM2PANDEMONIUM("NumpadBackspace", BACKSPACE);
	DOM2PANDEMONIUM("NumpadClear", CLEAR);
	DOM2PANDEMONIUM("NumpadClearEntry", CLEAR);
	//DOM2PANDEMONIUM("NumpadComma", UNKNOWN);
	DOM2PANDEMONIUM("NumpadDecimal", KP_PERIOD);
	DOM2PANDEMONIUM("NumpadDivide", KP_DIVIDE);
	DOM2PANDEMONIUM("NumpadEnter", KP_ENTER);
	DOM2PANDEMONIUM("NumpadEqual", EQUAL);
	//DOM2PANDEMONIUM("NumpadHash", UNKNOWN);
	//DOM2PANDEMONIUM("NumpadMemoryAdd", UNKNOWN);
	//DOM2PANDEMONIUM("NumpadMemoryClear", UNKNOWN);
	//DOM2PANDEMONIUM("NumpadMemoryRecall", UNKNOWN);
	//DOM2PANDEMONIUM("NumpadMemoryStore", UNKNOWN);
	//DOM2PANDEMONIUM("NumpadMemorySubtract", UNKNOWN);
	DOM2PANDEMONIUM("NumpadMultiply", KP_MULTIPLY);
	DOM2PANDEMONIUM("NumpadParenLeft", PARENLEFT);
	DOM2PANDEMONIUM("NumpadParenRight", PARENRIGHT);
	DOM2PANDEMONIUM("NumpadStar", KP_MULTIPLY); // or ASTERISK ?
	DOM2PANDEMONIUM("NumpadSubtract", KP_SUBTRACT);

	// Printable ASCII.
	if (!p_physical) {
		uint8_t b0 = (uint8_t)p_key[0];
		uint8_t b1 = (uint8_t)p_key[1];
		uint8_t b2 = (uint8_t)p_key[2];
		if (b1 == 0 && b0 > 0x1F && b0 < 0x7F) { // ASCII.
			if (b0 > 0x60 && b0 < 0x7B) { // Lowercase ASCII.
				b0 -= 32;
			}
			return b0;
		}

#define _U_2BYTES_MASK 0xE0
#define _U_2BYTES 0xC0
		// Latin-1 codes.
		if (b2 == 0 && (b0 & _U_2BYTES_MASK) == _U_2BYTES) { // 2-bytes utf8, only known latin.
			uint32_t key = ((b0 & ~_U_2BYTES_MASK) << 6) | (b1 & 0x3F);
			if (key >= 0xA0 && key <= 0xDF) {
				return key;
			}
			if (key >= 0xE0 && key <= 0xFF) { // Lowercase known latin.
				key -= 0x20;
				return key;
			}
		}
#undef _U_2BYTES_MASK
#undef _U_2BYTES
	}

	// Alphanumeric section.
	DOM2PANDEMONIUM("Backquote", QUOTELEFT);
	DOM2PANDEMONIUM("Backslash", BACKSLASH);
	DOM2PANDEMONIUM("BracketLeft", BRACKETLEFT);
	DOM2PANDEMONIUM("BracketRight", BRACKETRIGHT);
	DOM2PANDEMONIUM("Comma", COMMA);
	DOM2PANDEMONIUM("Digit0", 0);
	DOM2PANDEMONIUM("Digit1", 1);
	DOM2PANDEMONIUM("Digit2", 2);
	DOM2PANDEMONIUM("Digit3", 3);
	DOM2PANDEMONIUM("Digit4", 4);
	DOM2PANDEMONIUM("Digit5", 5);
	DOM2PANDEMONIUM("Digit6", 6);
	DOM2PANDEMONIUM("Digit7", 7);
	DOM2PANDEMONIUM("Digit8", 8);
	DOM2PANDEMONIUM("Digit9", 9);
	DOM2PANDEMONIUM("Equal", EQUAL);
	DOM2PANDEMONIUM("IntlBackslash", BACKSLASH);
	//DOM2PANDEMONIUM("IntlRo", UNKNOWN);
	DOM2PANDEMONIUM("IntlYen", YEN);

	DOM2PANDEMONIUM("KeyA", A);
	DOM2PANDEMONIUM("KeyB", B);
	DOM2PANDEMONIUM("KeyC", C);
	DOM2PANDEMONIUM("KeyD", D);
	DOM2PANDEMONIUM("KeyE", E);
	DOM2PANDEMONIUM("KeyF", F);
	DOM2PANDEMONIUM("KeyG", G);
	DOM2PANDEMONIUM("KeyH", H);
	DOM2PANDEMONIUM("KeyI", I);
	DOM2PANDEMONIUM("KeyJ", J);
	DOM2PANDEMONIUM("KeyK", K);
	DOM2PANDEMONIUM("KeyL", L);
	DOM2PANDEMONIUM("KeyM", M);
	DOM2PANDEMONIUM("KeyN", N);
	DOM2PANDEMONIUM("KeyO", O);
	DOM2PANDEMONIUM("KeyP", P);
	DOM2PANDEMONIUM("KeyQ", Q);
	DOM2PANDEMONIUM("KeyR", R);
	DOM2PANDEMONIUM("KeyS", S);
	DOM2PANDEMONIUM("KeyT", T);
	DOM2PANDEMONIUM("KeyU", U);
	DOM2PANDEMONIUM("KeyV", V);
	DOM2PANDEMONIUM("KeyW", W);
	DOM2PANDEMONIUM("KeyX", X);
	DOM2PANDEMONIUM("KeyY", Y);
	DOM2PANDEMONIUM("KeyZ", Z);

	DOM2PANDEMONIUM("Minus", MINUS);
	DOM2PANDEMONIUM("Period", PERIOD);
	DOM2PANDEMONIUM("Quote", APOSTROPHE);
	DOM2PANDEMONIUM("Semicolon", SEMICOLON);
	DOM2PANDEMONIUM("Slash", SLASH);

	// Functional keys in the Alphanumeric section.
	DOM2PANDEMONIUM("AltLeft", ALT);
	DOM2PANDEMONIUM("AltRight", ALT);
	DOM2PANDEMONIUM("Backspace", BACKSPACE);
	DOM2PANDEMONIUM("CapsLock", CAPSLOCK);
	DOM2PANDEMONIUM("ContextMenu", MENU);
	DOM2PANDEMONIUM("ControlLeft", CONTROL);
	DOM2PANDEMONIUM("ControlRight", CONTROL);
	DOM2PANDEMONIUM("Enter", ENTER);
	DOM2PANDEMONIUM("MetaLeft", SUPER_L);
	DOM2PANDEMONIUM("MetaRight", SUPER_R);
	DOM2PANDEMONIUM("ShiftLeft", SHIFT);
	DOM2PANDEMONIUM("ShiftRight", SHIFT);
	DOM2PANDEMONIUM("Space", SPACE);
	DOM2PANDEMONIUM("Tab", TAB);

	// ControlPad section.
	DOM2PANDEMONIUM("Delete", DELETE);
	DOM2PANDEMONIUM("End", END);
	DOM2PANDEMONIUM("Help", HELP);
	DOM2PANDEMONIUM("Home", HOME);
	DOM2PANDEMONIUM("Insert", INSERT);
	DOM2PANDEMONIUM("PageDown", PAGEDOWN);
	DOM2PANDEMONIUM("PageUp", PAGEUP);

	// ArrowPad section.
	DOM2PANDEMONIUM("ArrowDown", DOWN);
	DOM2PANDEMONIUM("ArrowLeft", LEFT);
	DOM2PANDEMONIUM("ArrowRight", RIGHT);
	DOM2PANDEMONIUM("ArrowUp", UP);

	// Function section.
	DOM2PANDEMONIUM("Escape", ESCAPE);
	DOM2PANDEMONIUM("F1", F1);
	DOM2PANDEMONIUM("F2", F2);
	DOM2PANDEMONIUM("F3", F3);
	DOM2PANDEMONIUM("F4", F4);
	DOM2PANDEMONIUM("F5", F5);
	DOM2PANDEMONIUM("F6", F6);
	DOM2PANDEMONIUM("F7", F7);
	DOM2PANDEMONIUM("F8", F8);
	DOM2PANDEMONIUM("F9", F9);
	DOM2PANDEMONIUM("F10", F10);
	DOM2PANDEMONIUM("F11", F11);
	DOM2PANDEMONIUM("F12", F12);
	//DOM2PANDEMONIUM("Fn", UNKNOWN); // never actually fired, but included in the standard draft.
	//DOM2PANDEMONIUM("FnLock", UNKNOWN);
	DOM2PANDEMONIUM("PrintScreen", PRINT);
	DOM2PANDEMONIUM("ScrollLock", SCROLLLOCK);
	DOM2PANDEMONIUM("Pause", PAUSE);

	// Media keys section.
	DOM2PANDEMONIUM("BrowserBack", BACK);
	DOM2PANDEMONIUM("BrowserFavorites", FAVORITES);
	DOM2PANDEMONIUM("BrowserForward", FORWARD);
	DOM2PANDEMONIUM("BrowserHome", OPENURL);
	DOM2PANDEMONIUM("BrowserRefresh", REFRESH);
	DOM2PANDEMONIUM("BrowserSearch", SEARCH);
	DOM2PANDEMONIUM("BrowserStop", STOP);
	//DOM2PANDEMONIUM("Eject", UNKNOWN);
	DOM2PANDEMONIUM("LaunchApp1", LAUNCH0);
	DOM2PANDEMONIUM("LaunchApp2", LAUNCH1);
	DOM2PANDEMONIUM("LaunchMail", LAUNCHMAIL);
	DOM2PANDEMONIUM("MediaPlayPause", MEDIAPLAY);
	DOM2PANDEMONIUM("MediaSelect", LAUNCHMEDIA);
	DOM2PANDEMONIUM("MediaStop", MEDIASTOP);
	DOM2PANDEMONIUM("MediaTrackNext", MEDIANEXT);
	DOM2PANDEMONIUM("MediaTrackPrevious", MEDIAPREVIOUS);
	//DOM2PANDEMONIUM("Power", UNKNOWN);
	//DOM2PANDEMONIUM("Sleep", UNKNOWN);
	DOM2PANDEMONIUM("AudioVolumeDown", VOLUMEDOWN);
	DOM2PANDEMONIUM("AudioVolumeMute", VOLUMEMUTE);
	DOM2PANDEMONIUM("AudioVolumeUp", VOLUMEUP);
	//DOM2PANDEMONIUM("WakeUp", UNKNOWN);
	return KEY_UNKNOWN;
#undef DOM2PANDEMONIUM
}
