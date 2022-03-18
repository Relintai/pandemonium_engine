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

#include "register_types.h"

#include "core/version.h"

#include "core/engine.h"

#include "fast_quadratic_mesh_simplifier.h"
#include "mesh_merger.h"
#include "mesh_utils.h"

static MeshUtils *mesh_utils = NULL;

void register_mesh_utils_types() {
	ClassDB::register_class<FastQuadraticMeshSimplifier>();
	ClassDB::register_class<MeshMerger>();

	mesh_utils = memnew(MeshUtils);
	ClassDB::register_class<MeshUtils>();
	Engine::get_singleton()->add_singleton(Engine::Singleton("MeshUtils", MeshUtils::get_singleton()));
}

void unregister_mesh_utils_types() {
	if (mesh_utils) {
		memdelete(mesh_utils);
	}
}
