/*************************************************************************/
/*  layered_tile_data_editors.cpp                                        */
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

#include "layered_tile_data_editors.h"

#include "layered_tile_set_editor.h"

#include "core/core_string_names.h"
#include "core/math/geometry.h"
#include "core/os/keyboard.h"

#include "editor/editor_node.h"
#include "editor/editor_properties.h"
#include "editor/editor_settings.h"

#include "core/object/undo_redo.h"
#include "editor/editor_scale.h"

#include "scene/gui/label.h"
#include "scene/gui/menu_button.h"
#include "scene/gui/option_button.h"
#include "scene/gui/panel.h"
#include "scene/gui/popup_menu.h"
#include "scene/gui/separator.h"
#include "scene/gui/spin_box.h"
#include "scene/main/control.h"

#include "editor/editor_property_name_processor.h"

#include "scene/resources/navigation_2d/navigation_mesh_source_geometry_data_2d.h"
#include "servers/navigation/navigation_mesh_generator.h"

#include "servers/navigation_2d_server.h"

void LayeredTileDataEditor::_tile_set_changed_plan_update() {
	_tile_set_changed_update_needed = true;

	call_deferred("_tile_set_changed_deferred_update");
}

void LayeredTileDataEditor::_tile_set_changed_deferred_update() {
	if (_tile_set_changed_update_needed) {
		_tile_set_changed();
		_tile_set_changed_update_needed = false;
	}
}

LayeredTileData *LayeredTileDataEditor::_get_tile_data(LayeredTileMapCell p_cell) {
	ERR_FAIL_COND_V(!tile_set.is_valid(), nullptr);
	ERR_FAIL_COND_V(!tile_set->has_source(p_cell.source_id), nullptr);

	LayeredTileData *td = nullptr;
	LayeredTileSetSource *source = *tile_set->get_source(p_cell.source_id);
	LayeredTileSetAtlasSource *atlas_source = Object::cast_to<LayeredTileSetAtlasSource>(source);
	if (atlas_source) {
		ERR_FAIL_COND_V(!atlas_source->has_tile(p_cell.get_atlas_coords()), nullptr);
		ERR_FAIL_COND_V(!atlas_source->has_alternative_tile(p_cell.get_atlas_coords(), p_cell.alternative_tile), nullptr);
		td = atlas_source->get_tile_data(p_cell.get_atlas_coords(), p_cell.alternative_tile);
	}

	return td;
}

void LayeredTileDataEditor::_bind_methods() {
	ADD_SIGNAL(MethodInfo("needs_redraw"));

	ClassDB::bind_method(D_METHOD("_tile_set_changed_deferred_update"), &LayeredTileDataEditor::_tile_set_changed_deferred_update);
	ClassDB::bind_method(D_METHOD("_tile_set_changed_plan_update"), &LayeredTileDataEditor::_tile_set_changed_plan_update);
	ClassDB::bind_method(D_METHOD("_tile_set_changed"), &LayeredTileDataEditor::_tile_set_changed);
}

void LayeredTileDataEditor::set_tile_set(Ref<LayeredTileSet> p_tile_set) {
	if (tile_set.is_valid()) {
		tile_set->disconnect(CoreStringNames::get_singleton()->changed, this, "_tile_set_changed_plan_update");
	}
	tile_set = p_tile_set;
	if (tile_set.is_valid()) {
		tile_set->connect(CoreStringNames::get_singleton()->changed, this, "_tile_set_changed_plan_update");
	}
	_tile_set_changed_plan_update();
}

bool DummyObject::_set(const StringName &p_name, const Variant &p_value) {
	if (properties.has(p_name)) {
		properties[p_name] = p_value;
		return true;
	}
	return false;
}

bool DummyObject::_get(const StringName &p_name, Variant &r_ret) const {
	if (properties.has(p_name)) {
		r_ret = properties[p_name];
		return true;
	}
	return false;
}

bool DummyObject::has_dummy_property(const StringName &p_name) {
	return properties.has(p_name);
}

void DummyObject::add_dummy_property(const StringName &p_name) {
	ERR_FAIL_COND(properties.has(p_name));
	properties[p_name] = Variant();
}

void DummyObject::remove_dummy_property(const StringName &p_name) {
	ERR_FAIL_COND(!properties.has(p_name));
	properties.erase(p_name);
}

void DummyObject::clear_dummy_properties() {
	properties.clear();
}

void GenericTilePolygonEditor::_base_control_draw() {
	ERR_FAIL_COND(!tile_set.is_valid());

	real_t grab_threshold = EDITOR_GET("editors/poly_editor/point_grab_radius");

	Color grid_color = EDITOR_GET("editors/layered_tiles_editor/grid_color");
	const Ref<Texture> handle = get_theme_icon("EditorPathSharpHandle", "EditorIcons");
	const Ref<Texture> add_handle = get_theme_icon("EditorHandleAdd", "EditorIcons");
	const Ref<StyleBox> focus_stylebox = get_theme_stylebox("Focus", "EditorStyles");

	// Draw the focus rectangle.
	if (base_control->has_focus()) {
		base_control->draw_style_box(focus_stylebox, Rect2(Vector2(), base_control->get_size()));
	}

	// Draw tile-related things.
	Size2 tile_size = tile_set->get_tile_size();

	Transform2D xform;
	xform.set_origin(base_control->get_size() / 2 + panning);
	xform.set_scale(Vector2(editor_zoom_widget->get_zoom(), editor_zoom_widget->get_zoom()));
	base_control->draw_set_transform_matrix(xform);

	// Draw the tile shape filled.
	Transform2D tile_xform;
	tile_xform.set_scale(tile_size);
	tile_set->draw_tile_shape(base_control, tile_xform, Color(1.0, 1.0, 1.0, 0.3), true);

	// Draw the background.
	if (background_texture.is_valid()) {
		Size2 region_size = background_region.size;
		if (background_h_flip) {
			region_size.x = -region_size.x;
		}
		if (background_v_flip) {
			region_size.y = -region_size.y;
		}
		base_control->draw_texture_rect_region(background_texture, Rect2(-background_region.size / 2 - background_offset, region_size), background_region, background_modulate, background_transpose);
	}

	// Draw grid.
	if (current_snap_option == SNAP_GRID) {
		Vector2 spacing = tile_size / snap_subdivision->get_value();
		Vector2 offset = -tile_size / 2;

		for (int i = 1; i < snap_subdivision->get_value(); i++) {
			base_control->draw_line(Vector2(spacing.x * i, 0) + offset, Vector2(spacing.x * i, tile_size.y) + offset, Color(1, 1, 1, 0.33));
			base_control->draw_line(Vector2(0, spacing.y * i) + offset, Vector2(tile_size.x, spacing.y * i) + offset, Color(1, 1, 1, 0.33));
		}
	}

	// Draw the polygons.
	for (uint32_t i = 0; i < polygons.size(); ++i) {
		const Vector<Vector2> &polygon = polygons[i];

		Color color = polygon_color;
		if (!in_creation_polygon.empty()) {
			color = color.darkened(0.3);
		}
		color.a = 0.5;
		Vector<Color> v_color;
		v_color.push_back(color);
		base_control->draw_polygon(polygon, v_color);

		color.a = 0.7;
		for (int j = 0; j < polygon.size(); j++) {
			base_control->draw_line(polygon[j], polygon[(j + 1) % polygon.size()], color);
		}
	}

	// Draw the polygon in creation.
	if (!in_creation_polygon.empty()) {
		for (int i = 0; i < in_creation_polygon.size() - 1; i++) {
			base_control->draw_line(in_creation_polygon[i], in_creation_polygon[i + 1], Color(1.0, 1.0, 1.0));
		}
	}

	Point2 in_creation_point = xform.affine_inverse().xform(base_control->get_local_mouse_position());
	float in_creation_distance = grab_threshold * 2.0;
	_snap_to_tile_shape(in_creation_point, in_creation_distance, grab_threshold / editor_zoom_widget->get_zoom());
	_snap_point(in_creation_point);

	if (drag_type == DRAG_TYPE_CREATE_POINT && !in_creation_polygon.empty()) {
		base_control->draw_line(in_creation_polygon[in_creation_polygon.size() - 1], in_creation_point, Color(1.0, 1.0, 1.0));
	}

	// Draw the handles.
	int tinted_polygon_index = -1;
	int tinted_point_index = -1;
	if (drag_type == DRAG_TYPE_DRAG_POINT) {
		tinted_polygon_index = drag_polygon_index;
		tinted_point_index = drag_point_index;
	} else if (hovered_point_index >= 0) {
		tinted_polygon_index = hovered_polygon_index;
		tinted_point_index = hovered_point_index;
	}

	base_control->draw_set_transform_matrix(Transform2D());
	if (!in_creation_polygon.empty()) {
		for (int i = 0; i < in_creation_polygon.size(); i++) {
			base_control->draw_texture(handle, xform.xform(in_creation_polygon[i]) - handle->get_size() / 2);
		}
	} else {
		for (int i = 0; i < (int)polygons.size(); i++) {
			const Vector<Vector2> &polygon = polygons[i];
			for (int j = 0; j < polygon.size(); j++) {
				const Color poly_modulate = (tinted_polygon_index == i && tinted_point_index == j) ? Color(0.5, 1, 2) : Color(1, 1, 1);
				base_control->draw_texture(handle, xform.xform(polygon[j]) - handle->get_size() / 2, poly_modulate);
			}
		}
	}

	// Draw the text on top of the selected point.
	if (tinted_polygon_index >= 0) {
		Ref<Font> font = get_theme_font("font", "Label");
		String text = multiple_polygon_mode ? vformat("%d:%d", tinted_polygon_index, tinted_point_index) : vformat("%d", tinted_point_index);
		Size2 text_size = font->get_string_size(text);
		base_control->draw_string(font, xform.xform(polygons[tinted_polygon_index][tinted_point_index]) - text_size * 0.5, text, Color(1.0, 1.0, 1.0, 0.5));
	}

	if (drag_type == DRAG_TYPE_CREATE_POINT) {
		base_control->draw_texture(handle, xform.xform(in_creation_point) - handle->get_size() / 2, Color(0.5, 1, 2));
	}

	// Draw the point creation preview in edit mode.
	if (hovered_segment_index >= 0) {
		base_control->draw_texture(add_handle, xform.xform(hovered_segment_point) - add_handle->get_size() / 2);
	}

	// Draw the tile shape line.
	base_control->draw_set_transform_matrix(xform);
	tile_set->draw_tile_shape(base_control, tile_xform, grid_color, false);
	base_control->draw_set_transform_matrix(Transform2D());
}

void GenericTilePolygonEditor::_center_view() {
	panning = Vector2();
	base_control->update();
	button_center_view->set_disabled(true);
}

void GenericTilePolygonEditor::_zoom_changed() {
	base_control->update();
}

void GenericTilePolygonEditor::_advanced_menu_item_pressed(int p_item_pressed) {
	UndoRedo *undo_redo = EditorNode::get_singleton()->get_undo_redo();

	switch (p_item_pressed) {
		case RESET_TO_DEFAULT_TILE: {
			undo_redo->create_action(TTR("Reset Polygons"));
			undo_redo->add_do_method(this, "clear_polygons");
			Vector<Vector2> polygon = tile_set->get_tile_shape_polygon();
			for (int i = 0; i < polygon.size(); i++) {
				polygon.write[i] = polygon[i] * tile_set->get_tile_size();
			}
			undo_redo->add_do_method(this, "add_polygon", polygon);
			undo_redo->add_do_method(base_control, "update");
			undo_redo->add_do_method(this, "emit_signal", "polygons_changed");
			undo_redo->add_undo_method(this, "clear_polygons");

			for (uint32_t i = 0; i < polygons.size(); ++i) {
				const Vector<Vector2> &poly = polygons[i];
				undo_redo->add_undo_method(this, "add_polygon", poly);
			}
			undo_redo->add_undo_method(base_control, "update");
			undo_redo->add_undo_method(this, "emit_signal", "polygons_changed");
			undo_redo->commit_action();
		} break;
		case CLEAR_TILE: {
			undo_redo->create_action(TTR("Clear Polygons"));
			undo_redo->add_do_method(this, "clear_polygons");
			undo_redo->add_do_method(base_control, "update");
			undo_redo->add_do_method(this, "emit_signal", "polygons_changed");
			undo_redo->add_undo_method(this, "clear_polygons");
			for (uint32_t i = 0; i < polygons.size(); ++i) {
				const Vector<Vector2> &polygon = polygons[i];
				undo_redo->add_undo_method(this, "add_polygon", polygon);
			}
			undo_redo->add_undo_method(base_control, "update");
			undo_redo->add_undo_method(this, "emit_signal", "polygons_changed");
			undo_redo->commit_action();
		} break;
		case ROTATE_RIGHT:
		case ROTATE_LEFT:
		case FLIP_HORIZONTALLY:
		case FLIP_VERTICALLY:
		case FLIP_WINDING_ORDER: {
			switch (p_item_pressed) {
				case ROTATE_RIGHT: {
					undo_redo->create_action(TTR("Rotate Polygons Right"));
				} break;
				case ROTATE_LEFT: {
					undo_redo->create_action(TTR("Rotate Polygons Left"));
				} break;
				case FLIP_HORIZONTALLY: {
					undo_redo->create_action(TTR("Flip Polygons Horizontally"));
				} break;
				case FLIP_VERTICALLY: {
					undo_redo->create_action(TTR("Flip Polygons Vertically"));
				} break;
				case FLIP_WINDING_ORDER: {
					undo_redo->create_action(TTR("Flip Polygons Winding Order"));
				} break;
				default:
					break;
			}
			if (p_item_pressed != FLIP_WINDING_ORDER) {
				for (unsigned int i = 0; i < polygons.size(); i++) {
					Vector<Point2> new_polygon;

					const Vector<Point2> &polygon_i = polygons[i];

					for (int j = 0; j < polygon_i.size(); ++j) {
						const Vector2 &vec = polygon_i[j];

						Vector2 point = vec;
						switch (p_item_pressed) {
							case ROTATE_RIGHT: {
								point = Vector2(-point.y, point.x);
							} break;
							case ROTATE_LEFT: {
								point = Vector2(point.y, -point.x);
							} break;
							case FLIP_HORIZONTALLY: {
								point = Vector2(-point.x, point.y);
							} break;
							case FLIP_VERTICALLY: {
								point = Vector2(point.x, -point.y);
							} break;
							default:
								break;
						}
						new_polygon.push_back(point);
					}
					undo_redo->add_do_method(this, "set_polygon", i, new_polygon);
				}
			} else {
				for (unsigned int i = 0; i < polygons.size(); i++) {
					Vector<Point2> new_polygon;

					const Vector<Point2> &polygon_i = polygons[i];

					for (int j = polygon_i.size() - 1; j >= 0; --j) {
						const Vector2 &vec = polygon_i[j];
						new_polygon.push_back(vec);
					}

					undo_redo->add_do_method(this, "set_polygon", i, new_polygon);
				}
			}
			undo_redo->add_do_method(base_control, "update");
			undo_redo->add_do_method(this, "emit_signal", "polygons_changed");
			for (unsigned int i = 0; i < polygons.size(); i++) {
				undo_redo->add_undo_method(this, "set_polygon", i, polygons[i]);
			}
			undo_redo->add_undo_method(base_control, "update");
			undo_redo->add_undo_method(this, "emit_signal", "polygons_changed");
			undo_redo->commit_action();
		} break;
		default:
			break;
	}
}

void GenericTilePolygonEditor::_grab_polygon_point(Vector2 p_pos, const Transform2D &p_polygon_xform, int &r_polygon_index, int &r_point_index) {
	const real_t grab_threshold = EDITOR_GET("editors/poly_editor/point_grab_radius");
	r_polygon_index = -1;
	r_point_index = -1;
	float closest_distance = grab_threshold + 1.0;
	for (unsigned int i = 0; i < polygons.size(); i++) {
		const Vector<Vector2> &polygon = polygons[i];
		for (int j = 0; j < polygon.size(); j++) {
			float distance = p_pos.distance_to(p_polygon_xform.xform(polygon[j]));
			if (distance < grab_threshold && distance < closest_distance) {
				r_polygon_index = i;
				r_point_index = j;
				closest_distance = distance;
			}
		}
	}
}

void GenericTilePolygonEditor::_grab_polygon_segment_point(Vector2 p_pos, const Transform2D &p_polygon_xform, int &r_polygon_index, int &r_segment_index, Vector2 &r_point) {
	const real_t grab_threshold = EDITOR_GET("editors/poly_editor/point_grab_radius");

	Point2 point = p_polygon_xform.affine_inverse().xform(p_pos);
	r_polygon_index = -1;
	r_segment_index = -1;
	float closest_distance = grab_threshold * 2.0;
	for (unsigned int i = 0; i < polygons.size(); i++) {
		const Vector<Vector2> &polygon = polygons[i];
		for (int j = 0; j < polygon.size(); j++) {
			Vector2 segment[2] = { polygon[j], polygon[(j + 1) % polygon.size()] };
			Vector2 closest_point = Geometry::get_closest_point_to_segment_2d(point, segment);
			float distance = closest_point.distance_to(point);
			if (distance < grab_threshold / editor_zoom_widget->get_zoom() && distance < closest_distance) {
				r_polygon_index = i;
				r_segment_index = j;
				r_point = closest_point;
				closest_distance = distance;
			}
		}
	}
}

void GenericTilePolygonEditor::_snap_to_tile_shape(Point2 &r_point, float &r_current_snapped_dist, float p_snap_dist) {
	ERR_FAIL_COND(!tile_set.is_valid());

	Vector<Point2> polygon = tile_set->get_tile_shape_polygon();
	for (int i = 0; i < polygon.size(); i++) {
		polygon.write[i] = polygon[i] * tile_set->get_tile_size();
	}
	Point2 snapped_point = r_point;

	// Snap to polygon vertices.
	bool snapped = false;
	for (int i = 0; i < polygon.size(); i++) {
		float distance = r_point.distance_to(polygon[i]);
		if (distance < p_snap_dist && distance < r_current_snapped_dist) {
			snapped_point = polygon[i];
			r_current_snapped_dist = distance;
			snapped = true;
		}
	}

	// Snap to edges if we did not snap to vertices.
	if (!snapped) {
		for (int i = 0; i < polygon.size(); i++) {
			Point2 segment[2] = { polygon[i], polygon[(i + 1) % polygon.size()] };
			Point2 point = Geometry::get_closest_point_to_segment_2d(r_point, segment);
			float distance = r_point.distance_to(point);
			if (distance < p_snap_dist && distance < r_current_snapped_dist) {
				snapped_point = point;
				r_current_snapped_dist = distance;
			}
		}
	}

	r_point = snapped_point;
}

void GenericTilePolygonEditor::_snap_point(Point2 &r_point) {
	switch (current_snap_option) {
		case SNAP_NONE:
			break;

		case SNAP_HALF_PIXEL:
			r_point = r_point.snapped(Vector2(0.5, 0.5));
			break;

		case SNAP_GRID: {
			const Vector2 tile_size = tile_set->get_tile_size();
			r_point = (r_point + tile_size / 2).snapped(tile_size / snap_subdivision->get_value()) - tile_size / 2;
		} break;
	}
}

