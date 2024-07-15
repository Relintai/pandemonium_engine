#ifndef MESH_INSTANCE_EDITOR_PLUGIN_H
#define MESH_INSTANCE_EDITOR_PLUGIN_H

/*************************************************************************/
/*  mesh_instance_editor_plugin.h                                        */
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

#include "editor/editor_plugin.h"
#include "scene/main/control.h"

#include "core/containers/vector.h"
#include "core/math/vector2.h"
#include "core/object/object.h"
#include "core/string/ustring.h"

class AcceptDialog;
class ConfirmationDialog;
class EditorNode;
class MenuButton;
class MeshInstance;
class Node;
class SpinBox;
class HBoxContainer;

class MeshInstanceEditor : public Control {
	GDCLASS(MeshInstanceEditor, Control);

	enum Menu {

		MENU_OPTION_CREATE_STATIC_TRIMESH_BODY,
		MENU_OPTION_CREATE_TRIMESH_COLLISION_SHAPE,
		MENU_OPTION_CREATE_SINGLE_CONVEX_COLLISION_SHAPE,
		MENU_OPTION_CREATE_SIMPLIFIED_CONVEX_COLLISION_SHAPE,
		MENU_OPTION_CREATE_MULTIPLE_CONVEX_COLLISION_SHAPES,
		MENU_OPTION_CREATE_NAVMESH,
		MENU_OPTION_CREATE_OUTLINE_MESH,
		MENU_OPTION_CREATE_UV2,
		MENU_OPTION_DEBUG_UV1,
		MENU_OPTION_DEBUG_UV2,
	};

	MeshInstance *node;

	HBoxContainer *options_container;
	MenuButton *options;

	ConfirmationDialog *outline_dialog;
	SpinBox *outline_size;

	AcceptDialog *err_dialog;

	AcceptDialog *debug_uv_dialog;
	Control *debug_uv;
	Vector<Vector2> uv_lines;

	void _menu_option(int p_option);
	void _create_outline_mesh();

	void _create_uv_lines(int p_layer);
	friend class MeshInstanceEditorPlugin;

	void _debug_uv_draw();

protected:
	void _node_removed(Node *p_node);
	static void _bind_methods();

public:
	void edit(MeshInstance *p_mesh);
	MeshInstanceEditor();
};

class MeshInstanceEditorPlugin : public EditorPlugin {
	GDCLASS(MeshInstanceEditorPlugin, EditorPlugin);

	MeshInstanceEditor *mesh_editor;
	EditorNode *editor;

public:
	virtual String get_name() const { return "MeshInstance"; }
	bool has_main_screen() const { return false; }
	virtual void edit(Object *p_object);
	virtual bool handles(Object *p_object) const;
	virtual void make_visible(bool p_visible);

	MeshInstanceEditorPlugin(EditorNode *p_node);
	~MeshInstanceEditorPlugin();
};

#endif // MESH_EDITOR_PLUGIN_H
