#ifndef EDITOR_RUN_NATIVE_H
#define EDITOR_RUN_NATIVE_H

/*************************************************************************/
/*  editor_run_native.h                                                  */
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

#include "core/containers/rb_map.h"
#include "core/object/object.h"

class MenuButton;
class RichTextLabel;
class AcceptDialog;

class EditorRunNative : public HBoxContainer {
	GDCLASS(EditorRunNative, HBoxContainer);

	RichTextLabel *result_dialog_log;
	AcceptDialog *result_dialog;

	RBMap<int, MenuButton *> menus;
	bool first;
	bool deploy_dumb;
	bool deploy_debug_remote;
	bool debug_collisions;
	bool debug_navigation;
	bool debug_avoidance;
	bool debug_paths;
	bool debug_shader_fallbacks;

	int resume_idx;
	int resume_platform;

	void _run_native(int p_idx, int p_platform);

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	void set_deploy_dumb(bool p_enabled);
	bool is_deploy_dumb_enabled() const;

	void set_deploy_debug_remote(bool p_enabled);
	bool is_deploy_debug_remote_enabled() const;

	void set_debug_collisions(bool p_debug);
	bool get_debug_collisions() const;

	void set_debug_navigation(bool p_debug);
	bool get_debug_navigation() const;

	void set_debug_avoidance(bool p_debug);
	bool get_debug_avoidance() const;

	void set_debug_paths(bool p_debug);
	bool get_debug_paths() const;

	void set_debug_shader_fallbacks(bool p_debug);
	bool get_debug_shader_fallbacks() const;

	void resume_run_native();

	EditorRunNative();
};

#endif // EDITOR_RUN_NATIVE_H
