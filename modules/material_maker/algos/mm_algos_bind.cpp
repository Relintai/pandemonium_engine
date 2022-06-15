
#include "mm_algos_bind.h"

#include "core/method_bind_ext.gen.inc"
#include "mm_algos.h"

Vector3 _MMAlgos::clampv3(const Vector3 &v, const Vector3 &mi, const Vector3 &ma) {
	return MMAlgos::clampv3(v, mi, ma);
}

Color _MMAlgos::floorc(const Color &a) {
	return MMAlgos::floorc(a);
}

Vector2 _MMAlgos::floorv2(const Vector2 &a) {
	return MMAlgos::floorv2(a);
}

Vector3 _MMAlgos::floorv3(const Vector3 &a) {
	return MMAlgos::floorv3(a);
}

Vector2 _MMAlgos::smoothstepv2(const float a, const float b, const Vector2 &c) {
	return MMAlgos::smoothstepv2(a, b, c);
}

Vector2 _MMAlgos::maxv2(const Vector2 &a, const Vector2 &b) {
	return MMAlgos::maxv2(a, b);
}

Vector3 _MMAlgos::maxv3(const Vector3 &a, const Vector3 &b) {
	return MMAlgos::maxv3(a, b);
}

Vector2 _MMAlgos::absv2(const Vector2 &v) {
	return MMAlgos::absv2(v);
}

Vector3 _MMAlgos::absv3(const Vector3 &v) {
	return MMAlgos::absv3(v);
}

Vector2 _MMAlgos::cosv2(const Vector2 &v) {
	return MMAlgos::cosv2(v);
}

Vector3 _MMAlgos::cosv3(const Vector3 &v) {
	return MMAlgos::cosv3(v);
}

Vector2 _MMAlgos::powv2(const Vector2 &x, const Vector2 &y) {
	return MMAlgos::powv2(x, y);
}

Vector3 _MMAlgos::modv3(const Vector3 &a, const Vector3 &b) {
	return MMAlgos::modv3(a, b);
}

Vector2 _MMAlgos::modv2(const Vector2 &a, const Vector2 &b) {
	return MMAlgos::modv2(a, b);
}

float _MMAlgos::modf(const float x, const float y) {
	return MMAlgos::modf(x, y);
}

Vector2 _MMAlgos::fractv2(const Vector2 &v) {
	return MMAlgos::fractv2(v);
}

Vector3 _MMAlgos::fractv3(const Vector3 &v) {
	return MMAlgos::fractv3(v);
}

float _MMAlgos::fract(const float f) {
	return MMAlgos::fract(f);
}

Vector2 _MMAlgos::clampv2(const Vector2 &v, const Vector2 &pmin, const Vector2 &pmax) {
	return MMAlgos::clampv2(v, pmin, pmax);
}

Vector2 _MMAlgos::minv2(const Vector2 &v1, const Vector2 &v2) {
	return MMAlgos::minv2(v1, v2);
}

Vector3 _MMAlgos::minv3(const Vector3 &v1, const Vector3 &v2) {
	return MMAlgos::minv3(v1, v2);
}

float _MMAlgos::rand(const Vector2 &x) {
	return MMAlgos::rand(x);
}

Vector2 _MMAlgos::rand2(const Vector2 &x) {
	return MMAlgos::rand2(x);
}

Vector3 _MMAlgos::rand3(const Vector2 &x) {
	return MMAlgos::rand3(x);
}

float _MMAlgos::step(const float edge, const float x) {
	return MMAlgos::step(edge, x);
}

Vector2 _MMAlgos::stepv2(const Vector2 &edge, const Vector2 &x) {
	return MMAlgos::stepv2(edge, x);
}

Vector2 _MMAlgos::signv2(const Vector2 &x) {
	return MMAlgos::signv2(x);
}

Vector2 _MMAlgos::transform(const Vector2 &uv, const Vector2 &translate, const float rotate, const Vector2 &scale, const bool repeat) {
	return MMAlgos::transform(uv, translate, rotate, scale, repeat);
}

float _MMAlgos::fractf(const float x) {
	return MMAlgos::fractf(x);
}

float _MMAlgos::mix_mul(const float x, const float y) {
	return MMAlgos::mix_mul(x, y);
}

float _MMAlgos::mix_add(const float x, const float y) {
	return MMAlgos::mix_add(x, y);
}

float _MMAlgos::mix_max(const float x, const float y) {
	return MMAlgos::mix_max(x, y);
}

float _MMAlgos::mix_min(const float x, const float y) {
	return MMAlgos::mix_min(x, y);
}

float _MMAlgos::mix_xor(const float x, const float y) {
	return MMAlgos::mix_xor(x, y);
}

float _MMAlgos::mix_pow(const float x, const float y) {
	return MMAlgos::mix_pow(x, y);
}

float _MMAlgos::wave_constant(const float x) {
	return MMAlgos::wave_constant(x);
}

float _MMAlgos::wave_sine(const float x) {
	return MMAlgos::wave_sine(x);
}

float _MMAlgos::wave_triangle(const float x) {
	return MMAlgos::wave_triangle(x);
}

float _MMAlgos::wave_sawtooth(const float x) {
	return MMAlgos::wave_sawtooth(x);
}

float _MMAlgos::wave_square(const float x) {
	return MMAlgos::wave_square(x);
}

float _MMAlgos::wave_bounce(const float x) {
	return MMAlgos::wave_bounce(x);
}

Color _MMAlgos::sinewave(const Vector2 &uv, const float amplitude, const float frequency, const float phase) {
	return MMAlgos::sinewave(uv, amplitude, frequency, phase);
}

float _MMAlgos::ThickLine(const Vector2 &uv, const Vector2 &posA, const Vector2 &posB, const float radiusInv) {
	return MMAlgos::ThickLine(uv, posA, posB, radiusInv);
}

float _MMAlgos::curve(const float x, const PoolRealArray &points) {
	return MMAlgos::curve(x, points);
}

Color _MMAlgos::flood_fill_preprocess(const Vector2 &uv, const float c, const float s) {
	return MMAlgos::flood_fill_preprocess(uv, c, s);
}

Vector3 _MMAlgos::fill_to_uv_stretch(const Vector2 &coord, const Color &bb, const float pseed) {
	return MMAlgos::fill_to_uv_stretch(coord, bb, pseed);
}

Vector3 _MMAlgos::fill_to_uv_square(const Vector2 &coord, const Color &bb, const float pseed) {
	return MMAlgos::fill_to_uv_square(coord, bb, pseed);
}

Vector3 _MMAlgos::rgb_to_hsv(const Vector3 &c) {
	return MMAlgos::rgb_to_hsv(c);
}

Vector3 _MMAlgos::hsv_to_rgb(const Vector3 &c) {
	return MMAlgos::hsv_to_rgb(c);
}

Color _MMAlgos::adjust_hsv(const Color &color, const float hue, const float saturation, const float value) {
	return MMAlgos::adjust_hsv(color, hue, saturation, value);
}

Color _MMAlgos::brightness_contrast(const Color &color, const float brightness, const float contrast) {
	return MMAlgos::brightness_contrast(color, brightness, contrast);
}

float _MMAlgos::grayscale_min(const Vector3 &c) {
	return MMAlgos::grayscale_min(c);
}

float _MMAlgos::grayscale_max(const Vector3 &c) {
	return MMAlgos::grayscale_max(c);
}

float _MMAlgos::grayscale_lightness(const Vector3 &c) {
	return MMAlgos::grayscale_lightness(c);
}

float _MMAlgos::grayscale_average(const Vector3 &c) {
	return MMAlgos::grayscale_average(c);
}

float _MMAlgos::grayscale_luminosity(const Vector3 &c) {
	return MMAlgos::grayscale_luminosity(c);
}

Color _MMAlgos::invert(const Color &color) {
	return MMAlgos::invert(color);
}

Vector3 _MMAlgos::blend_normal(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity) {
	return MMAlgos::blend_normal(uv, c1, c2, opacity);
}

Vector3 _MMAlgos::blend_dissolve(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity) {
	return MMAlgos::blend_dissolve(uv, c1, c2, opacity);
}

Vector3 _MMAlgos::blend_multiply(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity) {
	return MMAlgos::blend_multiply(uv, c1, c2, opacity);
}

Vector3 _MMAlgos::blend_screen(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity) {
	return MMAlgos::blend_screen(uv, c1, c2, opacity);
}

float _MMAlgos::blend_overlay_f(const float c1, const float c2) {
	return MMAlgos::blend_overlay_f(c1, c2);
}

Vector3 _MMAlgos::blend_overlay(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity) {
	return MMAlgos::blend_overlay(uv, c1, c2, opacity);
}

Vector3 _MMAlgos::blend_hard_light(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity) {
	return MMAlgos::blend_hard_light(uv, c1, c2, opacity);
}

float _MMAlgos::blend_soft_light_f(const float c1, const float c2) {
	return MMAlgos::blend_soft_light_f(c1, c2);
}

Vector3 _MMAlgos::blend_soft_light(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity) {
	return MMAlgos::blend_soft_light(uv, c1, c2, opacity);
}

float _MMAlgos::blend_burn_f(const float c1, const float c2) {
	return MMAlgos::blend_burn_f(c1, c2);
}

Vector3 _MMAlgos::blend_burn(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity) {
	return MMAlgos::blend_burn(uv, c1, c2, opacity);
}

float _MMAlgos::blend_dodge_f(const float c1, const float c2) {
	return MMAlgos::blend_dodge_f(c1, c2);
}

Vector3 _MMAlgos::blend_dodge(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity) {
	return MMAlgos::blend_dodge(uv, c1, c2, opacity);
}

Vector3 _MMAlgos::blend_lighten(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity) {
	return MMAlgos::blend_lighten(uv, c1, c2, opacity);
}

Vector3 _MMAlgos::blend_darken(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity) {
	return MMAlgos::blend_darken(uv, c1, c2, opacity);
}

