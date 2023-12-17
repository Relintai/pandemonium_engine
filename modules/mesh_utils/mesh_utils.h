#ifndef MESH_UTILS_H
#define MESH_UTILS_H


#include "core/variant/variant.h"

#include "core/object/object.h"

#include "scene/resources/texture.h"

#include "defines.h"

class MeshUtils : public Object {
	GDCLASS(MeshUtils, Object);

public:
	static MeshUtils *get_singleton();

	Array merge_mesh_array(Array arr) const;
	Array bake_mesh_array_uv(Array arr, Ref<Texture> tex, float mul_color = 0.7) const;
	//If normals are present they need to match too to be removed
	Array remove_doubles(Array arr) const;
	//Normals are always interpolated, merged
	Array remove_doubles_interpolate_normals(Array arr) const;

	//Only unwraps, does not create new seams
	PoolVector2Array uv_unwrap(Array arr, bool p_block_align = true, float p_texel_size = 0.05, int p_padding = 1, int p_max_chart_size = 4094) const;

	PoolIntArray delaunay3d_tetrahedralize(const Vector<Vector3> &p_points);

	MeshUtils();
	~MeshUtils();

protected:
	static void _bind_methods();

private:
	static MeshUtils *_instance;
};

#endif
