#ifndef MM_GRADIENT_EDITOR_H
#define MM_GRADIENT_EDITOR_H

#include "core/color.h"
#include "core/os/input_event.h"
#include "core/reference.h"
#include "core/undo_redo.h"
#include "core/variant.h"
#include "core/vector.h"

#include "scene/gui/control.h"

class MMGraphNode;
class GradientBase;
class GradientCursor;
class TextureRect;
class Label;
class OptionButton;

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

	GradientCursor *get_active_cursor();
	void set_active_cursor(GradientCursor *val);

	void _init();

	void ignore_changes(const bool val);
	void save_color_state();

	void undo_redo_save_color_state();

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
	void on_resized();

	MMGradientEditor();
	~MMGradientEditor();

protected:
	static void _bind_methods();

	MMGraphNode *graph_node;
	Ref<GradientBase> value;
	bool embedded;
	UndoRedo *_undo_redo;
	PoolRealArray _saved_points;
	GradientCursor *active_cursor;
	TextureRect *gradient;
	OptionButton *interpolation;
	Label *cursor_value_label;
};

#endif
