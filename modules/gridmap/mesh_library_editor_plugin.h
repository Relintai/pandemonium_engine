#ifndef MESH_LIBRARY_EDITOR_PLUGIN_H
#define MESH_LIBRARY_EDITOR_PLUGIN_H

/*************************************************************************/
/*  mesh_library_editor_plugin.h                                         */
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

class MeshLibrary;

class MeshLibraryEditor : public Control {
	GDCLASS(MeshLibraryEditor, Control);

	Ref<MeshLibrary> mesh_library;

	EditorNode *editor;
	MenuButton *menu;
	ConfirmationDialog *cd_remove;
	ConfirmationDialog *cd_update;
	EditorFileDialog *file;
	bool apply_xforms;
	int to_erase;

	enum {

		MENU_OPTION_ADD_ITEM,
		MENU_OPTION_REMOVE_ITEM,
		MENU_OPTION_UPDATE_FROM_SCENE,
		MENU_OPTION_IMPORT_FROM_SCENE,
		MENU_OPTION_IMPORT_FROM_SCENE_APPLY_XFORMS
	};

	int option;
	void _import_scene_cbk(const String &p_str);
	void _menu_cbk(int p_option);
	void _menu_remove_confirm();
	void _menu_update_confirm(bool p_apply_xforms);

	static void _import_scene(Node *p_scene, Ref<MeshLibrary> p_library, bool p_merge, bool p_apply_xforms);

protected:
	static void _bind_methods();

public:
	MenuButton *get_menu_button() const { return menu; }

	void edit(const Ref<MeshLibrary> &p_mesh_library);
	static Error update_library_file(Node *p_base_scene, Ref<MeshLibrary> ml, bool p_merge = true, bool p_apply_xforms = false);

	MeshLibraryEditor(EditorNode *p_editor);
};

class MeshLibraryEditorPlugin : public EditorPlugin {
	GDCLASS(MeshLibraryEditorPlugin, EditorPlugin);

	MeshLibraryEditor *mesh_library_editor;
	EditorNode *editor;

	EditorFileDialog *file_export_lib;
	CheckBox *file_export_lib_merge;
	CheckBox *file_export_lib_apply_xforms;

public:
	virtual String get_name() const { return "MeshLibrary"; }
	bool has_main_screen() const { return false; }
	virtual void edit(Object *p_node);
	virtual bool handles(Object *p_node) const;
	virtual void make_visible(bool p_visible);

	MeshLibraryEditorPlugin(EditorNode *p_node);

protected:
	static void _bind_methods();

	void _convert_scene_to_mesh_library(Variant p_null);
	void _dialog_action(String p_file);
};

#endif // MESH_LIBRARY_EDITOR_PLUGIN_H
