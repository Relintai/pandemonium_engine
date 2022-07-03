#ifndef PAGED_ARTICLE_H
#define PAGED_ARTICLE_H

#include "core/ustring.h"
#include "core/vector.h"

#include "web/file_cache.h"
#include "web/http/web_node.h"

#include "web/http/request.h"

// This class will load and process all md files from the folder set to it's articles_folder property,
// and serve every file from the directory set to it's serve_folder property.
// if it finds a summary.md, it will serve it as the root.
// It uses pagination.
// THe links is generates currently look like: <url>/01_test.md
// files are served under <url>/files/<file>

class PagedArticle : public WebNode {
	GDCLASS(PagedArticle, WebNode);

public:
	void handle_request_main(Request *request);

	void render_index(Request *request);
	void render_preview(Request *request);

	void load();
	void load_folder(const String &folder, const String &path);
	String get_index_page();
	String get_summary();

	virtual void generate_summary();

	void _notification(const int what);

	PagedArticle();
	~PagedArticle();

	String articles_folder;
	bool serve_folder_relative;
	String serve_folder;

protected:
	String index_page;
	String summary;
	std::map<String, String *> pages;
	FileCache *file_cache;
};

#endif
