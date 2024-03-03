/*************************************************************************/
/*  editor_zoom_widget.cpp                                               */
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

#include "editor_zoom_widget.h"

#include "core/input/input.h"
#include "core/os/keyboard.h"
#include "editor/editor_scale.h"
#include "editor/editor_settings.h"
#include "scene/gui/button.h"

void EditorZoomWidget::_update_zoom_label() {
	String zoom_text;
	// The zoom level displayed is relative to the editor scale
	// (like in most image editors). Its lower bound is clamped to 1 as some people
	// lower the editor scale to increase the available real estate,
	// even if their display doesn't have a particularly low DPI.
	if (zoom >= 10) {
		// Don't show a decimal when the zoom level is higher than 1000 %.
		zoom_text = rtos(Math::round((zoom / MAX(1, EDSCALE)) * 100)) + " %";
	} else {
		zoom_text = itos(static_cast<int>((zoom / MAX(1, EDSCALE)) * 100)) + " %";
	}

	zoom_reset->set_text(zoom_text);
}

void EditorZoomWidget::_button_zoom_minus() {
	set_zoom_by_increments(-6, Input::get_singleton()->is_key_pressed(KEY_ALT));
	emit_signal("zoom_changed", zoom);
}

void EditorZoomWidget::_button_zoom_reset() {
	set_zoom(1.0 * MAX(1, EDSCALE));
	emit_signal("zoom_changed", zoom);
}

void EditorZoomWidget::_button_zoom_plus() {
	set_zoom_by_increments(6, Input::get_singleton()->is_key_pressed(KEY_ALT));
	emit_signal("zoom_changed", zoom);
}

float EditorZoomWidget::get_zoom() {
	return zoom;
}

void EditorZoomWidget::set_zoom(float p_zoom) {
	if (zoom_limits_enabled) {
		float new_zoom = CLAMP(p_zoom, min_zoom, max_zoom);

		if (new_zoom != zoom) {
			zoom = new_zoom;
			_update_zoom_label();
		}
	} else {
		if (p_zoom > 0 && p_zoom != zoom) {
			zoom = p_zoom;
			_update_zoom_label();
		}
	}
}

float EditorZoomWidget::get_min_zoom() {
	return min_zoom;
}

float EditorZoomWidget::get_max_zoom() {
	return max_zoom;
}

void EditorZoomWidget::setup_zoom_limits(float p_min, float p_max) {
	ERR_FAIL_COND(p_min < 0 || p_min > p_max);

	zoom_limits_enabled = true;

	min_zoom = p_min;
	max_zoom = p_max;

	if (zoom > max_zoom) {
		set_zoom(max_zoom);
		emit_signal("zoom_changed", zoom);
	} else if (zoom < min_zoom) {
		set_zoom(min_zoom);
		emit_signal("zoom_changed", zoom);
	}
}

void EditorZoomWidget::set_enable_zoom_limits(bool p_enabled) {
	zoom_limits_enabled = p_enabled;
}

void EditorZoomWidget::set_zoom_by_increments(int p_increment_count, bool p_integer_only) {
	// Remove editor scale from the index computation.
	const float zoom_noscale = zoom / MAX(1, EDSCALE);

	if (p_integer_only) {
		// Only visit integer scaling factors above 100%, and fractions with an integer denominator below 100%
		// (1/2 = 50%, 1/3 = 33.33%, 1/4 = 25%, …).
		// This is useful when working on pixel art projects to avoid distortion.
		// This algorithm is designed to handle fractional start zoom values correctly
		// (e.g. 190% will zoom up to 200% and down to 100%).
		if (zoom_noscale + p_increment_count * 0.001 >= 1.0 - CMP_EPSILON) {
			// New zoom is certain to be above 100%.
			if (p_increment_count >= 1) {
				// Zooming.
				set_zoom(Math::floor(zoom_noscale + p_increment_count) * MAX(1, EDSCALE));
			} else {
				// Dezooming.
				set_zoom(Math::ceil(zoom_noscale + p_increment_count) * MAX(1, EDSCALE));
			}
		} else {
			if (p_increment_count >= 1) {
				// Zooming. Convert the current zoom into a denominator.
				float new_zoom = 1.0 / Math::ceil(1.0 / zoom_noscale - p_increment_count);
				if (Math::is_equal_approx(zoom_noscale, new_zoom)) {
					// New zoom is identical to the old zoom, so try again.
					// This can happen due to floating-point precision issues.
					new_zoom = 1.0 / Math::ceil(1.0 / zoom_noscale - p_increment_count - 1);
				}
				set_zoom(new_zoom * MAX(1, EDSCALE));
			} else {
				// Dezooming. Convert the current zoom into a denominator.
				float new_zoom = 1.0 / Math::floor(1.0 / zoom_noscale - p_increment_count);
				if (Math::is_equal_approx(zoom_noscale, new_zoom)) {
					// New zoom is identical to the old zoom, so try again.
					// This can happen due to floating-point precision issues.
					new_zoom = 1.0 / Math::floor(1.0 / zoom_noscale - p_increment_count + 1);
				}
				set_zoom(new_zoom * MAX(1, EDSCALE));
			}
		}
	} else {
		// Base increment factor defined as the twelveth root of two.
		// This allow a smooth geometric evolution of the zoom, with the advantage of
		// visiting all integer power of two scale factors.
		// note: this is analogous to the 'semitones' interval in the music world
		// In order to avoid numerical imprecisions, we compute and edit a zoom index
		// with the following relation: zoom = 2 ^ (index / 12)

		if (zoom < CMP_EPSILON || p_increment_count == 0) {
			return;
		}

		// zoom = 2**(index/12) => log2(zoom) = index/12
		float closest_zoom_index = Math::round(Math::log(zoom_noscale) * 12.f / Math::log(2.f));

		float new_zoom_index = closest_zoom_index + p_increment_count;
		float new_zoom = Math::pow(2.f, new_zoom_index / 12.f);

		// Restore Editor scale transformation
		new_zoom *= MAX(1, EDSCALE);

		set_zoom(new_zoom);
	}
}

