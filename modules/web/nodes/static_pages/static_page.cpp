#include "static_page.h"

#include "../../http/web_server_request.h"
#include "core/os/file_access.h"

String StaticPage::get_data() {
	return _data;
}
void StaticPage::set_data(const String &val) {
	_data = val;
}

String StaticPage::get_preview_data() {
	return _preview_data;
}
void StaticPage::set_preview_data(const String &val) {
	_preview_data = val;
}

bool StaticPage::get_should_render_menu() {
	return _should_render_menu;
}
void StaticPage::set_should_render_menu(const bool &val) {
	_should_render_menu = val;
}

void StaticPage::_handle_request(Ref<WebServerRequest> request) {
	if (_should_render_menu) {
		render_main_menu(request);
	}

	render_index(request);
	request->compile_and_send_body();
}

void StaticPage::_render_index(Ref<WebServerRequest> request) {
	request->body += _data;
}

void StaticPage::_render_preview(Ref<WebServerRequest> request) {
	request->body += _preview_data;
}

void StaticPage::load_file(const String &path) {
	FileAccess *f = FileAccess::open(path, FileAccess::READ);

	if (f) {
		_data = f->get_as_utf8_string();
		f->close();
		memdelete(f);
	} else {
		_data = "";
	}
}

void StaticPage::load_and_process_file(const String &path) {
	FileAccess *f = FileAccess::open(path, FileAccess::READ);

	if (f) {
		_data = f->get_as_utf8_string();
		f->close();
		memdelete(f);
	} else {
		_data = "";
	}

	//todo prcess
	//could support bbcode easily
	//should probably support md -> I should probably write a parser
}

StaticPage::StaticPage() {
	_should_render_menu = true;
}

StaticPage::~StaticPage() {
}

void StaticPage::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_data"), &StaticPage::get_data);
	ClassDB::bind_method(D_METHOD("set_data", "val"), &StaticPage::set_data);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "data", PROPERTY_HINT_MULTILINE_TEXT), "set_data", "get_data");

	ClassDB::bind_method(D_METHOD("get_preview_data"), &StaticPage::get_preview_data);
	ClassDB::bind_method(D_METHOD("set_preview_data", "val"), &StaticPage::set_preview_data);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "preview_data", PROPERTY_HINT_MULTILINE_TEXT), "set_preview_data", "get_preview_data");

	ClassDB::bind_method(D_METHOD("get_should_render_menu"), &StaticPage::get_should_render_menu);
	ClassDB::bind_method(D_METHOD("set_should_render_menu", "val"), &StaticPage::set_should_render_menu);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "should_render_menu"), "set_should_render_menu", "get_should_render_menu");

	ClassDB::bind_method(D_METHOD("load_file", "path"), &StaticPage::load_file);
	ClassDB::bind_method(D_METHOD("load_and_process_file", "path"), &StaticPage::load_and_process_file);
}
