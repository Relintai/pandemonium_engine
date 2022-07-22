#include "paged_article_web_page.h"

#include "core/os/dir_access.h"
#include "core/os/file_access.h"
#include "core/project_settings.h"

#include "../../file_cache.h"
#include "../../html/paginator.h"
#include "../../http/http_server_enums.h"
#include "../../http/web_permission.h"
#include "../../http/web_server_request.h"

String PagedArticle::get_articles_folder() {
	return articles_folder;
}
void PagedArticle::set_articles_folder(const String &val) {
	articles_folder = val;
}

bool PagedArticle::get_serve_folder_relative() {
	return serve_folder_relative;
}
void PagedArticle::set_serve_folder_relative(const bool &val) {
	serve_folder_relative = val;
}

String PagedArticle::get_serve_folder() {
	return serve_folder;
}
void PagedArticle::set_serve_folder(const String &val) {
	serve_folder = val;
}

void PagedArticle::_handle_request_main(Ref<WebServerRequest> request) {
	if (_web_permission.is_valid()) {
		if (_web_permission->activate(request)) {
			return;
		}
	}

	const String &rp = request->get_current_path_segment();

	if (request->get_remaining_segment_count() > 1 && rp == "files") {
		String file_name = "/" + request->get_path_segment(request->get_current_segment_index() + 1);

		int file_indx = file_cache->wwwroot_get_file_index(file_name);

		if (file_indx != -1) {
			String fp = file_cache->wwwroot_get_file_orig_path_abs(file_indx);

			request->send_file(fp);
			return;
		}
	}

	if (rp == "") {
		render_menu(request);
		render_index(request);

		request->compile_and_send_body();
		return;
	}

	for (int i = 0; i < pages.size(); ++i) {
		const PAEntry &e = pages[i];

		if (e.url == rp) {
			render_menu(request);
			request->body += e.data;
			request->compile_and_send_body();
		}
	}

	request->send_error(HTTPServerEnums::HTTP_STATUS_CODE_404_NOT_FOUND);
}

void PagedArticle::_render_index(Ref<WebServerRequest> request) {
	// summary page
	request->body += index_page;
}

void PagedArticle::_render_preview(Ref<WebServerRequest> request) {
}

void PagedArticle::load() {
	ERR_FAIL_COND_MSG(articles_folder == "", "Error: PagedArticle::load called, but a articles_folder is not set!");

	_articles_folder_abs = DirAccess::get_filesystem_abspath_for(articles_folder);
	_articles_folder_abs = _articles_folder_abs.path_ensure_end_slash();

	DirAccess *dir = DirAccess::open(_articles_folder_abs);

	ERR_FAIL_COND_MSG(!dir, "Error opening PagedArticle::folder! folder: " + _articles_folder_abs);

	Vector<String> files;

	dir->list_dir_begin();

	String file = dir->get_next();

	while (file != "") {
		if (!dir->current_is_dir()) {
			files.push_back(file);
		}

		file = dir->get_next();
	}

	dir->list_dir_end();
	memdelete(dir);
	dir = nullptr;

	if (files.size() == 0) {
		return;
	}

	files.sort();

	for (int i = 0; i < files.size(); ++i) {
		String file_path = _articles_folder_abs;
		file_path += files[i];

		String fd;

		FileAccess *f = FileAccess::open(file_path, FileAccess::READ);

		ERR_CONTINUE_MSG(!f, "Error opening file! " + file_path);

		fd = f->get_as_utf8_string();
		f->close();
		memdelete(f);

		//Utils::markdown_to_html(&fd);

		if (files[i].get_file().get_basename() == "summary") {
			summary = fd;

			continue;
		}

		String pagination;

		pagination = HTMLPaginator::get_pagination_links_old(get_full_uri(false), files, i);

		String finals;

		finals += pagination;
		finals += fd;
		finals += pagination;

		PAEntry e;
		e.url = files[i];
		e.data = finals;

		pages.push_back(e);

		if (i == 0) {
			index_page = finals;
		}
	}

	file_cache->clear();

	if (serve_folder != "") {
		if (serve_folder_relative) {
			file_cache->set_wwwroot(_articles_folder_abs.append_path(serve_folder));
		} else {
			file_cache->set_wwwroot(serve_folder);
		}

		file_cache->wwwroot_refresh_cache();
	}

	if (summary == "") {
		generate_summary();
	}
}

String PagedArticle::get_index_page() {
	return index_page;
}

String PagedArticle::get_summary() {
	return summary;
}

void PagedArticle::generate_summary() {
	summary = get_uri_segment();
}

void PagedArticle::_notification(const int what) {
	switch (what) {
		case NOTIFICATION_ENTER_TREE:
			load();
			break;
		default:
			break;
	}
}

PagedArticle::PagedArticle() {
	file_cache.instance();

	serve_folder_relative = true;
	serve_folder = "files";
}

PagedArticle::~PagedArticle() {
	pages.clear();
}

void PagedArticle::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_articles_folder"), &PagedArticle::get_articles_folder);
	ClassDB::bind_method(D_METHOD("set_articles_folder", "val"), &PagedArticle::set_articles_folder);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "articles_folder"), "set_articles_folder", "get_articles_folder");

	ClassDB::bind_method(D_METHOD("get_serve_folder_relative"), &PagedArticle::get_serve_folder_relative);
	ClassDB::bind_method(D_METHOD("set_serve_folder_relative", "val"), &PagedArticle::set_serve_folder_relative);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "serve_folder_relative"), "set_serve_folder_relative", "get_serve_folder_relative");

	ClassDB::bind_method(D_METHOD("get_serve_folder"), &PagedArticle::get_serve_folder);
	ClassDB::bind_method(D_METHOD("set_serve_folder", "val"), &PagedArticle::set_serve_folder);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "serve_folder"), "set_serve_folder", "get_serve_folder");

	ClassDB::bind_method(D_METHOD("load"), &PagedArticle::load);
	ClassDB::bind_method(D_METHOD("get_index_page"), &PagedArticle::get_index_page);
	ClassDB::bind_method(D_METHOD("get_summary"), &PagedArticle::get_summary);
	ClassDB::bind_method(D_METHOD("generate_summary"), &PagedArticle::generate_summary);
}
