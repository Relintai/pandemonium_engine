/*************************************************************************/
/*  smtp_client.cpp                                                      */
/*************************************************************************/
/*                         This file is part of:                         */
/*                   PANDEMONIUM ENGINE'S SMTP MODULE                    */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2021-2024 Nicolò Santilio                               */
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

#include "smtp_client.h"

#include "core/bind/core_bind.h"
#include "core/config/engine.h"
#include "core/io/stream_peer_ssl.h"
#include "core/io/stream_peer_tcp.h"
#include "core/log/logger.h"
#include "core/os/os.h"

#include "email.h"

String SMTPClient::get_client_id() const {
	return client_id;
}
void SMTPClient::set_client_id(const String &p_value) {
	client_id = p_value;
}

String SMTPClient::get_host() const {
	return host;
}
void SMTPClient::set_host(const String &p_value) {
	host = p_value;
}

int SMTPClient::get_port() const {
	return port;
}
void SMTPClient::set_port(const int p_value) {
	port = p_value;
}

SMTPClient::TLSMethod SMTPClient::get_tls_method() const {
	return tls_method;
}
void SMTPClient::set_tls_method(const TLSMethod p_value) {
	tls_method = p_value;
}

String SMTPClient::get_server_auth_username() const {
	return server_auth_username;
}
void SMTPClient::set_server_auth_username(const String &p_value) {
	server_auth_username = p_value;
}

String SMTPClient::get_server_auth_password() const {
	return server_auth_password;
}
void SMTPClient::set_server_auth_password(const String &p_value) {
	server_auth_password = p_value;
}

SMTPClient::ServerAuthMethod SMTPClient::get_server_auth_method() const {
	return server_auth_method;
}
void SMTPClient::set_server_auth_method(const ServerAuthMethod p_value) {
	server_auth_method = p_value;
}

String SMTPClient::get_email_default_sender_email() const {
	return email_default_sender_email;
}
void SMTPClient::set_email_default_sender_email(const String &p_value) {
	email_default_sender_email = p_value;
}

String SMTPClient::get_email_default_sender_name() const {
	return email_default_sender_name;
}
void SMTPClient::set_email_default_sender_name(const String &p_value) {
	email_default_sender_name = p_value;
}

bool SMTPClient::get_use_threads() const {
	return _use_threads;
}
void SMTPClient::set_use_threads(const bool p_value) {
	if (_use_threads == p_value) {
		return;
	}

	if (Engine::get_singleton()->is_editor_hint()) {
		_use_threads = p_value;
		return;
	}

	if (is_inside_tree()) {
		if (should_use_threading()) {
			//currently using threads, will no longer be the case

			if (_worker_thread) {
				_worker_thread_running = false;
				//slow quit = let the current mail finish processing
				_worker_thread_fast_quit = false;

				_worker_semaphore.post();
				_worker_thread->wait_to_finish();
				memdelete(_worker_thread);
				_worker_thread = NULL;
			}

		} else {
			//currently not using threads, we want to

			if (should_use_threading()) {
				_worker_thread_running = true;
				_worker_thread = memnew(Thread);
				_worker_thread->start(_worker_thread_func, this);
			}
		}
	}

	_use_threads = p_value;
}

int SMTPClient::get_thread_sleep_usec() const {
	return thread_sleep_usec;
}
void SMTPClient::set_thread_sleep_usec(const int p_value) {
	thread_sleep_usec = p_value;
}

void SMTPClient::send_email(const Ref<EMail> &p_email) {
	ERR_FAIL_COND(!Engine::get_singleton()->is_editor_hint());
	ERR_FAIL_COND(!is_inside_tree());
	ERR_FAIL_COND(!p_email.is_valid());

	if (should_use_threading()) {
		_mail_queue_mutex.lock();
		_mail_queue.push_back(p_email);
		_mail_queue_mutex.unlock();

		_worker_semaphore.post();
	} else {
		_send_email(p_email);
	}
}

bool SMTPClient::should_use_threading() const {
#if !defined(NO_THREADS)
	return _use_threads;
#else
	return false;
#endif
}

