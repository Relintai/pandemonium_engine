#ifndef QUICK_HULL_H
#define QUICK_HULL_H

/*************************************************************************/
/*  quick_hull.h                                                         */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "core/containers/list.h"
#include "core/containers/rb_set.h"
#include "core/math/aabb.h"
#include "core/math/geometry.h"

class QuickHull {
public:
	struct Edge {
		union {
			uint32_t vertices[2];
			uint64_t id;
		};

		bool operator<(const Edge &p_edge) const {
			return id < p_edge.id;
		}

		Edge(int p_vtx_a = 0, int p_vtx_b = 0) {
			if (p_vtx_a > p_vtx_b) {
				SWAP(p_vtx_a, p_vtx_b);
			}

			vertices[0] = p_vtx_a;
			vertices[1] = p_vtx_b;
		}
	};

	struct Face {
		Plane plane;
		uint32_t vertices[3];
		Vector<int> points_over;

		bool operator<(const Face &p_face) const {
			return points_over.size() < p_face.points_over.size();
		}
	};

private:
	struct FaceConnect {
		List<Face>::Element *left, *right;
		FaceConnect() {
			left = nullptr;
			right = nullptr;
		}
	};
	struct RetFaceConnect {
		List<Geometry::MeshData::Face>::Element *left, *right;
		RetFaceConnect() {
			left = nullptr;
			right = nullptr;
		}
	};

	static int find_or_create_output_index(int p_old_index, Vector<int> &r_out_indices) {
		for (int n = 0; n < r_out_indices.size(); n++) {
			if (r_out_indices[n] == p_old_index) {
				return n;
			}
		}
		r_out_indices.push_back(p_old_index);
		return r_out_indices.size() - 1;
	}

public:
	static uint32_t debug_stop_after;
	static bool _flag_warnings;
	static Error build(const Vector<Vector3> &p_points, Geometry::MeshData &r_mesh, real_t p_over_tolerance_epsilon = 3.0 * UNIT_EPSILON);
};

#endif // QUICK_HULL_H
