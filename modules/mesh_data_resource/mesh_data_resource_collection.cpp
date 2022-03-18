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

#include "mesh_data_resource_collection.h"

void MeshDataResourceCollection::add_mdr(Ref<MeshDataResource> mdr) {
	_mdrs.push_back(mdr);
}

Vector<Variant> MeshDataResourceCollection::get_mdrs() {
	Vector<Variant> r;
	for (int i = 0; i < _mdrs.size(); i++) {
		r.push_back(_mdrs[i].get_ref_ptr());
	}
	return r;
}
void MeshDataResourceCollection::set_mdrs(const Vector<Variant> &p_arrays) {
	_mdrs.clear();

	for (int i = 0; i < p_arrays.size(); ++i) {
		Ref<MeshDataResource> e = Ref<MeshDataResource>(p_arrays[i]);

		_mdrs.push_back(e);
	}
}

MeshDataResourceCollection::MeshDataResourceCollection() {
}

MeshDataResourceCollection::~MeshDataResourceCollection() {
	_mdrs.clear();
}

void MeshDataResourceCollection::_bind_methods() {
	ClassDB::bind_method(D_METHOD("add_mdr", "mdr"), &MeshDataResourceCollection::add_mdr);

	ClassDB::bind_method(D_METHOD("get_mdrs"), &MeshDataResourceCollection::get_mdrs);
	ClassDB::bind_method(D_METHOD("set_mdrs", "array"), &MeshDataResourceCollection::set_mdrs);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "mdrs"), "set_mdrs", "get_mdrs");
}
