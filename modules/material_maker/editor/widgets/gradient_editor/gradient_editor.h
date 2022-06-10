#ifndef GRADIENT_EDITOR_H
#define GRADIENT_EDITOR_H

#include "core/color.h"
#include "core/variant.h"

#include "scene/gui/control.h"

class GradientEditor : public Control {
	GDCLASS(GradientEditor, Control);

public:
	Variant get_Variant();
	void set_Variant(const Variant &val);

	Variant get_Variant();
	void set_Variant(const Variant &val);

	bool get_embedded() const;
	void set_embedded(const bool val);

	UndoRedo get_undo_redo();
	void set_undo_redo(const UndoRedo &val);

	PoolRealArray get__saved_points();
	void set__saved_points(const PoolRealArray &val);

	Variant get_Variant();
	void set_Variant(const Variant &val);

	void _init();
	void ignore_changes(const Variant &val);
	void save_color_state();
	void undo_redo_save_color_state();
	void set_undo_redo(const UndoRedo &ur);
	void set_value(const Variant &v);
	void update_cursors();
	void update_value();
	void add_cursor(const Variant &x, const Variant &color);
	void _gui_input(const Variant &ev);
	void select_color(const Variant &cursor, const Variant &position);
	Array get_sorted_cursors();
	void generate_preview_image();
	Color get_gradient_color(const Variant &x);
	void update_preview();
	void _on_Interpolation_item_selected(const Variant &ID);
	void on_resized();

	GradientEditor();
	~GradientEditor();

protected:
	static void _bind_methods();

	Variant = null;
	Variant = null;
	bool embedded = true;
	UndoRedo *_undo_redo = null;
	signal updated(value);
	PoolRealArray _saved_points = PoolRealArray();
	Variant;
};

#endif
