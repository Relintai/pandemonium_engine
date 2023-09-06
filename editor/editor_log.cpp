/*************************************************************************/
/*  editor_log.cpp                                                       */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
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

#include "editor_log.h"

#include "core/math/color.h"
#include "core/math/vector2.h"
#include "core/object/class_db.h"
#include "core/object/reference.h"
#include "core/object/undo_redo.h"
#include "core/os/keyboard.h"
#include "core/os/memory.h"
#include "core/os/os.h"
#include "core/version.h"
#include "editor/editor_settings.h"
#include "editor_node.h"
#include "editor_scale.h"
#include "scene/gui/button.h"
#include "scene/gui/control.h"
#include "scene/gui/label.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/rich_text_label.h"
#include "scene/gui/separator.h"
#include "scene/gui/tool_button.h"
#include "scene/main/node.h"
#include "scene/resources/dynamic_font.h"
#include "scene/resources/texture.h"

void EditorLog::LogFilter::initialize_button(const String &p_tooltip, Object *p_cb_obj, const StringName &p_method) {
	toggle_button = memnew(Button);
	toggle_button->set_toggle_mode(true);
	toggle_button->set_pressed(true);
	toggle_button->set_text(itos(message_count));
	toggle_button->set_tooltip(TTR(p_tooltip));
	// Don't tint the icon even when in "pressed" state.
	toggle_button->add_theme_color_override("icon_color_pressed", Color(1, 1, 1, 1));
	toggle_button->set_focus_mode(FOCUS_NONE);
	// When toggled call the callback and pass the MessageType this button is for.
	toggle_button->connect("toggled", p_cb_obj, p_method, varray(type));
}

int EditorLog::LogFilter::get_message_count() {
	return message_count;
}

void EditorLog::LogFilter::set_message_count(int p_count) {
	message_count = p_count;
	toggle_button->set_text(itos(message_count));
}

bool EditorLog::LogFilter::is_active() {
	return active;
}

void EditorLog::LogFilter::set_active(bool p_active) {
	toggle_button->set_pressed(p_active);
	active = p_active;
}

EditorLog::LogFilter::LogFilter(MessageType p_type) {
	message_count = 0;
	active = true;

	type = p_type;
	toggle_button = NULL;
}

void EditorLog::_error_handler(void *p_self, const char *p_func, const char *p_file, int p_line, const char *p_error, const char *p_errorexp, ErrorHandlerType p_type) {
	EditorLog *self = (EditorLog *)p_self;

	String err_str;

	if (p_errorexp && p_errorexp[0]) {
		err_str = String::utf8(p_errorexp);
	} else {
		err_str = String::utf8(p_file) + ":" + itos(p_line) + " - " + String::utf8(p_error);
	}

	MessageType message_type = p_type == ERR_HANDLER_WARNING ? MSG_TYPE_WARNING : MSG_TYPE_ERROR;

	if (self->current != Thread::get_caller_id()) {
		self->call_deferred("add_thread_message", Thread::get_caller_id(), err_str, message_type);
	} else {
		self->add_message(err_str, message_type);
	}
}

