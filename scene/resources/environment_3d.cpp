/*************************************************************************/
/*  environment_3d.cpp                                                   */
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

#include "environment_3d.h"

#include "core/config/project_settings.h"
#include "scene/resources/sky.h"
#include "scene/resources/texture.h"
#include "servers/rendering_server.h"
#include "texture.h"

RID Environment3D::get_rid() const {
	return environment;
}

void Environment3D::set_background(BGMode p_bg) {
	bg_mode = p_bg;
	RS::get_singleton()->environment_set_background(environment, RS::Environment3DBG(p_bg));
	_change_notify();
}

void Environment3D::set_sky(const Ref<Sky> &p_sky) {
	bg_sky = p_sky;

	RID sb_rid;
	if (bg_sky.is_valid()) {
		sb_rid = bg_sky->get_rid();
	}

	RS::get_singleton()->environment_set_sky(environment, sb_rid);
}

void Environment3D::set_sky_custom_fov(float p_scale) {
	bg_sky_custom_fov = p_scale;
	RS::get_singleton()->environment_set_sky_custom_fov(environment, p_scale);
}
void Environment3D::set_sky_orientation(const Basis &p_orientation) {
	bg_sky_orientation = p_orientation;
	_change_notify("background_sky_rotation");
	_change_notify("background_sky_rotation_degrees");
	RS::get_singleton()->environment_set_sky_orientation(environment, bg_sky_orientation);
}
void Environment3D::set_sky_rotation(const Vector3 &p_euler_rad) {
	bg_sky_orientation.set_euler(p_euler_rad);
	_change_notify("background_sky_orientation");
	_change_notify("background_sky_rotation_degrees");
	RS::get_singleton()->environment_set_sky_orientation(environment, bg_sky_orientation);
}
void Environment3D::set_sky_rotation_degrees(const Vector3 &p_euler_deg) {
	set_sky_rotation(p_euler_deg * Math_PI / 180.0);
	_change_notify("background_sky_rotation");
}
void Environment3D::set_bg_color(const Color &p_color) {
	bg_color = p_color;
	RS::get_singleton()->environment_set_bg_color(environment, p_color);
}
void Environment3D::set_bg_energy(float p_energy) {
	bg_energy = p_energy;
	RS::get_singleton()->environment_set_bg_energy(environment, p_energy);
}
void Environment3D::set_canvas_max_layer(int p_max_layer) {
	bg_canvas_max_layer = p_max_layer;
	RS::get_singleton()->environment_set_canvas_max_layer(environment, p_max_layer);
}
void Environment3D::set_ambient_light_color(const Color &p_color) {
	ambient_color = p_color;
	RS::get_singleton()->environment_set_ambient_light(environment, ambient_color, ambient_energy, ambient_sky_contribution);
}
void Environment3D::set_ambient_light_energy(float p_energy) {
	ambient_energy = p_energy;
	RS::get_singleton()->environment_set_ambient_light(environment, ambient_color, ambient_energy, ambient_sky_contribution);
}
void Environment3D::set_ambient_light_sky_contribution(float p_energy) {
	// Sky contribution values outside the [0.0; 1.0] range don't make sense and
	// can result in negative colors.
	ambient_sky_contribution = CLAMP(p_energy, 0.0, 1.0);
	RS::get_singleton()->environment_set_ambient_light(environment, ambient_color, ambient_energy, ambient_sky_contribution);
}

Environment3D::BGMode Environment3D::get_background() const {
	return bg_mode;
}
Ref<Sky> Environment3D::get_sky() const {
	return bg_sky;
}

float Environment3D::get_sky_custom_fov() const {
	return bg_sky_custom_fov;
}

Basis Environment3D::get_sky_orientation() const {
	return bg_sky_orientation;
}

Vector3 Environment3D::get_sky_rotation() const {
	// should we cache this? maybe overkill
	return bg_sky_orientation.get_euler();
}

Vector3 Environment3D::get_sky_rotation_degrees() const {
	return get_sky_rotation() * 180.0 / Math_PI;
}

Color Environment3D::get_bg_color() const {
	return bg_color;
}
float Environment3D::get_bg_energy() const {
	return bg_energy;
}
int Environment3D::get_canvas_max_layer() const {
	return bg_canvas_max_layer;
}
Color Environment3D::get_ambient_light_color() const {
	return ambient_color;
}
float Environment3D::get_ambient_light_energy() const {
	return ambient_energy;
}
float Environment3D::get_ambient_light_sky_contribution() const {
	return ambient_sky_contribution;
}

void Environment3D::set_tonemapper(ToneMapper p_tone_mapper) {
	tone_mapper = p_tone_mapper;
	RS::get_singleton()->environment_set_tonemap(environment, RS::Environment3DToneMapper(tone_mapper), tonemap_exposure, tonemap_white, tonemap_auto_exposure, tonemap_auto_exposure_min, tonemap_auto_exposure_max, tonemap_auto_exposure_speed, tonemap_auto_exposure_grey);
	_change_notify();
}

Environment3D::ToneMapper Environment3D::get_tonemapper() const {
	return tone_mapper;
}

void Environment3D::set_tonemap_exposure(float p_exposure) {
	tonemap_exposure = p_exposure;
	RS::get_singleton()->environment_set_tonemap(environment, RS::Environment3DToneMapper(tone_mapper), tonemap_exposure, tonemap_white, tonemap_auto_exposure, tonemap_auto_exposure_min, tonemap_auto_exposure_max, tonemap_auto_exposure_speed, tonemap_auto_exposure_grey);
}

float Environment3D::get_tonemap_exposure() const {
	return tonemap_exposure;
}

void Environment3D::set_tonemap_white(float p_white) {
	tonemap_white = p_white;
	RS::get_singleton()->environment_set_tonemap(environment, RS::Environment3DToneMapper(tone_mapper), tonemap_exposure, tonemap_white, tonemap_auto_exposure, tonemap_auto_exposure_min, tonemap_auto_exposure_max, tonemap_auto_exposure_speed, tonemap_auto_exposure_grey);
}
float Environment3D::get_tonemap_white() const {
	return tonemap_white;
}

void Environment3D::set_tonemap_auto_exposure(bool p_enabled) {
	tonemap_auto_exposure = p_enabled;
	RS::get_singleton()->environment_set_tonemap(environment, RS::Environment3DToneMapper(tone_mapper), tonemap_exposure, tonemap_white, tonemap_auto_exposure, tonemap_auto_exposure_min, tonemap_auto_exposure_max, tonemap_auto_exposure_speed, tonemap_auto_exposure_grey);
	_change_notify();
}
bool Environment3D::get_tonemap_auto_exposure() const {
	return tonemap_auto_exposure;
}

void Environment3D::set_tonemap_auto_exposure_max(float p_auto_exposure_max) {
	tonemap_auto_exposure_max = p_auto_exposure_max;
	RS::get_singleton()->environment_set_tonemap(environment, RS::Environment3DToneMapper(tone_mapper), tonemap_exposure, tonemap_white, tonemap_auto_exposure, tonemap_auto_exposure_min, tonemap_auto_exposure_max, tonemap_auto_exposure_speed, tonemap_auto_exposure_grey);
}
float Environment3D::get_tonemap_auto_exposure_max() const {
	return tonemap_auto_exposure_max;
}

void Environment3D::set_tonemap_auto_exposure_min(float p_auto_exposure_min) {
	tonemap_auto_exposure_min = p_auto_exposure_min;
	RS::get_singleton()->environment_set_tonemap(environment, RS::Environment3DToneMapper(tone_mapper), tonemap_exposure, tonemap_white, tonemap_auto_exposure, tonemap_auto_exposure_min, tonemap_auto_exposure_max, tonemap_auto_exposure_speed, tonemap_auto_exposure_grey);
}
float Environment3D::get_tonemap_auto_exposure_min() const {
	return tonemap_auto_exposure_min;
}

void Environment3D::set_tonemap_auto_exposure_speed(float p_auto_exposure_speed) {
	tonemap_auto_exposure_speed = p_auto_exposure_speed;
	RS::get_singleton()->environment_set_tonemap(environment, RS::Environment3DToneMapper(tone_mapper), tonemap_exposure, tonemap_white, tonemap_auto_exposure, tonemap_auto_exposure_min, tonemap_auto_exposure_max, tonemap_auto_exposure_speed, tonemap_auto_exposure_grey);
}
float Environment3D::get_tonemap_auto_exposure_speed() const {
	return tonemap_auto_exposure_speed;
}

void Environment3D::set_tonemap_auto_exposure_grey(float p_auto_exposure_grey) {
	tonemap_auto_exposure_grey = p_auto_exposure_grey;
	RS::get_singleton()->environment_set_tonemap(environment, RS::Environment3DToneMapper(tone_mapper), tonemap_exposure, tonemap_white, tonemap_auto_exposure, tonemap_auto_exposure_min, tonemap_auto_exposure_max, tonemap_auto_exposure_speed, tonemap_auto_exposure_grey);
}
float Environment3D::get_tonemap_auto_exposure_grey() const {
	return tonemap_auto_exposure_grey;
}

void Environment3D::set_adjustment_enable(bool p_enable) {
	adjustment_enabled = p_enable;
	RS::get_singleton()->environment_set_adjustment(environment, adjustment_enabled, adjustment_brightness, adjustment_contrast, adjustment_saturation, adjustment_color_correction.is_valid() ? adjustment_color_correction->get_rid() : RID());
	_change_notify();
}

bool Environment3D::is_adjustment_enabled() const {
	return adjustment_enabled;
}

