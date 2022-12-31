#ifndef FAST_QUADRATIC_MESH_SIMPLIFIER_SIMPLIFY_H
#define FAST_QUADRATIC_MESH_SIMPLIFIER_SIMPLIFY_H

/////////////////////////////////////////////
//
// Mesh Simplification Tutorial
//
// (C) by Sven Forstmann in 2014
//
// License : MIT
// http://opensource.org/licenses/MIT
//
// https://github.com/sp4cerat/Fast-Quadric-Mesh-Simplification
//
// 5/2016: Chris Rorden created minimal version for OSX/Linux/Windows compile

// Contains some improvements from https://github.com/Whinarn/MeshDecimator
// Copyright (c) 2017-2018 Mattias Edlund

// (C) by Péter Magyar in 2020

#include "core/variant/array.h"
#include "core/string/ustring.h"

#include "core/math/vector2.h"
#include "core/math/vector3.h"
#include "scene/resources/mesh.h"

#include <float.h> //FLT_EPSILON, DBL_EPSILON
#include <limits.h>
#include <algorithm>
#include <map>
#include <vector>

#include "servers/rendering_server.h"

namespace Simplify {

struct vector3 {
	double x, y, z;
};

struct vec3f {
	double x, y, z;

	inline vec3f(void) {}

	//inline vec3f operator =( vector3 a )
	// { vec3f b ; b.x = a.x; b.y = a.y; b.z = a.z; return b;}

	inline vec3f(vector3 a) {
		x = a.x;
		y = a.y;
		z = a.z;
	}

	inline vec3f(const double X, const double Y, const double Z) {
		x = X;
		y = Y;
		z = Z;
	}

	inline vec3f operator+(const vec3f &a) const { return vec3f(x + a.x, y + a.y, z + a.z); }

	inline vec3f operator+=(const vec3f &a) const { return vec3f(x + a.x, y + a.y, z + a.z); }

	inline vec3f operator*(const double a) const { return vec3f(x * a, y * a, z * a); }

	inline vec3f operator*(const vec3f a) const { return vec3f(x * a.x, y * a.y, z * a.z); }

	inline vec3f v3() const { return vec3f(x, y, z); }

	inline vec3f operator=(const vector3 a) {
		x = a.x;
		y = a.y;
		z = a.z;
		return *this;
	}

	inline vec3f operator=(const vec3f a) {
		x = a.x;
		y = a.y;
		z = a.z;
		return *this;
	}

	inline vec3f operator/(const vec3f a) const { return vec3f(x / a.x, y / a.y, z / a.z); }

	inline vec3f operator-(const vec3f &a) const { return vec3f(x - a.x, y - a.y, z - a.z); }

	inline vec3f operator/(const double a) const { return vec3f(x / a, y / a, z / a); }

	inline double dot(const vec3f &a) const { return a.x * x + a.y * y + a.z * z; }

	inline vec3f cross(const vec3f &a, const vec3f &b) {
		x = a.y * b.z - a.z * b.y;
		y = a.z * b.x - a.x * b.z;
		z = a.x * b.y - a.y * b.x;
		return *this;
	}

	inline double angle(const vec3f &v) {
		vec3f a = v, b = *this;
		double dot = v.x * x + v.y * y + v.z * z;
		double len = a.length() * b.length();
		if (len == 0)
			len = 0.00001f;
		double input = dot / len;
		if (input < -1)
			input = -1;
		if (input > 1)
			input = 1;
		return (double)acos(input);
	}

	inline double angle2(const vec3f &v, const vec3f &w) {
		vec3f a = v, b = *this;
		double dot = a.x * b.x + a.y * b.y + a.z * b.z;
		double len = a.length() * b.length();
		if (len == 0)
			len = 1;

		vec3f plane;
		plane.cross(b, w);

		if (plane.x * a.x + plane.y * a.y + plane.z * a.z > 0)
			return (double)-acos(dot / len);

		return (double)acos(dot / len);
	}

	inline vec3f rot_x(double a) {
		double yy = cos(a) * y + sin(a) * z;
		double zz = cos(a) * z - sin(a) * y;
		y = yy;
		z = zz;
		return *this;
	}
	inline vec3f rot_y(double a) {
		double xx = cos(-a) * x + sin(-a) * z;
		double zz = cos(-a) * z - sin(-a) * x;
		x = xx;
		z = zz;
		return *this;
	}
	inline void clamp(double min, double max) {
		if (x < min)
			x = min;
		if (y < min)
			y = min;
		if (z < min)
			z = min;
		if (x > max)
			x = max;
		if (y > max)
			y = max;
		if (z > max)
			z = max;
	}
	inline vec3f rot_z(double a) {
		double yy = cos(a) * y + sin(a) * x;
		double xx = cos(a) * x - sin(a) * y;
		y = yy;
		x = xx;
		return *this;
	}
	inline vec3f invert() {
		x = -x;
		y = -y;
		z = -z;
		return *this;
	}
	inline vec3f frac() {
		return vec3f(
				x - double(int(x)),
				y - double(int(y)),
				z - double(int(z)));
	}

