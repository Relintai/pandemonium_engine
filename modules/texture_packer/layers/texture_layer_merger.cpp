

#include "texture_layer_merger.h"

int TextureLayerMerger::get_width() const {
	return _width;
}

void TextureLayerMerger::set_width(const int p_value) {
	_width = p_value;
}

int TextureLayerMerger::get_height() const {
	return _height;
}

void TextureLayerMerger::set_height(const int p_value) {
	_height = p_value;
}

uint32_t TextureLayerMerger::get_texture_flags() const {
	return _texture_flags;
}
void TextureLayerMerger::set_texture_flags(uint32_t p_flags) {
	_texture_flags = p_flags;
}

Color TextureLayerMerger::get_base_color() const {
	return _base_color;
}
void TextureLayerMerger::set_base_color(const Color &p_color) {
	_base_color = p_color;
}

Ref<Image> TextureLayerMerger::get_data() const {
	return _image;
}

void TextureLayerMerger::set_data(const Ref<Image> &p_image) {
	ERR_FAIL_COND(p_image.is_null());

	_image = p_image;
}

Ref<ImageTexture> TextureLayerMerger::get_result_as_texture() const {
	ERR_FAIL_COND_V(!_image.is_valid(), Ref<ImageTexture>());

	Ref<ImageTexture> tex;
	tex.instance();
	tex->create_from_image(_image);

	return tex;
}

void TextureLayerMerger::add_texture(const Ref<Texture> &p_texture, const Color &p_color, const Vector2 &p_position, Rect2 p_rect) {
	ERR_FAIL_COND(!p_texture.is_valid());

	LayerMergerEntry entry;

	entry.texture = p_texture;
	entry.color = p_color;
	entry.position = p_position;

	if (p_rect.size.x <= 0)
		p_rect.size.x = p_texture->get_width();

	if (p_rect.size.y <= 0)
		p_rect.size.y = p_texture->get_height();

	entry.rect = p_rect;

	if (_width == 0 || _height == 0) {
		Ref<AtlasTexture> at = p_texture;

		int w = 0;
		int h = 0;

		if (at.is_valid()) {
			w = at->get_region().size.x;
			h = at->get_region().size.y;
		} else {
			w = p_texture->get_width();
			h = p_texture->get_height();
		}

		if (_width == 0)
			_width = w;

		if (_height == 0)
			_height = h;
	}

	_entries.push_back(entry);
}

Ref<Texture> TextureLayerMerger::get_texture(const int p_index) {
	ERR_FAIL_INDEX_V(p_index, _entries.size(), Ref<Texture>());

	return _entries.get(p_index).texture;
}
void TextureLayerMerger::set_texture(const int p_index, const Ref<Texture> &p_texture) {
	ERR_FAIL_INDEX(p_index, _entries.size());

	_entries.get(p_index).texture = p_texture;
}

Color TextureLayerMerger::get_color(const int p_index) {
	ERR_FAIL_INDEX_V(p_index, _entries.size(), Color());

	return _entries.get(p_index).color;
}
void TextureLayerMerger::set_color(const int p_index, const Color &p_color) {
	ERR_FAIL_INDEX(p_index, _entries.size());

	_entries.get(p_index).color = p_color;
}

Vector2 TextureLayerMerger::get_position(const int p_index) {
	ERR_FAIL_INDEX_V(p_index, _entries.size(), Vector2());

	return _entries.get(p_index).position;
}
void TextureLayerMerger::set_position(const int p_index, const Vector2 &p_position) {
	ERR_FAIL_INDEX(p_index, _entries.size());

	_entries.get(p_index).position = p_position;
}

Rect2 TextureLayerMerger::get_rect(const int p_index) {
	ERR_FAIL_INDEX_V(p_index, _entries.size(), Rect2());

	return _entries.get(p_index).rect;
}
void TextureLayerMerger::set_rect(const int p_index, const Rect2 &p_rect) {
	ERR_FAIL_INDEX(p_index, _entries.size());

	_entries.get(p_index).rect = p_rect;
}

void TextureLayerMerger::remove_texture(const int p_index) {
	ERR_FAIL_INDEX(p_index, _entries.size());

	return _entries.remove(p_index);
}

