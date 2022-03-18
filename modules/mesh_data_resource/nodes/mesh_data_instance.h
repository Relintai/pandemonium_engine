#ifndef PROP_MESH_DATA_INSTANCE_H
#define PROP_MESH_DATA_INSTANCE_H
/*
Copyright (c) 2020-2022 Péter Magyar

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
