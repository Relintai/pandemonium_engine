#include "alias_web_page.h"

#include "../../http/http_server_enums.h"
#include "../../http/web_server_request.h"
#include "core/os/file_access.h"

NodePath AliasWebPage::get_alias_path() {
	return _alias_path;
}
void AliasWebPage::set_alias_path(const NodePath &val) {
	_alias_path = val;
}

void AliasWebPage::_handle_request(Ref<WebServerRequest> request) {
	ERR_FAIL_COND(!_alias);
	ERR_FAIL_COND(!ObjectDB::instance_validate(_alias));

	_alias->handle_request(request);
}

AliasWebPage::AliasWebPage() {
	_alias = nullptr;
}

AliasWebPage::~AliasWebPage() {
}

void AliasWebPage::_notification(int p_what) {
	if (p_what == NOTIFICATION_ENTER_TREE) {
		_alias = Object::cast_to<WebNode>(get_node_or_null(_alias_path));
	} else if (p_what == NOTIFICATION_EXIT_TREE) {
		_alias = nullptr;
	}
}

void AliasWebPage::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_alias_path"), &AliasWebPage::get_alias_path);
	ClassDB::bind_method(D_METHOD("set_alias_path", "val"), &AliasWebPage::set_alias_path);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "alias_path"), "set_alias_path", "get_alias_path");
}
