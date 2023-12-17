/*************************************************************************/
/*  paged_articles_md_index_web_page.cpp                                 */
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
