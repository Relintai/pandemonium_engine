#ifndef RENDERINGSERVERCANVAS_H
#define RENDERINGSERVERCANVAS_H

/*************************************************************************/
/*  rendering_server_canvas.h                                            */
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

#include "rasterizer.h"
#include "rendering_server_constants.h"
#include "rendering_server_viewport.h"

class RenderingServerCanvas {
public:
	struct Item : public RasterizerCanvas::Item {
		RID parent; // canvas it belongs to
		List<Item *>::Element *E;
		int z_index;
		bool z_relative;
		bool sort_y;
		Color modulate;
		Color self_modulate;
		bool use_parent_material;
		int index;
		bool children_order_dirty;
		int ysort_children_count;
		Color ysort_modulate;
		Transform2D ysort_xform;
		Vector2 ysort_pos;
		int ysort_index;
#ifdef RENDERING_SERVER_CANVAS_DEBUG_ITEM_NAMES
		String name;
#endif

		Vector<Item *> child_items;

		Item() {
			children_order_dirty = true;
			E = nullptr;
			z_index = 0;
			modulate = Color(1, 1, 1, 1);
			self_modulate = Color(1, 1, 1, 1);
			sort_y = false;
			use_parent_material = false;
			z_relative = true;
			index = 0;
			ysort_children_count = -1;
			ysort_xform = Transform2D();
			ysort_pos = Vector2();
			ysort_index = 0;
		}
	};

	struct ItemIndexSort {
		_FORCE_INLINE_ bool operator()(const Item *p_left, const Item *p_right) const {
			return p_left->index < p_right->index;
		}
	};

	struct ItemPtrSort {
		_FORCE_INLINE_ bool operator()(const Item *p_left, const Item *p_right) const {
			if (Math::is_equal_approx(p_left->ysort_pos.y, p_right->ysort_pos.y)) {
				return p_left->ysort_index < p_right->ysort_index;
			}

			return p_left->ysort_pos.y < p_right->ysort_pos.y;
		}
	};

	struct LightOccluderPolygon : RID_Data {
		bool active;
		Rect2 aabb;
		RS::CanvasOccluderPolygonCullMode cull_mode;
		RID occluder;
		RBSet<RasterizerCanvas::LightOccluderInstance *> owners;

		LightOccluderPolygon() {
			active = false;
			cull_mode = RS::CANVAS_OCCLUDER_POLYGON_CULL_DISABLED;
		}
	};

	RID_Owner<LightOccluderPolygon> canvas_light_occluder_polygon_owner;

	RID_Owner<RasterizerCanvas::LightOccluderInstance> canvas_light_occluder_owner;

	struct Canvas : public RenderingServerViewport::CanvasBase {
		RBSet<RID> viewports;
		struct ChildItem {
			Point2 mirror;
			Item *item;
			bool operator<(const ChildItem &p_item) const {
				return item->index < p_item.item->index;
			}
		};

		RBSet<RasterizerCanvas::Light *> lights;

		RBSet<RasterizerCanvas::LightOccluderInstance *> occluders;

		bool children_order_dirty;
		Vector<ChildItem> child_items;
		Color modulate;
		RID parent;
		float parent_scale;

		int find_item(Item *p_item) {
			for (int i = 0; i < child_items.size(); i++) {
				if (child_items[i].item == p_item) {
					return i;
				}
			}
			return -1;
		}
		void erase_item(Item *p_item) {
			int idx = find_item(p_item);
			if (idx >= 0) {
				child_items.remove(idx);
			}
		}

		Canvas() {
			modulate = Color(1, 1, 1, 1);
			children_order_dirty = true;
			parent_scale = 1.0;
		}
	};

	mutable RID_Owner<Canvas> canvas_owner;
	RID_Owner<Item> canvas_item_owner;
	RID_Owner<RasterizerCanvas::Light> canvas_light_owner;

	bool disable_scale;

private:
	enum CanvasCullMode {
		CANVAS_CULL_MODE_ITEM,
		CANVAS_CULL_MODE_NODE,
	};
	CanvasCullMode _canvas_cull_mode;

	void _render_canvas_item_tree(Item *p_canvas_item, const Transform2D &p_transform, const Rect2 &p_clip_rect, const Color &p_modulate, RasterizerCanvas::Light *p_lights);

	void _light_mask_canvas_items(int p_z, RasterizerCanvas::Item *p_canvas_item, RasterizerCanvas::Light *p_masked_lights, int p_canvas_layer_id);

	RasterizerCanvas::Item **z_list;
	RasterizerCanvas::Item **z_last_list;
	Transform2D _current_camera_transform;