	inline vec3f integer() {
		return vec3f(
				double(int(x)),
				double(int(y)),
				double(int(z)));
	}

	inline double length() const {
		return (double)sqrt(x * x + y * y + z * z);
	}

	inline vec3f normalize(double desired_length = 1) {
		double square = sqrt(x * x + y * y + z * z);
		/*
		if (square <= 0.00001f )
		{
			x=1;y=0;z=0;
			return *this;
		}*/
		//double len = desired_length / square;
		x /= square;
		y /= square;
		z /= square;

		return *this;
	}
	static vec3f normalize(vec3f a);

	static void random_init();
	static double random_double();
	static vec3f random();

	static int random_number;

	double random_double_01(double a) {
		double rnf = a * 14.434252 + a * 364.2343 + a * 4213.45352 + a * 2341.43255 + a * 254341.43535 + a * 223454341.3523534245 + 23453.423412;
		int rni = ((int)rnf) % 100000;
		return double(rni) / (100000.0f - 1.0f);
	}

	vec3f random01_fxyz() {
		x = (double)random_double_01(x);
		y = (double)random_double_01(y);
		z = (double)random_double_01(z);
		return *this;
	}
};

static vec3f barycentric(const vec3f &p, const vec3f &a, const vec3f &b, const vec3f &c) {
	vec3f v0 = b - a;
	vec3f v1 = c - a;
	vec3f v2 = p - a;
	double d00 = v0.dot(v0);
	double d01 = v0.dot(v1);
	double d11 = v1.dot(v1);
	double d20 = v2.dot(v0);
	double d21 = v2.dot(v1);
	double denom = d00 * d11 - d01 * d01;
	double v = (d11 * d20 - d01 * d21) / denom;
	double w = (d00 * d21 - d01 * d20) / denom;
	double u = 1.0 - v - w;
	return vec3f(u, v, w);
}

static vec3f interpolate(const vec3f &p, const vec3f &a, const vec3f &b, const vec3f &c, const vec3f attrs[3]) {
	vec3f bary = barycentric(p, a, b, c);
	vec3f out = vec3f(0, 0, 0);
	out = out + attrs[0] * bary.x;
	out = out + attrs[1] * bary.y;
	out = out + attrs[2] * bary.z;
	return out;
}

static double min(double v1, double v2) {
	return fmin(v1, v2);
}

class SymetricMatrix {
public:
	// Constructor

	SymetricMatrix(double c = 0) {
		for (unsigned int i = 0; i < 10; ++i)
			m[i] = c;
	}

	SymetricMatrix(double m11, double m12, double m13, double m14,
			double m22, double m23, double m24,
			double m33, double m34,
			double m44) {
		m[0] = m11;
		m[1] = m12;
		m[2] = m13;
		m[3] = m14;
		m[4] = m22;
		m[5] = m23;
		m[6] = m24;
		m[7] = m33;
		m[8] = m34;
		m[9] = m44;
	}

	// Make plane

	SymetricMatrix(double a, double b, double c, double d) {
		m[0] = a * a;
		m[1] = a * b;
		m[2] = a * c;
		m[3] = a * d;
		m[4] = b * b;
		m[5] = b * c;
		m[6] = b * d;
		m[7] = c * c;
		m[8] = c * d;
		m[9] = d * d;
	}

	double operator[](int c) const { return m[c]; }

	// Determinant

	double det(int a11, int a12, int a13,
			int a21, int a22, int a23,
			int a31, int a32, int a33) {
		double det = m[a11] * m[a22] * m[a33] + m[a13] * m[a21] * m[a32] + m[a12] * m[a23] * m[a31] - m[a13] * m[a22] * m[a31] - m[a11] * m[a23] * m[a32] - m[a12] * m[a21] * m[a33];
		return det;
	}

	const SymetricMatrix operator+(const SymetricMatrix &n) const {
		return SymetricMatrix(m[0] + n[0], m[1] + n[1], m[2] + n[2], m[3] + n[3],
				m[4] + n[4], m[5] + n[5], m[6] + n[6],
				m[7] + n[7], m[8] + n[8],
				m[9] + n[9]);
	}

	SymetricMatrix &operator+=(const SymetricMatrix &n) {
		m[0] += n[0];
		m[1] += n[1];
		m[2] += n[2];
		m[3] += n[3];
		m[4] += n[4];
		m[5] += n[5];
		m[6] += n[6];
		m[7] += n[7];
		m[8] += n[8];
		m[9] += n[9];
		return *this;
	}

	double m[10];
};
///////////////////////////////////////////

struct BorderVertex {
	int index;
	int hash;
};

static bool compare_border_vertex(const BorderVertex &i1, const BorderVertex &i2) {
	return (i1.hash < i2.hash);
}

class FQMS {
public:
	struct Triangle {
		int v[3];
		double err[4];
		int deleted, dirty;
		vec3f n;
		vec3f uvs[3];
		vec3f uv2s[3];
		Color color[3];
		int material;
	};

