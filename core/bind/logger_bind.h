
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
	enum LogLevel {
		LOG_LEVEL_TRACE = 0,
		LOG_LEVEL_MESSAGE,
		LOG_LEVEL_WARNING,
		LOG_LEVEL_ERROR,
		LOG_LEVEL_NONE,
	};

	Ref<LoggerBackend> get_backend();
	void set_backend(const Ref<LoggerBackend> &backend);

	void log_trace(const String &str);
	void log_message(const String &str);
	void log_warning(const String &str);
	void log_error(const String &str);
	
	LogLevel get_log_level();
	void set_log_level(const LogLevel p_log_level);

	static _PLogger *get_singleton();

	_PLogger();
	~_PLogger();

protected:
	static void _bind_methods();

	static _PLogger *_self;
};

VARIANT_ENUM_CAST(_PLogger::LogLevel);

#endif
