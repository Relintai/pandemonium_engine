#ifndef STATIC_WEB_PAGE_H
#define STATIC_WEB_PAGE_H

#include "core/string/ustring.h"

#include "../../http/web_node.h"

class WebServerRequest;

class StaticWebPage : public WebNode {
	GDCLASS(StaticWebPage, WebNode);

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

	void load_md_file(const String &path);
	void set_data_md(const String &data);

	StaticWebPage();
	~StaticWebPage();

protected:
	static void _bind_methods();

	String _data;
	String _preview_data;
	bool _should_render_menu;
};

#endif
