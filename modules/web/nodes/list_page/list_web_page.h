#ifndef LIST_WEB_PAGE_H
#define LIST_WEB_PAGE_H

#include "core/string/ustring.h"
#include "core/containers/vector.h"

#include "../../http/web_node.h"

// This class will load and generate pages from a folder of md files. It supports pagination,
// it will put entry_per_page md files per page. It generates html on enter tree, and caches everything.
// Each md file gets rendered into a div with a class of "list_entry"

// HTML (If there are entries):
// render_menu()
// <div class="list_page">                       // Set the class via the main_div_class property
// <div class="list_entry">md file 1</div>       // Set the class via the entry_div_class property
// <div class="list_entry">md file 2</div>
// ...
// <pagination>
// </div>

// HTML (If there are no entries):
// render_menu()
// <div class="list_page">                               // Set the class via the main_div_class property
// <div class="list_entry_empty">No content yet!</div>   // Set the class via the empty_div_class property, text via placeholder_text property
// </div>

class WebServerRequest;

class ListWebPage : public WebNode {
	GDCLASS(ListWebPage, WebNode);

public:
	bool get_paginate();
	void set_paginate(const bool &val);

	int get_max_visible_navigation_links();
	void set_max_visible_navigation_links(const int &val);

	int get_entry_per_page();
	void set_entry_per_page(const int &val);

	String get_folder();
	void set_folder(const String &val);

	String get_main_div_class();
	void set_main_div_class(const String &val);

	String get_entry_div_class();
	void set_entry_div_class(const String &val);

	String get_empty_div_class();
	void set_empty_div_class(const String &val);

	String get_placeholder_text();
	void set_placeholder_text(const String &val);

	void _handle_request_main(Ref<WebServerRequest> request);

	void _render_index(Ref<WebServerRequest> request);
	void _render_preview(Ref<WebServerRequest> request);

	void load();

	virtual void render_entries(const Vector<String> &list_entries);
	virtual String render_page(const int page_index, const int page_count, const Vector<String> &list_entries, const int efrom, const int eto);
	virtual String render_entry(const String &list_entry);
	virtual void render_no_entries_response();

	ListWebPage();
	~ListWebPage();

protected:
	void _notification(const int what);
	static void _bind_methods();

	bool paginate;
	int max_visible_navigation_links;
	int entry_per_page;
	String folder;

	String main_div_class;
	String entry_div_class;
	String empty_div_class;
	String placeholder_text;

	Vector<String> _pages;
	String _no_entries_response;
};

#endif
