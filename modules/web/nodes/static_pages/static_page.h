#ifndef STATIC_PAGE_H
#define STATIC_PAGE_H

#include "core/ustring.h"

#include "../../http/web_node.h"

class WebServerRequest;

class StaticPage : public WebNode {
	GDCLASS(StaticPage, WebNode);

public:
	String get_data();
	void set_data(const String &val);

	String get_preview_data();
	void set_preview_data(const String &val);

	bool get_should_render_menu();
	void set_should_render_menu(const bool &val);

	void _handle_request(Ref<WebServerRequest> request);

	void _render_index(Ref<WebServerRequest> request);
	void _render_preview(Ref<WebServerRequest> request);

	void load_file(const String &path);
	void load_and_process_file(const String &path);

	StaticPage();
	~StaticPage();

protected:
	static void _bind_methods();

	String _data;
	String _preview_data;
	bool _should_render_menu;
};

#endif
