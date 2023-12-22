#ifndef SMTP_CLIENT_H
#define SMTP_CLIENT_H

/*************************************************************************/
/*  smtp_client.h                                                        */
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

#include "core/containers/vector.h"
#include "core/object/reference.h"
#include "core/io/stream_peer_ssl.h"
#include "core/io/stream_peer_tcp.h"
#include "core/os/mutex.h"
#include "core/os/semaphore.h"
#include "core/os/thread.h"

#include "scene/main/node.h"

class SMTPClient : public Node {
	GDCLASS(SMTPClient, Node);

public:
	enum AuthType {
		AUTH_TYPE_PLAINTEXT, // No SSL
		AUTH_TYPE_STARTTLS, // Connect, then upgrade to SSL using the STARTTLS command before sending login information
		AUTH_TYPE_SMTPS, // Just use SSL
	};

	AuthType get_auth_type() const;
	void set_auth_type(const AuthType value);

	String get_smtp_server_address() const;
	void set_smtp_server_address(const String &value);

	int get_smtp_server_port() const;
	void set_smtp_server_port(const int value);

	String get_smtp_server_user_name() const;
	void set_smtp_server_user_name(const String &value);

	String get_smtp_server_password() const;
	void set_smtp_server_password(const String &value);

	String get_smtp_server_sender_email_address() const;
	void set_smtp_server_sender_email_address(const String &value);

	String get_smtp_server_sender_name() const;
	void set_smtp_server_sender_name(const String &value);

	String get_smtp_client_id() const;
	void set_smtp_client_id(const String &value);

	int get_retries() const;
	void set_retries(const int value);

	int get_delay() const;
	void set_delay(const int value);

	bool get_use_threads() const;
	void set_use_threads(const bool value);

	void send_mail(const String &address, const String &subject, const String &message);

	SMTPClient();
	~SMTPClient();

protected:
	struct MailData {
		String address;
		String subject;
		String message;
	};

	static void _thread_main_loop(void *user_data);
	Error _send_mail(const MailData &data);

	Error _connect();
	void _disconnect();

	Error _smtp_hello();
	Error _smtp_starttls();
	Error _smtp_auth();
	Error _smtp_mail_from();
	Error _smtp_mail_to(const String &recipient);
	Error _smtp_mail_data(const String &subject, const String &message);
	Error _smtp_quit();

	Error _send(const String &data);

	Error _wait_for_answer(const String &expected_answer);
	Error _parse_answer_packet(const String &data, const String &expected_answer);

	void _wait();
	void _apply_thread_setting();

	void _notification(const int what);

	static void _bind_methods();

	AuthType _auth_type;

	String _smtp_server_address;
	int _smtp_server_port;
	String _smtp_server_user_name;
	String _smtp_server_password;
	String _smtp_server_sender_email_address;
	String _smtp_server_sender_name;

	// Can be anything, will be sent to the server at login.
	String _smtp_client_id;

	int _retries;
	int _delay;
	bool _use_threads;

	String _smtp_server_user_name_base64;
	String _smtp_server_password_base64;

	Ref<StreamPeerTCP> _socket_tcp;
	Ref<StreamPeerSSL> _socket_ssl;

	Thread *_worker_thread;
	Semaphore _thread_semaphore;
	bool _thread_running;

	Vector<MailData> _mail_queue;
	Mutex _mail_queue_lock;
};

VARIANT_ENUM_CAST(SMTPClient::AuthType);

#endif
