#ifndef PAINT_CURVE_2D_H
#define PAINT_CURVE_2D_H

/*************************************************************************/
/*  path_2d.h                                                            */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
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

class Curve2D;

class PaintCurve2D : public PaintNode {
	GDCLASS(PaintCurve2D, PaintNode);

public:
#ifdef TOOLS_ENABLED
	virtual Rect2 _edit_get_rect() const;
	virtual bool _edit_use_rect() const;
	virtual bool _edit_is_selected_on_click(const Point2 &p_point, double p_tolerance) const;
#endif

	void set_curve(const Ref<Curve2D> &p_curve);
	Ref<Curve2D> get_curve() const;

	void set_offset(const Vector2 &p_offset);
	Vector2 get_offset() const;

	void fill_set_enabled(bool p_enabled);
	bool fill_get_enabled() const;

	void fill_set_color(const Color &p_color);
	Color fill_get_color() const;

	void fill_set_texture(const Ref<Texture> &p_texture);
	Ref<Texture> fill_get_texture() const;

	void fill_set_texture_offset(const Vector2 &p_offset);
	Vector2 fill_get_texture_offset() const;

	void fill_set_texture_rotation(float p_rot);
	float fill_get_texture_rotation() const;

	void fill_set_texture_rotation_degrees(float p_rot);
	float fill_get_texture_rotation_degrees() const;

	void fill_set_texture_scale(const Size2 &p_scale);
	Size2 fill_get_texture_scale() const;

	void fill_set_invert(bool p_invert);
	bool fill_get_invert() const;

	void fill_set_antialiased(bool p_antialiased);
	bool fill_get_antialiased() const;

	void fill_set_invert_border(float p_invert_border);
	float fill_get_invert_border() const;

	void outline_set_enabled(bool p_enabled);
	bool outline_get_enabled() const;

	void outline_set_width(int p_width);
	int outline_get_width() const;

	void outline_set_color(const Color &p_color);
	Color outline_get_color() const;

	void outline_set_texture(const Ref<Texture> &p_texture);
	Ref<Texture> outline_get_texture() const;

	void outline_set_texture_offset(const Vector2 &p_offset);
	Vector2 outline_get_texture_offset() const;

	void outline_set_texture_rotation(float p_rot);
	float outline_get_texture_rotation() const;

	void outline_set_texture_rotation_degrees(float p_rot);
	float outline_get_texture_rotation_degrees() const;

	void outline_set_texture_scale(const Size2 &p_scale);
	Size2 outline_get_texture_scale() const;

	void outline_set_antialiased(bool p_antialiased);
	bool outline_get_antialiased() const;

	PaintCurve2D();

protected:
	void _notification(int p_what);
	static void _bind_methods();

protected:
	bool _fill_enabled;
	Color _fill_color;
	Ref<Texture> _fill_texture;
	Size2 _fill_tex_scale;
	Vector2 _fill_tex_ofs;
	bool _fill_tex_tile;
	float _fill_tex_rot;
	bool _fill_invert;
	float _fill_invert_border;
	bool _fill_antialiased;

	bool _outline_enabled;
	Color _outline_color;
	int _outline_width;
	Ref<Texture> _outline_texture;
	Size2 _outline_tex_scale;
	Vector2 _outline_tex_ofs;
	bool _outline_tex_tile;
	float _outline_tex_rot;
	bool _outline_antialiased;

	Vector2 offset;
	mutable bool rect_cache_dirty;
	mutable Rect2 item_rect;
	Ref<Image> _rendered_image;

	Ref<Curve2D> curve;
	Vector<Vector2> _cached_draw_pts;

	void _curve_changed();

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

#endif // PATH_2D_H