void SMTPClient::_send_email(const Ref<EMail> &p_email) {
	_current_session_email = p_email;

	if (!_current_session_email.is_valid()) {
		return;
	}

	Error err = _tcp_client->connect_to_host(host, port);
	if (err != OK) {
		PLOG_ERR("Could not connect! " + itos(err));

		Dictionary error_body;

		error_body["message"] = "Error connecting to host.";
		error_body["code"] = err;

		emit_signal("error", error_body);

		Dictionary result;
		result["success"] = false;
		result["error"] = error_body;

		emit_signal("result", result);

		if (!should_use_threading()) {
			set_process(false);
		}
	}

	if (tls_method == TLS_METHOD_SMTPS) {
		err = _tls_client->connect_to_stream(_tcp_client, false, host);

		if (err != OK) {
			_current_session_status = SESSION_STATUS_SERVER_ERROR;

			Dictionary error_body;

			error_body["message"] = "Error connecting to TLS Stream.";
			error_body["code"] = err;

			emit_signal("error", error_body);

			Dictionary result;
			result["success"] = false;
			result["error"] = error_body;

			emit_signal("result", result);

			if (!should_use_threading()) {
				set_process(false);
			}

			return;
		}

		_current_tls_started = true;
		_current_tls_established = true;
	}

	_current_session_status = SESSION_STATUS_HELO;

	if (!should_use_threading()) {
		set_process(true);
	}
}

Error SMTPClient::poll_client() {
	if (_current_tls_started or _current_tls_established) {
		_tls_client->poll();
		return OK;
	} else {
		return OK;
	}
}

bool SMTPClient::client_get_status() {
	if (_current_tls_started) {
		return _tls_client->get_status() == StreamPeerSSL::STATUS_CONNECTED;
	}

	return _tcp_client->get_status() == StreamPeerTCP::STATUS_CONNECTED;
}

int SMTPClient::client_get_available_bytes() {
	if (_current_tls_started) {
		return _tls_client->get_available_bytes();
	}

	return _tcp_client->get_available_bytes();
}

String SMTPClient::client_get_string(int bytes) {
	if (_current_tls_started) {
		return _tls_client->get_string(bytes);
	}

	return _tcp_client->get_string(bytes);
}

bool SMTPClient::start_auth() {
	if (server_auth_method == SERVER_AUTH_PLAIN) {
		_current_session_status = SESSION_STATUS_AUTHENTICATED;
		return true;
	}

	if (!write_command("AUTH LOGIN")) {
		return false;
	}

	_current_session_status = SESSION_STATUS_AUTH_LOGIN;
	return true;
}
bool SMTPClient::start_hello() {
	//_current_session_status = SESSION_STATUS_HELO
	if (!write_command("HELO " + client_id)) {
		return false;
	}

	_current_session_status = SESSION_STATUS_HELO_ACK;
	return true;
}

Error SMTPClient::client_put_data(const CharString &data) {
	if (_current_tls_established) {
		return _tls_client->put_data((const uint8_t *)data.ptr(), data.size());
	}

	return _tcp_client->put_data((const uint8_t *)data.ptr(), data.size());
}

bool SMTPClient::write_command(const String &command) {
	//print("COMMAND: " + command)
	Error err = client_put_data((command + "\n").utf8());

	if (err != OK) {
		_current_session_status = SESSION_STATUS_COMMAND_NOT_SENT;

		Dictionary error_body;
		error_body["message"] = "Session error on command: " + command;
		error_body["code"] = err;

		emit_signal("error", error_body);

		Dictionary result;
		result["success"] = false;
		result["error"] = error_body;

		emit_signal("result", result);
	}

	return (err == OK);
}

Error SMTPClient::write_data(const String &data) {
	return client_put_data((data + "\r\n.\r\n").utf8());
}

void SMTPClient::close_connection() {
	_current_session_status = SESSION_STATUS_NONE;
	_tls_client->disconnect_from_stream();
	_tcp_client->disconnect_from_host();
	_current_session_email.unref();
	_current_to_index = 0;
	_current_tls_started = false;
	_current_tls_established = false;

	if (!should_use_threading()) {
		set_process(false);
	}
}

String SMTPClient::encode_username() {
	return _Marshalls::get_singleton()->utf8_to_base64(server_auth_username);
}
String SMTPClient::encode_password() {
	return _Marshalls::get_singleton()->utf8_to_base64(server_auth_password);
}

