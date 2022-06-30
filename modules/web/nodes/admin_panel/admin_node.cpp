#include "admin_node.h"

#include "web/html/form_validator.h"
#include "web/html/html_builder.h"
#include "web/http/cookie.h"
#include "web/http/http_session.h"
#include "web/http/request.h"
#include "web/http/session_manager.h"

void AdminNode::admin_handle_request_main(Request *request) {

}

String AdminNode::admin_get_section_name() {
	return "";
}

void AdminNode::admin_add_section_links(Vector<AdminSectionLinkInfo> *links) {
	
}

bool AdminNode::admin_full_render() {
	return false;
}

AdminNode::AdminNode() :
		WebNode() {

}

AdminNode::~AdminNode() {
}
