/*
Copyright (c) 2019 Flairieve
Copyright (c) 2020-2022 cobrapitz
Copyright (c) 2022 Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "paint_layer_button.h"

#include "core/io/image.h"
#include "scene/resources/style_box.h"
#include "scene/resources/texture.h"

#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/check_button.h"
#include "scene/gui/margin_container.h"
#include "scene/gui/texture_button.h"

#include "../icons/paint_icons.h"

static float scale = 1;

template <class T>
static Ref<Texture> make_icon(T p_src) {
	Ref<ImageTexture> texture(memnew(ImageTexture));
	Ref<Image> img = memnew(Image(p_src));
	if (scale > 1) {
		Size2 orig_size = Size2(img->get_width(), img->get_height());

		img->convert(Image::FORMAT_RGBA8);
		img->expand_x2_hq2x();
		if (scale != 2.0) {
			img->resize(orig_size.x * scale, orig_size.y * scale);
		}
	} else if (scale < 1) {
		Size2 orig_size = Size2(img->get_width(), img->get_height());
		img->convert(Image::FORMAT_RGBA8);
		img->resize(orig_size.x * scale, orig_size.y * scale);
	}
	texture->create_from_image(img, ImageTexture::FLAG_FILTER);

	return texture;
}

PaintLayerButton::PaintLayerButton() {
	set_custom_minimum_size(Size2(0, 32));
	set_mouse_filter(Control::MOUSE_FILTER_IGNORE);

	Ref<StyleBoxFlat> style_box;
	style_box.instance();
	style_box->set("bg_color", Color(0.35, 0.5, 0.77));
	set("custom_styles/panel", style_box);

	HBoxContainer *main_box_container = memnew(HBoxContainer);
	add_child(main_box_container);

	MarginContainer *left_main_container = memnew(MarginContainer);
	left_main_container->set("custom_constants/margin_right", 2);
	left_main_container->set("custom_constants/margin_top", 2);
	left_main_container->set("custom_constants/margin_left", 2);
	left_main_container->set("custom_constants/margin_bottom", 2);
	left_main_container->set_h_size_flags(SIZE_EXPAND_FILL);
	left_main_container->set_v_size_flags(SIZE_EXPAND_FILL);
	main_box_container->add_child(left_main_container);

	// Layer Button
	layer_button = memnew(Button);
	layer_button->set_text("Layer 1");
	layer_button->set_text_align(Button::ALIGN_RIGHT);
	layer_button->set_h_size_flags(SIZE_EXPAND_FILL);
	layer_button->set_v_size_flags(SIZE_EXPAND_FILL);

	style_box.instance();
	style_box->set("bg_color", Color(0.25, 0.25, 0.25));
	layer_button->set("custom_styles/hover", style_box);
	style_box.instance();
	style_box->set("bg_color", Color(0.25, 0.25, 0.25));
	layer_button->set("custom_styles/pressed", style_box);
	style_box.instance();
	style_box->set("bg_color", Color(0.25, 0.25, 0.25));
	layer_button->set("custom_styles/focus", style_box);
	style_box.instance();
	style_box->set("bg_color", Color(0.25, 0.25, 0.25));
	layer_button->set("custom_styles/disabled", style_box);
	style_box.instance();
	style_box->set("bg_color", Color(0.25, 0.25, 0.25));
	layer_button->set("custom_styles/normal", style_box);
	left_main_container->add_child(layer_button);

	HBoxContainer *check_container = memnew(HBoxContainer);
	check_container->set_mouse_filter(Control::MOUSE_FILTER_IGNORE);
	left_main_container->add_child(check_container);

	// Visible Button
	visible_button = memnew(CheckButton);
	visible_button->set_pressed(true);
	visible_button->set("custom_icons/off", make_icon(minidotta_invis_png));
	visible_button->set("custom_icons/on", make_icon(minidotta_png));
	style_box.instance();
	style_box->set("bg_color", Color(0.6, 0.6, 0.6));
	layer_button->set("custom_styles/normal", style_box);
	check_container->add_child(visible_button);

	// Lock Button
	lock_button = memnew(CheckButton);
	lock_button->set_pressed(false);
	lock_button->set("custom_icons/off", make_icon(unlock_layer_png));
	lock_button->set("custom_icons/on", make_icon(lock_layer_1_png));
	style_box.instance();
	style_box->set("bg_color", Color(0.6, 0.6, 0.6));
	layer_button->set("custom_styles/normal", style_box);
	check_container->add_child(lock_button);

	// Right side
	VBoxContainer *right_main_container = memnew(VBoxContainer);
	main_box_container->add_child(right_main_container);

	up_button = memnew(TextureButton);
	up_button->set_expand(true);
	up_button->set_stretch_mode(TextureButton::STRETCH_KEEP_CENTERED);
	up_button->set_h_size_flags(SIZE_EXPAND_FILL);
	up_button->set_v_size_flags(SIZE_EXPAND_FILL);
	up_button->set_normal_texture(make_icon(arrow_up_png));
	up_button->set_pressed_texture(make_icon(minidotta_png));
	up_button->set_custom_minimum_size(Vector2(15, 15));
	right_main_container->add_child(up_button);

	down_button = memnew(TextureButton);
	down_button->set_expand(true);
	down_button->set_stretch_mode(TextureButton::STRETCH_KEEP_CENTERED);
	down_button->set_h_size_flags(SIZE_EXPAND_FILL);
	down_button->set_v_size_flags(SIZE_EXPAND_FILL);
	down_button->set_normal_texture(make_icon(arrow_down_png));
	down_button->set_pressed_texture(make_icon(minidotta_png));
	down_button->set_custom_minimum_size(Vector2(15, 15));
	right_main_container->add_child(down_button);
}

PaintLayerButton::~PaintLayerButton() {
}

void PaintLayerButton::_bind_methods() {
}
