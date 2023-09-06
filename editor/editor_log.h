#ifndef EDITOR_LOG_H
#define EDITOR_LOG_H
/*************************************************************************/
/*  editor_log.h                                                         */
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

#include "scene/gui/box_container.h"

#include "core/error/error_macros.h"
#include "core/object/object.h"
#include "core/os/thread.h"
#include "core/string/ustring.h"

class Button;
class Label;
class RichTextLabel;
class ToolButton;
class LineEdit;

class EditorLog : public HBoxContainer {
	GDCLASS(EditorLog, HBoxContainer);

public:
	enum MessageType {
		MSG_TYPE_STD,
		MSG_TYPE_ERROR,
		MSG_TYPE_WARNING,
		MSG_TYPE_EDITOR
	};

private:
	struct LogMessage {
		String text;
		MessageType type;
		int count = 1;
		bool clear = true;

		LogMessage() {}

		LogMessage(const String p_text, MessageType p_type, bool p_clear) :
				text(p_text),
				type(p_type),
				clear(p_clear) {
		}
	};

	struct {
		Color error_color;
		Ref<Texture> error_icon;

		Color warning_color;
		Ref<Texture> warning_icon;

		Color message_color;
	} theme_cache;

	// Encapsulates all data and functionality regarding filters.
	struct LogFilter {
	private:
		// Force usage of set method since it has functionality built-in.
		int message_count;
		bool active;

	public:
		MessageType type;
		Button *toggle_button;

		void initialize_button(const String &p_tooltip, Object *p_cb_obj, const StringName &p_method);

		int get_message_count();
		void set_message_count(int p_count);

		bool is_active();
		void set_active(bool p_active);

		LogFilter(MessageType p_type);
	};

private:
	Vector<LogMessage> messages;
	// Maps MessageTypes to LogFilters for convenient access and storage (don't need 1 member per filter).
	HashMap<MessageType, LogFilter *> type_filter_map;

	RichTextLabel *log;

	Button *clear_button;
	Button *copy_button;

	Button *collapse_button;
	bool collapse;

	Button *show_search_button;
	LineEdit *search_box;

	// Reference to the "Output" button on the toolbar so we can update it's icon when
	// Warnings or Errors are encounetered.
	ToolButton *tool_button;

	static void _error_handler(void *p_self, const char *p_func, const char *p_file, int p_line, const char *p_error, const char *p_errorexp, ErrorHandlerType p_type);

	ErrorHandlerList eh;

	Thread::ID current;

	void _clear_request();
	void _copy_request();

	void _rebuild_log();
	void _add_log_line(LogMessage &p_message, bool p_replace_previous = false);

	void _set_filter_active(bool p_active, MessageType p_message_type);
	void _set_search_visible(bool p_visible);
	void _search_changed(const String &p_text);

	void _process_message(const String &p_msg, MessageType p_type, bool p_clear);
	void _reset_message_counts();

	void _set_collapse(bool p_collapse);

	void _update_theme();

	static void _undo_redo_cbk(void *p_self, const String &p_name);

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	void add_message(const String &p_msg, MessageType p_type = MSG_TYPE_STD);
	void set_tool_button(ToolButton *p_tool_button);
	void deinit();

	void clear();
	void copy();

	EditorLog();
	~EditorLog();
};

VARIANT_ENUM_CAST(EditorLog::MessageType);

#endif // EDITOR_LOG_H
