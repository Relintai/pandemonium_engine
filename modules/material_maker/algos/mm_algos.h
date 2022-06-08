#ifndef MM_ALGOS_H
#define MM_ALGOS_H

#include "core/color.h"
#include "core/math/vector2.h"
#include "core/math/vector3.h"
#include "core/pool_vector.h"
#include "core/variant.h"

#include "core/reference.h"

class MMAlgos : public Reference {
	GDCLASS(MMAlgos, Reference);

public:
	static Vector3 clampv3(const Vector3 &v, const Vector3 &mi, const Vector3 &ma);
	static Color floorc(const Color &a);
	static Vector2 floorv2(const Vector2 &a);
	static Vector3 floorv3(const Vector3 &a);
	static Vector2 smoothstepv2(const float a, const float b, const Vector2 &c);
	static Vector2 maxv2(const Vector2 &a, const Vector2 &b);
	static Vector3 maxv3(const Vector3 &a, const Vector3 &b);
	static Vector2 absv2(const Vector2 &v);
	static Vector3 absv3(const Vector3 &v);
	static Vector2 cosv2(const Vector2 &v);
	static Vector3 cosv3(const Vector3 &v);
	static Vector2 powv2(const Vector2 &x, const Vector2 &y);
	static Vector3 modv3(const Vector3 &a, const Vector3 &b);
	static Vector2 modv2(const Vector2 &a, const Vector2 &b);
	static float modf(const float x, const float y);
	static Vector2 fractv2(const Vector2 &v);
	static Vector3 fractv3(const Vector3 &v);
	static float fract(const float f);
	static Vector2 clampv2(const Vector2 &v, const Vector2 &pmin, const Vector2 &pmax);
	static Vector2 minv2(const Vector2 &v1, const Vector2 &v2);
	static Vector3 minv3(const Vector3 &v1, const Vector3 &v2);
	static float rand(const Vector2 &x);
	static Vector2 rand2(const Vector2 &x);
	static Vector3 rand3(const Vector2 &x);
	static float step(const float edge, const float x);
	static Vector2 stepv2(const Vector2 &edge, const Vector2 &x);
	static Vector2 signv2(const Vector2 &x);
	static Vector2 transform(const Vector2 &uv, const Vector2 &translate, const float rotate, const Vector2 &scale, const bool repeat);
	static float fractf(const float x);
	static float mix_mul(const float x, const float y);
	static float mix_add(const float x, const float y);
	static float mix_max(const float x, const float y);
	static float mix_min(const float x, const float y);
	static float mix_xor(const float x, const float y);
	static float mix_pow(const float x, const float y);
	static float wave_constant(const float x);
	static float wave_sine(const float x);
	static float wave_triangle(const float x);
	static float wave_sawtooth(const float x);
	static float wave_square(const float x);
	static float wave_bounce(const float x);
	static Color sinewave(const Vector2 &uv, const float amplitude, const float frequency, const float phase);
	static float ThickLine(const Vector2 &uv, const Vector2 &posA, const Vector2 &posB, const float radiusInv);
	static float curve(const float x, const PoolRealArray &points);
	static Color flood_fill_preprocess(const Vector2 &uv, const float c, const float s);
	static Vector3 fill_to_uv_stretch(const Vector2 &coord, const Color &bb, const float pseed);
	static Vector3 fill_to_uv_square(const Vector2 &coord, const Color &bb, const float pseed);
	static Vector3 rgb_to_hsv(const Vector3 &c);
	static Vector3 hsv_to_rgb(const Vector3 &c);
	static Color adjust_hsv(const Color &color, const float hue, const float saturation, const float value);
	static Color brightness_contrast(const Color &color, const float brightness, const float contrast);
	static float grayscale_min(const Vector3 &c);
	static float grayscale_max(const Vector3 &c);
	static float grayscale_lightness(const Vector3 &c);
	static float grayscale_average(const Vector3 &c);
	static float grayscale_luminosity(const Vector3 &c);
	static Color invert(const Color &color);
	static Vector3 blend_normal(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity);
	static Vector3 blend_dissolve(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity);
	static Vector3 blend_multiply(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity);
	static Vector3 blend_screen(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity);
	static float blend_overlay_f(const float c1, const float c2);
	static Vector3 blend_overlay(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity);
	static Vector3 blend_hard_light(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity);
	static float blend_soft_light_f(const float c1, const float c2);
	static Vector3 blend_soft_light(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity);
	static float blend_burn_f(const float c1, const float c2);
	static Vector3 blend_burn(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity);
	static float blend_dodge_f(const float c1, const float c2);
	static Vector3 blend_dodge(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity);
	static Vector3 blend_lighten(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity);
	static Vector3 blend_darken(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity);
	static Vector3 blend_difference(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity);
	static Color radial_gradient_type_1(const Vector2 &uv, const float repeat, const PoolRealArray &data);
	static Color radial_gradient_type_2(const Vector2 &uv, const float repeat, const PoolRealArray &data);
	static Color radial_gradient_type_3(const Vector2 &uv, const float repeat, const PoolRealArray &data);
	static Color radial_gradient_type_4(const Vector2 &uv, const float repeat, const PoolRealArray &data);
	static Color normal_gradient_type_1(const Vector2 &uv, const float repeat, const float rotate, const PoolRealArray &data);
	static Color normal_gradient_type_2(const Vector2 &uv, const float repeat, const float rotate, const PoolRealArray &data);
	static Color normal_gradient_type_3(const Vector2 &uv, const float repeat, const float rotate, const PoolRealArray &data);
	static Color normal_gradient_type_4(const Vector2 &uv, const float repeat, const float rotate, const PoolRealArray &data);
	static Color circular_gradient_type_1(const Vector2 &uv, const float repeat, const PoolRealArray &data);
	static Color circular_gradient_type_2(const Vector2 &uv, const float repeat, const PoolRealArray &data);
	static Color circular_gradient_type_3(const Vector2 &uv, const float repeat, const PoolRealArray &data);
	static Color circular_gradient_type_4(const Vector2 &uv, const float repeat, const PoolRealArray &data);
	static Color gradient_type_1(const float x, const PoolRealArray &data);
	static Color gradient_type_2(const float x, const PoolRealArray &data);
	static Color gradient_type_3(const float x, const PoolRealArray &data);
	static Color get_data_color(const int index, const PoolRealArray &data);
	static float get_data_pos(const int index, const PoolRealArray &data);
	static Color gradient_type_4(const float x, const PoolRealArray &data);
	static Color gradient_type_5(const float x, const PoolRealArray &data);
	static float dots(const Vector2 &uv, const float size, const float density, const float pseed);
	static Color anisotropicc(const Vector2 &uv, const Vector2 &size, const float pseed, const float smoothness, const float interpolation);
	static float anisotropic(const Vector2 &uv, const Vector2 &size, const float pseed, const float smoothness, const float interpolation);
	static Vector3 color_dots(const Vector2 &uv, const float size, const float pseed);
	static Color noise_color(const Vector2 &uv, const float size, const float pseed);
	static Color fbmval(const Vector2 &uv, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed);
	static Color perlin(const Vector2 &uv, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed);
	static Color perlinabs(const Vector2 &uv, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed);
	static Color simplex(const Vector2 &uv, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed);
	static Color cellular(const Vector2 &uv, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed);
	static Color cellular2(const Vector2 &uv, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed);
	static Color cellular3(const Vector2 &uv, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed);
	static Color cellular4(const Vector2 &uv, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed);
	static Color cellular5(const Vector2 &uv, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed);
	static Color cellular6(const Vector2 &uv, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed);
	static float fbmf(const Vector2 &coord, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed);
	static float perlinf(const Vector2 &coord, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed);
	static float perlinabsf(const Vector2 &coord, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed);
	static float fbm_simplexf(const Vector2 &coord, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed);
	static float cellularf(const Vector2 &coord, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed);
	static float cellular2f(const Vector2 &coord, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed);
	static float cellular3f(const Vector2 &coord, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed);
	static float cellular4f(const Vector2 &coord, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed);
	static float cellular5f(const Vector2 &coord, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed);
	static float cellular6f(const Vector2 &coord, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed);
	static float fbm_value(const Vector2 &coord, const Vector2 &size, const float pseed);
	static float fbm_perlin(const Vector2 &coord, const Vector2 &size, const float pseed);
	static float fbm_perlinabs(const Vector2 &coord, const Vector2 &size, const float pseed);
	static Vector2 rgrad2(const Vector2 &p, const float rot, const float pseed);
	static float fbm_simplex(const Vector2 &coord, const Vector2 &size, const float pseed);
	static float fbm_cellular(const Vector2 &coord, const Vector2 &size, const float pseed);
	static float fbm_cellular2(const Vector2 &coord, const Vector2 &size, const float pseed);
	static float fbm_cellular3(const Vector2 &coord, const Vector2 &size, const float pseed);
	static float fbm_cellular4(const Vector2 &coord, const Vector2 &size, const float pseed);
	static float fbm_cellular5(const Vector2 &coord, const Vector2 &size, const float pseed);
	static float fbm_cellular6(const Vector2 &coord, const Vector2 &size, const float pseed);
	static Color perlinc(const Vector2 &uv, const Vector2 &size, const int iterations, const float persistence, const int pseed);
	static float perlin2c(const Vector2 &uv, const Vector2 &size, const int iterations, const float persistence, const int pseed);
	static Vector3 perlin_color(const Vector2 &uv, const Vector2 &size, const int iterations, const float persistence, const int pseed);
	static Color perlin_colorc(const Vector2 &uv, const Vector2 &size, const int iterations, const float persistence, const int pseed);
	static Color perlin_warp_1(const Vector2 &uv, const Vector2 &size, const int iterations, const float persistence, const int pseed, const Vector2 &translate, const float rotate, const Vector2 &size2);
	static Color perlin_warp_2(const Vector2 &uv, const Vector2 &size, const int iterations, const float persistence, const int pseed, const Vector2 &translate, const float rotate, const Vector2 &size2);
	static Color voronoi(const Vector2 &uv, const Vector2 &size, const Vector2 &stretch, const float intensity, const float randomness, const int pseed);
	static Color voronoi_1(const Vector2 &uv, const Vector2 &size, const Vector2 &stretch, const float intensity, const float randomness, const int pseed);
	static Color voronoi_2(const Vector2 &uv, const Vector2 &size, const Vector2 &stretch, const float intensity, const float randomness, const int pseed);
	static Color voronoi_3(const Vector2 &uv, const Vector2 &size, const Vector2 &stretch, const float intensity, const float randomness, const int pseed);

