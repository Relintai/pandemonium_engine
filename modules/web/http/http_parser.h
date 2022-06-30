#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

#include "core/ustring.h"

#include "core/reference.h"

class WebServerRequest;

class HTTPParser : public Reference {
	GDCLASS(HTTPParser, Reference);

public:
	Ref<WebServerRequest> get_request();
	void set_request(const Ref<WebServerRequest> &val);

	bool is_ready() const;

	void reset();

	//returns the index where processing was ended -> start of the next query if != data_length
	int read_from_buffer(const char *p_buffer, const int p_data_length);

	HTTPParser();
	~HTTPParser();

protected:
	static void _bind_methods();

	String _partial_data;

	Ref<WebServerRequest> _request;

	bool _is_ready;
};

#endif