Vector3 _MMAlgos::blend_difference(const Vector2 &uv, const Vector3 &c1, const Vector3 &c2, const float opacity) {
	return MMAlgos::blend_difference(uv, c1, c2, opacity);
}

Color _MMAlgos::radial_gradient_type_1(const Vector2 &uv, const float repeat, const PoolRealArray &data) {
	return MMAlgos::radial_gradient_type_1(uv, repeat, data);
}

Color _MMAlgos::radial_gradient_type_2(const Vector2 &uv, const float repeat, const PoolRealArray &data) {
	return MMAlgos::radial_gradient_type_2(uv, repeat, data);
}

Color _MMAlgos::radial_gradient_type_3(const Vector2 &uv, const float repeat, const PoolRealArray &data) {
	return MMAlgos::radial_gradient_type_3(uv, repeat, data);
}

Color _MMAlgos::radial_gradient_type_4(const Vector2 &uv, const float repeat, const PoolRealArray &data) {
	return MMAlgos::radial_gradient_type_4(uv, repeat, data);
}

Color _MMAlgos::normal_gradient_type_1(const Vector2 &uv, const float repeat, const float rotate, const PoolRealArray &data) {
	return MMAlgos::normal_gradient_type_1(uv, repeat, rotate, data);
}

Color _MMAlgos::normal_gradient_type_2(const Vector2 &uv, const float repeat, const float rotate, const PoolRealArray &data) {
	return MMAlgos::normal_gradient_type_2(uv, repeat, rotate, data);
}

Color _MMAlgos::normal_gradient_type_3(const Vector2 &uv, const float repeat, const float rotate, const PoolRealArray &data) {
	return MMAlgos::normal_gradient_type_3(uv, repeat, rotate, data);
}

Color _MMAlgos::normal_gradient_type_4(const Vector2 &uv, const float repeat, const float rotate, const PoolRealArray &data) {
	return MMAlgos::normal_gradient_type_4(uv, repeat, rotate, data);
}

Color _MMAlgos::circular_gradient_type_1(const Vector2 &uv, const float repeat, const PoolRealArray &data) {
	return MMAlgos::circular_gradient_type_1(uv, repeat, data);
}

Color _MMAlgos::circular_gradient_type_2(const Vector2 &uv, const float repeat, const PoolRealArray &data) {
	return MMAlgos::circular_gradient_type_2(uv, repeat, data);
}

Color _MMAlgos::circular_gradient_type_3(const Vector2 &uv, const float repeat, const PoolRealArray &data) {
	return MMAlgos::circular_gradient_type_3(uv, repeat, data);
}

Color _MMAlgos::circular_gradient_type_4(const Vector2 &uv, const float repeat, const PoolRealArray &data) {
	return MMAlgos::circular_gradient_type_4(uv, repeat, data);
}

Color _MMAlgos::gradient_type_1(const float x, const PoolRealArray &data) {
	return MMAlgos::gradient_type_1(x, data);
}

Color _MMAlgos::gradient_type_2(const float x, const PoolRealArray &data) {
	return MMAlgos::gradient_type_2(x, data);
}

Color _MMAlgos::gradient_type_3(const float x, const PoolRealArray &data) {
	return MMAlgos::gradient_type_3(x, data);
}

Color _MMAlgos::get_data_color(const int index, const PoolRealArray &data) {
	return MMAlgos::get_data_color(index, data);
}

float _MMAlgos::get_data_pos(const int index, const PoolRealArray &data) {
	return MMAlgos::get_data_pos(index, data);
}

Color _MMAlgos::gradient_type_4(const float x, const PoolRealArray &data) {
	return MMAlgos::gradient_type_4(x, data);
}

Color _MMAlgos::gradient_type_5(const float x, const PoolRealArray &data) {
	return MMAlgos::gradient_type_5(x, data);
}

float _MMAlgos::dots(const Vector2 &uv, const float size, const float density, const float pseed) {
	return MMAlgos::dots(uv, size, density, pseed);
}

Color _MMAlgos::anisotropicc(const Vector2 &uv, const Vector2 &size, const float pseed, const float smoothness, const float interpolation) {
	return MMAlgos::anisotropicc(uv, size, pseed, smoothness, interpolation);
}

float _MMAlgos::anisotropic(const Vector2 &uv, const Vector2 &size, const float pseed, const float smoothness, const float interpolation) {
	return MMAlgos::anisotropic(uv, size, pseed, smoothness, interpolation);
}

Vector3 _MMAlgos::color_dots(const Vector2 &uv, const float size, const float pseed) {
	return MMAlgos::color_dots(uv, size, pseed);
}

Color _MMAlgos::noise_color(const Vector2 &uv, const float size, const float pseed) {
	return MMAlgos::noise_color(uv, size, pseed);
}

Color _MMAlgos::fbmval(const Vector2 &uv, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed) {
	return MMAlgos::fbmval(uv, size, folds, octaves, persistence, pseed);
}

Color _MMAlgos::perlin(const Vector2 &uv, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed) {
	return MMAlgos::perlin(uv, size, folds, octaves, persistence, pseed);
}

Color _MMAlgos::perlinabs(const Vector2 &uv, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed) {
	return MMAlgos::perlinabs(uv, size, folds, octaves, persistence, pseed);
}

Color _MMAlgos::simplex(const Vector2 &uv, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed) {
	return MMAlgos::simplex(uv, size, folds, octaves, persistence, pseed);
}

Color _MMAlgos::cellular(const Vector2 &uv, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed) {
	return MMAlgos::cellular(uv, size, folds, octaves, persistence, pseed);
}

Color _MMAlgos::cellular2(const Vector2 &uv, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed) {
	return MMAlgos::cellular2(uv, size, folds, octaves, persistence, pseed);
}

Color _MMAlgos::cellular3(const Vector2 &uv, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed) {
	return MMAlgos::cellular3(uv, size, folds, octaves, persistence, pseed);
}

Color _MMAlgos::cellular4(const Vector2 &uv, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed) {
	return MMAlgos::cellular4(uv, size, folds, octaves, persistence, pseed);
}

Color _MMAlgos::cellular5(const Vector2 &uv, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed) {
	return MMAlgos::cellular5(uv, size, folds, octaves, persistence, pseed);
}

Color _MMAlgos::cellular6(const Vector2 &uv, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed) {
	return MMAlgos::cellular6(uv, size, folds, octaves, persistence, pseed);
}

float _MMAlgos::fbmf(const Vector2 &coord, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed) {
	return MMAlgos::fbmf(coord, size, folds, octaves, persistence, pseed);
}

float _MMAlgos::perlinf(const Vector2 &coord, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed) {
	return MMAlgos::perlinf(coord, size, folds, octaves, persistence, pseed);
}

float _MMAlgos::perlinabsf(const Vector2 &coord, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed) {
	return MMAlgos::perlinabsf(coord, size, folds, octaves, persistence, pseed);
}

float _MMAlgos::fbm_simplexf(const Vector2 &coord, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed) {
	return MMAlgos::fbm_simplexf(coord, size, folds, octaves, persistence, pseed);
}

float _MMAlgos::cellularf(const Vector2 &coord, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed) {
	return MMAlgos::cellularf(coord, size, folds, octaves, persistence, pseed);
}

float _MMAlgos::cellular2f(const Vector2 &coord, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed) {
	return MMAlgos::cellular2f(coord, size, folds, octaves, persistence, pseed);
}

float _MMAlgos::cellular3f(const Vector2 &coord, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed) {
	return MMAlgos::cellular3f(coord, size, folds, octaves, persistence, pseed);
}

float _MMAlgos::cellular4f(const Vector2 &coord, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed) {
	return MMAlgos::cellular4f(coord, size, folds, octaves, persistence, pseed);
}

float _MMAlgos::cellular5f(const Vector2 &coord, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed) {
	return MMAlgos::cellular5f(coord, size, folds, octaves, persistence, pseed);
}

float _MMAlgos::cellular6f(const Vector2 &coord, const Vector2 &size, const int folds, const int octaves, const float persistence, const float pseed) {
	return MMAlgos::cellular6f(coord, size, folds, octaves, persistence, pseed);
}

float _MMAlgos::fbm_value(const Vector2 &coord, const Vector2 &size, const float pseed) {
	return MMAlgos::fbm_value(coord, size, pseed);
}

float _MMAlgos::fbm_perlin(const Vector2 &coord, const Vector2 &size, const float pseed) {
	return MMAlgos::fbm_perlin(coord, size, pseed);
}

float _MMAlgos::fbm_perlinabs(const Vector2 &coord, const Vector2 &size, const float pseed) {
	return MMAlgos::fbm_perlinabs(coord, size, pseed);
}

Vector2 _MMAlgos::rgrad2(const Vector2 &p, const float rot, const float pseed) {
	return MMAlgos::rgrad2(p, rot, pseed);
}

float _MMAlgos::fbm_simplex(const Vector2 &coord, const Vector2 &size, const float pseed) {
	return MMAlgos::fbm_simplex(coord, size, pseed);
}

float _MMAlgos::fbm_cellular(const Vector2 &coord, const Vector2 &size, const float pseed) {
	return MMAlgos::fbm_cellular(coord, size, pseed);
}

float _MMAlgos::fbm_cellular2(const Vector2 &coord, const Vector2 &size, const float pseed) {
	return MMAlgos::fbm_cellular2(coord, size, pseed);
}

float _MMAlgos::fbm_cellular3(const Vector2 &coord, const Vector2 &size, const float pseed) {
	return MMAlgos::fbm_cellular3(coord, size, pseed);
}

float _MMAlgos::fbm_cellular4(const Vector2 &coord, const Vector2 &size, const float pseed) {
	return MMAlgos::fbm_cellular4(coord, size, pseed);
}

float _MMAlgos::fbm_cellular5(const Vector2 &coord, const Vector2 &size, const float pseed) {
	return MMAlgos::fbm_cellular5(coord, size, pseed);
}

