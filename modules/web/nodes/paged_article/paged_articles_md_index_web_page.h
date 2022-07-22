#ifndef PAGED_ARTICLES_WEB_PAGE_MD_INDEX_H
#define PAGED_ARTICLES_WEB_PAGE_MD_INDEX_H

#include "core/ustring.h"

#include "../../http/web_node.h"

// Inherit from PagedArticlesWebPage and override generate_index_page -> load and process md files in the set folder
// SHould probably be called something else. PagedArticlesWebPageMDIndex ?

class WebServerRequest;
class ListPage;
class PagedArticle;

class PagedArticlesWebPageMDIndex : public WebNode {
	GDCLASS(PagedArticlesWebPageMDIndex, WebNode);

public:
	String get_folder();
	void set_folder(const String &val);

	void _handle_request_main(Ref<WebServerRequest> request);

	void _render_index(Ref<WebServerRequest> request);
	void _render_preview(Ref<WebServerRequest> request);

	void load();

	PagedArticlesWebPageMDIndex();
	~PagedArticlesWebPageMDIndex();

protected:
	static void _bind_methods();

	String folder;

	ListPage *main_page;
	PagedArticle *articles;
};

#endif
