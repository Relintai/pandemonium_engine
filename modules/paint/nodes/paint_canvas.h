#ifndef PAINT_CANVAS_H
#define PAINT_CANVAS_H

#include "paint_node.h"

class PaintCanvas : public PaintNode {
	GDCLASS(PaintCanvas, PaintNode);

public:
	PaintCanvas();
	~PaintCanvas();

protected:
	static void _bind_methods();
};

#endif