float _MMAlgos::fbm_cellular6(const Vector2 &coord, const Vector2 &size, const float pseed) {
	return MMAlgos::fbm_cellular6(coord, size, pseed);
}

Color _MMAlgos::perlinc(const Vector2 &uv, const Vector2 &size, const int iterations, const float persistence, const int pseed) {
	return MMAlgos::perlinc(uv, size, iterations, persistence, pseed);
}

float _MMAlgos::perlin2c(const Vector2 &uv, const Vector2 &size, const int iterations, const float persistence, const int pseed) {
	return MMAlgos::perlin2c(uv, size, iterations, persistence, pseed);
}

Vector3 _MMAlgos::perlin_color(const Vector2 &uv, const Vector2 &size, const int iterations, const float persistence, const int pseed) {
	return MMAlgos::perlin_color(uv, size, iterations, persistence, pseed);
}

Color _MMAlgos::perlin_colorc(const Vector2 &uv, const Vector2 &size, const int iterations, const float persistence, const int pseed) {
	return MMAlgos::perlin_colorc(uv, size, iterations, persistence, pseed);
}

Color _MMAlgos::perlin_warp_1(const Vector2 &uv, const Vector2 &size, const int iterations, const float persistence, const int pseed, const Vector2 &translate, const float rotate, const Vector2 &size2) {
	return MMAlgos::perlin_warp_1(uv, size, iterations, persistence, pseed, translate, rotate, size2);
}

Color _MMAlgos::perlin_warp_2(const Vector2 &uv, const Vector2 &size, const int iterations, const float persistence, const int pseed, const Vector2 &translate, const float rotate, const Vector2 &size2) {
	return MMAlgos::perlin_warp_2(uv, size, iterations, persistence, pseed, translate, rotate, size2);
}

Color _MMAlgos::voronoi(const Vector2 &uv, const Vector2 &size, const Vector2 &stretch, const float intensity, const float randomness, const int pseed) {
	return MMAlgos::voronoi(uv, size, stretch, intensity, randomness, pseed);
}

Color _MMAlgos::voronoi_1(const Vector2 &uv, const Vector2 &size, const Vector2 &stretch, const float intensity, const float randomness, const int pseed) {
	return MMAlgos::voronoi_1(uv, size, stretch, intensity, randomness, pseed);
}

Color _MMAlgos::voronoi_2(const Vector2 &uv, const Vector2 &size, const Vector2 &stretch, const float intensity, const float randomness, const int pseed) {
	return MMAlgos::voronoi_2(uv, size, stretch, intensity, randomness, pseed);
}

Color _MMAlgos::voronoi_3(const Vector2 &uv, const Vector2 &size, const Vector2 &stretch, const float intensity, const float randomness, const int pseed) {
	return MMAlgos::voronoi_3(uv, size, stretch, intensity, randomness, pseed);
}

float _MMAlgos::pattern(const Vector2 &uv, const float x_scale, const float y_scale, const int ct, const int catx, const int caty) {
	return MMAlgos::pattern(uv, x_scale, y_scale, ct, catx, caty);
}

Color _MMAlgos::truchet1c(const Vector2 &uv, const float size, const float pseed) {
	return MMAlgos::truchet1c(uv, size, pseed);
}

float _MMAlgos::truchet1(const Vector2 &uv, const Vector2 &pseed) {
	return MMAlgos::truchet1(uv, pseed);
}

Color _MMAlgos::truchet2c(const Vector2 &uv, const float size, const float pseed) {
	return MMAlgos::truchet2c(uv, size, pseed);
}

float _MMAlgos::truchet2(const Vector2 &uv, const Vector2 &pseed) {
	return MMAlgos::truchet2(uv, pseed);
}

Color _MMAlgos::weavec(const Vector2 &uv, const Vector2 &count, const float width) {
	return MMAlgos::weavec(uv, count, width);
}

float _MMAlgos::weave(const Vector2 &uv, const Vector2 &count, const float width) {
	return MMAlgos::weave(uv, count, width);
}

Vector3 _MMAlgos::weave2(const Vector2 &uv, const Vector2 &count, const float stitch, const float width_x, const float width_y) {
	return MMAlgos::weave2(uv, count, stitch, width_x, width_y);
}

Color _MMAlgos::sinewavec(const Vector2 &uv, const float amplitude, const float frequency, const float phase) {
	return MMAlgos::sinewavec(uv, amplitude, frequency, phase);
}

float _MMAlgos::sinewavef(const Vector2 &uv, const float amplitude, const float frequency, const float phase) {
	return MMAlgos::sinewavef(uv, amplitude, frequency, phase);
}

float _MMAlgos::scratch(const Vector2 &uv, const Vector2 &size, const float waviness, const float angle, const float randomness, const Vector2 &pseed) {
	return MMAlgos::scratch(uv, size, waviness, angle, randomness, pseed);
}

float _MMAlgos::scratches(const Vector2 &uv, const int layers, const Vector2 &size, const float waviness, const float angle, const float randomness, const Vector2 &pseed) {
	return MMAlgos::scratches(uv, layers, size, waviness, angle, randomness, pseed);
}

Color _MMAlgos::scratchesc(const Vector2 &uv, const int layers, const Vector2 &size, const float waviness, const float angle, const float randomness, const Vector2 &pseed) {
	return MMAlgos::scratchesc(uv, layers, size, waviness, angle, randomness, pseed);
}

Color _MMAlgos::runesc(const Vector2 &uv, const Vector2 &col_row, const float pseed) {
	return MMAlgos::runesc(uv, col_row, pseed);
}

float _MMAlgos::runesf(const Vector2 &uv, const Vector2 &col_row, const float pseed) {
	return MMAlgos::runesf(uv, col_row, pseed);
}

float _MMAlgos::rune(const Vector2 &uv, const float pseed) {
	return MMAlgos::rune(uv, pseed);
}

Color _MMAlgos::IChingc(const Vector2 &uv, const Vector2 &row_col, const int pseed) {
	return MMAlgos::IChingc(uv, row_col, pseed);
}

float _MMAlgos::IChing(const Vector2 &uv, const float pseed) {
	return MMAlgos::IChing(uv, pseed);
}

Color _MMAlgos::beehive_1c(const Vector2 &uv, const Vector2 &size, const int pseed) {
	return MMAlgos::beehive_1c(uv, size, pseed);
}

Color _MMAlgos::beehive_2c(const Vector2 &uv, const Vector2 &size, const int pseed) {
	return MMAlgos::beehive_2c(uv, size, pseed);
}

Color _MMAlgos::beehive_3c(const Vector2 &uv, const Vector2 &size, const int pseed) {
	return MMAlgos::beehive_3c(uv, size, pseed);
}

float _MMAlgos::beehive_dist(const Vector2 &p) {
	return MMAlgos::beehive_dist(p);
}

Color _MMAlgos::beehive_center(const Vector2 &p) {
	return MMAlgos::beehive_center(p);
}

Vector3 _MMAlgos::brick_corner_uv(const Vector2 &uv, const Vector2 &bmin, const Vector2 &bmax, const float mortar, const float corner, const float pseed) {
	return MMAlgos::brick_corner_uv(uv, bmin, bmax, mortar, corner, pseed);
}

Color _MMAlgos::brick(const Vector2 &uv, const Vector2 &bmin, const Vector2 &bmax, const float mortar, const float pround, const float bevel) {
	return MMAlgos::brick(uv, bmin, bmax, mortar, pround, bevel);
}

Vector3 _MMAlgos::brick_random_color(const Vector2 &bmin, const Vector2 &bmax, const float pseed) {
	return MMAlgos::brick_random_color(bmin, bmax, pseed);
}

Vector3 _MMAlgos::brick_uv(const Vector2 &uv, const Vector2 &bmin, const Vector2 &bmax, const float pseed) {
	return MMAlgos::brick_uv(uv, bmin, bmax, pseed);
}

Color _MMAlgos::bricks_rb(const Vector2 &uv, const Vector2 &count, const float repeat, const float offset) {
	return MMAlgos::bricks_rb(uv, count, repeat, offset);
}

Color _MMAlgos::bricks_rb2(const Vector2 &uv, const Vector2 &count, const float repeat, const float offset) {
	return MMAlgos::bricks_rb2(uv, count, repeat, offset);
}

Color _MMAlgos::bricks_hb(const Vector2 &uv, const Vector2 &count, const float repeat, const float offset) {
	return MMAlgos::bricks_hb(uv, count, repeat, offset);
}

Color _MMAlgos::bricks_bw(const Vector2 &uv, const Vector2 &count, const float repeat, const float offset) {
	return MMAlgos::bricks_bw(uv, count, repeat, offset);
}

Color _MMAlgos::bricks_sb(const Vector2 &uv, const Vector2 &count, const float repeat, const float offset) {
	return MMAlgos::bricks_sb(uv, count, repeat, offset);
}

Color _MMAlgos::brick2(const Vector2 &uv, const Vector2 &bmin, const Vector2 &bmax, const float mortar, const float pround, const float bevel) {
	return MMAlgos::brick2(uv, bmin, bmax, mortar, pround, bevel);
}

Color _MMAlgos::bricks_uneven(const Vector2 &uv, const int iterations, const float min_size, const float randomness, const float pseed) {
	return MMAlgos::bricks_uneven(uv, iterations, min_size, randomness, pseed);
}

Vector2 _MMAlgos::truchet_generic_uv(const Vector2 &uv, const float pseed) {
	return MMAlgos::truchet_generic_uv(uv, pseed);
}

Color _MMAlgos::sdf_show(const float val, const float bevel) {
	return MMAlgos::sdf_show(val, bevel);
}

float _MMAlgos::sdf_circle(const Vector2 &uv, const Vector2 &c, const float r) {
	return MMAlgos::sdf_circle(uv, c, r);
}

float _MMAlgos::sdf_box(const Vector2 &uv, const Vector2 &c, const Vector2 &wh) {
	return MMAlgos::sdf_box(uv, c, wh);
}

