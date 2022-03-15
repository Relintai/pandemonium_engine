/*
Copyright (c) 2019-2022 Péter Magyar

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

#ifndef TEXTURE_LAYER_MERGER_H
#define TEXTURE_LAYER_MERGER_H

#include "core/version.h"

#if VERSION_MAJOR > 3
#include "core/object/ref_counted.h"
#ifndef Reference
#define Reference RefCounted
#endif
#include "core/templates/vector.h"
#include "core/io/image.h"
#else
#include "core/reference.h"
#include "core/vector.h"
#include "core/image.h"
#endif

#include "core/math/rect2.h"
#include "scene/resources/texture.h"

#include "core/version.h"

#if VERSION_MAJOR >= 4
#define PoolVector Vector
#define Texture Texture2D
#endif

class TextureLayerMerger : public Reference {

	GDCLASS(TextureLayerMerger, Reference);

public:
	int get_width() const;
	void set_width(const int p_value);

	int get_height() const;
	void set_height(const int p_value);

	uint32_t get_texture_flags() const;
	void set_texture_flags(const uint32_t p_flags);

	Color get_base_color() const;
	void set_base_color(const Color &p_color);

	Ref<Image> get_data() const;
	void set_data(const Ref<Image> &p_image);

	Ref<ImageTexture> get_result_as_texture() const;

	void add_texture(const Ref<Texture> &p_texture, const Color &p_color = Color(1, 1, 1, 1), const Vector2 &p_position = Vector2(), Rect2 p_rect = Rect2());

	Ref<Texture> get_texture(const int p_index);
	void set_texture(const int p_index, const Ref<Texture> &p_texture);

	Color get_color(const int p_index);
	void set_color(const int p_index, const Color &p_color);

	Vector2 get_position(const int p_index);
	void set_position(const int p_index, const Vector2 &p_position);

	Rect2 get_rect(const int p_index);
	void set_rect(const int p_index, const Rect2 &p_rect);

	void remove_texture(const int p_index);
	int get_texture_count();
	void clear();

	void merge();

	TextureLayerMerger();
	~TextureLayerMerger();

protected:
	void write_base_color_to_array(PoolVector<uint8_t> &data);

	static void _bind_methods();

	struct LayerMergerEntry {
		Ref<Texture> texture;
		Color color;
		Rect2 rect;
		Vector2 position;
	};

private:
	int _width;
	int _height;

	uint32_t _texture_flags;

	Color _base_color;

	Ref<Image> _image;

	Vector<LayerMergerEntry> _entries;
};

#endif
