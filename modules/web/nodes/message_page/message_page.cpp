/*************************************************************************/
/*  message_page.cpp                                                     */
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

#include "message_page.h"

#include "database/database.h"

#include "database/query_builder.h"
#include "database/query_result.h"
#include "database/table_builder.h"
#include "web/http/web_permission.h"

void MessagePage::handle_request_main(Request *request) {
	if (_web_permission.is_valid()) {
		if (_web_permission->activate(request)) {
			return;
		}
	}

	Ref<QueryBuilder> b = db->get_query_builder();

	b->select("text")->from("message_page")->end_command();

	Ref<QueryResult> res = db->query(b->query_result);

	Vector<String> msgs;

	while (res->next_row()) {
		msgs.push_back(res->get_cell(0));
	}

	String r = "<html><body>";

	for (uint32_t i = 0; i < messages.size(); ++i) {
		r += "<p>" + messages[i] + "</p><br>";
	}

	for (uint32_t i = 0; i < msgs.size(); ++i) {
		r += "<p>" + msgs[i] + "</p><br>";
	}

	r += "</html></body>";

	request->body += r;

	request->compile_and_send_body();
}

void MessagePage::_migrate(const bool clear, const bool seed_db) {
	Ref<TableBuilder> t = db->get_table_builder();

	t->drop_table("message_page");
	db->query_run(t->result);

	//printf("%s\n", t->result.utf8().get_data());

	t->result.clear();

	t->create_table("message_page")->integer("id")->auto_increment()->primary_key()->next_row()->varchar("text", 30)->ccreate_table();

	//printf("%s\n", t->result.c_str());

	db->query(t->result);

	Ref<QueryBuilder> b = db->get_query_builder();

	b->insert("message_page")->values("default, 'aaewdwd'");

	//printf("%s\n", b->query_result.c_str());

	db->query_run(b->query_result);

	b->query_result.clear();
	b->insert("message_page")->values("default, 'qqqqq'");

	//printf("%s\n", b->query_result.c_str());

	db->query_run(b->query_result);
}

MessagePage::MessagePage() :
		WebNode() {
	messages.push_back("T message 1");
	messages.push_back("T message 2");
}

MessagePage::~MessagePage() {
}