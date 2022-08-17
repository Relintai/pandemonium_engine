#ifndef PAGED_ARTICLES_WEB_PAGE_H
#define PAGED_ARTICLES_WEB_PAGE_H

#include "core/string/ustring.h"
#include "core/containers/vector.h"

#include "../../http/web_node.h"

// This class will load and process all md files from the folder set to it's folder property,
// and generate one page from them. TThe files are processed in alphabetical order.

// The generated HTML looks like:

// render_menu()
// <div class="article_list_entry">
//	Contents of the first md file.
// </div>
// <div class="article_list_entry">
//	Contents of the second md file.
// </div>
// ...
// </div>

class WebServerRequest;

class PagedArticlesWebPage : public WebNode {
	GDCLASS(PagedArticlesWebPage, WebNode);

public:
	String get_folder();
	void set_folder(const String &val);

	void _handle_request(Ref<WebServerRequest> request);

	void _render_index(Ref<WebServerRequest> request);
	void _render_preview(Ref<WebServerRequest> request);

	void load();
	void generate_index_page();

	PagedArticlesWebPage();
	~PagedArticlesWebPage();

protected:
	void _notification(int what);

	static void _bind_methods();

	String _folder;

	String _index_page;
};

#endif
