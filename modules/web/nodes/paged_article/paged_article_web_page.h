#ifndef PAGED_ARTICLE_WEB_PAGE_H
#define PAGED_ARTICLE_WEB_PAGE_H

#include "core/reference.h"
#include "core/ustring.h"
#include "core/containers/vector.h"

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

	void _handle_request_main(Ref<WebServerRequest> request);

	void _render_index(Ref<WebServerRequest> request);
	void _render_preview(Ref<WebServerRequest> request);

	void load();
	void load_folder(const String &folder, const String &path);
	String get_index_page();
	String get_summary();

	virtual void generate_summary();

	PagedArticleWebPage();
	~PagedArticleWebPage();

protected:
	void _notification(const int what);
	static void _bind_methods();

	String articles_folder;
	String _articles_folder_abs;
	bool serve_folder_relative;
	String serve_folder;

	String index_page;
	String summary;

	struct PAEntry {
		String url;
		String data;
	};

	Vector<PAEntry> pages;
	Ref<FileCache> file_cache;
};

#endif
