#ifndef PAINT_POLYGON_2D_H
#define PAINT_POLYGON_2D_H

/*************************************************************************/
/*  paint_polygon_2d.h                                                   */
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

#include "../paint_node.h"

class PaintPolygon2D : public PaintNode {
	GDCLASS(PaintPolygon2D, PaintNode);

	PoolVector<Vector2> polygon;
	PoolVector<Vector2> uv;
	PoolVector<Color> vertex_colors;
	Array polygons;
	int internal_vertices;

	Color color;
	Ref<Texture> texture;
	Size2 tex_scale;
	Vector2 tex_ofs;
	bool tex_tile;
	float tex_rot;
	bool invert;
	float invert_border;
	bool antialiased;

	Vector2 offset;
	mutable bool rect_cache_dirty;
	mutable Rect2 item_rect;
	Ref<Image> _rendered_image;

protected:
	void _prepare_render_data(Vector<Vector2> &r_points, Vector<Vector2> &r_uvs, Vector<Color> &r_colors, Vector<int> &r_indices);

	void _notification(int p_what);
	static void _bind_methods();

public:
#ifdef TOOLS_ENABLED
	virtual Dictionary _edit_get_state() const;
	virtual void _edit_set_state(const Dictionary &p_state);

	virtual void _edit_set_pivot(const Point2 &p_pivot);
	virtual Point2 _edit_get_pivot() const;
	virtual bool _edit_use_pivot() const;
	virtual Rect2 _edit_get_rect() const;
	virtual bool _edit_use_rect() const;

	virtual bool _edit_is_selected_on_click(const Point2 &p_point, double p_tolerance) const;
#endif

	void set_polygon(const PoolVector<Vector2> &p_polygon);
	PoolVector<Vector2> get_polygon() const;

	void set_internal_vertex_count(int p_count);
	int get_internal_vertex_count() const;

	void set_uv(const PoolVector<Vector2> &p_uv);
	PoolVector<Vector2> get_uv() const;

	void set_polygons(const Array &p_polygons);
	Array get_polygons() const;

	void set_color(const Color &p_color);
	Color get_color() const;

	void set_vertex_colors(const PoolVector<Color> &p_colors);
	PoolVector<Color> get_vertex_colors() const;

	void set_texture(const Ref<Texture> &p_texture);
	Ref<Texture> get_texture() const;

	void set_texture_offset(const Vector2 &p_offset);
	Vector2 get_texture_offset() const;

	void set_texture_rotation(float p_rot);
	float get_texture_rotation() const;

	void set_texture_rotation_degrees(float p_rot);
	float get_texture_rotation_degrees() const;

	void set_texture_scale(const Size2 &p_scale);
	Size2 get_texture_scale() const;

	void set_invert(bool p_invert);
	bool get_invert() const;

	void set_antialiased(bool p_antialiased);
	bool get_antialiased() const;

	void set_invert_border(float p_invert_border);
	float get_invert_border() const;

	void set_offset(const Vector2 &p_offset);
	Vector2 get_offset() const;

	virtual Ref<Image> _get_rendered_image();

	PaintPolygon2D();
	virtual ~PaintPolygon2D();

protected:
	struct Slope {
		Vector2 position_current;
		Vector2 position_step;

		Vector2 uv_current;
		Vector2 uv_step;

		Color color_current;
		Color color_step;

		_FORCE_INLINE_ void setup_position(Vector2 begin, Vector2 end, float num_steps) {
			float inv_step = 1.0 / num_steps;
			position_step = (end - begin) * Vector2(inv_step, inv_step);
			position_current = begin;
		}

		_FORCE_INLINE_ void setup_color(Color begin, Color end, float num_steps) {
			float inv_step = 1.0 / num_steps;
			color_step = (end - begin) * Color(inv_step, inv_step, inv_step);
			color_current = begin;
		}

		_FORCE_INLINE_ void setup_uv(Vector2 begin, Vector2 end, float num_steps) {
			float inv_step = 1.0 / num_steps;
			uv_step = (end - begin) * Vector2(inv_step, inv_step);
			uv_current = begin;
		}

		_FORCE_INLINE_ void advance() {
			position_current += position_step;
			color_current += color_step;
		}

		_FORCE_INLINE_ void advance_color() {
			color_current += color_step;
		}

		_FORCE_INLINE_ void advance_uv() {
			uv_current += uv_step;
		}
	};
};

#endif // POLYGON_2D_H
