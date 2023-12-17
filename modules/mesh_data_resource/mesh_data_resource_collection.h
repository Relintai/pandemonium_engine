#ifndef MESH_DATA_REOURCE_COLLECTION_H
#define MESH_DATA_REOURCE_COLLECTION_H


#include "core/object/resource.h"
#include "core/containers/vector.h"

#include "mesh_data_resource.h"

class MeshDataResourceCollection : public Resource {
	GDCLASS(MeshDataResourceCollection, Resource);

public:
	void add_mdr(Ref<MeshDataResource> mdr);

	Vector<Variant> get_mdrs();
	void set_mdrs(const Vector<Variant> &p_arrays);

	MeshDataResourceCollection();
	~MeshDataResourceCollection();

protected:
	static void _bind_methods();

private:
	Vector<Ref<MeshDataResource>> _mdrs;
};

#endif