void GenericTilePolygonEditor::_base_control_gui_input(Ref<InputEvent> p_event) {
	UndoRedo *undo_redo = EditorNode::get_singleton()->get_undo_redo();

	real_t grab_threshold = EDITOR_GET("editors/poly_editor/point_grab_radius");

	hovered_polygon_index = -1;
	hovered_point_index = -1;
	hovered_segment_index = -1;
	hovered_segment_point = Vector2();

	Transform2D xform;
	xform.set_origin(base_control->get_size() / 2 + panning);
	xform.set_scale(Vector2(editor_zoom_widget->get_zoom(), editor_zoom_widget->get_zoom()));

	Ref<InputEventMouseMotion> mm = p_event;
	if (mm.is_valid()) {
		if (drag_type == DRAG_TYPE_DRAG_POINT) {
			ERR_FAIL_INDEX(drag_polygon_index, (int)polygons.size());
			ERR_FAIL_INDEX(drag_point_index, polygons[drag_polygon_index].size());
			Point2 point = xform.affine_inverse().xform(mm->get_position());
			float distance = grab_threshold * 2.0;
			_snap_to_tile_shape(point, distance, grab_threshold / editor_zoom_widget->get_zoom());
			_snap_point(point);
			polygons[drag_polygon_index].write[drag_point_index] = point;
		} else if (drag_type == DRAG_TYPE_PAN) {
			panning += mm->get_position() - drag_last_pos;
			drag_last_pos = mm->get_position();
			button_center_view->set_disabled(panning.is_equal_approx(Vector2()));
		} else {
			// Update hovered point.
			_grab_polygon_point(mm->get_position(), xform, hovered_polygon_index, hovered_point_index);

			// If we have no hovered point, check if we hover a segment.
			if (hovered_point_index == -1) {
				_grab_polygon_segment_point(mm->get_position(), xform, hovered_polygon_index, hovered_segment_index, hovered_segment_point);
			}
		}
	}

	Ref<InputEventMouseButton> mb = p_event;
	if (mb.is_valid()) {
		if (mb->get_button_index() == BUTTON_WHEEL_UP && mb->get_command()) {
			editor_zoom_widget->set_zoom_by_increments(1);
			_zoom_changed();
			accept_event();
		} else if (mb->get_button_index() == BUTTON_WHEEL_DOWN && mb->get_command()) {
			editor_zoom_widget->set_zoom_by_increments(-1);
			_zoom_changed();
			accept_event();
		} else if (mb->get_button_index() == BUTTON_LEFT) {
			if (mb->is_pressed()) {
				if (tools_button_group->get_pressed_button() != button_create) {
					in_creation_polygon.clear();
				}
				if (tools_button_group->get_pressed_button() == button_create) {
					// Create points.
					if (in_creation_polygon.size() >= 3 && mb->get_position().distance_to(xform.xform(in_creation_polygon[0])) < grab_threshold) {
						// Closes and create polygon.
						if (!multiple_polygon_mode) {
							clear_polygons();
						}

						button_edit->set_pressed(true);
						undo_redo->create_action(TTR("Edit Polygons"));
						if (!multiple_polygon_mode) {
							undo_redo->add_do_method(this, "clear_polygons");
						}
						undo_redo->add_do_method(this, "add_polygon", in_creation_polygon);
						undo_redo->add_do_method(base_control, "update");
						undo_redo->add_undo_method(this, "remove_last_polygon");
						undo_redo->add_undo_method(base_control, "update");
						undo_redo->commit_action();
						emit_signal("polygons_changed");

						in_creation_polygon.clear();
					} else {
						// Create a new point.
						drag_type = DRAG_TYPE_CREATE_POINT;
					}
				} else if (tools_button_group->get_pressed_button() == button_edit) {
					// Edit points.
					int closest_polygon;
					int closest_point;
					_grab_polygon_point(mb->get_position(), xform, closest_polygon, closest_point);
					if (closest_polygon >= 0) {
						drag_type = DRAG_TYPE_DRAG_POINT;
						drag_polygon_index = closest_polygon;
						drag_point_index = closest_point;
						drag_old_polygon = polygons[drag_polygon_index];
					} else {
						// Create a point.
						Vector2 point_to_create;
						_grab_polygon_segment_point(mb->get_position(), xform, closest_polygon, closest_point, point_to_create);
						if (closest_polygon >= 0) {
							polygons[closest_polygon].insert(closest_point + 1, point_to_create);
							drag_type = DRAG_TYPE_DRAG_POINT;
							drag_polygon_index = closest_polygon;
							drag_point_index = closest_point + 1;
							drag_old_polygon = polygons[closest_polygon];
						}
					}
				} else if (tools_button_group->get_pressed_button() == button_delete) {
					// Remove point.
					int closest_polygon;
					int closest_point;
					_grab_polygon_point(mb->get_position(), xform, closest_polygon, closest_point);
					if (closest_polygon >= 0) {
						Vector<Point2> old_polygon = polygons[closest_polygon];
						polygons[closest_polygon].remove(closest_point);
						undo_redo->create_action(TTR("Edit Polygons"));
						if (polygons[closest_polygon].size() < 3) {
							remove_polygon(closest_polygon);
							undo_redo->add_do_method(this, "remove_polygon", closest_polygon);
							undo_redo->add_undo_method(this, "add_polygon", old_polygon, closest_polygon);
						} else {
							undo_redo->add_do_method(this, "set_polygon", closest_polygon, polygons[closest_polygon]);
							undo_redo->add_undo_method(this, "set_polygon", closest_polygon, old_polygon);
						}
						undo_redo->add_do_method(base_control, "update");
						undo_redo->add_undo_method(base_control, "update");
						undo_redo->commit_action();
						emit_signal("polygons_changed");
					}
				}
			} else {
				if (drag_type == DRAG_TYPE_DRAG_POINT) {
					undo_redo->create_action(TTR("Edit Polygons"));
					undo_redo->add_do_method(this, "set_polygon", drag_polygon_index, polygons[drag_polygon_index]);
					undo_redo->add_do_method(base_control, "update");
					undo_redo->add_undo_method(this, "set_polygon", drag_polygon_index, drag_old_polygon);
					undo_redo->add_undo_method(base_control, "update");
					undo_redo->commit_action();
					emit_signal("polygons_changed");
				} else if (drag_type == DRAG_TYPE_CREATE_POINT) {
					Point2 point = xform.affine_inverse().xform(mb->get_position());
					float distance = grab_threshold * 2;
					_snap_to_tile_shape(point, distance, grab_threshold / editor_zoom_widget->get_zoom());
					_snap_point(point);
					in_creation_polygon.push_back(point);
				}
				drag_type = DRAG_TYPE_NONE;
				drag_point_index = -1;
			}

		} else if (mb->get_button_index() == BUTTON_RIGHT) {
			if (mb->is_pressed()) {
				if (tools_button_group->get_pressed_button() == button_edit) {
					// Remove point or pan.
					int closest_polygon;
					int closest_point;
					_grab_polygon_point(mb->get_position(), xform, closest_polygon, closest_point);
					if (closest_polygon >= 0) {
						Vector<Point2> old_polygon = polygons[closest_polygon];
						polygons[closest_polygon].remove(closest_point);
						undo_redo->create_action(TTR("Edit Polygons"));
						if (polygons[closest_polygon].size() < 3) {
							remove_polygon(closest_polygon);
							undo_redo->add_do_method(this, "remove_polygon", closest_polygon);
							undo_redo->add_undo_method(this, "add_polygon", old_polygon, closest_polygon);
						} else {
							undo_redo->add_do_method(this, "set_polygon", closest_polygon, polygons[closest_polygon]);
							undo_redo->add_undo_method(this, "set_polygon", closest_polygon, old_polygon);
						}
						undo_redo->add_do_method(base_control, "update");
						undo_redo->add_undo_method(base_control, "update");
						undo_redo->commit_action();
						emit_signal("polygons_changed");
						drag_type = DRAG_TYPE_NONE;
					} else {
						drag_type = DRAG_TYPE_PAN;
						drag_last_pos = mb->get_position();
					}
				} else {
					drag_type = DRAG_TYPE_PAN;
					drag_last_pos = mb->get_position();
				}
			} else {
				drag_type = DRAG_TYPE_NONE;
			}
		} else if (mb->get_button_index() == BUTTON_MIDDLE) {
			if (mb->is_pressed()) {
				drag_type = DRAG_TYPE_PAN;
				drag_last_pos = mb->get_position();
			} else {
				drag_type = DRAG_TYPE_NONE;
			}
		}
	}

	base_control->update();
}

void GenericTilePolygonEditor::_set_snap_option(int p_index) {
	current_snap_option = p_index;
	button_pixel_snap->set_icon(button_pixel_snap->get_popup()->get_item_icon(p_index));
	snap_subdivision->set_visible(p_index == SNAP_GRID);
	base_control->update();
	_store_snap_options();
}

void GenericTilePolygonEditor::_store_snap_options() {
	EditorSettings::get_singleton()->set_project_metadata("editor_metadata", "tile_snap_option", current_snap_option);
	EditorSettings::get_singleton()->set_project_metadata("editor_metadata", "tile_snap_subdiv", snap_subdivision->get_value());
}

void GenericTilePolygonEditor::_toggle_expand(bool p_expand) {
	if (p_expand) {
		LayeredTileSetEditor::get_singleton()->add_expanded_editor(this);
	} else {
		LayeredTileSetEditor::get_singleton()->remove_expanded_editor();
	}
}

void GenericTilePolygonEditor::_snap_subdivision_value_changed(real_t p_value) {
	update();
	_store_snap_options();
}

void GenericTilePolygonEditor::_editor_zoom_widget_zoom_changed(real_t p_value) {
	_zoom_changed();
}

void GenericTilePolygonEditor::set_use_undo_redo(bool p_use_undo_redo) {
	use_undo_redo = p_use_undo_redo;
}

void GenericTilePolygonEditor::set_tile_set(Ref<LayeredTileSet> p_tile_set) {
	ERR_FAIL_COND(!p_tile_set.is_valid());
	if (tile_set == p_tile_set) {
		return;
	}

	// Set the default tile shape
	clear_polygons();
	if (p_tile_set.is_valid()) {
		Vector<Vector2> polygon = p_tile_set->get_tile_shape_polygon();
		for (int i = 0; i < polygon.size(); i++) {
			polygon.write[i] = polygon[i] * p_tile_set->get_tile_size();
		}
		add_polygon(polygon);
	}

	tile_set = p_tile_set;

	// Set the default zoom value.
	int default_control_y_size = 200 * EDSCALE;
	Vector2 zoomed_tile = editor_zoom_widget->get_zoom() * tile_set->get_tile_size();
	while (zoomed_tile.y < default_control_y_size) {
		editor_zoom_widget->set_zoom_by_increments(6, false);
		float current_zoom = editor_zoom_widget->get_zoom();
		zoomed_tile = current_zoom * tile_set->get_tile_size();

		if (Math::is_equal_approx(current_zoom, editor_zoom_widget->get_max_zoom())) {
			break;
		}
	}
	while (zoomed_tile.y > default_control_y_size) {
		editor_zoom_widget->set_zoom_by_increments(-6, false);
		float current_zoom = editor_zoom_widget->get_zoom();
		zoomed_tile = current_zoom * tile_set->get_tile_size();

		if (Math::is_equal_approx(current_zoom, editor_zoom_widget->get_min_zoom())) {
			break;
		}
	}
	editor_zoom_widget->set_zoom_by_increments(-6, false);
	_zoom_changed();
}

void GenericTilePolygonEditor::set_background(Ref<Texture> p_texture, Rect2 p_region, Vector2 p_offset, bool p_flip_h, bool p_flip_v, bool p_transpose, Color p_modulate) {
	background_texture = p_texture;
	background_region = p_region;
	background_offset = p_offset;
	background_h_flip = p_flip_h;
	background_v_flip = p_flip_v;
	background_transpose = p_transpose;
	background_modulate = p_modulate;
	base_control->update();
}

int GenericTilePolygonEditor::get_polygon_count() {
	return polygons.size();
}

int GenericTilePolygonEditor::add_polygon(const Vector<Point2> &p_polygon, int p_index) {
	ERR_FAIL_COND_V(p_polygon.size() < 3, -1);
	ERR_FAIL_COND_V(!multiple_polygon_mode && polygons.size() >= 1, -1);

	if (p_index < 0) {
		polygons.push_back(p_polygon);
		base_control->update();
		button_edit->set_pressed(true);
		return polygons.size() - 1;
	} else {
		polygons.insert(p_index, p_polygon);
		button_edit->set_pressed(true);
		base_control->update();
		return p_index;
	}
}

void GenericTilePolygonEditor::remove_polygon(int p_index) {
	ERR_FAIL_INDEX(p_index, (int)polygons.size());
	polygons.remove(p_index);

	if (polygons.size() == 0) {
		button_create->set_pressed(true);
	}
	base_control->update();
}

void GenericTilePolygonEditor::remove_last_polygon() {
	remove_polygon(polygons.size() - 1);
}

void GenericTilePolygonEditor::clear_polygons() {
	polygons.clear();
	base_control->update();
}

void GenericTilePolygonEditor::set_polygon(int p_polygon_index, const Vector<Point2> &p_polygon) {
	ERR_FAIL_INDEX(p_polygon_index, (int)polygons.size());
	ERR_FAIL_COND(p_polygon.size() < 3);
	polygons[p_polygon_index] = p_polygon;
	button_edit->set_pressed(true);
	base_control->update();
}

Vector<Point2> GenericTilePolygonEditor::get_polygon(int p_polygon_index) {
	ERR_FAIL_INDEX_V(p_polygon_index, (int)polygons.size(), Vector<Point2>());
	return polygons[p_polygon_index];
}

void GenericTilePolygonEditor::set_polygons_color(Color p_color) {
	polygon_color = p_color;
	base_control->update();
}

void GenericTilePolygonEditor::set_multiple_polygon_mode(bool p_multiple_polygon_mode) {
	multiple_polygon_mode = p_multiple_polygon_mode;
}

void GenericTilePolygonEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			if (!get_meta("reparented", false)) {
				button_expand->set_pressed_no_signal(false);
			}
		} break;
		case NOTIFICATION_THEME_CHANGED: {
			button_expand->set_icon(get_theme_icon("DistractionFree", "EditorIcons"));
			button_create->set_icon(get_theme_icon("CurveCreate", "EditorIcons"));
			button_edit->set_icon(get_theme_icon("CurveEdit", "EditorIcons"));
			button_delete->set_icon(get_theme_icon("CurveDelete", "EditorIcons"));
			button_center_view->set_icon(get_theme_icon("CenterView", "EditorIcons"));
			button_advanced_menu->set_icon(get_theme_icon("GuiTabMenuHl", "EditorIcons"));
			button_pixel_snap->get_popup()->set_item_icon(0, get_theme_icon("SnapDisable", "EditorIcons"));
			button_pixel_snap->get_popup()->set_item_icon(1, get_theme_icon("Snap", "EditorIcons"));
			button_pixel_snap->get_popup()->set_item_icon(2, get_theme_icon("SnapGrid", "EditorIcons"));
			button_pixel_snap->set_icon(button_pixel_snap->get_popup()->get_item_icon(current_snap_option));

			PopupMenu *p = button_advanced_menu->get_popup();
			p->set_item_icon(p->get_item_index(ROTATE_RIGHT), get_theme_icon("RotateRight", "EditorIcons"));
			p->set_item_icon(p->get_item_index(ROTATE_LEFT), get_theme_icon("RotateLeft", "EditorIcons"));
			p->set_item_icon(p->get_item_index(FLIP_HORIZONTALLY), get_theme_icon("MirrorX", "EditorIcons"));
			p->set_item_icon(p->get_item_index(FLIP_VERTICALLY), get_theme_icon("MirrorY", "EditorIcons"));
			p->set_item_icon(p->get_item_index(FLIP_WINDING_ORDER), get_theme_icon("PolygonPathFinder", "EditorIcons"));
		} break;
	}
}

void GenericTilePolygonEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_polygon_count"), &GenericTilePolygonEditor::get_polygon_count);
	ClassDB::bind_method(D_METHOD("add_polygon", "polygon", "index"), &GenericTilePolygonEditor::add_polygon, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("remove_polygon", "index"), &GenericTilePolygonEditor::remove_polygon);
	ClassDB::bind_method(D_METHOD("remove_last_polygon"), &GenericTilePolygonEditor::remove_last_polygon);
	ClassDB::bind_method(D_METHOD("clear_polygons"), &GenericTilePolygonEditor::clear_polygons);
	ClassDB::bind_method(D_METHOD("set_polygon", "index", "polygon"), &GenericTilePolygonEditor::set_polygon);
	ClassDB::bind_method(D_METHOD("get_polygon", "index"), &GenericTilePolygonEditor::get_polygon);

	ClassDB::bind_method(D_METHOD("_toggle_expand"), &GenericTilePolygonEditor::_toggle_expand);

	ClassDB::bind_method(D_METHOD("_advanced_menu_item_pressed"), &GenericTilePolygonEditor::_advanced_menu_item_pressed);
	ClassDB::bind_method(D_METHOD("_set_snap_option"), &GenericTilePolygonEditor::_set_snap_option);
	ClassDB::bind_method(D_METHOD("_base_control_draw"), &GenericTilePolygonEditor::_base_control_draw);
	ClassDB::bind_method(D_METHOD("_base_control_gui_input"), &GenericTilePolygonEditor::_base_control_gui_input);
	ClassDB::bind_method(D_METHOD("_center_view"), &GenericTilePolygonEditor::_center_view);
	ClassDB::bind_method(D_METHOD("_snap_subdivision_value_changed"), &GenericTilePolygonEditor::_snap_subdivision_value_changed);
	ClassDB::bind_method(D_METHOD("_editor_zoom_widget_zoom_changed"), &GenericTilePolygonEditor::_editor_zoom_widget_zoom_changed);

	ADD_SIGNAL(MethodInfo("polygons_changed"));
}

GenericTilePolygonEditor::GenericTilePolygonEditor() {
	toolbar = memnew(HBoxContainer);
	add_child(toolbar);

	tools_button_group.instance();

	button_expand = memnew(Button);
	button_expand->set_theme_type_variation("FlatButton");
	button_expand->set_toggle_mode(true);
	button_expand->set_pressed(false);
	button_expand->set_tooltip(TTR("Expand editor"));
	button_expand->connect("toggled", this, "_toggle_expand");
	toolbar->add_child(button_expand);

	toolbar->add_child(memnew(VSeparator));

	button_create = memnew(Button);
	button_create->set_theme_type_variation("FlatButton");
	button_create->set_toggle_mode(true);
	button_create->set_button_group(tools_button_group);
	button_create->set_pressed(true);
	button_create->set_tooltip(TTR("Add polygon tool"));
	toolbar->add_child(button_create);

	button_edit = memnew(Button);
	button_edit->set_theme_type_variation("FlatButton");
	button_edit->set_toggle_mode(true);
	button_edit->set_button_group(tools_button_group);
	button_edit->set_tooltip(TTR("Edit points tool"));
	toolbar->add_child(button_edit);

	button_delete = memnew(Button);
	button_delete->set_theme_type_variation("FlatButton");
	button_delete->set_toggle_mode(true);
	button_delete->set_button_group(tools_button_group);
	button_delete->set_tooltip(TTR("Delete points tool"));
	toolbar->add_child(button_delete);

	button_advanced_menu = memnew(MenuButton);
	button_advanced_menu->set_flat(false);
	button_advanced_menu->set_theme_type_variation("FlatMenuButton");
	button_advanced_menu->set_toggle_mode(true);
	button_advanced_menu->get_popup()->add_item(TTR("Reset to default tile shape"), RESET_TO_DEFAULT_TILE, KEY_F);
	button_advanced_menu->get_popup()->add_item(TTR("Clear"), CLEAR_TILE, KEY_C);
	button_advanced_menu->get_popup()->add_separator();
	button_advanced_menu->get_popup()->add_item(TTR("Rotate Right"), ROTATE_RIGHT, KEY_R);
	button_advanced_menu->get_popup()->add_item(TTR("Rotate Left"), ROTATE_LEFT, KEY_E);
	button_advanced_menu->get_popup()->add_item(TTR("Flip Horizontally"), FLIP_HORIZONTALLY, KEY_H);
	button_advanced_menu->get_popup()->add_item(TTR("Flip Vertically"), FLIP_VERTICALLY, KEY_V);
	button_advanced_menu->get_popup()->add_item(TTR("Flip Winding Order"), FLIP_WINDING_ORDER, KEY_O);
	button_advanced_menu->get_popup()->connect("id_pressed", this, "_advanced_menu_item_pressed");
	button_advanced_menu->set_focus_mode(FOCUS_ALL);
	toolbar->add_child(button_advanced_menu);

	toolbar->add_child(memnew(VSeparator));

	button_pixel_snap = memnew(MenuButton);
	toolbar->add_child(button_pixel_snap);
	button_pixel_snap->set_flat(false);
	button_pixel_snap->set_theme_type_variation("FlatMenuButton");
	button_pixel_snap->set_tooltip(TTR("Toggle Grid Snap"));
	button_pixel_snap->get_popup()->add_item(TTR("Disable Snap"), SNAP_NONE);
	button_pixel_snap->get_popup()->add_item(TTR("Half-Pixel Snap"), SNAP_HALF_PIXEL);
	button_pixel_snap->get_popup()->add_item(TTR("Grid Snap"), SNAP_GRID);
	button_pixel_snap->get_popup()->connect("index_pressed", this, "_set_snap_option");

	snap_subdivision = memnew(SpinBox);
	toolbar->add_child(snap_subdivision);
	snap_subdivision->get_line_edit()->add_theme_constant_override("minimum_character_width", 2);
	snap_subdivision->set_min(1);
	snap_subdivision->set_max(99);

	Control *root = memnew(Control);
	root->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	root->set_custom_minimum_size(Size2(0, 200 * EDSCALE));
	root->set_mouse_filter(Control::MOUSE_FILTER_IGNORE);
	add_child(root);

	panel = memnew(Panel);
	panel->set_anchors_and_margins_preset(Control::PRESET_WIDE);
	panel->set_mouse_filter(Control::MOUSE_FILTER_IGNORE);
	root->add_child(panel);

	base_control = memnew(Control);
	base_control->set_anchors_and_margins_preset(Control::PRESET_WIDE);
	base_control->connect("draw", this, "_base_control_draw");
	base_control->connect("gui_input", this, "_base_control_gui_input");
	base_control->set_clip_contents(true);
	base_control->set_focus_mode(Control::FOCUS_CLICK);
	root->add_child(base_control);
	snap_subdivision->connect("value_changed", this, "_snap_subdivision_value_changed");

	editor_zoom_widget = memnew(EditorZoomWidget);
	editor_zoom_widget->setup_zoom_limits(0.125, 128.0);
	editor_zoom_widget->set_position(Vector2(5, 5));
	editor_zoom_widget->connect("zoom_changed", this, "_editor_zoom_widget_zoom_changed");
	editor_zoom_widget->set_shortcut_context(this);
	root->add_child(editor_zoom_widget);

	button_center_view = memnew(Button);
	button_center_view->set_anchors_and_margins_preset(Control::PRESET_TOP_RIGHT, Control::PRESET_MODE_MINSIZE, 2 * EDSCALE);
	button_center_view->set_h_grow_direction(GrowDirection::GROW_DIRECTION_BEGIN);
	button_center_view->set_v_grow_direction(GrowDirection::GROW_DIRECTION_END);
	button_center_view->connect("pressed", this, "_center_view");
	button_center_view->set_theme_type_variation("FlatButton");
	button_center_view->set_disabled(true);
	root->add_child(button_center_view);

	snap_subdivision->set_value_no_signal(EditorSettings::get_singleton()->get_project_metadata("editor_metadata", "tile_snap_subdiv", 4));
	_set_snap_option(EditorSettings::get_singleton()->get_project_metadata("editor_metadata", "tile_snap_option", SNAP_NONE));
}

