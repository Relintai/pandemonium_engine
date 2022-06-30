#ifndef USER_H
#define USER_H

#include "core/string.h"

#include "core/resource.h"
#include "rapidjson/document.h"
#include <mutex>

class Request;
class FormValidator;

class User : public Resource {
	RCPP_OBJECT(User, Resource);

public:
	enum Permissions {
		PERMISSION_CREATE = 1 << 0,
		PERMISSION_READ = 1 << 1,
		PERMISSION_UPDATE = 1 << 2,
		PERMISSION_DELETE = 1 << 3,

		PERMISSION_ALL = PERMISSION_CREATE | PERMISSION_READ | PERMISSION_UPDATE | PERMISSION_DELETE,
		PERMISSION_NONE = 0
	};

	String name_user_input;
	String email_user_input;
	int rank;
	String pre_salt;
	String post_salt;
	String password_hash;
	bool banned;
	String password_reset_token;
	bool locked;

	String to_json(rapidjson::Document *into = nullptr);
	void from_json(const String &data);

	virtual int get_permissions(Request *request);
	virtual bool has_permission(Request *request, const int permission);
	virtual int get_additional_permissions(Request *request);
	virtual bool has_additional_permission(Request *request, const int permission);

	User();
	~User();
};

#endif