int TextureLayerMerger::get_texture_count() {
	return _entries.size();
}

void TextureLayerMerger::clear() {
	_entries.clear();
}

void TextureLayerMerger::merge() {
	ERR_FAIL_COND(_width <= 0 || _height <= 0);

	if (!_image.is_valid()) {
		_image.instance();
	}

	PoolVector<uint8_t> data;
	data.resize(_width * _height * 4);

	write_base_color_to_array(data);

	for (int i = 0; i < _entries.size(); ++i) {
		const LayerMergerEntry &e = _entries[i];

		ERR_CONTINUE(!e.texture.is_valid());

		int rx = e.rect.position.x + 0.1;
		int ry = e.rect.position.y + 0.1;
		int rw = e.rect.size.x + 0.1;
		int rh = e.rect.size.y + 0.1;

		int posx = e.position.x + 0.1;
		int posy = e.position.y + 0.1;

		int atlas_x = 0;
		int atlas_y = 0;

		if (posx > _width || posy > _height)
			continue;

		Ref<AtlasTexture> altas_texture = e.texture;
		Ref<Image> input_image;

		if (altas_texture.is_valid()) {
			Ref<Texture> atlas = altas_texture->get_atlas();

			ERR_CONTINUE(!atlas.is_valid());

			input_image = atlas->get_data();

			Rect2 region = altas_texture->get_region();

			atlas_x = region.position.x + 0.1;
			atlas_y = region.position.y + 0.1;

			int atlas_w = region.size.x + 0.1;
			int atlas_h = region.size.y + 0.1;

			if (rw > atlas_w)
				rw = atlas_w;

			if (rh > atlas_h)
				rh = atlas_h;
		} else {
			input_image = e.texture->get_data();
		}

		ERR_CONTINUE(!input_image.is_valid());

		int iiw = input_image->get_width();
		int iih = input_image->get_height();
		PoolVector<uint8_t> input_image_data = input_image->get_data();

		const Color &blendcolor = e.color;

		float blend_arr[] = {
			blendcolor.r,
			blendcolor.g,
			blendcolor.b,
			blendcolor.a
		};

		ERR_CONTINUE(iiw == 0 || iih == 0);
		ERR_CONTINUE(rx > iiw || ry > iih);

		//clamp width, and height if bigger (for ease of use)
		if (atlas_x + rx + rw >= iiw)
			rw -= (atlas_x + rx + rw) - iiw;

		if (atlas_y + ry + rh >= iih)
			rh -= (atlas_y + ry + rh) - iih;

		//Let's take position into account
		if (rx + rw + posx >= _width)
			rw -= (rx + rw + posx) - _width;

		if (ry + rh + posy >= _height)
			rh -= (ry + rh + posy) - _height;

		if (rw <= 0 || rh <= 0)
			continue;

		int elen = 0;

		if (input_image->get_format() == Image::FORMAT_RGBA8) {
			elen = 4;
		} else if (input_image->get_format() == Image::FORMAT_RGB8) {
			elen = 3;
		}

		ERR_CONTINUE_MSG(elen == 0, "Unsupported image format! Format: " + String::num(input_image->get_format()));

		for (int y = 0; y < rh; ++y) {
			int img_gen_index = (posx + ((y + posy) * _width)) * 4;
			int img_input_index = (rx + atlas_x + ((y + atlas_y + ry) * iiw)) * elen;

			for (int x = 0; x < rw; ++x) {
				float blend_alpha = blendcolor.a;

				if (elen == 4) {
					float orig_alpha = input_image_data.get(img_input_index + 3) / 255.0;

					blend_alpha -= 1.0 - orig_alpha;

					if (blend_alpha < 0)
						blend_alpha = 0;

					blend_alpha = orig_alpha;
				}

				for (int sp = 0; sp < elen; ++sp) {
					int main_index = img_gen_index + sp;

					int main_val = data.get(main_index);
					int input_val = input_image_data.get(img_input_index + sp);

					main_val = (input_val * blend_arr[sp]) * blend_alpha + (main_val * (1 - blend_alpha));

					if (main_val > 255)
						main_val = 255;

					data.set(main_index, main_val);
				}

				img_gen_index += 4;
				img_input_index += elen;
			}
		}
	}

	_image->create(_width, _height, (_texture_flags & Texture::FLAG_MIPMAPS) != 0, Image::FORMAT_RGBA8, data);
}

