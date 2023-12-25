/*************************************************************************/
/*  smtp_client.cpp                                                      */
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

#include "smtp_client.h"

#include "core/bind/core_bind.h"
#include "core/io/ip.h"
#include "core/os/os.h"

#define SMTP_DEBUG_ENABLED 0

SMTPClient::AuthType SMTPClient::get_auth_type() const {
	return _auth_type;
}
void SMTPClient::set_auth_type(const AuthType value) {
	_auth_type = value;
}

String SMTPClient::get_smtp_server_address() const {
	return _smtp_server_address;
}
void SMTPClient::set_smtp_server_address(const String &value) {
	_smtp_server_address = value;
}

int SMTPClient::get_smtp_server_port() const {
	return _smtp_server_port;
}
void SMTPClient::set_smtp_server_port(const int value) {
	_smtp_server_port = value;
}

String SMTPClient::get_smtp_server_user_name() const {
	return _smtp_server_user_name;
}
void SMTPClient::set_smtp_server_user_name(const String &value) {
	_smtp_server_user_name = value;

	_smtp_server_user_name_base64 = _Marshalls::get_singleton()->utf8_to_base64(_smtp_server_user_name);
}

String SMTPClient::get_smtp_server_password() const {
	return _smtp_server_password;
}
void SMTPClient::set_smtp_server_password(const String &value) {
	_smtp_server_password = value;

	_smtp_server_password_base64 = _Marshalls::get_singleton()->utf8_to_base64(_smtp_server_password);
}

String SMTPClient::get_smtp_server_sender_email_address() const {
	return _smtp_server_sender_email_address;
}
void SMTPClient::set_smtp_server_sender_email_address(const String &value) {
	_smtp_server_sender_email_address = value;
}

String SMTPClient::get_smtp_server_sender_name() const {
	return _smtp_server_sender_name;
}
void SMTPClient::set_smtp_server_sender_name(const String &value) {
	_smtp_server_sender_name = value;
}

String SMTPClient::get_smtp_client_id() const {
	return _smtp_client_id;
}
void SMTPClient::set_smtp_client_id(const String &value) {
	_smtp_client_id = value;
}

int SMTPClient::get_retries() const {
	return _retries;
}
void SMTPClient::set_retries(const int value) {
	_retries = value;
}

int SMTPClient::get_delay() const {
	return _delay;
}
void SMTPClient::set_delay(const int value) {
	_delay = value;
}

bool SMTPClient::get_use_threads() const {
#ifdef NO_THREADS
	return false;
#else
	return _use_threads;
#endif
}
void SMTPClient::set_use_threads(const bool value) {
	_use_threads = value;

	_apply_thread_setting();
}

void SMTPClient::send_mail(const String &address, const String &subject, const String &message) {
	MailData md;
	md.address = address;
	md.subject = subject;
	md.message = message;

	if (get_use_threads()) {
		_mail_queue_lock.lock();
		_mail_queue.push_back(md);
		_mail_queue_lock.unlock();

		_thread_semaphore.post();
	} else {
		_send_mail(md);
	}
}

SMTPClient::SMTPClient() {
	_auth_type = AUTH_TYPE_SMTPS;

	_smtp_server_port = 465;

	_retries = 3;
	_delay = 200;
	_use_threads = true;

	_worker_thread = NULL;
	_thread_running = false;
}

SMTPClient::~SMTPClient() {
}

void SMTPClient::_thread_main_loop(void *user_data) {
	SMTPClient *self = (SMTPClient *)user_data;

	while (self->_thread_running) {
		self->_mail_queue_lock.lock();

		if (self->_mail_queue.size() > 0) {
			int index = self->_mail_queue.size() - 1;
			MailData d = self->_mail_queue[index];
			self->_mail_queue.remove(index);

			self->_mail_queue_lock.unlock();

			self->_send_mail(d);
		} else {
			self->_mail_queue_lock.lock();
		}

		if (self->_mail_queue.size() == 0) {
			self->_thread_semaphore.wait();
		}
	}
}

Error SMTPClient::_send_mail(const MailData &data) {
	Error err = OK;

	err = _connect();

	if (err == OK) {
		err = _wait_for_answer("");
	}

	if (err == OK) {
		err = _smtp_hello();
	}

	if (err == OK && _auth_type == AUTH_TYPE_STARTTLS) {
		err = _smtp_starttls();

		if (err == OK) {
			err = _smtp_hello();
		}
	}

	if (err == OK) {
		err = _smtp_auth();
	}

	if (err == OK) {
		err = _smtp_mail_from();
	}

	if (err == OK) {
		err = _smtp_mail_to(data.address);
	}

	if (err == OK) {
		err = _smtp_mail_data(data.subject, data.message);
	}

	if (err == OK) {
		err = _smtp_quit();
	}

	_disconnect();

#if SMTP_DEBUG_ENABLED
	if (err == OK) {
		ERR_PRINT("Mail Sent");
	} else {
		ERR_PRINT("ERROR sending mail: " + itos(err));
	}
#endif

	return err;
}