Vector2 _MMAlgos::sdf_line(const Vector2 &uv, const Vector2 &a, const Vector2 &b, const float r) {
	return MMAlgos::sdf_line(uv, a, b, r);
}

float _MMAlgos::sdf_rhombus(const Vector2 &uv, const Vector2 &c, const Vector2 &wh) {
	return MMAlgos::sdf_rhombus(uv, c, wh);
}

float _MMAlgos::sdf_arc(const Vector2 &uv, const Vector2 &a, const Vector2 &r) {
	return MMAlgos::sdf_arc(uv, a, r);
}

float _MMAlgos::sdr_ndot(const Vector2 &a, const Vector2 &b) {
	return MMAlgos::sdr_ndot(a, b);
}

float _MMAlgos::sdRhombus(const Vector2 &p, const Vector2 &b) {
	return MMAlgos::sdRhombus(p, b);
}

float _MMAlgos::sdArc(const Vector2 &p, const float a1, const float a2, const float ra, const float rb) {
	return MMAlgos::sdArc(p, a1, a2, ra, rb);
}

float _MMAlgos::sdf_boolean_union(const float a, const float b) {
	return MMAlgos::sdf_boolean_union(a, b);
}

float _MMAlgos::sdf_boolean_substraction(const float a, const float b) {
	return MMAlgos::sdf_boolean_substraction(a, b);
}

float _MMAlgos::sdf_boolean_intersection(const float a, const float b) {
	return MMAlgos::sdf_boolean_intersection(a, b);
}

float _MMAlgos::sdf_smooth_boolean_union(const float d1, const float d2, const float k) {
	return MMAlgos::sdf_smooth_boolean_union(d1, d2, k);
}

float _MMAlgos::sdf_smooth_boolean_substraction(const float d1, const float d2, const float k) {
	return MMAlgos::sdf_smooth_boolean_substraction(d1, d2, k);
}

float _MMAlgos::sdf_smooth_boolean_intersection(const float d1, const float d2, const float k) {
	return MMAlgos::sdf_smooth_boolean_intersection(d1, d2, k);
}

float _MMAlgos::sdf_rounded_shape(const float a, const float r) {
	return MMAlgos::sdf_rounded_shape(a, r);
}

float _MMAlgos::sdf_annular_shape(const float a, const float r) {
	return MMAlgos::sdf_annular_shape(a, r);
}

float _MMAlgos::sdf_morph(const float a, const float b, const float amount) {
	return MMAlgos::sdf_morph(a, b, amount);
}

Vector2 _MMAlgos::sdLine(const Vector2 &p, const Vector2 &a, const Vector2 &b) {
	return MMAlgos::sdLine(p, a, b);
}

Vector2 _MMAlgos::sdf2d_rotate(const Vector2 &uv, const float a) {
	return MMAlgos::sdf2d_rotate(uv, a);
}

Vector2 _MMAlgos::sdBezier(const Vector2 &pos, const Vector2 &A, const Vector2 &B, const Vector2 &C) {
	return MMAlgos::sdBezier(pos, A, B, C);
}

Vector2 _MMAlgos::circle_repeat_transform_2d(const Vector2 &p, const float count) {
	return MMAlgos::circle_repeat_transform_2d(p, count);
}

Vector2 _MMAlgos::sdNgon(const Vector2 &pos, const float r, const float n) {
	return MMAlgos::sdNgon(pos, r, n);
}

Vector2 _MMAlgos::repeat_2d(const Vector2 &p, const Vector2 &r, const float pseed, const float randomness) {
	return MMAlgos::repeat_2d(p, r, pseed, randomness);
}

float _MMAlgos::sdSmoothUnion(const float d1, const float d2, const float k) {
	return MMAlgos::sdSmoothUnion(d1, d2, k);
}

float _MMAlgos::sdSmoothSubtraction(const float d1, const float d2, const float k) {
	return MMAlgos::sdSmoothSubtraction(d1, d2, k);
}

float _MMAlgos::sdSmoothIntersection(const float d1, const float d2, const float k) {
	return MMAlgos::sdSmoothIntersection(d1, d2, k);
}

float _MMAlgos::sdRipples(const float d, const float w, const int r) {
	return MMAlgos::sdRipples(d, w, r);
}

float _MMAlgos::sdPolygon(const Vector2 &p, const PoolVector2Array &v) {
	return MMAlgos::sdPolygon(p, v);
}

Color _MMAlgos::raymarch(const Vector2 &uv) {
	return MMAlgos::raymarch(uv);
}

Color _MMAlgos::raymarch2(const Vector2 &uv) {
	return MMAlgos::raymarch2(uv);
}

Color _MMAlgos::raymarch3(const Vector2 &uv) {
	return MMAlgos::raymarch3(uv);
}

Vector2 _MMAlgos::sdf3d_sphere(const Vector3 &p, const float r) {
	return MMAlgos::sdf3d_sphere(p, r);
}

Vector2 _MMAlgos::sdf3d_box(const Vector3 &p, const float sx, const float sy, const float sz, const float r) {
	return MMAlgos::sdf3d_box(p, sx, sy, sz, r);
}

Vector2 _MMAlgos::sdf3d_cylinder_y(const Vector3 &p, const float r, const float l) {
	return MMAlgos::sdf3d_cylinder_y(p, r, l);
}

Vector2 _MMAlgos::sdf3d_cylinder_x(const Vector3 &p, const float r, const float l) {
	return MMAlgos::sdf3d_cylinder_x(p, r, l);
}

Vector2 _MMAlgos::sdf3d_cylinder_z(const Vector3 &p, const float r, const float l) {
	return MMAlgos::sdf3d_cylinder_z(p, r, l);
}

Vector2 _MMAlgos::sdf3d_capsule_y(const Vector3 &p, const float r, const float l) {
	return MMAlgos::sdf3d_capsule_y(p, r, l);
}

Vector2 _MMAlgos::sdf3d_capsule_x(const Vector3 &p, const float r, const float l) {
	return MMAlgos::sdf3d_capsule_x(p, r, l);
}

Vector2 _MMAlgos::sdf3d_capsule_z(const Vector3 &p, const float r, const float l) {
	return MMAlgos::sdf3d_capsule_z(p, r, l);
}

Vector2 _MMAlgos::sdf3d_cone_px(const Vector3 &p, const float a) {
	return MMAlgos::sdf3d_cone_px(p, a);
}

Vector2 _MMAlgos::sdf3d_cone_nx(const Vector3 &p, const float a) {
	return MMAlgos::sdf3d_cone_nx(p, a);
}

Vector2 _MMAlgos::sdf3d_cone_py(const Vector3 &p, const float a) {
	return MMAlgos::sdf3d_cone_py(p, a);
}

Vector2 _MMAlgos::sdf3d_cone_ny(const Vector3 &p, const float a) {
	return MMAlgos::sdf3d_cone_ny(p, a);
}

Vector2 _MMAlgos::sdf3d_cone_pz(const Vector3 &p, const float a) {
	return MMAlgos::sdf3d_cone_pz(p, a);
}

Vector2 _MMAlgos::sdf3d_cone_nz(const Vector3 &p, const float a) {
	return MMAlgos::sdf3d_cone_nz(p, a);
}

Vector2 _MMAlgos::sdf3d_torus_x(const Vector3 &p, const float R, const float r) {
	return MMAlgos::sdf3d_torus_x(p, R, r);
}

Vector2 _MMAlgos::sdf3d_torus_y(const Vector3 &p, const float R, const float r) {
	return MMAlgos::sdf3d_torus_y(p, R, r);
}

Vector2 _MMAlgos::sdf3d_torus_z(const Vector3 &p, const float R, const float r) {
	return MMAlgos::sdf3d_torus_z(p, R, r);
}

Vector2 _MMAlgos::sdf3d_raymarch(const Vector2 &uv) {
	return MMAlgos::sdf3d_raymarch(uv);
}

Vector3 _MMAlgos::sdf3d_normal(const Vector3 &p) {
	return MMAlgos::sdf3d_normal(p);
}

Vector2 _MMAlgos::sdf3dc_union(const Vector2 &a, const Vector2 &b) {
	return MMAlgos::sdf3dc_union(a, b);
}

Vector2 _MMAlgos::sdf3dc_sub(const Vector2 &a, const Vector2 &b) {
	return MMAlgos::sdf3dc_sub(a, b);
}

Vector2 _MMAlgos::sdf3dc_inter(const Vector2 &a, const Vector2 &b) {
	return MMAlgos::sdf3dc_inter(a, b);
}

Vector2 _MMAlgos::sdf3d_smooth_union(const Vector2 &d1, const Vector2 &d2, const float k) {
	return MMAlgos::sdf3d_smooth_union(d1, d2, k);
}

Vector2 _MMAlgos::sdf3d_smooth_subtraction(const Vector2 &d1, const Vector2 &d2, const float k) {
	return MMAlgos::sdf3d_smooth_subtraction(d1, d2, k);
}

Vector2 _MMAlgos::sdf3d_smooth_intersection(const Vector2 &d1, const Vector2 &d2, const float k) {
	return MMAlgos::sdf3d_smooth_intersection(d1, d2, k);
}

Vector2 _MMAlgos::sdf3d_rounded(const Vector2 &v, const float r) {
	return MMAlgos::sdf3d_rounded(v, r);
}

Vector3 _MMAlgos::sdf3d_elongation(const Vector3 &p, const Vector3 &v) {
	return MMAlgos::sdf3d_elongation(p, v);
}

Vector3 _MMAlgos::sdf3d_repeat(const Vector3 &p, const Vector2 &r, const float randomness, const int pseed) {
	return MMAlgos::sdf3d_repeat(p, r, randomness, pseed);
}

Vector3 _MMAlgos::repeat(const Vector3 &p, const Vector3 &r, const float pseed, const float randomness) {
	return MMAlgos::repeat(p, r, pseed, randomness);
}

Vector3 _MMAlgos::rotate3d(const Vector3 &p, const Vector3 &a) {
	return MMAlgos::rotate3d(p, a);
}

