#ifndef PAINT_POLYGON_2D_EDITOR_PLUGIN_H
#define PAINT_POLYGON_2D_EDITOR_PLUGIN_H

/*************************************************************************/
/*  paint_polygon_2d_editor_plugin.h                                     */
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

#include "editor/plugins/abstract_polygon_2d_editor.h"

#include "core/containers/pool_vector.h"
#include "core/containers/vector.h"
#include "core/math/color.h"
#include "core/math/vector2.h"
#include "core/object/object.h"
#include "core/object/reference.h"
#include "core/variant/array.h"

class AcceptDialog;
class Button;
class EditorNode;
class HScrollBar;
class HSlider;
class InputEvent;
class Label;
class MenuButton;
class Node2D;
class Node;
class Panel;
class PaintPolygon2D;
class ScrollContainer;
class SpinBox;
class TextureRect;
class ToolButton;
class VBoxContainer;
class VScrollBar;
class PaintPolygon2D;

class PaintPolygon2DEditor : public AbstractPolygon2DEditor {
	GDCLASS(PaintPolygon2DEditor, AbstractPolygon2DEditor);

	enum Mode {
		MODE_EDIT_UV = MODE_CONT,
		UVEDIT_POLYGON_TO_UV,
		UVEDIT_UV_TO_POLYGON,
		UVEDIT_UV_CLEAR,
		UVEDIT_GRID_SETTINGS
	};

	enum UVMode {
		UV_MODE_CREATE,
		UV_MODE_CREATE_INTERNAL,
		UV_MODE_REMOVE_INTERNAL,
		UV_MODE_EDIT_POINT,
		UV_MODE_MOVE,
		UV_MODE_ROTATE,
		UV_MODE_SCALE,
		UV_MODE_ADD_POLYGON,
		UV_MODE_REMOVE_POLYGON,
		UV_MODE_MAX
	};

	ToolButton *uv_edit_mode[3];
	Ref<ButtonGroup> uv_edit_group;

	PaintPolygon2D *node;

	UVMode uv_mode;
	AcceptDialog *uv_edit;
	ToolButton *uv_button[UV_MODE_MAX];
	ToolButton *b_snap_enable;
	ToolButton *b_snap_grid;
	Panel *uv_edit_draw;
	HSlider *uv_zoom;
	SpinBox *uv_zoom_value;
	HScrollBar *uv_hscroll;
	VScrollBar *uv_vscroll;
	MenuButton *uv_menu;
	TextureRect *uv_icon_zoom;

	AcceptDialog *grid_settings;

	Vector2 uv_draw_ofs;
	float uv_draw_zoom;
	PoolVector<Vector2> points_prev;
	PoolVector<Vector2> uv_create_uv_prev;
	PoolVector<Vector2> uv_create_poly_prev;
	PoolVector<Color> uv_create_colors_prev;
	int uv_create_prev_internal_vertices;
	Array polygons_prev;

	Vector2 uv_create_to;
	int point_drag_index;
	bool uv_drag;
	bool uv_create;
	Vector<int> polygon_create;
	UVMode uv_move_current;
	Vector2 uv_drag_from;
	bool updating_uv_scroll;

	AcceptDialog *error;

	ToolButton *button_uv;

	bool use_snap;
	bool snap_show_grid;
	Vector2 snap_offset;
	Vector2 snap_step;

	virtual void _menu_option(int p_option);

	void _cancel_editing();
	void _update_polygon_editing_state();

	void _uv_scroll_changed(float);
	void _uv_input(const Ref<InputEvent> &p_input);
	void _uv_draw();
	void _uv_mode(int p_mode);

	void _set_use_snap(bool p_use);
	void _set_show_grid(bool p_show);
	void _set_snap_off_x(float p_val);
	void _set_snap_off_y(float p_val);
	void _set_snap_step_x(float p_val);
	void _set_snap_step_y(float p_val);

	void _uv_edit_mode_select(int p_mode);
	void _uv_edit_popup_hide();

	int _get_polygon_count() const;

protected:
	virtual Node2D *_get_node() const;
	virtual void _set_node(Node *p_polygon);

	virtual Vector2 _get_offset(int p_idx) const;

	virtual bool _has_uv() const { return true; };
	virtual void _commit_action();

	void _notification(int p_what);
	static void _bind_methods();

	Vector2 snap_point(Vector2 p_target) const;

public:
	PaintPolygon2DEditor(EditorNode *p_editor);
};

class PaintPolygon2DEditorPlugin : public AbstractPolygon2DEditorPlugin {
	GDCLASS(PaintPolygon2DEditorPlugin, AbstractPolygon2DEditorPlugin);

public:
	PaintPolygon2DEditorPlugin(EditorNode *p_node);
};

#endif // POLYGON_2D_EDITOR_PLUGIN_H