void Environment3D::set_adjustment_brightness(float p_brightness) {
	adjustment_brightness = p_brightness;
	RS::get_singleton()->environment_set_adjustment(environment, adjustment_enabled, adjustment_brightness, adjustment_contrast, adjustment_saturation, adjustment_color_correction.is_valid() ? adjustment_color_correction->get_rid() : RID());
}
float Environment3D::get_adjustment_brightness() const {
	return adjustment_brightness;
}

void Environment3D::set_adjustment_contrast(float p_contrast) {
	adjustment_contrast = p_contrast;
	RS::get_singleton()->environment_set_adjustment(environment, adjustment_enabled, adjustment_brightness, adjustment_contrast, adjustment_saturation, adjustment_color_correction.is_valid() ? adjustment_color_correction->get_rid() : RID());
}
float Environment3D::get_adjustment_contrast() const {
	return adjustment_contrast;
}

void Environment3D::set_adjustment_saturation(float p_saturation) {
	adjustment_saturation = p_saturation;
	RS::get_singleton()->environment_set_adjustment(environment, adjustment_enabled, adjustment_brightness, adjustment_contrast, adjustment_saturation, adjustment_color_correction.is_valid() ? adjustment_color_correction->get_rid() : RID());
}
float Environment3D::get_adjustment_saturation() const {
	return adjustment_saturation;
}

void Environment3D::set_adjustment_color_correction(const Ref<Texture> &p_ramp) {
	adjustment_color_correction = p_ramp;
	RS::get_singleton()->environment_set_adjustment(environment, adjustment_enabled, adjustment_brightness, adjustment_contrast, adjustment_saturation, adjustment_color_correction.is_valid() ? adjustment_color_correction->get_rid() : RID());
}
Ref<Texture> Environment3D::get_adjustment_color_correction() const {
	return adjustment_color_correction;
}

void Environment3D::_validate_property(PropertyInfo &property) const {
	if (property.name == "background_sky" || property.name == "background_sky_custom_fov" || property.name == "background_sky_orientation" || property.name == "background_sky_rotation" || property.name == "background_sky_rotation_degrees" || property.name == "ambient_light/sky_contribution") {
		if (bg_mode != BG_SKY && bg_mode != BG_COLOR_SKY) {
			property.usage = PROPERTY_USAGE_NOEDITOR | PROPERTY_USAGE_INTERNAL;
		}
	}

	if (property.name == "background_color") {
		if (bg_mode != BG_COLOR && bg_mode != BG_COLOR_SKY) {
			property.usage = PROPERTY_USAGE_NOEDITOR | PROPERTY_USAGE_INTERNAL;
		}
	}

	if (property.name == "background_canvas_max_layer") {
		if (bg_mode != BG_CANVAS) {
			property.usage = PROPERTY_USAGE_NOEDITOR | PROPERTY_USAGE_INTERNAL;
		}
	}

	if (property.name == "tonemap_white") {
		if (tone_mapper == TONE_MAPPER_LINEAR) {
			property.usage = PROPERTY_USAGE_NOEDITOR | PROPERTY_USAGE_INTERNAL;
		}
	}

	static const char *hide_prefixes[] = {
		"fog_",
		"auto_exposure_",
		"ss_reflections_",
		"ssao_",
		"dof_blur_far_",
		"dof_blur_near_",
		"glow_",
		"adjustment_",
		nullptr

	};

	static const char *high_end_prefixes[] = {
		"auto_exposure_",
		"tonemap_",
		"ss_reflections_",
		"ssao_",
		nullptr

	};

	const char **prefixes = hide_prefixes;
	while (*prefixes) {
		String prefix = String(*prefixes);

		String enabled = prefix + "enabled";
		if (property.name.begins_with(prefix) && property.name != enabled && !bool(get(enabled))) {
			property.usage = PROPERTY_USAGE_NOEDITOR | PROPERTY_USAGE_INTERNAL;
			return;
		}

		prefixes++;
	}

	if (RenderingServer::get_singleton()->is_low_end()) {
		prefixes = high_end_prefixes;
		while (*prefixes) {
			String prefix = String(*prefixes);

			if (property.name.begins_with(prefix)) {
				property.usage = PROPERTY_USAGE_NOEDITOR | PROPERTY_USAGE_INTERNAL;
				return;
			}

			prefixes++;
		}
	}
}

void Environment3D::set_ssr_enabled(bool p_enable) {
	ssr_enabled = p_enable;
	RS::get_singleton()->environment_set_ssr(environment, ssr_enabled, ssr_max_steps, ssr_fade_in, ssr_fade_out, ssr_depth_tolerance, ssr_roughness);
	_change_notify();
}

bool Environment3D::is_ssr_enabled() const {
	return ssr_enabled;
}

void Environment3D::set_ssr_max_steps(int p_steps) {
	ssr_max_steps = p_steps;
	RS::get_singleton()->environment_set_ssr(environment, ssr_enabled, ssr_max_steps, ssr_fade_in, ssr_fade_out, ssr_depth_tolerance, ssr_roughness);
}
int Environment3D::get_ssr_max_steps() const {
	return ssr_max_steps;
}

void Environment3D::set_ssr_fade_in(float p_fade_in) {
	ssr_fade_in = p_fade_in;
	RS::get_singleton()->environment_set_ssr(environment, ssr_enabled, ssr_max_steps, ssr_fade_in, ssr_fade_out, ssr_depth_tolerance, ssr_roughness);
}
float Environment3D::get_ssr_fade_in() const {
	return ssr_fade_in;
}

void Environment3D::set_ssr_fade_out(float p_fade_out) {
	ssr_fade_out = p_fade_out;
	RS::get_singleton()->environment_set_ssr(environment, ssr_enabled, ssr_max_steps, ssr_fade_in, ssr_fade_out, ssr_depth_tolerance, ssr_roughness);
}
float Environment3D::get_ssr_fade_out() const {
	return ssr_fade_out;
}

void Environment3D::set_ssr_depth_tolerance(float p_depth_tolerance) {
	ssr_depth_tolerance = p_depth_tolerance;
	RS::get_singleton()->environment_set_ssr(environment, ssr_enabled, ssr_max_steps, ssr_fade_in, ssr_fade_out, ssr_depth_tolerance, ssr_roughness);
}
float Environment3D::get_ssr_depth_tolerance() const {
	return ssr_depth_tolerance;
}

void Environment3D::set_ssr_rough(bool p_enable) {
	ssr_roughness = p_enable;
	RS::get_singleton()->environment_set_ssr(environment, ssr_enabled, ssr_max_steps, ssr_fade_in, ssr_fade_out, ssr_depth_tolerance, ssr_roughness);
}
bool Environment3D::is_ssr_rough() const {
	return ssr_roughness;
}

void Environment3D::set_ssao_enabled(bool p_enable) {
	ssao_enabled = p_enable;
	RS::get_singleton()->environment_set_ssao(environment, ssao_enabled, ssao_radius, ssao_intensity, ssao_radius2, ssao_intensity2, ssao_bias, ssao_direct_light_affect, ssao_ao_channel_affect, ssao_color, RS::Environment3DSSAOQuality(ssao_quality), RS::Environment3DSSAOBlur(ssao_blur), ssao_edge_sharpness);
	_change_notify();
}

bool Environment3D::is_ssao_enabled() const {
	return ssao_enabled;
}

void Environment3D::set_ssao_radius(float p_radius) {
	ssao_radius = p_radius;
	RS::get_singleton()->environment_set_ssao(environment, ssao_enabled, ssao_radius, ssao_intensity, ssao_radius2, ssao_intensity2, ssao_bias, ssao_direct_light_affect, ssao_ao_channel_affect, ssao_color, RS::Environment3DSSAOQuality(ssao_quality), RS::Environment3DSSAOBlur(ssao_blur), ssao_edge_sharpness);
}
float Environment3D::get_ssao_radius() const {
	return ssao_radius;
}

void Environment3D::set_ssao_intensity(float p_intensity) {
	ssao_intensity = p_intensity;
	RS::get_singleton()->environment_set_ssao(environment, ssao_enabled, ssao_radius, ssao_intensity, ssao_radius2, ssao_intensity2, ssao_bias, ssao_direct_light_affect, ssao_ao_channel_affect, ssao_color, RS::Environment3DSSAOQuality(ssao_quality), RS::Environment3DSSAOBlur(ssao_blur), ssao_edge_sharpness);
}

float Environment3D::get_ssao_intensity() const {
	return ssao_intensity;
}

void Environment3D::set_ssao_radius2(float p_radius) {
	ssao_radius2 = p_radius;
	RS::get_singleton()->environment_set_ssao(environment, ssao_enabled, ssao_radius, ssao_intensity, ssao_radius2, ssao_intensity2, ssao_bias, ssao_direct_light_affect, ssao_ao_channel_affect, ssao_color, RS::Environment3DSSAOQuality(ssao_quality), RS::Environment3DSSAOBlur(ssao_blur), ssao_edge_sharpness);
}
float Environment3D::get_ssao_radius2() const {
	return ssao_radius2;
}

void Environment3D::set_ssao_intensity2(float p_intensity) {
	ssao_intensity2 = p_intensity;
	RS::get_singleton()->environment_set_ssao(environment, ssao_enabled, ssao_radius, ssao_intensity, ssao_radius2, ssao_intensity2, ssao_bias, ssao_direct_light_affect, ssao_ao_channel_affect, ssao_color, RS::Environment3DSSAOQuality(ssao_quality), RS::Environment3DSSAOBlur(ssao_blur), ssao_edge_sharpness);
}
float Environment3D::get_ssao_intensity2() const {
	return ssao_intensity2;
}

