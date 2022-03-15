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

#include "texture_packer.h"

int TexturePacker::get_texture_flags() const {
	return _texture_flags;
}
void TexturePacker::set_texture_flags(const int flags) {
	_texture_flags = flags;
}

int TexturePacker::get_max_atlas_size() const {
	return _max_atlas_size;
}
void TexturePacker::set_max_atlas_size(const int size) {
	_max_atlas_size = size;
}

bool TexturePacker::get_keep_original_atlases() const {
	return _keep_original_atlases;
}
void TexturePacker::set_keep_original_atlases(const bool value) {
	_keep_original_atlases = value;
}

Color TexturePacker::get_background_color() const {
	return _background_color;
}
void TexturePacker::set_background_color(const Color &color) {
	_background_color = color;
}

int TexturePacker::get_margin() const {
	return _margin;
}
void TexturePacker::set_margin(const int margin) {
	_margin = margin;
}

Ref<AtlasTexture> TexturePacker::add_texture(const Ref<Texture> &texture) {
	ERR_FAIL_COND_V(!texture.is_valid(), Ref<AtlasTexture>());

	Ref<AtlasTexture> atlas_text = texture;

	if (atlas_text.is_valid()) {
		//we need to check differently this case
		for (int i = 0; i < _rects.size(); ++i) {
			rect_xywhf *r = _rects.get(i);

			Ref<Texture> t;
			Ref<AtlasTexture> at = texture;

			if (_keep_original_atlases && at.is_valid())
				t = r->atlas_texture;
			else
				t = r->original_texture;

			if (t == texture) {
				++(r->refcount);

				return r->atlas_texture;
			}
		}

		Ref<AtlasTexture> tex;
#if VERSION_MAJOR < 4
		tex.instance();
#else
		tex.instantiate();
#endif

		tex->set_atlas(atlas_text->get_atlas());
		tex->set_region(atlas_text->get_region());

		rect_xywhf *rect = memnew(rect_xywhf);

		rect->refcount = 1;
		rect->w = atlas_text->get_region().size.x + 2 * _margin;
		rect->h = atlas_text->get_region().size.y + 2 * _margin;

		_rects.push_back(rect);

		if (_keep_original_atlases) {
			rect->original_texture = tex;
			rect->atlas_texture = atlas_text;

			return atlas_text;
		} else {
			rect->original_texture = atlas_text;
			rect->atlas_texture = tex;

			return tex;
		}
	}

	for (int i = 0; i < _rects.size(); ++i) {
		rect_xywhf *r = _rects.get(i);

		if (r->original_texture == texture) {
			++(r->refcount);

			return r->atlas_texture;
		}
	}

	Ref<AtlasTexture> tex;
#if VERSION_MAJOR < 4
	tex.instance();
#else
	tex.instantiate();
#endif

	//Temp setup, so the texture is usable even while the atlases are generating.
	tex->set_atlas(texture);
	tex->set_region(Rect2(0, 0, texture->get_width(), texture->get_height()));

	rect_xywhf *rect = memnew(rect_xywhf);

	rect->refcount = 1;
	rect->original_texture = texture;
	rect->atlas_texture = tex;

	rect->w = texture->get_width() + 2 * _margin;
	rect->h = texture->get_height() + 2 * _margin;

	_rects.push_back(rect);

	return tex;
}

Ref<AtlasTexture> TexturePacker::get_texture(const Ref<Texture> &texture) {
	for (int i = 0; i < _rects.size(); ++i) {
		rect_xywhf *r = _rects.get(i);

		Ref<Texture> t;
		Ref<AtlasTexture> at = texture;

		if (_keep_original_atlases && at.is_valid())
			t = r->atlas_texture;
		else
			t = r->original_texture;

		if (t == texture) {
			return _rects.get(i)->atlas_texture;
		}
	}

	return Ref<Texture>();
}

Ref<AtlasTexture> TexturePacker::get_texture_index(const int index) {
	ERR_FAIL_INDEX_V(index, _rects.size(), Ref<AtlasTexture>());

	return _rects.get(index)->atlas_texture;
}

Ref<Texture> TexturePacker::get_original_texture(const int index) {
	ERR_FAIL_INDEX_V(index, _rects.size(), Ref<Texture>());

	return _rects.get(index)->original_texture;
}