	// 3.5 and earlier had no hierarchical culling.
	void _render_canvas_item_cull_by_item(Item *p_canvas_item, const Transform2D &p_transform, const Rect2 &p_clip_rect, const Color &p_modulate, int p_z, RasterizerCanvas::Item **z_list, RasterizerCanvas::Item **z_last_list, Item *p_canvas_clip, Item *p_material_owner);

	// Hierarchical culling by scene tree node ///////////////////////////////////
	void _render_canvas_item_cull_by_node(Item *p_canvas_item, const Transform2D &p_transform, const Rect2 &p_clip_rect, const Color &p_modulate, int p_z, RasterizerCanvas::Item **z_list, RasterizerCanvas::Item **z_last_list, Item *p_canvas_clip, Item *p_material_owner, bool p_enclosed);

	void _prepare_tree_bounds(Item *p_root);
	void _calculate_canvas_item_bound(Item *p_canvas_item, Rect2 *r_branch_bound);

	Transform2D _calculate_item_global_xform(const Item *p_canvas_item);
	void _finalize_and_merge_local_bound_to_branch(Item *p_canvas_item, Rect2 *r_branch_bound);
	void _merge_local_bound_to_branch(Item *p_canvas_item, Rect2 *r_branch_bound);

	// If bounds flags are attempted to be modified multithreaded, the
	// tree could become corrupt. Multithread access may not be possible,
	// but just in case we use a mutex until proven otherwise.
	Mutex _bound_mutex;

	void _make_bound_dirty_reparent(Item *p_item);
	void _make_bound_dirty(Item *p_item, bool p_changing_visibility = false);
	void _make_bound_dirty_down(Item *p_item);

#ifdef RENDERING_SERVER_CANVAS_CHECK_BOUNDS
	bool _check_bound_integrity(const Item *p_item);
	bool _check_bound_integrity_down(const Item *p_item, bool p_bound_dirty);
	void _print_tree(const Item *p_item);
	void _print_tree_down(int p_child_id, int p_depth, const Item *p_item, const Item *p_highlight, bool p_hidden = false);
#else
	bool _check_bound_integrity(const Item *p_item) { return true; }
#endif
	//////////////////////////////////////////////////////////////////////////////

public:
	void render_canvas(Canvas *p_canvas, const Transform2D &p_transform, RasterizerCanvas::Light *p_lights, RasterizerCanvas::Light *p_masked_lights, const Rect2 &p_clip_rect, int p_canvas_layer_id);

	RID canvas_create();
	void canvas_set_item_mirroring(RID p_canvas, RID p_item, const Point2 &p_mirroring);
	void canvas_set_modulate(RID p_canvas, const Color &p_color);
	void canvas_set_parent(RID p_canvas, RID p_parent, float p_scale);
	void canvas_set_disable_scale(bool p_disable);

	RID canvas_item_create();
	void canvas_item_set_parent(RID p_item, RID p_parent);
	void canvas_item_set_name(RID p_item, String p_name);

	void canvas_item_set_visible(RID p_item, bool p_visible);
	void canvas_item_set_light_mask(RID p_item, int p_mask);

	void canvas_item_set_transform(RID p_item, const Transform2D &p_transform);
	void canvas_item_set_clip(RID p_item, bool p_clip);
	void canvas_item_set_distance_field_mode(RID p_item, bool p_enable);
	void canvas_item_set_custom_rect(RID p_item, bool p_custom_rect, const Rect2 &p_rect = Rect2());
	void canvas_item_set_modulate(RID p_item, const Color &p_color);
	void canvas_item_set_self_modulate(RID p_item, const Color &p_color);

	void canvas_item_set_draw_behind_parent(RID p_item, bool p_enable);
	void canvas_item_set_use_identity_transform(RID p_item, bool p_enable);

	void canvas_item_set_update_when_visible(RID p_item, bool p_update);

