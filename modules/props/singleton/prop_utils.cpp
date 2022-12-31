/*
Copyright (c) 2019-2023 Péter Magyar

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

#include "prop_utils.h"

#include "../props/prop_data.h"
#include "../props/prop_data_entry.h"

#include "core/math/quick_hull.h"
#include "scene/3d/portal.h"
#include "scene/3d/room.h"
#include "scene/3d/room_manager.h"

#include "scene/3d/mesh_instance.h"
#include "scene/resources/mesh.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
#include "../../mesh_data_resource/nodes/mesh_data_instance.h"
#endif

#include "core/config/engine.h"

PropUtils *PropUtils::_instance;
Vector<Ref<PropDataEntry>> PropUtils::_processors;

PropUtils *PropUtils::get_singleton() {
	return _instance;
}

Ref<PropData> PropUtils::convert_tree(Node *root) {
	ERR_FAIL_COND_V(!ObjectDB::instance_validate(root), Ref<PropData>());

	Ref<PropData> data;
	data.instance();
	Transform t;

	_convert_tree(data, root, t);

	return data;
}

void PropUtils::_convert_tree(Ref<PropData> prop_data, Node *node, const Transform &transform) {
	ERR_FAIL_COND(!ObjectDB::instance_validate(node));

	for (int i = 0; i < PropUtils::_processors.size(); ++i) {
		Ref<PropDataEntry> proc = PropUtils::_processors.get(i);

		ERR_CONTINUE(!proc.is_valid());

		if (proc->processor_handles(node)) {
			proc->processor_process(prop_data, node, transform);

			if (!proc->processor_evaluate_children()) {
				return;
			}

			break;
		}
	}

	Spatial *sp = Object::cast_to<Spatial>(node);

	if (!sp) {
		//reset transform
		Transform t;

		for (int i = 0; i < node->get_child_count(); ++i) {
			Node *child = node->get_child(i);

			if (Engine::get_singleton()->is_editor_hint()) {
				//Skip it if it's hidden from the tree
				if (child->get_owner() != NULL) {
					_convert_tree(prop_data, node->get_child(i), t);
				}
			} else {
				_convert_tree(prop_data, node->get_child(i), t);
			}
		}
	} else {
		//only handle the first encountered room per prop
		if (!prop_data->get_is_room()) {
			Room *r = Object::cast_to<Room>(sp);

			if (r) {
				prop_data->set_is_room(true);

				PoolVector3Array points = r->get_points();

				prop_data->set_room_bounds(points);
			}
		}

		for (int i = 0; i < node->get_child_count(); ++i) {
			Node *child = node->get_child(i);

			if (Engine::get_singleton()->is_editor_hint()) {
				//Skip it if it's hidden from the tree
				if (child->get_owner() != NULL) {
					_convert_tree(prop_data, node->get_child(i), transform * sp->get_transform());
				}
			} else {
				_convert_tree(prop_data, node->get_child(i), transform * sp->get_transform());
			}
		}
	}
}

bool PropUtils::generate_room_points_node(Node *node) {
	ERR_FAIL_COND_V(!ObjectDB::instance_validate(node), false);

	Room *r = Object::cast_to<Room>(node);

	if (r) {
		generate_room_points(r);

		return true;
	}

	for (int i = 0; i < node->get_child_count(); ++i) {
		if (generate_room_points_node(node->get_child(i))) {
			return true;
		}
	}

	return false;
}

void PropUtils::generate_room_points(Room *room) {
	ERR_FAIL_COND(!ObjectDB::instance_validate(room));

	Vector<PoolVector<Vector3>> mesh_arrays;

	get_mesh_arrays(room, &mesh_arrays);

	PoolVector<Plane> planes;
	Vector<Vector3> points;
	PoolVector<Face3> faces;

	for (int i = 0; i < mesh_arrays.size(); ++i) {
		PoolVector<Vector3> verts = mesh_arrays[i];

		for (int j = 0; j < verts.size(); j += 3) {
			Plane p(verts[j], verts[j + 1], verts[j + 2]);

			faces.push_back(Face3(verts[j], verts[j + 1], verts[j + 2]));

			//points.push_back(verts[j]);
			//points.push_back(verts[j + 1]);
			//points.push_back(verts[j + 2]);

			if (!is_plane_unique(planes, p)) {
				continue;
			}

			planes.push_back(p);

			//points.push_back(verts[j]);
			//points.push_back(verts[j + 1]);
			//points.push_back(verts[j + 2]);
		}
	}

	PoolVector<Face3> wrapped = Geometry::wrap_geometry(faces);

	for (int i = 0; i < wrapped.size(); ++i) {
		Face3 f = wrapped[i];

		points.push_back(f.vertex[0]);
		points.push_back(f.vertex[1]);
		points.push_back(f.vertex[2]);
	}

	Geometry::MeshData md = Geometry::build_convex_mesh(planes);
	md.optimize_vertices();

	QuickHull q;

	// calculate an epsilon based on the simplify value, and use this to build the hull
	real_t s = 0.5;

	// value between  0.3 (accurate) and 10.0 (very rough)
	// * UNIT_EPSILON
	s *= s;
	s *= 40.0;
	s += 0.3; // minimum
	s *= UNIT_EPSILON;

	q.build(points, md, s);

	md.optimize_vertices();

	PoolVector<Vector3> vs;
	vs.resize(md.vertices.size());
	for (int i = 0; i < md.vertices.size(); ++i) {
		vs.set(i, md.vertices[i]);
	}

	/*
	//It will probably have a few unnecessary vertices still
	//let's try to get rid of those as well
	PoolVector<int> remove_indices;
	int vssize = vs.size();
	for (int i = 0; i < vssize - 2; ++i) {
		Plane p(vs[i], vs[i + 1], vs[i + 2]);

		for (int j = 0; j < vssize; ++j) {
			if (i == j) {
				//skip this and the next 2
				j += 3;

				if (j >= vssize) {
					break;
				}
			}

			if (p.has_point(vs[j], 0.1)) {
				bool found = false;
				for (int k = 0; k < remove_indices.size(); ++k) {
					if (remove_indices[k] == j) {
						found = true;
						break;
					}
				}

				if (!found) {
					remove_indices.push_back(j);
				}
			}
		}
	}

	for (int i = 0; i < remove_indices.size(); ++i) {
		int rindex = remove_indices[i];

		vs.remove(rindex);

		for (int j = i + 1; j < remove_indices.size(); ++j) {
			int rij = remove_indices[j];

			if (rij > rindex) {
				remove_indices.set(j, rij - 1);
			}
		}
	}
*/

	room->set_points(vs);
}

