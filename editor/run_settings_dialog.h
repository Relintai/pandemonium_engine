#ifndef RUN_SETTINGS_DIALOG_H
#define RUN_SETTINGS_DIALOG_H

/*************************************************************************/
/*  run_settings_dialog.h                                                */
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

#include "scene/gui/dialogs.h"

#include "core/string/ustring.h"

class LineEdit;
class OptionButton;

class RunSettingsDialog : public AcceptDialog {
	GDCLASS(RunSettingsDialog, AcceptDialog);

public:
	enum RunMode {
		RUN_LOCAL_SCENE,
		RUN_MAIN_SCENE,
	};

private:
	OptionButton *run_mode;
	LineEdit *arguments;

	void _run_mode_changed(int idx);

protected:
	static void _bind_methods();

public:
	int get_run_mode() const;
	void set_run_mode(int p_run_mode);

	void set_custom_arguments(const String &p_arguments);
	String get_custom_arguments() const;

	void popup_run_settings();

	RunSettingsDialog();
};

#endif // RUN_SETTINGS_DIALOG_H
