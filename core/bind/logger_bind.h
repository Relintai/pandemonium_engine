
#ifndef PLOGGER_BIND_H
#define PLOGGER_BIND_H

#include "core/log/logger_backend.h"
#include "core/object/reference.h"
#include "core/typedefs.h"
#include "core/string/ustring.h"

class String;

class _PLogger : public Object {
	GDCLASS(_PLogger, Object);

public:
	Ref<LoggerBackend> get_backend();
	void set_backend(const Ref<LoggerBackend> &backend);

	void log_trace(const String &str);
	void log_message(const String &str);
	void log_warning(const String &str);
	void log_error(const String &str);

	static _PLogger *get_singleton();

	_PLogger();
	~_PLogger();

protected:
	static void _bind_methods();

	static _PLogger *_self;
};

#endif
