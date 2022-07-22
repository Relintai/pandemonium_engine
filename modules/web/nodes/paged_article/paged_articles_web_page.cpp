#include "paged_articles_web_page.h"

#include "core/os/dir_access.h"
#include "core/os/os.h"
#include "core/project_settings.h"

#include "../../html/html_builder.h"
#include "../../http/web_server_request.h"

#include "paged_article_web_page.h"

String PagedArticlesWebPage::get_folder() {
	return _folder;
}
void PagedArticlesWebPage::set_folder(const String &val) {
	_folder = val;
}

void PagedArticlesWebPage::_handle_request(Ref<WebServerRequest> request) {
	render_menu(request);
	render_index(request);

	request->compile_and_send_body();
}

void PagedArticlesWebPage::_render_index(Ref<WebServerRequest> request) {
	// summary page
	request->body += _index_page;
}
void PagedArticlesWebPage::_render_preview(Ref<WebServerRequest> request) {
}

void PagedArticlesWebPage::load() {
	ERR_FAIL_COND_MSG(_folder == "", "Error: PagedArticlesWebPage::load called, but a folder is not set!");

	String folder = DirAccess::get_filesystem_abspath_for(_folder);
	folder = folder.path_clean_end_slash();

	DirAccess *dir = DirAccess::open(folder);

	ERR_FAIL_COND_MSG(!dir, "Error opening PagedArticlesWebPage::folder! folder: " + folder);

	dir->list_dir_begin();

	String file_name = dir->get_next();
	while (file_name != "") {
		if (dir->current_is_dir() && file_name != "." && file_name != "..") {
			String ff = folder + "/" + file_name;

			PagedArticleWebPage *p = memnew(PagedArticleWebPage);

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

void PagedArticlesWebPage::generate_index_page() {
	HTMLBuilder b;

	b.div("article_list");

	for (int i = 0; i < get_child_count(); ++i) {
		PagedArticleWebPage *a = Object::cast_to<PagedArticleWebPage>(get_child(i));

		if (a) {
			b.a(a->get_full_uri());
			b.div("article_list_entry")->w(a->get_summary())->cdiv();
			b.ca();
		}
	}

	b.cdiv();

	_index_page = b.result;
}

void PagedArticlesWebPage::_notification(int what) {
	switch (what) {
		case NOTIFICATION_ENTER_TREE:
			load();
			break;
		default:
			break;
	}
}

PagedArticlesWebPage::PagedArticlesWebPage() {
}

PagedArticlesWebPage::~PagedArticlesWebPage() {
}

void PagedArticlesWebPage::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_folder"), &PagedArticlesWebPage::get_folder);
	ClassDB::bind_method(D_METHOD("set_folder", "val"), &PagedArticlesWebPage::set_folder);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "folder"), "set_folder", "get_folder");
}
