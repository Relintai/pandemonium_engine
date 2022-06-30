#include "user.h"

#include "rapidjson/document.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/stringbuffer.h"
#include <rapidjson/writer.h>
#include <tinydir/tinydir.h>
#include <cstdio>

#include "database/database_manager.h"
#include "database/query_builder.h"
#include "database/query_result.h"
#include "database/table_builder.h"

#include "web/html/form_validator.h"
#include "web/html/html_builder.h"
#include "web/http/cookie.h"
#include "web/http/http_session.h"
#include "web/http/request.h"
#include "web/http/session_manager.h"
#include "web/html/utils.h"

String User::to_json(rapidjson::Document *into) {
	rapidjson::Document *document;

	if (into) {
		document = into;
	} else {
		document = new rapidjson::Document();
	}

	document->SetObject();

	document->AddMember("id", id, document->GetAllocator());

	document->AddMember("name", rapidjson::Value(name_user_input.c_str(), document->GetAllocator()), document->GetAllocator());
	document->AddMember("email", rapidjson::Value(email_user_input.c_str(), document->GetAllocator()), document->GetAllocator());
	document->AddMember("rank", rank, document->GetAllocator());
	document->AddMember("pre_salt", rapidjson::Value(pre_salt.c_str(), document->GetAllocator()), document->GetAllocator());
	document->AddMember("post_salt", rapidjson::Value(post_salt.c_str(), document->GetAllocator()), document->GetAllocator());
	document->AddMember("password_hash", rapidjson::Value(password_hash.c_str(), document->GetAllocator()), document->GetAllocator());
	document->AddMember("banned", banned, document->GetAllocator());
	document->AddMember("password_reset_token", rapidjson::Value(password_reset_token.c_str(), document->GetAllocator()), document->GetAllocator());
	document->AddMember("locked", locked, document->GetAllocator());

	if (into) {
		return "";
	}

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	document->Accept(writer);

	String s = buffer.GetString();

	delete document;

	return s;
}

void User::from_json(const String &p_data) {

	rapidjson::Document data;
	data.Parse(p_data.c_str());

	rapidjson::Value uobj = data.GetObject();

	id = uobj["id"].GetInt();
	name_user_input = uobj["name"].GetString();
	email_user_input = uobj["email"].GetString();
	rank = uobj["rank"].GetInt();
	pre_salt = uobj["pre_salt"].GetString();
	post_salt = uobj["post_salt"].GetString();
	password_hash = uobj["password_hash"].GetString();
	banned = uobj["banned"].GetBool();

	password_reset_token = uobj["password_reset_token"].GetString();
	locked = uobj["locked"].GetBool();
}

int User::get_permissions(Request *request) {
	return PERMISSION_ALL;
}

bool User::has_permission(Request *request, const int permission) {
	return true;
}

int User::get_additional_permissions(Request *request) {
	return 0;
}

bool User::has_additional_permission(Request *request, const int permission) {
	return true;
}

User::User() :
		Resource() {

	rank = 0;
	banned = false;
	locked = false;
}

User::~User() {
}
