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

#include "paint_canvas.h"

#include "paint_canvas_background.h"
#include "paint_canvas_outline.h"
#include "paint_utilities.h"
#include "paint_visual_grid.h"
#include "scene/gui/control.h"
#include "scene/gui/texture_rect.h"

#include "paint_canvas_layer.h"

#include "icons/paint_icons.h"

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

void PaintCanvas::_process(float delta) {
	if (!is_visible_in_tree()) {
		return;
	}

	Vector2 mouse_position = get_local_mouse_position();
	Rect2 rect = Rect2(Vector2(0, 0), get_size());
	mouse_in_region = rect.has_point(mouse_position);
}
void PaintCanvas::_draw() {
	for (int i = 0; i < layers.size(); ++i) {
		Ref<PaintCanvasLayer> layer = layers[i];

		ERR_CONTINUE(!layer.is_valid());

		layer->update_texture();
	}

	preview_layer->update_texture();
	tool_layer->update_texture();
}

int PaintCanvas::get_pixel_size() const {
	return _pixel_size;
}

void PaintCanvas::set_pixel_size(const int size) {
	_pixel_size = size;
	set_grid_size(_grid_size);
	set_big_grid_size(_big_grid_size);
	set_canvas_width(_canvas_width);
	set_canvas_height(_canvas_height);
}

int PaintCanvas::get_grid_size() const {
	return _grid_size;
}
void PaintCanvas::set_grid_size(const int size) {
	_grid_size = size;

	if (grid) {
		int s = size * _pixel_size;
		grid->set_grid_size(s);
	}
}

int PaintCanvas::get_big_grid_size() const {
	return _big_grid_size;
}
void PaintCanvas::set_big_grid_size(const int size) {
	_big_grid_size = size;

	if (big_grid) {
		int s = size * _pixel_size;
		big_grid->set_size(Size2(s, s));
	}
}

int PaintCanvas::get_canvas_width() const {
	return _canvas_width;
}
void PaintCanvas::set_canvas_width(const int val) {
	_canvas_width = val;

	Size2 s = get_size();
	s.x = _canvas_width * _pixel_size;
	set_size(s);
}

int PaintCanvas::get_canvas_height() const {
	return _canvas_height;
}
void PaintCanvas::set_canvas_height(const int val) {
	_canvas_height = val;

	Size2 s = get_size();
	s.y = _canvas_height * _pixel_size;
	set_size(s);
}
void PaintCanvas::toggle_alpha_locked(const String &layer_name) {
	Ref<PaintCanvasLayer> layer = find_layer_by_name(layer_name);

	if (layer.is_valid()) {
		layer->toggle_alpha_locked();
	}
}
bool PaintCanvas::is_alpha_locked() {
	if (!active_layer.is_valid()) {
		return false;
	}

	return active_layer->alpha_locked;
}
Rect2 PaintCanvas::get_content_margin() {
	Rect2 rect = Rect2(999999, 999999, -999999, -999999);

	for (int i = 0; i < layers.size(); ++i) {
		Ref<PaintCanvasLayer> l = layers[i];

		ERR_CONTINUE(!l.is_valid());

		Rect2 r = l->image->get_used_rect();

		if (r.position.x < rect.position.x) {
			rect.position.x = r.position.x;
		}

		if (r.position.y < rect.position.y) {
			rect.position.y = r.position.y;
		}

		if (r.size.x > rect.size.x) {
			rect.size.x = r.size.x;
		}

		if (r.size.y > rect.size.y) {
			rect.size.y = r.size.y;
		}
	}

	return rect;
}
void PaintCanvas::crop_to_content() {
	/*
	var rect = get_content_margin()

	#print(rect)

	for layer in layers:
		layer.image

#	set_canvas_width(rect.size.x)
#	set_canvas_height(rect.size.x)

#	preview_layer.resize(width, height)
#	tool_layer.resize(width, height)
#	for layer in layers:
#		layer.resize(width, height)
	*/
}
Ref<PaintCanvasLayer> PaintCanvas::get_active_layer() {
	return active_layer;
}
Ref<PaintCanvasLayer> PaintCanvas::get_preview_layer() {
	return preview_layer;
}
void PaintCanvas::clear_active_layer() {
	if (active_layer.is_valid()) {
		active_layer->clear();
	}
}
void PaintCanvas::clear_preview_layer() {
	preview_layer->clear();
}