Error SMTPClient::_connect() {
	Error error = OK;

	if (_socket_tcp.is_valid()) {
		return ERR_ALREADY_IN_USE;
	}

	_socket_tcp.instance();
	error = _socket_tcp->connect_to_host(_smtp_server_address, _smtp_server_port);

#if SMTP_DEBUG_ENABLED
	ERR_PRINT("Connecting server...");
#endif

	if (error != OK) {
#if SMTP_DEBUG_ENABLED
		ERR_PRINT("Connection failed.");
#endif

		IP_Address ip = IP::get_singleton()->resolve_hostname(_smtp_server_address);
		error = _socket_tcp->connect_to_host(ip, _smtp_server_port);
#if SMTP_DEBUG_ENABLED
		ERR_PRINT("Trying with IP");
#endif
	}

	for (int i = 0; i < _retries; ++i) {
#if SMTP_DEBUG_ENABLED
		ERR_PRINT("Try " + Streing::num(i) + " / " + String::num(_retries));
#endif

		StreamPeerTCP::Status status = _socket_tcp->get_status();

#if SMTP_DEBUG_ENABLED
		ERR_PRINT("StreamPeerSSL::STATUS: " + itos(status));
#endif
		if (status == StreamPeerTCP::STATUS_CONNECTED) {
			break;
		}

		_wait();
	}

	if (_socket_tcp->get_status() == StreamPeerTCP::STATUS_ERROR) {
		return ERR_CANT_CONNECT;
	}

	if (_auth_type == AUTH_TYPE_SMTPS) {
		_socket_ssl = Ref<StreamPeerSSL>(StreamPeerSSL::create());
		_socket_ssl->connect_to_stream(_socket_tcp, true, _smtp_server_address);

		for (int i = 0; i < _retries; ++i) {
#if SMTP_DEBUG_ENABLED
			ERR_PRINT("SSL Try " + Streing::num(i) + " / " + String::num(_retries));
#endif

			StreamPeerSSL::Status status = _socket_ssl->get_status();

#if SMTP_DEBUG_ENABLED
			ERR_PRINT("StreamPeerSSL::STATUS: " + itos(status));
#endif

			if (status == StreamPeerSSL::STATUS_CONNECTED) {
				break;
			}

			_wait();
		}

		if (_socket_ssl->get_status() == StreamPeerSSL::STATUS_ERROR) {
			return ERR_CANT_CONNECT;
		}
	}

	return OK;
}
void SMTPClient::_disconnect() {
	if (_socket_ssl.is_valid()) {
		_socket_ssl->disconnect_from_stream();
	}

	_socket_tcp->disconnect_from_host();

	_socket_ssl.unref();
	_socket_tcp.unref();
}

