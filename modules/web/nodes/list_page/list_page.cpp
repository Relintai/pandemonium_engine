#include "list_page.h"

#include "core/error_macros.h"
#include "core/os/dir_access.h"
#include "core/os/file_access.h"

#include "../../html/html_builder.h"
#include "../../html/paginator.h"
#include "../../http/http_server_enums.h"
#include "../../http/web_permission.h"
#include "../../http/web_server_request.h"

bool ListPage::get_paginate() {
	return paginate;
}
void ListPage::set_paginate(const bool &val) {
	paginate = val;
}

int ListPage::get_max_visible_navigation_links() {
	return max_visible_navigation_links;
}
void ListPage::set_max_visible_navigation_links(const int &val) {
	max_visible_navigation_links = val;
}

int ListPage::get_entry_per_page() {
	return entry_per_page;
}
void ListPage::set_entry_per_page(const int &val) {
	entry_per_page = val;
}

String ListPage::get_folder() {
	return folder;
}
void ListPage::set_folder(const String &val) {
	folder = val;
}

String ListPage::get_main_div_class() {
	return main_div_class;
}
void ListPage::set_main_div_class(const String &val) {
	main_div_class = val;
}

String ListPage::get_entry_div_class() {
	return entry_div_class;
}
void ListPage::set_entry_div_class(const String &val) {
	entry_div_class = val;
}

String ListPage::get_empty_div_class() {
	return empty_div_class;
}
void ListPage::set_empty_div_class(const String &val) {
	empty_div_class = val;
}

String ListPage::get_placeholder_text() {
	return placeholder_text;
}
void ListPage::set_placeholder_text(const String &val) {
	placeholder_text = val;
}

void ListPage::_handle_request_main(Ref<WebServerRequest> request) {
	if (_web_permission.is_valid()) {
		if (_web_permission->activate(request)) {
			return;
		}
	}

	if (_pages.size() == 0) {
		render_menu(request);
		request->body += _no_entries_response;
		request->compile_and_send_body();
		return;
	}

	String cs = request->get_current_path_segment();

	if (cs == "") {
		render_menu(request);
		render_index(request);
		request->compile_and_send_body();
		return;
	}

	if (!cs.is_valid_unsigned_integer()) {
		request->send_error(HTTPServerEnums::HTTP_STATUS_CODE_404_NOT_FOUND);
		return;
	}

	int p = cs.to_int();

	p = ((p == 0) ? (0) : (p - 1));

	if (p < 0 || p >= _pages.size()) {
		request->send_error(HTTPServerEnums::HTTP_STATUS_CODE_404_NOT_FOUND);
		return;
	}

	render_menu(request);
	request->body += _pages[p];
	request->compile_and_send_body();
}

void ListPage::_render_index(Ref<WebServerRequest> request) {
	request->body += _pages[0];
}
void ListPage::_render_preview(Ref<WebServerRequest> request) {
}

void ListPage::load() {
	ERR_FAIL_COND_MSG(folder == "", "Error: ListPage::load called, but a folder is not set!");

	Vector<String> files;

	DirAccess *dir = DirAccess::open(folder);

	ERR_FAIL_COND_MSG(!dir, "Error opening ListPage::folder! folder: \n" + folder);

	dir->list_dir_begin();

	String file = dir->get_next();

	while (file != "") {
		if (!dir->current_is_dir()) {
			files.push_back(folder.append_path(file));
		}

		file = dir->get_next();
	}

	dir->list_dir_end();
	memdelete(dir);
	dir = nullptr;

	files.sort();

	Vector<String> list_entries;

	for (int i = 0; i < files.size(); ++i) {
		FileAccess *f = FileAccess::open(files[i], FileAccess::READ);

		String fd = f->get_as_utf8_string();
		f->close();

		memdelete(f);

		//Utils::markdown_to_html(&fd);

		list_entries.push_back(fd);
	}

	render_entries(list_entries);
	render_no_entries_response();
}