	enum CombinerAxisType {
		COMBINER_AXIS_TYPE_SINE = 0,
		COMBINER_AXIS_TYPE_TRIANGLE,
		COMBINER_AXIS_TYPE_SQUARE,
		COMBINER_AXIS_TYPE_SAWTOOTH,
		COMBINER_AXIS_TYPE_CONSTANT,
		COMBINER_AXIS_TYPE_BOUNCE
	};

	enum CombinerType {
		COMBINER_TYPE_MULTIPLY = 0,
		COMBINER_TYPE_ADD,
		COMBINER_TYPE_MAX,
		COMBINER_TYPE_MIN,
		COMBINER_TYPE_XOR,
		COMBINER_TYPE_POW
	};

	static float pattern(const Vector2 &uv, const float x_scale, const float y_scale, const int ct, const int catx, const int caty);
	static Color truchet1c(const Vector2 &uv, const float size, const float pseed);
	static float truchet1(const Vector2 &uv, const Vector2 &pseed);
	static Color truchet2c(const Vector2 &uv, const float size, const float pseed);
	static float truchet2(const Vector2 &uv, const Vector2 &pseed);
	static Color weavec(const Vector2 &uv, const Vector2 &count, const float width);
	static float weave(const Vector2 &uv, const Vector2 &count, const float width);
	static Vector3 weave2(const Vector2 &uv, const Vector2 &count, const float stitch, const float width_x, const float width_y);
	static Color sinewavec(const Vector2 &uv, const float amplitude, const float frequency, const float phase);
	static float sinewavef(const Vector2 &uv, const float amplitude, const float frequency, const float phase);
	static float scratch(const Vector2 &uv, const Vector2 &size, const float waviness, const float angle, const float randomness, const Vector2 &pseed);
	static float scratches(const Vector2 &uv, const int layers, const Vector2 &size, const float waviness, const float angle, const float randomness, const Vector2 &pseed);
	static Color scratchesc(const Vector2 &uv, const int layers, const Vector2 &size, const float waviness, const float angle, const float randomness, const Vector2 &pseed);
	static Color runesc(const Vector2 &uv, const Vector2 &col_row, const float pseed);
	static float runesf(const Vector2 &uv, const Vector2 &col_row, const float pseed);
	static float rune(const Vector2 &uv, const float pseed);
	static Color IChingc(const Vector2 &uv, const Vector2 &row_col, const int pseed);
	static float IChing(const Vector2 &uv, const float pseed);
	static Color beehive_1c(const Vector2 &uv, const Vector2 &size, const int pseed);
	static Color beehive_2c(const Vector2 &uv, const Vector2 &size, const int pseed);
	static Color beehive_3c(const Vector2 &uv, const Vector2 &size, const int pseed);
	static float beehive_dist(const Vector2 &p);
	static Color beehive_center(const Vector2 &p);
	static Vector3 brick_corner_uv(const Vector2 &uv, const Vector2 &bmin, const Vector2 &bmax, const float mortar, const float corner, const float pseed);
	static Color brick(const Vector2 &uv, const Vector2 &bmin, const Vector2 &bmax, const float mortar, const float pround, const float bevel);
	static Vector3 brick_random_color(const Vector2 &bmin, const Vector2 &bmax, const float pseed);
	static Vector3 brick_uv(const Vector2 &uv, const Vector2 &bmin, const Vector2 &bmax, const float pseed);
	static Color bricks_rb(const Vector2 &uv, const Vector2 &count, const float repeat, const float offset);
	static Color bricks_rb2(const Vector2 &uv, const Vector2 &count, const float repeat, const float offset);
	static Color bricks_hb(const Vector2 &uv, const Vector2 &count, const float repeat, const float offset);
	static Color bricks_bw(const Vector2 &uv, const Vector2 &count, const float repeat, const float offset);
	static Color bricks_sb(const Vector2 &uv, const Vector2 &count, const float repeat, const float offset);
	static Color brick2(const Vector2 &uv, const Vector2 &bmin, const Vector2 &bmax, const float mortar, const float pround, const float bevel);
	static Color bricks_uneven(const Vector2 &uv, const int iterations, const float min_size, const float randomness, const float pseed);
	static Vector2 truchet_generic_uv(const Vector2 &uv, const float pseed);
	static Color sdf_show(const float val, const float bevel);
	static float sdf_circle(const Vector2 &uv, const Vector2 &c, const float r);
	static float sdf_box(const Vector2 &uv, const Vector2 &c, const Vector2 &wh);
	static Vector2 sdf_line(const Vector2 &uv, const Vector2 &a, const Vector2 &b, const float r);
	static float sdf_rhombus(const Vector2 &uv, const Vector2 &c, const Vector2 &wh);
	static float sdf_arc(const Vector2 &uv, const Vector2 &a, const Vector2 &r);
	static float sdr_ndot(const Vector2 &a, const Vector2 &b);
	static float sdRhombus(const Vector2 &p, const Vector2 &b);
	static float sdArc(const Vector2 &p, const float a1, const float a2, const float ra, const float rb);
	static float sdf_boolean_union(const float a, const float b);
	static float sdf_boolean_substraction(const float a, const float b);
	static float sdf_boolean_intersection(const float a, const float b);
	static float sdf_smooth_boolean_union(const float d1, const float d2, const float k);
	static float sdf_smooth_boolean_substraction(const float d1, const float d2, const float k);
	static float sdf_smooth_boolean_intersection(const float d1, const float d2, const float k);
	static float sdf_rounded_shape(const float a, const float r);
	static float sdf_annular_shape(const float a, const float r);
	static float sdf_morph(const float a, const float b, const float amount);
	static Vector2 sdLine(const Vector2 &p, const Vector2 &a, const Vector2 &b);
	static Vector2 sdf2d_rotate(const Vector2 &uv, const float a);
	static Vector2 sdBezier(const Vector2 &pos, const Vector2 &A, const Vector2 &B, const Vector2 &C);
	static Vector2 circle_repeat_transform_2d(const Vector2 &p, const float count);
	static Vector2 sdNgon(const Vector2 &pos, const float r, const float n);
	static Vector2 repeat_2d(const Vector2 &p, const Vector2 &r, const float pseed, const float randomness);
	static float sdSmoothUnion(const float d1, const float d2, const float k);
	static float sdSmoothSubtraction(const float d1, const float d2, const float k);
	static float sdSmoothIntersection(const float d1, const float d2, const float k);
	static float sdRipples(const float d, const float w, const int r);
	static float sdPolygon(const Vector2 &p, const PoolVector2Array &v);
	static Color raymarch(const Vector2 &uv);
	static Color raymarch2(const Vector2 &uv);
	static Color raymarch3(const Vector2 &uv);
	static Vector2 sdf3d_sphere(const Vector3 &p, const float r);
	static Vector2 sdf3d_box(const Vector3 &p, const float sx, const float sy, const float sz, const float r);
	static Vector2 sdf3d_cylinder_y(const Vector3 &p, const float r, const float l);
	static Vector2 sdf3d_cylinder_x(const Vector3 &p, const float r, const float l);
	static Vector2 sdf3d_cylinder_z(const Vector3 &p, const float r, const float l);
	static Vector2 sdf3d_capsule_y(const Vector3 &p, const float r, const float l);
	static Vector2 sdf3d_capsule_x(const Vector3 &p, const float r, const float l);
	static Vector2 sdf3d_capsule_z(const Vector3 &p, const float r, const float l);
	static Vector2 sdf3d_cone_px(const Vector3 &p, const float a);
	static Vector2 sdf3d_cone_nx(const Vector3 &p, const float a);
	static Vector2 sdf3d_cone_py(const Vector3 &p, const float a);
	static Vector2 sdf3d_cone_ny(const Vector3 &p, const float a);
	static Vector2 sdf3d_cone_pz(const Vector3 &p, const float a);
	static Vector2 sdf3d_cone_nz(const Vector3 &p, const float a);
	static Vector2 sdf3d_torus_x(const Vector3 &p, const float R, const float r);
	static Vector2 sdf3d_torus_y(const Vector3 &p, const float R, const float r);
	static Vector2 sdf3d_torus_z(const Vector3 &p, const float R, const float r);
	static Vector2 sdf3d_raymarch(const Vector2 &uv);
	static Vector3 sdf3d_normal(const Vector3 &p);
	static Vector2 sdf3dc_union(const Vector2 &a, const Vector2 &b);
	static Vector2 sdf3dc_sub(const Vector2 &a, const Vector2 &b);
	static Vector2 sdf3dc_inter(const Vector2 &a, const Vector2 &b);
	static Vector2 sdf3d_smooth_union(const Vector2 &d1, const Vector2 &d2, const float k);
	static Vector2 sdf3d_smooth_subtraction(const Vector2 &d1, const Vector2 &d2, const float k);
	static Vector2 sdf3d_smooth_intersection(const Vector2 &d1, const Vector2 &d2, const float k);
	static Vector2 sdf3d_rounded(const Vector2 &v, const float r);
	static Vector3 sdf3d_elongation(const Vector3 &p, const Vector3 &v);
	static Vector3 sdf3d_repeat(const Vector3 &p, const Vector2 &r, const float randomness, const int pseed);
	static Vector3 repeat(const Vector3 &p, const Vector3 &r, const float pseed, const float randomness);
	static Vector3 rotate3d(const Vector3 &p, const Vector3 &a);
	static Vector3 circle_repeat_transform(const Vector3 &p, const float count);
	static Vector2 sdf3d_input(const Vector3 &p);
	static float sphere(const Vector2 &uv, const Vector2 &c, const float r);
	static float shape_circle(const Vector2 &uv, const float sides, const float size, const float edge);
	static float shape_polygon(const Vector2 &uv, const float sides, const float size, const float edge);
	static float shape_star(const Vector2 &uv, const float sides, const float size, const float edge);
	static float shape_curved_star(const Vector2 &uv, const float sides, const float size, const float edge);
	static float shape_rays(const Vector2 &uv, const float sides, const float size, const float edge);
	static Vector2 transform2_clamp(const Vector2 &uv);
	static Vector2 transform2(const Vector2 &uv, const Vector2 &translate, const float rotate, const Vector2 &scale);
	static Vector2 rotate(const Vector2 &uv, const Vector2 &center, const float rotate);
	static Vector2 scale(const Vector2 &uv, const Vector2 &center, const Vector2 &scale);
	static Vector2 uvmirror_h(const Vector2 &uv, const float offset);
	static Vector2 uvmirror_v(const Vector2 &uv, const float offset);
	static Vector2 kal_rotate(const Vector2 &uv, const float count, const float offset);
	static Vector2 get_from_tileset(const float count, const float pseed, const Vector2 &uv);
	static Vector2 custom_uv_transform(const Vector2 &uv, const Vector2 &cst_scale, const float rnd_rotate, const float rnd_scale, const Vector2 &pseed);

	static void register_node_class(const String &category, const String &cls);
	static void unregister_node_class(const String &category, const String &cls);

	static void register_node_script(const String &category, const String &file_path);
	static void unregister_node_script(const String &category, const String &file_path);

	enum MMNodeRegistryType {
		MMNODE_REGISTRY_TYPE_CLASS = 0,
		MMNODE_REGISTRY_TYPE_SCRIPT
	};

	struct MMNodeRegistryEntry {
		MMNodeRegistryType type;
		String data;
	};

	struct MMNodeRegistryCategory {
		String category_name;
		Vector<MMNodeRegistryEntry> entries;
	};

	static Vector<MMNodeRegistryCategory> mm_node_registry;

	MMAlgos();
	~MMAlgos();

protected:
	static void _bind_methods();
};

#endif