	struct Vertex {
		vec3f p;
		int tstart, tcount;
		SymetricMatrix q;
		int border;
		bool seam;
		bool foldover;
	};

	struct Ref {
		int tid, tvertex;
	};

	std::vector<Triangle> triangles;
	std::vector<Vertex> vertices;
	std::vector<Ref> refs;

	int _max_iteration_count;
	int _max_lossless_iteration_count;
	bool _enable_smart_link;
	bool _preserve_border_dges;
	bool _preserve_uv_seam_edges;
	bool _preserve_uv_foldover_edges;
	int _format;
	double _vertex_link_distance;

	// Helper functions

	//
	// Main simplification function
	//
	// target_count  : target nr. of triangles
	// aggressiveness : sharpness to increase the threshold.
	//                 5..8 are good numbers
	//                 more iterations yield higher quality
	//

	void simplify_mesh(int target_count, double aggressiveness = 7, bool verbose = false) {
		ERR_FAIL_COND_MSG(_enable_smart_link, "FastQuadraticMeshSimplifier: enable_smart_link setting is not yet supported!");

		// init
		for (unsigned int i = 0; i < triangles.size(); ++i) {
			triangles[i].deleted = 0;
		}

		// main iteration loop
		int deleted_triangles = 0;
		std::vector<int> deleted0, deleted1;
		int triangle_count = triangles.size();

		for (int iteration = 0; iteration < _max_iteration_count; iteration++) {
			if (triangle_count - deleted_triangles <= target_count)
				break;

			// update mesh once in a while
			if (iteration % 5 == 0) {
				update_mesh(iteration);
			}

			// clear dirty flag
			for (unsigned int i = 0; i < triangles.size(); ++i) {
				triangles[i].dirty = 0;
			}

			//
			// All triangles with edges below the threshold will be removed
			//
			// The following numbers works well for most models.
			// If it does not, try to adjust the 3 parameters
			//
			double threshold = 0.000000001 * pow(double(iteration + 3), aggressiveness);

			// target number of triangles reached ? Then break
			if ((verbose) && (iteration % 5 == 0)) {
				print_line("iteration " + String::num(iteration) + " - triangles " + String::num(triangle_count - deleted_triangles) + " threshold " + String::num(threshold));
			}

			// remove vertices & mark deleted triangles
			for (unsigned int i = 0; i < triangles.size(); ++i) {
				Triangle &t = triangles[i];

				if (t.err[3] > threshold)
					continue;

				if (t.deleted)
					continue;

				if (t.dirty)
					continue;

				for (int j = 0; j < 3; ++j) {
					if (t.err[j] < threshold) {
						int i0 = t.v[j];
						Vertex &v0 = vertices[i0];
						int i1 = t.v[(j + 1) % 3];
						Vertex &v1 = vertices[i1];
						// Border check
						if (v0.border != v1.border)
							continue;
						else if (_preserve_border_dges && v0.border)
							continue;

						//if (v0.border || v1.border) continue;

						// Compute vertex to collapse to
						vec3f p;
						calculate_error(i0, i1, p);
						deleted0.resize(v0.tcount); // normals temporarily
						deleted1.resize(v1.tcount); // normals temporarily
						// don't remove if flipped

						if (flipped(p, i0, i1, v0, v1, deleted0))
							continue;

						if (flipped(p, i1, i0, v1, v0, deleted1))
							continue;

						if ((_format & RenderingServer::ARRAY_FORMAT_TEX_UV) != 0) {
							update_uvs(i0, v0, p, deleted0);
							update_uvs(i0, v1, p, deleted1);
						}

						if ((_format & RenderingServer::ARRAY_FORMAT_TEX_UV2) != 0) {
							update_uv2s(i0, v0, p, deleted0);
							update_uv2s(i0, v1, p, deleted1);
						}

						// not flipped, so remove edge
						v0.p = p;
						v0.q = v1.q + v0.q;
						int tstart = refs.size();

						update_triangles(i0, v0, deleted0, deleted_triangles);
						update_triangles(i0, v1, deleted1, deleted_triangles);

						int tcount = refs.size() - tstart;

						if (tcount <= v0.tcount) {
							// save ram
							if (tcount)
								memcpy(&refs[v0.tstart], &refs[tstart], tcount * sizeof(Ref));
						} else {
							// append
							v0.tstart = tstart;
						}

						v0.tcount = tcount;
						break;
					}
				}

				// done?
				if (triangle_count - deleted_triangles <= target_count)
					break;
			}
		}
		// clean up mesh
		compact_mesh();
	} //simplify_mesh()

