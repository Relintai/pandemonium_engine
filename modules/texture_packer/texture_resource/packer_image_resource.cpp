

#include "packer_image_resource.h"

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

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "data", PROPERTY_HINT_RESOURCE_TYPE, "Image", PROPERTY_USAGE_DEFAULT), "set_data", "get_data");
}

PackerImageResource::PackerImageResource() {
}

PackerImageResource::~PackerImageResource() {
	_image.unref();
}