Error SMTPClient::_smtp_hello() {
	Error err = _send("HELO " + _smtp_client_id);
	_wait_for_answer("");
	err = _send("EHLO " + _smtp_client_id);
	err = _wait_for_answer("250");

	return err;
}
Error SMTPClient::_smtp_starttls() {
	Error err = _send("STARTTLS");
	err = _wait_for_answer("220"); // 220 TLS go ahead

	if (err != OK) {
		return err;
	}

	_socket_ssl = Ref<StreamPeerSSL>(StreamPeerSSL::create());

	_socket_ssl->connect_to_stream(_socket_tcp, true, _smtp_server_address);

	for (int i = 0; i < _retries; ++i) {
#if SMTP_DEBUG_ENABLED
		ERR_PRINT("SSL Try " + String::num(i) + " / " + String::num(_retries));
#endif

		StreamPeerSSL::Status status = _socket_ssl->get_status();

		if (status == StreamPeerSSL::STATUS_ERROR) {
			return ERR_CANT_CONNECT;
		}

#if SMTP_DEBUG_ENABLED
		ERR_PRINT("StreamPeerSSL::STATUS: " + itos(status));
#endif

		if (status == StreamPeerSSL::STATUS_CONNECTED) {
			break;
		}

		_wait();
	}

	if (_socket_ssl->get_status() == StreamPeerSSL::STATUS_ERROR) {
		return ERR_CANT_CONNECT;
	}

	return OK;
}
Error SMTPClient::_smtp_auth() {
	Error err = _send("AUTH LOGIN");
	err = _wait_for_answer("334");

#if SMTP_DEBUG_ENABLED
	ERR_PRINT("_smtp_auth 1");
#endif

	if (err == OK) {
		err = _send(_smtp_server_user_name_base64);
	}

	err = _wait_for_answer("334");

#if SMTP_DEBUG_ENABLED
	ERR_PRINT("_smtp_auth 2");
#endif

	if (err == OK) {
		err = _send(_smtp_server_password_base64);
	}

	err = _wait_for_answer("235");

#if SMTP_DEBUG_ENABLED
	ERR_PRINT("_smtp_auth 3");
#endif

	return err;
}
Error SMTPClient::_smtp_mail_from() {
	Error err = _send("MAIL FROM: <" + _smtp_server_sender_email_address + ">");
	err = _wait_for_answer("250");
	return err;
}
Error SMTPClient::_smtp_mail_to(const String &recipient) {
	Error err = _send("RCPT TO: <" + recipient + ">");
	err = _wait_for_answer("250");
	return err;
}
Error SMTPClient::_smtp_mail_data(const String &subject, const String &message) {
	Error err = _send("DATA");

	err = _wait_for_answer("354");

	if (err == OK) {
		err = _send("FROM: " + _smtp_server_sender_name + " <" + _smtp_server_sender_email_address + ">");
		//err = _wait_for_answer("250");
	}

	if (err == OK && !subject.empty()) {
		err = _send("SUBJECT: " + subject);
		//err = _wait_for_answer("250");
	}

	if (err == OK && !message.empty()) {
		err = _send(message + "\r\n.\r\n");
		//err = _wait_for_answer("250");
	}

	err = _wait_for_answer("250");

	return err;
}
Error SMTPClient::_smtp_quit() {
	return _send("QUIT");
}

Error SMTPClient::_send(const String &data) {
#if SMTP_DEBUG_ENABLED
	ERR_PRINT("_send: " + data);
#endif

	String s = data + "\n";
	Vector<uint8_t> v = s.to_utf8_buffer();

	if (_socket_ssl.is_valid()) {
		return _socket_ssl->put_data(v.ptr(), v.size());
	}

	if (_socket_tcp.is_valid()) {
		return _socket_tcp->put_data(v.ptr(), v.size());
	}

	return ERR_CANT_CONNECT;
}

Error SMTPClient::_wait_for_answer(const String &expected_answer) {
	String _read_buffer;

	_wait();

	for (int i = 0; i < _retries; ++i) {
		if (_socket_ssl.is_valid()) {
			_socket_ssl->poll();
		}

		int available_bytes = 0;

		if (_socket_ssl.is_valid()) {
			available_bytes = _socket_ssl->get_available_bytes();
		} else if (_socket_tcp.is_valid()) {
			available_bytes = _socket_tcp->get_available_bytes();
		} else {
			return ERR_CANT_CONNECT;
		}

		if (available_bytes > 0) {
			if (_socket_ssl.is_valid()) {
				_read_buffer += _socket_ssl->get_utf8_string();
			} else if (_socket_tcp.is_valid()) {
				_read_buffer += _socket_tcp->get_available_bytes();
			} else {
				return ERR_CANT_CONNECT;
			}

#if SMTP_DEBUG_ENABLED
			ERR_PRINT("_wait_for_answer received: " + _read_buffer);
#endif

			break;
		} else {
			_wait();
		}
	}

	if (_read_buffer.empty()) {
		return ERR_CANT_ACQUIRE_RESOURCE;
	}

	if (_parse_answer_packet(_read_buffer, expected_answer) != OK) {
		return FAILED;
	}

	return OK;
}
Error SMTPClient::_parse_answer_packet(const String &data, const String &expected_answer) {
	if (expected_answer.empty()) {
		return OK;
	}

	int slicecount = data.get_slice_count("\r\n");

	if (slicecount <= 1) {
		if (data.left(expected_answer.length()) == expected_answer) {
			return OK;
		} else {
			return FAILED;
		}
	} else {
		String ll = data.get_slice("\r\n", slicecount - 2);
		if (ll.left(expected_answer.length()) == expected_answer) {
			return OK;
		} else {
			return FAILED;
		}
	}
}

void SMTPClient::_wait() {
	OS::get_singleton()->delay_usec(int64_t(_delay) * 1000);
}

void SMTPClient::_apply_thread_setting() {
	if (!is_inside_tree()) {
		return;
	}

	if (get_use_threads() || !_worker_thread) {
		_thread_running = true;
		_worker_thread = memnew(Thread);
		_worker_thread->start(_thread_main_loop, this);
	}

	if (!get_use_threads() && _worker_thread) {
		_thread_running = false;
		_thread_semaphore.post();
		_worker_thread->wait_to_finish();
		memdelete(_worker_thread);
		_worker_thread = NULL;
	}
}