	void canvas_item_add_line(RID p_item, const Point2 &p_from, const Point2 &p_to, const Color &p_color, float p_width = 1.0, bool p_antialiased = false);
	void canvas_item_add_polyline(RID p_item, const Vector<Point2> &p_points, const Vector<Color> &p_colors, float p_width = 1.0, bool p_antialiased = false);
	void canvas_item_add_multiline(RID p_item, const Vector<Point2> &p_points, const Vector<Color> &p_colors, float p_width = 1.0, bool p_antialiased = false);
	void canvas_item_add_rect(RID p_item, const Rect2 &p_rect, const Color &p_color);
	void canvas_item_add_circle(RID p_item, const Point2 &p_pos, float p_radius, const Color &p_color);
	void canvas_item_add_texture_rect(RID p_item, const Rect2 &p_rect, RID p_texture, bool p_tile = false, const Color &p_modulate = Color(1, 1, 1), bool p_transpose = false, RID p_normal_map = RID());
	void canvas_item_add_texture_rect_region(RID p_item, const Rect2 &p_rect, RID p_texture, const Rect2 &p_src_rect, const Color &p_modulate = Color(1, 1, 1), bool p_transpose = false, RID p_normal_map = RID(), bool p_clip_uv = false);
	void canvas_item_add_texture_multirect_region(RID p_item, const Vector<Rect2> &p_rects, RID p_texture, const Vector<Rect2> &p_src_rects, const Color &p_modulate = Color(1, 1, 1), uint32_t p_canvas_rect_flags = 0, RID p_normal_map = RID());
	void canvas_item_add_nine_patch(RID p_item, const Rect2 &p_rect, const Rect2 &p_source, RID p_texture, const Vector2 &p_topleft, const Vector2 &p_bottomright, RS::NinePatchAxisMode p_x_axis_mode = RS::NINE_PATCH_STRETCH, RS::NinePatchAxisMode p_y_axis_mode = RS::NINE_PATCH_STRETCH, bool p_draw_center = true, const Color &p_modulate = Color(1, 1, 1), RID p_normal_map = RID());
	void canvas_item_add_primitive(RID p_item, const Vector<Point2> &p_points, const Vector<Color> &p_colors, const Vector<Point2> &p_uvs, RID p_texture, float p_width = 1.0, RID p_normal_map = RID());
	void canvas_item_add_polygon(RID p_item, const Vector<Point2> &p_points, const Vector<Color> &p_colors, const Vector<Point2> &p_uvs = Vector<Point2>(), RID p_texture = RID(), RID p_normal_map = RID(), bool p_antialiased = false);
	void canvas_item_add_triangle_array(RID p_item, const Vector<int> &p_indices, const Vector<Point2> &p_points, const Vector<Color> &p_colors, const Vector<Point2> &p_uvs = Vector<Point2>(), const Vector<int> &p_bones = Vector<int>(), const Vector<float> &p_weights = Vector<float>(), RID p_texture = RID(), int p_count = -1, RID p_normal_map = RID(), bool p_antialiased = false, bool p_antialiasing_use_indices = false);
	void canvas_item_add_mesh(RID p_item, const RID &p_mesh, const Transform2D &p_transform = Transform2D(), const Color &p_modulate = Color(1, 1, 1), RID p_texture = RID(), RID p_normal_map = RID());
	void canvas_item_add_multimesh(RID p_item, RID p_mesh, RID p_texture = RID(), RID p_normal_map = RID());
	void canvas_item_add_texture_rect_animation(RID p_item, const Array &p_animation_data, const real_t p_start_time = 0.0);
	
	void canvas_item_add_set_transform(RID p_item, const Transform2D &p_transform);
	void canvas_item_add_clip_ignore(RID p_item, bool p_ignore);
	void canvas_item_set_sort_children_by_y(RID p_item, bool p_enable);
	void canvas_item_set_z_index(RID p_item, int p_z);
	void canvas_item_set_z_as_relative_to_parent(RID p_item, bool p_enable);
	void canvas_item_set_copy_to_backbuffer(RID p_item, bool p_enable, const Rect2 &p_rect);

	void canvas_item_clear(RID p_item);
	void canvas_item_set_draw_index(RID p_item, int p_index);
	void canvas_item_set_material(RID p_item, RID p_material);
	void canvas_item_set_use_parent_material(RID p_item, bool p_enable);

	void canvas_item_attach_skeleton(RID p_item, RID p_skeleton);
	void canvas_item_set_skeleton_relative_xform(RID p_item, Transform2D p_relative_xform);
	Rect2 _debug_canvas_item_get_rect(RID p_item);
	Rect2 _debug_canvas_item_get_local_bound(RID p_item);

	void canvas_item_set_interpolated(RID p_item, bool p_interpolated);
	void canvas_item_reset_physics_interpolation(RID p_item);
	void canvas_item_transform_physics_interpolation(RID p_item, const Transform2D &p_transform);

	void _canvas_item_invalidate_local_bound(RID p_item);
	void _canvas_item_remove_references(RID p_item, RID p_rid);

