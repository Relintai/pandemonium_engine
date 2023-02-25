#ifndef TILED_WALL_DATA_H
#define TILED_WALL_DATA_H
/*
Copyright (c) 2019-2023 Péter Magyar

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

#include "core/containers/vector.h"
#include "core/object/reference.h"

#include "core/math/rect2.h"
#include "core/math/transform.h"
#include "core/math/vector2.h"
#include "core/math/vector3.h"

#include "scene/resources/material.h"
#include "scene/resources/texture.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_TEXTURE_PACKER_ENABLED
#include "../../texture_packer/texture_packer.h"
#endif

class PropMaterialCache;
class PropMesher;
#ifdef MODULE_ENTITY_SPELL_SYSTEM_ENABLED
class ESSMaterialCache;
#endif
#ifdef MODULE_FASTNOISE_ENABLED
class FastnoiseNoiseParams;
#endif

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

	struct TextureEntry {
		Ref<Texture> texture;
		float y_size;
		float z_offset;
		int texture_scale;

		TextureEntry() {
			y_size = 1;
			z_offset = 0;
			texture_scale = 1;
		}

		TextureEntry(const Ref<Texture> &p_texture, const float p_y_size = 1, const float p_z_offset = 0, const float p_texture_scale = 0) {
			texture = p_texture;
			y_size = p_y_size;
			z_offset = p_z_offset;
			texture_scale = p_texture_scale;
		}
	};

	static const String BINDING_STRING_TILED_WALL_TILING_TYPE;
	static const String BINDING_STRING_TILED_WALL_COLLIDER_TYPE;

public:
	TiledWallTilingType get_tiling_type() const;
	void set_tiling_type(const TiledWallTilingType value);

	TiledWallColliderType get_collider_type() const;
	void set_collider_type(const TiledWallColliderType value);

	float get_collider_z_offset();
	void set_collider_z_offset(const float val);

	//textures
	void add_tile(const Ref<Texture> &texture, const float y_size = 1, const float z_offset = 0, const int texture_scale = 1);
	void remove_tile(const int index);
	TextureEntry get_tile(const int index) const;

	Ref<Texture> get_tile_texture(const int index) const;
	void set_tile_texture(const int index, const Ref<Texture> &texture);

	float get_tile_y_size(const int index) const;
	void set_tile_y_size(const int index, const float val);

	float get_tile_z_offset(const int index) const;
	void set_tile_z_offset(const int index, const float val);

	int get_tile_texture_scale(const int index) const;
	void set_tile_texture_scale(const int index, const int val);

	int get_tile_count() const;
	void set_tile_count(const int count);

	//flavour_textures
	void add_flavour_tile(const Ref<Texture> &texture, const float y_size = 1, const float z_offset = 0, const int texture_scale = 1);
	void remove_flavour_tile(const int index);
	TextureEntry get_flavour_tile(const int index) const;

	Ref<Texture> get_flavour_tile_texture(const int index) const;
	void set_flavour_tile_texture(const int index, const Ref<Texture> &texture);

	float get_flavour_tile_y_size(const int index) const;
	void set_flavour_tile_y_size(const int index, const float val);

	float get_flavour_tile_z_offset(const int index) const;
	void set_flavour_tile_z_offset(const int index, const float val);

	int get_flavour_tile_texture_scale(const int index) const;
	void set_flavour_tile_texture_scale(const int index, const int val);

	int get_flavour_tile_count() const;
	void set_flavour_tile_count(const int count);

	float get_flavour_tile_chance() const;
	void set_flavour_tile_chance(const float value);

#ifdef MODULE_FASTNOISE_ENABLED
	Ref<FastnoiseNoiseParams> get_offset_noise();
	void set_offset_noise(const Ref<FastnoiseNoiseParams> &val);

	float get_offset_noise_strength() const;
	void set_offset_noise_strength(const float val);

	bool get_offset_noise_randomize_seed() const;
	void set_offset_noise_randomize_seed(const bool val);

	bool get_offset_noise_skip_edges() const;
	void set_offset_noise_skip_edges(const bool val);
#endif

	//materials
	void material_add(const Ref<Material> &value);
	void material_set(const int index, const Ref<Material> &value);
	void material_remove(const int index);
	int material_get_num() const;
	void materials_clear();

	Vector<Variant> materials_get();
	void materials_set(const Vector<Variant> &materials);

#ifdef MODULE_TEXTURE_PACKER_ENABLED
	void add_textures_into(Ref<TexturePacker> texture_packer);
#endif

	void setup_cache(Ref<PropMaterialCache> cache);
	void _setup_cache(Ref<PropMaterialCache> cache);

#ifdef MODULE_ENTITY_SPELL_SYSTEM_ENABLED
	void setup_ess_cache(Ref<ESSMaterialCache> cache);
	void _setup_ess_cache(Ref<ESSMaterialCache> cache);
#endif

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
	TiledWallTilingType _tiling_type;
	TiledWallColliderType _collider_type;
	float _collider_z_offset;

	Vector<TextureEntry> _tiles;
	Vector<TextureEntry> _flavour_tiles;
	float _flavour_chance;

#ifdef MODULE_FASTNOISE_ENABLED
	Ref<FastnoiseNoiseParams> _offset_noise;
	float _offset_noise_strength;
	bool _offset_noise_randomize_seed;
	bool _offset_noise_skip_edges;
#endif

	Vector<Ref<Material>> _materials;
};

VARIANT_ENUM_CAST(TiledWallData::TiledWallTilingType);
VARIANT_ENUM_CAST(TiledWallData::TiledWallColliderType);

#endif
