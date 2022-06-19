#ifndef PAINT_CANVAS_LAYER_H
#define PAINT_CANVAS_LAYER_H

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

#include "core/reference.h"

#include "core/image.h"
#include "scene/resources/texture.h"

class TextureRect;

//class_name GELayer
class PaintCanvasLayer : public Reference {
	GDCLASS(PaintCanvasLayer, Reference);

public:
	String name;
	int layer_width;
	int layer_height;

	bool locked = false;
	bool alpha_locked = false;

	Ref<ImageTexture> texture;
	Ref<Image> image;
	TextureRect *texture_rect_ref;

	bool get_visible();
	void set_visible(const bool vis);

	void create(TextureRect *p_texture_rect_ref, const int width, const int height);

	void resize(const int width, const int height);
	void set_pixel(const int x, const int y, const Color &color);
	Color get_pixel(const int x, const int y);
	bool validate_pixel_v(const Vector2i &pos) const;
	void clear();
	void update_texture();

	void toggle_lock();
	void toggle_alpha_locked();

	PaintCanvasLayer();
	~PaintCanvasLayer();

protected:
	static void _bind_methods();

	bool _visible;
};

#endif