void ListPage::render_entries(const Vector<String> &list_entries) {
	if (list_entries.size() == 0) {
		return;
	}

	int pages = Math::floorf_int(Math::divf(list_entries.size(), entry_per_page));
	for (int i = 0; i < pages; ++i) {
		String r = "";

		int efrom = i * entry_per_page;
		int eto = MIN((i + 1) * entry_per_page, list_entries.size());

		r = render_page(i, pages, list_entries, efrom, eto);
		_pages.push_back(r);
	}
}

String ListPage::render_page(const int page_index, const int page_count, const Vector<String> &list_entries, const int efrom, const int eto) {
	HTMLBuilder b;

	b.div(main_div_class);

	for (int i = efrom; i < eto; ++i) {
		b.w(render_entry(list_entries[i]));
	}

	b.w(HTMLPaginator::get_pagination_old(get_full_uri(), page_count, page_index, max_visible_navigation_links));
	b.cdiv();

	return b.result;
}

String ListPage::render_entry(const String &list_entry) {
	HTMLBuilder b;

	b.div(main_div_class);
	b.div(empty_div_class)->w(list_entry)->cdiv();
	b.cdiv();

	return b.result;
}

void ListPage::render_no_entries_response() {
	HTMLBuilder b;

	b.div(empty_div_class)->w(placeholder_text)->cdiv();

	_no_entries_response = b.result;
}

void ListPage::_notification(const int what) {
	switch (what) {
		case NOTIFICATION_ENTER_TREE:
			load();
			break;
		case NOTIFICATION_EXIT_TREE:
			_pages.clear();
			break;
		default:
			break;
	}
}

ListPage::ListPage() :
		WebNode() {
	max_visible_navigation_links = 6;
	entry_per_page = 4;
	main_div_class = "list_page";
	entry_div_class = "list_entry";
	empty_div_class = "list_entry_empty";
	placeholder_text = "No content yet!";
}

ListPage::~ListPage() {
}

void ListPage::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_paginate"), &ListPage::get_paginate);
	ClassDB::bind_method(D_METHOD("set_paginate", "val"), &ListPage::set_paginate);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "paginate"), "set_paginate", "get_paginate");

	ClassDB::bind_method(D_METHOD("get_max_visible_navigation_links"), &ListPage::get_max_visible_navigation_links);
	ClassDB::bind_method(D_METHOD("set_max_visible_navigation_links", "val"), &ListPage::set_max_visible_navigation_links);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_visible_navigation_links"), "set_max_visible_navigation_links", "get_max_visible_navigation_links");

	ClassDB::bind_method(D_METHOD("get_entry_per_page"), &ListPage::get_entry_per_page);
	ClassDB::bind_method(D_METHOD("set_entry_per_page", "val"), &ListPage::set_entry_per_page);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "entry_per_page"), "set_entry_per_page", "get_entry_per_page");

	ClassDB::bind_method(D_METHOD("get_folder"), &ListPage::get_folder);
	ClassDB::bind_method(D_METHOD("set_folder", "val"), &ListPage::set_folder);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "folder"), "set_folder", "get_folder");

	ClassDB::bind_method(D_METHOD("get_main_div_class"), &ListPage::get_main_div_class);
	ClassDB::bind_method(D_METHOD("set_main_div_class", "val"), &ListPage::set_main_div_class);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "main_div_class"), "set_main_div_class", "get_main_div_class");

	ClassDB::bind_method(D_METHOD("get_entry_div_class"), &ListPage::get_entry_div_class);
	ClassDB::bind_method(D_METHOD("set_entry_div_class", "val"), &ListPage::set_entry_div_class);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "entry_div_class"), "set_entry_div_class", "get_entry_div_class");

	ClassDB::bind_method(D_METHOD("get_empty_div_class"), &ListPage::get_empty_div_class);
	ClassDB::bind_method(D_METHOD("set_empty_div_class", "val"), &ListPage::set_empty_div_class);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "empty_div_class"), "set_empty_div_class", "get_empty_div_class");

	ClassDB::bind_method(D_METHOD("get_placeholder_text"), &ListPage::get_placeholder_text);
	ClassDB::bind_method(D_METHOD("set_placeholder_text", "val"), &ListPage::set_placeholder_text);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "placeholder_text"), "set_placeholder_text", "get_placeholder_text");

	ClassDB::bind_method(D_METHOD("load"), &ListPage::load);
}
