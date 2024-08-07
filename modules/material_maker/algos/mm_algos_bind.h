#ifndef MM_ALGOS_BIND_H
#define MM_ALGOS_BIND_H

/*************************************************************************/
/*  mm_algos_bind.h                                                      */
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

#include "core/containers/pool_vector.h"
#include "core/io/image.h"
#include "core/math/color.h"
#include "core/math/vector2.h"
#include "core/math/vector3.h"
#include "core/variant/variant.h"

#include "core/object/object.h"

class _MMAlgos : public Object {
	GDCLASS(_MMAlgos, Object);

public:
	Vector3 clampv3(const Vector3 &v, const Vector3 &mi, const Vector3 &ma);
	Color floorc(const Color &a);
	Vector2 floorv2(const Vector2 &a);
	Vector3 floorv3(const Vector3 &a);
	Vector2 smoothstepv2(const float a, const float b, const Vector2 &c);
	Vector2 maxv2(const Vector2 &a, const Vector2 &b);
	Vector3 maxv3(const Vector3 &a, const Vector3 &b);
	Vector2 absv2(const Vector2 &v);
	Vector3 absv3(const Vector3 &v);
	Vector2 cosv2(const Vector2 &v);
	Vector3 cosv3(const Vector3 &v);
	Vector2 powv2(const Vector2 &x, const Vector2 &y);
	Vector3 modv3(const Vector3 &a, const Vector3 &b);
	Vector2 modv2(const Vector2 &a, const Vector2 &b);
	float modf(const float x, const float y);
	Vector2 fractv2(const Vector2 &v);
	Vector3 fractv3(const Vector3 &v);
	float fract(const float f);
	Vector2 clampv2(const Vector2 &v, const Vector2 &pmin, const Vector2 &pmax);
	Vector2 minv2(const Vector2 &v1, const Vector2 &v2);
	Vector3 minv3(const Vector3 &v1, const Vector3 &v2);
	float rand(const Vector2 &x);
	Vector2 rand2(const Vector2 &x);
	Vector3 rand3(const Vector2 &x);
	float step(const float edge, const float x);
	Vector2 stepv2(const Vector2 &edge, const Vector2 &x);
	Color stepc(const Color &edge, const Color &x);
	Color mixc(const Color &a, const Color &b, const Color &t);
	Vector2 signv2(const Vector2 &x);
	Vector2 transform(const Vector2 &uv, const Vector2 &translate, const float rotate, const Vector2 &scale, const bool repeat);
	float fractf(const float x);
	float mix_mul(const float x, const float y);
	float mix_add(const float x, const float y);
	float mix_max(const float x, const float y);
	float mix_min(const float x, const float y);
	float mix_xor(const float x, const float y);
	float mix_pow(const float x, const float y);
	float wave_constant(const float x);
	float wave_sine(const float x);
	float wave_triangle(const float x);
	float wave_sawtooth(const float x);
	float wave_square(const float x);
	float wave_bounce(const float x);
	Color sinewave(const Vector2 &uv, const float amplitude, const float frequency, const float phase);
	float ThickLine(const Vector2 &uv, const Vector2 &posA, const Vector2 &posB, const float radiusInv);
	float curve(const float x, const PoolRealArray &points);
	Color flood_fill_preprocess(const Vector2 &uv, const float c, const float s);
	Vector3 fill_to_uv_stretch(const Vector2 &coord, const Color &bb, const float pseed);
	Vector3 fill_to_uv_square(const Vector2 &coord, const Color &bb, const float pseed);
	Vector3 rgb_to_hsv(const Vector3 &c);
	Vector3 hsv_to_rgb(const Vector3 &c);
	Color adjust_hsv(const Color &color, const float hue, const float saturation, const float value);
	Color adjust_levels(const Color &color, const Color &in_min, const Color &in_mid, const Color &in_max, const Color &out_min, const Color &out_max);
	Color brightness_contrast(const Color &color, const float brightness, const float contrast);
	float grayscale_min(const Vector3 &c);
	float grayscale_max(const Vector3 &c);
	float grayscale_lightness(const Vector3 &c);
	float grayscale_average(const Vector3 &c);
	float grayscale_luminosity(const Vector3 &c);
	Color invert(const Color &color);
	Vector3 blend_normal(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity);
	Vector3 blend_dissolve(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity);
	Vector3 blend_multiply(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity);
	Vector3 blend_screen(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity);
	float blend_overlay_f(const float c1, const float c2);
	Vector3 blend_overlay(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity);
	Vector3 blend_hard_light(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity);
	float blend_soft_light_f(const float c1, const float c2);
	Vector3 blend_soft_light(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity);
	float blend_burn_f(const float c1, const float c2);
	Vector3 blend_burn(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity);
	float blend_dodge_f(const float c1, const float c2);
	Vector3 blend_dodge(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity);
	Vector3 blend_lighten(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity);
	Vector3 blend_darken(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity);
	Vector3 blend_difference(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity);
	Color radial_gradient_type_1(const Vector2 &uv, const float repeat, const PoolRealArray &data);
	Color radial_gradient_type_2(const Vector2 &uv, const float repeat, const PoolRealArray &data);
	Color radial_gradient_type_3(const Vector2 &uv, const float repeat, const PoolRealArray &data);
	Color radial_gradient_type_4(const Vector2 &uv, const float repeat, const PoolRealArray &data);
	Color normal_gradient_type_1(const Vector2 &uv, const float repeat, const float rotate, const PoolRealArray &data);
	Color normal_gradient_type_2(const Vector2 &uv, const float repeat, const float rotate, const PoolRealArray &data);
	Color normal_gradient_type_3(const Vector2 &uv, const float repeat, const float rotate, const PoolRealArray &data);
	Color normal_gradient_type_4(const Vector2 &uv, const float repeat, const float rotate, const PoolRealArray &data);
	Color circular_gradient_type_1(const Vector2 &uv, const float repeat, const PoolRealArray &data);
	Color circular_gradient_type_2(const Vector2 &uv, const float repeat, const PoolRealArray &data);
	Color circular_gradient_type_3(const Vector2 &uv, const float repeat, const PoolRealArray &data);
	Color circular_gradient_type_4(const Vector2 &uv, const float repeat, const PoolRealArray &data);
	Color gradient_type_1(const float x, const PoolRealArray &data);
	Color gradient_type_2(const float x, const PoolRealArray &data);
	Color gradient_type_3(const float x, const PoolRealArray &data);
	Color get_data_color(const int index, const PoolRealArray &data);
	float get_data_pos(const int index, const PoolRealArray &data);
	Color gradient_type_4(const float x, const PoolRealArray &data);
	Color gradient_type_5(const float x, const PoolRealArray &data);
	float dots(const Vector2 &uv, const float size, const float density, const float pseed);
	Color anisotropicc(const Vector2 &uv, const Vector2 &size, const float pseed, const float smoothness, const float interpolation);
	float anisotropic(const Vector2 &uv, const Vector2 &size, const float pseed, const float smoothness, const float interpolation);
	Vector3 color_dots(const Vector2 &uv, const float size, const float pseed);
	Color noise_color(const Vector2 &uv, const float size, const float pseed);
	Color fbmval(const Vector2 &uv, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed);
	Color perlin(const Vector2 &uv, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed);
	Color perlinabs(const Vector2 &uv, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed);
	Color simplex(const Vector2 &uv, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed);
	Color cellular(const Vector2 &uv, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed);
	Color cellular2(const Vector2 &uv, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed);
	Color cellular3(const Vector2 &uv, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed);
	Color cellular4(const Vector2 &uv, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed);
	Color cellular5(const Vector2 &uv, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed);
	Color cellular6(const Vector2 &uv, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed);
	float fbmf(const Vector2 &coord, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed);
	float perlinf(const Vector2 &coord, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed);
	float perlinabsf(const Vector2 &coord, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed);
	float fbm_simplexf(const Vector2 &coord, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed);
	float cellularf(const Vector2 &coord, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed);
	float cellular2f(const Vector2 &coord, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed);
	float cellular3f(const Vector2 &coord, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed);
	float cellular4f(const Vector2 &coord, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed);
	float cellular5f(const Vector2 &coord, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed);
	float cellular6f(const Vector2 &coord, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed);
	float fbm_value(const Vector2 &coord, const Vector2 &size, const float pseed);
	float fbm_perlin(const Vector2 &coord, const Vector2 &size, const float pseed);
	float fbm_perlinabs(const Vector2 &coord, const Vector2 &size, const float pseed);
	Vector2 rgrad2(const Vector2 &p, const float rot, const float pseed);
	float fbm_simplex(const Vector2 &coord, const Vector2 &size, const float pseed);
	float fbm_cellular(const Vector2 &coord, const Vector2 &size, const float pseed);
	float fbm_cellular2(const Vector2 &coord, const Vector2 &size, const float pseed);
	float fbm_cellular3(const Vector2 &coord, const Vector2 &size, const float pseed);
	float fbm_cellular4(const Vector2 &coord, const Vector2 &size, const float pseed);
	float fbm_cellular5(const Vector2 &coord, const Vector2 &size, const float pseed);
	float fbm_cellular6(const Vector2 &coord, const Vector2 &size, const float pseed);
	Color perlinc(const Vector2 &uv, const Vector2 &size, const int iterations, const float persistence, const int pseed);
	float perlin2c(const Vector2 &uv, const Vector2 &size, const int iterations, const float persistence, const int pseed);
	Vector3 perlin_color(const Vector2 &uv, const Vector2 &size, const int iterations, const float persistence, const int pseed);
	Color perlin_colorc(const Vector2 &uv, const Vector2 &size, const int iterations, const float persistence, const int pseed);
	Color perlin_warp_1(const Vector2 &uv, const Vector2 &size, const int iterations, const float persistence, const int pseed, const Vector2 &translate, const float rotate, const Vector2 &size2);
	Color perlin_warp_2(const Vector2 &uv, const Vector2 &size, const int iterations, const float persistence, const int pseed, const Vector2 &translate, const float rotate, const Vector2 &size2);
	Color voronoi(const Vector2 &uv, const Vector2 &size, const Vector2 &stretch, const float intensity, const float randomness, const int pseed);
	Color voronoi_1(const Vector2 &uv, const Vector2 &size, const Vector2 &stretch, const float intensity, const float randomness, const int pseed);
	Color voronoi_2(const Vector2 &uv, const Vector2 &size, const Vector2 &stretch, const float intensity, const float randomness, const int pseed);
	Color voronoi_3(const Vector2 &uv, const Vector2 &size, const Vector2 &stretch, const float intensity, const float randomness, const int pseed);
	float pattern(const Vector2 &uv, const float x_scale, const float y_scale, const int ct, const int catx, const int caty);
	Color truchet1c(const Vector2 &uv, const float size, const float pseed);
	float truchet1(const Vector2 &uv, const Vector2 &pseed);
	Color truchet2c(const Vector2 &uv, const float size, const float pseed);
	float truchet2(const Vector2 &uv, const Vector2 &pseed);
	Color weavec(const Vector2 &uv, const Vector2 &count, const float width);
	float weave(const Vector2 &uv, const Vector2 &count, const float width);
	Vector3 weave2(const Vector2 &uv, const Vector2 &count, const float stitch, const float width_x, const float width_y);
	Color sinewavec(const Vector2 &uv, const float amplitude, const float frequency, const float phase);
	float sinewavef(const Vector2 &uv, const float amplitude, const float frequency, const float phase);
	float scratch(const Vector2 &uv, const Vector2 &size, const float waviness, const float angle, const float randomness, const Vector2 &pseed);
	float scratches(const Vector2 &uv, const int layers, const Vector2 &size, const float waviness, const float angle, const float randomness, const Vector2 &pseed);
	Color scratchesc(const Vector2 &uv, const int layers, const Vector2 &size, const float waviness, const float angle, const float randomness, const Vector2 &pseed);
	Color runesc(const Vector2 &uv, const Vector2 &col_row, const float pseed);
	float runesf(const Vector2 &uv, const Vector2 &col_row, const float pseed);
	float rune(const Vector2 &uv, const float pseed);
	Color IChingc(const Vector2 &uv, const Vector2 &row_col, const int pseed);
	float IChing(const Vector2 &uv, const float pseed);
	Color beehive_1c(const Vector2 &uv, const Vector2 &size, const int pseed);
	Color beehive_2c(const Vector2 &uv, const Vector2 &size, const int pseed);
	Color beehive_3c(const Vector2 &uv, const Vector2 &size, const int pseed);
	float beehive_dist(const Vector2 &p);
	Color beehive_center(const Vector2 &p);
	Vector3 brick_corner_uv(const Vector2 &uv, const Vector2 &bmin, const Vector2 &bmax, const float mortar, const float corner, const float pseed);
	Color brick(const Vector2 &uv, const Vector2 &bmin, const Vector2 &bmax, const float mortar, const float pround, const float bevel);
	Vector3 brick_random_color(const Vector2 &bmin, const Vector2 &bmax, const float pseed);
	Vector3 brick_uv(const Vector2 &uv, const Vector2 &bmin, const Vector2 &bmax, const float pseed);
	Color bricks_rb(const Vector2 &uv, const Vector2 &count, const float repeat, const float offset);
	Color bricks_rb2(const Vector2 &uv, const Vector2 &count, const float repeat, const float offset);
	Color bricks_hb(const Vector2 &uv, const Vector2 &count, const float repeat, const float offset);
	Color bricks_bw(const Vector2 &uv, const Vector2 &count, const float repeat, const float offset);
	Color bricks_sb(const Vector2 &uv, const Vector2 &count, const float repeat, const float offset);
	Color brick2(const Vector2 &uv, const Vector2 &bmin, const Vector2 &bmax, const float mortar, const float pround, const float bevel);
	Color bricks_uneven(const Vector2 &uv, const int iterations, const float min_size, const float randomness, const float pseed);
	Vector2 truchet_generic_uv(const Vector2 &uv, const float pseed);
	Color sdf_show(const float val, const float bevel);
	float sdf_circle(const Vector2 &uv, const Vector2 &c, const float r);
	float sdf_box(const Vector2 &uv, const Vector2 &c, const Vector2 &wh);
	Vector2 sdf_line(const Vector2 &uv, const Vector2 &a, const Vector2 &b, const float r);
	float sdf_rhombus(const Vector2 &uv, const Vector2 &c, const Vector2 &wh);
	float sdf_arc(const Vector2 &uv, const Vector2 &a, const Vector2 &r);
	float sdr_ndot(const Vector2 &a, const Vector2 &b);
	float sdRhombus(const Vector2 &p, const Vector2 &b);
	float sdArc(const Vector2 &p, const float a1, const float a2, const float ra, const float rb);
	float sdf_boolean_union(const float a, const float b);
	float sdf_boolean_substraction(const float a, const float b);
	float sdf_boolean_intersection(const float a, const float b);
	float sdf_smooth_boolean_union(const float d1, const float d2, const float k);
	float sdf_smooth_boolean_substraction(const float d1, const float d2, const float k);
	float sdf_smooth_boolean_intersection(const float d1, const float d2, const float k);
	float sdf_rounded_shape(const float a, const float r);
	float sdf_annular_shape(const float a, const float r);
	float sdf_morph(const float a, const float b, const float amount);
	Vector2 sdLine(const Vector2 &p, const Vector2 &a, const Vector2 &b);
	Vector2 sdf2d_rotate(const Vector2 &uv, const float a);
	Vector2 sdBezier(const Vector2 &pos, const Vector2 &A, const Vector2 &B, const Vector2 &C);
	Vector2 circle_repeat_transform_2d(const Vector2 &p, const float count);
	Vector2 sdNgon(const Vector2 &pos, const float r, const float n);
	Vector2 repeat_2d(const Vector2 &p, const Vector2 &r, const float pseed, const float randomness);
	float sdSmoothUnion(const float d1, const float d2, const float k);
	float sdSmoothSubtraction(const float d1, const float d2, const float k);
	float sdSmoothIntersection(const float d1, const float d2, const float k);
	float sdRipples(const float d, const float w, const int r);
	float sdPolygon(const Vector2 &p, const PoolVector2Array &v);
	Color raymarch(const Vector2 &uv);
	Color raymarch2(const Vector2 &uv);
	Color raymarch3(const Vector2 &uv);
	Vector2 sdf3d_sphere(const Vector3 &p, const float r);
	Vector2 sdf3d_box(const Vector3 &p, const float sx, const float sy, const float sz, const float r);
	Vector2 sdf3d_cylinder_y(const Vector3 &p, const float r, const float l);
	Vector2 sdf3d_cylinder_x(const Vector3 &p, const float r, const float l);
	Vector2 sdf3d_cylinder_z(const Vector3 &p, const float r, const float l);
	Vector2 sdf3d_capsule_y(const Vector3 &p, const float r, const float l);
	Vector2 sdf3d_capsule_x(const Vector3 &p, const float r, const float l);
	Vector2 sdf3d_capsule_z(const Vector3 &p, const float r, const float l);
	Vector2 sdf3d_cone_px(const Vector3 &p, const float a);
	Vector2 sdf3d_cone_nx(const Vector3 &p, const float a);
	Vector2 sdf3d_cone_py(const Vector3 &p, const float a);
	Vector2 sdf3d_cone_ny(const Vector3 &p, const float a);
	Vector2 sdf3d_cone_pz(const Vector3 &p, const float a);
	Vector2 sdf3d_cone_nz(const Vector3 &p, const float a);
	Vector2 sdf3d_torus_x(const Vector3 &p, const float R, const float r);
	Vector2 sdf3d_torus_y(const Vector3 &p, const float R, const float r);
	Vector2 sdf3d_torus_z(const Vector3 &p, const float R, const float r);
	Vector2 sdf3d_raymarch(const Vector2 &uv);
	Vector3 sdf3d_normal(const Vector3 &p);
	Vector2 sdf3dc_union(const Vector2 &a, const Vector2 &b);
	Vector2 sdf3dc_sub(const Vector2 &a, const Vector2 &b);
	Vector2 sdf3dc_inter(const Vector2 &a, const Vector2 &b);
	Vector2 sdf3d_smooth_union(const Vector2 &d1, const Vector2 &d2, const float k);
	Vector2 sdf3d_smooth_subtraction(const Vector2 &d1, const Vector2 &d2, const float k);
	Vector2 sdf3d_smooth_intersection(const Vector2 &d1, const Vector2 &d2, const float k);
	Vector2 sdf3d_rounded(const Vector2 &v, const float r);
	Vector3 sdf3d_elongation(const Vector3 &p, const Vector3 &v);
	Vector3 sdf3d_repeat(const Vector3 &p, const Vector2 &r, const float randomness, const int pseed);
	Vector3 repeat(const Vector3 &p, const Vector3 &r, const float pseed, const float randomness);
	Vector3 rotate3d(const Vector3 &p, const Vector3 &a);
	Vector3 circle_repeat_transform(const Vector3 &p, const float count);
	Vector2 sdf3d_input(const Vector3 &p);
	float sphere(const Vector2 &uv, const Vector2 &c, const float r);
	float shape_circle(const Vector2 &uv, const float sides, const float size, const float edge);
	float shape_polygon(const Vector2 &uv, const float sides, const float size, const float edge);
	float shape_star(const Vector2 &uv, const float sides, const float size, const float edge);
	float shape_curved_star(const Vector2 &uv, const float sides, const float size, const float edge);
	float shape_rays(const Vector2 &uv, const float sides, const float size, const float edge);
	Vector2 transform2_clamp(const Vector2 &uv);
	Vector2 transform2(const Vector2 &uv, const Vector2 &translate, const float rotate, const Vector2 &scale);
	Vector2 rotate(const Vector2 &uv, const Vector2 &center, const float rotate);
	Vector2 scale(const Vector2 &uv, const Vector2 &center, const Vector2 &scale);
	Vector2 uvmirror_h(const Vector2 &uv, const float offset);
	Vector2 uvmirror_v(const Vector2 &uv, const float offset);
	Vector2 kal_rotate(const Vector2 &uv, const float count, const float offset);
	Vector2 get_from_tileset(const float count, const float pseed, const Vector2 &uv);
	Vector2 custom_uv_transform(const Vector2 &uv, const Vector2 &cst_scale, const float rnd_rotate, const float rnd_scale, const Vector2 &pseed);

	Ref<Image> generate_histogram(const Ref<Image> &input, const int texture_size = 256);

	void register_node_class(const String &category, const String &cls);
	void unregister_node_class(const String &category, const String &cls);

	void register_node_script(const String &category, const String &file_path);
	void unregister_node_script(const String &category, const String &file_path);

	static _MMAlgos *get_singleton();

	_MMAlgos();
	~_MMAlgos();

protected:
	static void _bind_methods();

	static _MMAlgos *self;
};

#endif
