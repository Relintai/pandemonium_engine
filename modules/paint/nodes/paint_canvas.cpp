#include "paint_canvas.h"

#include "../paint_utilities.h"
#include "core/io/image.h"
#include "scene/resources/texture.h"

bool PaintCanvas::get_symmetry_x() const {
	return _symmetry_x;
}
void PaintCanvas::set_symmetry_x(const bool val) {
	_symmetry_x = val;
}

bool PaintCanvas::get_symmetry_y() const {
	return _symmetry_y;
}
void PaintCanvas::set_symmetry_y(const bool val) {
	_symmetry_y = val;
}

bool PaintCanvas::get_alpha_locked() const {
	return _alpha_locked;
}
void PaintCanvas::set_alpha_locked(const bool val) {
	_alpha_locked = val;
}

bool PaintCanvas::validate_pixel_v(const Vector2i &pos) const {
	if (pos.x < 0 || pos.y < 0 || pos.x >= _image->get_width() || pos.y >= _image->get_height()) {
		return false;
	}

	return true;
}

bool PaintCanvas::is_inside_canvas(const int x, const int y) {
	if (x < 0 || y < 0) {
		return false;
	}
	if (x >= get_size().x || y >= get_size().y) {
		return false;
	}

	return true;
}

void PaintCanvas::set_pixel_arr(const PoolVector2iArray &pixels, const Color &color) {
	PoolVector2iArray::Read r = pixels.read();

	for (int i = 0; i < pixels.size(); ++i) {
		const Vector2i &pixel = r[i];

		set_pixel(pixel.x, pixel.y, color);
	}
}
void PaintCanvas::set_pixel_v(const Vector2i &pos, const Color &color) {
	set_pixel(pos.x, pos.y, color);
}
void PaintCanvas::set_pixel(const int x, const int y, const Color &color) {
	if (x < 0 || y < 0 || x >= _image->get_width() || y >= _image->get_height()) {
		return;
	}

	_image->lock();
	_image->set_pixel(x, y, color);
	_image->unlock();
}

Color PaintCanvas::get_pixel_v(const Vector2i &pos) {
	return get_pixel(pos.x, pos.y);
}
Color PaintCanvas::get_pixel(const int x, const int y) {
	if (x < 0 || y < 0 || x >= _image->get_width() || y >= _image->get_height()) {
		return Color();
	}

	_image->lock();
	Color pixel = _image->get_pixel(x, y);
	_image->unlock();

	return pixel;
}
void PaintCanvas::set_preview_pixel_v(const Vector2i &pos, const Color &color) {
	set_preview_pixel(pos.x, pos.y, color);
}

void PaintCanvas::set_preview_pixel(const int x, const int y, const Color &color) {
	if (x < 0 || y < 0 || x >= _preview_image->get_width() || y >= _preview_image->get_height()) {
		return;
	}

	_preview_image->lock();
	_preview_image->set_pixel(x, y, color);
	_preview_image->unlock();
}
Color PaintCanvas::get_preview_pixel_v(const Vector2i &pos) {
	return get_preview_pixel(pos.x, pos.y);
}

Color PaintCanvas::get_preview_pixel(const int x, const int y) {
	if (x < 0 || y < 0 || x >= _preview_image->get_width() || y >= _preview_image->get_height()) {
		return Color();
	}

	_preview_image->lock();
	Color pixel = _preview_image->get_pixel(x, y);
	_preview_image->unlock();

	return pixel;
}

void PaintCanvas::clear() {
	_image->fill(Color(1.00, 1.00, 1.00, 0.00));

	_image_texture->create_from_image(_image, 0);
}

void PaintCanvas::clear_preview() {
	_preview_image->fill(Color(1.00, 1.00, 1.00, 0.00));

	_preview_image_texture->create_from_image(_preview_image, 0);
}

void PaintCanvas::update_textures() {
	_image_texture->create_from_image(_image, 0);
	_preview_image_texture->create_from_image(_preview_image, 0);

	update();
}

void PaintCanvas::resize(int width, int height) {
	if (get_size().x == width && get_size().y == height) {
		return;
	}

	if (width < 0) {
		width = 1;
	}

	if (height < 0) {
		height = 1;
	}

	set_size(Vector2i(width, height));

	resize_image(_image);
	resize_image(_preview_image);

	update_textures();
}

void PaintCanvas::resize_image(Ref<Image> image) {
	ERR_FAIL_COND(!image.is_valid());

	PoolColorArray pixel_colors;
	int prev_width = image->get_size().x;
	int prev_height = image->get_size().y;

	if (prev_width != 0 && prev_height != 0) {
		image->lock();
		for (int y = 0; y < prev_height; ++y) {
			for (int x = 0; x < prev_width; ++x) {
				pixel_colors.append(image->get_pixel(x, y));
			}
		}
		image->unlock();
	}

	image->create(get_size().x, get_size().y, false, Image::FORMAT_RGBA8);

	image->lock();

	for (int x = 0; x < prev_width; ++x) {
		for (int y = 0; y < prev_height; ++y) {
			if (x >= get_size().x || y >= get_size().y) {
				continue;
			}

			image->set_pixel(x, y, pixel_colors[PaintUtilities::to_1D(x, y, prev_width)]);
		}
	}

	image->unlock();
}

