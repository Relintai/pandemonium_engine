#ifndef PAINT_PROJECT_TOOLS_PROPERTY_INSPECTOR_H
#define PAINT_PROJECT_TOOLS_PROPERTY_INSPECTOR_H



#include "core/object/object_id.h"
#include "core/object/reference.h"
#include "paint_custom_property_inspector.h"

class GridContainer;
class PaintNode;
class PaintProject;
class HFlowContainer;
class ColorSelectorButton;
class Button;
class FileDialog;

class PaintProjectToolsPropertyInspector : public PaintCustomPropertyInspector {
	GDCLASS(PaintProjectToolsPropertyInspector, PaintCustomPropertyInspector);

public:
	void add_action_button(const String &callback, const String &hint, const String &icon, const String &theme_type);

	void _set_paint_node(Node *paint_node);

	PaintProjectToolsPropertyInspector();
	~PaintProjectToolsPropertyInspector();

protected:
	void _on_export_pressed();
	void _on_export_as_pressed();
	void _on_export_as_dialog_file_selected(const String &f);
	void _on_set_colors_as_default_pressed();
	void _on_add_paint_visual_grid_pressed();
	void _on_add_paint_canvas_background_pressed();
	void _on_add_paint_canvas_pressed();
	void _on_add_paint_curve_2d_pressed();
	void _on_add_paint_polygon_2d_pressed();

	//void _notification(int p_what);

	static void _bind_methods();

	FileDialog *_export_as_file_dialog;

	HFlowContainer *_button_contianer;

	ObjectID _current_paint_node;
	ObjectID _current_paint_project;

	bool _ignore_preset_changed_event;
	bool _ignore_color_event;
};

#endif