bool TexturePacker::contains_texture(const Ref<Texture> &texture) {
	for (int i = 0; i < _rects.size(); ++i) {
		rect_xywhf *r = _rects.get(i);

		Ref<Texture> t;
		Ref<AtlasTexture> at = texture;

		if (_keep_original_atlases && at.is_valid())
			t = r->atlas_texture;
		else
			t = r->original_texture;

		if (t == texture) {
			return true;
		}
	}

	return false;
}

bool TexturePacker::unref_texture_index(const int index) {
	ERR_FAIL_INDEX_V(index, _rects.size(), false);

	rect_xywhf *r = _rects.get(index);

	int rc = --(r->refcount);

	if (rc <= 0) {
#if VERSION_MAJOR < 4
		_rects.remove(index);
#else
		_rects.remove_at(index);
#endif

		r->original_texture.unref();
		r->atlas_texture.unref();

		memdelete(r);

		return true;
	}

	return false;
}

bool TexturePacker::unref_texture(const Ref<Texture> &texture) {
	for (int i = 0; i < _rects.size(); ++i) {
		rect_xywhf *r = _rects.get(i);

		Ref<Texture> t;
		Ref<AtlasTexture> at = texture;

		if (_keep_original_atlases && at.is_valid())
			t = r->atlas_texture;
		else
			t = r->original_texture;

		if (t == texture) {
			int rc = --(r->refcount);

			if (rc <= 0) {
#if VERSION_MAJOR < 4
				_rects.remove(i);
#else
				_rects.remove_at(i);
#endif

				r->original_texture.unref();
				r->atlas_texture.unref();

				memdelete(r);

				return true;
			}

			return false;
		}
	}

	return false;
}

void TexturePacker::remove_texture_index(const int index) {
	ERR_FAIL_INDEX(index, _rects.size());

	rect_xywhf *r = _rects.get(index);

	r->original_texture.unref();
	r->atlas_texture.unref();

	memdelete(r);
}

void TexturePacker::remove_texture(const Ref<Texture> &texture) {
	for (int i = 0; i < _rects.size(); ++i) {
		rect_xywhf *r = _rects.get(i);

		Ref<Texture> t;
		Ref<AtlasTexture> at = texture;

		if (_keep_original_atlases && at.is_valid())
			t = r->atlas_texture;
		else
			t = r->original_texture;

		if (t == texture) {
#if VERSION_MAJOR < 4
			_rects.remove(i);
#else
			_rects.remove_at(i);
#endif

			r->original_texture.unref();
			r->atlas_texture.unref();

			memdelete(r);

			return;
		}
	}
}

int TexturePacker::get_texture_count() {
	return _rects.size();
}

void TexturePacker::clear() {
	for (int i = 0; i < _rects.size(); ++i) {
		rect_xywhf *r = _rects.get(i);

		r->atlas_texture.unref();
		r->original_texture.unref();

		memdelete(r);
	}

	_rects.clear();

	for (int i = 0; i < _generated_textures.size(); ++i) {
		_generated_textures.get(i).unref();
	}

	_generated_textures.clear();
}

Ref<ImageTexture> TexturePacker::get_generated_texture(const int index) {
	ERR_FAIL_INDEX_V(index, _generated_textures.size(), Ref<ImageTexture>());

	return _generated_textures.get(index);
}
int TexturePacker::get_generated_texture_count() {
	return _generated_textures.size();
}

