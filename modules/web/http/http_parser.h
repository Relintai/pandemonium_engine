#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

#include "core/reference.h"

class HTTPParser : public Reference {
public:
	HTTPParser();
	~HTTPParser();

protected:
	static void _bind_methods();
};

#endif
