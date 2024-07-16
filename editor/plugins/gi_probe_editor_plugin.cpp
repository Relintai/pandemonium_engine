/**************************************************************************/
/*  gi_probe_editor_plugin.cpp                                            */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "gi_probe_editor_plugin.h"

#include "editor/editor_node.h"
#include "scene/3d/gi_probe.h"
#include "scene/gui/tool_button.h"

void GIProbeEditorPlugin::_bake() {
	if (gi_probe) {
		gi_probe->bake();
	}
}

void GIProbeEditorPlugin::edit(Object *p_object) {
	GIProbe *s = Object::cast_to<GIProbe>(p_object);
	if (!s) {
		return;
	}

	gi_probe = s;
}

bool GIProbeEditorPlugin::handles(Object *p_object) const {
	return p_object->is_class("GIProbe");
}

void GIProbeEditorPlugin::make_visible(bool p_visible) {
	if (p_visible) {
		bake->show();
	} else {
		bake->hide();
	}
}

EditorProgress *GIProbeEditorPlugin::tmp_progress = nullptr;

void GIProbeEditorPlugin::bake_func_begin(int p_steps) {
	ERR_FAIL_COND(tmp_progress != nullptr);

	tmp_progress = memnew(EditorProgress("bake_gi", TTR("Bake GI Probe"), p_steps));
}

void GIProbeEditorPlugin::bake_func_step(int p_step, const String &p_description) {
	ERR_FAIL_COND(tmp_progress == nullptr);
	tmp_progress->step(p_description, p_step, false);
}

void GIProbeEditorPlugin::bake_func_end() {
	ERR_FAIL_COND(tmp_progress == nullptr);
	memdelete(tmp_progress);
	tmp_progress = nullptr;
}

void GIProbeEditorPlugin::_bind_methods() {
	ClassDB::bind_method("_bake", &GIProbeEditorPlugin::_bake);
}

GIProbeEditorPlugin::GIProbeEditorPlugin(EditorNode *p_node) {
	editor = p_node;
	bake = memnew(ToolButton);
	bake->set_icon(editor->get_gui_base()->get_theme_icon("Bake", "EditorIcons"));
	bake->set_text(TTR("Bake GI Probe"));
	bake->hide();
	bake->connect("pressed", this, "_bake");
	add_control_to_container(CONTAINER_SPATIAL_EDITOR_MENU, bake);
	gi_probe = nullptr;

	GIProbe::bake_begin_function = bake_func_begin;
	GIProbe::bake_step_function = bake_func_step;
	GIProbe::bake_end_function = bake_func_end;
}

GIProbeEditorPlugin::~GIProbeEditorPlugin() {
}
