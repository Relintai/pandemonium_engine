#ifndef ROOT_MOTION_EDITOR_PLUGIN_H
#define ROOT_MOTION_EDITOR_PLUGIN_H

/*************************************************************************/
/*  root_motion_editor_plugin.h                                          */
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

#include "core/object/object.h"
#include "core/string/node_path.h"
#include "core/string/ustring.h"
#include "core/variant/variant.h"

class Button;
class ConfirmationDialog;
class Tree;

class EditorPropertyRootMotion : public EditorProperty {
	GDCLASS(EditorPropertyRootMotion, EditorProperty);
	Button *assign;
	Button *clear;
	NodePath base_hint;

	ConfirmationDialog *filter_dialog;
	Tree *filters;

	void _confirmed();
	void _node_assign();
	void _node_clear();

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	virtual void update_property();
	void setup(const NodePath &p_base_hint);
	EditorPropertyRootMotion();
};

class EditorInspectorRootMotionPlugin : public EditorInspectorPlugin {
	GDCLASS(EditorInspectorRootMotionPlugin, EditorInspectorPlugin);

public:
	virtual bool can_handle(Object *p_object);
	virtual void parse_begin(Object *p_object);
	virtual bool parse_property(Object *p_object, Variant::Type p_type, const String &p_path, PropertyHint p_hint, const String &p_hint_text, int p_usage);
	virtual void parse_end();
};

#endif // ROOT_MOTION_EDITOR_PLUGIN_H