	void simplify_mesh_lossless(bool verbose = false) {
		ERR_FAIL_COND_MSG(_enable_smart_link, "FastQuadraticMeshSimplifier: enable_smart_link setting is not yet supported!");

		// init
		for (unsigned int i = 0; i < triangles.size(); ++i)
			triangles[i].deleted = 0;

		// main iteration loop
		int deleted_triangles = 0;
		std::vector<int> deleted0, deleted1;
		//unsigned int triangle_count = triangles.size();

		for (int iteration = 0; iteration < _max_lossless_iteration_count; iteration++) {
			// update mesh constantly
			update_mesh(iteration);
			// clear dirty flag
			for (unsigned int i = 0; i < triangles.size(); ++i)
				triangles[i].dirty = 0;
			//
			// All triangles with edges below the threshold will be removed
			//
			// The following numbers works well for most models.
			// If it does not, try to adjust the 3 parameters
			//
			double threshold = DBL_EPSILON; //1.0E-3 EPS;
			if (verbose) {
				print_line("lossless iteration " + String::num(iteration));
			}

			// remove vertices & mark deleted triangles
			for (unsigned int i = 0; i < triangles.size(); ++i) {
				Triangle &t = triangles[i];

				if (t.err[3] > threshold)
					continue;

				if (t.deleted)
					continue;

				if (t.dirty)
					continue;

				for (int j = 0; j < 3; ++j) {
					if (t.err[j] < threshold) {
						int i0 = t.v[j];
						Vertex &v0 = vertices[i0];
						int i1 = t.v[(j + 1) % 3];
						Vertex &v1 = vertices[i1];

						// Border check
						if (v0.border != v1.border)
							continue;

						// Compute vertex to collapse to
						vec3f p;
						calculate_error(i0, i1, p);

						deleted0.resize(v0.tcount); // normals temporarily
						deleted1.resize(v1.tcount); // normals temporarily

						// don't remove if flipped
						if (flipped(p, i0, i1, v0, v1, deleted0))
							continue;

						if (flipped(p, i1, i0, v1, v0, deleted1))
							continue;

						if ((_format & RenderingServer::ARRAY_FORMAT_TEX_UV) != 0) {
							update_uvs(i0, v0, p, deleted0);
							update_uvs(i0, v1, p, deleted1);
						}

						if ((_format & RenderingServer::ARRAY_FORMAT_TEX_UV2) != 0) {
							update_uv2s(i0, v0, p, deleted0);
							update_uv2s(i0, v1, p, deleted1);
						}

						// not flipped, so remove edge
						v0.p = p;
						v0.q = v1.q + v0.q;
						int tstart = refs.size();

						update_triangles(i0, v0, deleted0, deleted_triangles);
						update_triangles(i0, v1, deleted1, deleted_triangles);

						int tcount = refs.size() - tstart;

						if (tcount <= v0.tcount) {
							// save ram
							if (tcount)
								memcpy(&refs[v0.tstart], &refs[tstart], tcount * sizeof(Ref));
						} else {
							// append
							v0.tstart = tstart;
						}

						v0.tcount = tcount;
						break;
					}
				}
			}

			if (deleted_triangles <= 0)
				break;

			deleted_triangles = 0;
		} //for each iteration
		// clean up mesh
		compact_mesh();
	} //simplify_mesh_lossless()

	// Check if a triangle flips when this edge is removed

	bool flipped(vec3f p, int i0, int i1, Vertex &v0, Vertex &v1, std::vector<int> &deleted) {
		for (int k = 0; k < v0.tcount; ++k) {
			Triangle &t = triangles[refs[v0.tstart + k].tid];

			if (t.deleted)
				continue;

			int s = refs[v0.tstart + k].tvertex;
			int id1 = t.v[(s + 1) % 3];
			int id2 = t.v[(s + 2) % 3];

			if (id1 == i1 || id2 == i1) // delete ?
			{
				deleted[k] = 1;
				continue;
			}
			vec3f d1 = vertices[id1].p - p;
			d1.normalize();
			vec3f d2 = vertices[id2].p - p;
			d2.normalize();
			if (fabs(d1.dot(d2)) > 0.999)
				return true;
			vec3f n;
			n.cross(d1, d2);
			n.normalize();
			deleted[k] = 0;
			if (n.dot(t.n) < 0.2)
				return true;
		}
		return false;
	}

	// update_uvs

	void update_uvs(int i0, const Vertex &v, const vec3f &p, std::vector<int> &deleted) {
		for (int k = 0; k < v.tcount; ++k) {
			Ref &r = refs[v.tstart + k];
			Triangle &t = triangles[r.tid];

			if (t.deleted)
				continue;

			if (deleted[k])
				continue;

			vec3f p1 = vertices[t.v[0]].p;
			vec3f p2 = vertices[t.v[1]].p;
			vec3f p3 = vertices[t.v[2]].p;

			t.uvs[r.tvertex] = interpolate(p, p1, p2, p3, t.uvs);
		}
	}

	void update_uv2s(int i0, const Vertex &v, const vec3f &p, std::vector<int> &deleted) {
		for (int k = 0; k < v.tcount; ++k) {
			Ref &r = refs[v.tstart + k];
			Triangle &t = triangles[r.tid];

			if (t.deleted)
				continue;

			if (deleted[k])
				continue;

			vec3f p1 = vertices[t.v[0]].p;
			vec3f p2 = vertices[t.v[1]].p;
			vec3f p3 = vertices[t.v[2]].p;

			t.uv2s[r.tvertex] = interpolate(p, p1, p2, p3, t.uv2s);
		}
	}

