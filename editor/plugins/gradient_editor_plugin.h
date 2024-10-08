#ifndef TOOLS_EDITOR_PLUGINS_COLOR_RAMP_EDITOR_PLUGIN_H_
#define TOOLS_EDITOR_PLUGINS_COLOR_RAMP_EDITOR_PLUGIN_H_

/*************************************************************************/
/*  gradient_editor_plugin.h                                             */
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

#include "editor/editor_inspector.h"
#include "editor/editor_plugin.h"
#include "scene/gui/gradient_edit.h"

#include "core/math/vector2.h"
#include "core/object/object.h"
#include "core/object/reference.h"
#include "core/string/ustring.h"

class Gradient;
class EditorNode;

class GradientEditor : public GradientEdit {
	GDCLASS(GradientEditor, GradientEdit);

	bool editing;
	Ref<Gradient> gradient;

	void _gradient_changed();
	void _ramp_changed();

protected:
	static void _bind_methods();

public:
	virtual Size2 get_minimum_size() const;
	void set_gradient(const Ref<Gradient> &p_gradient);
	GradientEditor();
	~GradientEditor();
};

class EditorInspectorPluginGradient : public EditorInspectorPlugin {
	GDCLASS(EditorInspectorPluginGradient, EditorInspectorPlugin);

public:
	virtual bool can_handle(Object *p_object);
	virtual void parse_begin(Object *p_object);
};

class GradientEditorPlugin : public EditorPlugin {
	GDCLASS(GradientEditorPlugin, EditorPlugin);

public:
	virtual String get_name() const { return "ColorRamp"; }

	GradientEditorPlugin(EditorNode *p_node);
};

#endif /* TOOLS_EDITOR_PLUGINS_COLOR_RAMP_EDITOR_PLUGIN_H_ */
