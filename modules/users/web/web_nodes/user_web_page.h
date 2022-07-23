#ifndef USER_WEB_PAGE_H
#define USER_WEB_PAGE_H

#include "core/reference.h"
#include "core/ustring.h"
#include "core/vector.h"

#include "modules/web/http/web_node.h"

class WebServerRequest;

class UserWebPage : public WebNode {
	GDCLASS(UserWebPage, WebNode);

public:
	enum RenderType {
		RENDER_TYPE_RENDER = 0,
		RENDER_TYPE_REDIRECT,
		RENDER_TYPE_ERROR,
	};

	RenderType get_logged_out_render_type();
	void set_logged_out_render_type(const RenderType val);

	String get_logged_out_redirect_url();
	void set_logged_out_redirect_url(const String &val);

	int get_logged_out_error_code();
	void set_logged_out_error_code(const int val);

	RenderType get_logged_in_render_type();
	void set_logged_in_render_type(const RenderType val);

	String get_logged_in_redirect_url();
	void set_logged_in_redirect_url(const String &val);

	int get_logged_in_error_code();
	void set_logged_in_error_code(const int val);

	bool get_render_menu();
	void set_render_menu(const bool val);

	void _handle_request(Ref<WebServerRequest> request);
	void _render_index(Ref<WebServerRequest> request);

	void handle_not_logged_in(Ref<WebServerRequest> request);
	virtual void _handle_not_logged_in(Ref<WebServerRequest> request);

	void handle_logged_in(Ref<WebServerRequest> request);
	virtual void _handle_logged_in(Ref<WebServerRequest> request);

	UserWebPage();
	~UserWebPage();

protected:
	static void _bind_methods();

	bool _render_menu;

	RenderType _logged_out_render_type;
	String _logged_out_redirect_url;
	int _logged_out_error_code;

	RenderType _logged_in_render_type;
	String _logged_in_redirect_url;
	int _logged_in_error_code;
};

VARIANT_ENUM_CAST(UserWebPage::RenderType);

#endif