Vector3 _MMAlgos::circle_repeat_transform(const Vector3 &p, const float count) {
	return MMAlgos::circle_repeat_transform(p, count);
}

Vector2 _MMAlgos::sdf3d_input(const Vector3 &p) {
	return MMAlgos::sdf3d_input(p);
}

float _MMAlgos::sphere(const Vector2 &uv, const Vector2 &c, const float r) {
	return MMAlgos::sphere(uv, c, r);
}

float _MMAlgos::shape_circle(const Vector2 &uv, const float sides, const float size, const float edge) {
	return MMAlgos::shape_circle(uv, sides, size, edge);
}

float _MMAlgos::shape_polygon(const Vector2 &uv, const float sides, const float size, const float edge) {
	return MMAlgos::shape_polygon(uv, sides, size, edge);
}

float _MMAlgos::shape_star(const Vector2 &uv, const float sides, const float size, const float edge) {
	return MMAlgos::shape_star(uv, sides, size, edge);
}

float _MMAlgos::shape_curved_star(const Vector2 &uv, const float sides, const float size, const float edge) {
	return MMAlgos::shape_curved_star(uv, sides, size, edge);
}

float _MMAlgos::shape_rays(const Vector2 &uv, const float sides, const float size, const float edge) {
	return MMAlgos::shape_rays(uv, sides, size, edge);
}

Vector2 _MMAlgos::transform2_clamp(const Vector2 &uv) {
	return MMAlgos::transform2_clamp(uv);
}

Vector2 _MMAlgos::transform2(const Vector2 &uv, const Vector2 &translate, const float rotate, const Vector2 &scale) {
	return MMAlgos::transform2(uv, translate, rotate, scale);
}

Vector2 _MMAlgos::rotate(const Vector2 &uv, const Vector2 &center, const float rotate) {
	return MMAlgos::rotate(uv, center, rotate);
}

Vector2 _MMAlgos::scale(const Vector2 &uv, const Vector2 &center, const Vector2 &scale) {
	return MMAlgos::scale(uv, center, scale);
}

Vector2 _MMAlgos::uvmirror_h(const Vector2 &uv, const float offset) {
	return MMAlgos::uvmirror_h(uv, offset);
}

Vector2 _MMAlgos::uvmirror_v(const Vector2 &uv, const float offset) {
	return MMAlgos::uvmirror_v(uv, offset);
}

Vector2 _MMAlgos::kal_rotate(const Vector2 &uv, const float count, const float offset) {
	return MMAlgos::kal_rotate(uv, count, offset);
}

Vector2 _MMAlgos::get_from_tileset(const float count, const float pseed, const Vector2 &uv) {
	return MMAlgos::get_from_tileset(count, pseed, uv);
}

Vector2 _MMAlgos::custom_uv_transform(const Vector2 &uv, const Vector2 &cst_scale, const float rnd_rotate, const float rnd_scale, const Vector2 &pseed) {
	return MMAlgos::custom_uv_transform(uv, cst_scale, rnd_rotate, rnd_scale, pseed);
}

void _MMAlgos::register_node_class(const String &category, const String &cls) {
	MMAlgos::register_node_class(category, cls);
}
void _MMAlgos::unregister_node_class(const String &category, const String &cls) {
	MMAlgos::unregister_node_class(category, cls);
}

void _MMAlgos::register_node_script(const String &category, const String &file_path) {
	MMAlgos::register_node_script(category, file_path);
}
void _MMAlgos::unregister_node_script(const String &category, const String &file_path) {
	MMAlgos::unregister_node_script(category, file_path);
}

_MMAlgos *_MMAlgos::get_singleton() {
	return self;
}

_MMAlgos::_MMAlgos() {
	self = this;
}

_MMAlgos::~_MMAlgos() {
	self = nullptr;
}

