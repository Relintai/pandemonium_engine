#ifndef HTTP_SESSION_H
#define HTTP_SESSION_H

#include <map>

#include "core/reference.h"

#include "core/string.h"
#include "core/variant.h"
#include "core/threading/mutex.h"


class HTTPSession : public Reference {
	RCPP_OBJECT(HTTPSession, Reference);

public:
	void add(const String &key, const Variant &value);
	void remove(const String &key);
	bool has(const String &key);

	Variant get(const String &key);
	const Variant &get_const(const String &key);
	Object *get_object(const String &key);
	Ref<Reference> get_reference(const String &key);
	int get_int(const String &key);

	String session_id;
	int id;

	void clear();
	void reset();

	std::map<String, Variant> *get_data();

	HTTPSession();
	~HTTPSession();

protected:
	Mutex _mutex;

	std::map<String, Variant> _data;
};

#endif