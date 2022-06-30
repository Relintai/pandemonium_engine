#ifndef HTTP_WRITER_H
#define HTTP_WRITER_H

#include "core/reference.h"

class HTTPWriter : public Reference {
public:
	HTTPWriter();
	~HTTPWriter();

protected:
	static void _bind_methods();
};

#endif
