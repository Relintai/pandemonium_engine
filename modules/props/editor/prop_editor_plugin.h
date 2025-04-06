#ifndef PROP_EDITOR_PLUGIN_H
#define PROP_EDITOR_PLUGIN_H

/*************************************************************************/
/*  prop_editor_plugin.h                                                 */
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

#include "editor/editor_node.h"
#include "editor/editor_plugin.h"

#include "editor/spatial_editor_gizmos.h"

class PropInstanceSpatialGizmoPlugin;

class PropEditorPlugin : public EditorPlugin {
	GDCLASS(PropEditorPlugin, EditorPlugin);

public:
	virtual String get_name() const { return "PropEditorPlugin"; }
	bool has_main_screen() const { return false; }
	virtual void edit(Object *p_object) {}
	virtual bool handles(Object *p_object) const { return false; }
	virtual void make_visible(bool p_visible) {}

	void convert_active_scene_to_prop_data();
	void convert_selected_scene_to_prop_data();
	void convert_scene(Node *root, const String &path);

	void find_room_points(Variant param);

	void _convert_active_scene_to_prop_data(Variant param);
	void _convert_selected_scene_to_prop_data(Variant param);
	void _quick_convert_button_pressed();

	PropEditorPlugin(EditorNode *p_node);
	~PropEditorPlugin();

protected:
	static void _bind_methods();

	EditorNode *editor;

	Ref<PropInstanceSpatialGizmoPlugin> gizmo_plugin;
};

class PropInstanceSpatialGizmoPlugin : public EditorSpatialGizmoPlugin {
	GDCLASS(PropInstanceSpatialGizmoPlugin, EditorSpatialGizmoPlugin);

public:
	bool has_gizmo(Spatial *p_spatial);
	String get_gizmo_name() const;
	int get_priority() const;
	bool can_be_hidden() const;
	void redraw(EditorSpatialGizmo *p_gizmo);

	PropInstanceSpatialGizmoPlugin();
};

#endif