	RID canvas_light_create();
	void canvas_light_attach_to_canvas(RID p_light, RID p_canvas);
	void canvas_light_set_enabled(RID p_light, bool p_enabled);
	void canvas_light_set_scale(RID p_light, float p_scale);
	void canvas_light_set_transform(RID p_light, const Transform2D &p_transform);
	void canvas_light_set_texture(RID p_light, RID p_texture);
	void canvas_light_set_texture_offset(RID p_light, const Vector2 &p_offset);
	void canvas_light_set_color(RID p_light, const Color &p_color);
	void canvas_light_set_height(RID p_light, float p_height);
	void canvas_light_set_energy(RID p_light, float p_energy);
	void canvas_light_set_z_range(RID p_light, int p_min_z, int p_max_z);
	void canvas_light_set_layer_range(RID p_light, int p_min_layer, int p_max_layer);
	void canvas_light_set_item_cull_mask(RID p_light, int p_mask);
	void canvas_light_set_item_shadow_cull_mask(RID p_light, int p_mask);
	void canvas_light_set_mode(RID p_light, RS::CanvasLightMode p_mode);
	void canvas_light_set_shadow_enabled(RID p_light, bool p_enabled);
	void canvas_light_set_shadow_buffer_size(RID p_light, int p_size);
	void canvas_light_set_shadow_gradient_length(RID p_light, float p_length);
	void canvas_light_set_shadow_filter(RID p_light, RS::CanvasLightShadowFilter p_filter);
	void canvas_light_set_shadow_color(RID p_light, const Color &p_color);
	void canvas_light_set_shadow_smooth(RID p_light, float p_smooth);

	void canvas_light_set_interpolated(RID p_light, bool p_interpolated);
	void canvas_light_reset_physics_interpolation(RID p_light);
	void canvas_light_transform_physics_interpolation(RID p_light, const Transform2D &p_transform);

	RID canvas_light_occluder_create();
	void canvas_light_occluder_attach_to_canvas(RID p_occluder, RID p_canvas);
	void canvas_light_occluder_set_enabled(RID p_occluder, bool p_enabled);
	void canvas_light_occluder_set_polygon(RID p_occluder, RID p_polygon);
	void canvas_light_occluder_set_transform(RID p_occluder, const Transform2D &p_xform);
	void canvas_light_occluder_set_light_mask(RID p_occluder, int p_mask);

	void canvas_light_occluder_set_interpolated(RID p_occluder, bool p_interpolated);
	void canvas_light_occluder_reset_physics_interpolation(RID p_occluder);
	void canvas_light_occluder_transform_physics_interpolation(RID p_occluder, const Transform2D &p_transform);

	RID canvas_occluder_polygon_create();
	void canvas_occluder_polygon_set_shape(RID p_occluder_polygon, const PoolVector<Vector2> &p_shape, bool p_closed);
	void canvas_occluder_polygon_set_shape_as_lines(RID p_occluder_polygon, const PoolVector<Vector2> &p_shape);

	void canvas_occluder_polygon_set_cull_mode(RID p_occluder_polygon, RS::CanvasOccluderPolygonCullMode p_mode);

	bool free(RID p_rid);

	// Interpolation
	void tick();
	void update_interpolation_tick(bool p_process = true);
	void set_physics_interpolation_enabled(bool p_enabled) { _interpolation_data.interpolation_enabled = p_enabled; }

	struct InterpolationData {
		void notify_free_canvas_item(RID p_rid, RenderingServerCanvas::Item &r_canvas_item);
		void notify_free_canvas_light(RID p_rid, RasterizerCanvas::Light &r_canvas_light);
		void notify_free_canvas_light_occluder(RID p_rid, RasterizerCanvas::LightOccluderInstance &r_canvas_light_occluder);

		LocalVector<RID> canvas_item_transform_update_lists[2];
		LocalVector<RID> *canvas_item_transform_update_list_curr = &canvas_item_transform_update_lists[0];
		LocalVector<RID> *canvas_item_transform_update_list_prev = &canvas_item_transform_update_lists[1];

		LocalVector<RID> canvas_light_transform_update_lists[2];
		LocalVector<RID> *canvas_light_transform_update_list_curr = &canvas_light_transform_update_lists[0];
		LocalVector<RID> *canvas_light_transform_update_list_prev = &canvas_light_transform_update_lists[1];

		LocalVector<RID> canvas_light_occluder_transform_update_lists[2];
		LocalVector<RID> *canvas_light_occluder_transform_update_list_curr = &canvas_light_occluder_transform_update_lists[0];
		LocalVector<RID> *canvas_light_occluder_transform_update_list_prev = &canvas_light_occluder_transform_update_lists[1];

		bool interpolation_enabled = false;
	} _interpolation_data;

	RenderingServerCanvas();
	~RenderingServerCanvas();
};

#endif // RENDERINGSERVERCANVAS_H
