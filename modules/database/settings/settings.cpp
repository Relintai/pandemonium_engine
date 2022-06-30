#include "settings.h"

#include <stdio.h>
#include <stdlib.h>

Variant Settings::get_value(const String &key, const Variant &def) {
	std::map<String, Variant>::iterator e = _data.find(key);

	if (e != _data.end()) {
		return e->second;
	} else {
		return def;
	}
}
String Settings::get_value_string(const String &key, const String &def) {
	std::map<String, Variant>::iterator e = _data.find(key);

	if (e != _data.end()) {
		return e->second;
	} else {
		return def;
	}
}
int Settings::get_value_int(const String &key, const int def) {
	std::map<String, Variant>::iterator e = _data.find(key);

	if (e != _data.end()) {
		return e->second.to_int();
	} else {
		return def;
	}
}
float Settings::get_value_float(const String &key, const float def) {
	std::map<String, Variant>::iterator e = _data.find(key);

	if (e != _data.end()) {
		return e->second.to_float();
	} else {
		return def;
	}
}
double Settings::get_value_double(const String &key, const double def) {
	std::map<String, Variant>::iterator e = _data.find(key);

	if (e != _data.end()) {
		return e->second.to_float();
	} else {
		return def;
	}
}
bool Settings::get_value_bool(const String &key, const bool def) {
	std::map<String, Variant>::iterator e = _data.find(key);

	if (e != _data.end()) {
		return e->second.to_bool();
	} else {
		return def;
	}
}

void Settings::set_value(const String &key, const Variant &value) {
	_data[key] = value;
}

void Settings::parse_ini_file(const String &path) {
	FILE *f = fopen(path.c_str(), "r");

	if (!f) {
		printf("Settings::parse_file: Error opening file!\n");
		return;
	}

	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET); /* same as rewind(f); */

	String config_str;
	config_str.resize(fsize);

	fread(config_str.dataw(), 1, fsize, f);
	fclose(f);

	config_str.replace('\r', ' ');
	Vector<String> ns = config_str.split('\n');

	for (int i = 0; i < ns.size(); ++i) {
		String s = ns[i];

		s.trim();

		if (s.size() == 0) {
			continue;
		}

		if (s[0] == '#') {
			continue;
		}

		int eindex = s.find('=');

		if (eindex == -1 || eindex == s.size() - 1) {
			continue;
		}

		String k = s.substr_index(0, eindex - 1);
		String v = s.substr_index(eindex + 1, s.size() - 1);

		_data[k] = Variant::parse_string(v);
	}
}

Settings *Settings::get_singleton() {
	return _singleton;
}

Settings::Settings(const bool singleton) {
	if (singleton) {
		if (_singleton) {
			printf("Settings singleton overridden!\n");
		}

		_singleton = this;
	}
}

Settings::~Settings() {
	if (_singleton == this) {
		_singleton = nullptr;
	}
}

Settings *Settings::_singleton = nullptr;