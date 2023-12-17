

#include "ground_clutter_2d_foliage.h"

int GroundClutter2DFoliage::get_texture_count() const {
	return _textures.size();
}
Ref<Texture> GroundClutter2DFoliage::get_texture(const int index) {
	ERR_FAIL_INDEX_V(index, _textures.size(), Ref<Texture>());

	return _textures.get(index);
}
void GroundClutter2DFoliage::remove_texture(const int index) {
	ERR_FAIL_INDEX(index, _textures.size());

	_textures.remove(index);
}
void GroundClutter2DFoliage::add_texture(Ref<Texture> texture) {
	_textures.push_back(texture);
}

GroundClutter2DFoliage::GroundClutter2DFoliage() {
}

GroundClutter2DFoliage::~GroundClutter2DFoliage() {
	_textures.clear();
}

void GroundClutter2DFoliage::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_texture_count"), &GroundClutter2DFoliage::get_texture_count);
	ClassDB::bind_method(D_METHOD("get_texture", "index"), &GroundClutter2DFoliage::get_texture);
	ClassDB::bind_method(D_METHOD("remove_texture", "index"), &GroundClutter2DFoliage::remove_texture);
	ClassDB::bind_method(D_METHOD("add_texture", "texture"), &GroundClutter2DFoliage::add_texture);
}
