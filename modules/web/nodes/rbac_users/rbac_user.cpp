#include "rbac_user.h"

int RBACUser::get_permissions(Request *request) {
	if (!rbac_rank.is_valid()) {
		return 0;
	}

	return rbac_rank->get_permissions(request);
}
bool RBACUser::has_permission(Request *request, const int permission) {
	if (!rbac_rank.is_valid()) {
		return false;
	}

	return rbac_rank->has_permission(request, permission);
}
int RBACUser::get_additional_permissions(Request *request) {
	if (!rbac_rank.is_valid()) {
		return 0;
	}

	return rbac_rank->rank_permissions;
}
bool RBACUser::has_additional_permission(Request *request, const int permission) {
	if (!rbac_rank.is_valid()) {
		return false;
	}

	return rbac_rank->rank_permissions & permission;
}

RBACUser::RBACUser() :
		User() {
}

RBACUser::~RBACUser() {
}
