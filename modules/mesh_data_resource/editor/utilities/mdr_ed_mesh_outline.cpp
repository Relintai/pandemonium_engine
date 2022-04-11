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

#include "mdr_ed_mesh_outline.h"

void MDREDMeshOutline::setup(Ref<MeshDataResource> mdr) {
	_mdr = mdr;
}
void MDREDMeshOutline::reset() {
	lines.resize(0);
	seam_lines.resize(0);
	_normals.resize(0);
	_vertices.resize(0);
	_indices.resize(0);
}
bool MDREDMeshOutline::initialize() {
	/*
	if !_mdr:
		return false

	if _mdr.array.size() != ArrayMesh.ARRAY_MAX:
		return false

	if _mdr.array[ArrayMesh.ARRAY_VERTEX] == null || _mdr.array[ArrayMesh.ARRAY_INDEX] == null:
		return false

	var arr : Array = _mdr.array

	_vertices = arr[ArrayMesh.ARRAY_VERTEX]

	if _mdr.array[ArrayMesh.ARRAY_NORMAL] != null:
		_normals = arr[ArrayMesh.ARRAY_NORMAL]

	_indices = arr[ArrayMesh.ARRAY_INDEX]

	if _vertices.size() == 0:
		return false

	return true
	*/
}
Vector3 MDREDMeshOutline::get_vertex(int index) {
	/*
	if index > _vertices.size():
		return Vector3()

	var v : Vector3 = _vertices[index]

	# This should reduce z fighting
	if _normals.size() > 0:
		var n : Vector3 = _normals[index]

		v += n * 0.001

	return v
	*/
}
void MDREDMeshOutline::generate(bool mark_outline, bool mark_handles) {
	/*
	reset()

	if !initialize():
		return

	if mark_outline:
		for i in range(0, _indices.size(), 3):
			for j in range(3):
				lines.append(get_vertex(_indices[i + j]))
				lines.append(get_vertex(_indices[i + ((j + 1) % 3)]))

	if mark_handles:
		for i in range(_vertices.size()):
			var v : Vector3 = get_vertex(i)

			var l : float = marker_size

			lines.append(v + Vector3(l, 0, 0))
			lines.append(v + Vector3(-l, 0, 0))
			lines.append(v + Vector3(0, 0, l))
			lines.append(v + Vector3(0, 0, -l))
			lines.append(v + Vector3(0, l, 0))
			lines.append(v + Vector3(0, -l, 0))

	var seams : PoolIntArray = _mdr.seams

	for i in range(0, seams.size(), 2):
		seam_lines.append(get_vertex(seams[i]))
		seam_lines.append(get_vertex(seams[i + 1]))
	*/
}
void MDREDMeshOutline::generate_mark_edges(bool mark_outline, bool mark_handles) {
	/*
	reset()

	if !initialize():
		return

	for i in range(0, _indices.size(), 3):
		for j in range(3):
			var i0 : int = _indices[i + j]
			var i1 : int = _indices[i + ((j + 1) % 3)]

			var v0 : Vector3 = get_vertex(i0)
			var v1 : Vector3 = get_vertex(i1)

			if mark_outline:
				lines.append(v0)
				lines.append(v1)

			if mark_handles:
				var pmid : Vector3 = lerp(v0, v1, 0.5)
				var l : float = marker_size

				lines.append(pmid + Vector3(l, 0, 0))
				lines.append(pmid + Vector3(-l, 0, 0))
				lines.append(pmid + Vector3(0, 0, l))
				lines.append(pmid + Vector3(0, 0, -l))
				lines.append(pmid + Vector3(0, l, 0))
				lines.append(pmid + Vector3(0, -l, 0))


	var seams : PoolIntArray = _mdr.seams

	for i in range(0, seams.size(), 2):
		seam_lines.append(get_vertex(seams[i]))
		seam_lines.append(get_vertex(seams[i + 1]))
	*/
}
void MDREDMeshOutline::generate_mark_faces(bool mark_outline, bool mark_handles) {
	/*
	reset()

	if !initialize():
		return

	if mark_outline:
		for i in range(0, _indices.size(), 3):
			for j in range(3):
				lines.append(get_vertex(_indices[i + j]))
				lines.append(get_vertex(_indices[i + ((j + 1) % 3)]))

	if mark_handles:
		for i in range(0, _indices.size(), 3):
				var i0 : int = _indices[i + 0]
				var i1 : int = _indices[i + 1]
				var i2 : int = _indices[i + 2]

				var v0 : Vector3 = get_vertex(i0)
				var v1 : Vector3 = get_vertex(i1)
				var v2 : Vector3 = get_vertex(i2)

				var pmid : Vector3 = v0 + v1 + v2
				pmid /= 3
				var l : float = marker_size

				lines.append(pmid + Vector3(l, 0, 0))
				lines.append(pmid + Vector3(-l, 0, 0))
				lines.append(pmid + Vector3(0, 0, l))
				lines.append(pmid + Vector3(0, 0, -l))
				lines.append(pmid + Vector3(0, l, 0))
				lines.append(pmid + Vector3(0, -l, 0))

	var seams : PoolIntArray = _mdr.seams

	for i in range(0, seams.size(), 2):
		seam_lines.append(get_vertex(seams[i]))
		seam_lines.append(get_vertex(seams[i + 1]))
	*/
}

MDREDMeshOutline::MDREDMeshOutline() {
	marker_size = 0.05;
}