void PaintCanvas::clear_layer(const String &layer_name) {
	for (int i = 0; i < layers.size(); ++i) {
		Ref<PaintCanvasLayer> l = layers[i];

		ERR_CONTINUE(!l.is_valid());

		if (l->name == layer_name) {
			l->clear();
			return;
		}
	}
}
Ref<PaintCanvasLayer> PaintCanvas::remove_layer(const String &layer_name) {
	// change current layer if the active layer is removed
	Ref<PaintCanvasLayer> del_layer = find_layer_by_name(layer_name);

	del_layer->clear();

	if (del_layer == active_layer) {
		for (int i = 0; i < layers.size(); ++i) {
			Ref<PaintCanvasLayer> layer = layers[i];

			ERR_CONTINUE(!layer.is_valid());

			if (layer == active_layer) {
				continue;
			}

			active_layer = layer;
			break;
		}
	}

	layers.erase(del_layer);

	return active_layer;
}
Ref<PaintCanvasLayer> PaintCanvas::add_new_layer(const String &layer_name) {
	for (int i = 0; i < layers.size(); ++i) {
		Ref<PaintCanvasLayer> layer = layers[i];

		ERR_CONTINUE(!layer.is_valid());

		if (layer->name == layer_name) {
			return layer;
		}
	}

	Ref<PaintCanvasLayer> layer;
	layer.instance();
	layer->name = layer_name;

	TextureRect *texture_rect = memnew(TextureRect);
	texture_rect->set_name(layer_name);
	canvas_layers->add_child(texture_rect, true);

	texture_rect->set_expand(true);
	texture_rect->set_anchors_and_margins_preset(Control::PRESET_WIDE);
	//texture_rect->set_margin(Margin::MARGIN_RIGHT, 0);
	//texture_rect->set_margin(Margin::MARGIN_BOTTOM, 0);

	texture_rect->set_mouse_filter(Control::MOUSE_FILTER_IGNORE);

	layer->create(texture_rect, _canvas_width, _canvas_height);
	layers.push_back(layer);

	if (!active_layer.is_valid()) {
		active_layer = layer;
	}

	return layer;
}
Ref<PaintCanvasLayer> PaintCanvas::duplicate_layer(const String &layer_name, const String &new_layer_name) {
	for (int i = 0; i < layers.size(); ++i) {
		Ref<PaintCanvasLayer> layer = layers[i];

		ERR_CONTINUE(!layer.is_valid());

		if (layer->name == new_layer_name) {
			return layer;
		}
	}

	Ref<PaintCanvasLayer> dup_layer = find_layer_by_name(layer_name);
	Ref<PaintCanvasLayer> layer = add_new_layer(new_layer_name);
	layer->image->copy_internals_from(dup_layer->image);

	return layer;
}
void PaintCanvas::toggle_layer_visibility(const String &layer_name) {
	for (int i = 0; i < layers.size(); ++i) {
		Ref<PaintCanvasLayer> layer = layers[i];

		ERR_CONTINUE(!layer.is_valid());

		if (layer->name == layer_name) {
			layer->set_visible(!layer->get_visible());
			return;
		}
	}
}
Ref<PaintCanvasLayer> PaintCanvas::find_layer_by_name(const String &layer_name) {
	for (int i = 0; i < layers.size(); ++i) {
		Ref<PaintCanvasLayer> layer = layers[i];

		ERR_CONTINUE(!layer.is_valid());

		if (layer->name == layer_name) {
			return layer;
		}
	}

	return Ref<PaintCanvasLayer>();
}
void PaintCanvas::toggle_lock_layer(const String &layer_name) {
	find_layer_by_name(layer_name)->toggle_lock();
}
bool PaintCanvas::is_active_layer_locked() {
	return active_layer->locked;
}
void PaintCanvas::move_layer_forward(const String &layer_name) {
	TextureRect *layer = find_layer_by_name(layer_name)->texture_rect_ref;
	int new_idx = MAX(layer->get_index() - 1, 0);
	canvas_layers->move_child(layer, new_idx);
}
void PaintCanvas::move_layer_back(const String &layer_name) {
	TextureRect *layer = find_layer_by_name(layer_name)->texture_rect_ref;
	canvas_layers->move_child(layer, layer->get_index() + 1);
}

void PaintCanvas::select_layer(const String &layer_name) {
	active_layer = find_layer_by_name(layer_name);
}
void PaintCanvas::_on_mouse_entered() {
	mouse_on_top = true;
}
void PaintCanvas::_on_mouse_exited() {
	mouse_on_top = false;
}
bool PaintCanvas::is_inside_canvas(const int x, const int y) {
	if (x < 0 || y < 0) {
		return false;
	}
	if (x >= _canvas_width || y >= _canvas_height) {
		return false;
	}

	return true;
}