	// Update triangle connections and edge error after a edge is collapsed

	void update_triangles(int i0, Vertex &v, std::vector<int> &deleted, int &deleted_triangles) {
		vec3f p;

		for (int k = 0; k < v.tcount; ++k) {
			Ref &r = refs[v.tstart + k];
			Triangle &t = triangles[r.tid];

			if (t.deleted)
				continue;

			if (deleted[k]) {
				t.deleted = 1;
				deleted_triangles++;
				continue;
			}

			t.v[r.tvertex] = i0;

			t.dirty = 1;
			t.err[0] = calculate_error(t.v[0], t.v[1], p);
			t.err[1] = calculate_error(t.v[1], t.v[2], p);
			t.err[2] = calculate_error(t.v[2], t.v[0], p);
			t.err[3] = min(t.err[0], min(t.err[1], t.err[2]));
			refs.push_back(r);
		}
	}

	// compact triangles, compute edge error and build reference list

	void update_mesh(int iteration) {
		if (iteration > 0) // compact triangles
		{
			int dst = 0;
			for (unsigned int i = 0; i < triangles.size(); ++i) {
				if (!triangles[i].deleted) {
					triangles[dst++] = triangles[i];
				}
			}

			triangles.resize(dst);
		}

		//
		// Init Quadrics by Plane & Edge Errors
		//
		// required at the beginning ( iteration == 0 )
		// recomputing during the simplification is not required,
		// but mostly improves the result for closed meshes
		//
		if (iteration == 0) {
			for (unsigned int i = 0; i < vertices.size(); ++i) {
				vertices[i].q = SymetricMatrix(0.0);
			}

			for (unsigned int i = 0; i < triangles.size(); ++i) {
				Triangle &t = triangles[i];
				vec3f n, p[3];
				for (int j = 0; j < 3; ++j) {
					p[j] = vertices[t.v[j]].p;
				}

				n.cross(p[1] - p[0], p[2] - p[0]);
				n.normalize();
				t.n = n;
				for (int j = 0; j < 3; ++j) {
					vertices[t.v[j]].q = vertices[t.v[j]].q + SymetricMatrix(n.x, n.y, n.z, -n.dot(p[0]));
				}
			}
			for (unsigned int i = 0; i < triangles.size(); ++i) {
				// Calc Edge Error
				Triangle &t = triangles[i];
				vec3f p;

				for (int j = 0; j < 3; ++j) {
					t.err[j] = calculate_error(t.v[j], t.v[(j + 1) % 3], p);
				}

				t.err[3] = min(t.err[0], min(t.err[1], t.err[2]));
			}
		}

		// Init Reference ID list
		for (unsigned int i = 0; i < vertices.size(); ++i) {
			vertices[i].tstart = 0;
			vertices[i].tcount = 0;
		}

		for (unsigned int i = 0; i < triangles.size(); ++i) {
			Triangle &t = triangles[i];

			for (int j = 0; j < 3; ++j) {
				vertices[t.v[j]].tcount++;
			}
		}

		int tstart = 0;
		for (unsigned int i = 0; i < vertices.size(); ++i) {
			Vertex &v = vertices[i];
			v.tstart = tstart;
			tstart += v.tcount;
			v.tcount = 0;
		}

		// Write References
		refs.resize(triangles.size() * 3);
		for (unsigned int i = 0; i < triangles.size(); ++i) {
			Triangle &t = triangles[i];

			for (unsigned int j = 0; j < 3; ++j) {
				Vertex &v = vertices[t.v[j]];
				refs[v.tstart + v.tcount].tid = i;
				refs[v.tstart + v.tcount].tvertex = j;
				v.tcount++;
			}
		}

		// Identify boundary : vertices[].border=0,1
		if (iteration == 0) {
			std::vector<int> vcount, vids;

			for (unsigned int i = 0; i < vertices.size(); ++i) {
				Vertex &v = vertices[i];

				v.border = 0;
				v.seam = false;
				v.foldover = false;
			}

			//int border_vertex_count = 0;
			double border_min_x = DBL_MIN;
			double border_max_x = DBL_MAX;

			for (unsigned int i = 0; i < vertices.size(); ++i) {
				Vertex &v = vertices[i];
				vcount.clear();
				vids.clear();

				for (int j = 0; j < v.tcount; ++j) {
					int kt = refs[v.tstart + j].tid;
					Triangle &t = triangles[kt];

					for (int k = 0; k < 3; ++k) {
						unsigned int ofs = 0;
						int id = t.v[k];

						while (ofs < vcount.size()) {
							if (vids[ofs] == id)
								break;

							ofs++;
						}

						if (ofs == vcount.size()) {
							vcount.push_back(1);
							vids.push_back(id);
						} else {
							vcount[ofs]++;
						}
					}
				}

				for (unsigned int j = 0; j < vcount.size(); ++j) {
					if (vcount[j] == 1) {
						Vertex &vv = vertices[vids[j]];

						vv.border = 1;
						//++border_vertex_count;

						if (_enable_smart_link) {
							if (vv.p.x < border_min_x)
								border_min_x = vv.p.x;

							if (vv.p.x < border_max_x)
								border_max_x = vv.p.x;
						}
					}
				}

				if (_enable_smart_link) {
					std::vector<BorderVertex> border_vertices;

					double border_area_width = border_max_x - border_min_x;

					for (unsigned int j = 0; j < vertices.size(); j++) {
						Vertex &vj = vertices[i];

						if (v.border) {
							BorderVertex bv;
							bv.hash = static_cast<int>(((((vj.p.x - border_min_x) / border_area_width) * 2.0) - 1.0) * INT_MAX);
							bv.index = j;

							border_vertices.push_back(bv);
						}
					}

					std::sort(border_vertices.begin(), border_vertices.end(), compare_border_vertex);

					// Calculate the maximum hash distance based on the maximum vertex link distance
					int tdst = static_cast<int>((_vertex_link_distance / border_area_width) * INT_MAX);
					int hash_max_distance = MAX(tdst, 1);

					// Then find identical border vertices and bind them together as one
					for (unsigned int j = 0; j < border_vertices.size(); ++j) {
						BorderVertex &bv = border_vertices[j];

						if (bv.index == -1)
							continue;

						//var myPoint = vertices[myIndex].p;
						for (unsigned int k = i + 1; k < border_vertices.size(); ++k) {
							BorderVertex &obv = border_vertices[k];

							//int otherIndex = obv.index;
							//var otherPoint = vertices[otherIndex].p;
							if (obv.index == -1)
								continue;

							else if ((obv.hash - bv.hash) > hash_max_distance) // There is no point to continue beyond this point
								break;

							Vertex &vj = vertices[j];
							Vertex &ov = vertices[k];

							double sqr_x = ((vj.p.x - ov.p.x) * (vj.p.x - ov.p.x));
							double sqr_y = ((vj.p.y - ov.p.y) * (vj.p.y - ov.p.y));
							double sqr_z = ((vj.p.z - ov.p.z) * (vj.p.z - ov.p.z));
							double sqr_magnitude = sqr_x + sqr_y + sqr_z;

							if (sqr_magnitude <= _vertex_link_distance) {
								obv.index = -1; // NOTE: This makes sure that the "other" vertex is not processed again
								vj.border = false;
								ov.border = false;
								/*
								if (AreUVsTheSame(0, myIndex, otherIndex)) {
									vertices[myIndex].foldover = true;
									vertices[otherIndex].foldover = true;
								} else {
									vertices[myIndex].seam = true;
									vertices[otherIndex].seam = true;
								}

								int other_triangle_count = ov.tcount;
								int other_triangle_start = ov.tstart;
								for (int k = 0; k < other_triangle_count; k++) {
									Ref &r = refs[other_triangle_start + k];

									Triangle &t = triangles[r.tid];
									t.v[r.tvertex] = myIndex;
								}
								*/
							}
						}
					}

					// Update the references again
					//update_references();
				}
			}
		}
	}

