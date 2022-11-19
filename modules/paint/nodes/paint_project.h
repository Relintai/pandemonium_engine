#ifndef PAINT_PROJECT_H
#define PAINT_PROJECT_H

#include "core/containers/pool_vector.h"

#include "paint_node.h"

class PaintProject : public PaintNode {
	GDCLASS(PaintProject, PaintNode);

public:
	String get_save_file_name();
	void set_save_file_name(const String &fn);

	Color get_current_color();
	void set_current_color(const Color &color);

	void add_preset_color(const Color &color);
	void remove_preset_color(const int index);
	Color get_preset_color(const int index);
	void set_preset_color(const int index, const Color &color);
	int get_preset_color_count();

	PoolColorArray get_color_presets();
	void set_color_presets(const PoolColorArray &colors);

	void set_colors_as_default();

	void _save_image();

	void add_paint_canvas_backgorund();
	void add_paint_visual_grid();

	PaintProject();
	~PaintProject();

protected:
	void _notification(int p_what);

	static void _bind_methods();

	String _save_file_name;
	Color _current_color;
	PoolColorArray _color_presets;
};

#endif
