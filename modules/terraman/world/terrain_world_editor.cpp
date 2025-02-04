/*************************************************************************/
/*  terrain_world_editor.cpp                                             */
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

#include "terrain_world_editor.h"

#include "editor/editor_plugin.h"
#include "editor/editor_scale.h"
#include "editor/editor_settings.h"

#include "scene/gui/box_container.h"
#include "scene/gui/flow_container.h"
#include "scene/main/control.h"
#include "terrain_world.h"

#include "core/os/keyboard.h"

#include "terrain_chunk.h"

#include "../library/terrain_library.h"
#include "../library/terrain_surface.h"
#include "scene/resources/world_3d.h"
#include "servers/physics_server.h"

#include "editor/plugins/spatial_editor_plugin.h"
#include "scene/3d/camera.h"
#include "scene/gui/box_container.h"
#include "scene/gui/flow_container.h"
#include "scene/gui/label.h"
#include "scene/gui/scroll_container.h"
#include "scene/gui/separator.h"
#include "scene/gui/slider.h"
#include "scene/gui/spin_box.h"

EditorPlugin::AfterGUIInput TerrainWorldEditor::forward_spatial_input_event(Camera *p_camera, const Ref<InputEvent> &p_event) {
	if (!_world || !_world->get_editable()) {
		return EditorPlugin::AFTER_GUI_INPUT_PASS;
	}

	Ref<TerrainLibrary> lib = _world->get_library();

	if (!lib.is_valid()) {
		return EditorPlugin::AFTER_GUI_INPUT_PASS;
	}

	Ref<InputEventMouseButton> mb = p_event;

	if (mb.is_valid()) {
		if (mb->is_pressed()) {
			if (mb->get_button_index() == BUTTON_LEFT) {
				_mouse_down = true;

				switch (_tool_mode) {
					case TOOL_MODE_ADD:
					case TOOL_MODE_REMOVE:
						return do_add_remove_action(p_camera, Point2(mb->get_position().x, mb->get_position().y), true);
					case TOOL_MODE_ISOLEVEL_BRUSH:
						break;
					case TOOL_MODE_PAINT_BRUSH:
						break;
					case TOOL_MODE_PAINT_PICKER:
						break;
				}

				return EditorPlugin::AFTER_GUI_INPUT_STOP;
			} else {
				return EditorPlugin::AFTER_GUI_INPUT_PASS;
			}
		} else {
			if (mb->get_button_index() == BUTTON_LEFT) {
				_mouse_down = false;
				return EditorPlugin::AFTER_GUI_INPUT_STOP;
			}
		}
	}

	if (_mouse_down) {
		return EditorPlugin::AFTER_GUI_INPUT_STOP;
	}

	return EditorPlugin::AFTER_GUI_INPUT_PASS;
}

EditorPlugin::AfterGUIInput TerrainWorldEditor::do_add_remove_action(Camera *p_camera, const Point2 &p_point, bool p_click) {
	Camera *camera = p_camera;
	Vector3 from = camera->project_ray_origin(p_point);
	Vector3 to = from + camera->project_ray_normal(p_point) * 10000;
	Transform local_xform = _world->get_global_transform().affine_inverse();

	from = local_xform.xform(from);
	to = local_xform.xform(to);

	PhysicsDirectSpaceState *ss = _world->get_world_3d()->get_direct_space_state();

	PhysicsDirectSpaceState::RayResult res;

	if (ss->intersect_ray(from, to, res)) {
		int selected_terrain = 0;
		int channel = 0;

		channel = _channel_type;

		if (channel == -1) {
			return EditorPlugin::AFTER_GUI_INPUT_PASS;
		}

		int isolevel = _add_remove_current_isolevel;
		bool mode_add = false;

		if (_tool_mode == TOOL_MODE_ADD) {
			selected_terrain = _selected_type + 1;
			mode_add = true;
			_undo_redo->create_action("Add terrain voxel.");
		} else if (_tool_mode == TOOL_MODE_REMOVE) {
			selected_terrain = 0;
			isolevel = 0;
			mode_add = false;
			_undo_redo->create_action("Remove terrain voxel.");
		}

		Vector3 pos;

		if (mode_add) {
			pos = (res.position + (Vector3(0.1, 0.1, 0.1) * res.normal * _world->get_voxel_scale()));
		} else {
			pos = (res.position + (Vector3(0.1, 0.1, 0.1) * -res.normal * _world->get_voxel_scale()));
		}

		uint8_t prev_terrain = _world->get_voxel_at_world_position(pos, _channel_type);
		uint8_t prev_isolevel = 0;

		if (_channel_isolevel != -1) {
			prev_isolevel = _world->get_voxel_at_world_position(pos, _channel_isolevel);
		}

		if (_channel_isolevel == -1) {
			_undo_redo->add_do_method(_world, "set_voxel_at_world_position", pos, selected_terrain, _channel_type);
			_undo_redo->add_undo_method(_world, "set_voxel_at_world_position", pos, prev_terrain, _channel_type);
		} else {
			_undo_redo->add_do_method(_world, "set_voxel_at_world_position", pos, selected_terrain, _channel_type, false);
			_undo_redo->add_do_method(_world, "set_voxel_at_world_position", pos, isolevel, _channel_isolevel);
			_undo_redo->add_undo_method(_world, "set_voxel_at_world_position", pos, prev_terrain, _channel_type, false);
			_undo_redo->add_undo_method(_world, "set_voxel_at_world_position", pos, prev_isolevel, _channel_isolevel);
		}

		_undo_redo->commit_action();

		return EditorPlugin::AFTER_GUI_INPUT_STOP;
	}

	return EditorPlugin::AFTER_GUI_INPUT_PASS;
}