	// Finally compact mesh before exiting

	void compact_mesh() {
		int dst = 0;

		for (unsigned int i = 0; i < vertices.size(); ++i) {
			vertices[i].tcount = 0;
		}

		for (unsigned int i = 0; i < triangles.size(); ++i) {
			if (!triangles[i].deleted) {
				Triangle &t = triangles[i];

				triangles[dst++] = t;

				for (int j = 0; j < 3; ++j) {
					vertices[t.v[j]].tcount = 1;
				}
			}
		}

		triangles.resize(dst);
		dst = 0;
		for (unsigned int i = 0; i < vertices.size(); ++i) {
			if (vertices[i].tcount) {
				vertices[i].tstart = dst;
				vertices[dst].p = vertices[i].p;
				dst++;
			}
		}

		for (unsigned int i = 0; i < triangles.size(); ++i) {
			Triangle &t = triangles[i];

			for (int j = 0; j < 3; ++j) {
				t.v[j] = vertices[t.v[j]].tstart;
			}
		}
		vertices.resize(dst);
	}

	// Error between vertex and Quadric

	double vertex_error(SymetricMatrix q, double x, double y, double z) {
		return q[0] * x * x + 2 * q[1] * x * y + 2 * q[2] * x * z + 2 * q[3] * x + q[4] * y * y + 2 * q[5] * y * z + 2 * q[6] * y + q[7] * z * z + 2 * q[8] * z + q[9];
	}

	// Error for one edge

