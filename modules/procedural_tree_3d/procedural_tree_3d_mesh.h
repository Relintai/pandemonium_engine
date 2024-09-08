#ifndef PROCEDURAL_TREE_3D_MESH_H
#define PROCEDURAL_TREE_3D_MESH_H

#include "scene/resources/mesh/mesh.h"

class ProceduralTree3DMesh : public Mesh {
	GDCLASS(ProceduralTree3DMesh, Mesh);

public:
	enum TreeSurfaces {
		TREE_SURFACE_TRUNK = 0,
		TREE_SURFACE_TWIG = 1,
		TREE_SURFACE_COUNT,
	};

	virtual int get_surface_count() const;
	virtual int surface_get_array_len(int p_idx) const;
	virtual int surface_get_array_index_len(int p_idx) const;
	virtual Array surface_get_arrays(int p_surface) const;
	virtual Array surface_get_blend_shape_arrays(int p_surface) const;
	virtual uint32_t surface_get_format(int p_idx) const;
	virtual Mesh::PrimitiveType surface_get_primitive_type(int p_idx) const;
	virtual void surface_set_material(int p_idx, const Ref<Material> &p_material);
	virtual Ref<Material> surface_get_material(int p_idx) const;
	virtual int get_blend_shape_count() const;
	virtual StringName get_blend_shape_name(int p_index) const;
	virtual void set_blend_shape_name(int p_index, const StringName &p_name);
	virtual AABB get_aabb() const;
	virtual RID get_rid() const;

	void set_trunk_material(const Ref<Material> &p_material);
	Ref<Material> get_trunk_material() const;

	void set_twig_material(const Ref<Material> &p_material);
	Ref<Material> get_twig_material() const;

	Array get_mesh_arrays() const;

	void set_custom_aabb(const AABB &p_custom);
	AABB get_custom_aabb() const;

	ProceduralTree3DMesh();
	~ProceduralTree3DMesh();

protected:
	static void _bind_methods();

	void _request_update();

private:
	RID mesh;

	struct TreeSurface {
		Ref<Material> material;
	};

	TreeSurface _surfaces[TREE_SURFACE_COUNT];

	mutable AABB aabb;
	AABB custom_aabb;

	mutable bool pending_request;
	void _update() const;
};

VARIANT_ENUM_CAST(ProceduralTree3DMesh::TreeSurfaces);

#endif
