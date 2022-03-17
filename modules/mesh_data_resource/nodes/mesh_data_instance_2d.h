#ifndef PROP_MESH_DATA_INSTANCE_2D_H
#define PROP_MESH_DATA_INSTANCE_2D_H
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




#include "core/version.h"
#include "scene/resources/texture.h"

#include "scene/2d/node_2d.h"

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