void TerrainWorldEditor::edit(TerrainWorld *p_world) {
	if (_world == p_world) {
		return;
	}

	_world = p_world;

	if (!_world) {
		return;
	}

	_channel_type = _world->get_channel_index_info(TerrainWorld::CHANNEL_TYPE_INFO_TYPE);
	_channel_isolevel = _world->get_channel_index_info(TerrainWorld::CHANNEL_TYPE_INFO_ISOLEVEL);

	if (_channel_isolevel == -1) {
		_add_remove_isolevel_slider_label->hide();
		_add_remove_isolevel_slider->hide();
	} else {
		_add_remove_isolevel_slider_label->show();
		_add_remove_isolevel_slider->show();
	}

	_isolevel_brush_channel_select_sb->set_value(_channel_isolevel);
	_isolevel_brush_channel = _channel_isolevel;

	spatial_editor = Object::cast_to<SpatialEditorPlugin>(_editor->get_editor_plugin_screen());

	for (int i = 0; i < _surfaces_vbox_container->get_child_count(); ++i) {
		Node *child = _surfaces_vbox_container->get_child(i);

		if (!child->is_queued_for_deletion()) {
			child->queue_delete();
		}
	}

	Ref<TerrainLibrary> library = _world->get_library();

	if (!library.is_valid()) {
		return;
	}

	if (!library->get_initialized()) {
		library->refresh_rects();
	}

	bool f = false;
	for (int i = 0; i < library->terra_surface_get_num(); ++i) {
		Ref<TerrainSurface> surface = library->terra_surface_get(i);

		if (!surface.is_valid()) {
			continue;
		}

		String text = String::num(i) + " - " + surface->get_name();

		Button *button = memnew(Button);
		button->set_text(text);
		button->set_text_align(Button::ALIGN_LEFT);
		button->set_meta("index", i);
		button->set_toggle_mode(true);
		button->set_button_group(_surfaces_button_group);
		button->set_h_size_flags(SIZE_EXPAND_FILL);

		button->connect("button_up", this, "_on_surface_button_pressed");

		_surfaces_vbox_container->add_child(button);

		if (!f) {
			button->set_pressed(true);
			f = true;
		}
	}
}