void TileDataDefaultEditor::_property_value_changed(const StringName &p_property, const Variant &p_value, const StringName &p_field, bool p_changing) {
	ERR_FAIL_NULL(dummy_object);
	dummy_object->set(p_property, p_value);
	emit_signal("needs_redraw");
}

Variant TileDataDefaultEditor::_get_painted_value() {
	ERR_FAIL_NULL_V(dummy_object, Variant());
	return dummy_object->get(property);
}

void TileDataDefaultEditor::_set_painted_value(LayeredTileSetAtlasSource *p_tile_set_atlas_source, Vector2 p_coords, int p_alternative_tile) {
	LayeredTileData *tile_data = p_tile_set_atlas_source->get_tile_data(p_coords, p_alternative_tile);
	ERR_FAIL_NULL(tile_data);
	Variant value = tile_data->get(property);
	dummy_object->set(property, value);
	if (property_editor) {
		property_editor->update_property();
	}
}

void TileDataDefaultEditor::_set_value(LayeredTileSetAtlasSource *p_tile_set_atlas_source, Vector2 p_coords, int p_alternative_tile, const Variant &p_value) {
	LayeredTileData *tile_data = p_tile_set_atlas_source->get_tile_data(p_coords, p_alternative_tile);
	ERR_FAIL_NULL(tile_data);
	tile_data->set(property, p_value);
}

Variant TileDataDefaultEditor::_get_value(LayeredTileSetAtlasSource *p_tile_set_atlas_source, Vector2 p_coords, int p_alternative_tile) {
	LayeredTileData *tile_data = p_tile_set_atlas_source->get_tile_data(p_coords, p_alternative_tile);
	ERR_FAIL_NULL_V(tile_data, Variant());
	return tile_data->get(property);
}

void TileDataDefaultEditor::_setup_undo_redo_action(LayeredTileSetAtlasSource *p_tile_set_atlas_source, const HashMap<LayeredTileMapCell, Variant, LayeredTileMapCell> &p_previous_values, const Variant &p_new_value) {
	UndoRedo *undo_redo = EditorNode::get_singleton()->get_undo_redo();

	for (const HashMap<LayeredTileMapCell, Variant, LayeredTileMapCell>::Element *E = p_previous_values.front(); E; E = E->next) {
		Vector2i coords = E->key().get_atlas_coords();
		undo_redo->add_undo_property(p_tile_set_atlas_source, vformat("%d:%d/%d/%s", coords.x, coords.y, E->key().alternative_tile, property), E->value());
		undo_redo->add_do_property(p_tile_set_atlas_source, vformat("%d:%d/%d/%s", coords.x, coords.y, E->key().alternative_tile, property), p_new_value);
	}
}

void TileDataDefaultEditor::forward_draw_over_atlas(LayeredTileAtlasView *p_tile_atlas_view, LayeredTileSetAtlasSource *p_tile_set_atlas_source, CanvasItem *p_canvas_item, Transform2D p_transform) {
	if (drag_type == DRAG_TYPE_PAINT_RECT) {
		Color grid_color = EDITOR_GET("editors/layered_tiles_editor/grid_color");
		Color selection_color = Color().from_hsv(Math::fposmod(grid_color.get_h() + 0.5, 1.0), grid_color.get_s(), grid_color.get_v(), 1.0);

		p_canvas_item->draw_set_transform_matrix(p_transform);

		Rect2i rect;
		rect.set_position(p_tile_atlas_view->get_atlas_tile_coords_at_pos(drag_start_pos, true));
		rect.set_end(p_tile_atlas_view->get_atlas_tile_coords_at_pos(p_transform.affine_inverse().xform(p_canvas_item->get_local_mouse_position()), true));
		rect = rect.abs();

		RBSet<LayeredTileMapCell> edited;
		for (int x = rect.get_position().x; x <= rect.get_end().x; x++) {
			for (int y = rect.get_position().y; y <= rect.get_end().y; y++) {
				Vector2i coords = Vector2i(x, y);
				coords = p_tile_set_atlas_source->get_tile_at_coords(coords);
				if (coords != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
					LayeredTileMapCell cell;
					cell.source_id = 0;
					cell.set_atlas_coords(coords);
					cell.alternative_tile = 0;
					edited.insert(cell);
				}
			}
		}

		for (const RBSet<LayeredTileMapCell>::Element *E = edited.front(); E; E = E->next()) {
			Vector2i coords = E->get().get_atlas_coords();
			p_canvas_item->draw_rect(p_tile_set_atlas_source->get_tile_texture_region(coords), selection_color, false);
		}
		p_canvas_item->draw_set_transform_matrix(Transform2D());
	}
};

void TileDataDefaultEditor::forward_draw_over_alternatives(LayeredTileAtlasView *p_tile_atlas_view, LayeredTileSetAtlasSource *p_tile_set_atlas_source, CanvasItem *p_canvas_item, Transform2D p_transform) {

};

void TileDataDefaultEditor::forward_painting_atlas_gui_input(LayeredTileAtlasView *p_tile_atlas_view, LayeredTileSetAtlasSource *p_tile_set_atlas_source, const Ref<InputEvent> &p_event) {
	Ref<InputEventMouseMotion> mm = p_event;
	if (mm.is_valid()) {
		if (drag_type == DRAG_TYPE_PAINT) {
			Vector2i p0 = p_tile_atlas_view->get_atlas_tile_coords_at_pos(drag_last_pos, true);
			Vector2i p1 = p_tile_atlas_view->get_atlas_tile_coords_at_pos(mm->get_position(), true);

			Vector<Vector2i> line = Geometry::bresenham_line(p0.x, p1.x, p0.y, p1.y);
			for (int i = 0; i < line.size(); i++) {
				Vector2i coords = p_tile_set_atlas_source->get_tile_at_coords(line[i]);
				if (coords != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
					LayeredTileMapCell cell;
					cell.source_id = 0;
					cell.set_atlas_coords(coords);
					cell.alternative_tile = 0;
					if (!drag_modified.has(cell)) {
						drag_modified[cell] = _get_value(p_tile_set_atlas_source, coords, 0);
					}
					_set_value(p_tile_set_atlas_source, coords, 0, drag_painted_value);
				}
			}
			drag_last_pos = mm->get_position();
		}
	}

	UndoRedo *undo_redo = EditorNode::get_singleton()->get_undo_redo();
	Ref<InputEventMouseButton> mb = p_event;
	if (mb.is_valid()) {
		if (mb->get_button_index() == BUTTON_LEFT) {
			if (mb->is_pressed()) {
				if (picker_button->is_pressed() || (mb->get_control() && !mb->get_shift())) {
					Vector2i coords = p_tile_atlas_view->get_atlas_tile_coords_at_pos(mb->get_position(), true);
					coords = p_tile_set_atlas_source->get_tile_at_coords(coords);
					if (coords != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
						_set_painted_value(p_tile_set_atlas_source, coords, 0);
						picker_button->set_pressed(false);
					}
				} else if (mb->get_control() && mb->get_shift()) {
					drag_type = DRAG_TYPE_PAINT_RECT;
					drag_modified.clear();
					drag_painted_value = _get_painted_value();
					drag_start_pos = mb->get_position();
				} else {
					drag_type = DRAG_TYPE_PAINT;
					drag_modified.clear();
					drag_painted_value = _get_painted_value();
					Vector2i coords = p_tile_atlas_view->get_atlas_tile_coords_at_pos(mb->get_position(), true);
					coords = p_tile_set_atlas_source->get_tile_at_coords(coords);
					if (coords != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
						LayeredTileMapCell cell;
						cell.source_id = 0;
						cell.set_atlas_coords(coords);
						cell.alternative_tile = 0;
						drag_modified[cell] = _get_value(p_tile_set_atlas_source, coords, 0);
						_set_value(p_tile_set_atlas_source, coords, 0, drag_painted_value);
					}
					drag_last_pos = mb->get_position();
				}
			} else {
				if (drag_type == DRAG_TYPE_PAINT_RECT) {
					Rect2i rect;
					rect.set_position(p_tile_atlas_view->get_atlas_tile_coords_at_pos(drag_start_pos, true));
					rect.set_end(p_tile_atlas_view->get_atlas_tile_coords_at_pos(mb->get_position(), true));
					rect = rect.abs();

					drag_modified.clear();
					for (int x = rect.get_position().x; x <= rect.get_end().x; x++) {
						for (int y = rect.get_position().y; y <= rect.get_end().y; y++) {
							Vector2i coords = Vector2i(x, y);
							coords = p_tile_set_atlas_source->get_tile_at_coords(coords);
							if (coords != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
								LayeredTileMapCell cell;
								cell.source_id = 0;
								cell.set_atlas_coords(coords);
								cell.alternative_tile = 0;
								drag_modified[cell] = _get_value(p_tile_set_atlas_source, coords, 0);
							}
						}
					}
					undo_redo->create_action(TTR("Painting Tiles Property"));
					_setup_undo_redo_action(p_tile_set_atlas_source, drag_modified, drag_painted_value);
					undo_redo->commit_action();
					drag_type = DRAG_TYPE_NONE;
				} else if (drag_type == DRAG_TYPE_PAINT) {
					undo_redo->create_action(TTR("Painting Tiles Property"));
					_setup_undo_redo_action(p_tile_set_atlas_source, drag_modified, drag_painted_value);
					undo_redo->commit_action();
					drag_type = DRAG_TYPE_NONE;
				}
			}
		}
	}
}

void TileDataDefaultEditor::forward_painting_alternatives_gui_input(LayeredTileAtlasView *p_tile_atlas_view, LayeredTileSetAtlasSource *p_tile_set_atlas_source, const Ref<InputEvent> &p_event) {
	Ref<InputEventMouseMotion> mm = p_event;
	if (mm.is_valid()) {
		if (drag_type == DRAG_TYPE_PAINT) {
			Vector3i tile = p_tile_atlas_view->get_alternative_tile_at_pos(mm->get_position());
			Vector2i coords = Vector2i(tile.x, tile.y);
			int alternative_tile = tile.z;

			if (coords != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
				LayeredTileMapCell cell;
				cell.source_id = 0;
				cell.set_atlas_coords(coords);
				cell.alternative_tile = alternative_tile;
				if (!drag_modified.has(cell)) {
					drag_modified[cell] = _get_value(p_tile_set_atlas_source, coords, alternative_tile);
				}
				_set_value(p_tile_set_atlas_source, coords, alternative_tile, drag_painted_value);
			}

			drag_last_pos = mm->get_position();
		}
	}

	Ref<InputEventMouseButton> mb = p_event;
	if (mb.is_valid()) {
		if (mb->get_button_index() == BUTTON_LEFT) {
			if (mb->is_pressed()) {
				if (picker_button->is_pressed()) {
					Vector3i tile = p_tile_atlas_view->get_alternative_tile_at_pos(mb->get_position());
					Vector2i coords = Vector2i(tile.x, tile.y);
					int alternative_tile = tile.z;
					if (coords != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
						_set_painted_value(p_tile_set_atlas_source, coords, alternative_tile);
						picker_button->set_pressed(false);
					}
				} else {
					drag_type = DRAG_TYPE_PAINT;
					drag_modified.clear();
					drag_painted_value = _get_painted_value();

					Vector3i tile = p_tile_atlas_view->get_alternative_tile_at_pos(mb->get_position());
					Vector2i coords = Vector2i(tile.x, tile.y);
					int alternative_tile = tile.z;

					if (coords != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
						LayeredTileMapCell cell;
						cell.source_id = 0;
						cell.set_atlas_coords(coords);
						cell.alternative_tile = alternative_tile;
						drag_modified[cell] = _get_value(p_tile_set_atlas_source, coords, alternative_tile);
						_set_value(p_tile_set_atlas_source, coords, alternative_tile, drag_painted_value);
					}
					drag_last_pos = mb->get_position();
				}
			} else {
				UndoRedo *undo_redo = EditorNode::get_singleton()->get_undo_redo();
				undo_redo->create_action(TTR("Painting Tiles Property"));
				_setup_undo_redo_action(p_tile_set_atlas_source, drag_modified, drag_painted_value);
				undo_redo->commit_action();
				drag_type = DRAG_TYPE_NONE;
			}
		}
	}
}

void TileDataDefaultEditor::draw_over_tile(CanvasItem *p_canvas_item, Transform2D p_transform, LayeredTileMapCell p_cell, bool p_selected) {
	LayeredTileData *tile_data = _get_tile_data(p_cell);

	ERR_FAIL_NULL(tile_data);

	bool valid;
	Variant value = tile_data->get(property, &valid);
	if (!valid) {
		return;
	}

	Vector2 texture_origin = tile_data->get_texture_origin();
	if (value.get_type() == Variant::BOOL) {
		Ref<Texture> texture = (bool)value ? tile_bool_checked : tile_bool_unchecked;
		int size = MIN(tile_set->get_tile_size().x, tile_set->get_tile_size().y) / 3;
		Rect2 rect = p_transform.xform(Rect2(Vector2(-size / 2, -size / 2) - texture_origin, Vector2(size, size)));
		p_canvas_item->draw_texture_rect(texture, rect);
	} else if (value.get_type() == Variant::COLOR) {
		int size = MIN(tile_set->get_tile_size().x, tile_set->get_tile_size().y) / 3;
		Rect2 rect = p_transform.xform(Rect2(Vector2(-size / 2, -size / 2) - texture_origin, Vector2(size, size)));
		p_canvas_item->draw_rect(rect, value);
	} else {
		Ref<Font> font = LayeredTileSetEditor::get_singleton()->get_theme_font("bold", "EditorFonts");
		String text;
		// Round floating point precision to 2 digits, as tiles don't have that much space.
		switch (value.get_type()) {
			case Variant::REAL:
				text = vformat("%.2f", value);
				break;
			case Variant::VECTOR2:
			case Variant::VECTOR3:
			case Variant::VECTOR4:
				text = vformat("%.2v", value);
				break;
			default:
				List<const void *> stack;
				text = value.stringify(stack);
				break;
		}

		Color color = Color(1, 1, 1);
		if (p_selected) {
			Color grid_color = EDITOR_GET("editors/layered_tiles_editor/grid_color");
			Color selection_color = Color().from_hsv(Math::fposmod(grid_color.get_h() + 0.5, 1.0), grid_color.get_s(), grid_color.get_v(), 1.0);
			selection_color.set_v(0.9);
			color = selection_color;
		} else if (is_visible_in_tree()) {
			Variant painted_value = _get_painted_value();
			bool equal = (painted_value.get_type() == Variant::REAL && value.get_type() == Variant::REAL) ? Math::is_equal_approx(float(painted_value), float(value)) : painted_value == value;
			if (equal) {
				color = Color(0.7, 0.7, 0.7);
			}
		}

		Vector2 string_size = font->get_string_size(text);
		p_canvas_item->draw_string(font, p_transform.xform(-texture_origin) + Vector2i(-string_size.x / 2, string_size.y / 4), text, color);
	}
}

void TileDataDefaultEditor::setup_property_editor(Variant::Type p_type, const String &p_property, const String &p_label, const Variant &p_default_value, PropertyHint p_hint, const String &p_hint_text, int p_usage) {
	ERR_FAIL_COND_MSG(!property.empty(), "Cannot setup TileDataDefaultEditor twice");
	property = p_property;
	property_type = p_type;

	// Update everything.
	if (property_editor) {
		property_editor->queue_delete();
	}

	// Update the dummy object.
	dummy_object->add_dummy_property(p_property);

	// Get the default value for the type.
	if (p_default_value == Variant()) {
		Variant::CallError error;
		Variant painted_value = Variant::construct(p_type, nullptr, 0, error);
		dummy_object->set(p_property, painted_value);
	} else {
		dummy_object->set(p_property, p_default_value);
	}

	// Create and setup the property editor.
	property_editor = EditorInspector::instantiate_property_editor(dummy_object, p_type, p_property, p_hint, p_hint_text, p_usage);

	property_editor->set_object_and_property(dummy_object, p_property);
	if (p_label.empty()) {
		property_editor->set_label(EditorPropertyNameProcessor::get_singleton()->process_name(p_property));
	} else {
		property_editor->set_label(p_label);
	}

	property_editor->connect("property_changed", this, "_property_value_changed");
	property_editor->set_tooltip(p_property);
	property_editor->update_property();
	add_child(property_editor);
}

void TileDataDefaultEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE:
		case NOTIFICATION_THEME_CHANGED: {
			picker_button->set_icon(get_theme_icon("ColorPick", "EditorIcons"));
			tile_bool_checked = get_theme_icon("TileChecked", "EditorIcons");
			tile_bool_unchecked = get_theme_icon("TileUnchecked", "EditorIcons");
		} break;
	}
}

void TileDataDefaultEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_property_value_changed"), &TileDataDefaultEditor::_property_value_changed);
}

Variant::Type TileDataDefaultEditor::get_property_type() {
	return property_type;
}

TileDataDefaultEditor::TileDataDefaultEditor() {
	label = memnew(Label);
	label->set_text(TTR("Painting:"));
	label->set_theme_type_variation("HeaderSmall");
	add_child(label);

	picker_button = memnew(Button);
	picker_button->set_theme_type_variation("FlatButton");
	picker_button->set_toggle_mode(true);
	picker_button->set_shortcut(ED_SHORTCUT("layered_tiles_editor/picker", TTR("Picker"), KEY_P));
	toolbar->add_child(picker_button);
}

TileDataDefaultEditor::~TileDataDefaultEditor() {
	toolbar->queue_delete();
	memdelete(dummy_object);
}

void TileDataTextureOriginEditor::draw_over_tile(CanvasItem *p_canvas_item, Transform2D p_transform, LayeredTileMapCell p_cell, bool p_selected) {
	LayeredTileData *tile_data = _get_tile_data(p_cell);
	ERR_FAIL_NULL(tile_data);

	Vector2i tile_set_tile_size = tile_set->get_tile_size();
	Color color = Color(1.0, 1.0, 1.0);
	if (p_selected) {
		Color grid_color = EDITOR_GET("editors/layered_tiles_editor/grid_color");
		Color selection_color = Color().from_hsv(Math::fposmod(grid_color.get_h() + 0.5, 1.0), grid_color.get_s(), grid_color.get_v(), 1.0);
		color = selection_color;
	}

	LayeredTileSetSource *source = *(tile_set->get_source(p_cell.source_id));
	LayeredTileSetAtlasSource *atlas_source = Object::cast_to<LayeredTileSetAtlasSource>(source);
	if (atlas_source->is_position_in_tile_texture_region(p_cell.get_atlas_coords(), p_cell.alternative_tile, -tile_set_tile_size / 2) && atlas_source->is_position_in_tile_texture_region(p_cell.get_atlas_coords(), p_cell.alternative_tile, tile_set_tile_size / 2 - Vector2(1, 1))) {
		Transform2D tile_xform;
		tile_xform.set_scale(tile_set_tile_size);
		tile_set->draw_tile_shape(p_canvas_item, p_transform * tile_xform, color);
	}

	if (atlas_source->is_position_in_tile_texture_region(p_cell.get_atlas_coords(), p_cell.alternative_tile, Vector2())) {
		Ref<Texture> position_icon = LayeredTileSetEditor::get_singleton()->get_theme_icon("EditorPosition", "EditorIcons");
		p_canvas_item->draw_texture(position_icon, p_transform.xform(Vector2()) - (position_icon->get_size() / 2), color);
	} else {
		Ref<Font> font = LayeredTileSetEditor::get_singleton()->get_theme_font("bold", "EditorFonts");

		Vector2 texture_origin = tile_data->get_texture_origin();
		String text = vformat("%s", texture_origin);

		Vector2 string_size = font->get_string_size(text);

		p_canvas_item->draw_string(font, p_transform.xform(-texture_origin) + Vector2i(-string_size.x / 2, string_size.y / 2), text, color);
	}
}

