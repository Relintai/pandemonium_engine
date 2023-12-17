

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

#include "ui/paint_canvas_background.h"
#include "ui/paint_visual_grid.h"

#include "ui/property_inspectors/paint_custom_property_inspector.h"
#include "ui/property_inspectors/paint_project_property_inspector.h"
#include "ui/property_inspectors/paint_project_tools_property_inspector.h"
#include "ui/property_inspectors/paint_tools_property_inspector.h"

#include "nodes/paint_canvas.h"
#include "nodes/paint_node.h"
#include "nodes/paint_project.h"

#include "nodes/curve_2d/paint_curve_2d.h"
#include "nodes/polygon_2d/paint_polygon_2d.h"

#ifdef TOOLS_ENABLED
#include "editor/paint_editor_plugin.h"
#include "nodes/curve_2d/editor/paint_curve_2d_editor_plugin.h"
#include "nodes/polygon_2d/editor/paint_polygon_2d_editor_plugin.h"
#endif

void register_paint_types(ModuleRegistrationLevel p_level) {
	if (p_level == MODULE_REGISTRATION_LEVEL_SCENE) {
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
		ClassDB::register_class<PaintVisualGrid>();

		ClassDB::register_class<PaintCustomPropertyInspector>();
		ClassDB::register_class<PaintToolsPropertyInspector>();
		ClassDB::register_class<PaintProjectPropertyInspector>();
		ClassDB::register_class<PaintProjectToolsPropertyInspector>();

		ClassDB::register_class<PaintNode>();
		ClassDB::register_class<PaintCanvas>();
		ClassDB::register_class<PaintProject>();
		ClassDB::register_class<PaintCurve2D>();
		ClassDB::register_class<PaintPolygon2D>();
	}

#ifdef TOOLS_ENABLED
	if (p_level == MODULE_REGISTRATION_LEVEL_EDITOR) {
		EditorPlugins::add_by_type<PaintEditorPlugin>();

		EditorPlugins::add_by_type<PaintPolygon2DEditorPlugin>();
		EditorPlugins::add_by_type<PaintCurve2DEditorPlugin>();
	}
#endif
}

void unregister_paint_types(ModuleRegistrationLevel p_level) {
}
