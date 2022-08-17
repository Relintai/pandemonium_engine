#ifndef MDR_ED_MESH_OUTLINE_H
#define MDR_ED_MESH_OUTLINE_H

/*
Copyright (c) 2019-2022 Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

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
