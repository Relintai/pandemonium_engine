#ifndef NAV_UTILS_H
#define NAV_UTILS_H

/*************************************************************************/
/*  nav_utils.h                                                          */
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

#include "core/containers/hashfuncs.h"
#include "core/containers/rid.h"
#include "core/math/vector3.h"

class NavBase;

namespace gd {
struct Polygon;

union PointKey {
	struct {
		int64_t x : 21;
		int64_t y : 22;
		int64_t z : 21;
	};

	uint64_t key = 0;
};

struct EdgeKey {
	PointKey a;
	PointKey b;

	static uint32_t hash(const EdgeKey &p_val) {
		return hash_one_uint64(p_val.a.key) ^ hash_one_uint64(p_val.b.key);
	}

	bool operator<(const EdgeKey &p_key) const {
		return (a.key == p_key.a.key) ? (b.key < p_key.b.key) : (a.key < p_key.a.key);
	}

	bool operator==(const EdgeKey &p_key) const {
		return (a.key == p_key.a.key) && (b.key == p_key.b.key);
	}

	EdgeKey(const PointKey &p_a = PointKey(), const PointKey &p_b = PointKey()) :
			a(p_a),
			b(p_b) {
		if (a.key > b.key) {
			SWAP(a, b);
		}
	}
};

struct Point {
	Vector3 pos;
	PointKey key;
};

struct Edge {
	/// The gateway in the edge, as, in some case, the whole edge might not be navigable.
	struct Connection {
		/// Polygon that this connection leads to.
		Polygon *polygon;

		/// Edge of the source polygon where this connection starts from.
		int edge;

		/// Point on the edge where the gateway leading to the poly starts.
		Vector3 pathway_start;

		/// Point on the edge where the gateway leading to the poly ends.
		Vector3 pathway_end;

		Connection() {
			polygon = nullptr;
			edge = -1;
		}
	};

	/// Connections from this edge to other polygons.
	Vector<Connection> connections;

	Edge() {
	}
};

struct Polygon {
	/// Navigation region or link that contains this polygon.
	NavBase *owner;

	/// The points of this `Polygon`
	LocalVector<Point> points;

	/// Are the points clockwise?
	bool clockwise;

	/// The edges of this `Polygon`
	LocalVector<Edge> edges;

	/// The center of this `Polygon`
	Vector3 center;

	Polygon() {
		owner = nullptr;
	}
};

struct NavigationPoly {
	uint32_t self_id;
	/// This poly.
	const Polygon *poly;

	/// Those 4 variables are used to travel the path backwards.
	int back_navigation_poly_id;
	int back_navigation_edge;
	Vector3 back_navigation_edge_pathway_start;
	Vector3 back_navigation_edge_pathway_end;

	/// The entry position of this poly.
	Vector3 entry;
	/// The distance to the destination.
	real_t traveled_distance;

	NavigationPoly() { poly = nullptr; }

	NavigationPoly(const Polygon *p_poly) {
		self_id = 0;
		poly = p_poly;

		back_navigation_poly_id = -1;
		back_navigation_edge = -1;
		traveled_distance = 0.0;
	}

	bool operator==(const NavigationPoly &other) const {
		return this->poly == other.poly;
	}

	bool operator!=(const NavigationPoly &other) const {
		return !operator==(other);
	}
};

struct ClosestPointQueryResult {
	Vector3 point;
	Vector3 normal;
	RID owner;
};

} // namespace gd

#endif // NAV_UTILS_H
