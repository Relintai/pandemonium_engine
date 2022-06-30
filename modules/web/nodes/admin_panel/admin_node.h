#ifndef ADMIN_NODE_H
#define ADMIN_NODE_H

#include "web/http/web_node.h"

#include "core/containers/vector.h"
#include "core/string.h"

class Request;
class FormValidator;

struct AdminSectionLinkInfo {
	String name;
	String link;

	AdminSectionLinkInfo() {
	}

	AdminSectionLinkInfo(const String &p_name, const String &p_link) {
		name = p_name;
		link = p_link;
	}
};

class AdminNode : public WebNode {
	RCPP_OBJECT(AdminNode, WebNode);

public:
	virtual void admin_handle_request_main(Request *request);
	virtual String admin_get_section_name();
	virtual void admin_add_section_links(Vector<AdminSectionLinkInfo> *links);
	virtual bool admin_full_render();

	AdminNode();
	~AdminNode();

protected:
};

#endif