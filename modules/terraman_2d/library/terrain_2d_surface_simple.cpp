

#include "terrain_2d_surface_simple.h"

#include "terrain_2d_library_simple.h"

int Terrain2DSurfaceSimple::get_atlas_x() const {
	return _atlas_position_x;
}
void Terrain2DSurfaceSimple::set_atlas_x(int value) {
	_atlas_position_x = value;
}

int Terrain2DSurfaceSimple::get_atlas_y() const {
	return _atlas_position_y;
}
void Terrain2DSurfaceSimple::set_atlas_y(int value) {
	_atlas_position_y = value;
}

void Terrain2DSurfaceSimple::refresh_rects() {
	Terrain2DLibrarySimple *lib = Object::cast_to<Terrain2DLibrarySimple>(_library);

	ERR_FAIL_COND(lib == NULL);

	float culomn = 1.0 / static_cast<float>(lib->get_atlas_columns());
	float row = 1.0 / static_cast<float>(lib->get_atlas_rows());

	Rect2 r;

	r.position.x = _atlas_position_x * culomn;
	r.position.y = _atlas_position_y * row;

	r.size.x = culomn;
	r.size.y = row;

	_rect = r;
}

Terrain2DSurfaceSimple::Terrain2DSurfaceSimple() {
	_atlas_position_x = 0;
	_atlas_position_y = 0;
}

Terrain2DSurfaceSimple::~Terrain2DSurfaceSimple() {
}

void Terrain2DSurfaceSimple::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_atlas_x"), &Terrain2DSurfaceSimple::get_atlas_x);
	ClassDB::bind_method(D_METHOD("set_atlas_x", "value"), &Terrain2DSurfaceSimple::set_atlas_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "atlas_x"), "set_atlas_x", "get_atlas_x");

	ClassDB::bind_method(D_METHOD("get_atlas_y"), &Terrain2DSurfaceSimple::get_atlas_y);
	ClassDB::bind_method(D_METHOD("set_atlas_y", "value"), &Terrain2DSurfaceSimple::set_atlas_y);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "atlas_y"), "set_atlas_y", "get_atlas_y");
}
