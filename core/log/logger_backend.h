#ifndef LOGGER_BACKEND_H
#define LOGGER_BACKEND_H

#include "core/ustring.h"
#include "core/variant.h"

#include "core/reference.h"

class LoggerBackend : public Reference {
	GDCLASS(LoggerBackend, Reference);

public:
	virtual void log_trace(const String &str);
	virtual void log_message(const String &str);
	virtual void log_warning(const String &str);
	virtual void log_error(const String &str);

	virtual void _log_trace(const String &str);
	virtual void _log_message(const String &str);
	virtual void _log_warning(const String &str);
	virtual void _log_error(const String &str);

	LoggerBackend();
	~LoggerBackend();

protected:
	static void _bind_methods();
};

#endif