void TileDataPositionEditor::draw_over_tile(CanvasItem *p_canvas_item, Transform2D p_transform, LayeredTileMapCell p_cell, bool p_selected) {
	LayeredTileData *tile_data = _get_tile_data(p_cell);
	ERR_FAIL_NULL(tile_data);

	bool valid;
	Variant value = tile_data->get(property, &valid);
	if (!valid) {
		return;
	}
	ERR_FAIL_COND(value.get_type() != Variant::VECTOR2I && value.get_type() != Variant::VECTOR2);

	Color color = Color(1.0, 1.0, 1.0);
	if (p_selected) {
		Color grid_color = EDITOR_GET("editors/layered_tiles_editor/grid_color");
		Color selection_color = Color().from_hsv(Math::fposmod(grid_color.get_h() + 0.5, 1.0), grid_color.get_s(), grid_color.get_v(), 1.0);
		color = selection_color;
	}
	Ref<Texture> position_icon = LayeredTileSetEditor::get_singleton()->get_theme_icon("EditorPosition", "EditorIcons");
	p_canvas_item->draw_texture(position_icon, p_transform.xform(Vector2(value)) - position_icon->get_size() / 2, color);
}

void TileDataYSortEditor::draw_over_tile(CanvasItem *p_canvas_item, Transform2D p_transform, LayeredTileMapCell p_cell, bool p_selected) {
	LayeredTileData *tile_data = _get_tile_data(p_cell);
	ERR_FAIL_NULL(tile_data);

	Color color = Color(1.0, 1.0, 1.0);
	if (p_selected) {
		Color grid_color = EDITOR_GET("editors/layered_tiles_editor/grid_color");
		Color selection_color = Color().from_hsv(Math::fposmod(grid_color.get_h() + 0.5, 1.0), grid_color.get_s(), grid_color.get_v(), 1.0);
		color = selection_color;
	}
	Vector2 texture_origin = tile_data->get_texture_origin();
	LayeredTileSetSource *source = *(tile_set->get_source(p_cell.source_id));
	LayeredTileSetAtlasSource *atlas_source = Object::cast_to<LayeredTileSetAtlasSource>(source);
	if (atlas_source->is_position_in_tile_texture_region(p_cell.get_atlas_coords(), p_cell.alternative_tile, Vector2(0, tile_data->get_y_sort_origin()))) {
		Ref<Texture> position_icon = LayeredTileSetEditor::get_singleton()->get_theme_icon("EditorPosition", "EditorIcons");
		p_canvas_item->draw_texture(position_icon, p_transform.xform(Vector2(0, tile_data->get_y_sort_origin())) - position_icon->get_size() / 2, color);
	} else {
		Ref<Font> font = LayeredTileSetEditor::get_singleton()->get_theme_font("bold", "EditorFonts");
		String text = vformat("%s", tile_data->get_y_sort_origin());

		Vector2 string_size = font->get_string_size(text);

		p_canvas_item->draw_string(font, p_transform.xform(-texture_origin) + Vector2i(-string_size.x / 2, string_size.y / 2), text, color);
	}
}

void TileDataOcclusionShapeEditor::draw_over_tile(CanvasItem *p_canvas_item, Transform2D p_transform, LayeredTileMapCell p_cell, bool p_selected) {
	LayeredTileData *tile_data = _get_tile_data(p_cell);
	ERR_FAIL_NULL(tile_data);

	Color grid_color = EDITOR_GET("editors/layered_tiles_editor/grid_color");
	Color selection_color = Color().from_hsv(Math::fposmod(grid_color.get_h() + 0.5, 1.0), grid_color.get_s(), grid_color.get_v(), 1.0);
	Color color = grid_color.darkened(0.2);
	if (p_selected) {
		color = selection_color.darkened(0.2);
	}
	color.a *= 0.5;

	Vector<Color> debug_occlusion_color;
	debug_occlusion_color.push_back(color);

	RenderingServer::get_singleton()->canvas_item_add_set_transform(p_canvas_item->get_canvas_item(), p_transform);
	Ref<OccluderPolygon2D> occluder = tile_data->get_occluder(occlusion_layer);
	if (occluder.is_valid() && occluder->get_polygon().size() >= 3) {
		p_canvas_item->draw_polygon(Variant(occluder->get_polygon()), debug_occlusion_color);
	}
	RenderingServer::get_singleton()->canvas_item_add_set_transform(p_canvas_item->get_canvas_item(), Transform2D());
}

Variant TileDataOcclusionShapeEditor::_get_painted_value() {
	Ref<OccluderPolygon2D> occluder_polygon;
	if (polygon_editor->get_polygon_count() >= 1) {
		occluder_polygon.instance();
		occluder_polygon->set_polygon(Variant(polygon_editor->get_polygon(0)));
	}
	return occluder_polygon;
}

void TileDataOcclusionShapeEditor::_set_painted_value(LayeredTileSetAtlasSource *p_tile_set_atlas_source, Vector2 p_coords, int p_alternative_tile) {
	LayeredTileData *tile_data = p_tile_set_atlas_source->get_tile_data(p_coords, p_alternative_tile);
	ERR_FAIL_NULL(tile_data);

	Ref<OccluderPolygon2D> occluder_polygon = tile_data->get_occluder(occlusion_layer);
	polygon_editor->clear_polygons();
	if (occluder_polygon.is_valid()) {
		polygon_editor->add_polygon(Variant(occluder_polygon->get_polygon()));
	}
	polygon_editor->set_background(p_tile_set_atlas_source->get_texture(), p_tile_set_atlas_source->get_tile_texture_region(p_coords), tile_data->get_texture_origin(), tile_data->get_flip_h(), tile_data->get_flip_v(), tile_data->get_transpose(), tile_data->get_modulate());
}

void TileDataOcclusionShapeEditor::_set_value(LayeredTileSetAtlasSource *p_tile_set_atlas_source, Vector2 p_coords, int p_alternative_tile, const Variant &p_value) {
	LayeredTileData *tile_data = p_tile_set_atlas_source->get_tile_data(p_coords, p_alternative_tile);
	ERR_FAIL_NULL(tile_data);
	Ref<OccluderPolygon2D> occluder_polygon = p_value;
	tile_data->set_occluder(occlusion_layer, occluder_polygon);

	polygon_editor->set_background(p_tile_set_atlas_source->get_texture(), p_tile_set_atlas_source->get_tile_texture_region(p_coords), tile_data->get_texture_origin(), tile_data->get_flip_h(), tile_data->get_flip_v(), tile_data->get_transpose(), tile_data->get_modulate());
}

Variant TileDataOcclusionShapeEditor::_get_value(LayeredTileSetAtlasSource *p_tile_set_atlas_source, Vector2 p_coords, int p_alternative_tile) {
	LayeredTileData *tile_data = p_tile_set_atlas_source->get_tile_data(p_coords, p_alternative_tile);
	ERR_FAIL_NULL_V(tile_data, Variant());
	return tile_data->get_occluder(occlusion_layer);
}

void TileDataOcclusionShapeEditor::_setup_undo_redo_action(LayeredTileSetAtlasSource *p_tile_set_atlas_source, const HashMap<LayeredTileMapCell, Variant, LayeredTileMapCell> &p_previous_values, const Variant &p_new_value) {
	UndoRedo *undo_redo = EditorNode::get_singleton()->get_undo_redo();

	for (const HashMap<LayeredTileMapCell, Variant, LayeredTileMapCell>::Element *E = p_previous_values.front(); E; E = E->next) {
		Vector2i coords = E->key().get_atlas_coords();
		undo_redo->add_undo_property(p_tile_set_atlas_source, vformat("%d:%d/%d/occlusion_layer_%d/polygon", coords.x, coords.y, E->key().alternative_tile, occlusion_layer), E->value());
		undo_redo->add_do_property(p_tile_set_atlas_source, vformat("%d:%d/%d/occlusion_layer_%d/polygon", coords.x, coords.y, E->key().alternative_tile, occlusion_layer), p_new_value);
	}
}

void TileDataOcclusionShapeEditor::_tile_set_changed() {
	polygon_editor->set_tile_set(tile_set);
}

void TileDataOcclusionShapeEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			polygon_editor->set_polygons_color(get_tree()->get_debug_collisions_color());
		} break;
	}
}

TileDataOcclusionShapeEditor::TileDataOcclusionShapeEditor() {
	polygon_editor = memnew(GenericTilePolygonEditor);
	add_child(polygon_editor);
}

void TileDataCollisionEditor::_property_value_changed(const StringName &p_property, const Variant &p_value, const StringName &p_field, bool p_changing) {
	dummy_object->set(p_property, p_value);
}

void TileDataCollisionEditor::_property_selected(const StringName &p_path, int p_focusable) {
	// Deselect all other properties

	for (HashMap<StringName, EditorProperty *>::Element *editor = property_editors.front(); editor; editor = editor->next) {
		if (editor->key() != p_path) {
			editor->value()->deselect();
		}
	}
}

void TileDataCollisionEditor::_polygons_changed() {
	// Update the dummy object properties and their editors.
	for (int i = 0; i < polygon_editor->get_polygon_count(); i++) {
		StringName one_way_property = vformat("polygon_%d_one_way", i);
		StringName one_way_margin_property = vformat("polygon_%d_one_way_margin", i);

		if (!dummy_object->has_dummy_property(one_way_property)) {
			dummy_object->add_dummy_property(one_way_property);
			dummy_object->set(one_way_property, false);
		}

		if (!dummy_object->has_dummy_property(one_way_margin_property)) {
			dummy_object->add_dummy_property(one_way_margin_property);
			dummy_object->set(one_way_margin_property, 1.0);
		}

		if (!property_editors.has(one_way_property)) {
			EditorProperty *one_way_property_editor = EditorInspector::instantiate_property_editor(dummy_object, Variant::BOOL, one_way_property, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT);
			one_way_property_editor->set_object_and_property(dummy_object, one_way_property);
			one_way_property_editor->set_label(one_way_property);
			one_way_property_editor->connect("property_changed", this, "_property_value_changed");
			one_way_property_editor->connect("selected", this, "_property_selected");
			one_way_property_editor->set_tooltip(one_way_property_editor->get_edited_property());
			one_way_property_editor->update_property();
			add_child(one_way_property_editor);
			property_editors[one_way_property] = one_way_property_editor;
		}

		if (!property_editors.has(one_way_margin_property)) {
			EditorProperty *one_way_margin_property_editor = EditorInspector::instantiate_property_editor(dummy_object, Variant::REAL, one_way_margin_property, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT);
			one_way_margin_property_editor->set_object_and_property(dummy_object, one_way_margin_property);
			one_way_margin_property_editor->set_label(one_way_margin_property);
			one_way_margin_property_editor->connect("property_changed", this, "_property_value_changed");
			one_way_margin_property_editor->connect("selected", this, "_property_selected");
			one_way_margin_property_editor->set_tooltip(one_way_margin_property_editor->get_edited_property());
			one_way_margin_property_editor->update_property();
			add_child(one_way_margin_property_editor);
			property_editors[one_way_margin_property] = one_way_margin_property_editor;
		}
	}

	// Remove unneeded properties and their editors.
	for (int i = polygon_editor->get_polygon_count(); dummy_object->has_dummy_property(vformat("polygon_%d_one_way", i)); i++) {
		dummy_object->remove_dummy_property(vformat("polygon_%d_one_way", i));
	}
	for (int i = polygon_editor->get_polygon_count(); dummy_object->has_dummy_property(vformat("polygon_%d_one_way_margin", i)); i++) {
		dummy_object->remove_dummy_property(vformat("polygon_%d_one_way_margin", i));
	}
	for (int i = polygon_editor->get_polygon_count(); property_editors.has(vformat("polygon_%d_one_way", i)); i++) {
		property_editors[vformat("polygon_%d_one_way", i)]->queue_delete();
		property_editors.erase(vformat("polygon_%d_one_way", i));
	}
	for (int i = polygon_editor->get_polygon_count(); property_editors.has(vformat("polygon_%d_one_way_margin", i)); i++) {
		property_editors[vformat("polygon_%d_one_way_margin", i)]->queue_delete();
		property_editors.erase(vformat("polygon_%d_one_way_margin", i));
	}
}

Variant TileDataCollisionEditor::_get_painted_value() {
	Dictionary dict;
	dict["linear_velocity"] = dummy_object->get("linear_velocity");
	dict["angular_velocity"] = dummy_object->get("angular_velocity");
	Array array;
	for (int i = 0; i < polygon_editor->get_polygon_count(); i++) {
		ERR_FAIL_COND_V(polygon_editor->get_polygon(i).size() < 3, Variant());
		Dictionary polygon_dict;
		polygon_dict["points"] = polygon_editor->get_polygon(i);
		polygon_dict["one_way"] = dummy_object->get(vformat("polygon_%d_one_way", i));
		polygon_dict["one_way_margin"] = dummy_object->get(vformat("polygon_%d_one_way_margin", i));
		array.push_back(polygon_dict);
	}
	dict["polygons"] = array;

	return dict;
}

void TileDataCollisionEditor::_set_painted_value(LayeredTileSetAtlasSource *p_tile_set_atlas_source, Vector2 p_coords, int p_alternative_tile) {
	LayeredTileData *tile_data = p_tile_set_atlas_source->get_tile_data(p_coords, p_alternative_tile);
	ERR_FAIL_NULL(tile_data);

	polygon_editor->clear_polygons();
	for (int i = 0; i < tile_data->get_collision_polygons_count(physics_layer); i++) {
		Vector<Vector2> polygon = tile_data->get_collision_polygon_points(physics_layer, i);
		if (polygon.size() >= 3) {
			polygon_editor->add_polygon(polygon);
		}
	}

	_polygons_changed();
	dummy_object->set("linear_velocity", tile_data->get_constant_linear_velocity(physics_layer));
	dummy_object->set("angular_velocity", tile_data->get_constant_angular_velocity(physics_layer));
	for (int i = 0; i < tile_data->get_collision_polygons_count(physics_layer); i++) {
		dummy_object->set(vformat("polygon_%d_one_way", i), tile_data->is_collision_polygon_one_way(physics_layer, i));
		dummy_object->set(vformat("polygon_%d_one_way_margin", i), tile_data->get_collision_polygon_one_way_margin(physics_layer, i));
	}

	for (const HashMap<StringName, EditorProperty *>::Element *E = property_editors.front(); E; E = E->next) {
		E->value()->update_property();
	}

	polygon_editor->set_background(p_tile_set_atlas_source->get_texture(), p_tile_set_atlas_source->get_tile_texture_region(p_coords), tile_data->get_texture_origin(), tile_data->get_flip_h(), tile_data->get_flip_v(), tile_data->get_transpose(), tile_data->get_modulate());
}

void TileDataCollisionEditor::_set_value(LayeredTileSetAtlasSource *p_tile_set_atlas_source, Vector2 p_coords, int p_alternative_tile, const Variant &p_value) {
	LayeredTileData *tile_data = p_tile_set_atlas_source->get_tile_data(p_coords, p_alternative_tile);
	ERR_FAIL_NULL(tile_data);

	Dictionary dict = p_value;
	tile_data->set_constant_linear_velocity(physics_layer, dict["linear_velocity"]);
	tile_data->set_constant_angular_velocity(physics_layer, dict["angular_velocity"]);
	Array array = dict["polygons"];
	tile_data->set_collision_polygons_count(physics_layer, array.size());
	for (int i = 0; i < array.size(); i++) {
		Dictionary polygon_dict = array[i];
		tile_data->set_collision_polygon_points(physics_layer, i, polygon_dict["points"]);
		tile_data->set_collision_polygon_one_way(physics_layer, i, polygon_dict["one_way"]);
		tile_data->set_collision_polygon_one_way_margin(physics_layer, i, polygon_dict["one_way_margin"]);
	}

	polygon_editor->set_background(p_tile_set_atlas_source->get_texture(), p_tile_set_atlas_source->get_tile_texture_region(p_coords), tile_data->get_texture_origin(), tile_data->get_flip_h(), tile_data->get_flip_v(), tile_data->get_transpose(), tile_data->get_modulate());
}

Variant TileDataCollisionEditor::_get_value(LayeredTileSetAtlasSource *p_tile_set_atlas_source, Vector2 p_coords, int p_alternative_tile) {
	LayeredTileData *tile_data = p_tile_set_atlas_source->get_tile_data(p_coords, p_alternative_tile);
	ERR_FAIL_NULL_V(tile_data, Variant());

	Dictionary dict;
	dict["linear_velocity"] = tile_data->get_constant_linear_velocity(physics_layer);
	dict["angular_velocity"] = tile_data->get_constant_angular_velocity(physics_layer);
	Array array;
	for (int i = 0; i < tile_data->get_collision_polygons_count(physics_layer); i++) {
		Dictionary polygon_dict;
		polygon_dict["points"] = tile_data->get_collision_polygon_points(physics_layer, i);
		polygon_dict["one_way"] = tile_data->is_collision_polygon_one_way(physics_layer, i);
		polygon_dict["one_way_margin"] = tile_data->get_collision_polygon_one_way_margin(physics_layer, i);
		array.push_back(polygon_dict);
	}
	dict["polygons"] = array;
	return dict;
}

void TileDataCollisionEditor::_setup_undo_redo_action(LayeredTileSetAtlasSource *p_tile_set_atlas_source, const HashMap<LayeredTileMapCell, Variant, LayeredTileMapCell> &p_previous_values, const Variant &p_new_value) {
	Dictionary new_dict = p_new_value;
	UndoRedo *undo_redo = EditorNode::get_singleton()->get_undo_redo();
	for (const HashMap<LayeredTileMapCell, Variant, LayeredTileMapCell>::Element *E = p_previous_values.front(); E; E = E->next) {
		Vector2i coords = E->key().get_atlas_coords();

		Dictionary old_dict = E->value();
		undo_redo->add_undo_property(p_tile_set_atlas_source, vformat("%d:%d/%d/physics_layer_%d/linear_velocity", coords.x, coords.y, E->key().alternative_tile, physics_layer), old_dict["linear_velocity"]);
		undo_redo->add_undo_property(p_tile_set_atlas_source, vformat("%d:%d/%d/physics_layer_%d/angular_velocity", coords.x, coords.y, E->key().alternative_tile, physics_layer), old_dict["angular_velocity"]);
		Array old_polygon_array = old_dict["polygons"];
		undo_redo->add_undo_property(p_tile_set_atlas_source, vformat("%d:%d/%d/physics_layer_%d/polygons_count", coords.x, coords.y, E->key().alternative_tile, physics_layer), old_polygon_array.size());
		for (int i = 0; i < old_polygon_array.size(); i++) {
			Dictionary polygon_dict = old_polygon_array[i];
			undo_redo->add_undo_property(p_tile_set_atlas_source, vformat("%d:%d/%d/physics_layer_%d/polygon_%d/points", coords.x, coords.y, E->key().alternative_tile, physics_layer, i), polygon_dict["points"]);
			undo_redo->add_undo_property(p_tile_set_atlas_source, vformat("%d:%d/%d/physics_layer_%d/polygon_%d/one_way", coords.x, coords.y, E->key().alternative_tile, physics_layer, i), polygon_dict["one_way"]);
			undo_redo->add_undo_property(p_tile_set_atlas_source, vformat("%d:%d/%d/physics_layer_%d/polygon_%d/one_way_margin", coords.x, coords.y, E->key().alternative_tile, physics_layer, i), polygon_dict["one_way_margin"]);
		}

		undo_redo->add_do_property(p_tile_set_atlas_source, vformat("%d:%d/%d/physics_layer_%d/linear_velocity", coords.x, coords.y, E->key().alternative_tile, physics_layer), new_dict["linear_velocity"]);
		undo_redo->add_do_property(p_tile_set_atlas_source, vformat("%d:%d/%d/physics_layer_%d/angular_velocity", coords.x, coords.y, E->key().alternative_tile, physics_layer), new_dict["angular_velocity"]);
		Array new_polygon_array = new_dict["polygons"];
		undo_redo->add_do_property(p_tile_set_atlas_source, vformat("%d:%d/%d/physics_layer_%d/polygons_count", coords.x, coords.y, E->key().alternative_tile, physics_layer), new_polygon_array.size());
		for (int i = 0; i < new_polygon_array.size(); i++) {
			Dictionary polygon_dict = new_polygon_array[i];
			undo_redo->add_do_property(p_tile_set_atlas_source, vformat("%d:%d/%d/physics_layer_%d/polygon_%d/points", coords.x, coords.y, E->key().alternative_tile, physics_layer, i), polygon_dict["points"]);
			undo_redo->add_do_property(p_tile_set_atlas_source, vformat("%d:%d/%d/physics_layer_%d/polygon_%d/one_way", coords.x, coords.y, E->key().alternative_tile, physics_layer, i), polygon_dict["one_way"]);
			undo_redo->add_do_property(p_tile_set_atlas_source, vformat("%d:%d/%d/physics_layer_%d/polygon_%d/one_way_margin", coords.x, coords.y, E->key().alternative_tile, physics_layer, i), polygon_dict["one_way_margin"]);
		}
	}
}

