#ifndef TILED_WALL_DATA_H
#define TILED_WALL_DATA_H
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

#include "core/reference.h"
#include "core/vector.h"

#include "core/math/rect2.h"
#include "core/math/transform.h"
#include "core/math/vector2.h"
#include "core/math/vector3.h"

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

	enum TiledWallColliderType {
		TILED_WALL_COLLIDER_TYPE_NONE = 0,
		TILED_WALL_COLLIDER_TYPE_PLANE,
		TILED_WALL_COLLIDER_TYPE_BOX,
		TILED_WALL_COLLIDER_TYPE_CONVEX_MESH,
		TILED_WALL_COLLIDER_TYPE_CONCAVE_MESH
	};

	static const String BINDING_STRING_TILED_WALL_TILING_TYPE;
	static const String BINDING_STRING_TILED_WALL_COLLIDER_TYPE;

public:
	TiledWallTilingType get_tiling_type() const;
	void set_tiling_type(const TiledWallTilingType value);

	TiledWallColliderType get_collider_type() const;
	void set_collider_type(const TiledWallColliderType value);

	//textures
	void add_tile(const Ref<Texture> &texture, const Vector2 &val = Vector2(1, 1), const float z_offset = 0);
	void remove_tile(const int index);

	Ref<Texture> get_tile_texture(const int index) const;
	void set_tile_texture(const int index, const Ref<Texture> &texture);

	Vector2 get_tile_size(const int index) const;
	void set_tile_size(const int index, const Vector2 &val);

	float get_tile_z_offset(const int index) const;
	void set_tile_z_offset(const int index, const float val);

	int get_tile_count() const;
	void set_tile_count(const int count);

	//flavour_textures
	void add_flavour_tile(const Ref<Texture> &texture, const Vector2 &val = Vector2(1, 1), const float z_offset = 0);
	void remove_flavour_tile(const int index);

	Ref<Texture> get_flavour_tile_texture(const int index) const;
	void set_flavour_tile_texture(const int index, const Ref<Texture> &texture);

	Vector2 get_flavour_tile_size(const int index) const;
	void set_flavour_tile_size(const int index, const Vector2 &val);

	float get_flavour_tile_z_offset(const int index) const;
	void set_flavour_tile_z_offset(const int index, const float val);

	int get_flavour_tile_count() const;
	void set_flavour_tile_count(const int count);

	float get_flavour_tile_chance() const;
	void set_flavour_tile_chance(const float value);

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
	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;

	static void _bind_methods();

private:
	struct TextureEntry {
		Ref<Texture> texture;
		Vector2 size;
		float z_offset;

		TextureEntry() {
			size = Vector2(1, 1);
			z_offset = 0;
		}

		TextureEntry(const Ref<Texture> &p_texture, const Vector2 &p_size = Vector2(1, 1), const float p_z_offset = 0) {
			texture = p_texture;
			size = p_size;
			z_offset = p_z_offset;
		}
	};

	TiledWallTilingType _tiling_type;
	TiledWallColliderType _collider_type;

	Vector<TextureEntry> _tiles;
	Vector<TextureEntry> _flavour_tiles;
	float _flavour_chance;

	Vector<Ref<Material>> _materials;
};

VARIANT_ENUM_CAST(TiledWallData::TiledWallTilingType);
VARIANT_ENUM_CAST(TiledWallData::TiledWallColliderType);

#endif