TerrainWorldEditor::TerrainWorldEditor() {
	_undo_redo = NULL;
	_world = NULL;
	_selected_type = 0;
	_channel_type = -1;
	_add_remove_current_isolevel = 255;
	_channel_isolevel = -1;
	_editor = NULL;
	_tool_mode = TOOL_MODE_ADD;

	_mouse_down = false;

	_isolevel_picker_mode = false;

	_isolevel_brush_channel = -1;
	_brush_allow_create_chunks = true;
	_isolevel_brush_size = 10;
	_isolevel_brush_smoothness = 10;
	//_brush_type = BRUSH_TYPE_CIRCLE;
	_isolevel_brush_type = ISOLEVEL_BRUSH_TYPE_ADD;
}
TerrainWorldEditor::TerrainWorldEditor(EditorNode *p_editor) {
	_undo_redo = EditorNode::get_undo_redo();

	_world = NULL;
	_selected_type = 0;
	_channel_type = -1;
	_add_remove_current_isolevel = 255;
	_channel_isolevel = -1;

	_mouse_down = false;

	_isolevel_picker_mode = false;

	_isolevel_brush_channel = -1;
	_brush_allow_create_chunks = true;
	_isolevel_brush_size = 10;
	_isolevel_brush_smoothness = 10;
	//_brush_type = BRUSH_TYPE_CIRCLE;
	_isolevel_brush_type = ISOLEVEL_BRUSH_TYPE_ADD;

	_editor = p_editor;
	_tool_mode = TOOL_MODE_ADD;

	set_custom_minimum_size(Size2(200 * EDSCALE, 0));

	VBoxContainer *main_container = memnew(VBoxContainer);
	main_container->set_h_size_flags(SIZE_EXPAND_FILL);
	main_container->set_v_size_flags(SIZE_EXPAND_FILL);
	add_child(main_container);

	_tool_button_container = memnew(HFlowContainer);
	_tool_button_container->set_h_size_flags(SIZE_EXPAND_FILL);
	main_container->add_child(_tool_button_container);

	_tool_button_group.instance();

	_add_button = memnew(ToolButton);
	//_add_button->set_text("Add");
	_add_button->set_toggle_mode(true);
	_add_button->set_pressed(true);
	_add_button->set_button_group(_tool_button_group);
	_add_button->set_meta("tool_mode", TOOL_MODE_ADD);
	_add_button->connect("button_up", this, "_on_tool_button_pressed");
	_add_button->set_shortcut(ED_SHORTCUT("terrain_world_editor/add_mode", "Add Mode", KEY_A));
	_tool_button_container->add_child(_add_button);

	_remove_button = memnew(ToolButton);
	//_remove_button->set_text("Remove");
	_remove_button->set_toggle_mode(true);
	_remove_button->set_button_group(_tool_button_group);
	_remove_button->set_meta("tool_mode", TOOL_MODE_REMOVE);
	_remove_button->connect("button_up", this, "_on_tool_button_pressed");
	_remove_button->set_shortcut(ED_SHORTCUT("terrain_world_editor/remove_mode", "Remove Mode", KEY_S));
	_tool_button_container->add_child(_remove_button);

	_isolevel_brush_button = memnew(ToolButton);
	_isolevel_brush_button->set_toggle_mode(true);
	_isolevel_brush_button->set_button_group(_tool_button_group);
	_isolevel_brush_button->set_meta("tool_mode", TOOL_MODE_ISOLEVEL_BRUSH);
	_isolevel_brush_button->connect("button_up", this, "_on_tool_button_pressed");
	_isolevel_brush_button->set_shortcut(ED_SHORTCUT("terrain_world_editor/isolevel_brush", "Isolevel Brush", KEY_I));
	_tool_button_container->add_child(_isolevel_brush_button);

	_paint_brush_button = memnew(ToolButton);
	_paint_brush_button->set_toggle_mode(true);
	_paint_brush_button->set_button_group(_tool_button_group);
	_paint_brush_button->set_meta("tool_mode", TOOL_MODE_PAINT_BRUSH);
	_paint_brush_button->connect("button_up", this, "_on_tool_button_pressed");
	_paint_brush_button->set_shortcut(ED_SHORTCUT("terrain_world_editor/paint_brush", "Paint Brush", KEY_P));
	_tool_button_container->add_child(_paint_brush_button);

	_paint_picker_button = memnew(ToolButton);
	_paint_picker_button->set_toggle_mode(true);
	_paint_picker_button->set_button_group(_tool_button_group);
	_paint_picker_button->set_meta("tool_mode", TOOL_MODE_PAINT_PICKER);
	_paint_picker_button->connect("button_up", this, "_on_tool_button_pressed");
	_paint_picker_button->set_shortcut(ED_SHORTCUT("terrain_world_editor/paint_picker", "Paint Picker", KEY_L));
	_tool_button_container->add_child(_paint_picker_button);

	main_container->add_child(memnew(HSeparator));

	_add_remove_tool_container = memnew(VBoxContainer);
	main_container->add_child(_add_remove_tool_container);

	Label *add_remove_tool_label = memnew(Label);
	add_remove_tool_label->set_text(TTR("Add / Remove Tool"));
	_add_remove_tool_container->add_child(add_remove_tool_label);

	_insert_button = memnew(ToolButton);
	//_insert_button->set_text("Insert");
	_insert_button->connect("button_up", this, "_on_insert_block_at_camera_button_pressed");
	_insert_button->set_shortcut(ED_SHORTCUT("terrain_world_editor/instert_block_at_camera", "Insert at camera", KEY_B));
	_add_remove_tool_container->add_child(_insert_button);

	_add_remove_isolevel_slider_label = memnew(Label);
	_add_remove_isolevel_slider_label->set_text(TTR("Isolevel"));
	_add_remove_tool_container->add_child(_add_remove_isolevel_slider_label);

	_add_remove_isolevel_slider = memnew(HSlider);
	_add_remove_isolevel_slider->set_min(1);
	_add_remove_isolevel_slider->set_value(_add_remove_current_isolevel);
	_add_remove_isolevel_slider->set_custom_minimum_size(Size2(50 * EDSCALE, 0));
	_add_remove_isolevel_slider->set_v_size_flags(SIZE_EXPAND_FILL);
	_add_remove_isolevel_slider->set_h_size_flags(SIZE_EXPAND_FILL);
	_add_remove_isolevel_slider->set_tooltip(TTR("Isolevel"));
	_add_remove_tool_container->add_child(_add_remove_isolevel_slider);
	_add_remove_isolevel_slider->connect("value_changed", this, "_on_add_remove_isolevel_slider_value_changed");
	_add_remove_isolevel_slider->hide();

	// Isolevel Brush
	_isolevel_brush_tool_container = memnew(VBoxContainer);
	_isolevel_brush_tool_container->hide();
	main_container->add_child(_isolevel_brush_tool_container);

	_isolevel_brush_tool_button_group.instance();

	Label *isolevel_brush_label = memnew(Label);
	isolevel_brush_label->set_text(TTR("Isolevel Brush"));
	_isolevel_brush_tool_container->add_child(isolevel_brush_label);

	HFlowContainer *isolevel_brush_flow_container = memnew(HFlowContainer);
	_isolevel_brush_tool_container->add_child(isolevel_brush_flow_container);

	_isolevel_brush_type_add_button = memnew(ToolButton);
	_isolevel_brush_type_add_button->set_toggle_mode(true);
	_isolevel_brush_type_add_button->set_button_group(_isolevel_brush_tool_button_group);
	_isolevel_brush_type_add_button->set_meta("type", ISOLEVEL_BRUSH_TYPE_ADD);
	_isolevel_brush_type_add_button->connect("button_up", this, "_on_isolevel_brush_tool_button_pressed");
	_isolevel_brush_type_add_button->set_shortcut(ED_SHORTCUT("terrain_world_editor/isolevel_brush_type_add", "Isolevel Brush Type Add", KEY_G));
	_isolevel_brush_type_add_button->set_pressed(true);
	isolevel_brush_flow_container->add_child(_isolevel_brush_type_add_button);

	_isolevel_brush_type_substract_button = memnew(ToolButton);
	_isolevel_brush_type_substract_button->set_toggle_mode(true);
	_isolevel_brush_type_substract_button->set_button_group(_isolevel_brush_tool_button_group);
	_isolevel_brush_type_substract_button->set_meta("type", ISOLEVEL_BRUSH_TYPE_SUBSTRACT);
	_isolevel_brush_type_substract_button->connect("button_up", this, "_on_isolevel_brush_tool_button_pressed");
	_isolevel_brush_type_substract_button->set_shortcut(ED_SHORTCUT("terrain_world_editor/isolevel_brush_type_substract", "Isolevel Brush Type Substract", KEY_H));
	isolevel_brush_flow_container->add_child(_isolevel_brush_type_substract_button);

	_isolevel_brush_type_set_button = memnew(ToolButton);
	_isolevel_brush_type_set_button->set_toggle_mode(true);
	_isolevel_brush_type_set_button->set_button_group(_isolevel_brush_tool_button_group);
	_isolevel_brush_type_set_button->set_meta("type", ISOLEVEL_BRUSH_TYPE_SET);
	_isolevel_brush_type_set_button->connect("button_up", this, "_on_isolevel_brush_tool_button_pressed");
	_isolevel_brush_type_set_button->set_shortcut(ED_SHORTCUT("terrain_world_editor/isolevel_brush_type_set", "Isolevel Brush Type Set", KEY_J));
	isolevel_brush_flow_container->add_child(_isolevel_brush_type_set_button);

	_isolevel_brush_type_smooth_button = memnew(ToolButton);
	_isolevel_brush_type_smooth_button->set_toggle_mode(true);
	_isolevel_brush_type_smooth_button->set_button_group(_isolevel_brush_tool_button_group);
	_isolevel_brush_type_smooth_button->set_meta("type", ISOLEVEL_BRUSH_TYPE_SMOOTH);
	_isolevel_brush_type_smooth_button->connect("button_up", this, "_on_isolevel_brush_tool_button_pressed");
	_isolevel_brush_type_smooth_button->set_shortcut(ED_SHORTCUT("terrain_world_editor/isolevel_brush_type_smooth", "Isolevel Brush Type Smooth", KEY_K));
	isolevel_brush_flow_container->add_child(_isolevel_brush_type_smooth_button);

	Label *isolevel_brush_size_label = memnew(Label);
	isolevel_brush_size_label->set_text(TTR("Size"));
	_isolevel_brush_tool_container->add_child(isolevel_brush_size_label);

	_isolevel_brush_size_slider = memnew(HSlider);
	_isolevel_brush_size_slider->set_min(1);
	_isolevel_brush_size_slider->set_max(250);
	_isolevel_brush_size_slider->set_value(_isolevel_brush_size);
	_isolevel_brush_size_slider->set_v_size_flags(SIZE_EXPAND_FILL);
	_isolevel_brush_size_slider->set_tooltip(TTR("Brush Size"));
	_isolevel_brush_size_slider->connect("value_changed", this, "_on_isolevel_brush_size_slider_changed");
	_isolevel_brush_tool_container->add_child(_isolevel_brush_size_slider);

	Label *isolevel_brush_smoothness_label = memnew(Label);
	isolevel_brush_smoothness_label->set_text(TTR("Smoothness"));
	_isolevel_brush_tool_container->add_child(isolevel_brush_smoothness_label);

	_isolevel_brush_smoothness_slider = memnew(HSlider);
	_isolevel_brush_smoothness_slider->set_min(1);
	_isolevel_brush_smoothness_slider->set_max(250);
	_isolevel_brush_smoothness_slider->set_value(_isolevel_brush_smoothness);
	_isolevel_brush_smoothness_slider->set_v_size_flags(SIZE_EXPAND_FILL);
	_isolevel_brush_smoothness_slider->set_tooltip(TTR("Brush Smoothness"));
	_isolevel_brush_smoothness_slider->connect("value_changed", this, "_on_isolevel_brush_smoothness_slider_changed");
	_isolevel_brush_tool_container->add_child(_isolevel_brush_smoothness_slider);

	// Isolevel Brush Channel Select Dropdown
	Label *isolevel_brush_channel_label = memnew(Label);
	isolevel_brush_channel_label->set_text(TTR("Channel"));
	_isolevel_brush_tool_container->add_child(isolevel_brush_channel_label);

	_isolevel_brush_channel_select_sb = memnew(SpinBox);
	_isolevel_brush_channel_select_sb->set_v_size_flags(SIZE_EXPAND_FILL);
	_isolevel_brush_channel_select_sb->set_tooltip(TTR("Isolevel Channel"));
	_isolevel_brush_channel_select_sb->connect("value_changed", this, "_on_isolevel_brush_channel_select_sb_changed");
	_isolevel_brush_tool_container->add_child(_isolevel_brush_channel_select_sb);

	// Surface Selector Separator
	main_container->add_child(memnew(HSeparator));

	// Surface selector
	ScrollContainer *scs = memnew(ScrollContainer);
	scs->set_h_size_flags(SIZE_EXPAND_FILL);
	scs->set_v_size_flags(SIZE_EXPAND_FILL);
	scs->set_name("Surfaces");
	main_container->add_child(scs);

	_surfaces_vbox_container = memnew(VBoxContainer);
	scs->add_child(_surfaces_vbox_container);
	_surfaces_vbox_container->set_h_size_flags(SIZE_EXPAND_FILL);

	_surfaces_button_group.instance();
}
TerrainWorldEditor::~TerrainWorldEditor() {
	_world = NULL;

	_surfaces_button_group.unref();
}

void TerrainWorldEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE:
		case NOTIFICATION_THEME_CHANGED: {
			_add_button->set_icon(get_theme_icon("Add", "EditorIcons"));
			_remove_button->set_icon(get_theme_icon("Remove", "EditorIcons"));
			_insert_button->set_icon(get_theme_icon("InsertBefore", "EditorIcons"));
			_isolevel_brush_button->set_icon(get_theme_icon("CanvasItem", "EditorIcons"));
			_paint_brush_button->set_icon(get_theme_icon("CanvasItemShader", "EditorIcons"));
			_paint_picker_button->set_icon(get_theme_icon("ColorPick", "EditorIcons"));

			// Isolevel Brush
			_isolevel_brush_type_add_button->set_icon(get_theme_icon("MoveUp", "EditorIcons"));
			_isolevel_brush_type_substract_button->set_icon(get_theme_icon("MoveDown", "EditorIcons"));
			_isolevel_brush_type_set_button->set_icon(get_theme_icon("CanvasLayer", "EditorIcons"));
			_isolevel_brush_type_smooth_button->set_icon(get_theme_icon("Blend", "EditorIcons"));
		} break;
	}
}

void TerrainWorldEditor::_node_removed(Node *p_node) {
	if (p_node == _world) {
		_world = NULL;
	}
}

