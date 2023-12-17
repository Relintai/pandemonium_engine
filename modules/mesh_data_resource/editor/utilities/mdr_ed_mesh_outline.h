#ifndef MDR_ED_MESH_OUTLINE_H
#define MDR_ED_MESH_OUTLINE_H



#include "core/object/reference.h"

#include "core/variant/array.h"
#include "core/containers/pool_vector.h"
#include "core/variant/variant.h"

class MeshDataResource;

class MDREDMeshOutline : public Reference {
	GDCLASS(MDREDMeshOutline, Reference);

public:
	void setup(Ref<MeshDataResource> mdr);
	void reset();
	bool initialize();
	Vector3 get_vertex(int index);
	void generate(bool mark_outline, bool mark_handles);
	void generate_mark_edges(bool mark_outline, bool mark_handles);
	void generate_mark_faces(bool mark_outline, bool mark_handles);

	MDREDMeshOutline();
	~MDREDMeshOutline();

	float marker_size;

	Ref<MeshDataResource> _mdr;

	Vector<Vector3> lines;
	Vector<Vector3> seam_lines;

	PoolVector3Array _vertices;
	PoolVector3Array _normals;
	PoolIntArray _indices;

protected:
	static void _bind_methods();
};

#endif
