/*
Copyright (c) 2022 Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "web_node_editor.h"

#include "../http/web_node.h"
#include "../http/web_server_cookie.h"
#include "scene/gui/rich_text_label.h"
#include "scene/gui/text_edit.h"

#include "../html/html_parser.h"
#include "web_node_editor_web_server.h"
#include "web_node_editor_web_server_request.h"

void WebEditor::edit(WebNode *web_node) {
	_edited_node = web_node;

	refresh();
}

void WebEditor::refresh() {
	if (_edited_node && !ObjectDB::instance_validate(_edited_node)) {
		_edited_node = nullptr;
	}

	clear();

	if (_edited_node == nullptr) {
		//Add "Select a node" text
		return;
	}

	Ref<WebEditorWebServerRequest> request;
	request.instance();

	_web_server->web_editor_request(_edited_node, request);

	String request_info;

	request_info += "Response type: ";
	switch (request->_response_type) {
		case WebEditorWebServerRequest::RESPONSE_TYPE_NONE: {
			request_info += "NONE";
		} break;
		case WebEditorWebServerRequest::RESPONSE_TYPE_NORMAL: {
			request_info += "NORMAL";
		} break;
		case WebEditorWebServerRequest::RESPONSE_TYPE_FILE: {
			request_info += "FILE";
		} break;
		case WebEditorWebServerRequest::RESPONSE_TYPE_REDIRECT: {
			request_info += "REDIRECT";
		} break;
		default: {
			request_info += "ERROR";
		} break;
	}
	request_info += ". ";

	request_info += "Status code: ";
	request_info += itos(static_cast<int>(request->get_status_code()));
	request_info += ". ";

	request_info += "Error handler was called: ";
	if (request->_error_handler_called) {
		request_info += "YES";
	} else {
		request_info += "NO";
	}
	request_info += ".";

	_result_info_label->set_bbcode(request_info);

	String body;

	int cookie_count = request->response_get_cookie_count();

	if (cookie_count > 0) {
		body += "====================\n";

		body += "Cookies set:\n";

		for (int i = 0; i < cookie_count; ++i) {
			Ref<WebServerCookie> cookie = request->response_get_cookie(i);

			ERR_CONTINUE(!cookie.is_valid());

			String cookie_str = cookie->get_response_header_string();

			if (cookie_str != "") {
				body += cookie_str;
			}
		}

		body += "====================\n\n";
	}

	switch (request->_response_type) {
		case WebEditorWebServerRequest::RESPONSE_TYPE_NONE: {
			body += request->_sent_message;
		} break;
		case WebEditorWebServerRequest::RESPONSE_TYPE_NORMAL: {
			if (_prettify_html) {
				HTMLParser p;
				p.parse(request->_sent_message);
				body += p.convert_to_string();
			} else {
				body += request->_sent_message;
			}
		} break;
		case WebEditorWebServerRequest::RESPONSE_TYPE_FILE: {
			body += "Sent file:\n";
			body += request->_sent_message;
		} break;
		case WebEditorWebServerRequest::RESPONSE_TYPE_REDIRECT: {
			if (_prettify_html) {
				HTMLParser p;
				p.parse(request->_sent_message);
				body += p.convert_to_string();
			} else {
				body += request->_sent_message;
			}
		} break;
		default: {
			body += request->_sent_message;
		} break;
	}

	_results_label->set_text(body);
}

void WebEditor::clear() {
	_result_info_label->clear();
	_results_label->clear();
}

void WebEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
		} break;
		case NOTIFICATION_VISIBILITY_CHANGED: {
			if (!is_visible_in_tree()) {
				return;
			}

			refresh();
		} break;
		case NOTIFICATION_POSTINITIALIZE: {
		} break;
		default: {
		} break;
	}
}

WebEditor::WebEditor() {
	_prettify_html = true;

	_web_server = memnew(WebEditorWebServer);
	add_child(_web_server);

	set_h_size_flags(SIZE_EXPAND_FILL);
	set_v_size_flags(SIZE_EXPAND_FILL);
	set_anchors_and_margins_preset(Control::PRESET_WIDE);

	_toolbar = memnew(HBoxContainer);
	_toolbar->set_h_size_flags(SIZE_EXPAND_FILL);
	//TODO
	add_child(_toolbar);

	_result_info_label = memnew(RichTextLabel);
	_result_info_label->set_h_size_flags(SIZE_EXPAND_FILL);
	_result_info_label->set_fit_content_height(true);
	_result_info_label->set_scroll_active(false);
	_result_info_label->set_selection_enabled(true);
	_result_info_label->set_use_bbcode(true);
	add_child(_result_info_label);

	_results_label = memnew(TextEdit);
	_results_label->set_h_size_flags(SIZE_EXPAND_FILL);
	_results_label->set_v_size_flags(SIZE_EXPAND_FILL);
	_results_label->set_readonly(true);
	_results_label->set_highlight_current_line(true);
	_results_label->set_syntax_coloring(true);
	_results_label->set_show_line_length_guidelines(true);
	_results_label->set_draw_fold_gutter(true);
	_results_label->set_highlight_all_occurrences(true);
	_results_label->set_draw_minimap(true);
	_results_label->set_right_click_moves_caret(false);

	add_child(_results_label);

	//todo add all
	_results_label->add_color_region("<b>", "</b>", Color::color8(153, 153, 255, 255), false);
	_results_label->add_color_region("<i>", "</i>", Color::color8(153, 255, 153, 255), false);
	_results_label->add_color_region("<del>", "</del>", Color::color8(255, 153, 153, 255), false);
	_results_label->add_color_region("<ins>", "</ins>", Color::color8(255, 255, 102, 255), false);
	_results_label->add_color_region("<a", "</a>", Color::color8(153, 204, 255, 255), false);
	_results_label->add_color_region("<img", "/>", Color::color8(255, 204, 153, 255), true);
	_results_label->add_color_region("<pre>", "</pre>", Color::color8(192, 192, 192, 255), false);
	_results_label->add_color_region("<center>", "</center>", Color::color8(175, 238, 238, 255), false);
	_results_label->add_color_region("<right>", "</right>", Color::color8(135, 206, 235, 255), false);
}

WebEditor::~WebEditor() {
}

void WebEditor::_bind_methods() {
	//ClassDB::bind_method(D_METHOD("_input", "event"), &WebEditor::_input);
}