void TerrainWorldEditor::_on_surface_button_pressed() {
	BaseButton *button = _surfaces_button_group->get_pressed_button();

	if (button) {
		_selected_type = button->get_meta("index");
	}
}

void TerrainWorldEditor::_on_tool_button_pressed() {
	BaseButton *button = _tool_button_group->get_pressed_button();

	if (button) {
		_previous_tool_mode = _tool_mode;
		_tool_mode = static_cast<TerrainWorldEditorToolMode>(static_cast<int>(button->get_meta("tool_mode")));
	}

	switch (_tool_mode) {
		case TOOL_MODE_ADD:
		case TOOL_MODE_REMOVE:
			_add_remove_tool_container->show();
			_isolevel_brush_tool_container->hide();
			break;
		case TOOL_MODE_PAINT_BRUSH:
			_add_remove_tool_container->hide();
			_isolevel_brush_tool_container->hide();
			break;
		case TOOL_MODE_ISOLEVEL_BRUSH:
			_add_remove_tool_container->hide();
			_isolevel_brush_tool_container->show();
			break;
		case TOOL_MODE_PAINT_PICKER:
			_add_remove_tool_container->hide();
			_isolevel_brush_tool_container->hide();
			break;
		default:
			break;
	}
}

void TerrainWorldEditor::_on_insert_block_at_camera_button_pressed() {
	int selected_terrain = 0;
	int channel = 0;

	channel = _channel_type;

	if (channel == -1)
		return;

	SpatialEditorViewport *vp = SpatialEditor::get_singleton()->get_editor_viewport(0);

	if (!vp)
		return;

	Camera *cam = vp->get_camera();

	if (!cam)
		return;

	Vector3 pos = cam->get_transform().origin;
	selected_terrain = _selected_type + 1;

	_undo_redo->create_action("Insert terrain voxel at camera.");

	uint8_t prev_terrain = _world->get_voxel_at_world_position(pos, _channel_type);
	uint8_t prev_isolevel = 0;

	if (_channel_isolevel != -1) {
		prev_isolevel = _world->get_voxel_at_world_position(pos, _channel_isolevel);
	}

	if (_channel_isolevel == -1) {
		_undo_redo->add_do_method(_world, "set_voxel_at_world_position", pos, selected_terrain, channel);
		_undo_redo->add_undo_method(_world, "set_voxel_at_world_position", pos, prev_terrain, channel);
	} else {
		_undo_redo->add_do_method(_world, "set_voxel_at_world_position", pos, selected_terrain, channel, false);
		_undo_redo->add_do_method(_world, "set_voxel_at_world_position", pos, _add_remove_current_isolevel, _channel_isolevel);
		_undo_redo->add_undo_method(_world, "set_voxel_at_world_position", pos, prev_terrain, channel, false);
		_undo_redo->add_undo_method(_world, "set_voxel_at_world_position", pos, prev_isolevel, _channel_isolevel);
	}

	_undo_redo->commit_action();
}

