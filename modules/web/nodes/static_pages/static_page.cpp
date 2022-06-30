#include "static_page.h"

#include "core/os/directory.h"
#include "web/file_cache.h"
#include "web/html/html_builder.h"
#include "web/html/utils.h"
#include "web/http/request.h"

void StaticPage::_handle_request_main(Request *request) {
	if (should_render_menu) {
		render_menu(request);
	}

	render_index(request);
	request->compile_and_send_body();
}

void StaticPage::render_index(Request *request) {
	request->body += data;
}

void StaticPage::render_preview(Request *request) {
	request->body += preview_data;
}

void StaticPage::load_file(const String &path) {
	Ref<Directory> d;
	d.instance();

	d->read_file_into(path, &data);
}

void StaticPage::load_and_process_file(const String &path) {
	Ref<Directory> d;
	d.instance();

	d->read_file_into(path, &data);

	if (path.file_get_extension() == "md") {
		Utils::markdown_to_html(&data);
	}
}

void StaticPage::load_md_file(const String &path) {
	Ref<Directory> d;
	d.instance();

	d->read_file_into(path, &data);
	Utils::markdown_to_html(&data);
}

void StaticPage::set_data_md(const String &d) {
	data.clear();

	Utils::markdown_to_html(&data);
}

void StaticPage::set_data(const String &d) {
	data = d;
}

void StaticPage::set_preview_data(const String &d) {
	preview_data = d;
}

StaticPage::StaticPage() :
		WebNode() {

	should_render_menu = true;
}

StaticPage::~StaticPage() {
}
