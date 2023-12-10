#ifndef NAVIGATION_OBSTACLE_EDITOR_PLUGIN_H
#define NAVIGATION_OBSTACLE_EDITOR_PLUGIN_H

/**************************************************************************/
/*  navigation_obstacle_3d_editor_plugin.h                                */
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

#include "editor/editor_plugin.h"
#include "scene/3d/collision_polygon.h"
#include "scene/3d/mesh_instance.h"
#include "scene/gui/box_container.h"
#include "scene/resources/mesh/immediate_mesh.h"

#include "scene/3d/navigation_obstacle.h"

class CanvasItemEditor;
class MenuButton;

class NavigationObstacleEditor : public HBoxContainer {
	GDCLASS(NavigationObstacleEditor, HBoxContainer);

	enum Mode {
		MODE_CREATE,
		MODE_EDIT,
	};

	Mode mode;

	Button *button_create;
	Button *button_edit;

	Ref<SpatialMaterial> line_material;
	Ref<SpatialMaterial> handle_material;

	Panel *panel;
	NavigationObstacle *obstacle_node;
	Ref<ImmediateMesh> point_lines_mesh;
	MeshInstance *point_lines_meshinstance;
	MeshInstance *point_handles_meshinstance;
	Ref<ArrayMesh> point_handle_mesh;

	MenuButton *options;

	int edited_point;
	Vector2 edited_point_pos;
	PoolVector2Array pre_move_edit;
	PoolVector2Array wip;
	bool wip_active;
	bool snap_ignore;

	float prev_depth;

	void _wip_close();
	void _polygon_draw();
	void _menu_option(int p_option);

	PoolVector2Array _get_polygon();
	void _set_polygon(PoolVector2Array p_poly);

protected:
	void _notification(int p_what);
	void _node_removed(Node *p_node);
	static void _bind_methods();

public:
	virtual EditorPlugin::AfterGUIInput forward_3d_gui_input(Camera *p_camera, const Ref<InputEvent> &p_event);
	void edit(Node *p_node);

	NavigationObstacleEditor();
	~NavigationObstacleEditor();
};

class NavigationObstacleEditorPlugin : public EditorPlugin {
	GDCLASS(NavigationObstacleEditorPlugin, EditorPlugin);

	NavigationObstacleEditor *obstacle_editor = nullptr;

public:
	virtual EditorPlugin::AfterGUIInput forward_spatial_gui_input(Camera *p_camera, const Ref<InputEvent> &p_event) { return obstacle_editor->forward_3d_gui_input(p_camera, p_event); }

	virtual String get_name() const { return "NavigationObstacleEditor"; }
	bool has_main_screen() const { return false; }
	virtual void edit(Object *p_object);
	virtual bool handles(Object *p_object) const;
	virtual void make_visible(bool p_visible);

	NavigationObstacleEditorPlugin(EditorNode *p_node);
	~NavigationObstacleEditorPlugin();
};

#endif // NAVIGATION_OBSTACLE_3D_EDITOR_PLUGIN_H