void SMTPClient::_process_email() {
	if (_current_session_status == SESSION_STATUS_SERVER_ERROR) {
		close_connection();
	}

	if (poll_client() == OK) {
		bool connected = client_get_status();

		if (connected) {
			int bytes = client_get_available_bytes();

			if (bytes > 0) {
				String msg = client_get_string(bytes);
				//print("RECEIVED: " + msg)
				String code = msg.left(3);

				if (code == "220") {
					if (_current_session_status == SESSION_STATUS_HELO) {
						start_hello();
					} else if (_current_session_status == SESSION_STATUS_STARTTLS) {
						Error err = _tls_client->connect_to_stream(_tcp_client, false, host);

						if (err != OK) {
							_current_session_status = SESSION_STATUS_SERVER_ERROR;

							Dictionary error_body;
							error_body["message"] = "Error connecting to TLS Stream.";
							error_body["code"] = err;

							emit_signal("error", error_body);

							Dictionary result;
							result["success"] = false;
							result["error"] = error_body;

							emit_signal("result", result);

							return;
						}

						_current_tls_started = true;
						_current_tls_established = true;

						// We need to do HELO + EHLO again
						_current_session_status = SESSION_STATUS_HELO;
						start_hello();
					}

				} else if (code == "250") {
					if (_current_session_status == SESSION_STATUS_HELO_ACK) {
						if (!write_command("EHLO " + client_id)) {
							return;
						}

						_current_session_status = SESSION_STATUS_EHLO_ACK;

					} else if (_current_session_status == SESSION_STATUS_EHLO_ACK) {
						if (tls_method == TLS_METHOD_STARTTLS) {
							if (_current_tls_started) {
								// second round of HELO + EHLO done
								if (!start_auth()) {
									return;
								}

							} else {
								if (!write_command("STARTTLS")) {
									return;
								}

								_current_session_status = SESSION_STATUS_STARTTLS;
							}
						} else {
							if (!start_auth()) {
								return;
							}
						}

					} else if (_current_session_status == SESSION_STATUS_MAIL_FROM) {
						if (_current_to_index < _current_session_email->get_recipient_count()) {
							if (!write_command("RCPT TO: <" + _current_session_email->get_recipient_address(_current_to_index) + ">")) {
								return;
							}

							_current_to_index += 1;
						}

						if (_current_cc_index < _current_session_email->get_cc_count()) {
							if (!write_command("RCPT TO: <" + _current_session_email->get_cc_address(_current_cc_index) + ">")) {
								return;
							}

							_current_cc_index += 1;
						}

						_current_session_status = SESSION_STATUS_RCPT_TO;

					} else if (_current_session_status == SESSION_STATUS_RCPT_TO) {
						if (_current_to_index < _current_session_email->get_recipient_count()) {
							_current_session_status = SESSION_STATUS_MAIL_FROM;
							return;
						}

						if (_current_cc_index < _current_session_email->get_cc_count()) {
							_current_session_status = SESSION_STATUS_MAIL_FROM;
							return;
						}

						if (!write_command("DATA")) {
							return;
						}

						_current_session_status = SESSION_STATUS_DATA;

					} else if (_current_session_status == SESSION_STATUS_DATA_ACK) {
						if (!write_command("QUIT")) {
							return;
						}

						_current_session_status = SESSION_STATUS_QUIT;
					}

				} else if (code == "221") {
					if (_current_session_status == SESSION_STATUS_QUIT) {
						close_connection();
						emit_signal("email_sent");

						Dictionary result;
						result["success"] = true;

						emit_signal("result", result);
					}
				} else if (code == "235") {
					// Authentication Succeeded

					if (_current_session_status == SESSION_STATUS_PASSWORD) {
						_current_session_status = SESSION_STATUS_AUTHENTICATED;
					}

				} else if (code == "334") {
					if (_current_session_status == SESSION_STATUS_AUTH_LOGIN) {
						if (msg.begins_with("334 VXNlcm5hbWU6")) {
							if (!write_command(encode_username())) {
								return;
							}

							_current_session_status = SESSION_STATUS_USERNAME;
						}
					} else if (_current_session_status == SESSION_STATUS_USERNAME) {
						if (msg.begins_with("334 UGFzc3dvcmQ6")) {
							if (!write_command(encode_password())) {
								return;
							}

							_current_session_status = SESSION_STATUS_PASSWORD;
						}
					}
				} else if (code == "354") {
					if (_current_session_status == SESSION_STATUS_DATA) {
						if (!(write_data(_current_session_email->get_email_data_string(email_default_sender_name, email_default_sender_email)) == OK)) {
							_current_session_status = SESSION_STATUS_SERVER_ERROR;
							return;
						}

						_current_session_status = SESSION_STATUS_DATA_ACK;
					}
				} else {
					PLOG_ERR(msg);
				}
			}
		}

		if (_current_session_email.is_valid() && (_current_session_status == SESSION_STATUS_AUTHENTICATED)) {
			_current_session_status = SESSION_STATUS_MAIL_FROM;

			String sender_address = _current_session_email->get_sender_address();

			String fn;

			if (sender_address.size() > 0) {
				fn = "<" + sender_address + ">";
			} else {
				fn = "<" + email_default_sender_email + ">";
			}

			if (!write_command("MAIL FROM: " + fn)) {
				return;
			}
		} else {
			return;
		}
	} else {
		PLOG_ERR("Couldn't poll!")
	}
}