void _MMAlgos::_bind_methods() {
	ClassDB::bind_method(D_METHOD("clampv3", "v", "mi", "ma"), &_MMAlgos::clampv3);
	ClassDB::bind_method(D_METHOD("floorc", "a"), &_MMAlgos::floorc);
	ClassDB::bind_method(D_METHOD("floorv2", "a"), &_MMAlgos::floorv2);
	ClassDB::bind_method(D_METHOD("floorv3", "a"), &_MMAlgos::floorv3);
	ClassDB::bind_method(D_METHOD("smoothstepv2", "a", "b", "c"), &_MMAlgos::smoothstepv2);
	ClassDB::bind_method(D_METHOD("maxv2", "a", "b"), &_MMAlgos::maxv2);
	ClassDB::bind_method(D_METHOD("maxv3", "a", "b"), &_MMAlgos::maxv3);
	ClassDB::bind_method(D_METHOD("absv2", "v"), &_MMAlgos::absv2);
	ClassDB::bind_method(D_METHOD("absv3", "v"), &_MMAlgos::absv3);
	ClassDB::bind_method(D_METHOD("cosv2", "v"), &_MMAlgos::cosv2);
	ClassDB::bind_method(D_METHOD("cosv3", "v"), &_MMAlgos::cosv3);
	ClassDB::bind_method(D_METHOD("powv2", "x", "y"), &_MMAlgos::powv2);
	ClassDB::bind_method(D_METHOD("modv3", "a", "b"), &_MMAlgos::modv3);
	ClassDB::bind_method(D_METHOD("modv2", "a", "b"), &_MMAlgos::modv2);
	ClassDB::bind_method(D_METHOD("modf", "x", "y"), &_MMAlgos::modf);
	ClassDB::bind_method(D_METHOD("fractv2", "v"), &_MMAlgos::fractv2);
	ClassDB::bind_method(D_METHOD("fractv3", "v"), &_MMAlgos::fractv3);
	ClassDB::bind_method(D_METHOD("fract", "f"), &_MMAlgos::fract);
	ClassDB::bind_method(D_METHOD("clampv2", "v", "pmin", "pmax"), &_MMAlgos::clampv2);
	ClassDB::bind_method(D_METHOD("minv2", "v1", "v2"), &_MMAlgos::minv2);
	ClassDB::bind_method(D_METHOD("minv3", "v1", "v2"), &_MMAlgos::minv3);
	ClassDB::bind_method(D_METHOD("rand", "x"), &_MMAlgos::rand);
	ClassDB::bind_method(D_METHOD("rand2", "x"), &_MMAlgos::rand2);
	ClassDB::bind_method(D_METHOD("rand3", "x"), &_MMAlgos::rand3);
	ClassDB::bind_method(D_METHOD("step", "edge", "x"), &_MMAlgos::step);
	ClassDB::bind_method(D_METHOD("stepv2", "edge", "x"), &_MMAlgos::stepv2);
	ClassDB::bind_method(D_METHOD("signv2", "x"), &_MMAlgos::signv2);
	ClassDB::bind_method(D_METHOD("transform", "uv", "translate", "rotate", "scale", "repeat"), &_MMAlgos::transform);
	ClassDB::bind_method(D_METHOD("fractf", "x"), &_MMAlgos::fractf);
	ClassDB::bind_method(D_METHOD("mix_mul", "x", "y"), &_MMAlgos::mix_mul);
	ClassDB::bind_method(D_METHOD("mix_add", "x", "y"), &_MMAlgos::mix_add);
	ClassDB::bind_method(D_METHOD("mix_max", "x", "y"), &_MMAlgos::mix_max);
	ClassDB::bind_method(D_METHOD("mix_min", "x", "y"), &_MMAlgos::mix_min);
	ClassDB::bind_method(D_METHOD("mix_xor", "x", "y"), &_MMAlgos::mix_xor);
	ClassDB::bind_method(D_METHOD("mix_pow", "x", "y"), &_MMAlgos::mix_pow);
	ClassDB::bind_method(D_METHOD("wave_constant", "x"), &_MMAlgos::wave_constant);
	ClassDB::bind_method(D_METHOD("wave_sine", "x"), &_MMAlgos::wave_sine);
	ClassDB::bind_method(D_METHOD("wave_triangle", "x"), &_MMAlgos::wave_triangle);
	ClassDB::bind_method(D_METHOD("wave_sawtooth", "x"), &_MMAlgos::wave_sawtooth);
	ClassDB::bind_method(D_METHOD("wave_square", "x"), &_MMAlgos::wave_square);
	ClassDB::bind_method(D_METHOD("wave_bounce", "x"), &_MMAlgos::wave_bounce);
	ClassDB::bind_method(D_METHOD("sinewave", "uv", "amplitude", "frequency", "phase"), &_MMAlgos::sinewave);
	ClassDB::bind_method(D_METHOD("ThickLine", "uv", "posA", "posB", "radiusInv"), &_MMAlgos::ThickLine);
	ClassDB::bind_method(D_METHOD("curve", "x", "points"), &_MMAlgos::curve);
	ClassDB::bind_method(D_METHOD("flood_fill_preprocess", "uv", "c", "s"), &_MMAlgos::flood_fill_preprocess);
	ClassDB::bind_method(D_METHOD("fill_to_uv_stretch", "coord", "bb", "pseed"), &_MMAlgos::fill_to_uv_stretch);
	ClassDB::bind_method(D_METHOD("fill_to_uv_square", "coord", "bb", "pseed"), &_MMAlgos::fill_to_uv_square);
	ClassDB::bind_method(D_METHOD("rgb_to_hsv", "c"), &_MMAlgos::rgb_to_hsv);
	ClassDB::bind_method(D_METHOD("hsv_to_rgb", "c"), &_MMAlgos::hsv_to_rgb);
	ClassDB::bind_method(D_METHOD("adjust_hsv", "color", "hue", "saturation", "value"), &_MMAlgos::adjust_hsv);
	ClassDB::bind_method(D_METHOD("brightness_contrast", "color", "brightness", "contrast"), &_MMAlgos::brightness_contrast);
	ClassDB::bind_method(D_METHOD("grayscale_min", "c"), &_MMAlgos::grayscale_min);
	ClassDB::bind_method(D_METHOD("grayscale_max", "c"), &_MMAlgos::grayscale_max);
	ClassDB::bind_method(D_METHOD("grayscale_lightness", "c"), &_MMAlgos::grayscale_lightness);
	ClassDB::bind_method(D_METHOD("grayscale_average", "c"), &_MMAlgos::grayscale_average);
	ClassDB::bind_method(D_METHOD("grayscale_luminosity", "c"), &_MMAlgos::grayscale_luminosity);
	ClassDB::bind_method(D_METHOD("invert", "color"), &_MMAlgos::invert);
	ClassDB::bind_method(D_METHOD("blend_normal", "uv", "c1", "c2", "opacity"), &_MMAlgos::blend_normal);
	ClassDB::bind_method(D_METHOD("blend_dissolve", "uv", "c1", "c2", "opacity"), &_MMAlgos::blend_dissolve);
	ClassDB::bind_method(D_METHOD("blend_multiply", "uv", "c1", "c2", "opacity"), &_MMAlgos::blend_multiply);
	ClassDB::bind_method(D_METHOD("blend_screen", "uv", "c1", "c2", "opacity"), &_MMAlgos::blend_screen);
	ClassDB::bind_method(D_METHOD("blend_overlay_f", "c1", "c2"), &_MMAlgos::blend_overlay_f);
	ClassDB::bind_method(D_METHOD("blend_overlay", "uv", "c1", "c2", "opacity"), &_MMAlgos::blend_overlay);
	ClassDB::bind_method(D_METHOD("blend_hard_light", "uv", "c1", "c2", "opacity"), &_MMAlgos::blend_hard_light);
	ClassDB::bind_method(D_METHOD("blend_soft_light_f", "c1", "c2"), &_MMAlgos::blend_soft_light_f);
	ClassDB::bind_method(D_METHOD("blend_soft_light", "uv", "c1", "c2", "opacity"), &_MMAlgos::blend_soft_light);
	ClassDB::bind_method(D_METHOD("blend_burn_f", "c1", "c2"), &_MMAlgos::blend_burn_f);
	ClassDB::bind_method(D_METHOD("blend_burn", "uv", "c1", "c2", "opacity"), &_MMAlgos::blend_burn);
	ClassDB::bind_method(D_METHOD("blend_dodge_f", "c1", "c2"), &_MMAlgos::blend_dodge_f);
	ClassDB::bind_method(D_METHOD("blend_dodge", "uv", "c1", "c2", "opacity"), &_MMAlgos::blend_dodge);
	ClassDB::bind_method(D_METHOD("blend_lighten", "uv", "c1", "c2", "opacity"), &_MMAlgos::blend_lighten);
	ClassDB::bind_method(D_METHOD("blend_darken", "uv", "c1", "c2", "opacity"), &_MMAlgos::blend_darken);
	ClassDB::bind_method(D_METHOD("blend_difference", "uv", "c1", "c2", "opacity"), &_MMAlgos::blend_difference);
	ClassDB::bind_method(D_METHOD("radial_gradient_type_1", "uv", "repeat", "data"), &_MMAlgos::radial_gradient_type_1);
	ClassDB::bind_method(D_METHOD("radial_gradient_type_2", "uv", "repeat", "data"), &_MMAlgos::radial_gradient_type_2);
	ClassDB::bind_method(D_METHOD("radial_gradient_type_3", "uv", "repeat", "data"), &_MMAlgos::radial_gradient_type_3);
	ClassDB::bind_method(D_METHOD("radial_gradient_type_4", "uv", "repeat", "data"), &_MMAlgos::radial_gradient_type_4);
	ClassDB::bind_method(D_METHOD("normal_gradient_type_1", "uv", "repeat", "rotate", "data"), &_MMAlgos::normal_gradient_type_1);
	ClassDB::bind_method(D_METHOD("normal_gradient_type_2", "uv", "repeat", "rotate", "data"), &_MMAlgos::normal_gradient_type_2);
	ClassDB::bind_method(D_METHOD("normal_gradient_type_3", "uv", "repeat", "rotate", "data"), &_MMAlgos::normal_gradient_type_3);
	ClassDB::bind_method(D_METHOD("normal_gradient_type_4", "uv", "repeat", "rotate", "data"), &_MMAlgos::normal_gradient_type_4);
	ClassDB::bind_method(D_METHOD("circular_gradient_type_1", "uv", "repeat", "data"), &_MMAlgos::circular_gradient_type_1);
	ClassDB::bind_method(D_METHOD("circular_gradient_type_2", "uv", "repeat", "data"), &_MMAlgos::circular_gradient_type_2);
	ClassDB::bind_method(D_METHOD("circular_gradient_type_3", "uv", "repeat", "data"), &_MMAlgos::circular_gradient_type_3);
	ClassDB::bind_method(D_METHOD("circular_gradient_type_4", "uv", "repeat", "data"), &_MMAlgos::circular_gradient_type_4);
	ClassDB::bind_method(D_METHOD("gradient_type_1", "x", "data"), &_MMAlgos::gradient_type_1);
	ClassDB::bind_method(D_METHOD("gradient_type_2", "x", "data"), &_MMAlgos::gradient_type_2);
	ClassDB::bind_method(D_METHOD("gradient_type_3", "x", "data"), &_MMAlgos::gradient_type_3);
	ClassDB::bind_method(D_METHOD("get_data_color", "index", "data"), &_MMAlgos::get_data_color);
	ClassDB::bind_method(D_METHOD("get_data_pos", "index", "data"), &_MMAlgos::get_data_pos);
	ClassDB::bind_method(D_METHOD("gradient_type_4", "x", "data"), &_MMAlgos::gradient_type_4);
	ClassDB::bind_method(D_METHOD("gradient_type_5", "x", "data"), &_MMAlgos::gradient_type_5);
	ClassDB::bind_method(D_METHOD("dots", "uv", "size", "density", "pseed"), &_MMAlgos::dots);
	ClassDB::bind_method(D_METHOD("anisotropicc", "uv", "size", "pseed", "smoothness", "interpolation"), &_MMAlgos::anisotropicc);
	ClassDB::bind_method(D_METHOD("anisotropic", "uv", "size", "pseed", "smoothness", "interpolation"), &_MMAlgos::anisotropic);
	ClassDB::bind_method(D_METHOD("color_dots", "uv", "size", "pseed"), &_MMAlgos::color_dots);
	ClassDB::bind_method(D_METHOD("noise_color", "uv", "size", "pseed"), &_MMAlgos::noise_color);
	ClassDB::bind_method(D_METHOD("fbmval", "uv", "size", "folds", "octaves", "persistence", "pseed"), &_MMAlgos::fbmval);
	ClassDB::bind_method(D_METHOD("perlin", "uv", "size", "folds", "octaves", "persistence", "pseed"), &_MMAlgos::perlin);
	ClassDB::bind_method(D_METHOD("perlinabs", "uv", "size", "folds", "octaves", "persistence", "pseed"), &_MMAlgos::perlinabs);
	ClassDB::bind_method(D_METHOD("simplex", "uv", "size", "folds", "octaves", "persistence", "pseed"), &_MMAlgos::simplex);
	ClassDB::bind_method(D_METHOD("cellular", "uv", "size", "folds", "octaves", "persistence", "pseed"), &_MMAlgos::cellular);
	ClassDB::bind_method(D_METHOD("cellular2", "uv", "size", "folds", "octaves", "persistence", "pseed"), &_MMAlgos::cellular2);
	ClassDB::bind_method(D_METHOD("cellular3", "uv", "size", "folds", "octaves", "persistence", "pseed"), &_MMAlgos::cellular3);
	ClassDB::bind_method(D_METHOD("cellular4", "uv", "size", "folds", "octaves", "persistence", "pseed"), &_MMAlgos::cellular4);
	ClassDB::bind_method(D_METHOD("cellular5", "uv", "size", "folds", "octaves", "persistence", "pseed"), &_MMAlgos::cellular5);
	ClassDB::bind_method(D_METHOD("cellular6", "uv", "size", "folds", "octaves", "persistence", "pseed"), &_MMAlgos::cellular6);
	ClassDB::bind_method(D_METHOD("fbmf", "coord", "size", "folds", "octaves", "persistence", "pseed"), &_MMAlgos::fbmf);
	ClassDB::bind_method(D_METHOD("perlinf", "coord", "size", "folds", "octaves", "persistence", "pseed"), &_MMAlgos::perlinf);
	ClassDB::bind_method(D_METHOD("perlinabsf", "coord", "size", "folds", "octaves", "persistence", "pseed"), &_MMAlgos::perlinabsf);
	ClassDB::bind_method(D_METHOD("fbm_simplexf", "coord", "size", "folds", "octaves", "persistence", "pseed"), &_MMAlgos::fbm_simplexf);
	ClassDB::bind_method(D_METHOD("cellularf", "coord", "size", "folds", "octaves", "persistence", "pseed"), &_MMAlgos::cellularf);
	ClassDB::bind_method(D_METHOD("cellular2f", "coord", "size", "folds", "octaves", "persistence", "pseed"), &_MMAlgos::cellular2f);
	ClassDB::bind_method(D_METHOD("cellular3f", "coord", "size", "folds", "octaves", "persistence", "pseed"), &_MMAlgos::cellular3f);
	ClassDB::bind_method(D_METHOD("cellular4f", "coord", "size", "folds", "octaves", "persistence", "pseed"), &_MMAlgos::cellular4f);
	ClassDB::bind_method(D_METHOD("cellular5f", "coord", "size", "folds", "octaves", "persistence", "pseed"), &_MMAlgos::cellular5f);
	ClassDB::bind_method(D_METHOD("cellular6f", "coord", "size", "folds", "octaves", "persistence", "pseed"), &_MMAlgos::cellular6f);
	ClassDB::bind_method(D_METHOD("fbm_value", "coord", "size", "pseed"), &_MMAlgos::fbm_value);
	ClassDB::bind_method(D_METHOD("fbm_perlin", "coord", "size", "pseed"), &_MMAlgos::fbm_perlin);
	ClassDB::bind_method(D_METHOD("fbm_perlinabs", "coord", "size", "pseed"), &_MMAlgos::fbm_perlinabs);
	ClassDB::bind_method(D_METHOD("rgrad2", "p", "rot", "pseed"), &_MMAlgos::rgrad2);
	ClassDB::bind_method(D_METHOD("fbm_simplex", "coord", "size", "pseed"), &_MMAlgos::fbm_simplex);
	ClassDB::bind_method(D_METHOD("fbm_cellular", "coord", "size", "pseed"), &_MMAlgos::fbm_cellular);
	ClassDB::bind_method(D_METHOD("fbm_cellular2", "coord", "size", "pseed"), &_MMAlgos::fbm_cellular2);
	ClassDB::bind_method(D_METHOD("fbm_cellular3", "coord", "size", "pseed"), &_MMAlgos::fbm_cellular3);
	ClassDB::bind_method(D_METHOD("fbm_cellular4", "coord", "size", "pseed"), &_MMAlgos::fbm_cellular4);
	ClassDB::bind_method(D_METHOD("fbm_cellular5", "coord", "size", "pseed"), &_MMAlgos::fbm_cellular5);
	ClassDB::bind_method(D_METHOD("fbm_cellular6", "coord", "size", "pseed"), &_MMAlgos::fbm_cellular6);
	ClassDB::bind_method(D_METHOD("perlinc", "uv", "size", "iterations", "persistence", "pseed"), &_MMAlgos::perlinc);
	ClassDB::bind_method(D_METHOD("perlin2c", "uv", "size", "iterations", "persistence", "pseed"), &_MMAlgos::perlin2c);
	ClassDB::bind_method(D_METHOD("perlin_color", "uv", "size", "iterations", "persistence", "pseed"), &_MMAlgos::perlin_color);
	ClassDB::bind_method(D_METHOD("perlin_colorc", "uv", "size", "iterations", "persistence", "pseed"), &_MMAlgos::perlin_colorc);
	ClassDB::bind_method(D_METHOD("perlin_warp_1", "uv", "size", "iterations", "persistence", "pseed", "translate", "rotate", "size2"), &_MMAlgos::perlin_warp_1);
	ClassDB::bind_method(D_METHOD("perlin_warp_2", "uv", "size", "iterations", "persistence", "pseed", "translate", "rotate", "size2"), &_MMAlgos::perlin_warp_2);
	ClassDB::bind_method(D_METHOD("voronoi", "uv", "size", "stretch", "intensity", "randomness", "pseed"), &_MMAlgos::voronoi);
	ClassDB::bind_method(D_METHOD("voronoi_1", "uv", "size", "stretch", "intensity", "randomness", "pseed"), &_MMAlgos::voronoi_1);
	ClassDB::bind_method(D_METHOD("voronoi_2", "uv", "size", "stretch", "intensity", "randomness", "pseed"), &_MMAlgos::voronoi_2);
	ClassDB::bind_method(D_METHOD("voronoi_3", "uv", "size", "stretch", "intensity", "randomness", "pseed"), &_MMAlgos::voronoi_3);
	ClassDB::bind_method(D_METHOD("pattern", "uv", "x_scale", "y_scale", "ct", "catx", "caty"), &_MMAlgos::pattern);
	ClassDB::bind_method(D_METHOD("truchet1c", "uv", "size", "pseed"), &_MMAlgos::truchet1c);
	ClassDB::bind_method(D_METHOD("truchet1", "uv", "pseed"), &_MMAlgos::truchet1);
	ClassDB::bind_method(D_METHOD("truchet2c", "uv", "size", "pseed"), &_MMAlgos::truchet2c);
	ClassDB::bind_method(D_METHOD("truchet2", "uv", "pseed"), &_MMAlgos::truchet2);
	ClassDB::bind_method(D_METHOD("weavec", "uv", "count", "width"), &_MMAlgos::weavec);
	ClassDB::bind_method(D_METHOD("weave", "uv", "count", "width"), &_MMAlgos::weave);
	ClassDB::bind_method(D_METHOD("weave2", "uv", "count", "stitch", "width_x", "width_y"), &_MMAlgos::weave2);
	ClassDB::bind_method(D_METHOD("sinewavec", "uv", "amplitude", "frequency", "phase"), &_MMAlgos::sinewavec);
	ClassDB::bind_method(D_METHOD("sinewavef", "uv", "amplitude", "frequency", "phase"), &_MMAlgos::sinewavef);
	ClassDB::bind_method(D_METHOD("scratch", "uv", "size", "waviness", "angle", "randomness", "pseed"), &_MMAlgos::scratch);
	ClassDB::bind_method(D_METHOD("scratches", "uv", "layers", "size", "waviness", "angle", "randomness", "pseed"), &_MMAlgos::scratches);
	ClassDB::bind_method(D_METHOD("scratchesc", "uv", "layers", "size", "waviness", "angle", "randomness", "pseed"), &_MMAlgos::scratchesc);
	ClassDB::bind_method(D_METHOD("runesc", "uv", "col_row", "pseed"), &_MMAlgos::runesc);
	ClassDB::bind_method(D_METHOD("runesf", "uv", "col_row", "pseed"), &_MMAlgos::runesf);
	ClassDB::bind_method(D_METHOD("rune", "uv", "pseed"), &_MMAlgos::rune);
	ClassDB::bind_method(D_METHOD("IChingc", "uv", "row_col", "pseed"), &_MMAlgos::IChingc);
	ClassDB::bind_method(D_METHOD("IChing", "uv", "pseed"), &_MMAlgos::IChing);
	ClassDB::bind_method(D_METHOD("beehive_1c", "uv", "size", "pseed"), &_MMAlgos::beehive_1c);
	ClassDB::bind_method(D_METHOD("beehive_2c", "uv", "size", "pseed"), &_MMAlgos::beehive_2c);
	ClassDB::bind_method(D_METHOD("beehive_3c", "uv", "size", "pseed"), &_MMAlgos::beehive_3c);
	ClassDB::bind_method(D_METHOD("beehive_dist", "p"), &_MMAlgos::beehive_dist);
	ClassDB::bind_method(D_METHOD("beehive_center", "p"), &_MMAlgos::beehive_center);
	ClassDB::bind_method(D_METHOD("brick_corner_uv", "uv", "bmin", "bmax", "mortar", "corner", "pseed"), &_MMAlgos::brick_corner_uv);
	ClassDB::bind_method(D_METHOD("brick", "uv", "bmin", "bmax", "mortar", "pround", "bevel"), &_MMAlgos::brick);
	ClassDB::bind_method(D_METHOD("brick_random_color", "bmin", "bmax", "pseed"), &_MMAlgos::brick_random_color);
	ClassDB::bind_method(D_METHOD("brick_uv", "uv", "bmin", "bmax", "pseed"), &_MMAlgos::brick_uv);
	ClassDB::bind_method(D_METHOD("bricks_rb", "uv", "count", "repeat", "offset"), &_MMAlgos::bricks_rb);
	ClassDB::bind_method(D_METHOD("bricks_rb2", "uv", "count", "repeat", "offset"), &_MMAlgos::bricks_rb2);
	ClassDB::bind_method(D_METHOD("bricks_hb", "uv", "count", "repeat", "offset"), &_MMAlgos::bricks_hb);
	ClassDB::bind_method(D_METHOD("bricks_bw", "uv", "count", "repeat", "offset"), &_MMAlgos::bricks_bw);
	ClassDB::bind_method(D_METHOD("bricks_sb", "uv", "count", "repeat", "offset"), &_MMAlgos::bricks_sb);
	ClassDB::bind_method(D_METHOD("brick2", "uv", "bmin", "bmax", "mortar", "pround", "bevel"), &_MMAlgos::brick2);
	ClassDB::bind_method(D_METHOD("bricks_uneven", "uv", "iterations", "min_size", "randomness", "pseed"), &_MMAlgos::bricks_uneven);
	ClassDB::bind_method(D_METHOD("truchet_generic_uv", "uv", "pseed"), &_MMAlgos::truchet_generic_uv);
	ClassDB::bind_method(D_METHOD("sdf_show", "val", "bevel"), &_MMAlgos::sdf_show);
	ClassDB::bind_method(D_METHOD("sdf_circle", "uv", "c", "r"), &_MMAlgos::sdf_circle);
	ClassDB::bind_method(D_METHOD("sdf_box", "uv", "c", "wh"), &_MMAlgos::sdf_box);
	ClassDB::bind_method(D_METHOD("sdf_line", "uv", "a", "b", "r"), &_MMAlgos::sdf_line);
	ClassDB::bind_method(D_METHOD("sdf_rhombus", "uv", "c", "wh"), &_MMAlgos::sdf_rhombus);
	ClassDB::bind_method(D_METHOD("sdf_arc", "uv", "a", "r"), &_MMAlgos::sdf_arc);
	ClassDB::bind_method(D_METHOD("sdr_ndot", "a", "b"), &_MMAlgos::sdr_ndot);
	ClassDB::bind_method(D_METHOD("sdRhombus", "p", "b"), &_MMAlgos::sdRhombus);
	ClassDB::bind_method(D_METHOD("sdArc", "p", "a1", "a2", "ra", "rb"), &_MMAlgos::sdArc);
	ClassDB::bind_method(D_METHOD("sdf_boolean_union", "a", "b"), &_MMAlgos::sdf_boolean_union);
	ClassDB::bind_method(D_METHOD("sdf_boolean_substraction", "a", "b"), &_MMAlgos::sdf_boolean_substraction);
	ClassDB::bind_method(D_METHOD("sdf_boolean_intersection", "a", "b"), &_MMAlgos::sdf_boolean_intersection);
	ClassDB::bind_method(D_METHOD("sdf_smooth_boolean_union", "d1", "d2", "k"), &_MMAlgos::sdf_smooth_boolean_union);
	ClassDB::bind_method(D_METHOD("sdf_smooth_boolean_substraction", "d1", "d2", "k"), &_MMAlgos::sdf_smooth_boolean_substraction);
	ClassDB::bind_method(D_METHOD("sdf_smooth_boolean_intersection", "d1", "d2", "k"), &_MMAlgos::sdf_smooth_boolean_intersection);
	ClassDB::bind_method(D_METHOD("sdf_rounded_shape", "a", "r"), &_MMAlgos::sdf_rounded_shape);
	ClassDB::bind_method(D_METHOD("sdf_annular_shape", "a", "r"), &_MMAlgos::sdf_annular_shape);
	ClassDB::bind_method(D_METHOD("sdf_morph", "a", "b", "amount"), &_MMAlgos::sdf_morph);
	ClassDB::bind_method(D_METHOD("sdLine", "p", "a", "b"), &_MMAlgos::sdLine);
	ClassDB::bind_method(D_METHOD("sdf2d_rotate", "uv", "a"), &_MMAlgos::sdf2d_rotate);
	ClassDB::bind_method(D_METHOD("sdBezier", "pos", "A", "B", "C"), &_MMAlgos::sdBezier);
	ClassDB::bind_method(D_METHOD("circle_repeat_transform_2d", "p", "count"), &_MMAlgos::circle_repeat_transform_2d);
	ClassDB::bind_method(D_METHOD("sdNgon", "pos", "r", "n"), &_MMAlgos::sdNgon);
	ClassDB::bind_method(D_METHOD("repeat_2d", "p", "r", "pseed", "randomness"), &_MMAlgos::repeat_2d);
	ClassDB::bind_method(D_METHOD("sdSmoothUnion", "d1", "d2", "k"), &_MMAlgos::sdSmoothUnion);
	ClassDB::bind_method(D_METHOD("sdSmoothSubtraction", "d1", "d2", "k"), &_MMAlgos::sdSmoothSubtraction);
	ClassDB::bind_method(D_METHOD("sdSmoothIntersection", "d1", "d2", "k"), &_MMAlgos::sdSmoothIntersection);
	ClassDB::bind_method(D_METHOD("sdRipples", "d", "w", "r"), &_MMAlgos::sdRipples);
	ClassDB::bind_method(D_METHOD("sdPolygon", "p", "v"), &_MMAlgos::sdPolygon);
	ClassDB::bind_method(D_METHOD("raymarch", "uv"), &_MMAlgos::raymarch);
	ClassDB::bind_method(D_METHOD("raymarch2", "uv"), &_MMAlgos::raymarch2);
	ClassDB::bind_method(D_METHOD("raymarch3", "uv"), &_MMAlgos::raymarch3);
	ClassDB::bind_method(D_METHOD("sdf3d_sphere", "p", "r"), &_MMAlgos::sdf3d_sphere);
	ClassDB::bind_method(D_METHOD("sdf3d_box", "p", "sx", "sy", "sz", "r"), &_MMAlgos::sdf3d_box);
	ClassDB::bind_method(D_METHOD("sdf3d_cylinder_y", "p", "r", "l"), &_MMAlgos::sdf3d_cylinder_y);
	ClassDB::bind_method(D_METHOD("sdf3d_cylinder_x", "p", "r", "l"), &_MMAlgos::sdf3d_cylinder_x);
	ClassDB::bind_method(D_METHOD("sdf3d_cylinder_z", "p", "r", "l"), &_MMAlgos::sdf3d_cylinder_z);
	ClassDB::bind_method(D_METHOD("sdf3d_capsule_y", "p", "r", "l"), &_MMAlgos::sdf3d_capsule_y);
	ClassDB::bind_method(D_METHOD("sdf3d_capsule_x", "p", "r", "l"), &_MMAlgos::sdf3d_capsule_x);
	ClassDB::bind_method(D_METHOD("sdf3d_capsule_z", "p", "r", "l"), &_MMAlgos::sdf3d_capsule_z);
	ClassDB::bind_method(D_METHOD("sdf3d_cone_px", "p", "a"), &_MMAlgos::sdf3d_cone_px);
	ClassDB::bind_method(D_METHOD("sdf3d_cone_nx", "p", "a"), &_MMAlgos::sdf3d_cone_nx);
	ClassDB::bind_method(D_METHOD("sdf3d_cone_py", "p", "a"), &_MMAlgos::sdf3d_cone_py);
	ClassDB::bind_method(D_METHOD("sdf3d_cone_ny", "p", "a"), &_MMAlgos::sdf3d_cone_ny);
	ClassDB::bind_method(D_METHOD("sdf3d_cone_pz", "p", "a"), &_MMAlgos::sdf3d_cone_pz);
	ClassDB::bind_method(D_METHOD("sdf3d_cone_nz", "p", "a"), &_MMAlgos::sdf3d_cone_nz);
	ClassDB::bind_method(D_METHOD("sdf3d_torus_x", "p", "R", "r"), &_MMAlgos::sdf3d_torus_x);
	ClassDB::bind_method(D_METHOD("sdf3d_torus_y", "p", "R", "r"), &_MMAlgos::sdf3d_torus_y);
	ClassDB::bind_method(D_METHOD("sdf3d_torus_z", "p", "R", "r"), &_MMAlgos::sdf3d_torus_z);
	ClassDB::bind_method(D_METHOD("sdf3d_raymarch", "uv"), &_MMAlgos::sdf3d_raymarch);
	ClassDB::bind_method(D_METHOD("sdf3d_normal", "p"), &_MMAlgos::sdf3d_normal);
	ClassDB::bind_method(D_METHOD("sdf3dc_union", "a", "b"), &_MMAlgos::sdf3dc_union);
	ClassDB::bind_method(D_METHOD("sdf3dc_sub", "a", "b"), &_MMAlgos::sdf3dc_sub);
	ClassDB::bind_method(D_METHOD("sdf3dc_inter", "a", "b"), &_MMAlgos::sdf3dc_inter);
	ClassDB::bind_method(D_METHOD("sdf3d_smooth_union", "d1", "d2", "k"), &_MMAlgos::sdf3d_smooth_union);
	ClassDB::bind_method(D_METHOD("sdf3d_smooth_subtraction", "d1", "d2", "k"), &_MMAlgos::sdf3d_smooth_subtraction);
	ClassDB::bind_method(D_METHOD("sdf3d_smooth_intersection", "d1", "d2", "k"), &_MMAlgos::sdf3d_smooth_intersection);
	ClassDB::bind_method(D_METHOD("sdf3d_rounded", "v", "r"), &_MMAlgos::sdf3d_rounded);
	ClassDB::bind_method(D_METHOD("sdf3d_elongation", "p", "v"), &_MMAlgos::sdf3d_elongation);
	ClassDB::bind_method(D_METHOD("sdf3d_repeat", "p", "r", "randomness", "pseed"), &_MMAlgos::sdf3d_repeat);
	ClassDB::bind_method(D_METHOD("repeat", "p", "r", "pseed", "randomness"), &_MMAlgos::repeat);
	ClassDB::bind_method(D_METHOD("rotate3d", "p", "a"), &_MMAlgos::rotate3d);
	ClassDB::bind_method(D_METHOD("circle_repeat_transform", "p", "count"), &_MMAlgos::circle_repeat_transform);
	ClassDB::bind_method(D_METHOD("sdf3d_input", "p"), &_MMAlgos::sdf3d_input);
	ClassDB::bind_method(D_METHOD("sphere", "uv", "c", "r"), &_MMAlgos::sphere);
	ClassDB::bind_method(D_METHOD("shape_circle", "uv", "sides", "size", "edge"), &_MMAlgos::shape_circle);
	ClassDB::bind_method(D_METHOD("shape_polygon", "uv", "sides", "size", "edge"), &_MMAlgos::shape_polygon);
	ClassDB::bind_method(D_METHOD("shape_star", "uv", "sides", "size", "edge"), &_MMAlgos::shape_star);
	ClassDB::bind_method(D_METHOD("shape_curved_star", "uv", "sides", "size", "edge"), &_MMAlgos::shape_curved_star);
	ClassDB::bind_method(D_METHOD("shape_rays", "uv", "sides", "size", "edge"), &_MMAlgos::shape_rays);
	ClassDB::bind_method(D_METHOD("transform2_clamp", "uv"), &_MMAlgos::transform2_clamp);
	ClassDB::bind_method(D_METHOD("transform2", "uv", "translate", "rotate", "scale"), &_MMAlgos::transform2);
	ClassDB::bind_method(D_METHOD("rotate", "uv", "center", "rotate"), &_MMAlgos::rotate);
	ClassDB::bind_method(D_METHOD("scale", "uv", "center", "scale"), &_MMAlgos::scale);
	ClassDB::bind_method(D_METHOD("uvmirror_h", "uv", "offset"), &_MMAlgos::uvmirror_h);
	ClassDB::bind_method(D_METHOD("uvmirror_v", "uv", "offset"), &_MMAlgos::uvmirror_v);
	ClassDB::bind_method(D_METHOD("kal_rotate", "uv", "count", "offset"), &_MMAlgos::kal_rotate);
	ClassDB::bind_method(D_METHOD("get_from_tileset", "count", "pseed", "uv"), &_MMAlgos::get_from_tileset);
	ClassDB::bind_method(D_METHOD("custom_uv_transform", "uv", "cst_scale", "rnd_rotate", "rnd_scale", "pseed"), &_MMAlgos::custom_uv_transform);

	ClassDB::bind_method(D_METHOD("register_node_class", "category", "cls"), &_MMAlgos::register_node_class);
	ClassDB::bind_method(D_METHOD("unregister_node_class", "category", "cls"), &_MMAlgos::unregister_node_class);

	ClassDB::bind_method(D_METHOD("register_node_script", "category", "file_path"), &_MMAlgos::register_node_script);
	ClassDB::bind_method(D_METHOD("unregister_node_script", "category", "file_path"), &_MMAlgos::unregister_node_script);
}

_MMAlgos *_MMAlgos::self = nullptr;
