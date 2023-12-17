#ifndef PROP_DATA_MESH_H
#define PROP_DATA_MESH_H


#include "modules/modules_enabled.gen.h"

#ifdef MODULE_PROPS_ENABLED

#include "../../props/props/prop_data_entry.h"
#include "core/math/vector3.h"

#include "scene/resources/texture.h"

#include "../mesh_data_resource.h"

#ifdef MODULE_TEXTURE_PACKER_ENABLED
#include "../../texture_packer/texture_packer.h"
#endif

#include "../../props/props/prop_data.h"

class PropDataMeshData : public PropDataEntry {
	GDCLASS(PropDataMeshData, PropDataEntry);

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
	void _processor_process(Ref<PropData> prop_data, Node *node, const Transform &transform);
	Node *_processor_get_node_for(const Transform &transform);

	PropDataMeshData();
	~PropDataMeshData();

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
