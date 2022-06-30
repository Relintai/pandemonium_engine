#ifndef SETTINGS_H
#define SETTINGS_H

#include <map>

#include "core/object.h"

#include "core/string.h"
#include "core/variant.h"

class Settings : public Object {
	RCPP_OBJECT(Settings, Object);

public:
	Variant get_value(const String &key, const Variant &def = Variant());
	String get_value_string(const String &key, const String &def = "");
	int get_value_int(const String &key, const int def = 0);
	float get_value_float(const String &key, const float def = 0);
	double get_value_double(const String &key, const double def = 0);
	bool get_value_bool(const String &key, const bool def = false);

	virtual void set_value(const String &key, const Variant &value);

	void parse_ini_file(const String &path);

	static Settings *get_singleton();

	Settings(const bool singleton = false);
	virtual ~Settings();

protected:
	std::map<String, Variant> _data;

private:
	static Settings *_singleton;
};

#endif