void TileDataCollisionEditor::_tile_set_changed() {
	polygon_editor->set_tile_set(tile_set);
	_polygons_changed();
}

void TileDataCollisionEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			polygon_editor->set_polygons_color(get_tree()->get_debug_collisions_color());
		} break;
	}
}

void TileDataCollisionEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_property_selected"), &TileDataCollisionEditor::_property_selected);
	ClassDB::bind_method(D_METHOD("_polygons_changed"), &TileDataCollisionEditor::_polygons_changed);
}

TileDataCollisionEditor::TileDataCollisionEditor() {
	polygon_editor = memnew(GenericTilePolygonEditor);
	polygon_editor->set_multiple_polygon_mode(true);
	polygon_editor->connect("polygons_changed", this, "_polygons_changed");
	add_child(polygon_editor);

	dummy_object->add_dummy_property("linear_velocity");
	dummy_object->set("linear_velocity", Vector2());
	dummy_object->add_dummy_property("angular_velocity");
	dummy_object->set("angular_velocity", 0.0);

	EditorProperty *linear_velocity_editor = EditorInspector::instantiate_property_editor(dummy_object, Variant::VECTOR2, "linear_velocity", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT);
	linear_velocity_editor->set_object_and_property(dummy_object, "linear_velocity");
	linear_velocity_editor->set_label("linear_velocity");
	linear_velocity_editor->connect("property_changed", this, "_property_value_changed");
	linear_velocity_editor->connect("selected", this, "_property_selected");
	linear_velocity_editor->set_tooltip(linear_velocity_editor->get_edited_property());
	linear_velocity_editor->update_property();
	add_child(linear_velocity_editor);
	property_editors["linear_velocity"] = linear_velocity_editor;

	EditorProperty *angular_velocity_editor = EditorInspector::instantiate_property_editor(dummy_object, Variant::REAL, "angular_velocity", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT);
	angular_velocity_editor->set_object_and_property(dummy_object, "angular_velocity");
	angular_velocity_editor->set_label("angular_velocity");
	angular_velocity_editor->connect("property_changed", this, "_property_value_changed");
	angular_velocity_editor->connect("selected", this, "_property_selected");
	angular_velocity_editor->set_tooltip(angular_velocity_editor->get_edited_property());
	angular_velocity_editor->update_property();
	add_child(angular_velocity_editor);
	property_editors["angular_velocity"] = angular_velocity_editor;

	_polygons_changed();
}

TileDataCollisionEditor::~TileDataCollisionEditor() {
	memdelete(dummy_object);
}

void TileDataCollisionEditor::draw_over_tile(CanvasItem *p_canvas_item, Transform2D p_transform, LayeredTileMapCell p_cell, bool p_selected) {
	LayeredTileData *tile_data = _get_tile_data(p_cell);
	ERR_FAIL_NULL(tile_data);

	// Draw all shapes.
	Vector<Color> color;
	if (p_selected) {
		Color grid_color = EDITOR_GET("editors/layered_tiles_editor/grid_color");
		Color selection_color = Color().from_hsv(Math::fposmod(grid_color.get_h() + 0.5, 1.0), grid_color.get_s(), grid_color.get_v(), 1.0);
		selection_color.a = 0.7;
		color.push_back(selection_color);
	} else {
		Color debug_collision_color = p_canvas_item->get_tree()->get_debug_collisions_color();
		color.push_back(debug_collision_color);
	}

	RenderingServer::get_singleton()->canvas_item_add_set_transform(p_canvas_item->get_canvas_item(), p_transform);

	Ref<Texture> one_way_icon = get_theme_icon("OneWayTile", "EditorIcons");
	for (int i = 0; i < tile_data->get_collision_polygons_count(physics_layer); i++) {
		Vector<Vector2> polygon = tile_data->get_collision_polygon_points(physics_layer, i);
		if (polygon.size() < 3) {
			continue;
		}

		p_canvas_item->draw_polygon(polygon, color);

		if (tile_data->is_collision_polygon_one_way(physics_layer, i)) {
			Vector<Vector2> uvs;
			uvs.resize(polygon.size());
			Vector2 size_1 = Vector2(1, 1) / tile_set->get_tile_size();

			for (int j = 0; j < polygon.size(); j++) {
				uvs.write[j] = polygon[j] * size_1 + Vector2(0.5, 0.5);
			}

			Vector<Color> color2;
			color2.push_back(Color(1, 1, 1, 0.4));
			p_canvas_item->draw_polygon(polygon, color2, uvs, one_way_icon);
		}
	}

	RenderingServer::get_singleton()->canvas_item_add_set_transform(p_canvas_item->get_canvas_item(), Transform2D());
}

void TileDataTerrainsEditor::_update_terrain_selector() {
	ERR_FAIL_COND(!tile_set.is_valid());

	// Update the terrain set selector.
	Vector<String> options;
	options.push_back(String(TTR("No terrains")) + String(":-1"));
	for (int i = 0; i < tile_set->get_terrain_sets_count(); i++) {
		options.push_back(vformat("Terrain Set %d", i));
	}
	terrain_set_property_editor->setup(options);
	terrain_set_property_editor->update_property();

	// Update the terrain selector.
	int terrain_set = int(dummy_object->get("terrain_set"));
	if (terrain_set == -1) {
		terrain_property_editor->hide();
	} else {
		options.clear();
		Vector<Vector<Ref<Texture>>> icons = tile_set->generate_terrains_icons(Size2(16, 16) * EDSCALE);
		options.push_back(String(TTR("No terrain")) + String(":-1"));
		for (int i = 0; i < tile_set->get_terrains_count(terrain_set); i++) {
			String name = tile_set->get_terrain_name(terrain_set, i);
			if (name.empty()) {
				options.push_back(vformat("Terrain %d", i));
			} else {
				options.push_back(name);
			}
		}
		terrain_property_editor->setup(options);
		terrain_property_editor->update_property();

		// Kind of a hack to set icons.
		// We could provide a way to modify that in the EditorProperty.
		OptionButton *option_button = Object::cast_to<OptionButton>(terrain_property_editor->get_child(0));
		for (int terrain = 0; terrain < tile_set->get_terrains_count(terrain_set); terrain++) {
			option_button->set_item_icon(terrain + 1, icons[terrain_set][terrain]);
		}
		terrain_property_editor->show();
	}
}

void TileDataTerrainsEditor::_property_value_changed(const StringName &p_property, const Variant &p_value, const StringName &p_field, bool p_changing) {
	Variant old_value = dummy_object->get(p_property);
	dummy_object->set(p_property, p_value);
	if (p_property == "terrain_set") {
		if (p_value != old_value) {
			dummy_object->set("terrain", -1);
		}
		_update_terrain_selector();
	}
	emit_signal("needs_redraw");
}

void TileDataTerrainsEditor::_tile_set_changed() {
	ERR_FAIL_COND(!tile_set.is_valid());

	// Fix if wrong values are selected.
	int terrain_set = int(dummy_object->get("terrain_set"));
	if (terrain_set >= tile_set->get_terrain_sets_count()) {
		terrain_set = -1;
		dummy_object->set("terrain_set", -1);
	}
	if (terrain_set >= 0) {
		if (int(dummy_object->get("terrain")) >= tile_set->get_terrains_count(terrain_set)) {
			dummy_object->set("terrain", -1);
		}
	}

	_update_terrain_selector();
}

void TileDataTerrainsEditor::forward_draw_over_atlas(LayeredTileAtlasView *p_tile_atlas_view, LayeredTileSetAtlasSource *p_tile_set_atlas_source, CanvasItem *p_canvas_item, Transform2D p_transform) {
	ERR_FAIL_COND(!tile_set.is_valid());

	// Draw the hovered terrain bit, or the whole tile if it has the wrong terrain set.
	Vector2i hovered_coords = LayeredTileSetSource::INVALID_ATLAS_COORDS;
	if (drag_type == DRAG_TYPE_NONE) {
		Vector2i mouse_pos = p_transform.affine_inverse().xform(p_canvas_item->get_local_mouse_position());
		hovered_coords = p_tile_atlas_view->get_atlas_tile_coords_at_pos(mouse_pos);
		hovered_coords = p_tile_set_atlas_source->get_tile_at_coords(hovered_coords);
		if (hovered_coords != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
			LayeredTileData *tile_data = p_tile_set_atlas_source->get_tile_data(hovered_coords, 0);
			int terrain_set = tile_data->get_terrain_set();
			Rect2i texture_region = p_tile_set_atlas_source->get_tile_texture_region(hovered_coords);
			Vector2i position = texture_region.get_center() + tile_data->get_texture_origin();

			if (terrain_set >= 0 && terrain_set == int(dummy_object->get("terrain_set"))) {
				// Draw hovered bit.
				Transform2D xform;
				xform.set_origin(position);

				Vector<Color> color;
				color.push_back(Color(1.0, 1.0, 1.0, 0.5));

				Vector<Vector2> polygon = tile_set->get_terrain_polygon(terrain_set);
				if (Geometry::is_point_in_polygon(xform.affine_inverse().xform(mouse_pos), polygon)) {
					p_canvas_item->draw_set_transform_matrix(p_transform * xform);
					p_canvas_item->draw_polygon(polygon, color);
				}
				for (int i = 0; i < LayeredTileSet::CELL_NEIGHBOR_MAX; i++) {
					LayeredTileSet::CellNeighbor bit = LayeredTileSet::CellNeighbor(i);
					if (tile_set->is_valid_terrain_peering_bit(terrain_set, bit)) {
						polygon = tile_set->get_terrain_peering_bit_polygon(terrain_set, bit);
						if (Geometry::is_point_in_polygon(xform.affine_inverse().xform(mouse_pos), polygon)) {
							p_canvas_item->draw_set_transform_matrix(p_transform * xform);
							p_canvas_item->draw_polygon(polygon, color);
						}
					}
				}
			} else {
				// Draw hovered tile.
				Transform2D tile_xform;
				tile_xform.set_origin(position);
				tile_xform.set_scale(tile_set->get_tile_size());
				tile_set->draw_tile_shape(p_canvas_item, p_transform * tile_xform, Color(1.0, 1.0, 1.0, 0.5), true);
			}
		}
	}

	// Dim terrains with wrong terrain set.
	Ref<Font> font = LayeredTileSetEditor::get_singleton()->get_theme_font("bold", "EditorFonts");
	for (int i = 0; i < p_tile_set_atlas_source->get_tiles_count(); i++) {
		Vector2i coords = p_tile_set_atlas_source->get_tile_id(i);
		if (coords != hovered_coords) {
			LayeredTileData *tile_data = p_tile_set_atlas_source->get_tile_data(coords, 0);
			if (tile_data->get_terrain_set() != int(dummy_object->get("terrain_set"))) {
				// Dimming
				p_canvas_item->draw_set_transform_matrix(p_transform);
				Rect2i rect = p_tile_set_atlas_source->get_tile_texture_region(coords);
				p_canvas_item->draw_rect(rect, Color(0.0, 0.0, 0.0, 0.3));

				// Text
				p_canvas_item->draw_set_transform_matrix(Transform2D());
				Rect2i texture_region = p_tile_set_atlas_source->get_tile_texture_region(coords);
				Vector2i position = texture_region.get_center() + tile_data->get_texture_origin();

				Color color = Color(1, 1, 1);
				String text;
				if (tile_data->get_terrain_set() >= 0) {
					text = vformat("%d", tile_data->get_terrain_set());
				} else {
					text = "-";
				}
				Vector2 string_size = font->get_string_size(text);

				p_canvas_item->draw_string(font, p_transform.xform(position) + Vector2i(-string_size.x / 2, string_size.y / 2), text, color);
			}
		}
	}
	p_canvas_item->draw_set_transform_matrix(Transform2D());

	if (drag_type == DRAG_TYPE_PAINT_TERRAIN_SET_RECT) {
		// Draw selection rectangle.
		Color grid_color = EDITOR_GET("editors/layered_tiles_editor/grid_color");
		Color selection_color = Color().from_hsv(Math::fposmod(grid_color.get_h() + 0.5, 1.0), grid_color.get_s(), grid_color.get_v(), 1.0);

		p_canvas_item->draw_set_transform_matrix(p_transform);

		Rect2i rect;
		rect.set_position(p_tile_atlas_view->get_atlas_tile_coords_at_pos(drag_start_pos, true));
		rect.set_end(p_tile_atlas_view->get_atlas_tile_coords_at_pos(p_transform.affine_inverse().xform(p_canvas_item->get_local_mouse_position()), true));
		rect = rect.abs();

		RBSet<LayeredTileMapCell> edited;
		for (int x = rect.get_position().x; x <= rect.get_end().x; x++) {
			for (int y = rect.get_position().y; y <= rect.get_end().y; y++) {
				Vector2i coords = Vector2i(x, y);
				coords = p_tile_set_atlas_source->get_tile_at_coords(coords);
				if (coords != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
					LayeredTileMapCell cell;
					cell.source_id = 0;
					cell.set_atlas_coords(coords);
					cell.alternative_tile = 0;
					edited.insert(cell);
				}
			}
		}

		for (const RBSet<LayeredTileMapCell>::Element *E = edited.front(); E; E = E->next()) {
			Vector2i coords = E->get().get_atlas_coords();
			p_canvas_item->draw_rect(p_tile_set_atlas_source->get_tile_texture_region(coords), selection_color, false);
		}

		p_canvas_item->draw_set_transform_matrix(Transform2D());
	} else if (drag_type == DRAG_TYPE_PAINT_TERRAIN_BITS_RECT) {
		// Highlight selected peering bits.
		Dictionary painted = Dictionary(drag_painted_value);
		int terrain_set = int(painted["terrain_set"]);

		Rect2i rect;
		rect.set_position(p_tile_atlas_view->get_atlas_tile_coords_at_pos(drag_start_pos, true));
		rect.set_end(p_tile_atlas_view->get_atlas_tile_coords_at_pos(p_transform.affine_inverse().xform(p_canvas_item->get_local_mouse_position()), true));
		rect = rect.abs();

		RBSet<LayeredTileMapCell> edited;
		for (int x = rect.get_position().x; x <= rect.get_end().x; x++) {
			for (int y = rect.get_position().y; y <= rect.get_end().y; y++) {
				Vector2i coords = Vector2i(x, y);
				coords = p_tile_set_atlas_source->get_tile_at_coords(coords);
				if (coords != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
					LayeredTileData *tile_data = p_tile_set_atlas_source->get_tile_data(coords, 0);
					if (tile_data->get_terrain_set() == terrain_set) {
						LayeredTileMapCell cell;
						cell.source_id = 0;
						cell.set_atlas_coords(coords);
						cell.alternative_tile = 0;
						edited.insert(cell);
					}
				}
			}
		}

		Vector2 end = p_transform.affine_inverse().xform(p_canvas_item->get_local_mouse_position());
		Vector<Point2> mouse_pos_rect_polygon;
		mouse_pos_rect_polygon.push_back(drag_start_pos);
		mouse_pos_rect_polygon.push_back(Vector2(end.x, drag_start_pos.y));
		mouse_pos_rect_polygon.push_back(end);
		mouse_pos_rect_polygon.push_back(Vector2(drag_start_pos.x, end.y));

		Vector<Color> color;
		color.push_back(Color(1.0, 1.0, 1.0, 0.5));

		p_canvas_item->draw_set_transform_matrix(p_transform);

		for (const RBSet<LayeredTileMapCell>::Element *E = edited.front(); E; E = E->next()) {
			Vector2i coords = E->get().get_atlas_coords();

			Rect2i texture_region = p_tile_set_atlas_source->get_tile_texture_region(coords);
			Vector2i position = texture_region.get_center() + p_tile_set_atlas_source->get_tile_data(coords, 0)->get_texture_origin();

			Vector<Vector2> polygon = tile_set->get_terrain_polygon(terrain_set);
			for (int j = 0; j < polygon.size(); j++) {
				polygon.write[j] += position;
			}
			if (!Geometry::intersect_polygons_2d(polygon, mouse_pos_rect_polygon).empty()) {
				// Draw terrain.
				p_canvas_item->draw_polygon(polygon, color);
			}

			for (int i = 0; i < LayeredTileSet::CELL_NEIGHBOR_MAX; i++) {
				LayeredTileSet::CellNeighbor bit = LayeredTileSet::CellNeighbor(i);
				if (tile_set->is_valid_terrain_peering_bit(terrain_set, bit)) {
					polygon = tile_set->get_terrain_peering_bit_polygon(terrain_set, bit);
					for (int j = 0; j < polygon.size(); j++) {
						polygon.write[j] += position;
					}
					if (!Geometry::intersect_polygons_2d(polygon, mouse_pos_rect_polygon).empty()) {
						// Draw bit.
						p_canvas_item->draw_polygon(polygon, color);
					}
				}
			}
		}

		p_canvas_item->draw_set_transform_matrix(Transform2D());
	}
}

void TileDataTerrainsEditor::forward_draw_over_alternatives(LayeredTileAtlasView *p_tile_atlas_view, LayeredTileSetAtlasSource *p_tile_set_atlas_source, CanvasItem *p_canvas_item, Transform2D p_transform) {
	ERR_FAIL_COND(!tile_set.is_valid());

	// Draw the hovered terrain bit, or the whole tile if it has the wrong terrain set.
	Vector2i hovered_coords = LayeredTileSetSource::INVALID_ATLAS_COORDS;
	int hovered_alternative = LayeredTileSetSource::INVALID_TILE_ALTERNATIVE;
	if (drag_type == DRAG_TYPE_NONE) {
		Vector2i mouse_pos = p_transform.affine_inverse().xform(p_canvas_item->get_local_mouse_position());
		Vector3i hovered = p_tile_atlas_view->get_alternative_tile_at_pos(mouse_pos);
		hovered_coords = Vector2i(hovered.x, hovered.y);
		hovered_alternative = hovered.z;
		if (hovered_coords != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
			LayeredTileData *tile_data = p_tile_set_atlas_source->get_tile_data(hovered_coords, hovered_alternative);
			int terrain_set = tile_data->get_terrain_set();
			Rect2i texture_region = p_tile_atlas_view->get_alternative_tile_rect(hovered_coords, hovered_alternative);
			Vector2i position = texture_region.get_center() + tile_data->get_texture_origin();

			if (terrain_set == int(dummy_object->get("terrain_set"))) {
				// Draw hovered bit.
				Transform2D xform;
				xform.set_origin(position);

				Vector<Color> color;
				color.push_back(Color(1.0, 1.0, 1.0, 0.5));

				Vector<Vector2> polygon = tile_set->get_terrain_polygon(terrain_set);
				if (Geometry::is_point_in_polygon(xform.affine_inverse().xform(mouse_pos), polygon)) {
					p_canvas_item->draw_set_transform_matrix(p_transform * xform);
					p_canvas_item->draw_polygon(polygon, color);
				}

				for (int i = 0; i < LayeredTileSet::CELL_NEIGHBOR_MAX; i++) {
					LayeredTileSet::CellNeighbor bit = LayeredTileSet::CellNeighbor(i);
					if (tile_set->is_valid_terrain_peering_bit(terrain_set, bit)) {
						polygon = tile_set->get_terrain_peering_bit_polygon(terrain_set, bit);
						if (Geometry::is_point_in_polygon(xform.affine_inverse().xform(mouse_pos), polygon)) {
							p_canvas_item->draw_set_transform_matrix(p_transform * xform);
							p_canvas_item->draw_polygon(polygon, color);
						}
					}
				}
			} else {
				// Draw hovered tile.
				Transform2D tile_xform;
				tile_xform.set_origin(position);
				tile_xform.set_scale(tile_set->get_tile_size());
				tile_set->draw_tile_shape(p_canvas_item, p_transform * tile_xform, Color(1.0, 1.0, 1.0, 0.5), true);
			}
		}
	}

	// Dim terrains with wrong terrain set.
	Ref<Font> font = LayeredTileSetEditor::get_singleton()->get_theme_font("bold", "EditorFonts");

	for (int i = 0; i < p_tile_set_atlas_source->get_tiles_count(); i++) {
		Vector2i coords = p_tile_set_atlas_source->get_tile_id(i);
		for (int j = 1; j < p_tile_set_atlas_source->get_alternative_tiles_count(coords); j++) {
			int alternative_tile = p_tile_set_atlas_source->get_alternative_tile_id(coords, j);
			if (coords != hovered_coords || alternative_tile != hovered_alternative) {
				LayeredTileData *tile_data = p_tile_set_atlas_source->get_tile_data(coords, alternative_tile);
				if (tile_data->get_terrain_set() != int(dummy_object->get("terrain_set"))) {
					// Dimming
					p_canvas_item->draw_set_transform_matrix(p_transform);
					Rect2i rect = p_tile_atlas_view->get_alternative_tile_rect(coords, alternative_tile);
					p_canvas_item->draw_rect(rect, Color(0.0, 0.0, 0.0, 0.3));

					// Text
					p_canvas_item->draw_set_transform_matrix(Transform2D());
					Rect2i texture_region = p_tile_atlas_view->get_alternative_tile_rect(coords, alternative_tile);
					Vector2i position = texture_region.get_center() + tile_data->get_texture_origin();

					Color color = Color(1, 1, 1);
					String text;
					if (tile_data->get_terrain_set() >= 0) {
						text = vformat("%d", tile_data->get_terrain_set());
					} else {
						text = "-";
					}
					Vector2 string_size = font->get_string_size(text);

					p_canvas_item->draw_string(font, p_transform.xform(position) + Vector2i(-string_size.x / 2, string_size.y / 2), text, color);
				}
			}
		}
	}

	p_canvas_item->draw_set_transform_matrix(Transform2D());
}

