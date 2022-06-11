#ifndef LOGGER_BACKEND_H
#define LOGGER_BACKEND_H

#include "core/ustring.h"
#include "core/variant.h"

#include "core/reference.h"

class LoggerBackend : public Reference {
	GDCLASS(LoggerBackend, Reference);

public:
	LoggerBackend();
	~LoggerBackend();

protected:
	static void _bind_methods();
};

#endif
