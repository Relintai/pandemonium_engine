#ifndef PROGRESS_DIALOG_H
#define PROGRESS_DIALOG_H

/*************************************************************************/
/*  progress_dialog.h                                                    */
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

#include "scene/gui/box_container.h"
#include "scene/gui/popup.h"

#include "core/containers/rb_map.h"
#include "core/os/thread_safe.h"
#include "core/string/ustring.h"

class Button;
class Label;
class ProgressBar;

class BackgroundProgress : public HBoxContainer {
	GDCLASS(BackgroundProgress, HBoxContainer);

	_THREAD_SAFE_CLASS_

	struct Task {
		HBoxContainer *hb;
		ProgressBar *progress;
	};

	RBMap<String, Task> tasks;
	RBMap<String, int> updates;
	void _update();

protected:
	void _add_task(const String &p_task, const String &p_label, int p_steps);
	void _task_step(const String &p_task, int p_step = -1);
	void _end_task(const String &p_task);

	static void _bind_methods();

public:
	void add_task(const String &p_task, const String &p_label, int p_steps);
	void task_step(const String &p_task, int p_step = -1);
	void end_task(const String &p_task);

	BackgroundProgress() {}
};

class ProgressDialog : public Popup {
	GDCLASS(ProgressDialog, Popup);
	struct Task {
		String task;
		VBoxContainer *vb;
		ProgressBar *progress;
		Label *state;
		uint64_t last_progress_tick;
	};
	HBoxContainer *cancel_hb;
	Button *cancel;

	RBMap<String, Task> tasks;
	VBoxContainer *main;

	static ProgressDialog *singleton;
	void _popup();

	void _cancel_pressed();
	bool cancelled;

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	static ProgressDialog *get_singleton() { return singleton; }
	void add_task(const String &p_task, const String &p_label, int p_steps, bool p_can_cancel = false);
	bool task_step(const String &p_task, const String &p_state, int p_step = -1, bool p_force_redraw = true);
	void end_task(const String &p_task);

	ProgressDialog();
};

#endif // PROGRESS_DIALOG_H
