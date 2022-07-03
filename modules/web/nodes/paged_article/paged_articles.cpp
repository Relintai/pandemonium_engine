#include "paged_articles.h"

#include "core/os/dir_access.h"
#include "core/os/os.h"
#include "core/project_settings.h"

#include "../../html/html_builder.h"
#include "../../http/web_server_request.h"

#include "paged_article.h"

String PagedArticles::get_folder() {
	return _folder;
}
void PagedArticles::set_folder(const String &val) {
	_folder = val;
}

void PagedArticles::_handle_request(Ref<WebServerRequest> request) {
	render_menu(request);
	render_index(request);

	request->compile_and_send_body();
}

void PagedArticles::_render_index(Ref<WebServerRequest> request) {
	// summary page
	request->body += _index_page;
}
void PagedArticles::_render_preview(Ref<WebServerRequest> request) {
}

void PagedArticles::load() {
	ERR_FAIL_COND_MSG(_folder == "", "Error: PagedArticles::load called, but a folder is not set!");

	String folder = DirAccess::get_filesystem_abspath_for(_folder);
	folder = folder.path_clean_end_slash();

	DirAccess *dir = DirAccess::open(folder);

	ERR_FAIL_COND_MSG(!dir, "Error opening PagedArticles::folder! folder: " + folder);

	dir->list_dir_begin();

	String file_name = dir->get_next();
	while (file_name != "") {
		if (dir->current_is_dir() && file_name != "." && file_name != "..") {
			String ff = folder + "/" + file_name;

			PagedArticle *p = new PagedArticle();

			p->set_articles_folder(ff);
			p->set_uri_segment(file_name);
			add_child(p);
		}

		file_name = dir->get_next();
	}

	dir->list_dir_end();
	memdelete(dir);

	generate_index_page();
}

void PagedArticles::generate_index_page() {
	HTMLBuilder b;

	b.div("article_list");

	for (int i = 0; i < get_child_count(); ++i) {
		PagedArticle *a = Object::cast_to<PagedArticle>(get_child(i));

		if (a) {
			b.a(a->get_full_uri());
			b.div("article_list_entry")->w(a->get_summary())->cdiv();
			b.ca();
		}
	}

	b.cdiv();

	_index_page = b.result;
}

void PagedArticles::_notification(int what) {
	switch (what) {
		case NOTIFICATION_ENTER_TREE:
			load();
			break;
		default:
			break;
	}
}

PagedArticles::PagedArticles() {
}

PagedArticles::~PagedArticles() {
}

void PagedArticles::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_folder"), &PagedArticles::get_folder);
	ClassDB::bind_method(D_METHOD("set_folder", "val"), &PagedArticles::set_folder);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "folder"), "set_folder", "get_folder");
}