void Environment3D::set_ssao_bias(float p_bias) {
	ssao_bias = p_bias;
	RS::get_singleton()->environment_set_ssao(environment, ssao_enabled, ssao_radius, ssao_intensity, ssao_radius2, ssao_intensity2, ssao_bias, ssao_direct_light_affect, ssao_ao_channel_affect, ssao_color, RS::Environment3DSSAOQuality(ssao_quality), RS::Environment3DSSAOBlur(ssao_blur), ssao_edge_sharpness);
}
float Environment3D::get_ssao_bias() const {
	return ssao_bias;
}

void Environment3D::set_ssao_direct_light_affect(float p_direct_light_affect) {
	ssao_direct_light_affect = p_direct_light_affect;
	RS::get_singleton()->environment_set_ssao(environment, ssao_enabled, ssao_radius, ssao_intensity, ssao_radius2, ssao_intensity2, ssao_bias, ssao_direct_light_affect, ssao_ao_channel_affect, ssao_color, RS::Environment3DSSAOQuality(ssao_quality), RS::Environment3DSSAOBlur(ssao_blur), ssao_edge_sharpness);
}
float Environment3D::get_ssao_direct_light_affect() const {
	return ssao_direct_light_affect;
}

void Environment3D::set_ssao_ao_channel_affect(float p_ao_channel_affect) {
	ssao_ao_channel_affect = p_ao_channel_affect;
	RS::get_singleton()->environment_set_ssao(environment, ssao_enabled, ssao_radius, ssao_intensity, ssao_radius2, ssao_intensity2, ssao_bias, ssao_direct_light_affect, ssao_ao_channel_affect, ssao_color, RS::Environment3DSSAOQuality(ssao_quality), RS::Environment3DSSAOBlur(ssao_blur), ssao_edge_sharpness);
}
float Environment3D::get_ssao_ao_channel_affect() const {
	return ssao_ao_channel_affect;
}

void Environment3D::set_ssao_color(const Color &p_color) {
	ssao_color = p_color;
	RS::get_singleton()->environment_set_ssao(environment, ssao_enabled, ssao_radius, ssao_intensity, ssao_radius2, ssao_intensity2, ssao_bias, ssao_direct_light_affect, ssao_ao_channel_affect, ssao_color, RS::Environment3DSSAOQuality(ssao_quality), RS::Environment3DSSAOBlur(ssao_blur), ssao_edge_sharpness);
}

Color Environment3D::get_ssao_color() const {
	return ssao_color;
}

void Environment3D::set_ssao_blur(SSAOBlur p_blur) {
	ssao_blur = p_blur;
	RS::get_singleton()->environment_set_ssao(environment, ssao_enabled, ssao_radius, ssao_intensity, ssao_radius2, ssao_intensity2, ssao_bias, ssao_direct_light_affect, ssao_ao_channel_affect, ssao_color, RS::Environment3DSSAOQuality(ssao_quality), RS::Environment3DSSAOBlur(ssao_blur), ssao_edge_sharpness);
}
Environment3D::SSAOBlur Environment3D::get_ssao_blur() const {
	return ssao_blur;
}

void Environment3D::set_ssao_quality(SSAOQuality p_quality) {
	ssao_quality = p_quality;
	RS::get_singleton()->environment_set_ssao(environment, ssao_enabled, ssao_radius, ssao_intensity, ssao_radius2, ssao_intensity2, ssao_bias, ssao_direct_light_affect, ssao_ao_channel_affect, ssao_color, RS::Environment3DSSAOQuality(ssao_quality), RS::Environment3DSSAOBlur(ssao_blur), ssao_edge_sharpness);
}

Environment3D::SSAOQuality Environment3D::get_ssao_quality() const {
	return ssao_quality;
}

void Environment3D::set_ssao_edge_sharpness(float p_edge_sharpness) {
	ssao_edge_sharpness = p_edge_sharpness;
	RS::get_singleton()->environment_set_ssao(environment, ssao_enabled, ssao_radius, ssao_intensity, ssao_radius2, ssao_intensity2, ssao_bias, ssao_direct_light_affect, ssao_ao_channel_affect, ssao_color, RS::Environment3DSSAOQuality(ssao_quality), RS::Environment3DSSAOBlur(ssao_blur), ssao_edge_sharpness);
}

float Environment3D::get_ssao_edge_sharpness() const {
	return ssao_edge_sharpness;
}

void Environment3D::set_glow_enabled(bool p_enabled) {
	glow_enabled = p_enabled;
	_update_glow();
	_change_notify();
}

bool Environment3D::is_glow_enabled() const {
	return glow_enabled;
}

void Environment3D::set_glow_level(int p_level, bool p_enabled) {
	ERR_FAIL_INDEX(p_level, RS::MAX_GLOW_LEVELS);

	if (p_enabled) {
		glow_levels |= (1 << p_level);
	} else {
		glow_levels &= ~(1 << p_level);
	}

	_update_glow();
}
bool Environment3D::is_glow_level_enabled(int p_level) const {
	ERR_FAIL_INDEX_V(p_level, RS::MAX_GLOW_LEVELS, false);

	return glow_levels & (1 << p_level);
}

void Environment3D::set_glow_intensity(float p_intensity) {
	glow_intensity = p_intensity;

	_update_glow();
}
float Environment3D::get_glow_intensity() const {
	return glow_intensity;
}

void Environment3D::set_glow_strength(float p_strength) {
	glow_strength = p_strength;

	_update_glow();
}
float Environment3D::get_glow_strength() const {
	return glow_strength;
}

void Environment3D::set_glow_bloom(float p_threshold) {
	glow_bloom = p_threshold;

	_update_glow();
}
float Environment3D::get_glow_bloom() const {
	return glow_bloom;
}

void Environment3D::set_glow_blend_mode(GlowBlendMode p_mode) {
	glow_blend_mode = p_mode;

	_update_glow();
}
Environment3D::GlowBlendMode Environment3D::get_glow_blend_mode() const {
	return glow_blend_mode;
}

void Environment3D::set_glow_hdr_bleed_threshold(float p_threshold) {
	glow_hdr_bleed_threshold = p_threshold;

	_update_glow();
}
float Environment3D::get_glow_hdr_bleed_threshold() const {
	return glow_hdr_bleed_threshold;
}

void Environment3D::set_glow_hdr_luminance_cap(float p_amount) {
	glow_hdr_luminance_cap = p_amount;

	_update_glow();
}
float Environment3D::get_glow_hdr_luminance_cap() const {
	return glow_hdr_luminance_cap;
}

void Environment3D::set_glow_hdr_bleed_scale(float p_scale) {
	glow_hdr_bleed_scale = p_scale;

	_update_glow();
}
float Environment3D::get_glow_hdr_bleed_scale() const {
	return glow_hdr_bleed_scale;
}

void Environment3D::set_glow_bicubic_upscale(bool p_enable) {
	glow_bicubic_upscale = p_enable;

	_update_glow();
}

bool Environment3D::is_glow_bicubic_upscale_enabled() const {
	return glow_bicubic_upscale;
}

void Environment3D::set_glow_high_quality(bool p_enable) {
	glow_high_quality = p_enable;

	_update_glow();
}

bool Environment3D::is_glow_high_quality_enabled() const {
	return glow_high_quality;
}

void Environment3D::set_glow_map_strength(float p_glow_map_strength) {
	glow_map_strength = p_glow_map_strength;
	_update_glow_map();
}

float Environment3D::get_glow_map_strength() const {
	return glow_map_strength;
}

void Environment3D::set_glow_map(Ref<Texture> p_glow_map) {
	glow_map = p_glow_map;
	_update_glow_map();
}

Ref<Texture> Environment3D::get_glow_map() const {
	return glow_map;
}

void Environment3D::_update_glow() {
	RS::get_singleton()->environment_set_glow(
			environment,
			glow_enabled,
			glow_levels,
			glow_intensity,
			glow_strength,
			glow_bloom,
			RS::Environment3DGlowBlendMode(glow_blend_mode),
			glow_hdr_bleed_threshold,
			glow_hdr_bleed_scale,
			glow_hdr_luminance_cap,
			glow_bicubic_upscale,
			glow_high_quality);
}

void Environment3D::_update_glow_map() {
	float _glow_map_strength = 0.0f;
	RID glow_map_rid;
	if (glow_map.is_valid()) {
		glow_map_rid = glow_map->get_rid();
		_glow_map_strength = glow_map_strength;
	}

	RS::get_singleton()->environment_set_glow_map(environment, _glow_map_strength, glow_map_rid);
}

void Environment3D::set_dof_blur_far_enabled(bool p_enable) {
	dof_blur_far_enabled = p_enable;
	RS::get_singleton()->environment_set_dof_blur_far(environment, dof_blur_far_enabled, dof_blur_far_distance, dof_blur_far_transition, dof_blur_far_amount, RS::Environment3DDOFBlurQuality(dof_blur_far_quality));
	_change_notify();
}

bool Environment3D::is_dof_blur_far_enabled() const {
	return dof_blur_far_enabled;
}

void Environment3D::set_dof_blur_far_distance(float p_distance) {
	dof_blur_far_distance = p_distance;
	RS::get_singleton()->environment_set_dof_blur_far(environment, dof_blur_far_enabled, dof_blur_far_distance, dof_blur_far_transition, dof_blur_far_amount, RS::Environment3DDOFBlurQuality(dof_blur_far_quality));
}
float Environment3D::get_dof_blur_far_distance() const {
	return dof_blur_far_distance;
}

void Environment3D::set_dof_blur_far_transition(float p_distance) {
	dof_blur_far_transition = p_distance;
	RS::get_singleton()->environment_set_dof_blur_far(environment, dof_blur_far_enabled, dof_blur_far_distance, dof_blur_far_transition, dof_blur_far_amount, RS::Environment3DDOFBlurQuality(dof_blur_far_quality));
}
float Environment3D::get_dof_blur_far_transition() const {
	return dof_blur_far_transition;
}

