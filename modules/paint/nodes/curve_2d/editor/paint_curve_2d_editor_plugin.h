#ifndef PAINT_CURVE_2D_EDITOR_PLUGIN_H
#define PAINT_CURVE_2D_EDITOR_PLUGIN_H

/*************************************************************************/
/*  paint_curve_2d_editor_plugin.h                                       */
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
#include "scene/gui/box_container.h"

#include "core/math/vector2.h"
#include "core/object/object.h"
#include "core/object/reference.h"
#include "core/string/ustring.h"

class CanvasItemEditor;
class Control;
class EditorNode;
class InputEvent;
class MenuButton;
class Node;
class Panel;
class PaintCurve2D;
class Separator;
class ToolButton;
class UndoRedo;
class PaintCurve2D;

class PaintCurve2DEditor : public HBoxContainer {
	GDCLASS(PaintCurve2DEditor, HBoxContainer);

	UndoRedo *undo_redo;

	CanvasItemEditor *canvas_item_editor;
	EditorNode *editor;
	Panel *panel;
	PaintCurve2D *node;

	HBoxContainer *base_hb;
	Separator *sep;

	enum Mode {
		MODE_CREATE,
		MODE_EDIT,
		MODE_EDIT_CURVE,
		MODE_DELETE,
		ACTION_CLOSE
	};

	Mode mode;
	ToolButton *curve_create;
	ToolButton *curve_edit;
	ToolButton *curve_edit_curve;
	ToolButton *curve_del;
	ToolButton *curve_close;
	MenuButton *handle_menu;

	bool mirror_handle_angle;
	bool mirror_handle_length;
	bool on_edge;

	enum HandleOption {
		HANDLE_OPTION_ANGLE,
		HANDLE_OPTION_LENGTH
	};

	enum Action {

		ACTION_NONE,
		ACTION_MOVING_POINT,
		ACTION_MOVING_IN,
		ACTION_MOVING_OUT,
	};

	Action action;
	int action_point;
	Point2 moving_from;
	Point2 moving_screen_from;
	float orig_in_length;
	float orig_out_length;
	Vector2 edge_point;

	void _mode_selected(int p_mode);
	void _handle_option_pressed(int p_option);

	void _node_visibility_changed();
	friend class PaintCurve2DEditorPlugin;

protected:
	void _notification(int p_what);
	void _node_removed(Node *p_node);
	static void _bind_methods();

public:
	bool forward_gui_input(const Ref<InputEvent> &p_event);
	void forward_canvas_draw_over_viewport(Control *p_overlay);
	void edit(Node *p_path2d);
	PaintCurve2DEditor(EditorNode *p_editor);
};

class PaintCurve2DEditorPlugin : public EditorPlugin {
	GDCLASS(PaintCurve2DEditorPlugin, EditorPlugin);

	PaintCurve2DEditor *path2d_editor;
	EditorNode *editor;

public:
	virtual bool forward_canvas_gui_input(const Ref<InputEvent> &p_event) { return path2d_editor->forward_gui_input(p_event); }
	virtual void forward_canvas_draw_over_viewport(Control *p_overlay) { path2d_editor->forward_canvas_draw_over_viewport(p_overlay); }

	virtual String get_name() const { return "PaintCurve2D"; }
	bool has_main_screen() const { return false; }
	virtual void edit(Object *p_object);
	virtual bool handles(Object *p_object) const;
	virtual void make_visible(bool p_visible);

	PaintCurve2DEditorPlugin(EditorNode *p_node);
	~PaintCurve2DEditorPlugin();
};

#endif // PATH_2D_EDITOR_PLUGIN_H
