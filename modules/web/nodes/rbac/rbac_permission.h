#ifndef RBAC_PERMISSION_H
#define RBAC_PERMISSION_H

#include "core/string.h"

#include "core/resource.h"

class RBACPermission : public Resource {
	RCPP_OBJECT(RBACPermission, Resource);

public:
	int id;
	int rank_id;
	String name;
	String url;
	int sort_order;
	int permissions;

	bool is_smaller(const Ref<RBACPermission> &b) const;

	RBACPermission();
	~RBACPermission();
};

#endif