#ifndef ALIAS_WEB_PAGE_H
#define ALIAS_WEB_PAGE_H

#include "core/string/ustring.h"

#include "../../http/web_node.h"

class WebServerRequest;

class AliasWebPage : public WebNode {
	GDCLASS(AliasWebPage, WebNode);

public:
	NodePath get_alias_path();
	void set_alias_path(const NodePath &val);

	int get_code();
	void set_code(const int val);

	void _handle_request(Ref<WebServerRequest> request);

	AliasWebPage();
	~AliasWebPage();

protected:
	void _notification(int p_what);

	static void _bind_methods();

	NodePath _alias_path;
	WebNode *_alias;
};

#endif
