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

#include "mdi_ed_uv_editor.h"

void MDIEdUVEditor::set_mesh_data_resource(Ref<MeshDataResource> a) {
	/*
	if mesh_data_resource:
		mesh_data_resource.disconnect("changed", self, "on_mdr_changed")

	mesh_data_resource = a

	if mesh_data_resource:
		mesh_data_resource.connect("changed", self, "on_mdr_changed")

	update()
	*/
}
void MDIEdUVEditor::set_mesh_data_instance(MeshDataInstance *a) {
	/*

	if !a:
		background_texture = null

	background_texture = a.texture

	*/
}
void MDIEdUVEditor::on_mdr_changed() {
	update();
}
void MDIEdUVEditor::_draw() {
	/*

	if background_texture:
		draw_texture_rect_region(background_texture, Rect2(Vector2(), get_size()), Rect2(Vector2(), background_texture.get_size()))

	if !mesh_data_resource:
		return

	if mesh_data_resource.array.size() != ArrayMesh.ARRAY_MAX:
		return

	var uvs : PoolVector2Array = mesh_data_resource.array[ArrayMesh.ARRAY_TEX_UV]
	var indices : PoolIntArray = mesh_data_resource.array[ArrayMesh.ARRAY_INDEX]

	if indices.size() % 3 == 0:
		for i in range(0, len(indices), 3):
			var c : Color = Color(1, 1, 1, 1)

			if uvs[indices[i]].is_equal_approx(Vector2()) || uvs[indices[i + 1]].is_equal_approx(Vector2()):
				c = Color(1, 0, 0, 1)
			else:
				c = Color(1, 1, 1, 1)

			draw_line(uvs[indices[i]] * get_size(), uvs[indices[i + 1]] * get_size(), c, 1, false)

			if uvs[indices[i + 1]].is_equal_approx(Vector2()) || uvs[indices[i + 2]].is_equal_approx(Vector2()):
				c = Color(1, 0, 0, 1)
			else:
				c = Color(1, 1, 1, 1)

			draw_line(uvs[indices[i + 1]] * get_size(), uvs[indices[i + 2]] * get_size(), c, 1, false)

			if uvs[indices[i + 2]].is_equal_approx(Vector2()) || uvs[indices[i]].is_equal_approx(Vector2()):
				c = Color(1, 0, 0, 1)
			else:
				c = Color(1, 1, 1, 1)

			draw_line(uvs[indices[i + 2]] * get_size(), uvs[indices[i]] * get_size(), c, 1, false)


	*/
}

MDIEdUVEditor::MDIEdUVEditor() {
}

MDIEdUVEditor::~MDIEdUVEditor() {
}

void MDIEdUVEditor::_bind_methods() {
}