void SMTPClient::_notification(const int what) {
	switch (what) {
		case NOTIFICATION_ENTER_TREE: {
			if (get_use_threads()) {
				_thread_running = true;
				_worker_thread = memnew(Thread);
				_worker_thread->start(_thread_main_loop, this);
			}
		} break;
		case NOTIFICATION_EXIT_TREE: {
			if (_worker_thread) {
				_thread_running = false;
				_thread_semaphore.post();
				_worker_thread->wait_to_finish();
				memdelete(_worker_thread);
				_worker_thread = NULL;
			}
		} break;
	}
}

void SMTPClient::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_auth_type"), &SMTPClient::get_auth_type);
	ClassDB::bind_method(D_METHOD("set_auth_type", "val"), &SMTPClient::set_auth_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "auth_type", PROPERTY_HINT_ENUM, "Plaintext,STARTTLS,SMTPS"), "set_auth_type", "get_auth_type");

	ClassDB::bind_method(D_METHOD("get_smtp_server_address"), &SMTPClient::get_smtp_server_address);
	ClassDB::bind_method(D_METHOD("set_smtp_server_address", "val"), &SMTPClient::set_smtp_server_address);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "smtp_server_address"), "set_smtp_server_address", "get_smtp_server_address");

	ClassDB::bind_method(D_METHOD("get_smtp_server_port"), &SMTPClient::get_smtp_server_port);
	ClassDB::bind_method(D_METHOD("set_smtp_server_port", "val"), &SMTPClient::set_smtp_server_port);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "smtp_server_port"), "set_smtp_server_port", "get_smtp_server_port");

	ClassDB::bind_method(D_METHOD("get_smtp_server_user_name"), &SMTPClient::get_smtp_server_user_name);
	ClassDB::bind_method(D_METHOD("set_smtp_server_user_name", "val"), &SMTPClient::set_smtp_server_user_name);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "smtp_server_user_name"), "set_smtp_server_user_name", "get_smtp_server_user_name");

	ClassDB::bind_method(D_METHOD("get_smtp_server_password"), &SMTPClient::get_smtp_server_password);
	ClassDB::bind_method(D_METHOD("set_smtp_server_password", "val"), &SMTPClient::set_smtp_server_password);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "smtp_server_password"), "set_smtp_server_password", "get_smtp_server_password");

	ClassDB::bind_method(D_METHOD("get_smtp_server_sender_email_address"), &SMTPClient::get_smtp_server_sender_email_address);
	ClassDB::bind_method(D_METHOD("set_smtp_server_sender_email_address", "val"), &SMTPClient::set_smtp_server_sender_email_address);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "smtp_server_sender_email_address"), "set_smtp_server_sender_email_address", "get_smtp_server_sender_email_address");

	ClassDB::bind_method(D_METHOD("get_smtp_server_sender_name"), &SMTPClient::get_smtp_server_sender_name);
	ClassDB::bind_method(D_METHOD("set_smtp_server_sender_name", "val"), &SMTPClient::set_smtp_server_sender_name);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "smtp_server_sender_name"), "set_smtp_server_sender_name", "get_smtp_server_sender_name");

	ClassDB::bind_method(D_METHOD("get_smtp_client_id"), &SMTPClient::get_smtp_client_id);
	ClassDB::bind_method(D_METHOD("set_smtp_client_id", "val"), &SMTPClient::set_smtp_client_id);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "smtp_client_id"), "set_smtp_client_id", "get_smtp_client_id");

	ClassDB::bind_method(D_METHOD("get_retries"), &SMTPClient::get_retries);
	ClassDB::bind_method(D_METHOD("set_retries", "val"), &SMTPClient::set_retries);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "retries"), "set_retries", "get_retries");

	ClassDB::bind_method(D_METHOD("get_delay"), &SMTPClient::get_delay);
	ClassDB::bind_method(D_METHOD("set_delay", "val"), &SMTPClient::set_delay);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "get_delay"), "set_delay", "get_delay");

	ClassDB::bind_method(D_METHOD("get_use_threads"), &SMTPClient::get_use_threads);
	ClassDB::bind_method(D_METHOD("set_use_threads", "val"), &SMTPClient::set_use_threads);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_threads"), "set_use_threads", "get_use_threads");

	ClassDB::bind_method(D_METHOD("send_mail", "address", "subject", "message"), &SMTPClient::send_mail);

	BIND_ENUM_CONSTANT(AUTH_TYPE_PLAINTEXT);
	BIND_ENUM_CONSTANT(AUTH_TYPE_STARTTLS);
	BIND_ENUM_CONSTANT(AUTH_TYPE_SMTPS);
}
