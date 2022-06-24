#ifndef BBCODE_PARSER_H
#define BBCODE_PARSER_H

#include "core/containers/vector.h"
#include "core/string.h"

class BBCodeParserAttribute {
public:
	String attribute;
	String data;
	bool single;

	bool match_attrib(const String &attrib);
	bool match_data(const String &d);
	bool match_data(const Vector<String> &d);
	bool contains_data(const String &d);

	String to_string();
	void print();

	BBCodeParserAttribute();
	virtual ~BBCodeParserAttribute();
};

class BBCodeParserTag {
public:
	enum BBCodeParserTagType {
		BBCODE_PARSER_TAG_TYPE_NONE = 0,
		BBCODE_PARSER_TAG_TYPE_OPENING_TAG,
		BBCODE_PARSER_TAG_TYPE_CLOSING_TAG,
		BBCODE_PARSER_TAG_TYPE_SELF_CLOSING_TAG,
		BBCODE_PARSER_TAG_TYPE_CONTENT
	};

	int type;

	String tag;
	String data;

	Vector<BBCodeParserTag*> tags;
	Vector<BBCodeParserAttribute*> attributes;

	BBCodeParserTag *get_first(const String &t);
	BBCodeParserTag *get_first(const String &t, const String &attrib, const String &val);

	String get_attribute_value(const String &attrib);

	BBCodeParserAttribute *get_attribute(const String &attrib);
	bool has_attribute(const String &attrib);

	BBCodeParserAttribute *get_attribute(const String &attrib, const String &contains_val);
	bool has_attribute(const String &attrib, const String &contains_val);

	void process();
	void parse_args(const String &args);

	String to_string(const int level = 0);
	void print();

	BBCodeParserTag();
	virtual ~BBCodeParserTag();
};

class BBCodeParser {
public:
	BBCodeParserTag *root;

	void parse(const String &data);
	//void parse_tag(const String &data, const int index);

	String to_string();
	void print();

	BBCodeParser();
	virtual ~BBCodeParser();
};

#endif