void Environment3D::set_dof_blur_far_amount(float p_amount) {
	dof_blur_far_amount = p_amount;
	RS::get_singleton()->environment_set_dof_blur_far(environment, dof_blur_far_enabled, dof_blur_far_distance, dof_blur_far_transition, dof_blur_far_amount, RS::Environment3DDOFBlurQuality(dof_blur_far_quality));
}
float Environment3D::get_dof_blur_far_amount() const {
	return dof_blur_far_amount;
}

void Environment3D::set_dof_blur_far_quality(DOFBlurQuality p_quality) {
	dof_blur_far_quality = p_quality;
	RS::get_singleton()->environment_set_dof_blur_far(environment, dof_blur_far_enabled, dof_blur_far_distance, dof_blur_far_transition, dof_blur_far_amount, RS::Environment3DDOFBlurQuality(dof_blur_far_quality));
}

Environment3D::DOFBlurQuality Environment3D::get_dof_blur_far_quality() const {
	return dof_blur_far_quality;
}

void Environment3D::set_dof_blur_near_enabled(bool p_enable) {
	dof_blur_near_enabled = p_enable;
	RS::get_singleton()->environment_set_dof_blur_near(environment, dof_blur_near_enabled, dof_blur_near_distance, dof_blur_near_transition, dof_blur_near_amount, RS::Environment3DDOFBlurQuality(dof_blur_near_quality));
	_change_notify();
}

bool Environment3D::is_dof_blur_near_enabled() const {
	return dof_blur_near_enabled;
}

void Environment3D::set_dof_blur_near_distance(float p_distance) {
	dof_blur_near_distance = p_distance;
	RS::get_singleton()->environment_set_dof_blur_near(environment, dof_blur_near_enabled, dof_blur_near_distance, dof_blur_near_transition, dof_blur_near_amount, RS::Environment3DDOFBlurQuality(dof_blur_near_quality));
}

float Environment3D::get_dof_blur_near_distance() const {
	return dof_blur_near_distance;
}

void Environment3D::set_dof_blur_near_transition(float p_distance) {
	dof_blur_near_transition = p_distance;
	RS::get_singleton()->environment_set_dof_blur_near(environment, dof_blur_near_enabled, dof_blur_near_distance, dof_blur_near_transition, dof_blur_near_amount, RS::Environment3DDOFBlurQuality(dof_blur_near_quality));
}

float Environment3D::get_dof_blur_near_transition() const {
	return dof_blur_near_transition;
}

void Environment3D::set_dof_blur_near_amount(float p_amount) {
	dof_blur_near_amount = p_amount;
	RS::get_singleton()->environment_set_dof_blur_near(environment, dof_blur_near_enabled, dof_blur_near_distance, dof_blur_near_transition, dof_blur_near_amount, RS::Environment3DDOFBlurQuality(dof_blur_near_quality));
}

float Environment3D::get_dof_blur_near_amount() const {
	return dof_blur_near_amount;
}

void Environment3D::set_dof_blur_near_quality(DOFBlurQuality p_quality) {
	dof_blur_near_quality = p_quality;
	RS::get_singleton()->environment_set_dof_blur_near(environment, dof_blur_near_enabled, dof_blur_near_distance, dof_blur_near_transition, dof_blur_near_amount, RS::Environment3DDOFBlurQuality(dof_blur_near_quality));
}

Environment3D::DOFBlurQuality Environment3D::get_dof_blur_near_quality() const {
	return dof_blur_near_quality;
}

void Environment3D::set_fog_enabled(bool p_enabled) {
	fog_enabled = p_enabled;
	RS::get_singleton()->environment_set_fog(environment, fog_enabled, fog_color, fog_sun_color, fog_sun_amount);
	_change_notify();
}

bool Environment3D::is_fog_enabled() const {
	return fog_enabled;
}

void Environment3D::set_fog_color(const Color &p_color) {
	fog_color = p_color;
	RS::get_singleton()->environment_set_fog(environment, fog_enabled, fog_color, fog_sun_color, fog_sun_amount);
}
Color Environment3D::get_fog_color() const {
	return fog_color;
}

void Environment3D::set_fog_sun_color(const Color &p_color) {
	fog_sun_color = p_color;
	RS::get_singleton()->environment_set_fog(environment, fog_enabled, fog_color, fog_sun_color, fog_sun_amount);
}
Color Environment3D::get_fog_sun_color() const {
	return fog_sun_color;
}

void Environment3D::set_fog_sun_amount(float p_amount) {
	fog_sun_amount = p_amount;
	RS::get_singleton()->environment_set_fog(environment, fog_enabled, fog_color, fog_sun_color, fog_sun_amount);
}
float Environment3D::get_fog_sun_amount() const {
	return fog_sun_amount;
}

void Environment3D::set_fog_depth_enabled(bool p_enabled) {
	fog_depth_enabled = p_enabled;
	RS::get_singleton()->environment_set_fog_depth(environment, fog_depth_enabled, fog_depth_begin, fog_depth_end, fog_depth_curve, fog_transmit_enabled, fog_transmit_curve);
}
bool Environment3D::is_fog_depth_enabled() const {
	return fog_depth_enabled;
}

void Environment3D::set_fog_depth_begin(float p_distance) {
	fog_depth_begin = p_distance;
	RS::get_singleton()->environment_set_fog_depth(environment, fog_depth_enabled, fog_depth_begin, fog_depth_end, fog_depth_curve, fog_transmit_enabled, fog_transmit_curve);
}
float Environment3D::get_fog_depth_begin() const {
	return fog_depth_begin;
}

void Environment3D::set_fog_depth_end(float p_distance) {
	fog_depth_end = p_distance;
	RS::get_singleton()->environment_set_fog_depth(environment, fog_depth_enabled, fog_depth_begin, fog_depth_end, fog_depth_curve, fog_transmit_enabled, fog_transmit_curve);
}

float Environment3D::get_fog_depth_end() const {
	return fog_depth_end;
}

void Environment3D::set_fog_depth_curve(float p_curve) {
	fog_depth_curve = p_curve;
	RS::get_singleton()->environment_set_fog_depth(environment, fog_depth_enabled, fog_depth_begin, fog_depth_end, fog_depth_curve, fog_transmit_enabled, fog_transmit_curve);
}
float Environment3D::get_fog_depth_curve() const {
	return fog_depth_curve;
}

void Environment3D::set_fog_transmit_enabled(bool p_enabled) {
	fog_transmit_enabled = p_enabled;
	RS::get_singleton()->environment_set_fog_depth(environment, fog_depth_enabled, fog_depth_begin, fog_depth_end, fog_depth_curve, fog_transmit_enabled, fog_transmit_curve);
}
bool Environment3D::is_fog_transmit_enabled() const {
	return fog_transmit_enabled;
}

void Environment3D::set_fog_transmit_curve(float p_curve) {
	fog_transmit_curve = p_curve;
	RS::get_singleton()->environment_set_fog_depth(environment, fog_depth_enabled, fog_depth_begin, fog_depth_end, fog_depth_curve, fog_transmit_enabled, fog_transmit_curve);
}
float Environment3D::get_fog_transmit_curve() const {
	return fog_transmit_curve;
}

void Environment3D::set_fog_height_enabled(bool p_enabled) {
	fog_height_enabled = p_enabled;
	RS::get_singleton()->environment_set_fog_height(environment, fog_height_enabled, fog_height_min, fog_height_max, fog_height_curve);
}
bool Environment3D::is_fog_height_enabled() const {
	return fog_height_enabled;
}

void Environment3D::set_fog_height_min(float p_distance) {
	fog_height_min = p_distance;
	RS::get_singleton()->environment_set_fog_height(environment, fog_height_enabled, fog_height_min, fog_height_max, fog_height_curve);
}
float Environment3D::get_fog_height_min() const {
	return fog_height_min;
}

void Environment3D::set_fog_height_max(float p_distance) {
	fog_height_max = p_distance;
	RS::get_singleton()->environment_set_fog_height(environment, fog_height_enabled, fog_height_min, fog_height_max, fog_height_curve);
}
float Environment3D::get_fog_height_max() const {
	return fog_height_max;
}

void Environment3D::set_fog_height_curve(float p_distance) {
	fog_height_curve = p_distance;
	RS::get_singleton()->environment_set_fog_height(environment, fog_height_enabled, fog_height_min, fog_height_max, fog_height_curve);
}
float Environment3D::get_fog_height_curve() const {
	return fog_height_curve;
}

