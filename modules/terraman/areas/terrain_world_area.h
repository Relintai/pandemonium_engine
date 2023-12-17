#ifndef TERRAIN_WORLD_AREA_H
#define TERRAIN_WORLD_AREA_H


#include "core/object/reference.h"
#include "core/string/ustring.h"

#include "core/math/aabb.h"

#include "scene/resources/texture.h"

class TerrainWorldArea : public Reference {
	GDCLASS(TerrainWorldArea, Reference);

public:
	AABB get_aabb() const;
	void set_aabb(const AABB value);

	Ref<Texture> get_map_texture() const;
	void set_map_texture(const Ref<Texture> &value);

	Ref<Texture> get_fov_texture() const;
	void set_fov_texture(const Ref<Texture> &value);

	String get_name() const;
	void set_name(const String &value);

	int get_level() const;
	void set_level(const int value);

	TerrainWorldArea();
	~TerrainWorldArea();

private:
	static void _bind_methods();

private:
	AABB _aabb;
	Ref<Texture> _map_texture;
	Ref<Texture> _fov_texture;
	String _name;
	int _level;
};

#endif
