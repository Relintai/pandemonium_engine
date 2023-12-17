

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