void TerrainWorldEditor::_on_add_remove_isolevel_slider_value_changed(float value) {
	_add_remove_current_isolevel = value;
}

void TerrainWorldEditor::_on_isolevel_brush_tool_button_pressed() {
	BaseButton *button = _isolevel_brush_tool_button_group->get_pressed_button();

	if (button) {
		_isolevel_brush_type = static_cast<IsolevelBrushType>(static_cast<int>(button->get_meta("type")));
	}
}

void TerrainWorldEditor::_on_isolevel_brush_size_slider_changed(float value) {
	_isolevel_brush_size = value;
}
void TerrainWorldEditor::_on_isolevel_brush_smoothness_slider_changed(float value) {
	_isolevel_brush_smoothness = value;
}

void TerrainWorldEditor::_on_isolevel_brush_channel_select_sb_changed(int value) {
	_isolevel_brush_channel = value;
}

void TerrainWorldEditor::_bind_methods() {
	ClassDB::bind_method("_node_removed", &TerrainWorldEditor::_node_removed);
	ClassDB::bind_method("_on_surface_button_pressed", &TerrainWorldEditor::_on_surface_button_pressed);
	ClassDB::bind_method("_on_tool_button_pressed", &TerrainWorldEditor::_on_tool_button_pressed);
	ClassDB::bind_method("_on_insert_block_at_camera_button_pressed", &TerrainWorldEditor::_on_insert_block_at_camera_button_pressed);
	ClassDB::bind_method("_on_add_remove_isolevel_slider_value_changed", &TerrainWorldEditor::_on_add_remove_isolevel_slider_value_changed);
	ClassDB::bind_method("_on_isolevel_brush_tool_button_pressed", &TerrainWorldEditor::_on_isolevel_brush_tool_button_pressed);
	ClassDB::bind_method("_on_isolevel_brush_size_slider_changed", &TerrainWorldEditor::_on_isolevel_brush_size_slider_changed);
	ClassDB::bind_method("_on_isolevel_brush_smoothness_slider_changed", &TerrainWorldEditor::_on_isolevel_brush_smoothness_slider_changed);
	ClassDB::bind_method("_on_isolevel_brush_channel_select_sb_changed", &TerrainWorldEditor::_on_isolevel_brush_channel_select_sb_changed);
}

