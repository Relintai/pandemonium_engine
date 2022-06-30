#ifndef RBAC_USER_H
#define RBAC_USER_H

#include "core/string.h"

#include "web_modules/users/user.h"
#include "web_modules/rbac/rbac_rank.h"

class Request;
class FormValidator;

class RBACUser : public User {
	RCPP_OBJECT(RBACUser, User);

public:
	Ref<RBACRank> rbac_rank;

	int get_permissions(Request *request);
	bool has_permission(Request *request, const int permission);
	int get_additional_permissions(Request *request);
	bool has_additional_permission(Request *request, const int permission);

	RBACUser();
	~RBACUser();
};

#endif