void EditorLog::_update_theme() {
	const Ref<Font> normal_font = get_theme_font(("output_source"), ("EditorFonts"));
	if (normal_font.is_valid()) {
		log->add_theme_font_override("normal_font", normal_font);
	}

	const Ref<Font> bold_font = get_theme_font(("output_source_bold"), ("EditorFonts"));
	if (bold_font.is_valid()) {
		log->add_theme_font_override("bold_font", bold_font);
	}

	const Ref<Font> italics_font = get_theme_font(("output_source_italic"), ("EditorFonts"));
	if (italics_font.is_valid()) {
		log->add_theme_font_override("italics_font", italics_font);
	}

	const Ref<Font> bold_italics_font = get_theme_font(("output_source_bold_italic"), ("EditorFonts"));
	if (bold_italics_font.is_valid()) {
		log->add_theme_font_override("bold_italics_font", bold_italics_font);
	}

	const Ref<Font> mono_font = get_theme_font(("output_source_mono"), ("EditorFonts"));
	if (mono_font.is_valid()) {
		log->add_theme_font_override("mono_font", mono_font);
	}

	// Disable padding for highlighted background/foreground to prevent highlights from overlapping on close lines.
	// This also better matches terminal output, which does not use any form of padding.
	log->add_theme_constant_override("text_highlight_h_padding", 0);
	log->add_theme_constant_override("text_highlight_v_padding", 0);

	//const int font_size = get_theme_font_size(("output_source_size"), ("EditorFonts"));
	//log->add_theme_font_size_override("normal_font_size", font_size);
	//log->add_theme_font_size_override("bold_font_size", font_size);
	//log->add_theme_font_size_override("italics_font_size", font_size);
	//log->add_theme_font_size_override("mono_font_size", font_size);

	type_filter_map[MSG_TYPE_STD]->toggle_button->set_icon(get_theme_icon(("Popup"), ("EditorIcons")));
	type_filter_map[MSG_TYPE_ERROR]->toggle_button->set_icon(get_theme_icon(("StatusError"), ("EditorIcons")));
	type_filter_map[MSG_TYPE_WARNING]->toggle_button->set_icon(get_theme_icon(("StatusWarning"), ("EditorIcons")));
	type_filter_map[MSG_TYPE_EDITOR]->toggle_button->set_icon(get_theme_icon(("Edit"), ("EditorIcons")));

	type_filter_map[MSG_TYPE_STD]->toggle_button->set_theme_type_variation("EditorLogFilterButton");
	type_filter_map[MSG_TYPE_ERROR]->toggle_button->set_theme_type_variation("EditorLogFilterButton");
	type_filter_map[MSG_TYPE_WARNING]->toggle_button->set_theme_type_variation("EditorLogFilterButton");
	type_filter_map[MSG_TYPE_EDITOR]->toggle_button->set_theme_type_variation("EditorLogFilterButton");

	clear_button->set_icon(get_theme_icon(("Clear"), ("EditorIcons")));
	copy_button->set_icon(get_theme_icon(("ActionCopy"), ("EditorIcons")));
	collapse_button->set_icon(get_theme_icon(("CombineLines"), ("EditorIcons")));
	show_search_button->set_icon(get_theme_icon(("Search"), ("EditorIcons")));
	search_box->set_right_icon(get_theme_icon(("Search"), ("EditorIcons")));

	theme_cache.error_color = get_theme_color(("error_color"), ("Editor"));
	theme_cache.error_icon = get_theme_icon(("Error"), ("EditorIcons"));
	theme_cache.warning_color = get_theme_color(("warning_color"), ("Editor"));
	theme_cache.warning_icon = get_theme_icon(("Warning"), ("EditorIcons"));
	theme_cache.message_color = get_theme_color(("font_color"), ("Editor")) * Color(1, 1, 1, 0.6);
}

void EditorLog::_undo_redo_cbk(void *p_self, const String &p_name) {
	EditorLog *self = (EditorLog *)p_self;
	self->add_message(p_name, EditorLog::MSG_TYPE_EDITOR);
}

void EditorLog::_notification(int p_what) {
	switch (p_what) {
		case (NOTIFICATION_ENTER_TREE): {
			/*
			log->add_theme_font_override("normal_font", get_theme_font("output_source", "EditorFonts"));
			log->add_theme_color_override("selection_color", get_theme_color("accent_color", "Editor") * Color(1, 1, 1, 0.4));
			*/
			_update_theme();
			break;
		}
		case (NOTIFICATION_THEME_CHANGED): {
			/*
			Ref<DynamicFont> df_output_code = get_theme_font("output_source", "EditorFonts");
			if (df_output_code.is_valid()) {
				if (log != nullptr) {
					log->add_theme_font_override("normal_font", get_theme_font("output_source", "EditorFonts"));
					log->add_theme_color_override("selection_color", get_theme_color("accent_color", "Editor") * Color(1, 1, 1, 0.4));
				}
			}
			*/
			_update_theme();
			break;
		}
	}
}

void EditorLog::_set_collapse(bool p_collapse) {
	collapse = p_collapse;
	_rebuild_log();
}

void EditorLog::_clear_request() {
	log->clear();
	messages.clear();
	_reset_message_counts();
	tool_button->set_icon(Ref<Texture>());
}