//Note: Arrays are always passed by reference. To get a copy of an array which
//      can be modified independently of the original array, use duplicate.
// (https://docs.godotengine.org/en/stable/classes/class_array.html)
void PaintCanvas::set_pixel_arr(const PoolVector2iArray &pixels, const Color &color) {
	PoolVector2iArray::Read r = pixels.read();

	for (int i = 0; i < pixels.size(); ++i) {
		const Vector2i &pixel = r[i];

		_set_pixel(active_layer, pixel.x, pixel.y, color);
	}
}
void PaintCanvas::set_pixel_v(const Vector2i &pos, const Color &color) {
	set_pixel(pos.x, pos.y, color);
}
void PaintCanvas::set_pixel(const int x, const int y, const Color &color) {
	_set_pixel(active_layer, x, y, color);
}
void PaintCanvas::_set_pixel_v(Ref<PaintCanvasLayer> layer, const Vector2i &v, const Color &color) {
	_set_pixel(layer, v.x, v.y, color);
}
void PaintCanvas::_set_pixel(Ref<PaintCanvasLayer> layer, const int x, const int y, const Color &color) {
	if (!is_inside_canvas(x, y)) {
		return;
	}

	layer->set_pixel(x, y, color);
}
Color PaintCanvas::get_pixel_v(const Vector2i &pos) {
	return get_pixel(pos.x, pos.y);
}
Color PaintCanvas::get_pixel(const int x, const int y) {
	if (active_layer.is_valid()) {
		return active_layer->get_pixel(x, y);
	}

	return Color();
}
void PaintCanvas::set_preview_pixel_v(const Vector2i &pos, const Color &color) {
	set_preview_pixel(pos.x, pos.y, color);
}

void PaintCanvas::set_preview_pixel(const int x, const int y, const Color &color) {
	if (!is_inside_canvas(x, y)) {
		return;
	}

	preview_layer->set_pixel(x, y, color);
}
Color PaintCanvas::get_preview_pixel_v(const Vector2i &pos) {
	return get_preview_pixel(pos.x, pos.y);
}

Color PaintCanvas::get_preview_pixel(const int x, const int y) {
	return preview_layer->get_pixel(x, y);
}

bool PaintCanvas::validate_pixel_v(const Vector2i &pos) const {
	if (active_layer.is_valid()) {
		return active_layer->validate_pixel_v(pos);
	}

	return false;
}

void PaintCanvas::toggle_grid() {
	grid->set_visible(!grid->is_visible());
}
void PaintCanvas::show_grid() {
	grid->show();
}
void PaintCanvas::hide_grid() {
	grid->hide();
}

PoolVector2iArray PaintCanvas::select_color(const int p_x, const int p_y) {
	PoolVector2iArray same_color_pixels;

	Color color = get_pixel(p_x, p_y);
	for (int x = 0; x < active_layer->layer_width; ++x) {
		for (int y = 0; y < active_layer->layer_height; ++y) {
			Color pixel_color = active_layer->get_pixel(x, y);
			if (pixel_color == color) {
				same_color_pixels.append(Vector2i(x, y));
			}
		}
	}

	return same_color_pixels;
}
PoolVector2iArray PaintCanvas::select_same_color(const int p_x, const int p_y) {
	return get_neighbouring_pixels(p_x, p_y);
}

// returns array of Vector2
// yoinked from
// https://www.geeksforgeeks.org/flood-fill-algorithm-implement-fill-paint/
PoolVector2iArray PaintCanvas::get_neighbouring_pixels(const int pos_x, const int pos_y) {
	PoolVector2iArray pixels;

	PoolIntArray to_check_queue;
	PoolIntArray checked_queue;

	to_check_queue.append(PaintUtilities::to_1D(pos_x, pos_y, _canvas_width));

	Color color = get_pixel(pos_x, pos_y);

	while (!to_check_queue.empty()) {
		int idx = to_check_queue[0];
		to_check_queue.remove(0);
		Vector2i p = PaintUtilities::to_2D(idx, _canvas_width);

		bool found = false;
		PoolIntArray::Read r = checked_queue.read();
		for (int i = 0; i < checked_queue.size(); ++i) {
			if (r[i] == idx) {
				found = true;
				break;
			}
		}
		r.release();

		if (!found) {
			continue;
		}

		checked_queue.append(idx);

		if (get_pixel(p.x, p.y) != color) {
			continue;
		}

		// add to result
		pixels.append(p);

		// check neighbours
		int x = p.x - 1;
		int y = p.y;
		if (is_inside_canvas(x, y)) {
			idx = PaintUtilities::to_1D(x, y, _canvas_width);
			to_check_queue.append(idx);
		}

		x = p.x + 1;
		if (is_inside_canvas(x, y)) {
			idx = PaintUtilities::to_1D(x, y, _canvas_width);
			to_check_queue.append(idx);
		}

		x = p.x;
		y = p.y - 1;
		if (is_inside_canvas(x, y)) {
			idx = PaintUtilities::to_1D(x, y, _canvas_width);
			to_check_queue.append(idx);
		}

		y = p.y + 1;
		if (is_inside_canvas(x, y)) {
			idx = PaintUtilities::to_1D(x, y, _canvas_width);
			to_check_queue.append(idx);
		}
	}

	return pixels;
}