void SMTPClient::_worker_thread_func(void *user_data) {
	SMTPClient *self = (SMTPClient *)user_data;

	while (self->_worker_thread_running) {
		Ref<EMail> _mail;

		self->_mail_queue_mutex.lock();

		int size = self->_mail_queue.size();
		if (size > 0) {
			_mail = self->_mail_queue[0];
			self->_mail_queue.remove(0);
		}

		self->_mail_queue_mutex.unlock();

		if (_mail.is_valid()) {
			self->_send_email(_mail);
		}

		while (self->_current_session_email.is_valid()) {
			OS::get_singleton()->delay_usec(self->thread_sleep_usec);

			// Early return if we want to quit
			if (!self->_worker_thread_running && self->_worker_thread_fast_quit) {
				self->close_connection();
				return;
			}

			self->_process_email();
		}

		if (!self->_worker_thread_running) {
			return;
		}

		if (self->_mail_queue.size() == 0) {
			self->_worker_semaphore.wait();
		}
	}
}

SMTPClient::SMTPClient() {
	client_id = "smtp.pandemoniumengine.org";
	port = 465;

	tls_method = TLS_METHOD_SMTPS;

	server_auth_method = SERVER_AUTH_LOGIN;

	_use_threads = true;
	_worker_thread = NULL;
	// 10 msec
	thread_sleep_usec = 10000;

	// Networking
	_tcp_client.instance();
	_tls_client = Ref<StreamPeerSSL>(StreamPeerSSL::create());

	//SessionStatus
	_current_session_status = SESSION_STATUS_NONE;
	_current_to_index = 0;
	_current_cc_index = 0;

	_current_tls_started = false;
	_current_tls_established = false;

	// Threading
	_worker_thread_running = false;
	_worker_thread_fast_quit = false;

	set_process(false);
}
SMTPClient::~SMTPClient() {
}

void SMTPClient::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_PROCESS: {
			if (Engine::get_singleton()->is_editor_hint()) {
				set_process(false);
				return;
			}

			if (should_use_threading()) {
				set_process(false);
				return;
			}

			_process_email();
		} break;
		case NOTIFICATION_ENTER_TREE: {
			if (Engine::get_singleton()->is_editor_hint()) {
				set_process(false);
				return;
			}

			set_process(false);

			_worker_thread_fast_quit = false;

			if (should_use_threading()) {
				_worker_thread_running = true;
				_worker_thread = memnew(Thread);
				_worker_thread->start(_worker_thread_func, this);
			}

		} break;
		case NOTIFICATION_EXIT_TREE: {
			if (Engine::get_singleton()->is_editor_hint()) {
				set_process(false);
				return;
			}

			if (_worker_thread) {
				_worker_thread_running = false;
				_worker_thread_fast_quit = true;
				_worker_semaphore.post();
				_worker_thread->wait_to_finish();
				memdelete(_worker_thread);
				_worker_thread = NULL;
			}

		} break;
	}
}