void EditorZoomWidget::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE:
		case NOTIFICATION_THEME_CHANGED: {
			zoom_minus->set_icon(get_theme_icon("ZoomLess", "EditorIcons"));
			zoom_plus->set_icon(get_theme_icon("ZoomMore", "EditorIcons"));
		} break;
	}
}

void EditorZoomWidget::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_zoom", "zoom"), &EditorZoomWidget::set_zoom);
	ClassDB::bind_method(D_METHOD("get_zoom"), &EditorZoomWidget::get_zoom);
	ClassDB::bind_method(D_METHOD("set_zoom_by_increments", "increment", "integer_only"), &EditorZoomWidget::set_zoom_by_increments);

	ClassDB::bind_method(D_METHOD("_button_zoom_minus"), &EditorZoomWidget::_button_zoom_minus);
	ClassDB::bind_method(D_METHOD("_button_zoom_reset"), &EditorZoomWidget::_button_zoom_reset);
	ClassDB::bind_method(D_METHOD("_button_zoom_plus"), &EditorZoomWidget::_button_zoom_plus);

	ADD_PROPERTY(PropertyInfo(Variant::REAL, "zoom"), "set_zoom", "get_zoom");

	ADD_SIGNAL(MethodInfo("zoom_changed", PropertyInfo(Variant::REAL, "zoom")));
}

void EditorZoomWidget::set_shortcut_context(Node *p_node) const {
	zoom_minus->set_shortcut_context(p_node);
	zoom_plus->set_shortcut_context(p_node);
	zoom_reset->set_shortcut_context(p_node);
}

EditorZoomWidget::EditorZoomWidget() {
	// Zoom buttons
	zoom_minus = memnew(Button);
	zoom_minus->set_flat(true);
	add_child(zoom_minus);
	zoom_minus->connect("pressed", this, "_button_zoom_minus");
	zoom_minus->set_shortcut(ED_SHORTCUT("canvas_item_editor/zoom_minus", TTR("Zoom Out"), KEY_MASK_CMD | KEY_MINUS));
	zoom_minus->set_shortcut_context(this);
	zoom_minus->set_focus_mode(FOCUS_NONE);

	zoom_reset = memnew(Button);
	zoom_reset->set_flat(true);
	add_child(zoom_reset);
	zoom_reset->add_theme_constant_override("outline_size", 1);
	zoom_reset->add_theme_color_override("font_outline_color", Color(0, 0, 0));
	zoom_reset->add_theme_color_override("font_color", Color(1, 1, 1));
	zoom_reset->connect("pressed", this, "_button_zoom_reset");
	zoom_reset->set_shortcut(ED_SHORTCUT("canvas_item_editor/zoom_reset", TTR("Zoom Reset"), KEY_MASK_CMD | KEY_0));
	zoom_reset->set_shortcut_context(this);
	zoom_reset->set_focus_mode(FOCUS_NONE);
	zoom_reset->set_text_align(Button::ALIGN_CENTER);

	// Prevent the button's size from changing when the text size changes
	zoom_reset->set_custom_minimum_size(Size2(75 * EDSCALE, 0));

	zoom_plus = memnew(Button);
	zoom_plus->set_flat(true);
	add_child(zoom_plus);
	zoom_plus->connect("pressed", this, "_button_zoom_plus");
	zoom_plus->set_shortcut(ED_SHORTCUT("canvas_item_editor/zoom_plus", TTR("Zoom In"), KEY_MASK_CMD | KEY_EQUAL)); // Usually direct access key for PLUS
	zoom_plus->set_shortcut_context(this);
	zoom_plus->set_focus_mode(FOCUS_NONE);

	_update_zoom_label();

	add_theme_constant_override("separation", Math::round(-8 * EDSCALE));
}