	double calculate_error(int id_v1, int id_v2, vec3f &p_result) {
		// compute interpolated vertex

		SymetricMatrix q = vertices[id_v1].q + vertices[id_v2].q;
		bool border = vertices[id_v1].border & vertices[id_v2].border;
		double error = 0;
		double det = q.det(0, 1, 2, 1, 4, 5, 2, 5, 7);
		if (det != 0 && !border) {
			// q_delta is invertible
			p_result.x = -1 / det * (q.det(1, 2, 3, 4, 5, 6, 5, 7, 8)); // vx = A41/det(q_delta)
			p_result.y = 1 / det * (q.det(0, 2, 3, 1, 5, 6, 2, 7, 8)); // vy = A42/det(q_delta)
			p_result.z = -1 / det * (q.det(0, 1, 3, 1, 4, 6, 2, 5, 8)); // vz = A43/det(q_delta)

			error = vertex_error(q, p_result.x, p_result.y, p_result.z);
		} else {
			// det = 0 -> try to find best result
			vec3f p1 = vertices[id_v1].p;
			vec3f p2 = vertices[id_v2].p;
			vec3f p3 = (p1 + p2) / 2;
			double error1 = vertex_error(q, p1.x, p1.y, p1.z);
			double error2 = vertex_error(q, p2.x, p2.y, p2.z);
			double error3 = vertex_error(q, p3.x, p3.y, p3.z);
			error = min(error1, min(error2, error3));
			if (error1 == error)
				p_result = p1;
			if (error2 == error)
				p_result = p2;
			if (error3 == error)
				p_result = p3;
		}
		return error;
	}

	void initialize(const Array &arrays) {
		ERR_FAIL_COND(arrays.size() != ArrayMesh::ARRAY_MAX);

		PoolVector<Vector3> pvertices = arrays.get(ArrayMesh::ARRAY_VERTEX);
		PoolVector<Vector3> pnormals = arrays.get(ArrayMesh::ARRAY_NORMAL);
		PoolVector<Color> pcolors = arrays.get(ArrayMesh::ARRAY_COLOR);
		PoolVector<Vector2> puvs = arrays.get(ArrayMesh::ARRAY_TEX_UV);
		PoolVector<Vector2> puv2s = arrays.get(ArrayMesh::ARRAY_TEX_UV2);

		_format = 0;

		if (pnormals.size() > 0)
			_format |= RenderingServer::ARRAY_FORMAT_NORMAL;

		if (pcolors.size() > 0)
			_format |= RenderingServer::ARRAY_FORMAT_COLOR;

		if (puvs.size() > 0)
			_format |= RenderingServer::ARRAY_FORMAT_TEX_UV;

		if (puv2s.size() > 0)
			_format |= RenderingServer::ARRAY_FORMAT_TEX_UV2;

		//_vertices.resize(vertices.size());
		for (int i = 0; i < pvertices.size(); ++i) {
			Vector3 v3 = pvertices[i];

			Vertex vert;
			vert.p.x = v3.x;
			vert.p.y = v3.y;
			vert.p.z = v3.z;

			vertices.push_back(vert);
		}

		std::vector<vec3f> uvs;

		for (int i = 0; i < puvs.size(); ++i) {
			Vector2 v2 = puvs[i];

			vec3f uv;
			uv.x = v2.x;
			uv.y = v2.y;
			uv.z = 0;

			uvs.push_back(uv);
		}

		PoolVector<int> pindices = arrays.get(ArrayMesh::ARRAY_INDEX);

		if ((pindices.size() % 3) != 0) {
			ERR_FAIL_MSG("The index array length must be a multiple of 3 in order to represent triangles.");
		}

		//std::vector<std::vector<int> > uvMap;

		for (int i = 0; i < pindices.size(); i += 3) {
			Triangle t;

			int i0 = pindices[i];
			int i1 = pindices[i + 1];
			int i2 = pindices[i + 2];

			t.v[0] = i0;
			t.v[1] = i1;
			t.v[2] = i2;

			if ((_format & RenderingServer::ARRAY_FORMAT_COLOR) != 0) {
				t.color[0] = pcolors[i0];
				t.color[1] = pcolors[i1];
				t.color[2] = pcolors[i2];
			}

			if ((_format & RenderingServer::ARRAY_FORMAT_NORMAL) != 0) {
				Vector3 v = pnormals[i0];

				vec3f vn(v.x, v.y, v.z);

				t.n = vn;
			}

			if ((_format & RenderingServer::ARRAY_FORMAT_TEX_UV) != 0) {
				Vector2 tv0 = puvs[i0];
				Vector2 tv1 = puvs[i1];
				Vector2 tv2 = puvs[i2];

				t.uvs[0] = vec3f(tv0.x, tv0.y, 0);
				t.uvs[1] = vec3f(tv1.x, tv1.y, 0);
				t.uvs[2] = vec3f(tv2.x, tv2.y, 0);
			}

			if ((_format & RenderingServer::ARRAY_FORMAT_TEX_UV2) != 0) {
				Vector2 tv0 = puvs[i0];
				Vector2 tv1 = puvs[i1];
				Vector2 tv2 = puvs[i2];

				t.uv2s[0] = vec3f(tv0.x, tv0.y, 0);
				t.uv2s[1] = vec3f(tv1.x, tv1.y, 0);
				t.uv2s[2] = vec3f(tv2.x, tv2.y, 0);
			}

			//std::vector<int> indices;
			//indices.push_back(pindices[i]);
			//indices.push_back(pindices[i + 1]);
			//indices.push_back(pindices[i + 2]);
			//uvMap.push_back(indices);

			t.material = 0;
			triangles.push_back(t);
		}

		//if (uvs.size()) {
		//	for (int i = 0; i < triangles.size(); ++i) {
		//		for (int j = 0; j < 3; ++j)
		//			triangles[i].uvs[j] = uvs[uvMap[i][j]];
		//	}
		//}
	}

