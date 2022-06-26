#ifndef HTTP_SESSION_H
#define HTTP_SESSION_H

#include "core/hash_map.h"

#include "core/reference.h"

#include "core/os/mutex.h"
#include "core/ustring.h"
#include "core/variant.h"

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
	int get_int(const String &key);

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
