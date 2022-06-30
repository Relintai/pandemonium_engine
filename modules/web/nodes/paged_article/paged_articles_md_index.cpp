#include "paged_articles_md_index.h"

#include "web/html/html_builder.h"
#include "web/http/request.h"
#include "web/http/web_permission.h"

void PagedArticlesMDIndex::handle_request_main(Request *request) {
	if (_web_permission.is_valid()) {
		if (_web_permission->activate(request)) {
			return;
		}
	}

	const String path = request->get_current_path_segment();

	if (request->get_remaining_segment_count() == 0) {
		main_page->handle_request_main(request);
		return;
	}

	articles->handle_request_main(request);
}

void PagedArticlesMDIndex::render_index(Request *request) {
	main_page->render_index(request);
}

void PagedArticlesMDIndex::render_preview(Request *request) {
	main_page->render_preview(request);
}

void PagedArticlesMDIndex::load() {
	main_page->folder = folder;
	main_page->load();

	articles->articles_folder = folder;
	articles->serve_folder = folder + "/files";
	articles->load();
}

PagedArticlesMDIndex::PagedArticlesMDIndex() :
		WebNode() {

	main_page = new ListPage();
	articles = new PagedArticle();
}

PagedArticlesMDIndex::~PagedArticlesMDIndex() {
	delete main_page;
	delete articles;
}