	Array get_arrays() {
		Array arr;

		arr.resize(ArrayMesh::ARRAY_MAX);

		PoolVector<Vector3> pvertices;
		PoolVector<Vector3> pnormals;
		PoolVector<Color> pcolors;
		PoolVector<Vector2> puvs;
		PoolVector<Vector2> puv2s;
		PoolVector<int> pindices;

		pvertices.resize(vertices.size());
		for (int i = 0; i < pvertices.size(); ++i) {
			Vector3 v;
			vec3f vf = vertices[i].p;
			v.x = vf.x;
			v.y = vf.y;
			v.z = vf.z;

			pvertices.set(i, v);
		}

		if ((_format & RenderingServer::ARRAY_FORMAT_COLOR) != 0) {
			pcolors.resize(pvertices.size());

			for (unsigned int i = 0; i < triangles.size(); ++i) {
				Triangle t = triangles[i];

				if (!t.deleted) {
					pcolors.set(t.v[0], t.color[0]);
					pcolors.set(t.v[1], t.color[1]);
					pcolors.set(t.v[2], t.color[2]);
				}
			}

			arr.set(ArrayMesh::ARRAY_COLOR, pcolors);
		}

		if ((_format & RenderingServer::ARRAY_FORMAT_NORMAL) != 0) {
			pnormals.resize(pvertices.size());

			for (unsigned int i = 0; i < triangles.size(); ++i) {
				Triangle t = triangles[i];

				if (!t.deleted) {
					Vector3 v(t.n.x, t.n.y, t.n.z);

					pnormals.set(t.v[0], v);
					pnormals.set(t.v[1], v);
					pnormals.set(t.v[2], v);
				}
			}

			arr.set(ArrayMesh::ARRAY_NORMAL, pnormals);
		}

		if ((_format & RenderingServer::ARRAY_FORMAT_TEX_UV) != 0) {
			puvs.resize(pvertices.size());

			for (unsigned int i = 0; i < triangles.size(); ++i) {
				Triangle t = triangles[i];

				if (!t.deleted) {
					Vector2 v1(t.uvs[0].x, t.uvs[0].y);
					Vector2 v2(t.uvs[1].x, t.uvs[1].y);
					Vector2 v3(t.uvs[2].x, t.uvs[2].y);

					puvs.set(t.v[0], v1);
					puvs.set(t.v[1], v2);
					puvs.set(t.v[2], v3);
				}
			}

			arr.set(ArrayMesh::ARRAY_TEX_UV, puvs);
		}

		if ((_format & RenderingServer::ARRAY_FORMAT_TEX_UV2) != 0) {
			puv2s.resize(pvertices.size());

			for (unsigned int i = 0; i < triangles.size(); ++i) {
				Triangle t = triangles[i];

				if (!t.deleted) {
					Vector2 v1(t.uv2s[0].x, t.uv2s[0].y);
					Vector2 v2(t.uv2s[1].x, t.uv2s[1].y);
					Vector2 v3(t.uv2s[2].x, t.uv2s[2].y);

					puv2s.set(t.v[0], v1);
					puv2s.set(t.v[1], v2);
					puv2s.set(t.v[2], v3);
				}
			}

			arr.set(ArrayMesh::ARRAY_TEX_UV2, puv2s);
		}

		//pindices.resize(_mu_triangles.size() * 3);
		for (unsigned int i = 0; i < triangles.size(); ++i) {
			Triangle t = triangles[i];

			if (!t.deleted) {
				pindices.push_back(t.v[0]);
				pindices.push_back(t.v[1]);
				pindices.push_back(t.v[2]);

				//print_error(String::num(t.v[0]) + " " + String::num(t.v[1]) + " " + String::num(t.v[2]) + " ");
			}
		}

		arr.set(ArrayMesh::ARRAY_VERTEX, pvertices);
		arr.set(ArrayMesh::ARRAY_INDEX, pindices);

		return arr;
	}

	FQMS() {
		_max_iteration_count = 100;
		_max_lossless_iteration_count = 9990;
		_enable_smart_link = false;
		_preserve_border_dges = false;
		_preserve_uv_seam_edges = false;
		_preserve_uv_foldover_edges = false;
		_format = 0;
		_vertex_link_distance = sqrt(DBL_EPSILON);
	}

	~FQMS() {
		triangles.clear();
		vertices.clear();
		refs.clear();
	}
}; // namespace Simplify

} // namespace Simplify

// namespace Simplify
///////////////////////////////////////////

#endif
