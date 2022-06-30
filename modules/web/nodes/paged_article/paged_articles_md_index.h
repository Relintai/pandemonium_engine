#ifndef PAGED_ARTICLES_MD_INDEX_H
#define PAGED_ARTICLES_MD_INDEX_H

#include "core/string.h"

#include "web/http/web_node.h"

#include "web_modules/list_page/list_page.h"
#include "web_modules/paged_article/paged_article.h"

// Inherit from PagedArticles and override generate_index_page -> load and process md files in the set folder
// SHould probably be called something else. PagedArticlesMDIndex ?

class PagedArticlesMDIndex : public WebNode {
	RCPP_OBJECT(PagedArticlesMDIndex, WebNode);

public:
	void handle_request_main(Request *request);

	void render_index(Request *request);
	void render_preview(Request *request);

	void load();

	PagedArticlesMDIndex();
	~PagedArticlesMDIndex();

	String folder;
	String base_path;

	ListPage *main_page;
	PagedArticle *articles;
};

#endif