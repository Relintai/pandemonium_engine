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
#include "scene/gui/button.h"
#include "scene/gui/margin_container.h"
#include "scene/gui/rich_text_label.h"
#include "scene/gui/text_edit.h"

#include "../html/html_parser.h"
#include "web_node_editor_web_server.h"
#include "web_node_editor_web_server_request.h"

void WebNodeEditor::add_control_to_tool_bar(Control *control) {
	_toolbar->add_child(control);
}
void WebNodeEditor::remove_control_from_tool_bar(Control *control) {
	_toolbar->remove_child(control);
}

void WebNodeEditor::add_main_screen_tab(Control *control) {
	_main_container->add_child(control);
}
void WebNodeEditor::remove_main_screen_tab(Control *control) {
	_main_container->remove_child(control);
}
void WebNodeEditor::switch_to_main_screen_tab(Control *control) {
	int cc = _main_container->get_child_count();
	for (int i = 0; i < cc; ++i) {
		Control *c = Object::cast_to<Control>(_main_container->get_child(i));

		if (!c) {
			continue;
		}

		if (c == control) {
			c->set_visible(true);
		} else {
			c->set_visible(false);
		}
	}
}

Ref<ButtonGroup> WebNodeEditor::get_main_button_group() {
	return _main_button_group;
}

void WebNodeEditor::edit(WebNode *web_node) {
	_edited_node = web_node;

	refresh_html_preview();

	emit_signal("edited_node_changed", _edited_node);
}