void TexturePacker::merge() {
	std::vector<bin> bins;

	if (pack(_rects.ptr(), _rects.size(), _max_atlas_size, false, bins)) {
		_generated_textures.clear();

		_generated_textures.resize(bins.size());

		for (uint32_t i = 0; i < bins.size(); ++i) {
			bin b = bins[i];

			PoolByteArray data;
			data.resize(b.size.w * b.size.h * 4);

			//Setup background color
			uint8_t cr = _background_color.r * 255.0;
			uint8_t cg = _background_color.g * 255.0;
			uint8_t cb = _background_color.b * 255.0;
			uint8_t ca = _background_color.a * 255.0;

			for (int j = 0; j < data.size(); j += 4) {
				data.set(j, cr);
				data.set(j + 1, cg);
				data.set(j + 2, cb);
				data.set(j + 3, ca);
			}

			//Process rects
			for (uint32_t j = 0; j < b.rects.size(); ++j) {
				rect_xywhf *r = b.rects[j];

				Ref<Texture> otext = r->original_texture;
				Ref<AtlasTexture> aotext = otext;

				int rect_pos_x = 0;
				int rect_pos_y = 0;

				if (aotext.is_valid()) {
					otext = aotext->get_atlas();

					Rect2 rect = aotext->get_region();

					rect_pos_x = rect.position.x + 0.5;
					rect_pos_y = rect.position.y + 0.5;
				}

				ERR_CONTINUE(!otext.is_valid());

#if VERSION_MAJOR < 4
				Ref<Image> img = otext->get_data();
#else
				Ref<Image> img = otext->get_image();
#endif

				ERR_CONTINUE(!img.is_valid());

				int img_width = img->get_width();

				PoolByteArray image_data = img->get_data();

				int input_format_offset = get_offset_for_format(img->get_format());

				ERR_CONTINUE_MSG(input_format_offset == 0, "Format is not implemented, Skipping!");

				int h_wo_margin = r->h - 2 * _margin;
				for (int y = 0; y < h_wo_margin; ++y) {
					int orig_img_indx = (rect_pos_y + y) * img_width * input_format_offset + rect_pos_x * input_format_offset;
					int start_indx = (r->y + y + _margin) * b.size.w * 4 + (r->x + _margin) * 4;

					int row_width = (r->w - 2 * _margin);
					for (int x = 0; x < row_width; ++x) {
						for (int sx = 0; sx < input_format_offset; ++sx) {
							data.set(start_indx + (x * 4) + sx, image_data[orig_img_indx + sx + (x * input_format_offset)]);
						}
					}
				}
			}

			Ref<Image> image;
#if VERSION_MAJOR < 4
			image.instance();
#else
			image.instantiate();
#endif
			image->create(b.size.w, b.size.h, false, Image::FORMAT_RGBA8, data);

			Ref<ImageTexture> texture;
#if VERSION_MAJOR < 4
			texture.instance();
#else
			texture.instantiate();
#endif

#if VERSION_MAJOR < 4
			texture->create_from_image(image, _texture_flags);
#else
			texture->create_from_image(image);
#endif

			_generated_textures.set(i, texture);

			for (uint32_t j = 0; j < b.rects.size(); ++j) {
				rect_xywhf *r = b.rects[j];

				Ref<AtlasTexture> at = r->atlas_texture;

				at->set_atlas(texture);
				at->set_region(Rect2(r->x + _margin, r->y + _margin, r->w - 2 * _margin, r->h - 2 * _margin));
			}
		}
	}
}

int TexturePacker::get_offset_for_format(const Image::Format format) {
	switch (format) {
		case Image::FORMAT_RGB8:
			return 3;
		case Image::FORMAT_RGBA8:
			return 4;

		case Image::FORMAT_L8:
		case Image::FORMAT_LA8:
		case Image::FORMAT_R8:
		case Image::FORMAT_RG8:
		case Image::FORMAT_RGBA4444:
		case Image::FORMAT_RF:
		case Image::FORMAT_RGF:
		case Image::FORMAT_RGBF:
		case Image::FORMAT_RGBAF:
		case Image::FORMAT_RH:
		case Image::FORMAT_RGH:
		case Image::FORMAT_RGBH:
		case Image::FORMAT_RGBAH:
		case Image::FORMAT_RGBE9995:
		case Image::FORMAT_DXT1:
		case Image::FORMAT_DXT3:
		case Image::FORMAT_DXT5:
		case Image::FORMAT_RGTC_R:
		case Image::FORMAT_RGTC_RG:
		case Image::FORMAT_BPTC_RGBA:
		case Image::FORMAT_BPTC_RGBF:
		case Image::FORMAT_BPTC_RGBFU:
#if VERSION_MAJOR <= 3
		case Image::FORMAT_PVRTC2:
		case Image::FORMAT_PVRTC2A:
		case Image::FORMAT_PVRTC4:
		case Image::FORMAT_PVRTC4A:
#endif
		case Image::FORMAT_ETC:
		case Image::FORMAT_ETC2_R11:
		case Image::FORMAT_ETC2_R11S:
		case Image::FORMAT_ETC2_RG11:
		case Image::FORMAT_ETC2_RG11S:
		case Image::FORMAT_ETC2_RGB8:
		case Image::FORMAT_ETC2_RGBA8:
		case Image::FORMAT_ETC2_RGB8A1:
#if VERSION_MAJOR >= 4
		case Image::FORMAT_RGB565:
		case Image::FORMAT_ETC2_RA_AS_RG:
		case Image::FORMAT_DXT5_RA_AS_RG:
#else
		case Image::FORMAT_RGBA5551:
#endif
		case Image::FORMAT_MAX:
			return 0;
	}

	return 0;
}

