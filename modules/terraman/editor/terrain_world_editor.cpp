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

#include "core/error/error_macros.h"
#include "core/math/plane.h"

#include "editor/editor_plugin.h"
#include "editor/editor_scale.h"
#include "editor/editor_settings.h"

#include "terrain_world_gizmo.h"
#include "terrain_world_gizmo_plugin.h"

#include "../world/terrain_world.h"
#include "scene/gui/box_container.h"
#include "scene/gui/flow_container.h"
#include "scene/main/control.h"

#include "core/os/keyboard.h"

#include "../world/terrain_chunk.h"

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

#include "editor/editor_inspector.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_PROPS_ENABLED
#include "modules/props/prop_instance_merger.h"
#include "modules/props/props/prop_data.h"
#endif

EditorPlugin::AfterGUIInput TerrainWorldEditor::forward_spatial_input_event(Camera *p_camera, const Ref<InputEvent> &p_event) {
	if (!_world || !_world->get_editable()) {
		return EditorPlugin::AFTER_GUI_INPUT_PASS;
	}

	Ref<TerrainLibrary> lib = _world->get_library();

	if (!lib.is_valid()) {
		return EditorPlugin::AFTER_GUI_INPUT_PASS;
	}

	Ref<InputEventMouseMotion> mm = p_event;

	if (mm.is_valid()) {
		if (!_mouse_down) {
			switch (_tool_mode) {
				case TOOL_MODE_ISOLEVEL_BRUSH: {
					Vector3 position;
					Vector3 normal;

					if (get_draw_world_coordinate(p_camera, Point2(mm->get_position().x, mm->get_position().y), position, normal)) {
						_gizmo->visible = true;
						_gizmo->drawing = false;
						_gizmo->position = position;
						_gizmo->refresh_lines(_world);
					} else {
						_gizmo->visible = false;
						_gizmo->redraw();
					}
				} break;
				case TOOL_MODE_PAINT_BRUSH: {
					Vector3 position;
					Vector3 normal;

					if (get_draw_world_coordinate(p_camera, Point2(mm->get_position().x, mm->get_position().y), position, normal)) {
						_gizmo->visible = true;
						_gizmo->drawing = false;
						_gizmo->position = position;
						_gizmo->refresh_lines(_world);
					} else {
						_gizmo->visible = false;
						_gizmo->redraw();
					}
				} break;
				case TOOL_MODE_PAINT_PICKER: {
					// Ignore
				} break;
				case TOOL_MODE_CHUNK_SPAWN_BRUSH: {
					Vector3 position;
					Vector3 normal;

					if (get_draw_world_coordinate(p_camera, Point2(mm->get_position().x, mm->get_position().y), position, normal)) {
						_gizmo->visible = true;
						_gizmo->drawing = false;
						_gizmo->position = position;
						_gizmo->refresh_lines(_world);
					} else {
						_gizmo->visible = false;
						_gizmo->redraw();
					}
				} break;
				case TOOL_MODE_CHUNK_REMOVE_BRUSH: {
					Vector3 position;
					Vector3 normal;

					if (get_draw_world_coordinate(p_camera, Point2(mm->get_position().x, mm->get_position().y), position, normal)) {
						_gizmo->visible = true;
						_gizmo->drawing = false;
						_gizmo->position = position;
						_gizmo->refresh_lines(_world);
					} else {
						_gizmo->visible = false;
						_gizmo->redraw();
					}
				} break;
				case TOOL_MODE_BAKING_TOOLS: {
					// Ignore
					break;
				}
			}

			return EditorPlugin::AFTER_GUI_INPUT_PASS;
		}

		switch (_tool_mode) {
			case TOOL_MODE_ISOLEVEL_BRUSH: {
				Vector3 position;
				Vector3 normal;

				if (get_draw_world_coordinate(p_camera, Point2(mm->get_position().x, mm->get_position().y), position, normal)) {
					isolevel_brush_draw(position);

					_gizmo->visible = true;
					_gizmo->drawing = true;
					_gizmo->position = position;
					_gizmo->refresh_lines(_world);
				} else {
					_gizmo->visible = false;
					_gizmo->redraw();
				}
			} break;
			case TOOL_MODE_PAINT_BRUSH: {
				Vector3 position;
				Vector3 normal;

				if (get_draw_world_coordinate(p_camera, Point2(mm->get_position().x, mm->get_position().y), position, normal)) {
					paint_brush_draw(position);

					_gizmo->visible = true;
					_gizmo->drawing = true;
					_gizmo->position = position;
					_gizmo->refresh_lines(_world);
				} else {
					_gizmo->visible = false;
					_gizmo->redraw();
				}
			} break;
			case TOOL_MODE_PAINT_PICKER: {
				// Ignore
			} break;
			case TOOL_MODE_CHUNK_SPAWN_BRUSH: {
				Vector3 position;
				Vector3 normal;

				if (get_draw_world_coordinate(p_camera, Point2(mm->get_position().x, mm->get_position().y), position, normal)) {
					chunk_spawn_brush_draw(position);

					_gizmo->visible = true;
					_gizmo->drawing = true;
					_gizmo->position = position;
					_gizmo->refresh_lines(_world);
				} else {
					_gizmo->visible = false;
					_gizmo->redraw();
				}
			} break;
			case TOOL_MODE_CHUNK_REMOVE_BRUSH: {
				Vector3 position;
				Vector3 normal;

				if (get_draw_world_coordinate(p_camera, Point2(mm->get_position().x, mm->get_position().y), position, normal)) {
					chunk_remove_brush_draw(position);

					_gizmo->visible = true;
					_gizmo->drawing = true;
					_gizmo->position = position;
					_gizmo->refresh_lines(_world);
				} else {
					_gizmo->visible = false;
					_gizmo->redraw();
				}
			} break;
			case TOOL_MODE_BAKING_TOOLS: {
				// Ignore
				break;
			}
		}

		return EditorPlugin::AFTER_GUI_INPUT_STOP;
	}

	Ref<InputEventMouseButton> mb = p_event;

	if (mb.is_valid()) {
		if (mb->is_pressed()) {
			if (mb->get_button_index() == BUTTON_LEFT) {
				switch (_tool_mode) {
					case TOOL_MODE_ISOLEVEL_BRUSH: {
						Vector3 position;
						Vector3 normal;

						if (get_draw_world_coordinate(p_camera, Point2(mb->get_position().x, mb->get_position().y), position, normal)) {
							_mouse_down = true;

							_current_action = "Isolevel Brush Draw";

							isolevel_brush_draw(position);

							_gizmo->visible = true;
							_gizmo->drawing = true;
							_gizmo->position = position;
							_gizmo->refresh_lines(_world);

							return EditorPlugin::AFTER_GUI_INPUT_STOP;
						} else {
							_gizmo->visible = false;
							_gizmo->redraw();

							return EditorPlugin::AFTER_GUI_INPUT_PASS;
						}
					} break;
					case TOOL_MODE_PAINT_BRUSH: {
						Vector3 position;
						Vector3 normal;

						if (get_draw_world_coordinate(p_camera, Point2(mb->get_position().x, mb->get_position().y), position, normal)) {
							_mouse_down = true;

							_current_action = "Paint Brush Draw";

							paint_brush_draw(position);

							_gizmo->visible = true;
							_gizmo->drawing = true;
							_gizmo->position = position;
							_gizmo->refresh_lines(_world);

							return EditorPlugin::AFTER_GUI_INPUT_STOP;
						} else {
							_gizmo->visible = false;
							_gizmo->redraw();

							return EditorPlugin::AFTER_GUI_INPUT_PASS;
						}
					} break;
					case TOOL_MODE_PAINT_PICKER: {
						_mouse_down = true;
					} break;
					case TOOL_MODE_CHUNK_SPAWN_BRUSH: {
						Vector3 position;
						Vector3 normal;

						if (get_draw_world_coordinate(p_camera, Point2(mb->get_position().x, mb->get_position().y), position, normal)) {
							_mouse_down = true;

							chunk_spawn_brush_draw(position);

							_gizmo->visible = true;
							_gizmo->drawing = true;
							_gizmo->position = position;
							_gizmo->refresh_lines(_world);

							return EditorPlugin::AFTER_GUI_INPUT_STOP;
						} else {
							_gizmo->visible = false;
							_gizmo->redraw();

							return EditorPlugin::AFTER_GUI_INPUT_PASS;
						}
					} break;
					case TOOL_MODE_CHUNK_REMOVE_BRUSH: {
						Vector3 position;
						Vector3 normal;

						if (get_draw_world_coordinate(p_camera, Point2(mb->get_position().x, mb->get_position().y), position, normal)) {
							_mouse_down = true;

							chunk_remove_brush_draw(position);

							_gizmo->visible = true;
							_gizmo->drawing = true;
							_gizmo->position = position;
							_gizmo->refresh_lines(_world);

							return EditorPlugin::AFTER_GUI_INPUT_STOP;
						} else {
							_gizmo->visible = false;
							_gizmo->redraw();

							return EditorPlugin::AFTER_GUI_INPUT_PASS;
						}
					} break;
					case TOOL_MODE_BAKING_TOOLS: {
						break;
					}
				}

				return EditorPlugin::AFTER_GUI_INPUT_PASS;
			} else {
				return EditorPlugin::AFTER_GUI_INPUT_PASS;
			}
		} else {
			if (mb->get_button_index() == BUTTON_LEFT) {
				switch (_tool_mode) {
					case TOOL_MODE_ISOLEVEL_BRUSH: {
						create_undo_point(_current_action, _isolevel_brush_channel, _isolevel_brush_allow_create_chunks);
					} break;
					case TOOL_MODE_PAINT_BRUSH: {
						create_undo_point(_current_action, _paint_brush_channel, _paint_brush_allow_create_chunks);
					} break;
					case TOOL_MODE_PAINT_PICKER: {
						Vector3 position;
						Vector3 normal;

						if (get_draw_world_coordinate(p_camera, Point2(mb->get_position().x, mb->get_position().y), position, normal)) {
							paint_pick(position);
						}
					} break;
					case TOOL_MODE_CHUNK_SPAWN_BRUSH: {
						create_chunk_created_undo_point();
					} break;
					case TOOL_MODE_CHUNK_REMOVE_BRUSH: {
						create_chunk_removed_undo_point();
					} break;
					case TOOL_MODE_BAKING_TOOLS: {
						break;
					}
				}

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

bool TerrainWorldEditor::get_draw_world_coordinate(Camera *p_camera, const Point2 &p_point, Vector3 &r_position, Vector3 &r_normal) {
	Camera *camera = p_camera;
	Vector3 from = camera->project_ray_origin(p_point);
	Vector3 to = from + camera->project_ray_normal(p_point) * 10000;
	Transform local_xform = _world->get_global_transform().affine_inverse();

	from = local_xform.xform(from);
	to = local_xform.xform(to);

	PhysicsDirectSpaceState *ss = _world->get_world_3d()->get_direct_space_state();

	PhysicsDirectSpaceState::RayResult res;

	if (ss->intersect_ray(from, to, res)) {
		r_position = res.position;
		r_normal = res.normal;

		return true;
	}

	Vector3 intersection;
	Plane xz_plane(Vector3(), Vector3(0, 1, 0));
	if (xz_plane.intersects_ray(from, to, &intersection)) {
		r_normal = Vector3(0, 1, 0);
		r_position = intersection;

		return true;
	}

	return false;
}

void TerrainWorldEditor::isolevel_brush_draw(const Vector3 &p_world_position) {
	if (_isolevel_brush_size == 0) {
		return;
	}

	if (_isolevel_brush_channel == -1) {
		return;
	}

	Vector2i wdp = _world->world_position_to_world_data_position(p_world_position);

	int ilbh = _isolevel_brush_size / 2;
	// Value will likely need more fine tuning.
	float s = 10.0 * _isolevel_brush_strength;

	Array draw_data;

	// TODO use a proper circle drawing algorithm.
	for (int x = -ilbh; x < ilbh; ++x) {
		for (int y = -ilbh; y < ilbh; ++y) {
			float l = Vector2(x, y).length();

			if (l > ilbh) {
				continue;
			}

			Vector2i vwp = wdp + Vector2i(x, y);

			uint8_t orig_val = _world->get_voxel_at_world_data_position(vwp, _isolevel_brush_channel);
			int pil = orig_val;

			Vector2 tv = Vector2(ilbh - ABS(x), ilbh - ABS(y));
			float t = tv.length() / (float)ilbh;

			float sl = Math::lerp(0, s, CLAMP(t + 1 - _isolevel_brush_smoothness, 0, 1));

			float npil = 0;

			switch (_isolevel_brush_type) {
				case ISOLEVEL_BRUSH_TYPE_ADD: {
					npil = pil + sl;
				} break;
				case ISOLEVEL_BRUSH_TYPE_SUBSTRACT: {
					npil = pil - sl;
				} break;
				case ISOLEVEL_BRUSH_TYPE_SET: {
					npil = _isolevel_brush_strength * 255.0;
				} break;
			}

			npil = CLAMP(npil, 0, 255);

			uint8_t new_val = static_cast<uint8_t>(npil);

			draw_data.push_back(vwp);
			draw_data.push_back(new_val);

			if (!_original_data.has(vwp)) {
				_original_data[vwp] = orig_val;
			}

			_current_data[vwp] = new_val;
		}
	}

	_world->set_voxels_at_world_data_position(draw_data, _isolevel_brush_channel, true, _isolevel_brush_allow_create_chunks, false);
}

void TerrainWorldEditor::paint_brush_draw(const Vector3 &p_world_position) {
	if (_paint_brush_size == 0) {
		return;
	}

	if (_paint_brush_channel == -1) {
		return;
	}

	Vector2i wdp = _world->world_position_to_world_data_position(p_world_position);

	int ilbh = _paint_brush_size / 2;

	int selected_terrain = 0;
	selected_terrain = _selected_type + 1;
	uint8_t new_val = static_cast<uint8_t>(selected_terrain);

	Array draw_data;

	// TODO use a proper circle drawing algorithm.
	for (int x = -ilbh; x < ilbh; ++x) {
		for (int y = -ilbh; y < ilbh; ++y) {
			float l = Vector2(x, y).length();

			if (l > ilbh) {
				continue;
			}

			Vector2i vwp = wdp + Vector2i(x, y);

			uint8_t orig_val = _world->get_voxel_at_world_data_position(vwp, _paint_brush_channel);

			draw_data.push_back(vwp);
			draw_data.push_back(new_val);

			if (!_original_data.has(vwp)) {
				_original_data[vwp] = orig_val;
			}

			_current_data[vwp] = new_val;
		}
	}

	_world->set_voxels_at_world_data_position(draw_data, _paint_brush_channel, true, _paint_brush_allow_create_chunks, true);
}

void TerrainWorldEditor::paint_pick(const Vector3 &p_world_position) {
	Vector2i wdp = _world->world_position_to_world_data_position(p_world_position);

	uint8_t val = 0;

	if (_previous_tool_mode == TOOL_MODE_ISOLEVEL_BRUSH) {
		val = _world->get_voxel_at_world_data_position(wdp, _isolevel_brush_channel);

		_isolevel_brush_strength_slider->set_value((float)val / 255.0);
		_isolevel_brush_type_set_button->set_pressed(true);
		_isolevel_brush_button->set_pressed(true);
	} else {
		val = _world->get_voxel_at_world_data_position(wdp, _paint_brush_channel);

		int index = (int)val - 1;

		for (int i = 0; i < _surfaces_vbox_container->get_child_count(); ++i) {
			Button *b = Object::cast_to<Button>(_surfaces_vbox_container->get_child(i));

			if (!b) {
				continue;
			}

			int bindex = b->get_meta("index");

			if (bindex == index) {
				b->set_pressed(true);
				_paint_brush_button->set_pressed(true);
				return;
			}
		}
	}
}

void TerrainWorldEditor::chunk_spawn_brush_draw(const Vector3 &p_world_position) {
	if (_chunk_spawn_brush_size == 0) {
		return;
	}

	Vector2i wdp = _world->world_position_to_world_data_position(p_world_position);

	int ilbh = _paint_brush_size / 2;

	// TODO use a proper circle drawing algorithm.
	for (int x = -ilbh; x < ilbh; ++x) {
		for (int y = -ilbh; y < ilbh; ++y) {
			float l = Vector2(x, y).length();

			if (l > ilbh) {
				continue;
			}

			Vector2i vwp = wdp + Vector2i(x, y);

			Vector2i chunk_pos = _world->world_data_position_to_chunk_position(vwp);

			if (_created_chunks.has(chunk_pos)) {
				continue;
			}

			Ref<TerrainChunk> chunk = _world->chunk_get(chunk_pos.x, chunk_pos.y);

			if (chunk.is_valid()) {
				continue;
			}

			chunk = _world->chunk_create(chunk_pos.x, chunk_pos.y);

			_created_chunks[chunk_pos] = chunk;
		}
	}
}

void TerrainWorldEditor::chunk_remove_brush_draw(const Vector3 &p_world_position) {
	if (_chunk_spawn_brush_size == 0) {
		return;
	}

	Vector2i wdp = _world->world_position_to_world_data_position(p_world_position);

	int ilbh = _paint_brush_size / 2;

	// TODO use a proper circle drawing algorithm.
	for (int x = -ilbh; x < ilbh; ++x) {
		for (int y = -ilbh; y < ilbh; ++y) {
			float l = Vector2(x, y).length();

			if (l > ilbh) {
				continue;
			}

			Vector2i vwp = wdp + Vector2i(x, y);

			Vector2i chunk_pos = _world->world_data_position_to_chunk_position(vwp);

			Ref<TerrainChunk> chunk = _world->chunk_get(chunk_pos.x, chunk_pos.y);

			if (!chunk.is_valid()) {
				continue;
			}

			_removed_chunks[chunk_pos] = chunk;
			_world->chunk_remove(chunk_pos.x, chunk_pos.y);
		}
	}
}

void TerrainWorldEditor::edit(TerrainWorld *p_world) {
	if (_world == p_world) {
		return;
	}

	if (_gizmo.is_valid()) {
		_gizmo->visible = false;
		_gizmo->redraw();
	}

	_world = p_world;

	if (!_world) {
		return;
	}

	_gizmo = get_gizmo_from(_world);

	if (!_gizmo.is_valid()) {
		return;
	}

	switch (_tool_mode) {
		case TOOL_MODE_ISOLEVEL_BRUSH: {
			_gizmo->size = _isolevel_brush_size;
		} break;
		case TOOL_MODE_PAINT_BRUSH: {
			_gizmo->size = _paint_brush_size;
		} break;
		case TOOL_MODE_PAINT_PICKER: {
		} break;
		case TOOL_MODE_CHUNK_SPAWN_BRUSH: {
			_gizmo->size = _chunk_spawn_brush_size;
		} break;
		case TOOL_MODE_CHUNK_REMOVE_BRUSH: {
			_gizmo->size = _chunk_remove_brush_size;
		} break;
		case TOOL_MODE_BAKING_TOOLS: {
			break;
		}
	}

	if (_paint_brush_liquid_mode) {
		_channel_type = _world->get_channel_index_info(TerrainWorld::CHANNEL_TYPE_INFO_LIQUID_TYPE);
	} else {
		_channel_type = _world->get_channel_index_info(TerrainWorld::CHANNEL_TYPE_INFO_TYPE);
	}

	if (_isolevel_brush_liquid_mode) {
		_channel_isolevel = _world->get_channel_index_info(TerrainWorld::CHANNEL_TYPE_INFO_LIQUID_ISOLEVEL);
	} else {
		_channel_isolevel = _world->get_channel_index_info(TerrainWorld::CHANNEL_TYPE_INFO_ISOLEVEL);
	}

	_paint_brush_channel = _channel_type;
	_paint_brush_channel_select_sb->set_value(_paint_brush_channel);

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

	Button *button = memnew(Button);
	button->set_text(TTR("Erase"));
	button->set_text_align(Button::ALIGN_LEFT);
	button->set_meta("index", -1);
	button->set_toggle_mode(true);
	button->set_button_group(_surfaces_button_group);
	button->set_h_size_flags(SIZE_EXPAND_FILL);
	button->set_pressed(true);

	_surfaces_vbox_container->add_child(button);

	bool f = false;
	for (int i = 0; i < library->terra_surface_get_num(); ++i) {
		Ref<TerrainSurface> surface = library->terra_surface_get(i);

		if (!surface.is_valid()) {
			continue;
		}

		String text = String::num(i) + " - " + surface->get_name();

		button = memnew(Button);
		button->set_text(text);
		button->set_text_align(Button::ALIGN_LEFT);
		button->set_meta("index", i);
		button->set_toggle_mode(true);
		button->set_button_group(_surfaces_button_group);
		button->set_h_size_flags(SIZE_EXPAND_FILL);

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
	_selected_type = -1;
	_channel_type = -1;
	_channel_isolevel = -1;
	_editor = NULL;
	_tool_mode = TOOL_MODE_ISOLEVEL_BRUSH;
	_previous_tool_mode = _tool_mode;

	_mouse_down = false;

	_isolevel_brush_liquid_mode = false;

	_isolevel_brush_channel = -1;
	_isolevel_brush_size = 10;
	_isolevel_brush_strength = 0.25;
	_isolevel_brush_smoothness = 1;
	//_brush_type = BRUSH_TYPE_CIRCLE;
	_isolevel_brush_type = ISOLEVEL_BRUSH_TYPE_ADD;
	_isolevel_brush_allow_create_chunks = false;

	_paint_brush_liquid_mode = false;

	_paint_brush_channel = -1;
	_paint_brush_size = 10;
	_paint_brush_allow_create_chunks = false;

	_chunk_spawn_brush_size = 10;
	_chunk_remove_brush_size = 10;
}
TerrainWorldEditor::TerrainWorldEditor(EditorNode *p_editor) {
	_undo_redo = EditorNode::get_undo_redo();

	_world = NULL;
	_selected_type = -1;
	_channel_type = -1;
	_channel_isolevel = -1;

	_mouse_down = false;

	_isolevel_brush_liquid_mode = false;

	_isolevel_brush_channel = -1;
	_isolevel_brush_size = 10;
	_isolevel_brush_strength = 0.25;
	_isolevel_brush_smoothness = 1;
	//_brush_type = BRUSH_TYPE_CIRCLE;
	_isolevel_brush_type = ISOLEVEL_BRUSH_TYPE_ADD;
	_isolevel_brush_allow_create_chunks = false;

	_paint_brush_liquid_mode = false;

	_paint_brush_channel = -1;
	_paint_brush_size = 10;
	_paint_brush_allow_create_chunks = false;

	_editor = p_editor;
	_tool_mode = TOOL_MODE_ISOLEVEL_BRUSH;
	_previous_tool_mode = _tool_mode;

	_chunk_spawn_brush_size = 10;
	_chunk_remove_brush_size = 10;

	set_custom_minimum_size(Size2(200 * EDSCALE, 0));

	VBoxContainer *main_container = memnew(VBoxContainer);
	main_container->set_h_size_flags(SIZE_EXPAND_FILL);
	main_container->set_v_size_flags(SIZE_EXPAND_FILL);
	add_child(main_container);

	_tool_button_container = memnew(HFlowContainer);
	_tool_button_container->set_h_size_flags(SIZE_EXPAND_FILL);
	main_container->add_child(_tool_button_container);

	_tool_button_group.instance();
	_tool_button_group->connect("pressed", this, "_on_tool_button_pressed");

	// Main buttons
	_isolevel_brush_button = memnew(ToolButton);
	_isolevel_brush_button->set_toggle_mode(true);
	_isolevel_brush_button->set_pressed(true);
	_isolevel_brush_button->set_button_group(_tool_button_group);
	_isolevel_brush_button->set_meta("tool_mode", TOOL_MODE_ISOLEVEL_BRUSH);
	_isolevel_brush_button->set_shortcut(ED_SHORTCUT("terrain_world_editor/isolevel_brush", "Isolevel Brush", KEY_I));
	_tool_button_container->add_child(_isolevel_brush_button);

	_paint_brush_button = memnew(ToolButton);
	_paint_brush_button->set_toggle_mode(true);
	_paint_brush_button->set_button_group(_tool_button_group);
	_paint_brush_button->set_meta("tool_mode", TOOL_MODE_PAINT_BRUSH);
	_paint_brush_button->set_shortcut(ED_SHORTCUT("terrain_world_editor/paint_brush", "Paint Brush", KEY_P));
	_tool_button_container->add_child(_paint_brush_button);

	_paint_picker_button = memnew(ToolButton);
	_paint_picker_button->set_toggle_mode(true);
	_paint_picker_button->set_button_group(_tool_button_group);
	_paint_picker_button->set_meta("tool_mode", TOOL_MODE_PAINT_PICKER);
	_paint_picker_button->set_shortcut(ED_SHORTCUT("terrain_world_editor/paint_picker", "Paint Picker", KEY_L));
	_tool_button_container->add_child(_paint_picker_button);

	_chunk_spawn_brush_button = memnew(ToolButton);
	_chunk_spawn_brush_button->set_toggle_mode(true);
	_chunk_spawn_brush_button->set_button_group(_tool_button_group);
	_chunk_spawn_brush_button->set_meta("tool_mode", TOOL_MODE_CHUNK_SPAWN_BRUSH);
	_chunk_spawn_brush_button->set_shortcut(ED_SHORTCUT("terrain_world_editor/chunk_spawn_brush", "Chunk Spawn Brush", KEY_M));
	_tool_button_container->add_child(_chunk_spawn_brush_button);

	_chunk_remove_brush_button = memnew(ToolButton);
	_chunk_remove_brush_button->set_toggle_mode(true);
	_chunk_remove_brush_button->set_button_group(_tool_button_group);
	_chunk_remove_brush_button->set_meta("tool_mode", TOOL_MODE_CHUNK_REMOVE_BRUSH);
	_chunk_remove_brush_button->set_shortcut(ED_SHORTCUT("terrain_world_editor/chunk_remove_brush", "Chunk Remove Brush", KEY_U));
	_tool_button_container->add_child(_chunk_remove_brush_button);

	_baking_tools_button = memnew(ToolButton);
	_baking_tools_button->set_toggle_mode(true);
	_baking_tools_button->set_button_group(_tool_button_group);
	_baking_tools_button->set_meta("tool_mode", TOOL_MODE_BAKING_TOOLS);
	_baking_tools_button->set_shortcut(ED_SHORTCUT("terrain_world_editor/baking_tools", "Baking Tools", KEY_B));
	_tool_button_container->add_child(_baking_tools_button);

	main_container->add_child(memnew(HSeparator));

	// Isolevel Brush
	_isolevel_brush_tool_container = memnew(VBoxContainer);
	main_container->add_child(_isolevel_brush_tool_container);

	_isolevel_brush_tool_button_group.instance();
	_isolevel_brush_tool_button_group->connect("pressed", this, "_on_isolevel_brush_tool_button_pressed");

	Label *isolevel_brush_label = memnew(Label);
	isolevel_brush_label->set_text(TTR("Isolevel Brush"));
	isolevel_brush_label->set_align(Label::ALIGN_CENTER);
	_isolevel_brush_tool_container->add_child(isolevel_brush_label);

	HFlowContainer *isolevel_brush_flow_container = memnew(HFlowContainer);
	_isolevel_brush_tool_container->add_child(isolevel_brush_flow_container);

	_isolevel_brush_type_add_button = memnew(ToolButton);
	_isolevel_brush_type_add_button->set_toggle_mode(true);
	_isolevel_brush_type_add_button->set_button_group(_isolevel_brush_tool_button_group);
	_isolevel_brush_type_add_button->set_meta("type", ISOLEVEL_BRUSH_TYPE_ADD);
	_isolevel_brush_type_add_button->set_shortcut(ED_SHORTCUT("terrain_world_editor/isolevel_brush_type_add", "Isolevel Brush Type Add", KEY_G));
	_isolevel_brush_type_add_button->set_pressed(true);
	isolevel_brush_flow_container->add_child(_isolevel_brush_type_add_button);

	_isolevel_brush_type_substract_button = memnew(ToolButton);
	_isolevel_brush_type_substract_button->set_toggle_mode(true);
	_isolevel_brush_type_substract_button->set_button_group(_isolevel_brush_tool_button_group);
	_isolevel_brush_type_substract_button->set_meta("type", ISOLEVEL_BRUSH_TYPE_SUBSTRACT);
	_isolevel_brush_type_substract_button->set_shortcut(ED_SHORTCUT("terrain_world_editor/isolevel_brush_type_substract", "Isolevel Brush Type Substract", KEY_H));
	isolevel_brush_flow_container->add_child(_isolevel_brush_type_substract_button);

	_isolevel_brush_type_set_button = memnew(ToolButton);
	_isolevel_brush_type_set_button->set_toggle_mode(true);
	_isolevel_brush_type_set_button->set_button_group(_isolevel_brush_tool_button_group);
	_isolevel_brush_type_set_button->set_meta("type", ISOLEVEL_BRUSH_TYPE_SET);
	_isolevel_brush_type_set_button->set_shortcut(ED_SHORTCUT("terrain_world_editor/isolevel_brush_type_set", "Isolevel Brush Type Set", KEY_J));
	isolevel_brush_flow_container->add_child(_isolevel_brush_type_set_button);

	//_isolevel_brush_type_smooth_button = memnew(ToolButton);
	//_isolevel_brush_type_smooth_button->set_toggle_mode(true);
	//_isolevel_brush_type_smooth_button->set_button_group(_isolevel_brush_tool_button_group);
	//_isolevel_brush_type_smooth_button->set_meta("type", ISOLEVEL_BRUSH_TYPE_SMOOTH);
	//_isolevel_brush_type_smooth_button->connect("pressed", this, "_on_isolevel_brush_tool_button_pressed");
	//_isolevel_brush_type_smooth_button->set_shortcut(ED_SHORTCUT("terrain_world_editor/isolevel_brush_type_smooth", "Isolevel Brush Type Smooth", KEY_K));
	//isolevel_brush_flow_container->add_child(_isolevel_brush_type_smooth_button);

	_isolevel_brush_liquid_mode_button = memnew(ToolButton);
	_isolevel_brush_liquid_mode_button->set_toggle_mode(true);
	_isolevel_brush_liquid_mode_button->connect("toggled", this, "_on_isolevel_brush_liquid_mode_selected");
	_isolevel_brush_liquid_mode_button->set_shortcut(ED_SHORTCUT("terrain_world_editor/isolevel_brush_liquid_mode", "Isolevel Brush Liquid Mode", KEY_L));
	isolevel_brush_flow_container->add_child(_isolevel_brush_liquid_mode_button);

	_isolevel_brush_allow_creating_chunks_button = memnew(ToolButton);
	_isolevel_brush_allow_creating_chunks_button->set_toggle_mode(true);
	_isolevel_brush_allow_creating_chunks_button->connect("toggled", this, "_on_isolevel_brush_allow_creating_chunks_selected");
	_isolevel_brush_allow_creating_chunks_button->set_shortcut(ED_SHORTCUT("terrain_world_editor/isolevel_brush_allow_creating_chunks", "Isolevel Brush Allow Chunk Creation", KEY_N));
	isolevel_brush_flow_container->add_child(_isolevel_brush_allow_creating_chunks_button);

	Label *isolevel_brush_size_label = memnew(Label);
	isolevel_brush_size_label->set_text(TTR("Size"));
	_isolevel_brush_tool_container->add_child(isolevel_brush_size_label);

	_isolevel_brush_size_slider = memnew(HSlider);
	_isolevel_brush_size_slider->set_min(1);
	_isolevel_brush_size_slider->set_max(100);
	_isolevel_brush_size_slider->set_value(_isolevel_brush_size);
	_isolevel_brush_size_slider->set_v_size_flags(SIZE_EXPAND_FILL);
	_isolevel_brush_size_slider->set_tooltip(TTR("Brush Size"));
	_isolevel_brush_size_slider->connect("value_changed", this, "_on_isolevel_brush_size_slider_changed");
	_isolevel_brush_tool_container->add_child(_isolevel_brush_size_slider);

	Label *isolevel_brush_strength_label = memnew(Label);
	isolevel_brush_strength_label->set_text(TTR("Strength"));
	_isolevel_brush_tool_container->add_child(isolevel_brush_strength_label);

	_isolevel_brush_strength_slider = memnew(HSlider);
	_isolevel_brush_strength_slider->set_min(0);
	_isolevel_brush_strength_slider->set_max(1);
	_isolevel_brush_strength_slider->set_step(0);
	_isolevel_brush_strength_slider->set_value(_isolevel_brush_strength);
	_isolevel_brush_strength_slider->set_v_size_flags(SIZE_EXPAND_FILL);
	_isolevel_brush_strength_slider->set_tooltip(TTR("Brush Strength"));
	_isolevel_brush_strength_slider->connect("value_changed", this, "_on_isolevel_brush_strength_slider_changed");
	_isolevel_brush_tool_container->add_child(_isolevel_brush_strength_slider);

	Label *isolevel_brush_smoothness_label = memnew(Label);
	isolevel_brush_smoothness_label->set_text(TTR("Smoothness"));
	_isolevel_brush_tool_container->add_child(isolevel_brush_smoothness_label);

	_isolevel_brush_smoothness_slider = memnew(HSlider);
	_isolevel_brush_smoothness_slider->set_min(0);
	_isolevel_brush_smoothness_slider->set_max(1);
	_isolevel_brush_smoothness_slider->set_step(0);
	_isolevel_brush_smoothness_slider->set_value(_isolevel_brush_smoothness);
	_isolevel_brush_smoothness_slider->set_v_size_flags(SIZE_EXPAND_FILL);
	_isolevel_brush_smoothness_slider->set_tooltip(TTR("Brush Smoothness"));
	_isolevel_brush_smoothness_slider->connect("value_changed", this, "_on_isolevel_brush_smoothness_slider_changed");
	_isolevel_brush_tool_container->add_child(_isolevel_brush_smoothness_slider);

	// Isolevel Brush Channel Select
	Label *isolevel_brush_channel_label = memnew(Label);
	isolevel_brush_channel_label->set_text(TTR("Channel"));
	_isolevel_brush_tool_container->add_child(isolevel_brush_channel_label);

	_isolevel_brush_channel_select_sb = memnew(SpinBox);
	_isolevel_brush_channel_select_sb->set_v_size_flags(SIZE_EXPAND_FILL);
	_isolevel_brush_channel_select_sb->set_tooltip(TTR("Isolevel Channel"));
	_isolevel_brush_channel_select_sb->connect("value_changed", this, "_on_isolevel_brush_channel_select_sb_changed");
	_isolevel_brush_tool_container->add_child(_isolevel_brush_channel_select_sb);

	// Paint Brush
	_paint_brush_tool_container = memnew(VBoxContainer);
	_paint_brush_tool_container->hide();
	main_container->add_child(_paint_brush_tool_container);

	Label *paint_brush_label = memnew(Label);
	paint_brush_label->set_align(Label::ALIGN_CENTER);
	paint_brush_label->set_text(TTR("Paint Brush"));
	_paint_brush_tool_container->add_child(paint_brush_label);

	HFlowContainer *paint_brush_flow_container = memnew(HFlowContainer);
	_paint_brush_tool_container->add_child(paint_brush_flow_container);

	_paint_brush_liquid_mode_button = memnew(ToolButton);
	_paint_brush_liquid_mode_button->set_toggle_mode(true);
	_paint_brush_liquid_mode_button->connect("toggled", this, "_on_paint_brush_liquid_mode_selected");
	_paint_brush_liquid_mode_button->set_shortcut(ED_SHORTCUT("terrain_world_editor/paint_brush_liquid_mode", "Paint Brush Liquid Mode", KEY_L));
	paint_brush_flow_container->add_child(_paint_brush_liquid_mode_button);

	_paint_brush_allow_creating_chunks_button = memnew(ToolButton);
	_paint_brush_allow_creating_chunks_button->set_toggle_mode(true);
	_paint_brush_allow_creating_chunks_button->connect("toggled", this, "_on_paint_brush_allow_creating_chunks_selected");
	_paint_brush_allow_creating_chunks_button->set_shortcut(ED_SHORTCUT("terrain_world_editor/paint_brush_allow_creating_chunks", "Paint Brush Allow Chunk Creation", KEY_L));
	paint_brush_flow_container->add_child(_paint_brush_allow_creating_chunks_button);

	Label *paint_brush_size_label = memnew(Label);
	paint_brush_size_label->set_text(TTR("Size"));
	_paint_brush_tool_container->add_child(paint_brush_size_label);

	_paint_brush_size_slider = memnew(HSlider);
	_paint_brush_size_slider->set_min(1);
	_paint_brush_size_slider->set_max(100);
	_paint_brush_size_slider->set_value(_isolevel_brush_size);
	_paint_brush_size_slider->set_v_size_flags(SIZE_EXPAND_FILL);
	_paint_brush_size_slider->set_tooltip(TTR("Brush Size"));
	_paint_brush_size_slider->connect("value_changed", this, "_on_paint_brush_size_slider_changed");
	_paint_brush_tool_container->add_child(_paint_brush_size_slider);

	Label *paint_brush_channel_label = memnew(Label);
	paint_brush_channel_label->set_text(TTR("Channel"));
	_paint_brush_tool_container->add_child(paint_brush_channel_label);

	_paint_brush_channel_select_sb = memnew(SpinBox);
	_paint_brush_channel_select_sb->set_v_size_flags(SIZE_EXPAND_FILL);
	_paint_brush_channel_select_sb->set_tooltip(TTR("Channel"));
	_paint_brush_channel_select_sb->connect("value_changed", this, "_on_paint_brush_channel_select_sb_changed");
	_paint_brush_tool_container->add_child(_paint_brush_channel_select_sb);

	// Paint Picker Tool
	_paint_picker_tool_container = memnew(VBoxContainer);
	_paint_picker_tool_container->hide();
	main_container->add_child(_paint_picker_tool_container);

	Label *paint_picker_label = memnew(Label);
	paint_picker_label->set_text(TTR("Paint Picker"));
	paint_picker_label->set_align(Label::ALIGN_CENTER);
	_paint_picker_tool_container->add_child(paint_picker_label);

	paint_picker_label = memnew(Label);
	paint_picker_label->set_text(TTR("Target Tool:"));
	_paint_picker_tool_container->add_child(paint_picker_label);

	_paint_picker_tool_label = memnew(Label);
	_paint_picker_tool_label->set_align(Label::ALIGN_CENTER);
	_paint_picker_tool_container->add_child(_paint_picker_tool_label);

	paint_picker_label = memnew(Label);
	paint_picker_label->set_text(TTR("Target Channel:"));
	_paint_picker_tool_container->add_child(paint_picker_label);

	_paint_picker_channel_label = memnew(Label);
	_paint_picker_channel_label->set_align(Label::ALIGN_CENTER);
	_paint_picker_tool_container->add_child(_paint_picker_channel_label);

	// Chunk Spawn Brush Tool
	_chunk_spawn_brush_tool_container = memnew(VBoxContainer);
	_chunk_spawn_brush_tool_container->hide();
	main_container->add_child(_chunk_spawn_brush_tool_container);

	Label *chunk_spawn_brush_label = memnew(Label);
	chunk_spawn_brush_label->set_align(Label::ALIGN_CENTER);
	chunk_spawn_brush_label->set_text(TTR("Chunk Spawn Brush"));
	_chunk_spawn_brush_tool_container->add_child(chunk_spawn_brush_label);

	Label *chunk_spawn_brush_size_label = memnew(Label);
	chunk_spawn_brush_size_label->set_text(TTR("Size"));
	_chunk_spawn_brush_tool_container->add_child(chunk_spawn_brush_size_label);

	_chunk_spawn_brush_size_slider = memnew(HSlider);
	_chunk_spawn_brush_size_slider->set_min(1);
	_chunk_spawn_brush_size_slider->set_max(100);
	_chunk_spawn_brush_size_slider->set_value(_chunk_spawn_brush_size);
	_chunk_spawn_brush_size_slider->set_v_size_flags(SIZE_EXPAND_FILL);
	_chunk_spawn_brush_size_slider->set_tooltip(TTR("Brush Size"));
	_chunk_spawn_brush_size_slider->connect("value_changed", this, "_on_chunk_spawn_brush_size_slider_changed");
	_chunk_spawn_brush_tool_container->add_child(_chunk_spawn_brush_size_slider);

	// Chunk Remove Brush Tool
	_chunk_remove_brush_tool_container = memnew(VBoxContainer);
	_chunk_remove_brush_tool_container->hide();
	main_container->add_child(_chunk_remove_brush_tool_container);

	Label *chunk_remove_brush_label = memnew(Label);
	chunk_remove_brush_label->set_align(Label::ALIGN_CENTER);
	chunk_remove_brush_label->set_text(TTR("Chunk Remove Brush"));
	_chunk_remove_brush_tool_container->add_child(chunk_remove_brush_label);

	Label *chunk_remove_brush_size_label = memnew(Label);
	chunk_remove_brush_size_label->set_text(TTR("Size"));
	_chunk_remove_brush_tool_container->add_child(chunk_remove_brush_size_label);

	_chunk_remove_brush_size_slider = memnew(HSlider);
	_chunk_remove_brush_size_slider->set_min(1);
	_chunk_remove_brush_size_slider->set_max(100);
	_chunk_remove_brush_size_slider->set_value(_chunk_remove_brush_size);
	_chunk_remove_brush_size_slider->set_v_size_flags(SIZE_EXPAND_FILL);
	_chunk_remove_brush_size_slider->set_tooltip(TTR("Brush Size"));
	_chunk_remove_brush_size_slider->connect("value_changed", this, "_on_chunk_remove_brush_size_slider_changed");
	_chunk_remove_brush_tool_container->add_child(_chunk_remove_brush_size_slider);

	// Baking Tools
	_baking_tools_tool_container = memnew(VBoxContainer);
	_baking_tools_tool_container->hide();
	main_container->add_child(_baking_tools_tool_container);

	Label *baking_tools_label = memnew(Label);
	baking_tools_label->set_align(Label::ALIGN_CENTER);
	baking_tools_label->set_text(TTR("Baking Tools"));
	_baking_tools_tool_container->add_child(baking_tools_label);

	// Baking Tools - Scenes
	Label *bake_scenes_label = memnew(Label);
	bake_scenes_label->set_align(Label::ALIGN_CENTER);
	bake_scenes_label->set_text(TTR("Scenes"));
	_baking_tools_tool_container->add_child(bake_scenes_label);

	Button *bake_scenes_button = memnew(Button);
	bake_scenes_button->set_text(TTR("Bake Scenes"));
	bake_scenes_button->set_tooltip(TTR("Takes the current world's direct child instances scenes, adds them directly as scenes and deletes them.\nNode names will be remembered.\nIf the Editable children option is set then the scene will be skipped.\nPlease note that property overrides ar not supported, and ignored currently."));
	bake_scenes_button->connect("pressed", this, "_on_bake_scenes_button_pressed");
	_baking_tools_tool_container->add_child(bake_scenes_button);

	Button *un_bake_scenes_button = memnew(Button);
	un_bake_scenes_button->set_text(TTR("Un-Bake Scenes"));
	un_bake_scenes_button->set_tooltip(TTR("Takes the scenes stored in the current world, instances them, and removes them from world."));
	un_bake_scenes_button->connect("pressed", this, "_on_un_bake_scenes_button_pressed");
	_baking_tools_tool_container->add_child(un_bake_scenes_button);

#ifdef MODULE_PROPS_ENABLED
	// Baking Tools - Props
	Label *bake_props_label = memnew(Label);
	bake_props_label->set_align(Label::ALIGN_CENTER);
	bake_props_label->set_text(TTR("Props"));
	_baking_tools_tool_container->add_child(bake_props_label);

	Button *bake_props_button = memnew(Button);
	bake_props_button->set_text(TTR("Bake Props"));
	bake_props_button->set_tooltip(TTR("Takes the current world's direct child PropInstanceMergers, adds them directly as props and delets them.\nThe ones that have a script attached will be skipped. Node names will be remembered."));
	bake_props_button->connect("pressed", this, "_on_bake_props_button_pressed");
	_baking_tools_tool_container->add_child(bake_props_button);

	Button *un_bake_props_button = memnew(Button);
	un_bake_props_button->set_text(TTR("Un-Bake Props"));
	un_bake_props_button->set_tooltip(TTR("Takes the props stored in the current world, makes PropInstanceMergers from them, and removes them from world."));
	un_bake_props_button->connect("pressed", this, "_on_un_bake_props_button_pressed");
	_baking_tools_tool_container->add_child(un_bake_props_button);
#endif

	// Surface Selector Separator
	main_container->add_child(memnew(HSeparator));

	// Surface selector
	ScrollContainer *scs = memnew(ScrollContainer);
	scs->set_h_size_flags(SIZE_EXPAND_FILL);
	scs->set_v_size_flags(SIZE_EXPAND_FILL);
	scs->set_name("Surfaces");
	main_container->add_child(scs);

	_surfaces_vbox_container = memnew(VBoxContainer);
	_surfaces_vbox_container->hide();
	scs->add_child(_surfaces_vbox_container);
	_surfaces_vbox_container->set_h_size_flags(SIZE_EXPAND_FILL);

	_surfaces_button_group.instance();
	_surfaces_button_group->connect("pressed", this, "_on_surface_button_pressed");
}
TerrainWorldEditor::~TerrainWorldEditor() {
	_world = NULL;

	_surfaces_button_group.unref();
}

void TerrainWorldEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE:
		case NOTIFICATION_THEME_CHANGED: {
			_isolevel_brush_button->set_icon(get_theme_icon("CanvasItem", "EditorIcons"));
			_paint_brush_button->set_icon(get_theme_icon("CanvasItemShader", "EditorIcons"));
			_paint_picker_button->set_icon(get_theme_icon("ColorPick", "EditorIcons"));

			// Isolevel Brush
			_isolevel_brush_type_add_button->set_icon(get_theme_icon("MoveUp", "EditorIcons"));
			_isolevel_brush_type_substract_button->set_icon(get_theme_icon("MoveDown", "EditorIcons"));
			_isolevel_brush_type_set_button->set_icon(get_theme_icon("CanvasLayer", "EditorIcons"));
			//_isolevel_brush_type_smooth_button->set_icon(get_theme_icon("Blend", "EditorIcons"));
			_isolevel_brush_allow_creating_chunks_button->set_icon(get_theme_icon("Add", "EditorIcons"));
			_isolevel_brush_liquid_mode_button->set_icon(get_theme_icon("CylinderShape", "EditorIcons"));

			// Paint Brush
			_paint_brush_allow_creating_chunks_button->set_icon(get_theme_icon("Add", "EditorIcons"));
			_paint_brush_liquid_mode_button->set_icon(get_theme_icon("CylinderShape", "EditorIcons"));

			// Spawn Brush
			_chunk_spawn_brush_button->set_icon(get_theme_icon("AssetLib", "EditorIcons"));

			// Chunk Remove
			_chunk_remove_brush_button->set_icon(get_theme_icon("Clear", "EditorIcons"));

			// Baking Tools
			_baking_tools_button->set_icon(get_theme_icon("Bake", "EditorIcons"));
		} break;
	}
}

void TerrainWorldEditor::_node_removed(Node *p_node) {
	if (p_node == _world) {
		_world = NULL;
		_gizmo.unref();
	}
}

Ref<TerrainWorldGizmo> TerrainWorldEditor::get_gizmo_from(TerrainWorld *w) {
	if (!w) {
		return Ref<TerrainWorldGizmo>();
	}

	Vector<Ref<SpatialGizmo>> gizmos = w->get_gizmos();

	for (int i = 0; i < gizmos.size(); ++i) {
		Ref<TerrainWorldGizmo> g = gizmos[i];

		if (g.is_valid()) {
			return g;
		}
	}

	return Ref<TerrainWorldGizmo>();
}

void TerrainWorldEditor::apply_data(const Array &p_data) {
	ERR_FAIL_COND(p_data.size() != 4);

	ObjectID wid = p_data[0];

	TerrainWorld *world = Object::cast_to<TerrainWorld>(ObjectDB::get_instance(wid));

	if (!world) {
		return;
	}

	int channel = p_data[1];
	bool allow_create_chunks = p_data[2];
	Array data = p_data[3];

	_world->set_voxels_at_world_data_position(data, channel, true, allow_create_chunks);
}

void TerrainWorldEditor::do_chunk_added_action(const Array &p_data) {
	ERR_FAIL_COND(p_data.size() != 2);

	ObjectID wid = p_data[0];

	TerrainWorld *world = Object::cast_to<TerrainWorld>(ObjectDB::get_instance(wid));

	if (!world) {
		return;
	}

	Array data = p_data[1];

	for (int i = 0; i < data.size(); i += 2) {
		Vector2i chunk_position = data[i];
		Ref<TerrainChunk> chunk = Ref<TerrainChunk>(data[i + 1]);

		if (!world->chunk_has(chunk_position.x, chunk_position.y)) {
			world->chunk_add(chunk, chunk_position.x, chunk_position.y);
		}
	}
}

void TerrainWorldEditor::undo_chunk_added_action(const Array &p_data) {
	ERR_FAIL_COND(p_data.size() != 2);

	ObjectID wid = p_data[0];

	TerrainWorld *world = Object::cast_to<TerrainWorld>(ObjectDB::get_instance(wid));

	if (!world) {
		return;
	}

	Array data = p_data[1];

	for (int i = 0; i < data.size(); i += 2) {
		Vector2i chunk_position = data[i];
		Ref<TerrainChunk> chunk = Ref<TerrainChunk>(data[i + 1]);

		if (world->chunk_has(chunk_position.x, chunk_position.y)) {
			world->chunk_remove(chunk_position.x, chunk_position.y);
		}
	}
}

void TerrainWorldEditor::do_chunk_removed_action(const Array &p_data) {
	ERR_FAIL_COND(p_data.size() != 2);

	ObjectID wid = p_data[0];

	TerrainWorld *world = Object::cast_to<TerrainWorld>(ObjectDB::get_instance(wid));

	if (!world) {
		return;
	}

	Array data = p_data[1];

	for (int i = 0; i < data.size(); i += 2) {
		Vector2i chunk_position = data[i];
		Ref<TerrainChunk> chunk = Ref<TerrainChunk>(data[i + 1]);

		if (world->chunk_has(chunk_position.x, chunk_position.y)) {
			world->chunk_remove(chunk_position.x, chunk_position.y);
		}
	}
}

void TerrainWorldEditor::undo_chunk_removed_action(const Array &p_data) {
	ERR_FAIL_COND(p_data.size() != 2);

	ObjectID wid = p_data[0];

	TerrainWorld *world = Object::cast_to<TerrainWorld>(ObjectDB::get_instance(wid));

	if (!world) {
		return;
	}

	Array data = p_data[1];

	for (int i = 0; i < data.size(); i += 2) {
		Vector2i chunk_position = data[i];
		Ref<TerrainChunk> chunk = Ref<TerrainChunk>(data[i + 1]);

		if (!world->chunk_has(chunk_position.x, chunk_position.y)) {
			world->chunk_add(chunk, chunk_position.x, chunk_position.y);
		}
	}
}

void TerrainWorldEditor::create_undo_point(const String &p_action, const int p_channel, const bool p_allow_create_chunks) {
	if (!_world) {
		return;
	}

	ObjectID wid = _world->get_instance_id();

	Array arr_do;
	arr_do.push_back(wid);
	arr_do.push_back(p_channel);
	arr_do.push_back(p_allow_create_chunks);

	Array arr_undo;
	arr_undo.push_back(wid);
	arr_undo.push_back(p_channel);
	arr_undo.push_back(p_allow_create_chunks);

	Array arr_do_data;

	for (HashMap<Vector2i, uint8_t>::Element *E = _current_data.front(); E; E = E->next) {
		arr_do_data.push_back(E->key());
		arr_do_data.push_back(E->value());
	}

	Array arr_undo_data;

	for (HashMap<Vector2i, uint8_t>::Element *E = _original_data.front(); E; E = E->next) {
		arr_undo_data.push_back(E->key());
		arr_undo_data.push_back(E->value());
	}

	arr_do.push_back(arr_do_data);
	arr_undo.push_back(arr_undo_data);

	_undo_redo->create_action(p_action);
	_undo_redo->add_do_method(this, "apply_data", arr_do);
	_undo_redo->add_undo_method(this, "apply_data", arr_undo);
	_undo_redo->commit_action();

	_original_data.clear();
	_current_data.clear();
}

void TerrainWorldEditor::create_chunk_created_undo_point() {
	if (!_world) {
		return;
	}

	ObjectID wid = _world->get_instance_id();

	Array arr_do;
	arr_do.push_back(wid);

	Array arr_undo;
	arr_undo.push_back(wid);

	Array arr_do_data;

	for (HashMap<Vector2i, Ref<TerrainChunk>>::Element *E = _created_chunks.front(); E; E = E->next) {
		arr_do_data.push_back(E->key());
		arr_do_data.push_back(E->value().get_ref_ptr());
	}

	Array arr_undo_data;

	for (HashMap<Vector2i, Ref<TerrainChunk>>::Element *E = _created_chunks.front(); E; E = E->next) {
		arr_undo_data.push_back(E->key());
		arr_undo_data.push_back(E->value().get_ref_ptr());
	}

	arr_do.push_back(arr_do_data);
	arr_undo.push_back(arr_undo_data);

	_undo_redo->create_action("Chunk Spawn Brush Draw");
	_undo_redo->add_do_method(this, "do_chunk_added_action", arr_do);
	_undo_redo->add_undo_method(this, "undo_chunk_added_action", arr_undo);
	_undo_redo->commit_action();

	_created_chunks.clear();
}

void TerrainWorldEditor::bake_props(const ObjectID p_world) {
#ifdef MODULE_PROPS_ENABLED
	TerrainWorld *world = ObjectDB::get_instance<TerrainWorld>(p_world);

	if (!world) {
		return;
	}

	int baked_props_count = 0;
	int skipped_props_count = 0;

	for (int i = 0; i < world->get_child_count(); ++i) {
		PropInstanceMerger *prop_instance = Object::cast_to<PropInstanceMerger>(world->get_child(i));

		if (!prop_instance) {
			continue;
		}

		if (!prop_instance->get_script().is_null()) {
			++skipped_props_count;
			continue;
		}

		Ref<PropData> pd = prop_instance->get_prop_data();
		Transform t = prop_instance->get_global_transform();
		String name = prop_instance->get_name();

		prop_instance->queue_delete();

		++baked_props_count;

		if (!pd.is_valid()) {
			continue;
		}

		world->prop_add(t, pd, false, true, name);
	}

	String results = "Baked " + String::num_int64(baked_props_count) + " direct child props into TerrainWorld. ";
	results += String::num_int64(skipped_props_count) + "were skipped (they had scripts).";

	print_line(results);
#endif
}
void TerrainWorldEditor::un_bake_props(const ObjectID p_world) {
#ifdef MODULE_PROPS_ENABLED
	TerrainWorld *world = ObjectDB::get_instance<TerrainWorld>(p_world);

	if (!world) {
		return;
	}

	int un_baked_props_count = 0;

	for (int i = 0; i < world->chunk_get_count(); ++i) {
		Ref<TerrainChunk> chunk = world->chunk_get_index(i);

		for (int j = 0; j < chunk->prop_get_count(); ++j) {
			if (!chunk->prop_get_is_original(j)) {
				continue;
			}

			Ref<PropData> pd = chunk->prop_get(j);
			Transform t = chunk->prop_get_transform(j);
			String name = chunk->prop_get_name(j);

			PropInstanceMerger *prop_instance = memnew(PropInstanceMerger);

			prop_instance->set_global_transform(t);
			prop_instance->set_prop_data(pd);
			if (!name.empty()) {
				prop_instance->set_name(name);
			}

			world->add_child(prop_instance);
			prop_instance->set_owner(_editor->get_edited_scene());

			++un_baked_props_count;
		}

		chunk->props_clear();

		for (int j = 0; j < chunk->light_get_count(); ++j) {
			Ref<TerrainLight> light = chunk->light_get_index(j);

			if (light->get_owner_type() == TerrainLight::OWNER_TYPE_PROP) {
				chunk->light_remove_index(j);
				--j;
			}
		}

		for (int j = 0; j < chunk->scene_get_count(); ++j) {
			if (!chunk->scene_get_is_original(j)) {
				chunk->scene_remove(j);
				--j;
			}
		}

#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
		for (int j = 0; j < chunk->mesh_data_resource_get_count(); ++j) {
			if (!chunk->mesh_data_resource_get_is_original(j)) {
				chunk->mesh_data_resource_remove(j);
				--j;
			}
		}
#endif

		chunk->build();
	}

	String results = "Un-Baked " + String::num_int64(un_baked_props_count) + " direct child props into TerrainWorld.";

	print_line(results);
#endif
}

void TerrainWorldEditor::bake_scenes(const ObjectID p_world) {
	TerrainWorld *world = ObjectDB::get_instance<TerrainWorld>(p_world);

	if (!world) {
		return;
	}

	int baked_scene_count = 0;
	int skipped_editable_scene_count = 0;
	int skipped_scene_count = 0;

	for (int i = 0; i < world->get_child_count(); ++i) {
		Node *n = world->get_child(i);

		if (n->get_filename().empty()) {
			continue;
		}

		bool editable = EditorNode::get_singleton()->get_edited_scene()->is_editable_instance(n);

		if (editable) {
			++skipped_editable_scene_count;
			continue;
		}

		Ref<PackedScene> scene = ResourceLoader::load(n->get_filename(), "PackedScene");

		if (!scene.is_valid()) {
			++skipped_scene_count;
			continue;
		}

		String name = n->get_name();

		Transform t;
		Spatial *sp = Object::cast_to<Spatial>(n);
		if (sp) {
			t = sp->get_global_transform();
		}

		n->set_owner(NULL);

		++baked_scene_count;

		world->scene_add(scene, t, n, true, name);
	}

	String results = "Baked " + String::num_int64(baked_scene_count) + " direct child scenes into TerrainWorld.\n";
	results += String::num_int64(skipped_editable_scene_count) + "were skipped because they were editable,\n";
	results += String::num_int64(skipped_scene_count) + "were skipped due to other reasons.";

	print_line(results);
}
void TerrainWorldEditor::un_bake_scenes(const ObjectID p_world) {
	TerrainWorld *world = ObjectDB::get_instance<TerrainWorld>(p_world);

	if (!world) {
		return;
	}

	int un_baked_scenes_count = 0;

	for (int i = 0; i < world->chunk_get_count(); ++i) {
		Ref<TerrainChunk> chunk = world->chunk_get_index(i);

		for (int j = 0; j < chunk->scene_get_count(); ++j) {
			if (!chunk->scene_get_is_original(j)) {
				continue;
			}

			Ref<PackedScene> pd = chunk->scene_get(j);
			Transform t = chunk->prop_get_transform(j);
			String name = chunk->scene_get_name(j);
			Node *n = chunk->scene_get_node(j);

			if (n) {
				chunk->scene_remove(j, false);
				--j;
				n->set_owner(_editor->get_edited_scene());
			} else {
				chunk->scene_remove(j);
				--j;

				ERR_CONTINUE(!pd.is_valid());

				n = pd->instance();

				Spatial *sp = Object::cast_to<Spatial>(n);

				if (sp) {
					sp->set_global_transform(t);
				}

				if (!name.empty()) {
					n->set_name(name);
				}

				world->add_child(n);
				n->set_owner(_editor->get_edited_scene());
			}

			++un_baked_scenes_count;
		}
	}

	String results = "Un-Baked " + String::num_int64(un_baked_scenes_count) + " direct child scenes from TerrainWorld.";

	print_line(results);
}

void TerrainWorldEditor::create_chunk_removed_undo_point() {
	if (!_world) {
		return;
	}

	ObjectID wid = _world->get_instance_id();

	Array arr_do;
	arr_do.push_back(wid);

	Array arr_undo;
	arr_undo.push_back(wid);

	Array arr_do_data;

	for (HashMap<Vector2i, Ref<TerrainChunk>>::Element *E = _removed_chunks.front(); E; E = E->next) {
		arr_do_data.push_back(E->key());
		arr_do_data.push_back(E->value().get_ref_ptr());
	}

	Array arr_undo_data;

	for (HashMap<Vector2i, Ref<TerrainChunk>>::Element *E = _removed_chunks.front(); E; E = E->next) {
		arr_undo_data.push_back(E->key());
		arr_undo_data.push_back(E->value().get_ref_ptr());
	}

	arr_do.push_back(arr_do_data);
	arr_undo.push_back(arr_undo_data);

	_undo_redo->create_action("Chunk Remove Brush Draw");
	_undo_redo->add_do_method(this, "do_chunk_removed_action", arr_do);
	_undo_redo->add_undo_method(this, "undo_chunk_removed_action", arr_undo);
	_undo_redo->commit_action();

	_removed_chunks.clear();
}

void TerrainWorldEditor::_on_surface_button_pressed(Object *p_button) {
	BaseButton *button = Object::cast_to<BaseButton>(p_button);

	if (button) {
		_selected_type = button->get_meta("index");
	}
}

void TerrainWorldEditor::_on_tool_button_pressed(Object *p_button) {
	BaseButton *button = Object::cast_to<BaseButton>(p_button);

	if (!button) {
		return;
	}

	TerrainWorldEditorToolMode new_mode = static_cast<TerrainWorldEditorToolMode>(static_cast<int>(button->get_meta("tool_mode")));

	if (new_mode == _tool_mode) {
		return;
	}

	_previous_tool_mode = _tool_mode;
	_tool_mode = new_mode;

	switch (_tool_mode) {
		case TOOL_MODE_PAINT_BRUSH:
			_gizmo->size = _paint_brush_size;

			_isolevel_brush_tool_container->hide();
			_paint_brush_tool_container->show();
			_surfaces_vbox_container->show();
			_paint_picker_tool_container->hide();
			_chunk_spawn_brush_tool_container->hide();
			_chunk_remove_brush_tool_container->hide();
			_baking_tools_tool_container->hide();

			break;
		case TOOL_MODE_ISOLEVEL_BRUSH:
			_gizmo->size = _isolevel_brush_size;

			_isolevel_brush_tool_container->show();
			_paint_brush_tool_container->hide();
			_surfaces_vbox_container->hide();
			_paint_picker_tool_container->hide();
			_chunk_spawn_brush_tool_container->hide();
			_chunk_remove_brush_tool_container->hide();
			_baking_tools_tool_container->hide();

			break;
		case TOOL_MODE_PAINT_PICKER:
			_gizmo->visible = false;
			_gizmo->redraw();

			_isolevel_brush_tool_container->hide();
			_paint_brush_tool_container->hide();
			_surfaces_vbox_container->hide();
			_paint_picker_tool_container->show();
			_chunk_spawn_brush_tool_container->hide();
			_chunk_remove_brush_tool_container->hide();
			_baking_tools_tool_container->hide();

			if (_previous_tool_mode == TOOL_MODE_ISOLEVEL_BRUSH) {
				_paint_picker_tool_label->set_text(TTR("Isolevel Brush"));
				_paint_picker_channel_label->set_text(String::num(_isolevel_brush_channel));

			} else {
				_paint_picker_tool_label->set_text(TTR("Paint Brush"));
				_paint_picker_channel_label->set_text(String::num(_paint_brush_channel));
			}

			break;
		case TOOL_MODE_CHUNK_SPAWN_BRUSH:
			_gizmo->size = _chunk_spawn_brush_size;

			_isolevel_brush_tool_container->hide();
			_paint_brush_tool_container->hide();
			_surfaces_vbox_container->hide();
			_paint_picker_tool_container->hide();
			_chunk_spawn_brush_tool_container->show();
			_chunk_remove_brush_tool_container->hide();
			_baking_tools_tool_container->hide();

			break;
		case TOOL_MODE_CHUNK_REMOVE_BRUSH:
			_gizmo->size = _chunk_remove_brush_size;

			_isolevel_brush_tool_container->hide();
			_paint_brush_tool_container->hide();
			_surfaces_vbox_container->hide();
			_paint_picker_tool_container->hide();
			_chunk_spawn_brush_tool_container->hide();
			_chunk_remove_brush_tool_container->show();
			_baking_tools_tool_container->hide();

			break;
		case TOOL_MODE_BAKING_TOOLS:
			_gizmo->visible = false;
			_gizmo->redraw();

			_isolevel_brush_tool_container->hide();
			_paint_brush_tool_container->hide();
			_surfaces_vbox_container->hide();
			_paint_picker_tool_container->hide();
			_chunk_spawn_brush_tool_container->hide();
			_chunk_remove_brush_tool_container->hide();
			_baking_tools_tool_container->show();

			break;
		default:
			_gizmo->visible = false;
			_gizmo->redraw();

			break;
	}
}

void TerrainWorldEditor::_on_isolevel_brush_tool_button_pressed(Object *p_button) {
	BaseButton *button = Object::cast_to<BaseButton>(p_button);

	if (button) {
		_isolevel_brush_type = static_cast<IsolevelBrushType>(static_cast<int>(button->get_meta("type")));
	}
}

void TerrainWorldEditor::_on_isolevel_brush_size_slider_changed(float value) {
	_isolevel_brush_size = value;

	if (_gizmo.is_valid()) {
		_gizmo->size = _isolevel_brush_size;
	}
}
void TerrainWorldEditor::_on_isolevel_brush_strength_slider_changed(float value) {
	_isolevel_brush_strength = value;
}
void TerrainWorldEditor::_on_isolevel_brush_smoothness_slider_changed(float value) {
	_isolevel_brush_smoothness = value;
}

void TerrainWorldEditor::_on_isolevel_brush_channel_select_sb_changed(int value) {
	_isolevel_brush_channel = value;
}

void TerrainWorldEditor::_on_isolevel_brush_allow_creating_chunks_selected(bool p_on) {
	_isolevel_brush_allow_create_chunks = p_on;
}

void TerrainWorldEditor::_on_isolevel_brush_liquid_mode_selected(bool p_on) {
	_isolevel_brush_liquid_mode = p_on;

	if (_world) {
		if (_isolevel_brush_liquid_mode) {
			_channel_isolevel = _world->get_channel_index_info(TerrainWorld::CHANNEL_TYPE_INFO_LIQUID_ISOLEVEL);
		} else {
			_channel_isolevel = _world->get_channel_index_info(TerrainWorld::CHANNEL_TYPE_INFO_ISOLEVEL);
		}

		_isolevel_brush_channel_select_sb->set_value(_channel_isolevel);
		_isolevel_brush_channel = _channel_isolevel;
	}
}

void TerrainWorldEditor::_on_paint_brush_liquid_mode_selected(bool p_on) {
	_paint_brush_liquid_mode = p_on;

	if (_world) {
		if (_paint_brush_liquid_mode) {
			_channel_type = _world->get_channel_index_info(TerrainWorld::CHANNEL_TYPE_INFO_LIQUID_TYPE);
		} else {
			_channel_type = _world->get_channel_index_info(TerrainWorld::CHANNEL_TYPE_INFO_TYPE);
		}

		_paint_brush_channel_select_sb->set_value(_channel_type);
		_paint_brush_channel = _channel_type;
	}
}

void TerrainWorldEditor::_on_paint_brush_allow_creating_chunks_selected(bool p_on) {
	_paint_brush_allow_create_chunks = p_on;
}
void TerrainWorldEditor::_on_paint_brush_size_slider_changed(float value) {
	_paint_brush_size = value;

	if (_gizmo.is_valid()) {
		_gizmo->size = _paint_brush_size;
	}
}
void TerrainWorldEditor::_on_paint_brush_channel_select_sb_changed(int value) {
	_paint_brush_channel = value;
}

void TerrainWorldEditor::_on_chunk_spawn_brush_size_slider_changed(float value) {
	_chunk_spawn_brush_size = value;

	if (_gizmo.is_valid()) {
		_gizmo->size = _chunk_spawn_brush_size;
	}
}
void TerrainWorldEditor::_on_chunk_remove_brush_size_slider_changed(float value) {
	_chunk_remove_brush_size = value;

	if (_gizmo.is_valid()) {
		_gizmo->size = _chunk_remove_brush_size;
	}
}

void TerrainWorldEditor::_on_bake_props_button_pressed() {
	if (!_world) {
		return;
	}

	ObjectID world = _world->get_instance_id();

	_undo_redo->create_action(TTR("Bake Props"));
	_undo_redo->add_do_method(this, "bake_props", world);
	_undo_redo->add_undo_method(this, "un_bake_props", world);
	_undo_redo->commit_action();
}
void TerrainWorldEditor::_on_un_bake_props_button_pressed() {
	if (!_world) {
		return;
	}

	ObjectID world = _world->get_instance_id();

	_undo_redo->create_action(TTR("Bake Props"));
	_undo_redo->add_do_method(this, "un_bake_props", world);
	_undo_redo->add_undo_method(this, "bake_props", world);
	_undo_redo->commit_action();
}

void TerrainWorldEditor::_on_bake_scenes_button_pressed() {
	if (!_world) {
		return;
	}

	ObjectID world = _world->get_instance_id();

	_undo_redo->create_action(TTR("Bake Scenes"));
	_undo_redo->add_do_method(this, "bake_scenes", world);
	_undo_redo->add_undo_method(this, "un_bake_scenes", world);
	_undo_redo->commit_action();
}
void TerrainWorldEditor::_on_un_bake_scenes_button_pressed() {
	if (!_world) {
		return;
	}

	ObjectID world = _world->get_instance_id();

	_undo_redo->create_action(TTR("Bake Scenes"));
	_undo_redo->add_do_method(this, "un_bake_scenes", world);
	_undo_redo->add_undo_method(this, "bake_scenes", world);
	_undo_redo->commit_action();
}

void TerrainWorldEditor::_bind_methods() {
	ClassDB::bind_method("_node_removed", &TerrainWorldEditor::_node_removed);

	ClassDB::bind_method("_on_surface_button_pressed", &TerrainWorldEditor::_on_surface_button_pressed);
	ClassDB::bind_method("_on_tool_button_pressed", &TerrainWorldEditor::_on_tool_button_pressed);

	ClassDB::bind_method("_on_isolevel_brush_tool_button_pressed", &TerrainWorldEditor::_on_isolevel_brush_tool_button_pressed);
	ClassDB::bind_method("_on_isolevel_brush_size_slider_changed", &TerrainWorldEditor::_on_isolevel_brush_size_slider_changed);
	ClassDB::bind_method("_on_isolevel_brush_strength_slider_changed", &TerrainWorldEditor::_on_isolevel_brush_strength_slider_changed);
	ClassDB::bind_method("_on_isolevel_brush_smoothness_slider_changed", &TerrainWorldEditor::_on_isolevel_brush_smoothness_slider_changed);
	ClassDB::bind_method("_on_isolevel_brush_channel_select_sb_changed", &TerrainWorldEditor::_on_isolevel_brush_channel_select_sb_changed);
	ClassDB::bind_method("_on_isolevel_brush_allow_creating_chunks_selected", &TerrainWorldEditor::_on_isolevel_brush_allow_creating_chunks_selected);
	ClassDB::bind_method("_on_isolevel_brush_liquid_mode_selected", &TerrainWorldEditor::_on_isolevel_brush_liquid_mode_selected);

	ClassDB::bind_method("_on_paint_brush_liquid_mode_selected", &TerrainWorldEditor::_on_paint_brush_liquid_mode_selected);
	ClassDB::bind_method("_on_paint_brush_allow_creating_chunks_selected", &TerrainWorldEditor::_on_paint_brush_allow_creating_chunks_selected);
	ClassDB::bind_method("_on_paint_brush_size_slider_changed", &TerrainWorldEditor::_on_paint_brush_size_slider_changed);
	ClassDB::bind_method("_on_paint_brush_channel_select_sb_changed", &TerrainWorldEditor::_on_paint_brush_channel_select_sb_changed);

	ClassDB::bind_method("_on_chunk_spawn_brush_size_slider_changed", &TerrainWorldEditor::_on_chunk_spawn_brush_size_slider_changed);
	ClassDB::bind_method("_on_chunk_remove_brush_size_slider_changed", &TerrainWorldEditor::_on_chunk_remove_brush_size_slider_changed);

	ClassDB::bind_method("apply_data", &TerrainWorldEditor::apply_data);

	ClassDB::bind_method("do_chunk_added_action", &TerrainWorldEditor::do_chunk_added_action);
	ClassDB::bind_method("undo_chunk_added_action", &TerrainWorldEditor::undo_chunk_added_action);
	ClassDB::bind_method("do_chunk_removed_action", &TerrainWorldEditor::do_chunk_removed_action);
	ClassDB::bind_method("undo_chunk_removed_action", &TerrainWorldEditor::undo_chunk_removed_action);

	ClassDB::bind_method("_on_bake_props_button_pressed", &TerrainWorldEditor::_on_bake_props_button_pressed);
	ClassDB::bind_method("_on_un_bake_props_button_pressed", &TerrainWorldEditor::_on_un_bake_props_button_pressed);

	ClassDB::bind_method("bake_props", &TerrainWorldEditor::bake_props);
	ClassDB::bind_method("un_bake_props", &TerrainWorldEditor::un_bake_props);

	ClassDB::bind_method("_on_bake_scenes_button_pressed", &TerrainWorldEditor::_on_bake_scenes_button_pressed);
	ClassDB::bind_method("_on_un_bake_scenes_button_pressed", &TerrainWorldEditor::_on_un_bake_scenes_button_pressed);

	ClassDB::bind_method("bake_scenes", &TerrainWorldEditor::bake_scenes);
	ClassDB::bind_method("un_bake_scenes", &TerrainWorldEditor::un_bake_scenes);
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
	TerrainWorld *ow = terrain_world_editor->get_edited_world();

	if (p_object == ow) {
		return;
	}

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

	gizmo_plugin.instance();
	gizmo_plugin->plugin = this;

	add_spatial_gizmo_plugin(gizmo_plugin);

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

	TerrainWorldEditorPlugin::make_visible(false);
}

TerrainWorldEditorPlugin::~TerrainWorldEditorPlugin() {
}
