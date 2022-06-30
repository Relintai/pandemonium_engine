#include "paged_article.h"

#include "core/os/directory.h"
#include "web/html/utils.h"
#include "web/http/web_permission.h"

#include <iostream>

void PagedArticle::handle_request_main(Request *request) {
	if (_web_permission.is_valid()) {
		if (_web_permission->activate(request)) {
			return;
		}
	}

	const String &rp = request->get_current_path_segment();

	if (request->get_remaining_segment_count() > 1 && rp == "files") {
		String file_name = "/" + request->get_path_segment(request->get_current_segment_index() + 1);

		if (file_cache->wwwroot_has_file(file_name)) {
			String fp = file_cache->wwwroot + file_name;

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

	const String *page = pages[rp];

	if (page == nullptr) {
		// bad url
		request->send_error(404);
		return;
	}

	render_menu(request);
	request->body += (*page);
	request->compile_and_send_body();
}

void PagedArticle::render_index(Request *request) {
	// summary page
	request->body += index_page;
}

void PagedArticle::render_preview(Request *request) {
}

void PagedArticle::load() {
	ERR_FAIL_COND_MSG(articles_folder == "", "Error: PagedArticle::load called, but a articles_folder is not set!");

	Ref<Directory> dir;
	dir.instance();

	ERR_FAIL_COND_MSG(dir->open_dir(articles_folder.c_str()) != OK, "Error opening PagedArticle::folder! folder: " + articles_folder);

	Vector<String> files;

	while (dir->next()) {
		if (dir->current_is_file()) {
			files.push_back(dir->current_get_name());
		}
	}

	dir->close_dir();

	if (files.size() == 0) {
		return;
	}

	files.sort_inc();

	for (uint32_t i = 0; i < files.size(); ++i) {
		String file_path = articles_folder;
		file_path.append_path(files[i]);

		String fd;

		ERR_CONTINUE_MSG(dir->read_file_into(file_path, &fd) != OK, "PagedArticle::load_folder: Error opening file! " + file_path);

		Utils::markdown_to_html(&fd);

		if (files[i] == "summary.md") {
			summary = fd;

			continue;
		}

		String pagination;

		pagination = Utils::get_pagination_links(get_full_uri(), files, i);

		String *finals = new String();

		(*finals) += pagination;
		(*finals) += fd;
		(*finals) += pagination;

		pages[files[i]] = finals;

		if (i == 0) {
			index_page = (*finals);
		}
	}

	file_cache->clear();

	if (serve_folder != "") {
		if (serve_folder_relative) {
			file_cache->wwwroot = articles_folder;
			file_cache->wwwroot.append_path(serve_folder);
		} else {
			file_cache->wwwroot = serve_folder;
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

PagedArticle::PagedArticle() :
		WebNode() {

	file_cache = new FileCache();

	serve_folder_relative = true;
	serve_folder = "files";
}

PagedArticle::~PagedArticle() {
	for (std::map<String, String *>::iterator it = pages.begin(); it != pages.end(); ++it) {
		delete ((*it).second);
	}

	pages.clear();

	delete file_cache;
}