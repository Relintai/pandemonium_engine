#ifndef FAST_QUADRATIC_MESH_SIMPLIFIER_H
#define FAST_QUADRATIC_MESH_SIMPLIFIER_H

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

#include "core/variant/array.h"
#include "core/object/reference.h"

#include "simplify.h"

class FastQuadraticMeshSimplifier : public Reference {
	GDCLASS(FastQuadraticMeshSimplifier, Reference);

public:
	int get_max_iteration_count() const;
	void set_max_iteration_count(const int value);

	int get_max_lossless_iteration_count() const;
	void set_max_lossless_iteration_count(const int value);

	bool get_enable_smart_link() const;
	void set_enable_smart_link(const bool value);

	bool get_preserve_border_edges() const;
	void set_preserve_border_edges(const bool value);

	bool get_preserve_uv_seam_edges() const;
	void set_preserve_uv_seam_edges(const bool value);

	bool get_preserve_uv_foldover_edges() const;
	void set_preserve_uv_foldover_edges(const bool value);

	int get_format() const;
	void set_format(const int value);

	void initialize(const Array &arrays);
	Array get_arrays();
	void simplify_mesh(int target_count, double aggressiveness = 7, bool verbose = false);
	void simplify_mesh_lossless(bool verbose = false);

	FastQuadraticMeshSimplifier();
	~FastQuadraticMeshSimplifier();

protected:
	static void _bind_methods();

private:
	Simplify::FQMS simplify;
};

#endif
