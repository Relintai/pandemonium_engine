#ifndef PROP_MESH_DATA_INSTANCE_2D_H
#define PROP_MESH_DATA_INSTANCE_2D_H


#include "scene/resources/texture.h"

#include "scene/main/node_2d.h"

#include "core/math/vector3.h"

#include "../mesh_data_resource.h"

class PropInstance;

class MeshDataInstance2D : public Node2D {
	GDCLASS(MeshDataInstance2D, Node2D);

public:
	Ref<MeshDataResource> get_mesh_data();
	void set_mesh_data(const Ref<MeshDataResource> &mesh);

	Ref<Texture> get_texture();
	void set_texture(const Ref<Texture> &texture);

	Ref<Texture> get_normal_map();
	void set_normal_map(const Ref<Texture> &texture);

	void refresh();

#ifdef TOOLS_ENABLED
	virtual Rect2 _edit_get_rect() const;
	virtual bool _edit_use_rect() const;
#endif

	MeshDataInstance2D();
	~MeshDataInstance2D();

protected:
	void _notification(int p_what);
	static void _bind_methods();

private:
	Ref<MeshDataResource> _mesh;
	Ref<Texture> _texture;
	Ref<Texture> _normal_map;

	RID _mesh_rid;
};

#endif
