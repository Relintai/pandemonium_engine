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

#ifndef TILED_WALL_DATA_H
#define TILED_WALL_DATA_H

#include "core/version.h"

#if VERSION_MAJOR > 3
#include "core/object/reference.h"
#include "core/templates/vector.h"
#else
#include "core/reference.h"
#include "core/vector.h"
#endif

#include "core/math/rect2.h"
#include "core/math/transform.h"
#include "core/math/vector2.h"
#include "core/math/vector3.h"

#include "core/version.h"

#include "scene/resources/material.h"
#include "scene/resources/texture.h"

#if TEXTURE_PACKER_PRESENT
#include "../../texture_packer/texture_packer.h"
#endif

class PropMaterialCache;
class PropMesher;

class TiledWallData : public Resource {
	GDCLASS(TiledWallData, Resource);

public:
	enum TiledWallTilingType {
		TILED_WALL_TILING_TYPE_NONE = 0,
		TILED_WALL_TILING_TYPE_HORIZONTAL,
		TILED_WALL_TILING_TYPE_VERTICAL,
		TILED_WALL_TILING_TYPE_BOTH
	};

	static const String BINDING_STRING_TILED_WALL_TILING_TYPE;

public:
	TiledWallTilingType get_tiling_type() const;
	void set_tiling_type(const TiledWallTilingType value);

	//textures
	Ref<Texture> get_texture(const int index) const;
	void set_texture(const int index, const Ref<Texture> &texture);
	void add_texture(const Ref<Texture> &texture);
	void remove_texture(const int index);

	int get_texture_count() const;

	Vector<Variant> get_textures();
	void set_textures(const Vector<Variant> &textures);

	//flavour_textures
	Ref<Texture> get_flavour_texture(const int index) const;
	void set_flavour_texture(const int index, const Ref<Texture> &texture);
	void add_flavour_texture(const Ref<Texture> &texture);
	void remove_flavour_texture(const int index);

	int get_flavour_texture_count() const;

	Vector<Variant> get_flavour_textures();
	void set_flavour_textures(const Vector<Variant> &textures);

	float get_flavour_chance() const;
	void set_flavour_chance(const float value);

	//materials
	void material_add(const Ref<Material> &value);
	void material_set(const int index, const Ref<Material> &value);
	void material_remove(const int index);
	int material_get_num() const;
	void materials_clear();

	Vector<Variant> materials_get();
	void materials_set(const Vector<Variant> &materials);

#if TEXTURE_PACKER_PRESENT
	void add_textures_into(Ref<TexturePacker> texture_packer);
#endif

	void setup_cache(Ref<PropMaterialCache> cache);
	void _setup_cache(Ref<PropMaterialCache> cache);

	void copy_from(const Ref<TiledWallData> &tiled_wall_data);

	//Ref<Shape> get_collider_shape();

	TiledWallData();
	~TiledWallData();

protected:
	static void _bind_methods();

private:
	TiledWallTilingType _tiling_type;

	Vector<Ref<Texture>> _textures;
	Vector<Ref<Texture>> _flavour_textures;
	float _flavour_chance;

	Vector<Ref<Material>> _materials;
};

VARIANT_ENUM_CAST(TiledWallData::TiledWallTilingType);

#endif
