#ifndef TILED_WALL_H
#define TILED_WALL_H


#include "scene/resources/texture.h"

#include "scene/3d/visual_instance.h"

#include "core/math/vector3.h"

class TiledWallData;
class PropMaterialCache;
class PropMesher;

class TiledWall : public GeometryInstance {
	GDCLASS(TiledWall, GeometryInstance);

public:
	int get_width() const;
	void set_width(const int value);

	int get_heigth() const;
	void set_heigth(const int value);

	Ref<TiledWallData> get_data();
	void set_data(const Ref<TiledWallData> &data);

	bool get_collision() const;
	void set_collision(const bool value);

	uint32_t get_collision_layer() const;
	void set_collision_layer(uint32_t p_layer);

	uint32_t get_collision_mask() const;
	void set_collision_mask(uint32_t p_mask);

	AABB get_aabb() const;
	PoolVector<Face3> get_faces(uint32_t p_usage_flags) const;

	void refresh();
	void generate_mesh();
	void clear_mesh();
	void free_mesh();

	void create_colliders();
	void free_colliders();

	TiledWall();
	~TiledWall();

protected:
	void _notification(int p_what);
	static void _bind_methods();

private:
	int _width;
	int _height;
	bool _collision;

	uint32_t _collision_layer;
	uint32_t _collision_mask;

	Ref<TiledWallData> _data;
	Ref<PropMaterialCache> _cache;
	Ref<PropMesher> _mesher;
	AABB _aabb;

	RID _mesh_rid;
	RID _physics_shape_rid;
	RID _physics_body_rid;

	Array _mesh_array;
};

#endif
