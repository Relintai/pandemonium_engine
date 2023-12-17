/*************************************************************************/
/*  gdscript_language_server.cpp                                         */
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

#include "gdscript_language_server.h"

#include "core/os/file_access.h"
#include "core/os/os.h"
#include "editor/editor_log.h"
#include "editor/editor_node.h"
#include "editor/editor_settings.h"

int GDScriptLanguageServer::port_override = -1;

GDScriptLanguageServer::GDScriptLanguageServer() {
	enabled = false;
	thread_running = false;
	started = false;
	use_thread = false;
	host = "127.0.0.1";
	port = 6008;

	_EDITOR_DEF("network/language_server/enabled", enabled, true);
	_EDITOR_DEF("network/language_server/remote_host", host);
	_EDITOR_DEF("network/language_server/remote_port", port);
	_EDITOR_DEF("network/language_server/enable_smart_resolve", true);
	_EDITOR_DEF("network/language_server/show_native_symbols_in_editor", false);
	_EDITOR_DEF("network/language_server/use_thread", use_thread);
}

void GDScriptLanguageServer::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE:
			start();
			break;
		case NOTIFICATION_EXIT_TREE:
			stop();
			break;
		case NOTIFICATION_INTERNAL_PROCESS: {
			if (started && !use_thread) {
				protocol.poll();
			}
		} break;
		case EditorSettings::NOTIFICATION_EDITOR_SETTINGS_CHANGED: {
			if (!enabled) {
				return;
			}

			String host = String(_EDITOR_GET("network/language_server/remote_host"));
			int port = (GDScriptLanguageServer::port_override > -1) ? GDScriptLanguageServer::port_override : (int)_EDITOR_GET("network/language_server/remote_port");
			bool use_thread = (bool)_EDITOR_GET("network/language_server/use_thread");
			if (host != this->host || port != this->port || use_thread != this->use_thread) {
				this->stop();
				this->start();
			}
		} break;
	}
}

void GDScriptLanguageServer::thread_main(void *p_userdata) {
	GDScriptLanguageServer *self = static_cast<GDScriptLanguageServer *>(p_userdata);
	while (self->thread_running) {
		// Poll 20 times per second
		self->protocol.poll();
		OS::get_singleton()->delay_usec(50000);
	}
}

void GDScriptLanguageServer::start() {
	enabled = _EDITOR_GET("network/language_server/enabled");
	host = String(_EDITOR_GET("network/language_server/remote_host"));
	port = (GDScriptLanguageServer::port_override > -1) ? GDScriptLanguageServer::port_override : (int)_EDITOR_GET("network/language_server/remote_port");
	use_thread = (bool)_EDITOR_GET("network/language_server/use_thread");

	if (!enabled) {
		return;
	}

	if (protocol.start(port, IP_Address(host)) == OK) {
		EditorNode::get_log()->add_message("--- GDScript language server started on port " + itos(port) + " ---", EditorLog::MSG_TYPE_EDITOR);
		if (use_thread) {
			thread_running = true;
			thread.start(GDScriptLanguageServer::thread_main, this);
		}
		set_process_internal(!use_thread);
		started = true;
	}
}

void GDScriptLanguageServer::stop() {
	if (!enabled) {
		return;
	}

	if (use_thread) {
		ERR_FAIL_COND(!thread.is_started());
		thread_running = false;
		thread.wait_to_finish();
	}
	protocol.stop();
	started = false;
	EditorNode::get_log()->add_message("--- GDScript language server stopped ---", EditorLog::MSG_TYPE_EDITOR);
}

void register_lsp_types() {
	ClassDB::register_class<GDScriptLanguageProtocol>();
	ClassDB::register_class<GDScriptTextDocument>();
	ClassDB::register_class<GDScriptWorkspace>();
}
