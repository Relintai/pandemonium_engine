#ifndef PAINT_PROJECT_H
#define PAINT_PROJECT_H

#include "paint_node.h"

class PaintProject : public PaintNode {
	GDCLASS(PaintProject, PaintNode);

public:
	Color get_current_color();
	void set_current_color(const Color &color);

	PaintProject();
	~PaintProject();

protected:
	static void _bind_methods();

	Color _current_color;
};

#endif
