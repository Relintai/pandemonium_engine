#ifndef PROP_2D_DATA_MESH_H
#define PROP_2D_DATA_MESH_H


#include "modules/modules_enabled.gen.h"

#ifdef MODULE_PROPS_2D_ENABLED

#include "../../props_2d/props/prop_2d_data_entry.h"
#include "core/math/vector3.h"

#include "scene/resources/texture.h"

#include "../mesh_data_resource.h"

#ifdef MODULE_TEXTURE_PACKER_ENABLED
#include "../../texture_packer/texture_packer.h"
#endif

#include "../../props_2d/props/prop_2d_data.h"

class Prop2DDataMeshData : public Prop2DDataEntry {
	GDCLASS(Prop2DDataMeshData, Prop2DDataEntry);

public:
	Ref<MeshDataResource> get_mesh() const;
	void set_mesh(const Ref<MeshDataResource> mesh);

	Ref<Texture> get_texture() const;
	void set_texture(const Ref<Texture> texture);

	bool get_snap_to_mesh();
	void set_snap_to_mesh(bool value);

	Vector3 get_snap_axis();
	void set_snap_axis(Vector3 value);

#ifdef MODULE_TEXTURE_PACKER_ENABLED
	void _add_textures_into(Ref<TexturePacker> texture_packer);
#endif

	bool _processor_handles(Node *node);
	void _processor_process(Ref<Prop2DData> prop_data, Node *node, const Transform2D &transform, Ref<Prop2DDataEntry> entry = Ref<Prop2DDataEntry>());
	Node *_processor_get_node_for(const Transform2D &transform, Node *node = nullptr);

	Prop2DDataMeshData();
	~Prop2DDataMeshData();

protected:
	static void _bind_methods();

private:
	bool _snap_to_mesh;
	Vector3 _snap_axis;
	Ref<MeshDataResource> _mesh;
	Ref<Texture> _texture;
};

#endif

#endif