void Environment3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_background", "mode"), &Environment3D::set_background);
	ClassDB::bind_method(D_METHOD("set_sky", "sky"), &Environment3D::set_sky);
	ClassDB::bind_method(D_METHOD("set_sky_custom_fov", "scale"), &Environment3D::set_sky_custom_fov);
	ClassDB::bind_method(D_METHOD("set_sky_orientation", "orientation"), &Environment3D::set_sky_orientation);
	ClassDB::bind_method(D_METHOD("set_sky_rotation", "euler_radians"), &Environment3D::set_sky_rotation);
	ClassDB::bind_method(D_METHOD("set_sky_rotation_degrees", "euler_degrees"), &Environment3D::set_sky_rotation_degrees);
	ClassDB::bind_method(D_METHOD("set_bg_color", "color"), &Environment3D::set_bg_color);
	ClassDB::bind_method(D_METHOD("set_bg_energy", "energy"), &Environment3D::set_bg_energy);
	ClassDB::bind_method(D_METHOD("set_canvas_max_layer", "layer"), &Environment3D::set_canvas_max_layer);
	ClassDB::bind_method(D_METHOD("set_ambient_light_color", "color"), &Environment3D::set_ambient_light_color);
	ClassDB::bind_method(D_METHOD("set_ambient_light_energy", "energy"), &Environment3D::set_ambient_light_energy);
	ClassDB::bind_method(D_METHOD("set_ambient_light_sky_contribution", "energy"), &Environment3D::set_ambient_light_sky_contribution);

	ClassDB::bind_method(D_METHOD("get_background"), &Environment3D::get_background);
	ClassDB::bind_method(D_METHOD("get_sky"), &Environment3D::get_sky);
	ClassDB::bind_method(D_METHOD("get_sky_custom_fov"), &Environment3D::get_sky_custom_fov);
	ClassDB::bind_method(D_METHOD("get_sky_orientation"), &Environment3D::get_sky_orientation);
	ClassDB::bind_method(D_METHOD("get_sky_rotation"), &Environment3D::get_sky_rotation);
	ClassDB::bind_method(D_METHOD("get_sky_rotation_degrees"), &Environment3D::get_sky_rotation_degrees);
	ClassDB::bind_method(D_METHOD("get_bg_color"), &Environment3D::get_bg_color);
	ClassDB::bind_method(D_METHOD("get_bg_energy"), &Environment3D::get_bg_energy);
	ClassDB::bind_method(D_METHOD("get_canvas_max_layer"), &Environment3D::get_canvas_max_layer);
	ClassDB::bind_method(D_METHOD("get_ambient_light_color"), &Environment3D::get_ambient_light_color);
	ClassDB::bind_method(D_METHOD("get_ambient_light_energy"), &Environment3D::get_ambient_light_energy);
	ClassDB::bind_method(D_METHOD("get_ambient_light_sky_contribution"), &Environment3D::get_ambient_light_sky_contribution);

	ADD_GROUP("Background", "background_");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "background_mode", PROPERTY_HINT_ENUM, "Clear Color,Custom Color,Sky,Color+Sky,Canvas,Keep,Camera Feed"), "set_background", "get_background");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "background_sky", PROPERTY_HINT_RESOURCE_TYPE, "Sky"), "set_sky", "get_sky");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "background_sky_custom_fov", PROPERTY_HINT_RANGE, "0,180,0.1"), "set_sky_custom_fov", "get_sky_custom_fov");
	ADD_PROPERTY(PropertyInfo(Variant::BASIS, "background_sky_orientation"), "set_sky_orientation", "get_sky_orientation");
	// Only display rotation in degrees in the inspector (like in Spatial).
	// This avoids displaying the same information twice.
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "background_sky_rotation", PROPERTY_HINT_NONE, "", 0), "set_sky_rotation", "get_sky_rotation");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "background_sky_rotation_degrees", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR), "set_sky_rotation_degrees", "get_sky_rotation_degrees");
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "background_color"), "set_bg_color", "get_bg_color");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "background_energy", PROPERTY_HINT_RANGE, "0,16,0.01"), "set_bg_energy", "get_bg_energy");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "background_canvas_max_layer", PROPERTY_HINT_RANGE, "-1000,1000,1"), "set_canvas_max_layer", "get_canvas_max_layer");
	ADD_GROUP("Ambient Light", "ambient_light_");
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "ambient_light_color"), "set_ambient_light_color", "get_ambient_light_color");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "ambient_light_energy", PROPERTY_HINT_RANGE, "0,16,0.01"), "set_ambient_light_energy", "get_ambient_light_energy");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "ambient_light_sky_contribution", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_ambient_light_sky_contribution", "get_ambient_light_sky_contribution");

	ClassDB::bind_method(D_METHOD("set_fog_enabled", "enabled"), &Environment3D::set_fog_enabled);
	ClassDB::bind_method(D_METHOD("is_fog_enabled"), &Environment3D::is_fog_enabled);

	ClassDB::bind_method(D_METHOD("set_fog_color", "color"), &Environment3D::set_fog_color);
	ClassDB::bind_method(D_METHOD("get_fog_color"), &Environment3D::get_fog_color);

	ClassDB::bind_method(D_METHOD("set_fog_sun_color", "color"), &Environment3D::set_fog_sun_color);
	ClassDB::bind_method(D_METHOD("get_fog_sun_color"), &Environment3D::get_fog_sun_color);

	ClassDB::bind_method(D_METHOD("set_fog_sun_amount", "amount"), &Environment3D::set_fog_sun_amount);
	ClassDB::bind_method(D_METHOD("get_fog_sun_amount"), &Environment3D::get_fog_sun_amount);

	ClassDB::bind_method(D_METHOD("set_fog_depth_enabled", "enabled"), &Environment3D::set_fog_depth_enabled);
	ClassDB::bind_method(D_METHOD("is_fog_depth_enabled"), &Environment3D::is_fog_depth_enabled);

	ClassDB::bind_method(D_METHOD("set_fog_depth_begin", "distance"), &Environment3D::set_fog_depth_begin);
	ClassDB::bind_method(D_METHOD("get_fog_depth_begin"), &Environment3D::get_fog_depth_begin);

	ClassDB::bind_method(D_METHOD("set_fog_depth_end", "distance"), &Environment3D::set_fog_depth_end);
	ClassDB::bind_method(D_METHOD("get_fog_depth_end"), &Environment3D::get_fog_depth_end);

	ClassDB::bind_method(D_METHOD("set_fog_depth_curve", "curve"), &Environment3D::set_fog_depth_curve);
	ClassDB::bind_method(D_METHOD("get_fog_depth_curve"), &Environment3D::get_fog_depth_curve);

	ClassDB::bind_method(D_METHOD("set_fog_transmit_enabled", "enabled"), &Environment3D::set_fog_transmit_enabled);
	ClassDB::bind_method(D_METHOD("is_fog_transmit_enabled"), &Environment3D::is_fog_transmit_enabled);

	ClassDB::bind_method(D_METHOD("set_fog_transmit_curve", "curve"), &Environment3D::set_fog_transmit_curve);
	ClassDB::bind_method(D_METHOD("get_fog_transmit_curve"), &Environment3D::get_fog_transmit_curve);

	ClassDB::bind_method(D_METHOD("set_fog_height_enabled", "enabled"), &Environment3D::set_fog_height_enabled);
	ClassDB::bind_method(D_METHOD("is_fog_height_enabled"), &Environment3D::is_fog_height_enabled);

	ClassDB::bind_method(D_METHOD("set_fog_height_min", "height"), &Environment3D::set_fog_height_min);
	ClassDB::bind_method(D_METHOD("get_fog_height_min"), &Environment3D::get_fog_height_min);

	ClassDB::bind_method(D_METHOD("set_fog_height_max", "height"), &Environment3D::set_fog_height_max);
	ClassDB::bind_method(D_METHOD("get_fog_height_max"), &Environment3D::get_fog_height_max);

	ClassDB::bind_method(D_METHOD("set_fog_height_curve", "curve"), &Environment3D::set_fog_height_curve);
	ClassDB::bind_method(D_METHOD("get_fog_height_curve"), &Environment3D::get_fog_height_curve);

	ADD_GROUP("Fog", "fog_");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "fog_enabled"), "set_fog_enabled", "is_fog_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "fog_color"), "set_fog_color", "get_fog_color");
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "fog_sun_color"), "set_fog_sun_color", "get_fog_sun_color");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "fog_sun_amount", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_fog_sun_amount", "get_fog_sun_amount");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "fog_depth_enabled"), "set_fog_depth_enabled", "is_fog_depth_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "fog_depth_begin", PROPERTY_HINT_RANGE, "0,4000,0.1"), "set_fog_depth_begin", "get_fog_depth_begin");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "fog_depth_end", PROPERTY_HINT_RANGE, "0,4000,0.1,or_greater"), "set_fog_depth_end", "get_fog_depth_end");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "fog_depth_curve", PROPERTY_HINT_EXP_EASING), "set_fog_depth_curve", "get_fog_depth_curve");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "fog_transmit_enabled"), "set_fog_transmit_enabled", "is_fog_transmit_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "fog_transmit_curve", PROPERTY_HINT_EXP_EASING), "set_fog_transmit_curve", "get_fog_transmit_curve");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "fog_height_enabled"), "set_fog_height_enabled", "is_fog_height_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "fog_height_min", PROPERTY_HINT_RANGE, "-4000,4000,0.1,or_lesser,or_greater"), "set_fog_height_min", "get_fog_height_min");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "fog_height_max", PROPERTY_HINT_RANGE, "-4000,4000,0.1,or_lesser,or_greater"), "set_fog_height_max", "get_fog_height_max");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "fog_height_curve", PROPERTY_HINT_EXP_EASING), "set_fog_height_curve", "get_fog_height_curve");

	ClassDB::bind_method(D_METHOD("set_tonemapper", "mode"), &Environment3D::set_tonemapper);
	ClassDB::bind_method(D_METHOD("get_tonemapper"), &Environment3D::get_tonemapper);

	ClassDB::bind_method(D_METHOD("set_tonemap_exposure", "exposure"), &Environment3D::set_tonemap_exposure);
	ClassDB::bind_method(D_METHOD("get_tonemap_exposure"), &Environment3D::get_tonemap_exposure);

	ClassDB::bind_method(D_METHOD("set_tonemap_white", "white"), &Environment3D::set_tonemap_white);
	ClassDB::bind_method(D_METHOD("get_tonemap_white"), &Environment3D::get_tonemap_white);

	ClassDB::bind_method(D_METHOD("set_tonemap_auto_exposure", "auto_exposure"), &Environment3D::set_tonemap_auto_exposure);
	ClassDB::bind_method(D_METHOD("get_tonemap_auto_exposure"), &Environment3D::get_tonemap_auto_exposure);

	ClassDB::bind_method(D_METHOD("set_tonemap_auto_exposure_max", "exposure_max"), &Environment3D::set_tonemap_auto_exposure_max);
	ClassDB::bind_method(D_METHOD("get_tonemap_auto_exposure_max"), &Environment3D::get_tonemap_auto_exposure_max);

	ClassDB::bind_method(D_METHOD("set_tonemap_auto_exposure_min", "exposure_min"), &Environment3D::set_tonemap_auto_exposure_min);
	ClassDB::bind_method(D_METHOD("get_tonemap_auto_exposure_min"), &Environment3D::get_tonemap_auto_exposure_min);

	ClassDB::bind_method(D_METHOD("set_tonemap_auto_exposure_speed", "exposure_speed"), &Environment3D::set_tonemap_auto_exposure_speed);
	ClassDB::bind_method(D_METHOD("get_tonemap_auto_exposure_speed"), &Environment3D::get_tonemap_auto_exposure_speed);

	ClassDB::bind_method(D_METHOD("set_tonemap_auto_exposure_grey", "exposure_grey"), &Environment3D::set_tonemap_auto_exposure_grey);
	ClassDB::bind_method(D_METHOD("get_tonemap_auto_exposure_grey"), &Environment3D::get_tonemap_auto_exposure_grey);

	ADD_GROUP("Tonemap", "tonemap_");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "tonemap_mode", PROPERTY_HINT_ENUM, "Linear,Reinhard,Filmic,ACES,ACES Fitted"), "set_tonemapper", "get_tonemapper");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "tonemap_exposure", PROPERTY_HINT_RANGE, "0,16,0.01"), "set_tonemap_exposure", "get_tonemap_exposure");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "tonemap_white", PROPERTY_HINT_RANGE, "0,16,0.01"), "set_tonemap_white", "get_tonemap_white");
	ADD_GROUP("Auto Exposure", "auto_exposure_");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "auto_exposure_enabled"), "set_tonemap_auto_exposure", "get_tonemap_auto_exposure");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "auto_exposure_scale", PROPERTY_HINT_RANGE, "0.01,64,0.01"), "set_tonemap_auto_exposure_grey", "get_tonemap_auto_exposure_grey");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "auto_exposure_min_luma", PROPERTY_HINT_RANGE, "0,16,0.01"), "set_tonemap_auto_exposure_min", "get_tonemap_auto_exposure_min");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "auto_exposure_max_luma", PROPERTY_HINT_RANGE, "0,16,0.01"), "set_tonemap_auto_exposure_max", "get_tonemap_auto_exposure_max");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "auto_exposure_speed", PROPERTY_HINT_RANGE, "0.01,64,0.01"), "set_tonemap_auto_exposure_speed", "get_tonemap_auto_exposure_speed");

	ClassDB::bind_method(D_METHOD("set_ssr_enabled", "enabled"), &Environment3D::set_ssr_enabled);
	ClassDB::bind_method(D_METHOD("is_ssr_enabled"), &Environment3D::is_ssr_enabled);

	ClassDB::bind_method(D_METHOD("set_ssr_max_steps", "max_steps"), &Environment3D::set_ssr_max_steps);
	ClassDB::bind_method(D_METHOD("get_ssr_max_steps"), &Environment3D::get_ssr_max_steps);

	ClassDB::bind_method(D_METHOD("set_ssr_fade_in", "fade_in"), &Environment3D::set_ssr_fade_in);
	ClassDB::bind_method(D_METHOD("get_ssr_fade_in"), &Environment3D::get_ssr_fade_in);

	ClassDB::bind_method(D_METHOD("set_ssr_fade_out", "fade_out"), &Environment3D::set_ssr_fade_out);
	ClassDB::bind_method(D_METHOD("get_ssr_fade_out"), &Environment3D::get_ssr_fade_out);

	ClassDB::bind_method(D_METHOD("set_ssr_depth_tolerance", "depth_tolerance"), &Environment3D::set_ssr_depth_tolerance);
	ClassDB::bind_method(D_METHOD("get_ssr_depth_tolerance"), &Environment3D::get_ssr_depth_tolerance);

	ClassDB::bind_method(D_METHOD("set_ssr_rough", "rough"), &Environment3D::set_ssr_rough);
	ClassDB::bind_method(D_METHOD("is_ssr_rough"), &Environment3D::is_ssr_rough);

	ADD_GROUP("SS Reflections", "ss_reflections_");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "ss_reflections_enabled"), "set_ssr_enabled", "is_ssr_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "ss_reflections_max_steps", PROPERTY_HINT_RANGE, "1,512,1"), "set_ssr_max_steps", "get_ssr_max_steps");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "ss_reflections_fade_in", PROPERTY_HINT_EXP_EASING), "set_ssr_fade_in", "get_ssr_fade_in");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "ss_reflections_fade_out", PROPERTY_HINT_EXP_EASING), "set_ssr_fade_out", "get_ssr_fade_out");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "ss_reflections_depth_tolerance", PROPERTY_HINT_RANGE, "0.1,128,0.1"), "set_ssr_depth_tolerance", "get_ssr_depth_tolerance");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "ss_reflections_roughness"), "set_ssr_rough", "is_ssr_rough");

	ClassDB::bind_method(D_METHOD("set_ssao_enabled", "enabled"), &Environment3D::set_ssao_enabled);
	ClassDB::bind_method(D_METHOD("is_ssao_enabled"), &Environment3D::is_ssao_enabled);

	ClassDB::bind_method(D_METHOD("set_ssao_radius", "radius"), &Environment3D::set_ssao_radius);
	ClassDB::bind_method(D_METHOD("get_ssao_radius"), &Environment3D::get_ssao_radius);

	ClassDB::bind_method(D_METHOD("set_ssao_intensity", "intensity"), &Environment3D::set_ssao_intensity);
	ClassDB::bind_method(D_METHOD("get_ssao_intensity"), &Environment3D::get_ssao_intensity);

	ClassDB::bind_method(D_METHOD("set_ssao_radius2", "radius"), &Environment3D::set_ssao_radius2);
	ClassDB::bind_method(D_METHOD("get_ssao_radius2"), &Environment3D::get_ssao_radius2);

	ClassDB::bind_method(D_METHOD("set_ssao_intensity2", "intensity"), &Environment3D::set_ssao_intensity2);
	ClassDB::bind_method(D_METHOD("get_ssao_intensity2"), &Environment3D::get_ssao_intensity2);

	ClassDB::bind_method(D_METHOD("set_ssao_bias", "bias"), &Environment3D::set_ssao_bias);
	ClassDB::bind_method(D_METHOD("get_ssao_bias"), &Environment3D::get_ssao_bias);

	ClassDB::bind_method(D_METHOD("set_ssao_direct_light_affect", "amount"), &Environment3D::set_ssao_direct_light_affect);
	ClassDB::bind_method(D_METHOD("get_ssao_direct_light_affect"), &Environment3D::get_ssao_direct_light_affect);

	ClassDB::bind_method(D_METHOD("set_ssao_ao_channel_affect", "amount"), &Environment3D::set_ssao_ao_channel_affect);
	ClassDB::bind_method(D_METHOD("get_ssao_ao_channel_affect"), &Environment3D::get_ssao_ao_channel_affect);

	ClassDB::bind_method(D_METHOD("set_ssao_color", "color"), &Environment3D::set_ssao_color);
	ClassDB::bind_method(D_METHOD("get_ssao_color"), &Environment3D::get_ssao_color);

	ClassDB::bind_method(D_METHOD("set_ssao_blur", "mode"), &Environment3D::set_ssao_blur);
	ClassDB::bind_method(D_METHOD("get_ssao_blur"), &Environment3D::get_ssao_blur);

	ClassDB::bind_method(D_METHOD("set_ssao_quality", "quality"), &Environment3D::set_ssao_quality);
	ClassDB::bind_method(D_METHOD("get_ssao_quality"), &Environment3D::get_ssao_quality);

	ClassDB::bind_method(D_METHOD("set_ssao_edge_sharpness", "edge_sharpness"), &Environment3D::set_ssao_edge_sharpness);
	ClassDB::bind_method(D_METHOD("get_ssao_edge_sharpness"), &Environment3D::get_ssao_edge_sharpness);

	ADD_GROUP("SSAO", "ssao_");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "ssao_enabled"), "set_ssao_enabled", "is_ssao_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "ssao_radius", PROPERTY_HINT_RANGE, "0.1,128,0.01"), "set_ssao_radius", "get_ssao_radius");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "ssao_intensity", PROPERTY_HINT_RANGE, "0.0,128,0.01"), "set_ssao_intensity", "get_ssao_intensity");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "ssao_radius2", PROPERTY_HINT_RANGE, "0.0,128,0.01"), "set_ssao_radius2", "get_ssao_radius2");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "ssao_intensity2", PROPERTY_HINT_RANGE, "0.0,128,0.01"), "set_ssao_intensity2", "get_ssao_intensity2");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "ssao_bias", PROPERTY_HINT_RANGE, "0.001,8,0.001"), "set_ssao_bias", "get_ssao_bias");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "ssao_light_affect", PROPERTY_HINT_RANGE, "0.00,1,0.01"), "set_ssao_direct_light_affect", "get_ssao_direct_light_affect");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "ssao_ao_channel_affect", PROPERTY_HINT_RANGE, "0.00,1,0.01"), "set_ssao_ao_channel_affect", "get_ssao_ao_channel_affect");
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "ssao_color", PROPERTY_HINT_COLOR_NO_ALPHA), "set_ssao_color", "get_ssao_color");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "ssao_quality", PROPERTY_HINT_ENUM, "Low,Medium,High"), "set_ssao_quality", "get_ssao_quality");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "ssao_blur", PROPERTY_HINT_ENUM, "Disabled,1x1,2x2,3x3"), "set_ssao_blur", "get_ssao_blur");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "ssao_edge_sharpness", PROPERTY_HINT_RANGE, "0,32,0.01"), "set_ssao_edge_sharpness", "get_ssao_edge_sharpness");

	ClassDB::bind_method(D_METHOD("set_dof_blur_far_enabled", "enabled"), &Environment3D::set_dof_blur_far_enabled);
	ClassDB::bind_method(D_METHOD("is_dof_blur_far_enabled"), &Environment3D::is_dof_blur_far_enabled);

	ClassDB::bind_method(D_METHOD("set_dof_blur_far_distance", "intensity"), &Environment3D::set_dof_blur_far_distance);
	ClassDB::bind_method(D_METHOD("get_dof_blur_far_distance"), &Environment3D::get_dof_blur_far_distance);

	ClassDB::bind_method(D_METHOD("set_dof_blur_far_transition", "intensity"), &Environment3D::set_dof_blur_far_transition);
	ClassDB::bind_method(D_METHOD("get_dof_blur_far_transition"), &Environment3D::get_dof_blur_far_transition);

	ClassDB::bind_method(D_METHOD("set_dof_blur_far_amount", "intensity"), &Environment3D::set_dof_blur_far_amount);
	ClassDB::bind_method(D_METHOD("get_dof_blur_far_amount"), &Environment3D::get_dof_blur_far_amount);

	ClassDB::bind_method(D_METHOD("set_dof_blur_far_quality", "intensity"), &Environment3D::set_dof_blur_far_quality);
	ClassDB::bind_method(D_METHOD("get_dof_blur_far_quality"), &Environment3D::get_dof_blur_far_quality);

	ClassDB::bind_method(D_METHOD("set_dof_blur_near_enabled", "enabled"), &Environment3D::set_dof_blur_near_enabled);
	ClassDB::bind_method(D_METHOD("is_dof_blur_near_enabled"), &Environment3D::is_dof_blur_near_enabled);

	ClassDB::bind_method(D_METHOD("set_dof_blur_near_distance", "intensity"), &Environment3D::set_dof_blur_near_distance);
	ClassDB::bind_method(D_METHOD("get_dof_blur_near_distance"), &Environment3D::get_dof_blur_near_distance);

	ClassDB::bind_method(D_METHOD("set_dof_blur_near_transition", "intensity"), &Environment3D::set_dof_blur_near_transition);
	ClassDB::bind_method(D_METHOD("get_dof_blur_near_transition"), &Environment3D::get_dof_blur_near_transition);

	ClassDB::bind_method(D_METHOD("set_dof_blur_near_amount", "intensity"), &Environment3D::set_dof_blur_near_amount);
	ClassDB::bind_method(D_METHOD("get_dof_blur_near_amount"), &Environment3D::get_dof_blur_near_amount);

	ClassDB::bind_method(D_METHOD("set_dof_blur_near_quality", "level"), &Environment3D::set_dof_blur_near_quality);
	ClassDB::bind_method(D_METHOD("get_dof_blur_near_quality"), &Environment3D::get_dof_blur_near_quality);

	ADD_GROUP("DOF Far Blur", "dof_blur_far_");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "dof_blur_far_enabled"), "set_dof_blur_far_enabled", "is_dof_blur_far_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "dof_blur_far_distance", PROPERTY_HINT_EXP_RANGE, "0.01,8192,0.01"), "set_dof_blur_far_distance", "get_dof_blur_far_distance");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "dof_blur_far_transition", PROPERTY_HINT_EXP_RANGE, "0.01,8192,0.01"), "set_dof_blur_far_transition", "get_dof_blur_far_transition");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "dof_blur_far_amount", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_dof_blur_far_amount", "get_dof_blur_far_amount");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "dof_blur_far_quality", PROPERTY_HINT_ENUM, "Low,Medium,High"), "set_dof_blur_far_quality", "get_dof_blur_far_quality");

	ADD_GROUP("DOF Near Blur", "dof_blur_near_");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "dof_blur_near_enabled"), "set_dof_blur_near_enabled", "is_dof_blur_near_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "dof_blur_near_distance", PROPERTY_HINT_EXP_RANGE, "0.01,8192,0.01"), "set_dof_blur_near_distance", "get_dof_blur_near_distance");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "dof_blur_near_transition", PROPERTY_HINT_EXP_RANGE, "0.01,8192,0.01"), "set_dof_blur_near_transition", "get_dof_blur_near_transition");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "dof_blur_near_amount", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_dof_blur_near_amount", "get_dof_blur_near_amount");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "dof_blur_near_quality", PROPERTY_HINT_ENUM, "Low,Medium,High"), "set_dof_blur_near_quality", "get_dof_blur_near_quality");

	ClassDB::bind_method(D_METHOD("set_glow_enabled", "enabled"), &Environment3D::set_glow_enabled);
	ClassDB::bind_method(D_METHOD("is_glow_enabled"), &Environment3D::is_glow_enabled);

	ClassDB::bind_method(D_METHOD("set_glow_level", "idx", "enabled"), &Environment3D::set_glow_level);
	ClassDB::bind_method(D_METHOD("is_glow_level_enabled", "idx"), &Environment3D::is_glow_level_enabled);

	ClassDB::bind_method(D_METHOD("set_glow_intensity", "intensity"), &Environment3D::set_glow_intensity);
	ClassDB::bind_method(D_METHOD("get_glow_intensity"), &Environment3D::get_glow_intensity);

	ClassDB::bind_method(D_METHOD("set_glow_strength", "strength"), &Environment3D::set_glow_strength);
	ClassDB::bind_method(D_METHOD("get_glow_strength"), &Environment3D::get_glow_strength);

	ClassDB::bind_method(D_METHOD("set_glow_bloom", "amount"), &Environment3D::set_glow_bloom);
	ClassDB::bind_method(D_METHOD("get_glow_bloom"), &Environment3D::get_glow_bloom);

	ClassDB::bind_method(D_METHOD("set_glow_blend_mode", "mode"), &Environment3D::set_glow_blend_mode);
	ClassDB::bind_method(D_METHOD("get_glow_blend_mode"), &Environment3D::get_glow_blend_mode);

	ClassDB::bind_method(D_METHOD("set_glow_hdr_bleed_threshold", "threshold"), &Environment3D::set_glow_hdr_bleed_threshold);
	ClassDB::bind_method(D_METHOD("get_glow_hdr_bleed_threshold"), &Environment3D::get_glow_hdr_bleed_threshold);

	ClassDB::bind_method(D_METHOD("set_glow_hdr_luminance_cap", "amount"), &Environment3D::set_glow_hdr_luminance_cap);
	ClassDB::bind_method(D_METHOD("get_glow_hdr_luminance_cap"), &Environment3D::get_glow_hdr_luminance_cap);

	ClassDB::bind_method(D_METHOD("set_glow_hdr_bleed_scale", "scale"), &Environment3D::set_glow_hdr_bleed_scale);
	ClassDB::bind_method(D_METHOD("get_glow_hdr_bleed_scale"), &Environment3D::get_glow_hdr_bleed_scale);

	ClassDB::bind_method(D_METHOD("set_glow_bicubic_upscale", "enabled"), &Environment3D::set_glow_bicubic_upscale);
	ClassDB::bind_method(D_METHOD("is_glow_bicubic_upscale_enabled"), &Environment3D::is_glow_bicubic_upscale_enabled);

	ClassDB::bind_method(D_METHOD("set_glow_high_quality", "enabled"), &Environment3D::set_glow_high_quality);
	ClassDB::bind_method(D_METHOD("is_glow_high_quality_enabled"), &Environment3D::is_glow_high_quality_enabled);

	ClassDB::bind_method(D_METHOD("set_glow_map_strength", "strength"), &Environment3D::set_glow_map_strength);
	ClassDB::bind_method(D_METHOD("get_glow_map_strength"), &Environment3D::get_glow_map_strength);

	ClassDB::bind_method(D_METHOD("set_glow_map", "mode"), &Environment3D::set_glow_map);
	ClassDB::bind_method(D_METHOD("get_glow_map"), &Environment3D::get_glow_map);

	ADD_GROUP("Glow", "glow_");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "glow_enabled"), "set_glow_enabled", "is_glow_enabled");
	ADD_PROPERTYI(PropertyInfo(Variant::BOOL, "glow_levels/1"), "set_glow_level", "is_glow_level_enabled", 0);
	ADD_PROPERTYI(PropertyInfo(Variant::BOOL, "glow_levels/2"), "set_glow_level", "is_glow_level_enabled", 1);
	ADD_PROPERTYI(PropertyInfo(Variant::BOOL, "glow_levels/3"), "set_glow_level", "is_glow_level_enabled", 2);
	ADD_PROPERTYI(PropertyInfo(Variant::BOOL, "glow_levels/4"), "set_glow_level", "is_glow_level_enabled", 3);
	ADD_PROPERTYI(PropertyInfo(Variant::BOOL, "glow_levels/5"), "set_glow_level", "is_glow_level_enabled", 4);
	ADD_PROPERTYI(PropertyInfo(Variant::BOOL, "glow_levels/6"), "set_glow_level", "is_glow_level_enabled", 5);
	ADD_PROPERTYI(PropertyInfo(Variant::BOOL, "glow_levels/7"), "set_glow_level", "is_glow_level_enabled", 6);

	ADD_PROPERTY(PropertyInfo(Variant::REAL, "glow_intensity", PROPERTY_HINT_RANGE, "0.0,8.0,0.01"), "set_glow_intensity", "get_glow_intensity");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "glow_strength", PROPERTY_HINT_RANGE, "0.0,2.0,0.01"), "set_glow_strength", "get_glow_strength");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "glow_bloom", PROPERTY_HINT_RANGE, "0.0,1.0,0.01"), "set_glow_bloom", "get_glow_bloom");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "glow_blend_mode", PROPERTY_HINT_ENUM, "Additive,Screen,Softlight,Replace"), "set_glow_blend_mode", "get_glow_blend_mode");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "glow_hdr_threshold", PROPERTY_HINT_RANGE, "0.0,4.0,0.01"), "set_glow_hdr_bleed_threshold", "get_glow_hdr_bleed_threshold");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "glow_hdr_luminance_cap", PROPERTY_HINT_RANGE, "0.0,256.0,0.01"), "set_glow_hdr_luminance_cap", "get_glow_hdr_luminance_cap");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "glow_hdr_scale", PROPERTY_HINT_RANGE, "0.0,4.0,0.01"), "set_glow_hdr_bleed_scale", "get_glow_hdr_bleed_scale");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "glow_bicubic_upscale"), "set_glow_bicubic_upscale", "is_glow_bicubic_upscale_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "glow_high_quality"), "set_glow_high_quality", "is_glow_high_quality_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "glow_map_strength", PROPERTY_HINT_RANGE, "0.0,1.0,0.01"), "set_glow_map_strength", "get_glow_map_strength");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "glow_map", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_glow_map", "get_glow_map");

	ClassDB::bind_method(D_METHOD("set_adjustment_enable", "enabled"), &Environment3D::set_adjustment_enable);
	ClassDB::bind_method(D_METHOD("is_adjustment_enabled"), &Environment3D::is_adjustment_enabled);

	ClassDB::bind_method(D_METHOD("set_adjustment_brightness", "brightness"), &Environment3D::set_adjustment_brightness);
	ClassDB::bind_method(D_METHOD("get_adjustment_brightness"), &Environment3D::get_adjustment_brightness);

	ClassDB::bind_method(D_METHOD("set_adjustment_contrast", "contrast"), &Environment3D::set_adjustment_contrast);
	ClassDB::bind_method(D_METHOD("get_adjustment_contrast"), &Environment3D::get_adjustment_contrast);

	ClassDB::bind_method(D_METHOD("set_adjustment_saturation", "saturation"), &Environment3D::set_adjustment_saturation);
	ClassDB::bind_method(D_METHOD("get_adjustment_saturation"), &Environment3D::get_adjustment_saturation);

	ClassDB::bind_method(D_METHOD("set_adjustment_color_correction", "color_correction"), &Environment3D::set_adjustment_color_correction);
	ClassDB::bind_method(D_METHOD("get_adjustment_color_correction"), &Environment3D::get_adjustment_color_correction);

	ADD_GROUP("Adjustments", "adjustment_");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "adjustment_enabled"), "set_adjustment_enable", "is_adjustment_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "adjustment_brightness", PROPERTY_HINT_RANGE, "0.01,8,0.01"), "set_adjustment_brightness", "get_adjustment_brightness");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "adjustment_contrast", PROPERTY_HINT_RANGE, "0.01,8,0.01"), "set_adjustment_contrast", "get_adjustment_contrast");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "adjustment_saturation", PROPERTY_HINT_RANGE, "0.01,8,0.01"), "set_adjustment_saturation", "get_adjustment_saturation");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "adjustment_color_correction", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_adjustment_color_correction", "get_adjustment_color_correction");

	BIND_ENUM_CONSTANT(BG_KEEP);
	BIND_ENUM_CONSTANT(BG_CLEAR_COLOR);
	BIND_ENUM_CONSTANT(BG_COLOR);
	BIND_ENUM_CONSTANT(BG_SKY);
	BIND_ENUM_CONSTANT(BG_COLOR_SKY);
	BIND_ENUM_CONSTANT(BG_CANVAS);
	BIND_ENUM_CONSTANT(BG_CAMERA_FEED);
	BIND_ENUM_CONSTANT(BG_MAX);

	BIND_ENUM_CONSTANT(GLOW_BLEND_MODE_ADDITIVE);
	BIND_ENUM_CONSTANT(GLOW_BLEND_MODE_SCREEN);
	BIND_ENUM_CONSTANT(GLOW_BLEND_MODE_SOFTLIGHT);
	BIND_ENUM_CONSTANT(GLOW_BLEND_MODE_REPLACE);

	BIND_ENUM_CONSTANT(TONE_MAPPER_LINEAR);
	BIND_ENUM_CONSTANT(TONE_MAPPER_REINHARDT);
	BIND_ENUM_CONSTANT(TONE_MAPPER_FILMIC);
	BIND_ENUM_CONSTANT(TONE_MAPPER_ACES);
	BIND_ENUM_CONSTANT(TONE_MAPPER_ACES_FITTED);

	BIND_ENUM_CONSTANT(DOF_BLUR_QUALITY_LOW);
	BIND_ENUM_CONSTANT(DOF_BLUR_QUALITY_MEDIUM);
	BIND_ENUM_CONSTANT(DOF_BLUR_QUALITY_HIGH);

	BIND_ENUM_CONSTANT(SSAO_BLUR_DISABLED);
	BIND_ENUM_CONSTANT(SSAO_BLUR_1x1);
	BIND_ENUM_CONSTANT(SSAO_BLUR_2x2);
	BIND_ENUM_CONSTANT(SSAO_BLUR_3x3);

	BIND_ENUM_CONSTANT(SSAO_QUALITY_LOW);
	BIND_ENUM_CONSTANT(SSAO_QUALITY_MEDIUM);
	BIND_ENUM_CONSTANT(SSAO_QUALITY_HIGH);
}