//based on Room::SimplifyInfo::add_plane_if_unique
bool PropUtils::is_plane_unique(const PoolVector<Plane> &planes, const Plane &p) {
	for (int n = 0; n < planes.size(); n++) {
		const Plane &o = planes[n];

		// this is a fudge factor for how close planes can be to be considered the same ...
		// to prevent ridiculous amounts of planes
		const real_t d = 0.08f; //_plane_simplify_dist; // 0.08f

		if (Math::abs(p.d - o.d) > d) {
			continue;
		}

		real_t dot = p.normal.dot(o.normal);
		if (dot < 0.98f) //_plane_simplify_dot) // 0.98f
		{
			continue;
		}

		// match!
		return false;
	}

	return true;
}

void PropUtils::get_mesh_arrays(Node *node, Vector<PoolVector<Vector3>> *arrs) {
	ERR_FAIL_COND(!ObjectDB::instance_validate(node));

	for (int i = 0; i < node->get_child_count(); ++i) {
		get_mesh_arrays(node->get_child(i), arrs);
	}

	{
		Portal *pn = Object::cast_to<Portal>(node);

		if (pn) {
			if (!pn->get_portal_active()) {
				return;
			}

			if (!pn->is_visible_in_tree()) {
				return;
			}

			PoolVector<Vector2> points = pn->get_points();
			PoolVector<Vector3> v3p;
			v3p.resize(points.size());

			for (int i = 0; i < points.size(); ++i) {
				v3p.set(i, Vector3(points[i].x, points[i].y, 0));
			}

			Transform t = pn->get_global_transform();

			int fvertcount = (points.size() - 2) * 3;

			PoolVector<Vector3> tverts;
			tverts.resize(fvertcount);

			for (int i = 0; i < points.size() - 2; ++i) {
				int sindex = i * 3;

				tverts.set(sindex, t.xform(v3p[i]));
				tverts.set(sindex + 1, t.xform(v3p[i + 1]));
				tverts.set(sindex + 2, t.xform(v3p[i + 2]));
			}

			//portal planes need to take precedence
			arrs->insert(0, tverts);

			return;
		}
	}

#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
	{
		MeshDataInstance *mdi = Object::cast_to<MeshDataInstance>(node);

		if (mdi) {
			if (!mdi->is_visible_in_tree()) {
				return;
			}

			Ref<MeshDataResource> mdr = mdi->get_mesh_data();

			if (!mdr.is_valid()) {
				return;
			}

			Array arr = mdr->get_array();

			if (arr.size() != Mesh::ARRAY_MAX) {
				return;
			}

			Transform t = mdi->get_global_transform();

			PoolVector<Vector3> verts = arr[Mesh::ARRAY_VERTEX];
			PoolVector<Vector3> tverts;
			tverts.resize(verts.size());

			for (int i = 0; i < verts.size(); ++i) {
				tverts.set(i, t.xform(verts[i]));
			}

			PoolVector<int> indices = arr[Mesh::ARRAY_INDEX];

			if (indices.size() == 0) {
				arrs->push_back(tverts);
				return;
			}

			PoolVector<Vector3> fverts;
			fverts.resize(indices.size());

			for (int i = 0; i < indices.size(); ++i) {
				fverts.set(i, tverts[indices[i]]);
			}

			arrs->push_back(fverts);

			return;
		}
	}
#endif

	{
		MeshInstance *min = Object::cast_to<MeshInstance>(node);

		if (min) {
			if (!min->is_visible_in_tree()) {
				return;
			}

			Ref<ArrayMesh> am = min->get_mesh();

			if (!am.is_valid()) {
				return;
			}

			Transform t = min->get_global_transform();

			for (int si = 0; si < am->get_surface_count(); ++si) {
				Array arr = am->surface_get_arrays(si);

				if (arr.size() != Mesh::ARRAY_MAX) {
					continue;
				}

				PoolVector<Vector3> verts = arr[Mesh::ARRAY_VERTEX];
				PoolVector<Vector3> tverts;
				tverts.resize(verts.size());

				for (int i = 0; i < verts.size(); ++i) {
					tverts.set(i, t.xform(verts[i]));
				}

				PoolVector<int> indices = arr[Mesh::ARRAY_INDEX];

				if (indices.size() == 0) {
					arrs->push_back(tverts);
					continue;
				}

				PoolVector<Vector3> fverts;
				fverts.resize(indices.size());

				for (int i = 0; i < indices.size(); ++i) {
					fverts.set(i, tverts[indices[i]]);
				}

				arrs->push_back(fverts);
			}

			return;
		}
	}
}

