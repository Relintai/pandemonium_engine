#ifndef PAINT_PROJECT_H
#define PAINT_PROJECT_H

#include "core/containers/pool_vector.h"

#include "paint_node.h"

class PaintProject : public PaintNode {
	GDCLASS(PaintProject, PaintNode);

public:
	Color get_current_color();
	void set_current_color(const Color &color);

	PoolColorArray get_color_presets();
	void set_color_presets(const PoolColorArray &colors);

	void set_colors_as_default();

	PaintProject();
	~PaintProject();

protected:
	void _notification(int p_what);

	static void _bind_methods();

	Color _current_color;
	PoolColorArray _color_presets;
};

#endif