void WebNodeEditor::refresh_html_preview() {
	if (_edited_node && !ObjectDB::instance_validate(_edited_node)) {
		_edited_node = nullptr;
	}

	clear();

	if (!_html_previewer->is_visible_in_tree()) {
		return;
	}

	if (_edited_node == nullptr) {
		_result_info_label->set_bbcode("");
		_results_label->set_text("Select a WebNode to view it's output.");
		return;
	}

	Ref<WebNodeEditorWebServerRequest> request;
	request.instance();

	_web_server->web_editor_request(_edited_node, request);

	String request_info;

	request_info += "Response type: ";
	switch (request->_response_type) {
		case WebNodeEditorWebServerRequest::RESPONSE_TYPE_NONE: {
			request_info += "NONE";
		} break;
		case WebNodeEditorWebServerRequest::RESPONSE_TYPE_NORMAL: {
			request_info += "NORMAL";
		} break;
		case WebNodeEditorWebServerRequest::RESPONSE_TYPE_FILE: {
			request_info += "FILE";
		} break;
		case WebNodeEditorWebServerRequest::RESPONSE_TYPE_REDIRECT: {
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
		case WebNodeEditorWebServerRequest::RESPONSE_TYPE_NONE: {
			body += request->_sent_message;
		} break;
		case WebNodeEditorWebServerRequest::RESPONSE_TYPE_NORMAL: {
			if (_prettify_html) {
				HTMLParser p;
				p.parse(request->_sent_message);
				body += p.convert_to_string();
			} else {
				body += request->_sent_message;
			}
		} break;
		case WebNodeEditorWebServerRequest::RESPONSE_TYPE_FILE: {
			body += "Sent file:\n";
			body += request->_sent_message;
		} break;
		case WebNodeEditorWebServerRequest::RESPONSE_TYPE_REDIRECT: {
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

void WebNodeEditor::clear() {
	_result_info_label->clear();
	_results_label->clear();
}

void WebNodeEditor::_on_html_previewer_tool_button_toggled(bool on) {
	if (on) {
		switch_to_main_screen_tab(_html_previewer);
	}
}

void WebNodeEditor::_on_html_previewer_visibility_changed() {
	if (_html_previewer->is_visible_in_tree()) {
		refresh_html_preview();
	}
}

void WebNodeEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
		} break;
		case NOTIFICATION_VISIBILITY_CHANGED: {
			if (!is_visible_in_tree()) {
				return;
			}

			refresh_html_preview();
		} break;
		case NOTIFICATION_POSTINITIALIZE: {
		} break;
		default: {
		} break;
	}
}

WebNodeEditor::WebNodeEditor() {
	_singleton = this;

	_prettify_html = true;

	_web_server = memnew(WebNodeEditorWebServer);
	add_child(_web_server);

	set_h_size_flags(SIZE_EXPAND_FILL);
	set_v_size_flags(SIZE_EXPAND_FILL);
	set_anchors_and_margins_preset(Control::PRESET_WIDE);

	_toolbar = memnew(HBoxContainer);
	_toolbar->set_h_size_flags(SIZE_EXPAND_FILL);
	add_child(_toolbar);

	_main_container = memnew(MarginContainer);
	_main_container->set_h_size_flags(SIZE_EXPAND_FILL);
	_main_container->set_v_size_flags(SIZE_EXPAND_FILL);
	add_child(_main_container);

	_html_previewer = memnew(VBoxContainer);
	_html_previewer->set_h_size_flags(SIZE_EXPAND_FILL);
	_html_previewer->set_v_size_flags(SIZE_EXPAND_FILL);
	_main_container->add_child(_html_previewer);

	_result_info_label = memnew(RichTextLabel);
	_result_info_label->set_h_size_flags(SIZE_EXPAND_FILL);
	_result_info_label->set_fit_content_height(true);
	_result_info_label->set_scroll_active(false);
	_result_info_label->set_selection_enabled(true);
	_result_info_label->set_use_bbcode(true);
	_html_previewer->add_child(_result_info_label);

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
	_html_previewer->add_child(_results_label);

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

	_main_button_group.instance();

	_html_previewer_tool_button = memnew(Button);
	_html_previewer_tool_button->set_text("HTML");
	_html_previewer_tool_button->set_tooltip("HTML preview");
	_html_previewer_tool_button->set_toggle_mode(true);
	_html_previewer_tool_button->set_pressed(true);
	_html_previewer_tool_button->set_button_group(_main_button_group);
	_html_previewer_tool_button->set_keep_pressed_outside(true);
	_html_previewer_tool_button->connect("toggled", this, "_on_html_previewer_tool_button_toggled");
	_toolbar->add_child(_html_previewer_tool_button);
}

WebNodeEditor::~WebNodeEditor() {
	_singleton = NULL;
}

void WebNodeEditor::_bind_methods() {
	ADD_SIGNAL(MethodInfo("edited_node_changed", PropertyInfo(Variant::OBJECT, "web_node", PROPERTY_HINT_RESOURCE_TYPE, "WebNode")));

	ClassDB::bind_method(D_METHOD("add_control_to_tool_bar", "control"), &WebNodeEditor::add_control_to_tool_bar);
	ClassDB::bind_method(D_METHOD("remove_control_from_tool_bar", "control"), &WebNodeEditor::remove_control_from_tool_bar);

	ClassDB::bind_method(D_METHOD("add_main_screen_tab", "control"), &WebNodeEditor::add_main_screen_tab);
	ClassDB::bind_method(D_METHOD("remove_main_screen_tab", "control"), &WebNodeEditor::remove_main_screen_tab);
	ClassDB::bind_method(D_METHOD("switch_to_main_screen_tab", "control"), &WebNodeEditor::switch_to_main_screen_tab);

	ClassDB::bind_method(D_METHOD("get_main_button_group"), &WebNodeEditor::get_main_button_group);

	ClassDB::bind_method(D_METHOD("_on_html_previewer_tool_button_toggled", "on"), &WebNodeEditor::_on_html_previewer_tool_button_toggled);
	ClassDB::bind_method(D_METHOD("_on_html_previewer_visibility_changed"), &WebNodeEditor::_on_html_previewer_visibility_changed);
}

WebNodeEditor* WebNodeEditor::_singleton = NULL;