void SMTPClient::_bind_methods() {
	ADD_SIGNAL(MethodInfo("error", PropertyInfo(Variant::DICTIONARY, "error")));
	ADD_SIGNAL(MethodInfo("email_sent"));
	ADD_SIGNAL(MethodInfo("result", PropertyInfo(Variant::DICTIONARY, "content")));

	ClassDB::bind_method(D_METHOD("get_client_id"), &SMTPClient::get_client_id);
	ClassDB::bind_method(D_METHOD("set_client_id", "val"), &SMTPClient::set_client_id);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "client_id"), "set_client_id", "get_client_id");

	ClassDB::bind_method(D_METHOD("get_host"), &SMTPClient::get_host);
	ClassDB::bind_method(D_METHOD("set_host", "val"), &SMTPClient::set_host);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "host"), "set_host", "get_host");

	ClassDB::bind_method(D_METHOD("get_port"), &SMTPClient::get_port);
	ClassDB::bind_method(D_METHOD("set_port", "val"), &SMTPClient::set_port);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "port"), "set_port", "get_port");

	ClassDB::bind_method(D_METHOD("get_tls_method"), &SMTPClient::get_tls_method);
	ClassDB::bind_method(D_METHOD("set_tls_method", "val"), &SMTPClient::set_tls_method);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "tls_method", PROPERTY_HINT_ENUM, "NONE,STARTTLS,SMTPS"), "set_tls_method", "get_tls_method");

	ClassDB::bind_method(D_METHOD("get_server_auth_username"), &SMTPClient::get_server_auth_username);
	ClassDB::bind_method(D_METHOD("set_server_auth_username", "val"), &SMTPClient::set_server_auth_username);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "server_auth_username"), "set_server_auth_username", "get_server_auth_username");

	ClassDB::bind_method(D_METHOD("get_server_auth_password"), &SMTPClient::get_server_auth_password);
	ClassDB::bind_method(D_METHOD("set_server_auth_password", "val"), &SMTPClient::set_server_auth_password);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "server_auth_password"), "set_server_auth_password", "get_server_auth_password");

	ClassDB::bind_method(D_METHOD("get_server_auth_method"), &SMTPClient::get_server_auth_method);
	ClassDB::bind_method(D_METHOD("set_server_auth_method", "val"), &SMTPClient::set_server_auth_method);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "server_auth_method", PROPERTY_HINT_ENUM, "Plain,Login"), "set_server_auth_method", "get_server_auth_method");

	ClassDB::bind_method(D_METHOD("get_email_default_sender_email"), &SMTPClient::get_email_default_sender_email);
	ClassDB::bind_method(D_METHOD("set_email_default_sender_email", "val"), &SMTPClient::set_email_default_sender_email);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "email_default_sender_email"), "set_email_default_sender_email", "get_email_default_sender_email");

	ClassDB::bind_method(D_METHOD("get_email_default_sender_name"), &SMTPClient::get_email_default_sender_name);
	ClassDB::bind_method(D_METHOD("set_email_default_sender_name", "val"), &SMTPClient::set_email_default_sender_name);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "email_default_sender_name"), "set_email_default_sender_name", "get_email_default_sender_name");

	ClassDB::bind_method(D_METHOD("get_use_threads"), &SMTPClient::get_use_threads);
	ClassDB::bind_method(D_METHOD("set_use_threads", "val"), &SMTPClient::set_use_threads);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_threads"), "set_use_threads", "get_use_threads");

	ClassDB::bind_method(D_METHOD("get_thread_sleep_usec"), &SMTPClient::get_thread_sleep_usec);
	ClassDB::bind_method(D_METHOD("set_thread_sleep_usec", "val"), &SMTPClient::set_thread_sleep_usec);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "thread_sleep_usec"), "set_thread_sleep_usec", "get_thread_sleep_usec");

	ClassDB::bind_method(D_METHOD("send_email", "email"), &SMTPClient::send_email);

	BIND_ENUM_CONSTANT(TLS_METHOD_NONE);
	BIND_ENUM_CONSTANT(TLS_METHOD_STARTTLS);
	BIND_ENUM_CONSTANT(TLS_METHOD_SMTPS);

	BIND_ENUM_CONSTANT(SERVER_AUTH_PLAIN);
	BIND_ENUM_CONSTANT(SERVER_AUTH_LOGIN);
}
