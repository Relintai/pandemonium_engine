#ifndef RBAC_USER_CONTROLLER_H
#define RBAC_USER_CONTROLLER_H

#include "web_modules/users/user_controller.h"

#include "web/http/middleware.h"

class Request;

class RBACUserController : public UserController {
	RCPP_OBJECT(RBACUserController, UserController);

public:
	// db

	Ref<User> db_get_user(const int id);
	Ref<User> db_get_user(const String &user_name_input);

	Vector<Ref<User> > db_get_all();

	Ref<User> create_user();

	RBACUserController();
	~RBACUserController();

protected:
};

// just session setup
class RBACUserSessionSetupMiddleware : public Middleware {
	RCPP_OBJECT(RBACUserSessionSetupMiddleware, Middleware);

public:
	//returnring true means handled, false means continue
	bool on_before_handle_request_main(Request *request);

	RBACUserSessionSetupMiddleware();
	~RBACUserSessionSetupMiddleware();
};

// this one also handles missing read permission / redirect
class RBACDefaultUserSessionSetupMiddleware : public Middleware {
	RCPP_OBJECT(RBACDefaultUserSessionSetupMiddleware, Middleware);

public:
	//returnring true means handled, false means continue
	bool on_before_handle_request_main(Request *request);

	RBACDefaultUserSessionSetupMiddleware();
	~RBACDefaultUserSessionSetupMiddleware();
};


#endif