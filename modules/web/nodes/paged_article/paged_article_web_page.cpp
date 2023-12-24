/*************************************************************************/
/*  paged_article_web_page.cpp                                           */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "paged_article_web_page.h"

#include "core/config/project_settings.h"
#include "core/os/dir_access.h"
#include "core/os/file_access.h"

#include "../../file_cache.h"
#include "../../html/markdown_renderer.h"
#include "../../html/paginator.h"
#include "../../http/http_server_enums.h"
#include "../../http/web_permission.h"
#include "../../http/web_server_request.h"

String PagedArticleWebPage::get_articles_folder() {
	return articles_folder;
}
void PagedArticleWebPage::set_articles_folder(const String &val) {
	articles_folder = val;
}

bool PagedArticleWebPage::get_serve_folder_relative() {
	return serve_folder_relative;
}
void PagedArticleWebPage::set_serve_folder_relative(const bool &val) {
	serve_folder_relative = val;
}

String PagedArticleWebPage::get_serve_folder() {
	return serve_folder;
}
void PagedArticleWebPage::set_serve_folder(const String &val) {
	serve_folder = val;
}

int PagedArticleWebPage::get_max_pagination_links() {
	return _max_pagination_links;
}
void PagedArticleWebPage::set_max_pagination_links(const int val) {
	_max_pagination_links = val;
}

String PagedArticleWebPage::get_summary() {
	return summary;
}
void PagedArticleWebPage::set_summary(const String &val) {
	summary = val;
}

void PagedArticleWebPage::page_set(const String &url, const String &data) {
	pages[url] = data;
}
String PagedArticleWebPage::page_get(const String &url) {
	if (pages.has(url)) {
		return pages[url];
	}

	ERR_FAIL_V("");
}
void PagedArticleWebPage::page_remove(const String &url) {
	if (pages.has(url)) {
		pages.erase(url);
	}
}

Dictionary PagedArticleWebPage::get_pages() {
	Dictionary d;

	for (HashMap<String, String>::Element *E = pages.front(); E; E = E->next) {
		d[E->key()] = E->value();
	}

	return d;
}
void PagedArticleWebPage::set_pages(const Dictionary &data) {
	List<Variant> keys;

	data.get_key_list(&keys);

	pages.clear();

	for (List<Variant>::Element *E = keys.front(); E; E = E->next()) {
		pages[E->get()] = data[E->get()];
	}
}

void PagedArticleWebPage::_handle_request_main(Ref<WebServerRequest> request) {
	if (_web_permission.is_valid()) {
		if (_web_permission->activate(request)) {
			return;
		}
	}

	const String &rp = request->get_current_path_segment();

	if (request->get_remaining_segment_count() > 1 && rp == "files") {
		String file_name = "/" + request->get_path_segment(request->get_current_segment_index() + 1);
		String file_path = file_cache->wwwroot_get_file_abspath(file_name);

		if (!file_path.empty()) {
			request->send_file(file_path);
			return;
		}
	}

	if (rp == "") {
		render_menu(request);
		render_index(request);

		request->compile_and_send_body();
		return;
	}

	if (pages.has(rp)) {
		render_menu(request);
		request->body += pages[rp];
		request->compile_and_send_body();
		return;
	}

	request->send_error(HTTPServerEnums::HTTP_STATUS_CODE_404_NOT_FOUND);
}

void PagedArticleWebPage::_render_index(Ref<WebServerRequest> request) {
	// summary page
	request->body += summary;
}

void PagedArticleWebPage::_render_preview(Ref<WebServerRequest> request) {
}

void PagedArticleWebPage::load() {
	call("_load");
}

