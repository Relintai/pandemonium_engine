#ifndef PAINT_TOOLS_PROPERTY_INSPECTOR_H
#define PAINT_TOOLS_PROPERTY_INSPECTOR_H



#include "core/object/object_id.h"
#include "core/object/reference.h"
#include "paint_custom_property_inspector.h"

class HFlowContainer;
class PaintNode;
class PaintCanvas;
class PaintProject;
class ButtonGroup;
class HSlider;
class Label;
class FileDialog;
class ConfirmationDialog;
class SpinBox;
class OptionButton;

class PaintToolsPropertyInspector : public PaintCustomPropertyInspector {
	GDCLASS(PaintToolsPropertyInspector, PaintCustomPropertyInspector);

public:
	void add_tool_button(int id, const String &hint, const String &icon, const String &theme_type);
	void add_action_button(const String &callback, const String &hint, const String &icon, const String &theme_type);
	void add_brush_prefab(int id, const Ref<Texture> &normal_texture, const Ref<Texture> &hover_texture);

	void _set_paint_node(Node *paint_node);

	PaintToolsPropertyInspector();
	~PaintToolsPropertyInspector();

protected:
	void _on_button_toggled(bool on, int id);
	void _on_tool_changed();
	void _on_brush_prefab_button_pressed(const int id);
	void _on_brush_size_slider_value_changed(const float value);
	void _on_brush_size_changed();
	void _on_undo_pressed();
	void _on_redo_pressed();
	void _on_import_pressed();
	void _on_export_pressed();
	void _on_import_dialog_file_selected(const String &f);
	void _on_export_dialog_file_selected(const String &f);
	void _on_pixel_scale_canvas_pressed();
	void _on_pixel_scale_canvas_confirmed();

	static void _bind_methods();

	FileDialog *_import_file_dialog;
	FileDialog *_export_file_dialog;
	ConfirmationDialog *_pixel_resize_canvas;
	SpinBox *_pixel_resize_x_spinbox;
	SpinBox *_pixel_resize_y_spinbox;
	OptionButton *_pixel_resize_interpolation_option_button;

	HFlowContainer *_grid;
	HFlowContainer *_brush_prefabs;
	HSlider *_brush_size_slider;
	Label *_brush_size_label;

	ObjectID _paint_canvas;

	Ref<ButtonGroup> _group;

	bool _ignore_signal;
};

#endif
