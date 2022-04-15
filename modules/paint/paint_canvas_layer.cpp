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

#include "paint_canvas_layer.h"

void PaintCanvasLayer::_init() {
    /*
	texture = ImageTexture.new()
    */
}
void PaintCanvasLayer::create(Node *texture_rect_ref, int width, int height) {
    /*
	self.texture_rect_ref = texture_rect_ref

	layer_width = width
	layer_height = height

	image = Image.new()
	image.create(width, height, false, Image.FORMAT_RGBA8)
	image.fill(Color.transparent)
	update_texture()
    */
}

void PaintCanvasLayer::resize(int width, int height) {
    /*
	var pixel_colors = []
	var prev_width = layer_width
	var prev_height = layer_height

	image.lock()
	for y in range(prev_height):
		for x in range(prev_width):
			pixel_colors.append(image.get_pixel(x, y))
	image.unlock()

	layer_width = width
	layer_height = height

	image.create(width, height, false, Image.FORMAT_RGBA8)

	image.lock()
	for x in range(prev_width):
		for y in range(prev_height):
			if x >= width or y >= height:
				continue
			image.set_pixel(x, y, pixel_colors[GEUtils.to_1D(x, y, prev_width)])
	image.unlock()

	update_texture()
    */
}
void PaintCanvasLayer::set_pixel(int x, int y, Color color) {
    /*
	image.lock()
	image.set_pixel(x, y, color)
	image.unlock()
    */
}
Color PaintCanvasLayer::get_pixel(int x, int y) {
    /*
	if x < 0 or y < 0 or x >= image.get_width() or y >= image.get_height():
		return null
	image.lock()
	var pixel = image.get_pixel(x, y)
	image.unlock()
	return pixel
    */
}
void PaintCanvasLayer::clear() {
    /*
	image.fill(Color.transparent)
	update_texture()
    */
}
void PaintCanvasLayer::update_texture() {
    /*
	texture.create_from_image(image, 0)
	texture_rect_ref.texture = texture
	texture_rect_ref.margin_right = 0
	texture_rect_ref.margin_bottom = 0
    */
}
void PaintCanvasLayer::set_visible(bool vis) {
    /*
	visible = vis
	texture_rect_ref.visible = visible
    */
}
void PaintCanvasLayer::toggle_lock() {
    /*
	locked = not locked
    */
}
void PaintCanvasLayer::toggle_alpha_locked() {
    /*
	alpha_locked = not alpha_locked
    */
}

PaintCanvasLayer::PaintCanvasLayer() {
	/*
    var name
    var layer_width
    var layer_height
    var visible = true setget set_visible
    var locked = false
    var alpha_locked = false

    var texture: ImageTexture
    var image: Image
    var texture_rect_ref
	*/
}

PaintCanvasLayer::~PaintCanvasLayer() {
}

void PaintCanvasLayer::_bind_methods() {
}