PoolVector2iArray PaintCanvas::select_color(const int p_x, const int p_y) {
	PoolVector2iArray same_color_pixels;

	Color color = get_pixel(p_x, p_y);
	for (int x = 0; x < get_size().x; ++x) {
		for (int y = 0; y < get_size().y; ++y) {
			Color pixel_color = get_pixel(x, y);

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

// yoinked from
// https://www.geeksforgeeks.org/flood-fill-algorithm-implement-fill-paint/
PoolVector2iArray PaintCanvas::get_neighbouring_pixels(const int pos_x, const int pos_y) {
	PoolVector2iArray pixels;

	Vector<int> to_check_queue;
	Vector<int> checked_queue;

	to_check_queue.push_back(PaintUtilities::to_1D(pos_x, pos_y, get_size().x));

	Color color = get_pixel(pos_x, pos_y);

	while (!to_check_queue.empty()) {
		int idx = to_check_queue[0];
		to_check_queue.remove(0);
		Vector2i p = PaintUtilities::to_2D(idx, get_size().x);

		if (checked_queue.find(idx) != -1) {
			continue;
		}

		checked_queue.push_back(idx);

		if (get_pixel(p.x, p.y) != color) {
			continue;
		}

		// add to result
		pixels.push_back(p);

		// check neighbours
		int x = p.x - 1;
		int y = p.y;
		if (is_inside_canvas(x, y)) {
			idx = PaintUtilities::to_1D(x, y, get_size().x);
			to_check_queue.push_back(idx);
		}

		x = p.x + 1;
		if (is_inside_canvas(x, y)) {
			idx = PaintUtilities::to_1D(x, y, get_size().x);
			to_check_queue.push_back(idx);
		}

		x = p.x;
		y = p.y - 1;
		if (is_inside_canvas(x, y)) {
			idx = PaintUtilities::to_1D(x, y, get_size().x);
			to_check_queue.push_back(idx);
		}

		y = p.y + 1;
		if (is_inside_canvas(x, y)) {
			idx = PaintUtilities::to_1D(x, y, get_size().x);
			to_check_queue.push_back(idx);
		}
	}

	return pixels;
}

PaintCanvas::PaintCanvas() {
	_symmetry_x = false;
	_symmetry_y = false;
	_alpha_locked = false;

	_image.instance();
	_preview_image.instance();

	_image_texture.instance();
	_preview_image_texture.instance();
}

PaintCanvas::~PaintCanvas() {
}

void PaintCanvas::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_DRAW: {
			draw_texture(_image_texture, Point2());
			draw_texture(_preview_image_texture, Point2());
		} break;
	}
}

void PaintCanvas::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_symmetry_x"), &PaintCanvas::get_symmetry_x);
	ClassDB::bind_method(D_METHOD("set_symmetry_x", "val"), &PaintCanvas::set_symmetry_x);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "symmetry_x"), "set_symmetry_x", "get_symmetry_x");

	ClassDB::bind_method(D_METHOD("get_symmetry_y"), &PaintCanvas::get_symmetry_y);
	ClassDB::bind_method(D_METHOD("set_symmetry_y", "val"), &PaintCanvas::set_symmetry_y);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "symmetry_y"), "set_symmetry_y", "get_symmetry_y");

	ClassDB::bind_method(D_METHOD("get_alpha_locked"), &PaintCanvas::get_alpha_locked);
	ClassDB::bind_method(D_METHOD("set_alpha_locked", "val"), &PaintCanvas::set_alpha_locked);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "alpha_locked"), "set_alpha_locked", "get_alpha_locked");

	ClassDB::bind_method(D_METHOD("is_inside_canvas", "x", "y"), &PaintCanvas::is_inside_canvas);
	ClassDB::bind_method(D_METHOD("set_pixel_arr", "pixels", "color"), &PaintCanvas::set_pixel_arr);

	ClassDB::bind_method(D_METHOD("set_pixel_v", "pos", "color"), &PaintCanvas::set_pixel_v);
	ClassDB::bind_method(D_METHOD("set_pixel", "x", "y", "color"), &PaintCanvas::set_pixel);

	ClassDB::bind_method(D_METHOD("get_pixel_v", "pos"), &PaintCanvas::get_pixel_v);
	ClassDB::bind_method(D_METHOD("get_pixel", "x", "y"), &PaintCanvas::get_pixel);

	ClassDB::bind_method(D_METHOD("set_preview_pixel_v", "pos", "color"), &PaintCanvas::set_preview_pixel_v);
	ClassDB::bind_method(D_METHOD("set_preview_pixel", "x", "y", "color"), &PaintCanvas::set_preview_pixel);

	ClassDB::bind_method(D_METHOD("get_preview_pixel_v", "pos"), &PaintCanvas::get_preview_pixel_v);
	ClassDB::bind_method(D_METHOD("get_preview_pixel", "x", "y"), &PaintCanvas::get_preview_pixel);

	ClassDB::bind_method(D_METHOD("validate_pixel_v", "pos"), &PaintCanvas::validate_pixel_v);

	ClassDB::bind_method(D_METHOD("clear"), &PaintCanvas::clear);
	ClassDB::bind_method(D_METHOD("clear_preview"), &PaintCanvas::clear_preview);
	ClassDB::bind_method(D_METHOD("update_textures"), &PaintCanvas::update_textures);

	ClassDB::bind_method(D_METHOD("resize", "width", "height"), &PaintCanvas::resize);
	ClassDB::bind_method(D_METHOD("resize_image", "image"), &PaintCanvas::resize_image);

	ClassDB::bind_method(D_METHOD("select_color", "x", "y"), &PaintCanvas::select_color);
	ClassDB::bind_method(D_METHOD("select_same_color", "x", "y"), &PaintCanvas::select_same_color);
	ClassDB::bind_method(D_METHOD("get_neighbouring_pixels", "x", "y"), &PaintCanvas::get_neighbouring_pixels);
}