void EditorLog::_copy_request() {
	String text = log->get_selected_text();

	if (text.empty()) {
		text = log->get_text();
		//get_parsed_text TODO
	}

	if (!text.empty()) {
		OS::get_singleton()->set_clipboard(text);
	}
}

void EditorLog::clear() {
	_clear_request();
}

void EditorLog::copy() {
	_copy_request();
}

void EditorLog::_process_message(const String &p_msg, MessageType p_type, bool p_clear) {
	if (messages.size() > 0 && messages[messages.size() - 1].text == p_msg && messages[messages.size() - 1].type == p_type) {
		// If previous message is the same as the new one, increase previous count rather than adding another
		// instance to the messages list.
		LogMessage &previous = messages.write[messages.size() - 1];
		previous.count++;

		_add_log_line(previous, collapse);
	} else {
		// Different message to the previous one received.
		LogMessage message(p_msg, p_type, p_clear);
		_add_log_line(message);
		messages.push_back(message);
	}

	type_filter_map[p_type]->set_message_count(type_filter_map[p_type]->get_message_count() + 1);
}

void EditorLog::add_message(const String &p_msg, MessageType p_type) {
	// Make text split by new lines their own message.
	// See #41321 for reasoning. At time of writing, multiple print()'s in running projects
	// get grouped together and sent to the editor log as one message. This can mess with the
	// search functionality (see the comments on the PR above for more details). This behavior
	// also matches that of other IDE's.
	Vector<String> lines = p_msg.split("\n", true);
	int line_count = lines.size();

	for (int i = 0; i < line_count; i++) {
		_process_message(lines[i], p_type, i == line_count - 1);
	}
}

void EditorLog::add_thread_message(Thread::ID thread_id, const String &p_msg, MessageType p_type) {
	// Make text split by new lines their own message.
	// See #41321 for reasoning. At time of writing, multiple print()'s in running projects
	// get grouped together and sent to the editor log as one message. This can mess with the
	// search functionality (see the comments on the PR above for more details). This behavior
	// also matches that of other IDE's.
	Vector<String> lines = p_msg.split("\n", true);
	int line_count = lines.size();

	String thread_str = "(T: " + String::num(thread_id) + ") ";

	for (int i = 0; i < line_count; i++) {
		_process_message(thread_str + lines[i], p_type, i == line_count - 1);
	}
}

void EditorLog::set_tool_button(ToolButton *p_tool_button) {
	tool_button = p_tool_button;
}

void EditorLog::_rebuild_log() {
	log->clear();

	for (int msg_idx = 0; msg_idx < messages.size(); msg_idx++) {
		LogMessage msg = messages[msg_idx];

		if (collapse) {
			// If collapsing, only log one instance of the message.
			_add_log_line(msg);
		} else {
			// If not collapsing, log each instance on a line.
			for (int i = 0; i < msg.count; i++) {
				_add_log_line(msg);
			}
		}
	}
}

void EditorLog::_add_log_line(LogMessage &p_message, bool p_replace_previous) {
	if (!is_inside_tree()) {
		// The log will be built all at once when it enters the tree and has its theme items.
		return;
	}

	// Only add the message to the log if it passes the filters.
	bool filter_active = type_filter_map[p_message.type]->is_active();
	String search_text = search_box->get_text();
	bool search_match = search_text.empty() || p_message.text.findn(search_text) > -1;

	if (!filter_active || !search_match) {
		return;
	}

	if (p_replace_previous) {
		// Remove last line if replacing, as it will be replace by the next added line.
		// Why "- 2"? RichTextLabel is weird. When you add a line with add_newline(), it also adds an element to the list of lines which is null/blank,
		// but it still counts as a line. So if you remove the last line (count - 1) you are actually removing nothing...
		log->remove_line(log->get_line_count() - 2);
	}

	switch (p_message.type) {
		case MSG_TYPE_STD: {
		} break;
		case MSG_TYPE_ERROR: {
			log->push_color(theme_cache.error_color);
			Ref<Texture> icon = theme_cache.error_icon;
			log->add_image(icon);
			log->add_text(" ");
			tool_button->set_icon(icon);
		} break;
		case MSG_TYPE_WARNING: {
			log->push_color(theme_cache.warning_color);
			Ref<Texture> icon = theme_cache.warning_icon;
			log->add_image(icon);
			log->add_text(" ");
			tool_button->set_icon(icon);
		} break;
		case MSG_TYPE_EDITOR: {
			// Distinguish editor messages from messages printed by the project
			log->push_color(theme_cache.message_color);
		} break;
	}

	// If collapsing, add the count of this message in bold at the start of the line.
	if (collapse && p_message.count > 1) {
		log->push_bold();
		log->add_text(vformat("(%s) ", itos(p_message.count)));
		log->pop();
	}

	log->add_text(p_message.text);

	if (p_message.clear) {
		log->pop_all(); // Pop all unclosed tags.
	}

	log->add_newline();
}

