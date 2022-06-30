#ifndef LIST_PAGE_H
#define LIST_PAGE_H

#include "core/containers/vector.h"
#include "core/string.h"

#include "web/http/web_node.h"

#include "web/http/request.h"

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

class ListPage : public WebNode {
	RCPP_OBJECT(ListPage, WebNode);

public:
	void handle_request_main(Request *request);

	void render_index(Request *request);
	void render_preview(Request *request);

	void load();

	virtual void render_entries(const Vector<String> &list_entries);
	virtual String render_page(const int page_index, const int page_count, const Vector<String> &list_entries, const int efrom, const int eto);
	virtual String render_entry(const String &list_entry);
	virtual void render_no_entries_response();

	void _notification(const int what);

	ListPage();
	~ListPage();

	bool paginate;
	int max_visible_navigation_links;
	int entry_per_page;
	String folder;

	String main_div_class;
	String entry_div_class;
	String empty_div_class;
	String placeholder_text;

protected:
	Vector<String> _pages;
	String _no_entries_response;
};

#endif