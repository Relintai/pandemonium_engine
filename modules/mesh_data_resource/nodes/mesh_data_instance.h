#ifndef PROP_MESH_DATA_INSTANCE_H
#define PROP_MESH_DATA_INSTANCE_H


#include "scene/resources/texture.h"

#include "scene/3d/visual_instance.h"

#include "core/math/vector3.h"

#include "../mesh_data_resource.h"

class PropInstance;

class MeshDataInstance : public GeometryInstance {
	GDCLASS(MeshDataInstance, GeometryInstance);

public:
	bool get_snap_to_mesh() const;
	void set_snap_to_mesh(const bool value);

	Vector3 get_snap_axis() const;
	void set_snap_axis(const Vector3 &value);

	Ref<MeshDataResource> get_mesh_data();
	void set_mesh_data(const Ref<MeshDataResource> &mesh);

	Ref<Texture> get_texture();
	void set_texture(const Ref<Texture> &texture);

	Ref<Material> get_material();
	void set_material(const Ref<Material> &mat);

	AABB get_aabb() const;
	PoolVector<Face3> get_faces(uint32_t p_usage_flags) const;

	void refresh();
	void setup_material_texture();
	void free_meshes();

	MeshDataInstance();
	~MeshDataInstance();

protected:
	void _notification(int p_what);
	static void _bind_methods();

private:
	bool _dirty;
	bool _snap_to_mesh;
	Vector3 _snap_axis;
	Ref<MeshDataResource> _mesh;
	Ref<Texture> _texture;
	Ref<Material> _material;

	RID _mesh_rid;
};

#endif
