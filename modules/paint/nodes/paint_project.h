#ifndef PAINT_PROJECT_H
#define PAINT_PROJECT_H

#include "paint_node.h"

class PaintProject : public PaintNode {
	GDCLASS(PaintProject, PaintNode);

public:
	PaintProject();
	~PaintProject();

protected:
	static void _bind_methods();
};

#endif
