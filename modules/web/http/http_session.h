#ifndef HTTP_SESSION_H
#define HTTP_SESSION_H

/*************************************************************************/
/*  http_session.h                                                       */
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

#include "core/containers/hash_map.h"

#include "core/object/reference.h"

#include "core/os/mutex.h"
#include "core/string/ustring.h"
#include "core/variant/variant.h"

//TODO
//Clean up the api
//Add api for ints, floats, strings and these should be saved (like they used to be)
//Make sure it's clear that thye will be like add_saved_int()...
//Add an api for variants, similar to get set_meta, but rw locked.
//Make sure it's obvious that it's not saved. Maybe set_temp_data ...

//Also add an RWLock

class HTTPSession : public Reference {
	GDCLASS(HTTPSession, Reference);

public:
	String get_session_id();
	void set_session_id(const String &val);

	int get_id();
	void set_id(const int val);

	void add(const String &key, const Variant &value);
	void remove(const String &key);
	bool has(const String &key);
	Variant get_value(const String &key);

	const Variant &get_const(const String &key);
	Object *get_object(const String &key);
	Ref<Reference> get_reference(const String &key);
	int get_int(const String &key, const int def = 0);

	void clear();
	void reset();

	HashMap<String, Variant> *get_data();

	HTTPSession();
	~HTTPSession();

	String session_id;
	int id;

protected:
	static void _bind_methods();

	Mutex _mutex;

	HashMap<String, Variant> _data;
};

#endif