TexturePacker::TexturePacker() {
#if VERSION_MAJOR < 4
	_texture_flags = Texture::FLAG_MIPMAPS | Texture::FLAG_FILTER;
#else
	_texture_flags = 0;
#endif

	_max_atlas_size = 1024;
	_keep_original_atlases = false;
	_margin = 0;
}

TexturePacker::~TexturePacker() {
	clear();
}

void TexturePacker::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_texture_flags"), &TexturePacker::get_texture_flags);
	ClassDB::bind_method(D_METHOD("set_texture_flags", "flags"), &TexturePacker::set_texture_flags);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "texture_flags", PROPERTY_HINT_FLAGS, "Mipmaps,Repeat,Filter,Anisotropic Linear,Convert to Linear,Mirrored Repeat,Video Surface"), "set_texture_flags", "get_texture_flags");

	ClassDB::bind_method(D_METHOD("get_max_atlas_size"), &TexturePacker::get_max_atlas_size);
	ClassDB::bind_method(D_METHOD("set_max_atlas_size", "size"), &TexturePacker::set_max_atlas_size);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_atlas_size"), "set_max_atlas_size", "get_max_atlas_size");

	ClassDB::bind_method(D_METHOD("get_keep_original_atlases"), &TexturePacker::get_keep_original_atlases);
	ClassDB::bind_method(D_METHOD("set_keep_original_atlases", "value"), &TexturePacker::set_keep_original_atlases);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "keep_original_atlases"), "set_keep_original_atlases", "get_keep_original_atlases");

	ClassDB::bind_method(D_METHOD("get_background_color"), &TexturePacker::get_background_color);
	ClassDB::bind_method(D_METHOD("set_background_color", "color"), &TexturePacker::set_background_color);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "background_color"), "set_background_color", "get_background_color");

	ClassDB::bind_method(D_METHOD("get_margin"), &TexturePacker::get_margin);
	ClassDB::bind_method(D_METHOD("set_margin", "size"), &TexturePacker::set_margin);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "margin"), "set_margin", "get_margin");

	ClassDB::bind_method(D_METHOD("add_texture", "texture"), &TexturePacker::add_texture);

	ClassDB::bind_method(D_METHOD("get_texture", "texture"), &TexturePacker::get_texture);
	ClassDB::bind_method(D_METHOD("get_texture_index", "index"), &TexturePacker::get_texture_index);

	ClassDB::bind_method(D_METHOD("get_original_texture", "index"), &TexturePacker::get_original_texture);

	ClassDB::bind_method(D_METHOD("unref_texture_index", "index"), &TexturePacker::unref_texture_index);
	ClassDB::bind_method(D_METHOD("unref_texture", "texture"), &TexturePacker::unref_texture);
	ClassDB::bind_method(D_METHOD("remove_texture_index", "index"), &TexturePacker::remove_texture_index);
	ClassDB::bind_method(D_METHOD("remove_texture", "texture"), &TexturePacker::remove_texture);

	ClassDB::bind_method(D_METHOD("get_texture_count"), &TexturePacker::get_texture_count);
	ClassDB::bind_method(D_METHOD("clear"), &TexturePacker::clear);

	ClassDB::bind_method(D_METHOD("get_generated_texture", "index"), &TexturePacker::get_generated_texture);
	ClassDB::bind_method(D_METHOD("get_generated_texture_count"), &TexturePacker::get_generated_texture_count);

	ClassDB::bind_method(D_METHOD("merge"), &TexturePacker::merge);
}
