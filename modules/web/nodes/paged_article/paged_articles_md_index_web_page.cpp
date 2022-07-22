#include "paged_articles_md_index_web_page.h"

#include "core/os/dir_access.h"

#include "../../html/html_builder.h"
#include "../../http/web_permission.h"
#include "../../http/web_server_request.h"

#include "../list_page/list_web_page.h"
#include "paged_article_web_page.h"

String PagedArticlesWebPageMDIndex::get_folder() {
	return folder;
}
void PagedArticlesWebPageMDIndex::set_folder(const String &val) {
	folder = val;
}

void PagedArticlesWebPageMDIndex::_handle_request_main(Ref<WebServerRequest> request) {
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

void PagedArticlesWebPageMDIndex::_render_index(Ref<WebServerRequest> request) {
	main_page->render_index(request);
}

void PagedArticlesWebPageMDIndex::_render_preview(Ref<WebServerRequest> request) {
	main_page->render_preview(request);
}

void PagedArticlesWebPageMDIndex::load() {
	main_page->set_folder(folder);
	main_page->load();

	articles->set_articles_folder(folder);
	//articles->set_serve_folder(folder + "/files");
	articles->load();
}

PagedArticlesWebPageMDIndex::PagedArticlesWebPageMDIndex() {
	main_page = memnew(ListPage);
	articles = memnew(PagedArticle);
}

PagedArticlesWebPageMDIndex::~PagedArticlesWebPageMDIndex() {
	memdelete(main_page);
	memdelete(articles);
}

void PagedArticlesWebPageMDIndex::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_folder"), &PagedArticlesWebPageMDIndex::get_folder);
	ClassDB::bind_method(D_METHOD("set_folder", "val"), &PagedArticlesWebPageMDIndex::set_folder);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "folder"), "set_folder", "get_folder");
}
