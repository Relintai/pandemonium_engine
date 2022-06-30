#include "admin_panel.h"

#include "web/html/form_validator.h"
#include "web/html/html_builder.h"
#include "web/http/cookie.h"
#include "web/http/http_session.h"
#include "web/http/request.h"
#include "web/http/session_manager.h"
#include "web/http/web_permission.h"

#include "admin_node.h"

void AdminPanel::handle_request_main(Request *request) {
	if (_web_permission.is_valid()) {
		if (_web_permission->activate(request)) {
			return;
		}
	}

	String seg = request->get_current_path_segment();

	if (seg == "") {
		render_admin_panel_list(request);
		return;
	}

	for (int i = 0; i < _controllers.size(); ++i) {
		AdminPanelSection &s = _controllers[i];

		if (s.section_url == seg) {
			AdminNode *c = s.controller;

			request->push_path();

			if (c->admin_full_render()) {
				c->admin_handle_request_main(request);
				return;
			}

			// render_menu(request);
			render_headers(request);
			render_segment_body_top(request);
			render_controller_panel(request, c);
			render_footer(request);

			// request->pop_path();

			return;
		}
	}

	request->send_error(404);
}

void AdminPanel::render_admin_panel_list(Request *request) {
	render_headers(request);
	render_main_body_top(request);

	String rootlink = request->get_url_root();

	HTMLBuilder b;

	Vector<AdminSectionLinkInfo> links;

	b.div()->cls("content");

	for (int i = 0; i < _controllers.size(); ++i) {
		b.div()->cls("section");

		AdminPanelSection &s = _controllers[i];

		b.div()->cls("section_header");
		b.w(s.controller->admin_get_section_name());
		b.cdiv();

		s.controller->admin_add_section_links(&links);

		for (int j = 0; j < links.size(); ++j) {
			AdminSectionLinkInfo &li = links[j];

			b.div()->cls("section_entry");
			b.a()->href(rootlink + s.section_url + "/" + li.link);
			b.w(li.name);
			b.ca();
			b.cdiv();
		}

		links.clear();

		b.cdiv();
	}

	b.cdiv();

	render_footer(request);

	request->body += b.result;
	request->compile_and_send_body();
}

void AdminPanel::render_controller_panel(Request *request, AdminNode *controller) {
	// set up headers
	controller->admin_handle_request_main(request);
	// set up footers
	request->compile_and_send_body();
}

void AdminPanel::register_admin_controller(const String &section, AdminNode *controller) {
	AdminPanelSection sec;

	sec.section_url = section;
	sec.controller = controller;

	_controllers.push_back(sec);
}

void AdminPanel::clear() {
	_controllers.clear();
}

void AdminPanel::render_headers(Request *request) {
	request->head += _default_headers;
}

void AdminPanel::render_main_body_top(Request *request) {
	request->body += _default_main_body_top;
}

void AdminPanel::render_segment_body_top(Request *request) {
	request->body += _default_segment_body_top;
}

void AdminPanel::render_footer(Request *request) {
	request->body += _default_footer;
}

void AdminPanel::set_default_header(const String &val) {
	_default_headers = val;
}
void AdminPanel::set_default_main_body_top(const String &val) {
	_default_main_body_top = val;
}
void AdminPanel::set_default_segment_body_top(const String &val) {
	_default_segment_body_top = val;
}

void AdminPanel::set_default_footer(const String &val) {
	_default_footer = val;
}

AdminPanel *AdminPanel::get_singleton() {
	return _self;
}

AdminPanel::AdminPanel() :
		WebNode() {

	if (_self) {
		printf("AdminPanel::AdminPanel(): Error! self is not null!/n");
	}

	_self = this;
}

AdminPanel::~AdminPanel() {
	if (_self == this) {
		_self = nullptr;
	}
}

AdminPanel *AdminPanel::_self = nullptr;
