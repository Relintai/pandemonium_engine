#include "rbac_permission.h"

bool RBACPermission::is_smaller(const Ref<RBACPermission> &b) const {
	if (!b.is_valid()) {
		return true;
	}

	return sort_order < b->sort_order;
}

RBACPermission::RBACPermission() :
		Resource() {

	id = 0;
	rank_id = 0;
	sort_order = 0;
	permissions = 0;
}

RBACPermission::~RBACPermission() {
}