Environment3D::Environment3D() :
		bg_mode(BG_CLEAR_COLOR),
		tone_mapper(TONE_MAPPER_LINEAR),
		ssao_blur(SSAO_BLUR_3x3),
		ssao_quality(SSAO_QUALITY_MEDIUM),
		glow_blend_mode(GLOW_BLEND_MODE_ADDITIVE),
		dof_blur_far_quality(DOF_BLUR_QUALITY_LOW),
		dof_blur_near_quality(DOF_BLUR_QUALITY_LOW) {
	environment = RID_PRIME(RS::get_singleton()->environment_create());

	bg_mode = BG_CLEAR_COLOR;
	bg_sky_custom_fov = 0;
	bg_sky_orientation = Basis();
	bg_energy = 1.0;
	bg_canvas_max_layer = 0;
	ambient_energy = 1.0;
	//ambient_sky_contribution = 1.0;
	set_ambient_light_sky_contribution(1.0);

	tone_mapper = TONE_MAPPER_LINEAR;
	tonemap_exposure = 1.0;
	tonemap_white = 1.0;
	tonemap_auto_exposure = false;
	tonemap_auto_exposure_max = 8;
	tonemap_auto_exposure_min = 0.05;
	tonemap_auto_exposure_speed = 0.5;
	tonemap_auto_exposure_grey = 0.4;

	set_tonemapper(tone_mapper); //update

	adjustment_enabled = false;
	adjustment_contrast = 1.0;
	adjustment_saturation = 1.0;
	adjustment_brightness = 1.0;

	set_adjustment_enable(adjustment_enabled); //update

	ssr_enabled = false;
	ssr_max_steps = 64;
	ssr_fade_in = 0.15;
	ssr_fade_out = 2.0;
	ssr_depth_tolerance = 0.2;
	ssr_roughness = true;

	ssao_enabled = false;
	ssao_radius = 1;
	ssao_intensity = 1;
	ssao_radius2 = 0;
	ssao_intensity2 = 1;
	ssao_bias = 0.01;
	ssao_direct_light_affect = 0.0;
	ssao_ao_channel_affect = 0.0;
	ssao_blur = SSAO_BLUR_3x3;
	set_ssao_edge_sharpness(4);
	set_ssao_quality(SSAO_QUALITY_MEDIUM);

	glow_enabled = false;
	glow_levels = (1 << 2) | (1 << 4);
	glow_intensity = 0.8;
	glow_strength = 1.0;
	glow_bloom = 0.0;
	glow_blend_mode = GLOW_BLEND_MODE_SOFTLIGHT;
	glow_hdr_bleed_threshold = 1.0;
	glow_hdr_luminance_cap = 12.0;
	glow_hdr_bleed_scale = 2.0;
	glow_bicubic_upscale = false;
	glow_high_quality = false;
	glow_map_strength = 0.8f;

	dof_blur_far_enabled = false;
	dof_blur_far_distance = 10;
	dof_blur_far_transition = 5;
	dof_blur_far_amount = 0.1;
	dof_blur_far_quality = DOF_BLUR_QUALITY_MEDIUM;

	dof_blur_near_enabled = false;
	dof_blur_near_distance = 2;
	dof_blur_near_transition = 1;
	dof_blur_near_amount = 0.1;
	dof_blur_near_quality = DOF_BLUR_QUALITY_MEDIUM;

	fog_enabled = false;
	fog_color = Color(0.5, 0.5, 0.5);
	fog_sun_color = Color(0.8, 0.8, 0.0);
	fog_sun_amount = 0;

	fog_depth_enabled = true;

	fog_depth_begin = 10;
	fog_depth_end = 100;
	fog_depth_curve = 1;

	fog_transmit_enabled = false;
	fog_transmit_curve = 1;

	fog_height_enabled = false;
	fog_height_min = 10;
	fog_height_max = 0;
	fog_height_curve = 1;

	set_fog_color(Color(0.5, 0.6, 0.7));
	set_fog_sun_color(Color(1.0, 0.9, 0.7));
}

Environment3D::~Environment3D() {
	RS::get_singleton()->free(environment);
}

