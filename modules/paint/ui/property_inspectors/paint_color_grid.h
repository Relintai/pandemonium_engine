#ifndef PAINT_COLOR_GRID_H
#define PAINT_COLOR_GRID_H

/*
Copyright (c) 2019 Flairieve
Copyright (c) 2020-2022 cobrapitz
Copyright (c) 2022 Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "paint_custom_property_inspector.h"
#include "core/object/object_id.h"

class GridContainer;
class PaintNode;
class PaintProject;

class PaintColorGrid : public PaintCustomPropertyInspector {
	GDCLASS(PaintColorGrid, PaintCustomPropertyInspector);

public:
	// TODO this should store settings in PaintProjects
	// TODO the default settigns could be stored in the editor settings, so project specific pallettes can be done
	// TODO It should get the project node for a given PaintNode
	// NOTE The current color should be stored in PaintNodes

	void change_color_to(const Color &color);
	void add_color_prefab(const Color &color);

	void _on_paint_node_selected(Node *paint_node);

	PaintColorGrid();
	~PaintColorGrid();

protected:
	static void _bind_methods();

	GridContainer *_grid;

	ObjectID _current_paint_node;
	ObjectID _current_paint_project;
};

#endif