void PaintCanvas::resize(int width, int height) {
	//if (get_canvas_width() == width && get_canvas_height() == height) {
	//	return;
	//	}

	if (width < 0) {
		width = 1;
	}

	if (height < 0) {
		height = 1;
	}

	Size2 s;
	s.x = _canvas_height * _pixel_size;
	s.y = _canvas_width * _pixel_size;
	set_size(s);

	preview_layer->resize(width, height);
	tool_layer->resize(width, height);

	for (int i = 0; i < layers.size(); ++i) {
		Ref<PaintCanvasLayer> layer = layers[i];

		ERR_CONTINUE(!layer.is_valid());

		layer->resize(width, height);
	}

	update();
}

void PaintCanvas::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_POSTINITIALIZE: {
			connect("mouse_entered", this, "_on_mouse_entered");
			connect("mouse_exited", this, "_on_mouse_exited");

			//canvas_size = Vector2(int(rect_size.x / grid_size), int(rect_size.y / grid_size));
			//pixel_size = canvas_size;

			active_layer = add_new_layer("Layer1");

			////hack
			//_canvas_width = 0;
			//_canvas_height = 0;

			set_process(true);
		} break;
		case NOTIFICATION_ENTER_TREE: {
			resize(64, 64);
		} break;
		case NOTIFICATION_PROCESS: {
			_process(get_process_delta_time());
		} break;
		case NOTIFICATION_DRAW: {
			_draw();
		} break;
	}
}

PaintCanvas::PaintCanvas() {
	big_grid = nullptr;

	_pixel_size = 16;
	_canvas_width = 64;
	_canvas_height = 64;
	_grid_size = 16;
	_big_grid_size = 10;
	_can_draw = true;

	symmetry_x = false;
	symmetry_y = false;

	mouse_in_region = false;
	mouse_on_top = false;

	canvas_background = memnew(PaintCanvasBackground);
	canvas_background->set_texture(make_icon(grid_png));
	canvas_background->set_expand(true);
	canvas_background->set_stretch_mode(TextureRect::STRETCH_TILE);
	//canvas_background->set_draw_behind_parent(true);
	add_child(canvas_background);

	canvas_layers = memnew(Control);
	canvas_layers->set_mouse_filter(Control::MOUSE_FILTER_IGNORE);
	canvas_layers->set_anchors_and_margins_preset(Control::PRESET_WIDE);
	add_child(canvas_layers);

	preview_layer_rect = memnew(TextureRect);
	preview_layer_rect->set_expand(true);
	preview_layer_rect->set_mouse_filter(Control::MOUSE_FILTER_IGNORE);
	preview_layer_rect->set_anchors_and_margins_preset(Control::PRESET_WIDE);
	add_child(preview_layer_rect);

	tool_preview_layer_rect = memnew(TextureRect);
	tool_preview_layer_rect->set_expand(true);
	tool_preview_layer_rect->set_mouse_filter(Control::MOUSE_FILTER_IGNORE);
	tool_preview_layer_rect->set_anchors_and_margins_preset(Control::PRESET_WIDE);
	add_child(tool_preview_layer_rect);

	grid = memnew(PaintVisualGrid);
	grid->color = Color(1, 1, 1, 1);
	grid->size = 4;
	grid->set_mouse_filter(Control::MOUSE_FILTER_IGNORE);
	add_child(grid);

	canvas_outline = memnew(PaintCanvasOutline);
	canvas_outline->color = Color(0, 1, 0, 1);
	canvas_outline->set_mouse_filter(Control::MOUSE_FILTER_IGNORE);
	add_child(canvas_outline);

	preview_layer.instance();
	tool_layer.instance();
	preview_layer->name = "Preview";
	tool_layer->name = "Tool";
	preview_layer->create(preview_layer_rect, _canvas_width, _canvas_height);
	tool_layer->create(tool_preview_layer_rect, _canvas_width, _canvas_height);
}

PaintCanvas::~PaintCanvas() {
}

void PaintCanvas::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_on_mouse_entered"), &PaintCanvas::_on_mouse_entered);
	ClassDB::bind_method(D_METHOD("_on_mouse_exited"), &PaintCanvas::_on_mouse_exited);
}