void TerrainWorldEditorPlugin::_notification(int p_what) {
	if (p_what == EditorSettings::NOTIFICATION_EDITOR_SETTINGS_CHANGED) {
		switch ((int)EditorSettings::get_singleton()->get("editors/terrain/editor_side")) {
			case 0: { // Left.
				SpatialEditor::get_singleton()->move_control_to_left_panel(terrain_world_editor);
			} break;
			case 1: { // Right.
				SpatialEditor::get_singleton()->move_control_to_right_panel(terrain_world_editor);
			} break;
		}
	}
}

void TerrainWorldEditorPlugin::edit(Object *p_object) {
	terrain_world_editor->edit(Object::cast_to<TerrainWorld>(p_object));
}

bool TerrainWorldEditorPlugin::handles(Object *p_object) const {
	if (!p_object->is_class("TerrainWorld")) {
		return false;
	}

	TerrainWorld *w = Object::cast_to<TerrainWorld>(p_object);

	return w->get_editable();
}

void TerrainWorldEditorPlugin::make_visible(bool p_visible) {
	if (p_visible) {
		terrain_world_editor->show();
		terrain_world_editor->set_process(true);
	} else {
		terrain_world_editor->hide();
		terrain_world_editor->edit(NULL);
		terrain_world_editor->set_process(false);
	}
}

TerrainWorldEditorPlugin::TerrainWorldEditorPlugin(EditorNode *p_node) {
	editor = p_node;

	EDITOR_DEF("editors/terrain/editor_side", 1);
	EditorSettings::get_singleton()->add_property_hint(PropertyInfo(Variant::INT, "editors/terrain/editor_side", PROPERTY_HINT_ENUM, "Left,Right"));

	terrain_world_editor = memnew(TerrainWorldEditor(editor));
	switch ((int)EditorSettings::get_singleton()->get("editors/terrain/editor_side")) {
		case 0: { // Left.
			add_control_to_container(CONTAINER_SPATIAL_EDITOR_SIDE_LEFT, terrain_world_editor);
		} break;
		case 1: { // Right.
			add_control_to_container(CONTAINER_SPATIAL_EDITOR_SIDE_RIGHT, terrain_world_editor);
		} break;
	}
	make_visible(false);
}

TerrainWorldEditorPlugin::~TerrainWorldEditorPlugin() {
}
