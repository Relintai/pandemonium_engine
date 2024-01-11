#ifndef SMTP_CLIENT_H
#define SMTP_CLIENT_H

/*************************************************************************/
/*  smtp_client.h                                                        */
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

#include "core/object/reference.h"
#include "core/os/mutex.h"
#include "core/os/semaphore.h"
#include "core/os/thread.h"

#include "scene/main/node.h"

class EMail;
class StreamPeerTCP;
class StreamPeerSSL;

class SMTPClient : public Node {
	GDCLASS(SMTPClient, Node);

public:
	enum TLSMethod {
		TLS_METHOD_NONE = 0,
		TLS_METHOD_STARTTLS,
		TLS_METHOD_SMTPS,
	};

	enum ServerAuthMethod {
		SERVER_AUTH_PLAIN,
		SERVER_AUTH_LOGIN
	};

	String get_client_id() const;
	void set_client_id(const String &p_value);

	String get_host() const;
	void set_host(const String &p_value);

	int get_port() const;
	void set_port(const int p_value);

	TLSMethod get_tls_method() const;
	void set_tls_method(const TLSMethod p_value);

	String get_server_auth_username() const;
	void set_server_auth_username(const String &p_value);

	String get_server_auth_password() const;
	void set_server_auth_password(const String &p_value);

	ServerAuthMethod get_server_auth_method() const;
	void set_server_auth_method(const ServerAuthMethod p_value);

	String get_email_default_sender_email() const;
	void set_email_default_sender_email(const String &p_value);

	String get_email_default_sender_name() const;
	void set_email_default_sender_name(const String &p_value);

	bool get_use_threads() const;
	void set_use_threads(const bool p_value);

	int get_thread_sleep_usec() const;
	void set_thread_sleep_usec(const int p_value);

	void send_email(const Ref<EMail> &p_email);

	SMTPClient();
	~SMTPClient();

protected:
	enum SessionStatus {
		SESSION_STATUS_NONE = 0,
		SESSION_STATUS_SERVER_ERROR,
		SESSION_STATUS_COMMAND_NOT_SENT,
		SESSION_STATUS_COMMAND_REFUSED,
		SESSION_STATUS_HELO,
		SESSION_STATUS_HELO_ACK,
		SESSION_STATUS_EHLO,
		SESSION_STATUS_EHLO_ACK,
		SESSION_STATUS_MAIL_FROM,
		SESSION_STATUS_RCPT_TO,
		SESSION_STATUS_DATA,
		SESSION_STATUS_DATA_ACK,
		SESSION_STATUS_QUIT,
		SESSION_STATUS_STARTTLS,
		SESSION_STATUS_STARTTLS_ACK,
		SESSION_STATUS_AUTH_LOGIN,
		SESSION_STATUS_USERNAME,
		SESSION_STATUS_PASSWORD,
		SESSION_STATUS_AUTHENTICATED
	};

	bool should_use_threading() const;

	void _send_email(const Ref<EMail> &p_email);

	Error poll_client();

	bool client_get_status();
	int client_get_available_bytes();
	String client_get_string(int bytes);

	bool start_auth();
	bool start_hello();

	Error client_put_data(const String &data);

	bool write_command(const String &command);
	Error write_data(const String &data);

	void close_connection();

	String encode_username();
	String encode_password();

	void _process_email();
	void _no_thread_next_email();

	static void _worker_thread_func(void *user_data);

	void _notification(int p_what);
	static void _bind_methods();

	String client_id;

	String host;
	int port;

	TLSMethod tls_method;

	String server_auth_username;
	String server_auth_password;

	ServerAuthMethod server_auth_method;

	String email_default_sender_email;
	String email_default_sender_name;

	bool _use_threads;
	int thread_sleep_usec;

	// Networking
	Ref<StreamPeerSSL> _tls_client;
	Ref<StreamPeerTCP> _tcp_client;

	// SessionStatus
	SessionStatus _current_session_status;
	Ref<EMail> _current_session_email;
	int _current_to_index;
	int _current_cc_index;

	bool _current_tls_started;
	bool _current_tls_established;

	// Threading
	bool _worker_thread_running;
	bool _worker_thread_fast_quit;
	Thread *_worker_thread;
	Semaphore _worker_semaphore;
	Mutex _mail_queue_mutex;

	Vector<Ref<EMail>> _mail_queue;
};

VARIANT_ENUM_CAST(SMTPClient::TLSMethod);
VARIANT_ENUM_CAST(SMTPClient::ServerAuthMethod);

#endif
