#ifndef REDIRECT_WEB_PAGE_H
#define REDIRECT_WEB_PAGE_H

#include "core/ustring.h"

#include "../../http/web_node.h"

class WebServerRequest;

class RedirectWebPage : public WebNode {
	GDCLASS(RedirectWebPage, WebNode);

public:
	String get_redirect_path();
	void set_redirect_path(const String &val);

	int get_code();
	void set_code(const int val);

	void _handle_request(Ref<WebServerRequest> request);

	RedirectWebPage();
	~RedirectWebPage();

protected:
	static void _bind_methods();

	String _redirect_path;
	int _code;
};

#endif
