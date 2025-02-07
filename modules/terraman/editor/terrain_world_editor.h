#ifndef TERRAIN_WORLD_EDITOR_PLUGIN_H
#define TERRAIN_WORLD_EDITOR_PLUGIN_H

/*************************************************************************/
/*  terrain_world_editor.h                                               */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "editor/editor_node.h"
#include "editor/editor_plugin.h"

#include "scene/gui/panel_container.h"

class TerrainWorldGizmo;
class TerrainWorld;
class SpatialEditorPlugin;
class HSlider;
class BoxContainer;
class HFlowContainer;
class SpinBox;
class TerrainWorldGizmoPlugin;

class TerrainWorldEditor : public PanelContainer {
	GDCLASS(TerrainWorldEditor, PanelContainer);

public:
	enum TerrainWorldEditorToolMode {
		TOOL_MODE_ADD = 0,
		TOOL_MODE_REMOVE,
		TOOL_MODE_ISOLEVEL_BRUSH,
		TOOL_MODE_PAINT_BRUSH,
		TOOL_MODE_PAINT_PICKER,
		TOOL_MODE_SPAWN_BRUSH,
		//TOOL_MODE_DELETE_BRUSH, //Sets data in channel to 0
		// TODO object manipulation tools.
	};

public:
	TerrainWorld *get_edited_world() { return _world; }

	EditorPlugin::AfterGUIInput forward_spatial_input_event(Camera *p_camera, const Ref<InputEvent> &p_event);

	void edit(TerrainWorld *p_world);
	EditorPlugin::AfterGUIInput do_add_remove_action(Camera *p_camera, const Point2 &p_point, bool p_click);
	bool get_draw_world_coordinate(Camera *p_camera, const Point2 &p_point, Vector3 &r_position, Vector3 &r_normal);
	void isolevel_brush_draw(const Vector3 &p_world_position);
	void paint_brush_draw(const Vector3 &p_world_position);

	TerrainWorldEditor();
	TerrainWorldEditor(EditorNode *p_editor);
	~TerrainWorldEditor();

protected:
	Ref<TerrainWorldGizmo> get_gizmo_from(TerrainWorld *w);

	// Used by UndoRedo
	void apply_data(const Array &p_data);
	void create_undo_point(const String &p_action, const int p_channel, const bool p_allow_create_chunks);
	String _current_action;
	HashMap<Vector2i, uint8_t> _original_data;
	HashMap<Vector2i, uint8_t> _current_data;

protected:
	static void _bind_methods();
	void _notification(int p_what);
	void _node_removed(Node *p_node);

	void _on_surface_button_pressed();
	void _on_tool_button_pressed();
	void _on_insert_block_at_camera_button_pressed();
	void _on_add_remove_isolevel_slider_value_changed(float value);
	void _on_isolevel_brush_tool_button_pressed();
	void _on_isolevel_brush_size_slider_changed(float value);
	void _on_isolevel_brush_strength_slider_changed(float value);
	void _on_isolevel_brush_smoothness_slider_changed(float value);
	void _on_isolevel_brush_channel_select_sb_changed(int value);
	void _on_isolevel_brush_allow_creating_chunks_selected();

	void _on_paint_brush_allow_creating_chunks_selected();
	void _on_paint_brush_size_slider_changed(float value);
	void _on_paint_brush_channel_select_sb_changed(int value);

private:
	UndoRedo *_undo_redo;
	bool _mouse_down;

private:
	//enum BrushType {
	//	BRUSH_TYPE_CIRCLE = 0,
	//	BRUSH_TYPE_SQUARE
	//};

	enum IsolevelBrushType {
		ISOLEVEL_BRUSH_TYPE_ADD = 0,
		ISOLEVEL_BRUSH_TYPE_SUBSTRACT,
		ISOLEVEL_BRUSH_TYPE_SET,
		//ISOLEVEL_BRUSH_TYPE_SMOOTH,
	};

	bool _isolevel_picker_mode;

	int _isolevel_brush_channel;
	float _isolevel_brush_size;
	float _isolevel_brush_strength;
	float _isolevel_brush_smoothness;
	//BrushType _brush_type;
	IsolevelBrushType _isolevel_brush_type;
	bool _isolevel_brush_allow_create_chunks;

private:
	bool _paint_brush_picker_mode;

	int _paint_brush_channel;
	float _paint_brush_size;
	bool _paint_brush_allow_create_chunks;

private:
	TerrainWorld *_world;
	Ref<TerrainWorldGizmo> _gizmo;

	int _selected_type;

	SpatialEditorPlugin *spatial_editor;
	EditorNode *_editor;

	int _channel_type;
	int _channel_isolevel;

	TerrainWorldEditorToolMode _tool_mode;
	TerrainWorldEditorToolMode _previous_tool_mode;

	ToolButton *_add_button;
	ToolButton *_remove_button;
	ToolButton *_isolevel_brush_button;
	ToolButton *_paint_brush_button;
	ToolButton *_paint_picker_button;
	ToolButton *_spawn_brush_button;

	HFlowContainer *_tool_button_container;
	Ref<ButtonGroup> _tool_button_group;

	VBoxContainer *_add_remove_tool_container;
	Label *_add_single_label;
	Label *_remove_single_label;
	Label *_add_remove_isolevel_slider_label;
	HSlider *_add_remove_isolevel_slider;
	int _add_remove_current_isolevel;
	ToolButton *_insert_button;

	VBoxContainer *_isolevel_brush_tool_container;
	Ref<ButtonGroup> _isolevel_brush_tool_button_group;
	ToolButton *_isolevel_brush_type_add_button;
	ToolButton *_isolevel_brush_type_substract_button;
	ToolButton *_isolevel_brush_type_set_button;
	//ToolButton *_isolevel_brush_type_smooth_button;
	ToolButton *_isolevel_brush_allow_creating_chunks_button;
	HSlider *_isolevel_brush_size_slider;
	HSlider *_isolevel_brush_strength_slider;
	HSlider *_isolevel_brush_smoothness_slider;
	SpinBox *_isolevel_brush_channel_select_sb;

	VBoxContainer *_paint_brush_tool_container;
	ToolButton *_paint_brush_allow_creating_chunks_button;
	HSlider *_paint_brush_size_slider;
	SpinBox *_paint_brush_channel_select_sb;

	VBoxContainer *_paint_picker_tool_container;

	VBoxContainer *_surfaces_vbox_container;
	Ref<ButtonGroup> _surfaces_button_group;
};

class TerrainWorldEditorPlugin : public EditorPlugin {
	GDCLASS(TerrainWorldEditorPlugin, EditorPlugin);

	TerrainWorldEditor *terrain_world_editor;
	EditorNode *editor;
	Ref<TerrainWorldGizmoPlugin> gizmo_plugin;

protected:
	void _notification(int p_what);

public:
	bool forward_spatial_input_event(Camera *p_camera, const Ref<InputEvent> &p_event) { return terrain_world_editor->forward_spatial_input_event(p_camera, p_event); }
	virtual EditorPlugin::AfterGUIInput forward_spatial_gui_input(Camera *p_camera, const Ref<InputEvent> &p_event) { return terrain_world_editor->forward_spatial_input_event(p_camera, p_event); }
	virtual String get_name() const { return "TerrainWorldEditor"; }
	bool has_main_screen() const { return false; }
	virtual void edit(Object *p_object);
	virtual bool handles(Object *p_object) const;
	virtual void make_visible(bool p_visible);

	TerrainWorldEditorPlugin(EditorNode *p_node);
	~TerrainWorldEditorPlugin();
};

#endif