void PagedArticleWebPage::_load() {
	ERR_FAIL_COND_MSG(articles_folder == "", "Error: PagedArticleWebPage::load called, but a articles_folder is not set!");

	_articles_folder_abs = articles_folder; //DirAccess::get_filesystem_abspath_for(articles_folder);
	_articles_folder_abs = _articles_folder_abs.path_ensure_end_slash();

	DirAccess *dir = DirAccess::open(_articles_folder_abs);

	ERR_FAIL_COND_MSG(!dir, "Error opening PagedArticleWebPage::folder! folder: " + _articles_folder_abs);

	Vector<String> files;
	String summary_file;

	dir->list_dir_begin();

	String file = dir->get_next();

	while (!file.empty()) {
		if (!dir->current_is_dir()) {
			if (file.get_basename() == "summary") {
				summary_file = file;
			} else {
				files.push_back(file);
			}
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

	Ref<MarkdownRenderer> r;
	r.instance();

	for (int i = 0; i < files.size(); ++i) {
		String file_path = _articles_folder_abs;
		file_path += files[i];

		String fd;

		FileAccess *f = FileAccess::open(file_path, FileAccess::READ);

		ERR_CONTINUE_MSG(!f, "Error opening file! " + file_path);

		fd = f->get_as_utf8_string();
		f->close();
		memdelete(f);

		if (files[i].get_extension() == "md") {
			fd = r->render_to_html(fd);
		}

		String pagination;

		pagination = HTMLPaginator::get_pagination_links_old(get_full_uri(false), files, i, _max_pagination_links);

		String finals;

		finals += pagination;
		finals += fd;
		finals += pagination;

		pages[files[i]] = finals;
	}

	if (!summary_file.empty()) {
		String file_path = _articles_folder_abs;
		file_path += summary_file;

		String fd;

		FileAccess *f = FileAccess::open(file_path, FileAccess::READ);

		if (!f) {
			ERR_PRINT("Error opening file! " + file_path);
		}

		fd = f->get_as_utf8_string();
		f->close();
		memdelete(f);

		if (summary_file.get_extension() == "md") {
			fd = r->render_to_html(fd);
		}

		summary = fd;
	}

	file_cache->clear();

	if (serve_folder != "") {
		if (serve_folder_relative) {
			file_cache->set_wwwroot(_articles_folder_abs.append_path(serve_folder));
		} else {
			file_cache->set_wwwroot(serve_folder);
		}
	}

	if (summary.empty()) {
		generate_summary();
	}
}

void PagedArticleWebPage::generate_summary() {
	call("_generate_summary");
}

void PagedArticleWebPage::_generate_summary() {
	// TODO generate a page with links
	summary = get_uri_segment();
}

void PagedArticleWebPage::_notification(const int what) {
	switch (what) {
		case NOTIFICATION_ENTER_TREE:
			load();
			break;
		default:
			break;
	}
}

PagedArticleWebPage::PagedArticleWebPage() {
	file_cache.instance();

	_max_pagination_links = 10;

	serve_folder_relative = true;
	serve_folder = "files";
}

PagedArticleWebPage::~PagedArticleWebPage() {
	pages.clear();
}

void PagedArticleWebPage::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_articles_folder"), &PagedArticleWebPage::get_articles_folder);
	ClassDB::bind_method(D_METHOD("set_articles_folder", "val"), &PagedArticleWebPage::set_articles_folder);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "articles_folder"), "set_articles_folder", "get_articles_folder");

	ClassDB::bind_method(D_METHOD("get_serve_folder_relative"), &PagedArticleWebPage::get_serve_folder_relative);
	ClassDB::bind_method(D_METHOD("set_serve_folder_relative", "val"), &PagedArticleWebPage::set_serve_folder_relative);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "serve_folder_relative"), "set_serve_folder_relative", "get_serve_folder_relative");

	ClassDB::bind_method(D_METHOD("get_serve_folder"), &PagedArticleWebPage::get_serve_folder);
	ClassDB::bind_method(D_METHOD("set_serve_folder", "val"), &PagedArticleWebPage::set_serve_folder);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "serve_folder"), "set_serve_folder", "get_serve_folder");

	ClassDB::bind_method(D_METHOD("get_max_pagination_links"), &PagedArticleWebPage::get_max_pagination_links);
	ClassDB::bind_method(D_METHOD("set_max_pagination_links", "val"), &PagedArticleWebPage::set_max_pagination_links);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_pagination_links"), "set_max_pagination_links", "get_max_pagination_links");

	ClassDB::bind_method(D_METHOD("get_summary"), &PagedArticleWebPage::get_summary);
	ClassDB::bind_method(D_METHOD("set_summary", "val"), &PagedArticleWebPage::set_summary);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "summary", PROPERTY_HINT_NONE, "", 0), "set_summary", "get_summary");

	ClassDB::bind_method(D_METHOD("page_set", "url", "data"), &PagedArticleWebPage::page_set);
	ClassDB::bind_method(D_METHOD("page_get", "url"), &PagedArticleWebPage::page_get);
	ClassDB::bind_method(D_METHOD("page_remove", "url"), &PagedArticleWebPage::page_remove);

	ClassDB::bind_method(D_METHOD("get_pages"), &PagedArticleWebPage::get_pages);
	ClassDB::bind_method(D_METHOD("set_pages", "val"), &PagedArticleWebPage::set_pages);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "pages", PROPERTY_HINT_NONE, "", 0), "set_pages", "get_pages");

	BIND_VMETHOD(MethodInfo("_load"));

	ClassDB::bind_method(D_METHOD("load"), &PagedArticleWebPage::load);
	ClassDB::bind_method(D_METHOD("_load"), &PagedArticleWebPage::_load);

	BIND_VMETHOD(MethodInfo("_generate_summary"));

	ClassDB::bind_method(D_METHOD("generate_summary"), &PagedArticleWebPage::generate_summary);
	ClassDB::bind_method(D_METHOD("_generate_summary"), &PagedArticleWebPage::_generate_summary);
}
