/*
Copyright (c) 2019-2022 Péter Magyar

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

#include "register_types.h"

#include "actions/paint_action.h"

#include "actions/brighten_action.h"
#include "actions/brush_action.h"
#include "actions/bucket_action.h"
#include "actions/cut_action.h"
#include "actions/darken_action.h"
#include "actions/line_action.h"
#include "actions/multiline_action.h"
#include "actions/paste_cut_action.h"
#include "actions/pencil_action.h"
#include "actions/rainbow_action.h"
#include "actions/rect_action.h"

#include "deprecated/paint_window.h"

#include "ui/paint_canvas_background.h"
#include "ui/paint_canvas_outline.h"
#include "ui/paint_color_grid.h"
#include "ui/paint_selection_box.h"
#include "ui/paint_visual_grid.h"

#include "nodes/paint_node.h"
#include "nodes/paint_canvas.h"

#ifdef TOOLS_ENABLED
#include "paint_editor_plugin.h"
#endif

void register_paint_types() {
	ClassDB::register_class<PaintWindow>();

	ClassDB::register_class<PaintAction>();

	ClassDB::register_class<BrightenAction>();
	ClassDB::register_class<BrushAction>();
	ClassDB::register_class<BucketAction>();
	ClassDB::register_class<CutAction>();
	ClassDB::register_class<DarkenAction>();
	ClassDB::register_class<LineAction>();
	ClassDB::register_class<MultiLineAction>();
	ClassDB::register_class<PasteCutAction>();
	ClassDB::register_class<PencilAction>();
	ClassDB::register_class<RainbowAction>();
	ClassDB::register_class<RectAction>();

	ClassDB::register_class<PaintCanvasBackground>();
	ClassDB::register_class<PaintCanvasOutline>();
	ClassDB::register_class<PaintColorGrid>();
	ClassDB::register_class<PaintSelectionBox>();
	ClassDB::register_class<PaintVisualGrid>();

	ClassDB::register_class<PaintNode>();
	ClassDB::register_class<PaintCanvas>();

#ifdef TOOLS_ENABLED
	EditorPlugins::add_by_type<PaintEditorPlugin>();
#endif
}

void unregister_paint_types() {
}