void EditorLog::_set_filter_active(bool p_active, MessageType p_message_type) {
	type_filter_map[p_message_type]->set_active(p_active);
	_rebuild_log();
}

void EditorLog::_set_search_visible(bool p_visible) {
	search_box->set_visible(p_visible);
	if (p_visible) {
		search_box->grab_focus();
	}
}

void EditorLog::_search_changed(const String &p_text) {
	_rebuild_log();
}

void EditorLog::_reset_message_counts() {
	for (HashMap<MessageType, LogFilter *>::Element *E = type_filter_map.front(); E; E = E->next) {
		E->value()->set_message_count(0);
	}
}

void EditorLog::_bind_methods() {
	ClassDB::bind_method(D_METHOD("add_message"), &EditorLog::add_message);
	ClassDB::bind_method(D_METHOD("add_thread_message"), &EditorLog::add_thread_message);

	ClassDB::bind_method(D_METHOD("_clear_request"), &EditorLog::_clear_request);
	ClassDB::bind_method(D_METHOD("_copy_request"), &EditorLog::_copy_request);

	ClassDB::bind_method(D_METHOD("_set_filter_active"), &EditorLog::_set_filter_active);
	ClassDB::bind_method(D_METHOD("_set_search_visible"), &EditorLog::_set_search_visible);
	ClassDB::bind_method(D_METHOD("_search_changed"), &EditorLog::_search_changed);
	ClassDB::bind_method(D_METHOD("_set_collapse"), &EditorLog::_set_collapse);

	ADD_SIGNAL(MethodInfo("clear_request"));
	ADD_SIGNAL(MethodInfo("copy_request"));
}