void TileDataTerrainsEditor::forward_painting_atlas_gui_input(LayeredTileAtlasView *p_tile_atlas_view, LayeredTileSetAtlasSource *p_tile_set_atlas_source, const Ref<InputEvent> &p_event) {
	Ref<InputEventMouseMotion> mm = p_event;
	if (mm.is_valid()) {
		if (drag_type == DRAG_TYPE_PAINT_TERRAIN_SET) {
			Vector2i p0 = p_tile_atlas_view->get_atlas_tile_coords_at_pos(drag_last_pos, true);
			Vector2i p1 = p_tile_atlas_view->get_atlas_tile_coords_at_pos(mm->get_position(), true);

			Vector<Vector2i> line = Geometry::bresenham_line(p0.x, p1.x, p0.y, p1.y);

			for (int i = 0; i < line.size(); i++) {
				Vector2i coords = p_tile_set_atlas_source->get_tile_at_coords(line[i]);
				if (coords != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
					int terrain_set = drag_painted_value;
					LayeredTileMapCell cell;
					cell.source_id = 0;
					cell.set_atlas_coords(coords);
					cell.alternative_tile = 0;

					// Save the old terrain_set and terrains bits.
					LayeredTileData *tile_data = p_tile_set_atlas_source->get_tile_data(coords, 0);
					if (!drag_modified.has(cell)) {
						Dictionary dict;
						dict["terrain_set"] = tile_data->get_terrain_set();
						dict["terrain"] = tile_data->get_terrain();
						Array array;
						for (int j = 0; j < LayeredTileSet::CELL_NEIGHBOR_MAX; j++) {
							LayeredTileSet::CellNeighbor bit = LayeredTileSet::CellNeighbor(j);
							array.push_back(tile_data->is_valid_terrain_peering_bit(bit) ? tile_data->get_terrain_peering_bit(bit) : -1);
						}
						dict["terrain_peering_bits"] = array;
						drag_modified[cell] = dict;
					}

					// Set the terrain_set.
					tile_data->set_terrain_set(terrain_set);
				}
			}
			drag_last_pos = mm->get_position();
			accept_event();
		} else if (drag_type == DRAG_TYPE_PAINT_TERRAIN_BITS) {
			int terrain_set = Dictionary(drag_painted_value)["terrain_set"];
			int terrain = Dictionary(drag_painted_value)["terrain"];

			Vector2i p0 = p_tile_atlas_view->get_atlas_tile_coords_at_pos(drag_last_pos, true);
			Vector2i p1 = p_tile_atlas_view->get_atlas_tile_coords_at_pos(mm->get_position(), true);

			Vector<Vector2i> line = Geometry::bresenham_line(p0.x, p1.x, p0.y, p1.y);

			for (int i = 0; i < line.size(); i++) {
				Vector2i coords = p_tile_set_atlas_source->get_tile_at_coords(line[i]);
				if (coords != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
					LayeredTileMapCell cell;
					cell.source_id = 0;
					cell.set_atlas_coords(coords);
					cell.alternative_tile = 0;

					LayeredTileData *tile_data = p_tile_set_atlas_source->get_tile_data(coords, 0);
					if (tile_data->get_terrain_set() == terrain_set) {
						// Save the old terrain_set and terrains bits.
						if (!drag_modified.has(cell)) {
							Dictionary dict;
							dict["terrain_set"] = tile_data->get_terrain_set();
							dict["terrain"] = tile_data->get_terrain();
							Array array;
							for (int j = 0; j < LayeredTileSet::CELL_NEIGHBOR_MAX; j++) {
								LayeredTileSet::CellNeighbor bit = LayeredTileSet::CellNeighbor(j);
								array.push_back(tile_data->is_valid_terrain_peering_bit(bit) ? tile_data->get_terrain_peering_bit(bit) : -1);
							}
							dict["terrain_peering_bits"] = array;
							drag_modified[cell] = dict;
						}

						// Set the terrains bits.
						Rect2i texture_region = p_tile_set_atlas_source->get_tile_texture_region(coords);
						Vector2i position = texture_region.get_center() + tile_data->get_texture_origin();

						Vector<Vector2> polygon = tile_set->get_terrain_polygon(tile_data->get_terrain_set());
						if (Geometry::is_segment_intersecting_polygon(mm->get_position() - position, drag_last_pos - position, polygon)) {
							tile_data->set_terrain(terrain);
						}
						for (int j = 0; j < LayeredTileSet::CELL_NEIGHBOR_MAX; j++) {
							LayeredTileSet::CellNeighbor bit = LayeredTileSet::CellNeighbor(j);
							if (tile_data->is_valid_terrain_peering_bit(bit)) {
								polygon = tile_set->get_terrain_peering_bit_polygon(tile_data->get_terrain_set(), bit);
								if (Geometry::is_segment_intersecting_polygon(mm->get_position() - position, drag_last_pos - position, polygon)) {
									tile_data->set_terrain_peering_bit(bit, terrain);
								}
							}
						}
					}
				}
			}
			drag_last_pos = mm->get_position();
			accept_event();
		}
	}

	Ref<InputEventMouseButton> mb = p_event;
	if (mb.is_valid()) {
		if (mb->get_button_index() == BUTTON_LEFT || mb->get_button_index() == BUTTON_RIGHT) {
			if (mb->is_pressed()) {
				if (picker_button->is_pressed() || (mb->get_control() && !mb->get_shift())) {
					Vector2i coords = p_tile_atlas_view->get_atlas_tile_coords_at_pos(mb->get_position());
					coords = p_tile_set_atlas_source->get_tile_at_coords(coords);
					if (coords != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
						LayeredTileData *tile_data = p_tile_set_atlas_source->get_tile_data(coords, 0);
						int terrain_set = tile_data->get_terrain_set();
						Rect2i texture_region = p_tile_set_atlas_source->get_tile_texture_region(coords);
						Vector2i position = texture_region.get_center() + tile_data->get_texture_origin();
						dummy_object->set("terrain_set", terrain_set);
						dummy_object->set("terrain", -1);

						Vector<Vector2> polygon = tile_set->get_terrain_polygon(terrain_set);
						if (Geometry::is_point_in_polygon(mb->get_position() - position, polygon)) {
							dummy_object->set("terrain", tile_data->get_terrain());
						}
						for (int i = 0; i < LayeredTileSet::CELL_NEIGHBOR_MAX; i++) {
							LayeredTileSet::CellNeighbor bit = LayeredTileSet::CellNeighbor(i);
							if (tile_set->is_valid_terrain_peering_bit(terrain_set, bit)) {
								polygon = tile_set->get_terrain_peering_bit_polygon(terrain_set, bit);
								if (Geometry::is_point_in_polygon(mb->get_position() - position, polygon)) {
									dummy_object->set("terrain", tile_data->get_terrain_peering_bit(bit));
								}
							}
						}
						terrain_set_property_editor->update_property();
						_update_terrain_selector();
						picker_button->set_pressed(false);
						accept_event();
					}
				} else {
					Vector2i coords = p_tile_atlas_view->get_atlas_tile_coords_at_pos(mb->get_position());
					coords = p_tile_set_atlas_source->get_tile_at_coords(coords);
					LayeredTileData *tile_data = nullptr;
					if (coords != LayeredTileSetAtlasSource::INVALID_ATLAS_COORDS) {
						tile_data = p_tile_set_atlas_source->get_tile_data(coords, 0);
					}
					int terrain_set = int(dummy_object->get("terrain_set"));
					int terrain = int(dummy_object->get("terrain"));
					if (terrain_set == -1 || !tile_data || tile_data->get_terrain_set() != terrain_set) {
						// Paint terrain sets.
						if (mb->get_button_index() == BUTTON_RIGHT) {
							terrain_set = -1;
						}
						if (mb->get_control() && mb->get_shift()) {
							// Paint terrain set with rect.
							drag_type = DRAG_TYPE_PAINT_TERRAIN_SET_RECT;
							drag_modified.clear();
							drag_painted_value = terrain_set;
							drag_start_pos = mb->get_position();
						} else {
							// Paint terrain set.
							drag_type = DRAG_TYPE_PAINT_TERRAIN_SET;
							drag_modified.clear();
							drag_painted_value = terrain_set;

							if (coords != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
								LayeredTileMapCell cell;
								cell.source_id = 0;
								cell.set_atlas_coords(coords);
								cell.alternative_tile = 0;

								// Save the old terrain_set and terrains bits.
								Dictionary dict;
								dict["terrain_set"] = tile_data->get_terrain_set();
								dict["terrain"] = tile_data->get_terrain();
								Array array;
								for (int i = 0; i < LayeredTileSet::CELL_NEIGHBOR_MAX; i++) {
									LayeredTileSet::CellNeighbor bit = LayeredTileSet::CellNeighbor(i);
									array.push_back(tile_data->is_valid_terrain_peering_bit(bit) ? tile_data->get_terrain_peering_bit(bit) : -1);
								}
								dict["terrain_peering_bits"] = array;
								drag_modified[cell] = dict;

								// Set the terrain_set.
								tile_data->set_terrain_set(terrain_set);
							}
							drag_last_pos = mb->get_position();
						}
						accept_event();
					} else if (tile_data->get_terrain_set() == terrain_set) {
						// Paint terrain bits.
						if (mb->get_button_index() == BUTTON_RIGHT) {
							terrain = -1;
						}
						if (mb->get_control() && mb->get_shift()) {
							// Paint terrain bits with rect.
							drag_type = DRAG_TYPE_PAINT_TERRAIN_BITS_RECT;
							drag_modified.clear();
							Dictionary painted_dict;
							painted_dict["terrain_set"] = terrain_set;
							painted_dict["terrain"] = terrain;
							drag_painted_value = painted_dict;
							drag_start_pos = mb->get_position();
						} else {
							// Paint terrain bits.
							drag_type = DRAG_TYPE_PAINT_TERRAIN_BITS;
							drag_modified.clear();
							Dictionary painted_dict;
							painted_dict["terrain_set"] = terrain_set;
							painted_dict["terrain"] = terrain;
							drag_painted_value = painted_dict;

							if (coords != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
								LayeredTileMapCell cell;
								cell.source_id = 0;
								cell.set_atlas_coords(coords);
								cell.alternative_tile = 0;

								// Save the old terrain_set and terrains bits.
								Dictionary dict;
								dict["terrain_set"] = tile_data->get_terrain_set();
								dict["terrain"] = tile_data->get_terrain();
								Array array;
								for (int i = 0; i < LayeredTileSet::CELL_NEIGHBOR_MAX; i++) {
									LayeredTileSet::CellNeighbor bit = LayeredTileSet::CellNeighbor(i);
									array.push_back(tile_data->is_valid_terrain_peering_bit(bit) ? tile_data->get_terrain_peering_bit(bit) : -1);
								}
								dict["terrain_peering_bits"] = array;
								drag_modified[cell] = dict;

								// Set the terrain bit.
								Rect2i texture_region = p_tile_set_atlas_source->get_tile_texture_region(coords);
								Vector2i position = texture_region.get_center() + tile_data->get_texture_origin();

								Vector<Vector2> polygon = tile_set->get_terrain_polygon(terrain_set);
								if (Geometry::is_point_in_polygon(mb->get_position() - position, polygon)) {
									tile_data->set_terrain(terrain);
								}
								for (int i = 0; i < LayeredTileSet::CELL_NEIGHBOR_MAX; i++) {
									LayeredTileSet::CellNeighbor bit = LayeredTileSet::CellNeighbor(i);
									if (tile_set->is_valid_terrain_peering_bit(terrain_set, bit)) {
										polygon = tile_set->get_terrain_peering_bit_polygon(terrain_set, bit);
										if (Geometry::is_point_in_polygon(mb->get_position() - position, polygon)) {
											tile_data->set_terrain_peering_bit(bit, terrain);
										}
									}
								}
							}
							drag_last_pos = mb->get_position();
						}
						accept_event();
					}
				}
			} else {
				UndoRedo *undo_redo = EditorNode::get_singleton()->get_undo_redo();
				if (drag_type == DRAG_TYPE_PAINT_TERRAIN_SET_RECT) {
					Rect2i rect;
					rect.set_position(p_tile_atlas_view->get_atlas_tile_coords_at_pos(drag_start_pos, true));
					rect.set_end(p_tile_atlas_view->get_atlas_tile_coords_at_pos(mb->get_position(), true));
					rect = rect.abs();

					RBSet<LayeredTileMapCell> edited;
					for (int x = rect.get_position().x; x <= rect.get_end().x; x++) {
						for (int y = rect.get_position().y; y <= rect.get_end().y; y++) {
							Vector2i coords = Vector2i(x, y);
							coords = p_tile_set_atlas_source->get_tile_at_coords(coords);
							if (coords != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
								LayeredTileMapCell cell;
								cell.source_id = 0;
								cell.set_atlas_coords(coords);
								cell.alternative_tile = 0;
								edited.insert(cell);
							}
						}
					}
					undo_redo->create_action(TTR("Painting Terrain Set"));
					for (const RBSet<LayeredTileMapCell>::Element *E = edited.front(); E; E = E->next()) {
						Vector2i coords = E->get().get_atlas_coords();
						LayeredTileData *tile_data = p_tile_set_atlas_source->get_tile_data(coords, 0);
						undo_redo->add_do_property(p_tile_set_atlas_source, vformat("%d:%d/%d/terrain_set", coords.x, coords.y, E->get().alternative_tile), drag_painted_value);
						undo_redo->add_undo_property(p_tile_set_atlas_source, vformat("%d:%d/%d/terrain_set", coords.x, coords.y, E->get().alternative_tile), tile_data->get_terrain_set());
						if (tile_data->get_terrain_set() >= 0) {
							undo_redo->add_undo_property(p_tile_set_atlas_source, vformat("%d:%d/%d/terrain", coords.x, coords.y, E->get().alternative_tile), tile_data->get_terrain());
							for (int i = 0; i < LayeredTileSet::CELL_NEIGHBOR_MAX; i++) {
								LayeredTileSet::CellNeighbor bit = LayeredTileSet::CellNeighbor(i);
								if (tile_data->is_valid_terrain_peering_bit(bit)) {
									undo_redo->add_undo_property(p_tile_set_atlas_source, vformat("%d:%d/%d/terrains_peering_bit/" + String(LayeredTileSet::CELL_NEIGHBOR_ENUM_TO_TEXT[i]), coords.x, coords.y, E->get().alternative_tile), tile_data->get_terrain_peering_bit(bit));
								}
							}
						}
					}
					undo_redo->commit_action();
					drag_type = DRAG_TYPE_NONE;
					accept_event();
				} else if (drag_type == DRAG_TYPE_PAINT_TERRAIN_SET) {
					undo_redo->create_action(TTR("Painting Terrain Set"));

					for (HashMap<LayeredTileMapCell, Variant, LayeredTileMapCell>::Element *E = drag_modified.front(); E; E = E->next) {
						Dictionary dict = E->value();
						Vector2i coords = E->key().get_atlas_coords();
						undo_redo->add_do_property(p_tile_set_atlas_source, vformat("%d:%d/%d/terrain_set", coords.x, coords.y, E->key().alternative_tile), drag_painted_value);
						undo_redo->add_undo_property(p_tile_set_atlas_source, vformat("%d:%d/%d/terrain_set", coords.x, coords.y, E->key().alternative_tile), dict["terrain_set"]);
						if (int(dict["terrain_set"]) >= 0) {
							undo_redo->add_undo_property(p_tile_set_atlas_source, vformat("%d:%d/%d/terrain", coords.x, coords.y, E->key().alternative_tile), dict["terrain"]);
							Array array = dict["terrain_peering_bits"];
							for (int i = 0; i < array.size(); i++) {
								LayeredTileSet::CellNeighbor bit = LayeredTileSet::CellNeighbor(i);
								if (tile_set->is_valid_terrain_peering_bit(dict["terrain_set"], bit)) {
									undo_redo->add_undo_property(p_tile_set_atlas_source, vformat("%d:%d/%d/terrains_peering_bit/" + String(LayeredTileSet::CELL_NEIGHBOR_ENUM_TO_TEXT[i]), coords.x, coords.y, E->key().alternative_tile), array[i]);
								}
							}
						}
					}
					undo_redo->commit_action();
					drag_type = DRAG_TYPE_NONE;
					accept_event();
				} else if (drag_type == DRAG_TYPE_PAINT_TERRAIN_BITS) {
					Dictionary painted = Dictionary(drag_painted_value);
					int terrain_set = int(painted["terrain_set"]);
					int terrain = int(painted["terrain"]);
					undo_redo->create_action(TTR("Painting Terrain"));
					for (HashMap<LayeredTileMapCell, Variant, LayeredTileMapCell>::Element *E = drag_modified.front(); E; E = E->next) {
						Dictionary dict = E->value();
						Vector2i coords = E->key().get_atlas_coords();

						LayeredTileData *tile_data = p_tile_set_atlas_source->get_tile_data(coords, E->key().alternative_tile);

						undo_redo->add_do_property(p_tile_set_atlas_source, vformat("%d:%d/%d/terrain", coords.x, coords.y, E->key().alternative_tile), terrain);
						undo_redo->add_undo_property(p_tile_set_atlas_source, vformat("%d:%d/%d/terrain", coords.x, coords.y, E->key().alternative_tile), dict["terrain"]);
						Array array = dict["terrain_peering_bits"];

						for (int i = 0; i < array.size(); i++) {
							LayeredTileSet::CellNeighbor bit = LayeredTileSet::CellNeighbor(i);
							if (tile_set->is_valid_terrain_peering_bit(terrain_set, bit)) {
								undo_redo->add_do_property(p_tile_set_atlas_source, vformat("%d:%d/%d/terrains_peering_bit/" + String(LayeredTileSet::CELL_NEIGHBOR_ENUM_TO_TEXT[i]), coords.x, coords.y, E->key().alternative_tile), tile_data->get_terrain_peering_bit(bit));
							}
							if (tile_set->is_valid_terrain_peering_bit(dict["terrain_set"], bit)) {
								undo_redo->add_undo_property(p_tile_set_atlas_source, vformat("%d:%d/%d/terrains_peering_bit/" + String(LayeredTileSet::CELL_NEIGHBOR_ENUM_TO_TEXT[i]), coords.x, coords.y, E->key().alternative_tile), array[i]);
							}
						}
					}
					undo_redo->commit_action();
					drag_type = DRAG_TYPE_NONE;
					accept_event();
				} else if (drag_type == DRAG_TYPE_PAINT_TERRAIN_BITS_RECT) {
					Dictionary painted = Dictionary(drag_painted_value);
					int terrain_set = int(painted["terrain_set"]);
					int terrain = int(painted["terrain"]);

					Rect2i rect;
					rect.set_position(p_tile_atlas_view->get_atlas_tile_coords_at_pos(drag_start_pos, true));
					rect.set_end(p_tile_atlas_view->get_atlas_tile_coords_at_pos(mb->get_position(), true));
					rect = rect.abs();

					RBSet<LayeredTileMapCell> edited;
					for (int x = rect.get_position().x; x <= rect.get_end().x; x++) {
						for (int y = rect.get_position().y; y <= rect.get_end().y; y++) {
							Vector2i coords = Vector2i(x, y);
							coords = p_tile_set_atlas_source->get_tile_at_coords(coords);
							if (coords != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
								LayeredTileData *tile_data = p_tile_set_atlas_source->get_tile_data(coords, 0);
								if (tile_data->get_terrain_set() == terrain_set) {
									LayeredTileMapCell cell;
									cell.source_id = 0;
									cell.set_atlas_coords(coords);
									cell.alternative_tile = 0;
									edited.insert(cell);
								}
							}
						}
					}

					Vector<Point2> mouse_pos_rect_polygon;
					mouse_pos_rect_polygon.push_back(drag_start_pos);
					mouse_pos_rect_polygon.push_back(Vector2(mb->get_position().x, drag_start_pos.y));
					mouse_pos_rect_polygon.push_back(mb->get_position());
					mouse_pos_rect_polygon.push_back(Vector2(drag_start_pos.x, mb->get_position().y));

					undo_redo->create_action(TTR("Painting Terrain"));

					for (const RBSet<LayeredTileMapCell>::Element *E = edited.front(); E; E = E->next()) {
						Vector2i coords = E->get().get_atlas_coords();
						LayeredTileData *tile_data = p_tile_set_atlas_source->get_tile_data(coords, 0);

						Rect2i texture_region = p_tile_set_atlas_source->get_tile_texture_region(coords);
						Vector2i position = texture_region.get_center() + tile_data->get_texture_origin();

						Vector<Vector2> polygon = tile_set->get_terrain_polygon(terrain_set);
						for (int j = 0; j < polygon.size(); j++) {
							polygon.write[j] += position;
						}
						if (!Geometry::intersect_polygons_2d(polygon, mouse_pos_rect_polygon).empty()) {
							// Draw terrain.
							undo_redo->add_do_property(p_tile_set_atlas_source, vformat("%d:%d/%d/terrain", coords.x, coords.y, E->get().alternative_tile), terrain);
							undo_redo->add_undo_property(p_tile_set_atlas_source, vformat("%d:%d/%d/terrain", coords.x, coords.y, E->get().alternative_tile), tile_data->get_terrain());
						}

						for (int i = 0; i < LayeredTileSet::CELL_NEIGHBOR_MAX; i++) {
							LayeredTileSet::CellNeighbor bit = LayeredTileSet::CellNeighbor(i);
							if (tile_set->is_valid_terrain_peering_bit(terrain_set, bit)) {
								polygon = tile_set->get_terrain_peering_bit_polygon(terrain_set, bit);
								for (int j = 0; j < polygon.size(); j++) {
									polygon.write[j] += position;
								}
								if (!Geometry::intersect_polygons_2d(polygon, mouse_pos_rect_polygon).empty()) {
									// Draw bit.
									undo_redo->add_do_property(p_tile_set_atlas_source, vformat("%d:%d/%d/terrains_peering_bit/" + String(LayeredTileSet::CELL_NEIGHBOR_ENUM_TO_TEXT[i]), coords.x, coords.y, E->get().alternative_tile), terrain);
									undo_redo->add_undo_property(p_tile_set_atlas_source, vformat("%d:%d/%d/terrains_peering_bit/" + String(LayeredTileSet::CELL_NEIGHBOR_ENUM_TO_TEXT[i]), coords.x, coords.y, E->get().alternative_tile), tile_data->get_terrain_peering_bit(bit));
								}
							}
						}
					}
					undo_redo->commit_action();
					drag_type = DRAG_TYPE_NONE;
					accept_event();
				}
			}
		}
	}
}

