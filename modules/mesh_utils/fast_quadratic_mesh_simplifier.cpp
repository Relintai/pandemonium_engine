#include "fast_quadratic_mesh_simplifier.h"

/*

Copyright (c) 2020-2023 Péter Magyar

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

int FastQuadraticMeshSimplifier::get_max_iteration_count() const {
	return simplify._max_iteration_count;
}
void FastQuadraticMeshSimplifier::set_max_iteration_count(const int value) {
	simplify._max_iteration_count = value;
}

int FastQuadraticMeshSimplifier::get_max_lossless_iteration_count() const {
	return simplify._max_lossless_iteration_count;
}
void FastQuadraticMeshSimplifier::set_max_lossless_iteration_count(const int value) {
	simplify._max_lossless_iteration_count = value;
}

bool FastQuadraticMeshSimplifier::get_enable_smart_link() const {
	return simplify._enable_smart_link;
}
void FastQuadraticMeshSimplifier::set_enable_smart_link(const bool value) {
	simplify._enable_smart_link = value;
}

bool FastQuadraticMeshSimplifier::get_preserve_border_edges() const {
	return simplify._preserve_border_dges;
}
void FastQuadraticMeshSimplifier::set_preserve_border_edges(const bool value) {
	simplify._preserve_border_dges = value;
}

bool FastQuadraticMeshSimplifier::get_preserve_uv_seam_edges() const {
	return simplify._preserve_uv_seam_edges;
}
void FastQuadraticMeshSimplifier::set_preserve_uv_seam_edges(const bool value) {
	simplify._preserve_uv_seam_edges = value;
}

bool FastQuadraticMeshSimplifier::get_preserve_uv_foldover_edges() const {
	return simplify._preserve_uv_foldover_edges;
}
void FastQuadraticMeshSimplifier::set_preserve_uv_foldover_edges(const bool value) {
	simplify._preserve_uv_foldover_edges = value;
}

int FastQuadraticMeshSimplifier::get_format() const {
	return simplify._format;
}
void FastQuadraticMeshSimplifier::set_format(const int value) {
	simplify._format = value;
}

void FastQuadraticMeshSimplifier::initialize(const Array &arrays) {
	simplify.initialize(arrays);
}

Array FastQuadraticMeshSimplifier::get_arrays() {
	return simplify.get_arrays();
}

void FastQuadraticMeshSimplifier::simplify_mesh(int target_count, double aggressiveness, bool verbose) {
	simplify.simplify_mesh(target_count, aggressiveness, verbose);
}

void FastQuadraticMeshSimplifier::simplify_mesh_lossless(bool verbose) {
	simplify.simplify_mesh_lossless(verbose);
}

FastQuadraticMeshSimplifier::FastQuadraticMeshSimplifier() {
}

FastQuadraticMeshSimplifier::~FastQuadraticMeshSimplifier() {
}

void FastQuadraticMeshSimplifier::_bind_methods() {
	ClassDB::bind_method(D_METHOD("initialize", "arrays"), &FastQuadraticMeshSimplifier::initialize);
	ClassDB::bind_method(D_METHOD("get_arrays"), &FastQuadraticMeshSimplifier::get_arrays);
	ClassDB::bind_method(D_METHOD("simplify_mesh", "target_count", "aggressiveness", "verbose"), &FastQuadraticMeshSimplifier::simplify_mesh, DEFVAL(7), DEFVAL(false));
	ClassDB::bind_method(D_METHOD("simplify_mesh_lossless", "verbose"), &FastQuadraticMeshSimplifier::simplify_mesh_lossless, DEFVAL(false));

	ClassDB::bind_method(D_METHOD("get_max_iteration_count"), &FastQuadraticMeshSimplifier::get_max_iteration_count);
	ClassDB::bind_method(D_METHOD("set_max_iteration_count", "value"), &FastQuadraticMeshSimplifier::set_max_iteration_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_iteration_count"), "set_max_iteration_count", "get_max_iteration_count");

	ClassDB::bind_method(D_METHOD("get_max_lossless_iteration_count"), &FastQuadraticMeshSimplifier::get_max_lossless_iteration_count);
	ClassDB::bind_method(D_METHOD("set_max_lossless_iteration_count", "value"), &FastQuadraticMeshSimplifier::set_max_lossless_iteration_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_lossless_iteration_count"), "set_max_lossless_iteration_count", "get_max_lossless_iteration_count");

	ClassDB::bind_method(D_METHOD("get_enable_smart_link"), &FastQuadraticMeshSimplifier::get_enable_smart_link);
	ClassDB::bind_method(D_METHOD("set_enable_smart_link", "value"), &FastQuadraticMeshSimplifier::set_enable_smart_link);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "enable_smart_link"), "set_enable_smart_link", "get_enable_smart_link");

	ClassDB::bind_method(D_METHOD("get_preserve_border_edges"), &FastQuadraticMeshSimplifier::get_preserve_border_edges);
	ClassDB::bind_method(D_METHOD("set_preserve_border_edges", "value"), &FastQuadraticMeshSimplifier::set_preserve_border_edges);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "preserve_border_edges"), "set_preserve_border_edges", "get_preserve_border_edges");

	ClassDB::bind_method(D_METHOD("get_preserve_uv_seam_edges"), &FastQuadraticMeshSimplifier::get_preserve_uv_seam_edges);
	ClassDB::bind_method(D_METHOD("set_preserve_uv_seam_edges", "value"), &FastQuadraticMeshSimplifier::set_preserve_uv_seam_edges);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "preserve_uv_seam_edges"), "set_preserve_uv_seam_edges", "get_preserve_uv_seam_edges");

	ClassDB::bind_method(D_METHOD("get_preserve_uv_foldover_edges"), &FastQuadraticMeshSimplifier::get_preserve_uv_foldover_edges);
	ClassDB::bind_method(D_METHOD("set_preserve_uv_foldover_edges", "value"), &FastQuadraticMeshSimplifier::set_preserve_uv_foldover_edges);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "preserve_uv_foldover_edges"), "set_preserve_uv_foldover_edges", "get_preserve_uv_foldover_edges");

	ClassDB::bind_method(D_METHOD("get_format"), &FastQuadraticMeshSimplifier::get_format);
	ClassDB::bind_method(D_METHOD("set_format", "value"), &FastQuadraticMeshSimplifier::set_format);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "format"), "set_format", "get_format");
}