EditorLog::EditorLog() {
	collapse = false;

	HBoxContainer *hb = this;

	VBoxContainer *vb_left = memnew(VBoxContainer);
	vb_left->set_custom_minimum_size(Size2(0, 180) * EDSCALE);
	vb_left->set_v_size_flags(SIZE_EXPAND_FILL);
	vb_left->set_h_size_flags(SIZE_EXPAND_FILL);
	hb->add_child(vb_left);

	// Log - Rich Text Label.
	log = memnew(RichTextLabel);
	//log->set_threaded(true);
	log->set_use_bbcode(true);
	log->set_scroll_follow(true);
	log->set_selection_enabled(true);
	//log->set_context_menu_enabled(true);
	log->set_focus_mode(FOCUS_CLICK);
	log->set_v_size_flags(SIZE_EXPAND_FILL);
	log->set_h_size_flags(SIZE_EXPAND_FILL);
	log->set_deselect_on_focus_loss_enabled(false);
	vb_left->add_child(log);

	// Search box
	search_box = memnew(LineEdit);
	search_box->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	search_box->set_placeholder(TTR("Filter Messages"));
	search_box->set_clear_button_enabled(true);
	search_box->set_visible(true);
	search_box->connect("text_changed", this, "_search_changed");
	search_box->hide();
	vb_left->add_child(search_box);

	VBoxContainer *vb_right = memnew(VBoxContainer);
	hb->add_child(vb_right);

	// Tools grid
	HBoxContainer *hb_tools = memnew(HBoxContainer);
	hb_tools->set_h_size_flags(SIZE_SHRINK_CENTER);
	vb_right->add_child(hb_tools);

	// Clear.
	clear_button = memnew(Button);
	clear_button->set_flat(true);
	clear_button->set_focus_mode(FOCUS_NONE);
	clear_button->set_shortcut(ED_SHORTCUT("editor/clear_output", TTR("Clear Output"), KEY_MASK_CTRL | KEY_MASK_SHIFT | KEY_K));
	//clear_button->set_shortcut_context(this);
	clear_button->connect("pressed", this, "_clear_request");
	hb_tools->add_child(clear_button);

	// Copy.
	copy_button = memnew(Button);
	copy_button->set_flat(true);
	copy_button->set_focus_mode(FOCUS_NONE);
	copy_button->set_shortcut(ED_SHORTCUT("editor/copy_output", TTR("Copy Selection"), KEY_MASK_CTRL | KEY_K));
	//copy_button->set_shortcut_context(this);
	copy_button->connect("pressed", this, "_copy_request");
	hb_tools->add_child(copy_button);

	// A second hbox to make a 2x2 grid of buttons.
	HBoxContainer *hb_tools2 = memnew(HBoxContainer);
	hb_tools2->set_h_size_flags(SIZE_SHRINK_CENTER);
	vb_right->add_child(hb_tools2);

	// Collapse.
	collapse_button = memnew(Button);
	collapse_button->set_flat(true);
	collapse_button->set_focus_mode(FOCUS_NONE);
	collapse_button->set_tooltip(TTR("Collapse duplicate messages into one log entry. Shows number of occurrences."));
	collapse_button->set_toggle_mode(true);
	collapse_button->set_pressed(false);
	collapse_button->connect("toggled", this, "_set_collapse");
	hb_tools2->add_child(collapse_button);

	// Show Search.
	show_search_button = memnew(Button);
	show_search_button->set_flat(true);
	show_search_button->set_focus_mode(FOCUS_NONE);
	show_search_button->set_toggle_mode(true);
	show_search_button->set_pressed(false);
	show_search_button->set_shortcut(ED_SHORTCUT("editor/open_search", TTR("Focus Search/Filter Bar"), KEY_MASK_CTRL | KEY_F));
	//show_search_button->set_shortcut_context(this);
	show_search_button->connect("toggled", this, "_set_search_visible");
	hb_tools2->add_child(show_search_button);

	// Message Type Filters.
	vb_right->add_child(memnew(HSeparator));

	LogFilter *std_filter = memnew(LogFilter(MSG_TYPE_STD));
	std_filter->initialize_button(TTR("Toggle visibility of standard output messages."), this, "_set_filter_active");
	vb_right->add_child(std_filter->toggle_button);
	type_filter_map.insert(MSG_TYPE_STD, std_filter);

	LogFilter *error_filter = memnew(LogFilter(MSG_TYPE_ERROR));
	error_filter->initialize_button(TTR("Toggle visibility of errors."), this, "_set_filter_active");
	vb_right->add_child(error_filter->toggle_button);
	type_filter_map.insert(MSG_TYPE_ERROR, error_filter);

	LogFilter *warning_filter = memnew(LogFilter(MSG_TYPE_WARNING));
	warning_filter->initialize_button(TTR("Toggle visibility of warnings."), this, "_set_filter_active");
	vb_right->add_child(warning_filter->toggle_button);
	type_filter_map.insert(MSG_TYPE_WARNING, warning_filter);

	LogFilter *editor_filter = memnew(LogFilter(MSG_TYPE_EDITOR));
	editor_filter->initialize_button(TTR("Toggle visibility of editor messages."), this, "_set_filter_active");
	vb_right->add_child(editor_filter->toggle_button);
	type_filter_map.insert(MSG_TYPE_EDITOR, editor_filter);

	add_message(VERSION_FULL_NAME " (c) 2022-present Péter Magyar, 2007-2022 Juan Linietsky, Ariel Manzur & Godot Contributors.");

	eh.errfunc = _error_handler;
	eh.userdata = this;
	add_error_handler(&eh);

	current = Thread::get_caller_id();

	//add_theme_constant_override("separation", get_theme_constant("separation", "VBoxContainer"));

	EditorNode::get_undo_redo()->set_commit_notify_callback(_undo_redo_cbk, this);
}

void EditorLog::deinit() {
	remove_error_handler(&eh);
}

EditorLog::~EditorLog() {
	for (HashMap<MessageType, LogFilter *>::Element *E = type_filter_map.front(); E; E = E->next) {
		memdelete(E->value());
	}
}
