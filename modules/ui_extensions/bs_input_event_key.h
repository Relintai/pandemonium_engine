#ifndef BS_INPUT_EVENT_KEY_H
#define BS_INPUT_EVENT_KEY_H
/*
Copyright (c) 2019-2023 Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "core/input/input_event.h"

class BSInputEventKey : public InputEventKey {
	GDCLASS(BSInputEventKey, InputEventKey);

public:
	void from_input_event_key(const Ref<InputEventKey> p_event);
	virtual bool action_match(const Ref<InputEvent> &p_event, bool p_exact_match, bool *p_pressed, float *p_strength, float *p_raw_strength, float p_deadzone) const;

	BSInputEventKey();

protected:
	static void _bind_methods();
};

#endif