void TileDataTerrainsEditor::forward_painting_alternatives_gui_input(LayeredTileAtlasView *p_tile_atlas_view, LayeredTileSetAtlasSource *p_tile_set_atlas_source, const Ref<InputEvent> &p_event) {
	Ref<InputEventMouseMotion> mm = p_event;
	if (mm.is_valid()) {
		if (drag_type == DRAG_TYPE_PAINT_TERRAIN_SET) {
			Vector3i tile = p_tile_atlas_view->get_alternative_tile_at_pos(mm->get_position());
			Vector2i coords = Vector2i(tile.x, tile.y);
			int alternative_tile = tile.z;

			if (coords != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
				LayeredTileMapCell cell;
				cell.source_id = 0;
				cell.set_atlas_coords(coords);
				cell.alternative_tile = alternative_tile;
				LayeredTileData *tile_data = p_tile_set_atlas_source->get_tile_data(coords, alternative_tile);
				if (!drag_modified.has(cell)) {
					Dictionary dict;
					dict["terrain_set"] = tile_data->get_terrain_set();
					dict["terrain"] = tile_data->get_terrain();
					Array array;
					for (int j = 0; j < LayeredTileSet::CELL_NEIGHBOR_MAX; j++) {
						LayeredTileSet::CellNeighbor bit = LayeredTileSet::CellNeighbor(j);
						array.push_back(tile_data->is_valid_terrain_peering_bit(bit) ? tile_data->get_terrain_peering_bit(bit) : -1);
					}
					dict["terrain_peering_bits"] = array;
					drag_modified[cell] = dict;
				}
				tile_data->set_terrain_set(drag_painted_value);
			}

			drag_last_pos = mm->get_position();
			accept_event();
		} else if (drag_type == DRAG_TYPE_PAINT_TERRAIN_BITS) {
			Dictionary painted = Dictionary(drag_painted_value);
			int terrain_set = int(painted["terrain_set"]);
			int terrain = int(painted["terrain"]);

			Vector3i tile = p_tile_atlas_view->get_alternative_tile_at_pos(mm->get_position());
			Vector2i coords = Vector2i(tile.x, tile.y);
			int alternative_tile = tile.z;

			if (coords != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
				LayeredTileMapCell cell;
				cell.source_id = 0;
				cell.set_atlas_coords(coords);
				cell.alternative_tile = alternative_tile;

				// Save the old terrain_set and terrains bits.
				LayeredTileData *tile_data = p_tile_set_atlas_source->get_tile_data(coords, alternative_tile);
				if (tile_data->get_terrain_set() == terrain_set) {
					if (!drag_modified.has(cell)) {
						Dictionary dict;
						dict["terrain_set"] = tile_data->get_terrain_set();
						dict["terrain"] = tile_data->get_terrain();
						Array array;
						for (int j = 0; j < LayeredTileSet::CELL_NEIGHBOR_MAX; j++) {
							LayeredTileSet::CellNeighbor bit = LayeredTileSet::CellNeighbor(j);
							array.push_back(tile_data->is_valid_terrain_peering_bit(bit) ? tile_data->get_terrain_peering_bit(bit) : -1);
						}
						dict["terrain_peering_bits"] = array;
						drag_modified[cell] = dict;
					}

					// Set the terrains bits.
					Rect2i texture_region = p_tile_atlas_view->get_alternative_tile_rect(coords, alternative_tile);
					Vector2i position = texture_region.get_center() + tile_data->get_texture_origin();

					Vector<Vector2> polygon = tile_set->get_terrain_polygon(tile_data->get_terrain_set());
					if (Geometry::is_segment_intersecting_polygon(mm->get_position() - position, drag_last_pos - position, polygon)) {
						tile_data->set_terrain(terrain);
					}

					for (int j = 0; j < LayeredTileSet::CELL_NEIGHBOR_MAX; j++) {
						LayeredTileSet::CellNeighbor bit = LayeredTileSet::CellNeighbor(j);
						if (tile_data->is_valid_terrain_peering_bit(bit)) {
							polygon = tile_set->get_terrain_peering_bit_polygon(tile_data->get_terrain_set(), bit);
							if (Geometry::is_segment_intersecting_polygon(mm->get_position() - position, drag_last_pos - position, polygon)) {
								tile_data->set_terrain_peering_bit(bit, terrain);
							}
						}
					}
				}
			}
			drag_last_pos = mm->get_position();
			accept_event();
		}
	}

	Ref<InputEventMouseButton> mb = p_event;
	if (mb.is_valid()) {
		if (mb->get_button_index() == BUTTON_LEFT || mb->get_button_index() == BUTTON_RIGHT) {
			if (mb->is_pressed()) {
				if (mb->get_button_index() == BUTTON_LEFT && picker_button->is_pressed()) {
					Vector3i tile = p_tile_atlas_view->get_alternative_tile_at_pos(mb->get_position());
					Vector2i coords = Vector2i(tile.x, tile.y);
					int alternative_tile = tile.z;

					if (coords != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
						LayeredTileData *tile_data = p_tile_set_atlas_source->get_tile_data(coords, alternative_tile);
						int terrain_set = tile_data->get_terrain_set();
						Rect2i texture_region = p_tile_atlas_view->get_alternative_tile_rect(coords, alternative_tile);
						Vector2i position = texture_region.get_center() + tile_data->get_texture_origin();
						dummy_object->set("terrain_set", terrain_set);
						dummy_object->set("terrain", -1);

						Vector<Vector2> polygon = tile_set->get_terrain_polygon(terrain_set);
						if (Geometry::is_point_in_polygon(mb->get_position() - position, polygon)) {
							dummy_object->set("terrain", tile_data->get_terrain());
						}

						for (int i = 0; i < LayeredTileSet::CELL_NEIGHBOR_MAX; i++) {
							LayeredTileSet::CellNeighbor bit = LayeredTileSet::CellNeighbor(i);
							if (tile_set->is_valid_terrain_peering_bit(terrain_set, bit)) {
								polygon = tile_set->get_terrain_peering_bit_polygon(terrain_set, bit);
								if (Geometry::is_point_in_polygon(mb->get_position() - position, polygon)) {
									dummy_object->set("terrain", tile_data->get_terrain_peering_bit(bit));
								}
							}
						}
						terrain_set_property_editor->update_property();
						_update_terrain_selector();
						picker_button->set_pressed(false);
						accept_event();
					}
				} else {
					int terrain_set = int(dummy_object->get("terrain_set"));
					int terrain = int(dummy_object->get("terrain"));

					Vector3i tile = p_tile_atlas_view->get_alternative_tile_at_pos(mb->get_position());
					Vector2i coords = Vector2i(tile.x, tile.y);
					int alternative_tile = tile.z;

					if (coords != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
						LayeredTileData *tile_data = p_tile_set_atlas_source->get_tile_data(coords, alternative_tile);

						if (terrain_set == -1 || !tile_data || tile_data->get_terrain_set() != terrain_set) {
							// Paint terrain sets.
							drag_type = DRAG_TYPE_PAINT_TERRAIN_SET;
							drag_modified.clear();
							drag_painted_value = int(dummy_object->get("terrain_set"));
							if (coords != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
								LayeredTileMapCell cell;
								cell.source_id = 0;
								cell.set_atlas_coords(coords);
								cell.alternative_tile = alternative_tile;
								Dictionary dict;
								dict["terrain_set"] = tile_data->get_terrain_set();
								Array array;
								for (int i = 0; i < LayeredTileSet::CELL_NEIGHBOR_MAX; i++) {
									LayeredTileSet::CellNeighbor bit = LayeredTileSet::CellNeighbor(i);
									array.push_back(tile_data->is_valid_terrain_peering_bit(bit) ? tile_data->get_terrain_peering_bit(bit) : -1);
								}
								dict["terrain_peering_bits"] = array;
								drag_modified[cell] = dict;
								tile_data->set_terrain_set(drag_painted_value);
							}
							drag_last_pos = mb->get_position();
							accept_event();
						} else if (tile_data->get_terrain_set() == terrain_set) {
							// Paint terrain bits.
							if (mb->get_button_index() == BUTTON_RIGHT) {
								terrain = -1;
							}
							// Paint terrain bits.
							drag_type = DRAG_TYPE_PAINT_TERRAIN_BITS;
							drag_modified.clear();
							Dictionary painted_dict;
							painted_dict["terrain_set"] = terrain_set;
							painted_dict["terrain"] = terrain;
							drag_painted_value = painted_dict;

							if (coords != LayeredTileSetSource::INVALID_ATLAS_COORDS) {
								LayeredTileMapCell cell;
								cell.source_id = 0;
								cell.set_atlas_coords(coords);
								cell.alternative_tile = alternative_tile;

								// Save the old terrain_set and terrains bits.
								Dictionary dict;
								dict["terrain_set"] = tile_data->get_terrain_set();
								dict["terrain"] = tile_data->get_terrain();
								Array array;
								for (int i = 0; i < LayeredTileSet::CELL_NEIGHBOR_MAX; i++) {
									LayeredTileSet::CellNeighbor bit = LayeredTileSet::CellNeighbor(i);
									array.push_back(tile_data->is_valid_terrain_peering_bit(bit) ? tile_data->get_terrain_peering_bit(bit) : -1);
								}
								dict["terrain_peering_bits"] = array;
								drag_modified[cell] = dict;

								// Set the terrain bit.
								Rect2i texture_region = p_tile_atlas_view->get_alternative_tile_rect(coords, alternative_tile);
								Vector2i position = texture_region.get_center() + tile_data->get_texture_origin();

								Vector<Vector2> polygon = tile_set->get_terrain_polygon(terrain_set);
								if (Geometry::is_point_in_polygon(mb->get_position() - position, polygon)) {
									tile_data->set_terrain(terrain);
								}
								for (int i = 0; i < LayeredTileSet::CELL_NEIGHBOR_MAX; i++) {
									LayeredTileSet::CellNeighbor bit = LayeredTileSet::CellNeighbor(i);
									if (tile_set->is_valid_terrain_peering_bit(terrain_set, bit)) {
										polygon = tile_set->get_terrain_peering_bit_polygon(terrain_set, bit);
										if (Geometry::is_point_in_polygon(mb->get_position() - position, polygon)) {
											tile_data->set_terrain_peering_bit(bit, terrain);
										}
									}
								}
							}
							drag_last_pos = mb->get_position();
							accept_event();
						}
					}
				}
			} else {
				UndoRedo *undo_redo = EditorNode::get_singleton()->get_undo_redo();
				if (drag_type == DRAG_TYPE_PAINT_TERRAIN_SET) {
					undo_redo->create_action(TTR("Painting Tiles Property"));
					for (HashMap<LayeredTileMapCell, Variant, LayeredTileMapCell>::Element *E = drag_modified.front(); E; E = E->next) {
						Dictionary dict = E->value();
						Vector2i coords = E->key().get_atlas_coords();
						undo_redo->add_do_property(p_tile_set_atlas_source, vformat("%d:%d/%d/terrain_set", coords.x, coords.y, E->key().alternative_tile), drag_painted_value);
						undo_redo->add_undo_property(p_tile_set_atlas_source, vformat("%d:%d/%d/terrain_set", coords.x, coords.y, E->key().alternative_tile), dict["terrain_set"]);
						if (int(dict["terrain_set"]) >= 0) {
							undo_redo->add_undo_property(p_tile_set_atlas_source, vformat("%d:%d/%d/terrain", coords.x, coords.y, E->key().alternative_tile), dict["terrain"]);
							Array array = dict["terrain_peering_bits"];
							for (int i = 0; i < array.size(); i++) {
								undo_redo->add_undo_property(p_tile_set_atlas_source, vformat("%d:%d/%d/terrains_peering_bit/" + String(LayeredTileSet::CELL_NEIGHBOR_ENUM_TO_TEXT[i]), coords.x, coords.y, E->key().alternative_tile), array[i]);
							}
						}
					}

					undo_redo->commit_action();

					drag_type = DRAG_TYPE_NONE;
					accept_event();
				} else if (drag_type == DRAG_TYPE_PAINT_TERRAIN_BITS) {
					Dictionary painted = Dictionary(drag_painted_value);
					int terrain_set = int(painted["terrain_set"]);
					int terrain = int(painted["terrain"]);
					undo_redo->create_action(TTR("Painting Terrain"));
					for (HashMap<LayeredTileMapCell, Variant, LayeredTileMapCell>::Element *E = drag_modified.front(); E; E = E->next) {
						Dictionary dict = E->value();
						Vector2i coords = E->key().get_atlas_coords();

						LayeredTileData *tile_data = p_tile_set_atlas_source->get_tile_data(coords, E->key().alternative_tile);

						undo_redo->add_do_property(p_tile_set_atlas_source, vformat("%d:%d/%d/terrain", coords.x, coords.y, E->key().alternative_tile), terrain);
						undo_redo->add_undo_property(p_tile_set_atlas_source, vformat("%d:%d/%d/terrain", coords.x, coords.y, E->key().alternative_tile), dict["terrain"]);
						Array array = dict["terrain_peering_bits"];
						for (int i = 0; i < array.size(); i++) {
							LayeredTileSet::CellNeighbor bit = LayeredTileSet::CellNeighbor(i);
							if (tile_set->is_valid_terrain_peering_bit(terrain_set, bit)) {
								undo_redo->add_do_property(p_tile_set_atlas_source, vformat("%d:%d/%d/terrains_peering_bit/" + String(LayeredTileSet::CELL_NEIGHBOR_ENUM_TO_TEXT[i]), coords.x, coords.y, E->key().alternative_tile), tile_data->get_terrain_peering_bit(bit));
							}
							if (tile_set->is_valid_terrain_peering_bit(dict["terrain_set"], bit)) {
								undo_redo->add_undo_property(p_tile_set_atlas_source, vformat("%d:%d/%d/terrains_peering_bit/" + String(LayeredTileSet::CELL_NEIGHBOR_ENUM_TO_TEXT[i]), coords.x, coords.y, E->key().alternative_tile), array[i]);
							}
						}
					}
					undo_redo->commit_action();
					drag_type = DRAG_TYPE_NONE;
					accept_event();
				}
			}
		}
	}
}

void TileDataTerrainsEditor::draw_over_tile(CanvasItem *p_canvas_item, Transform2D p_transform, LayeredTileMapCell p_cell, bool p_selected) {
	LayeredTileData *tile_data = _get_tile_data(p_cell);
	ERR_FAIL_NULL(tile_data);

	tile_set->draw_terrains(p_canvas_item, p_transform, tile_data);
}

void TileDataTerrainsEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE:
		case NOTIFICATION_THEME_CHANGED: {
			picker_button->set_icon(get_theme_icon("ColorPick", "EditorIcons"));
		} break;
	}
}

void TileDataTerrainsEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_property_value_changed"), &TileDataTerrainsEditor::_property_value_changed);
}

TileDataTerrainsEditor::TileDataTerrainsEditor() {
	label = memnew(Label);
	label->set_text(TTR("Painting:"));
	label->set_theme_type_variation("HeaderSmall");
	add_child(label);

	// Toolbar
	picker_button = memnew(Button);
	picker_button->set_theme_type_variation("FlatButton");
	picker_button->set_toggle_mode(true);
	picker_button->set_shortcut(ED_SHORTCUT("layered_tiles_editor/picker", TTR("Picker"), KEY_P));
	toolbar->add_child(picker_button);

	// Setup
	dummy_object->add_dummy_property("terrain_set");
	dummy_object->set("terrain_set", -1);
	dummy_object->add_dummy_property("terrain");
	dummy_object->set("terrain", -1);

	// Get the default value for the type.
	terrain_set_property_editor = memnew(EditorPropertyEnum);
	terrain_set_property_editor->set_object_and_property(dummy_object, "terrain_set");
	terrain_set_property_editor->set_label("Terrain Set");
	terrain_set_property_editor->connect("property_changed", this, "_property_value_changed");
	terrain_set_property_editor->set_tooltip(terrain_set_property_editor->get_edited_property());
	add_child(terrain_set_property_editor);

	terrain_property_editor = memnew(EditorPropertyEnum);
	terrain_property_editor->set_object_and_property(dummy_object, "terrain");
	terrain_property_editor->set_label("Terrain");
	terrain_property_editor->connect("property_changed", this, "_property_value_changed");
	add_child(terrain_property_editor);
}

TileDataTerrainsEditor::~TileDataTerrainsEditor() {
	toolbar->queue_delete();
	memdelete(dummy_object);
}

