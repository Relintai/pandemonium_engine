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

#include "packer_image_resource.h"

#include "core/version.h"

int PackerImageResource::get_width() const {
	if (_image.is_valid()) {
		return _image->get_width();
	}

	return 0;
}

int PackerImageResource::get_height() const {
	if (_image.is_valid()) {
		return _image->get_height();
	}

	return 0;
}

RID PackerImageResource::get_rid() const {
	return RID();
}
bool PackerImageResource::has_alpha() const {
	return true;
}

void PackerImageResource::set_flags(uint32_t p_flags) {
}
uint32_t PackerImageResource::get_flags() const {
	return 0;
}

void PackerImageResource::draw(RID p_canvas_item, const Point2 &p_pos, const Color &p_modulate, bool p_transpose, const Ref<Texture> &p_normal_map) const {
}
void PackerImageResource::draw_rect(RID p_canvas_item, const Rect2 &p_rect, bool p_tile, const Color &p_modulate, bool p_transpose, const Ref<Texture> &p_normal_map) const {
}
void PackerImageResource::draw_rect_region(RID p_canvas_item, const Rect2 &p_rect, const Rect2 &p_src_rect, const Color &p_modulate, bool p_transpose, const Ref<Texture> &p_normal_map, bool p_clip_uv) const {
}
bool PackerImageResource::get_rect_region(const Rect2 &p_rect, const Rect2 &p_src_rect, Rect2 &r_rect, Rect2 &r_src_rect) const {
	return false;
}

void PackerImageResource::set_data(const Ref<Image> &p_image) {
	_image = p_image;
}

Ref<Image> PackerImageResource::get_data() const {
	return _image;
}

void PackerImageResource::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_data", "image"), &PackerImageResource::set_data);
#if VERSION_MAJOR < 4
	//for some reason this crashes on 4.0. Not yet sure why
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "data", PROPERTY_HINT_RESOURCE_TYPE, "Image", PROPERTY_USAGE_DEFAULT), "set_data", "get_data");
#endif
}

PackerImageResource::PackerImageResource() {
}

PackerImageResource::~PackerImageResource() {
	_image.unref();
}
