

#include "mdr_ed_mesh_outline.h"

#include "../../mesh_data_resource.h"

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
	if (!_mdr.is_valid()) {
		return false;
	}

	Array arr = _mdr->get_array();

	if (arr.size() != ArrayMesh::ARRAY_MAX) {
		return false;
	}

	if (arr[ArrayMesh::ARRAY_VERTEX].is_null() || arr[ArrayMesh::ARRAY_INDEX].is_null()) {
		return false;
	}

	_vertices = arr[ArrayMesh::ARRAY_VERTEX];

	if (!arr[ArrayMesh::ARRAY_NORMAL].is_null()) {
		_normals = arr[ArrayMesh::ARRAY_NORMAL];
	}

	_indices = arr[ArrayMesh::ARRAY_INDEX];

	if (_vertices.size() == 0) {
		return false;
	}

	return true;
}
Vector3 MDREDMeshOutline::get_vertex(int index) {
	if (index > _vertices.size()) {
		return Vector3();
	}

	Vector3 v = _vertices[index];

	// This should reduce z fighting
	if (_normals.size() > 0) {
		Vector3 n = _normals[index];

		v += n * 0.001;
	}

	return v;
}

void MDREDMeshOutline::generate(bool mark_outline, bool mark_handles) {
	reset();

	if (!initialize()) {
		return;
	}

	if (mark_outline) {
		for (int i = 0; i < _indices.size(); i += 3) {
			for (int j = 0; j < 3; ++j) {
				lines.push_back(get_vertex(_indices[i + j]));
				lines.push_back(get_vertex(_indices[i + ((j + 1) % 3)]));
			}
		}
	}

	if (mark_handles) {
		for (int i = 0; i < _vertices.size(); ++i) {
			Vector3 v = get_vertex(i);

			float l = marker_size;

			lines.push_back(v + Vector3(l, 0, 0));
			lines.push_back(v + Vector3(-l, 0, 0));
			lines.push_back(v + Vector3(0, 0, l));
			lines.push_back(v + Vector3(0, 0, -l));
			lines.push_back(v + Vector3(0, l, 0));
			lines.push_back(v + Vector3(0, -l, 0));
		}
	}

	PoolIntArray seams = _mdr->get_seams();

	for (int i = 0; i < seams.size(); i += 2) {
		seam_lines.push_back(get_vertex(seams[i]));
		seam_lines.push_back(get_vertex(seams[i + 1]));
	}
}

void MDREDMeshOutline::generate_mark_edges(bool mark_outline, bool mark_handles) {
	reset();

	if (!initialize()) {
		return;
	}

	for (int i = 0; i < _indices.size(); i += 3) {
		for (int j = 0; j < 3; ++j) {
			int i0 = _indices[i + j];
			int i1 = _indices[i + ((j + 1) % 3)];

			Vector3 v0 = get_vertex(i0);
			Vector3 v1 = get_vertex(i1);

			if (mark_outline) {
				lines.push_back(v0);
				lines.push_back(v1);
			}

			if (mark_handles) {
				Vector3 pmid = v0.linear_interpolate(v1, 0.5);
				float l = marker_size;

				lines.push_back(pmid + Vector3(l, 0, 0));
				lines.push_back(pmid + Vector3(-l, 0, 0));
				lines.push_back(pmid + Vector3(0, 0, l));
				lines.push_back(pmid + Vector3(0, 0, -l));
				lines.push_back(pmid + Vector3(0, l, 0));
				lines.push_back(pmid + Vector3(0, -l, 0));
			}
		}
	}

	PoolIntArray seams = _mdr->get_seams();

	for (int i = 0; i < seams.size(); i += 2) {
		seam_lines.push_back(get_vertex(seams[i]));
		seam_lines.push_back(get_vertex(seams[i + 1]));
	}
}
void MDREDMeshOutline::generate_mark_faces(bool mark_outline, bool mark_handles) {
	reset();

	if (!initialize()) {
		return;
	}

	if (mark_outline) {
		for (int i = 0; i < _indices.size(); i += 3) {
			for (int j = 0; j < 3; ++j) {
				lines.push_back(get_vertex(_indices[i + j]));
				lines.push_back(get_vertex(_indices[i + ((j + 1) % 3)]));
			}
		}
	}

	if (mark_handles) {
		for (int i = 0; i < _indices.size(); i += 3) {
			int i0 = _indices[i + 0];
			int i1 = _indices[i + 1];
			int i2 = _indices[i + 2];

			Vector3 v0 = get_vertex(i0);
			Vector3 v1 = get_vertex(i1);
			Vector3 v2 = get_vertex(i2);

			Vector3 pmid = v0 + v1 + v2;
			pmid /= 3;
			float l = marker_size;

			lines.push_back(pmid + Vector3(l, 0, 0));
			lines.push_back(pmid + Vector3(-l, 0, 0));
			lines.push_back(pmid + Vector3(0, 0, l));
			lines.push_back(pmid + Vector3(0, 0, -l));
			lines.push_back(pmid + Vector3(0, l, 0));
			lines.push_back(pmid + Vector3(0, -l, 0));
		}
	}

	PoolIntArray seams = _mdr->get_seams();

	for (int i = 0; i < seams.size(); i += 2) {
		seam_lines.push_back(get_vertex(seams[i]));
		seam_lines.push_back(get_vertex(seams[i + 1]));
	}
}

MDREDMeshOutline::MDREDMeshOutline() {
	marker_size = 0.05;
}

MDREDMeshOutline::~MDREDMeshOutline() {
}

void MDREDMeshOutline::_bind_methods() {
}