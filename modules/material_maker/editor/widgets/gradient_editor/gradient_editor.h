#ifndef MM_GRADIENT_EDITOR_H
#define MM_GRADIENT_EDITOR_H

/*************************************************************************/
/*  gradient_editor.h                                                    */
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

#include "core/containers/vector.h"
#include "core/input/input_event.h"
#include "core/math/color.h"
#include "core/object/reference.h"
#include "core/object/undo_redo.h"
#include "core/variant/variant.h"

#include "scene/main/control.h"

class MMGraphNode;
class GradientBase;
class GradientCursor;
class TextureRect;
class Label;
class OptionButton;
class MMMaterial;

class MMGradientEditor : public Control {
	GDCLASS(MMGradientEditor, Control);

public:
	MMGraphNode *get_graph_node();
	void set_graph_node(MMGraphNode *val);

	Ref<GradientBase> get_value();
	void set_value(const Ref<GradientBase> &val);

	bool get_embedded() const;
	void set_embedded(const bool val);

	UndoRedo *get_undo_redo();
	void set_undo_redo(UndoRedo *val);

	PoolRealArray get_saved_points();
	void set_saved_points(const PoolRealArray &val);

	void ignore_changes(const bool val);

	void save_color_state();
	void undo_redo_save_color_state();

	void cursor_move_started(GradientCursor *c);
	void cursor_move_ended();
	void cursor_delete_request(GradientCursor *c);
	void cursor_doubleclicked(GradientCursor *c, const Vector2 &position);

	void update_cursors();
	void update_value();

	void add_cursor(const float x, const Color &color);
	void _gui_input(const Ref<InputEvent> &ev);

	void select_color(GradientCursor *cursor, const Vector2 &position);

	Vector<GradientCursor *> get_sorted_cursors();
	void generate_preview_image();

	Color get_gradient_color(const float x);
	void update_preview();

	void _on_Interpolation_item_selected(const int ID);

	void apply_new_interpolation();
	void on_resized();
	void on_color_selector_closed();
	void on_value_changed();

	MMGradientEditor();
	~MMGradientEditor();

protected:
	void _notification(int p_what);

	static void _bind_methods();

	MMGraphNode *graph_node;
	Ref<GradientBase> value;
	Ref<MMMaterial> _material;
	bool embedded;
	UndoRedo *_undo_redo;
	PoolRealArray _saved_points;
	GradientCursor *active_cursor;
	TextureRect *gradient;
	OptionButton *interpolation;
	Label *cursor_value_label;

	bool _update_preview_queued;
	bool _update_cursors_queued;
	//bool _value_update_queued;

	bool _cursor_moving;
	bool _cursor_started_moving;
	bool _cursor_stopped_moving;
	GradientCursor *_cursor_doubleclicked;
	Vector2 _cursor_doubleclick_position;
	GradientCursor *_cursor_delete_request;
	bool _cursor_add_queued;
	float _cursor_add_x;
	bool _selecting_color;
	bool _color_selection_done;

	bool _interpolation_change_queued;
	int _new_interpolation;
};

#endif