int PropUtils::add_processor(const Ref<PropDataEntry> &processor) {
	ERR_FAIL_COND_V(!processor.is_valid(), 0);

	PropUtils::_processors.push_back(processor);

	return PropUtils::_processors.size() - 1;
}
Ref<PropDataEntry> PropUtils::get_processor(const int index) {
	ERR_FAIL_INDEX_V(index, PropUtils::_processors.size(), Ref<PropDataEntry>());

	return PropUtils::_processors[index];
}
void PropUtils::swap_processors(const int index1, const int index2) {
	ERR_FAIL_INDEX(index1, PropUtils::_processors.size());
	ERR_FAIL_INDEX(index2, PropUtils::_processors.size());

	Ref<PropDataEntry> a = PropUtils::_processors.get(index1);
	PropUtils::_processors.set(index1, PropUtils::_processors.get(index2));
	PropUtils::_processors.set(index2, a);
}
void PropUtils::remove_processor(const int index) {
	ERR_FAIL_INDEX(index, PropUtils::_processors.size());

	PropUtils::_processors.remove(index);
}
int PropUtils::get_processor_count() {
	return PropUtils::_processors.size();
}

PropUtils::PropUtils() {
	_instance = this;
}

PropUtils::~PropUtils() {
	_instance = NULL;

	PropUtils::_processors.clear();
}

void PropUtils::_bind_methods() {
	ClassDB::bind_method(D_METHOD("convert_tree", "root"), &PropUtils::convert_tree);

	ClassDB::bind_method(D_METHOD("add_processor", "processor"), &PropUtils::_add_processor_bind);
	ClassDB::bind_method(D_METHOD("get_processor", "index"), &PropUtils::_get_processor_bind);
	ClassDB::bind_method(D_METHOD("swap_processors", "index1", "index2"), &PropUtils::_swap_processors_bind);
	ClassDB::bind_method(D_METHOD("remove_processor", "index"), &PropUtils::_remove_processor_bind);
	ClassDB::bind_method(D_METHOD("get_processor_count"), &PropUtils::_get_processor_count_bind);
}

int PropUtils::_add_processor_bind(const Ref<PropDataEntry> &processor) {
	return PropUtils::add_processor(processor);
}
Ref<PropDataEntry> PropUtils::_get_processor_bind(const int index) {
	return PropUtils::get_processor(index);
}
void PropUtils::_swap_processors_bind(const int index1, const int index2) {
	PropUtils::swap_processors(index1, index2);
}
void PropUtils::_remove_processor_bind(const int index) {
	PropUtils::remove_processor(index);
}
int PropUtils::_get_processor_count_bind() {
	return PropUtils::get_processor_count();
}