Variant TileDataNavigationEditor::_get_painted_value() {
	Ref<NavigationPolygon> nav_polygon;
	nav_polygon.instance();

	if (polygon_editor->get_polygon_count() > 0) {
		Ref<NavigationMeshSourceGeometryData2D> source_geometry_data;
		source_geometry_data.instance();
		for (int i = 0; i < polygon_editor->get_polygon_count(); i++) {
			Vector<Vector2> polygon = polygon_editor->get_polygon(i);
			nav_polygon->add_outline(Variant(polygon));
			source_geometry_data->add_traversable_outline(Variant(polygon));
		}
		nav_polygon->set_agent_radius(0.0);
		NavigationMeshGenerator::get_singleton()->bake_2d_from_source_geometry_data(nav_polygon, source_geometry_data);
	} else {
		nav_polygon->clear();
	}

	return nav_polygon;
}

void TileDataNavigationEditor::_set_painted_value(LayeredTileSetAtlasSource *p_tile_set_atlas_source, Vector2 p_coords, int p_alternative_tile) {
	LayeredTileData *tile_data = p_tile_set_atlas_source->get_tile_data(p_coords, p_alternative_tile);
	ERR_FAIL_NULL(tile_data);

	Ref<NavigationPolygon> nav_polygon = tile_data->get_navigation_polygon(navigation_layer);
	polygon_editor->clear_polygons();
	if (nav_polygon.is_valid()) {
		for (int i = 0; i < nav_polygon->get_outline_count(); i++) {
			polygon_editor->add_polygon(Variant(nav_polygon->get_outline(i)));
		}
	}
	polygon_editor->set_background(p_tile_set_atlas_source->get_texture(), p_tile_set_atlas_source->get_tile_texture_region(p_coords), tile_data->get_texture_origin(), tile_data->get_flip_h(), tile_data->get_flip_v(), tile_data->get_transpose(), tile_data->get_modulate());
}

void TileDataNavigationEditor::_set_value(LayeredTileSetAtlasSource *p_tile_set_atlas_source, Vector2 p_coords, int p_alternative_tile, const Variant &p_value) {
	LayeredTileData *tile_data = p_tile_set_atlas_source->get_tile_data(p_coords, p_alternative_tile);
	ERR_FAIL_NULL(tile_data);
	Ref<NavigationPolygon> nav_polygon = p_value;
	tile_data->set_navigation_polygon(navigation_layer, nav_polygon);

	polygon_editor->set_background(p_tile_set_atlas_source->get_texture(), p_tile_set_atlas_source->get_tile_texture_region(p_coords), tile_data->get_texture_origin(), tile_data->get_flip_h(), tile_data->get_flip_v(), tile_data->get_transpose(), tile_data->get_modulate());
}

Variant TileDataNavigationEditor::_get_value(LayeredTileSetAtlasSource *p_tile_set_atlas_source, Vector2 p_coords, int p_alternative_tile) {
	LayeredTileData *tile_data = p_tile_set_atlas_source->get_tile_data(p_coords, p_alternative_tile);
	ERR_FAIL_NULL_V(tile_data, Variant());
	return tile_data->get_navigation_polygon(navigation_layer);
}

void TileDataNavigationEditor::_setup_undo_redo_action(LayeredTileSetAtlasSource *p_tile_set_atlas_source, const HashMap<LayeredTileMapCell, Variant, LayeredTileMapCell> &p_previous_values, const Variant &p_new_value) {
	UndoRedo *undo_redo = EditorNode::get_singleton()->get_undo_redo();
	for (const HashMap<LayeredTileMapCell, Variant, LayeredTileMapCell>::Element *E = p_previous_values.front(); E; E = E->next) {
		Vector2i coords = E->key().get_atlas_coords();
		undo_redo->add_undo_property(p_tile_set_atlas_source, vformat("%d:%d/%d/navigation_layer_%d/polygon", coords.x, coords.y, E->key().alternative_tile, navigation_layer), E->value());
		undo_redo->add_do_property(p_tile_set_atlas_source, vformat("%d:%d/%d/navigation_layer_%d/polygon", coords.x, coords.y, E->key().alternative_tile, navigation_layer), p_new_value);
	}
}

void TileDataNavigationEditor::_tile_set_changed() {
	polygon_editor->set_tile_set(tile_set);
}

void TileDataNavigationEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
#ifdef DEBUG_ENABLED
			polygon_editor->set_polygons_color(Navigation2DServer::get_singleton()->get_debug_navigation_geometry_face_color());
#endif // DEBUG_ENABLED
		} break;
	}
}

TileDataNavigationEditor::TileDataNavigationEditor() {
	polygon_editor = memnew(GenericTilePolygonEditor);
	polygon_editor->set_multiple_polygon_mode(true);
	add_child(polygon_editor);
}

void TileDataNavigationEditor::draw_over_tile(CanvasItem *p_canvas_item, Transform2D p_transform, LayeredTileMapCell p_cell, bool p_selected) {
	LayeredTileData *tile_data = _get_tile_data(p_cell);
	ERR_FAIL_NULL(tile_data);

	// Draw all shapes.
	RenderingServer::get_singleton()->canvas_item_add_set_transform(p_canvas_item->get_canvas_item(), p_transform);

	Ref<NavigationPolygon> nav_polygon = tile_data->get_navigation_polygon(navigation_layer);
	if (nav_polygon.is_valid()) {
		Vector<Vector2> verts = Variant(nav_polygon->get_vertices());
		if (verts.size() < 3) {
			return;
		}

		Color color = Color(0.5, 1.0, 1.0, 1.0);
#ifdef DEBUG_ENABLED
		color = Navigation2DServer::get_singleton()->get_debug_navigation_geometry_face_color();
#endif // DEBUG_ENABLED
		if (p_selected) {
			Color grid_color = EDITOR_GET("editors/layered_tiles_editor/grid_color");
			Color selection_color = Color().from_hsv(Math::fposmod(grid_color.get_h() + 0.5, 1.0), grid_color.get_s(), grid_color.get_v(), 1.0);
			selection_color.a = 0.7;
			color = selection_color;
		}

		RandomPCG rand;
		for (int i = 0; i < nav_polygon->get_polygon_count(); i++) {
			// An array of vertices for this polygon.
			Vector<int> polygon = nav_polygon->get_polygon(i);
			Vector<Vector2> vertices;
			vertices.resize(polygon.size());
			for (int j = 0; j < polygon.size(); j++) {
				ERR_FAIL_INDEX(polygon[j], verts.size());
				vertices.write[j] = verts[polygon[j]];
			}

			// Generate the polygon color, slightly randomly modified from the settings one.
			Color random_variation_color;
			random_variation_color.set_hsv(color.get_h() + rand.random(-1.0, 1.0) * 0.05, color.get_s(), color.get_v() + rand.random(-1.0, 1.0) * 0.1);
			random_variation_color.a = color.a;
			Vector<Color> colors;
			colors.push_back(random_variation_color);

			RenderingServer::get_singleton()->canvas_item_add_polygon(p_canvas_item->get_canvas_item(), vertices, colors);
		}
	}

	RenderingServer::get_singleton()->canvas_item_add_set_transform(p_canvas_item->get_canvas_item(), Transform2D());
}

////  ======   TileDataAvoidanceEditor   ======

void TileDataAvoidanceEditor::_property_value_changed(const StringName &p_property, const Variant &p_value, const StringName &p_field, bool p_changing) {
	dummy_object->set(p_property, p_value);
}

void TileDataAvoidanceEditor::_property_selected(const StringName &p_path, int p_focusable) {
	// Deselect all other properties

	for (HashMap<StringName, EditorProperty *>::Element *editor = property_editors.front(); editor; editor = editor->next) {
		if (editor->key() != p_path) {
			editor->value()->deselect();
		}
	}
}

void TileDataAvoidanceEditor::_polygon_changed() {
	// Update the dummy object properties and their editors.

	StringName radius_property = "radius";
	StringName position_property = "position";
	//StringName polygon_property = "polygon";

	if (!dummy_object->has_dummy_property(radius_property)) {
		dummy_object->add_dummy_property(radius_property);
		dummy_object->set(radius_property, 0.0);
	}

	if (!dummy_object->has_dummy_property(position_property)) {
		dummy_object->add_dummy_property(position_property);
		dummy_object->set(position_property, Vector2i());
	}

	if (!property_editors.has(radius_property)) {
		EditorProperty *radius_property_editor = EditorInspector::instantiate_property_editor(dummy_object, Variant::REAL, radius_property, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT);
		radius_property_editor->set_object_and_property(dummy_object, radius_property);
		radius_property_editor->set_label(radius_property);
		radius_property_editor->connect("property_changed", this, "_property_value_changed");
		radius_property_editor->connect("selected", this, "_property_selected");
		radius_property_editor->set_tooltip(radius_property_editor->get_edited_property());
		radius_property_editor->update_property();
		add_child(radius_property_editor);
		property_editors[radius_property] = radius_property_editor;
	}

	if (!property_editors.has(position_property)) {
		EditorProperty *position_property_editor = EditorInspector::instantiate_property_editor(dummy_object, Variant::VECTOR2I, position_property, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT);
		position_property_editor->set_object_and_property(dummy_object, position_property);
		position_property_editor->set_label(position_property);
		position_property_editor->connect("property_changed", this, "_property_value_changed");
		position_property_editor->connect("selected", this, "_property_selected");
		position_property_editor->set_tooltip(position_property_editor->get_edited_property());
		position_property_editor->update_property();
		add_child(position_property_editor);
		property_editors[position_property] = position_property_editor;
	}
}

Variant TileDataAvoidanceEditor::_get_painted_value() {
	Dictionary dict;
	dict["radius"] = dummy_object->get("radius");
	dict["position"] = dummy_object->get("position");

	if (polygon_editor->get_polygon_count() > 0) {
		ERR_FAIL_COND_V(polygon_editor->get_polygon(0).size() < 3, Variant());

		dict["polygon"] = polygon_editor->get_polygon(0);
	}

	return dict;
}

void TileDataAvoidanceEditor::_set_painted_value(LayeredTileSetAtlasSource *p_tile_set_atlas_source, Vector2 p_coords, int p_alternative_tile) {
	LayeredTileData *tile_data = p_tile_set_atlas_source->get_tile_data(p_coords, p_alternative_tile);
	ERR_FAIL_NULL(tile_data);

	Vector<Vector2> polygon = tile_data->get_avoidance_polygon_points(avoidance_layer);
	polygon_editor->clear_polygons();

	if (polygon.size() > 0) {
		polygon_editor->add_polygon(Variant(polygon));

		update_polygon_color(polygon);
	}

	_polygon_changed();
	dummy_object->set("radius", tile_data->get_avoidance_radius(avoidance_layer));
	dummy_object->set("position", tile_data->get_avoidance_position(avoidance_layer));

	for (const HashMap<StringName, EditorProperty *>::Element *E = property_editors.front(); E; E = E->next) {
		E->value()->update_property();
	}

	polygon_editor->set_background(p_tile_set_atlas_source->get_texture(), p_tile_set_atlas_source->get_tile_texture_region(p_coords), tile_data->get_texture_origin(), tile_data->get_flip_h(), tile_data->get_flip_v(), tile_data->get_transpose(), tile_data->get_modulate());
}

void TileDataAvoidanceEditor::_set_value(LayeredTileSetAtlasSource *p_tile_set_atlas_source, Vector2 p_coords, int p_alternative_tile, const Variant &p_value) {
	LayeredTileData *tile_data = p_tile_set_atlas_source->get_tile_data(p_coords, p_alternative_tile);
	ERR_FAIL_NULL(tile_data);

	Dictionary dict = p_value;
	tile_data->set_avoidance_radius(avoidance_layer, dict["radius"]);
	tile_data->set_avoidance_position(avoidance_layer, dict["position"]);

	Vector<Vector2> polygon = dict["polygon"];
	tile_data->set_avoidance_polygon_points(avoidance_layer, polygon);

	update_polygon_color(polygon);

	polygon_editor->set_background(p_tile_set_atlas_source->get_texture(), p_tile_set_atlas_source->get_tile_texture_region(p_coords), tile_data->get_texture_origin(), tile_data->get_flip_h(), tile_data->get_flip_v(), tile_data->get_transpose(), tile_data->get_modulate());
}

Variant TileDataAvoidanceEditor::_get_value(LayeredTileSetAtlasSource *p_tile_set_atlas_source, Vector2 p_coords, int p_alternative_tile) {
	LayeredTileData *tile_data = p_tile_set_atlas_source->get_tile_data(p_coords, p_alternative_tile);
	ERR_FAIL_NULL_V(tile_data, Variant());

	Dictionary dict;
	dict["radius"] = tile_data->get_avoidance_radius(avoidance_layer);
	dict["position"] = tile_data->get_avoidance_position(avoidance_layer);
	dict["polygon"] = tile_data->get_avoidance_polygon_points(avoidance_layer);

	return dict;
}

void TileDataAvoidanceEditor::_setup_undo_redo_action(LayeredTileSetAtlasSource *p_tile_set_atlas_source, const HashMap<LayeredTileMapCell, Variant, LayeredTileMapCell> &p_previous_values, const Variant &p_new_value) {
	Dictionary new_dict = p_new_value;

	UndoRedo *undo_redo = EditorNode::get_singleton()->get_undo_redo();

	for (const HashMap<LayeredTileMapCell, Variant, LayeredTileMapCell>::Element *E = p_previous_values.front(); E; E = E->next) {
		Vector2i coords = E->key().get_atlas_coords();
		Dictionary old_dict = E->value();

		undo_redo->add_undo_property(p_tile_set_atlas_source, vformat("%d:%d/%d/avoidance_layer_%d/radius", coords.x, coords.y, E->key().alternative_tile, avoidance_layer), old_dict["radius"]);
		undo_redo->add_do_property(p_tile_set_atlas_source, vformat("%d:%d/%d/avoidance_layer_%d/radius", coords.x, coords.y, E->key().alternative_tile, avoidance_layer), new_dict["radius"]);

		undo_redo->add_undo_property(p_tile_set_atlas_source, vformat("%d:%d/%d/avoidance_layer_%d/position", coords.x, coords.y, E->key().alternative_tile, avoidance_layer), old_dict["position"]);
		undo_redo->add_do_property(p_tile_set_atlas_source, vformat("%d:%d/%d/avoidance_layer_%d/position", coords.x, coords.y, E->key().alternative_tile, avoidance_layer), new_dict["position"]);

		undo_redo->add_undo_property(p_tile_set_atlas_source, vformat("%d:%d/%d/avoidance_layer_%d/polygon", coords.x, coords.y, E->key().alternative_tile, avoidance_layer), old_dict["polygon"]);
		undo_redo->add_do_property(p_tile_set_atlas_source, vformat("%d:%d/%d/avoidance_layer_%d/polygon", coords.x, coords.y, E->key().alternative_tile, avoidance_layer), new_dict["polygon"]);
	}
}

void TileDataAvoidanceEditor::update_polygon_color(const Vector<Vector2> &polygon) {
	if (polygon.size() < 3) {
		return;
	}

	bool obstacle_pushes_inward = Geometry::is_polygon_clockwise(polygon);

	Color color;

#ifdef DEBUG_ENABLED
	if (obstacle_pushes_inward) {
		color = Navigation2DServer::get_singleton()->get_debug_navigation_avoidance_static_obstacle_pushin_face_color();
	} else {
		color = Navigation2DServer::get_singleton()->get_debug_navigation_avoidance_static_obstacle_pushout_face_color();
	}
#else
	if (obstacle_pushes_inward) {
		color = Color(1.0, 0.0, 0.0, 0.5);
	} else {
		color = Color(1.0, 1.0, 0.0, 0.5);
	}
#endif // DEBUG_ENABLED

	polygon_editor->set_polygons_color(color);
}

void TileDataAvoidanceEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_property_selected"), &TileDataAvoidanceEditor::_property_selected);
	ClassDB::bind_method(D_METHOD("_polygon_changed"), &TileDataAvoidanceEditor::_polygon_changed);
}

void TileDataAvoidanceEditor::_tile_set_changed() {
	polygon_editor->set_tile_set(tile_set);
	_polygon_changed();
}

void TileDataAvoidanceEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
#ifdef DEBUG_ENABLED
			polygon_editor->set_polygons_color(Navigation2DServer::get_singleton()->get_debug_navigation_avoidance_static_obstacle_pushout_face_color());
#endif // DEBUG_ENABLED
		} break;
	}
}

TileDataAvoidanceEditor::TileDataAvoidanceEditor() {
	polygon_editor = memnew(GenericTilePolygonEditor);
	polygon_editor->set_multiple_polygon_mode(false);
	polygon_editor->connect("polygons_changed", this, "_polygon_changed");
	add_child(polygon_editor);

	dummy_object->add_dummy_property("radius");
	dummy_object->set("radius", 0.0);
	dummy_object->add_dummy_property("position");
	dummy_object->set("position", Vector2i());

	EditorProperty *radius_editor = EditorInspector::instantiate_property_editor(dummy_object, Variant::REAL, "radius", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT);
	radius_editor->set_object_and_property(dummy_object, "radius");
	radius_editor->set_label("radius");
	radius_editor->connect("property_changed", this, "_property_value_changed");
	radius_editor->connect("selected", this, "_property_selected");
	radius_editor->set_tooltip(radius_editor->get_edited_property());
	radius_editor->update_property();
	add_child(radius_editor);
	property_editors["radius"] = radius_editor;

	EditorProperty *position_editor = EditorInspector::instantiate_property_editor(dummy_object, Variant::VECTOR2I, "position", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT);
	position_editor->set_object_and_property(dummy_object, "position");
	position_editor->set_label("position");
	position_editor->connect("property_changed", this, "_property_value_changed");
	position_editor->connect("selected", this, "_property_selected");
	position_editor->set_tooltip(position_editor->get_edited_property());
	position_editor->update_property();
	add_child(position_editor);
	property_editors["position"] = position_editor;

	_polygon_changed();
}

TileDataAvoidanceEditor::~TileDataAvoidanceEditor() {
	memdelete(dummy_object);
}

void TileDataAvoidanceEditor::draw_over_tile(CanvasItem *p_canvas_item, Transform2D p_transform, LayeredTileMapCell p_cell, bool p_selected) {
	LayeredTileData *tile_data = _get_tile_data(p_cell);
	ERR_FAIL_NULL(tile_data);

	// Draw all shapes.
	RenderingServer::get_singleton()->canvas_item_add_set_transform(p_canvas_item->get_canvas_item(), p_transform);

	Vector<Vector2> verts = tile_data->get_avoidance_polygon_points(avoidance_layer);

	if (verts.size() < 3) {
		return;
	}

	bool obstacle_pushes_inward = Geometry::is_polygon_clockwise(verts);

	Color color;

#ifdef DEBUG_ENABLED
	if (obstacle_pushes_inward) {
		color = Navigation2DServer::get_singleton()->get_debug_navigation_avoidance_static_obstacle_pushin_face_color();
	} else {
		color = Navigation2DServer::get_singleton()->get_debug_navigation_avoidance_static_obstacle_pushout_face_color();
	}
#else
	if (obstacle_pushes_inward) {
		color = Color(1.0, 0.0, 0.0, 0.5);
	} else {
		color = Color(1.0, 1.0, 0.0, 0.5);
	}
#endif // DEBUG_ENABLED

	if (p_selected) {
		color = color.lightened(0.5);
		//color.a += 0.2;
	}

	Vector<Color> colors;
	colors.push_back(color);

	RenderingServer::get_singleton()->canvas_item_add_polygon(p_canvas_item->get_canvas_item(), verts, colors);
	RenderingServer::get_singleton()->canvas_item_add_set_transform(p_canvas_item->get_canvas_item(), Transform2D());

	real_t radius = tile_data->get_avoidance_radius(avoidance_layer);
	Vector2i position = tile_data->get_avoidance_position(avoidance_layer);

	color = Color(1.0, 1.0, 1.0);
	if (p_selected) {
		Color grid_color = EDITOR_GET("editors/layered_tiles_editor/grid_color");
		Color selection_color = Color().from_hsv(Math::fposmod(grid_color.get_h() + 0.5, 1.0), grid_color.get_s(), grid_color.get_v(), 1.0);
		color = selection_color;
	}
	Ref<Texture> position_icon = LayeredTileSetEditor::get_singleton()->get_theme_icon("EditorPosition", "EditorIcons");
	p_canvas_item->draw_texture(position_icon, p_transform.xform(Vector2(position)) - position_icon->get_size() / 2, color);

#ifdef DEBUG_ENABLED
	color = Navigation2DServer::get_singleton()->get_debug_navigation_avoidance_obstacles_radius_color();
#else
	color = Color(1.0, 0.5, 0.0, 0.25);
#endif // DEBUG_ENABLED

	if (radius > 0.0) {
		p_canvas_item->draw_circle(p_transform.xform(Vector2(position)), radius * p_transform.get_scale().x, color);
	}
}
