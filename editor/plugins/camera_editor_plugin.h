#ifndef CAMERA_EDITOR_PLUGIN_H
#define CAMERA_EDITOR_PLUGIN_H

/*************************************************************************/
/*  camera_editor_plugin.h                                               */
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

#include "core/object/object.h"
#include "core/string/ustring.h"

class Button;
class EditorNode;
class Node;
class Panel;

class CameraEditor : public Control {
	GDCLASS(CameraEditor, Control);

	Panel *panel;
	Button *preview;
	Node *node;

	void _pressed();

protected:
	void _node_removed(Node *p_node);
	static void _bind_methods();

public:
	void edit(Node *p_camera);
	CameraEditor();
};

class CameraEditorPlugin : public EditorPlugin {
	GDCLASS(CameraEditorPlugin, EditorPlugin);

	//CameraEditor *camera_editor;
	EditorNode *editor;

public:
	virtual String get_name() const { return "Camera"; }
	bool has_main_screen() const { return false; }
	virtual void edit(Object *p_object);
	virtual bool handles(Object *p_object) const;
	virtual void make_visible(bool p_visible);

	CameraEditorPlugin(EditorNode *p_node);
	~CameraEditorPlugin();
};

#endif // CAMERA_EDITOR_PLUGIN_H
