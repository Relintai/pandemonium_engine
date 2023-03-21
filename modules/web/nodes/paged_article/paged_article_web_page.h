#ifndef PAGED_ARTICLE_WEB_PAGE_H
#define PAGED_ARTICLE_WEB_PAGE_H

#include "core/containers/vector.h"
#include "core/object/reference.h"
#include "core/string/ustring.h"

#include "../../http/web_node.h"

// This class will load and process all md files from the folder set to it's articles_folder property,
// and serve every file from the directory set to it's serve_folder property.
// if it finds a summary.md, it will serve it as the root.
// It uses pagination.
// THe links is generates currently look like: <url>/01_test.md
// files are served under <url>/files/<file>

class WebServerRequest;
class FileCache;

class PagedArticleWebPage : public WebNode {
	GDCLASS(PagedArticleWebPage, WebNode);

public:
	String get_articles_folder();
	void set_articles_folder(const String &val);

	bool get_serve_folder_relative();
	void set_serve_folder_relative(const bool &val);

	String get_serve_folder();
	void set_serve_folder(const String &val);

	int get_max_pagination_links();
	void set_max_pagination_links(const int val);

	String get_summary();
	void set_summary(const String &val);

	void page_set(const String &url, const String &data);
	String page_get(const String &url);
	void page_remove(const String &url);

	Dictionary get_pages();
	void set_pages(const Dictionary &data);

	void _handle_request_main(Ref<WebServerRequest> request);

	void _render_index(Ref<WebServerRequest> request);
	void _render_preview(Ref<WebServerRequest> request);

	void load();
	virtual void _load();

	void generate_summary();
	virtual void _generate_summary();

	PagedArticleWebPage();
	~PagedArticleWebPage();

protected:
	void _notification(const int what);
	static void _bind_methods();

	String articles_folder;
	String _articles_folder_abs;
	bool serve_folder_relative;
	String serve_folder;

	int _max_pagination_links;

	String summary;

	HashMap<String, String> pages;
	Ref<FileCache> file_cache;
};

#endif