void TextureLayerMerger::write_base_color_to_array(PoolVector<uint8_t> &data) {
	int cr = _base_color.r * 255;
	int cg = _base_color.g * 255;
	int cb = _base_color.b * 255;
	int ca = _base_color.a * 255;

	for (int i = 0; i < data.size(); i += 4) {
		data.set(i, cr);
		data.set(i + 1, cg);
		data.set(i + 2, cb);
		data.set(i + 3, ca);
	}
}

void TextureLayerMerger::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_width"), &TextureLayerMerger::get_width);
	ClassDB::bind_method(D_METHOD("set_width", "image"), &TextureLayerMerger::set_width);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "width"), "set_width", "get_width");

	ClassDB::bind_method(D_METHOD("get_height"), &TextureLayerMerger::get_height);
	ClassDB::bind_method(D_METHOD("set_height", "image"), &TextureLayerMerger::set_height);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "height"), "set_height", "get_height");

	ClassDB::bind_method(D_METHOD("get_texture_flags"), &TextureLayerMerger::get_texture_flags);
	ClassDB::bind_method(D_METHOD("set_texture_flags", "image"), &TextureLayerMerger::set_texture_flags);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "texture_flags", PROPERTY_HINT_FLAGS, "Mipmaps,Repeat,Filter"), "set_texture_flags", "get_texture_flags");

	ClassDB::bind_method(D_METHOD("get_base_color"), &TextureLayerMerger::get_base_color);
	ClassDB::bind_method(D_METHOD("set_base_color", "color"), &TextureLayerMerger::set_base_color);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "base_color"), "set_base_color", "get_base_color");

	ClassDB::bind_method(D_METHOD("get_data"), &TextureLayerMerger::get_data);
	ClassDB::bind_method(D_METHOD("set_data", "image"), &TextureLayerMerger::set_data);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "data", PROPERTY_HINT_RESOURCE_TYPE, "Image"), "set_data", "get_data");

	ClassDB::bind_method(D_METHOD("get_result_as_texture"), &TextureLayerMerger::get_result_as_texture);

	ClassDB::bind_method(D_METHOD("add_texture", "texture", "color", "position", "rect"), &TextureLayerMerger::add_texture, DEFVAL(Color(1, 1, 1, 1)), DEFVAL(Vector2()), DEFVAL(Rect2()));

	ClassDB::bind_method(D_METHOD("get_texture", "index"), &TextureLayerMerger::get_texture);
	ClassDB::bind_method(D_METHOD("set_texture", "index", "texture"), &TextureLayerMerger::set_texture);

	ClassDB::bind_method(D_METHOD("get_color", "index"), &TextureLayerMerger::get_color);
	ClassDB::bind_method(D_METHOD("set_color", "index", "color"), &TextureLayerMerger::set_color);

	ClassDB::bind_method(D_METHOD("get_position", "index"), &TextureLayerMerger::get_position);
	ClassDB::bind_method(D_METHOD("set_position", "index", "position"), &TextureLayerMerger::set_position);

	ClassDB::bind_method(D_METHOD("get_rect", "index"), &TextureLayerMerger::get_rect);
	ClassDB::bind_method(D_METHOD("set_rect", "index", "rect"), &TextureLayerMerger::set_rect);

	ClassDB::bind_method(D_METHOD("remove_texture", "index"), &TextureLayerMerger::remove_texture);
	ClassDB::bind_method(D_METHOD("get_texture_count"), &TextureLayerMerger::get_texture_count);
	ClassDB::bind_method(D_METHOD("clear"), &TextureLayerMerger::clear);

	ClassDB::bind_method(D_METHOD("merge"), &TextureLayerMerger::merge);
}

TextureLayerMerger::TextureLayerMerger() {
	_width = 0;
	_height = 0;

	_texture_flags = 0;
}

TextureLayerMerger::~TextureLayerMerger() {
	_image.unref();
	_entries.clear();
}
