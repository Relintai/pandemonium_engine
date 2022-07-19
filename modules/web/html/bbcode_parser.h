#ifndef BBCODE_PARSER_H
#define BBCODE_PARSER_H

#include "core/ustring.h"
#include "core/vector.h"

#include "core/reference.h"

class BBCodeParserAttribute : public Reference {
	GDCLASS(BBCodeParserAttribute, Reference);

public:
	String attribute;
	String data;
	bool single;

	bool match_attrib(const String &attrib);
	bool match_data(const String &d);
	bool match_data(const Vector<String> &d);
	bool contains_data(const String &d);

	String convert_to_string() const;
	void print() const;

	BBCodeParserAttribute();
	virtual ~BBCodeParserAttribute();
};

class BBCodeParserTag : public Reference {
	GDCLASS(BBCodeParserTag, Reference);

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

	Vector<Ref<BBCodeParserTag>> tags;
	Vector<Ref<BBCodeParserAttribute>> attributes;

	Ref<BBCodeParserTag> get_first(const String &t);
	Ref<BBCodeParserTag> get_first(const String &t, const String &attrib, const String &val);

	String get_attribute_value(const String &attrib);

	Ref<BBCodeParserAttribute> get_attribute(const String &attrib);
	bool has_attribute(const String &attrib);

	Ref<BBCodeParserAttribute> get_attribute(const String &attrib, const String &contains_val);
	bool has_attribute(const String &attrib, const String &contains_val);

	void process();
	void parse_args(const String &args);

	String convert_to_string(const int level = 0) const;
	void print() const;

	BBCodeParserTag();
	virtual ~BBCodeParserTag();
};

class BBCodeParser : public Reference {
	GDCLASS(BBCodeParser, Reference);

public:
	Ref<BBCodeParserTag> root;

	void parse(const String &data);
	//void parse_tag(const String &data, const int index);

	String convert_to_string() const;
	void print() const;

	BBCodeParser();
	virtual ~BBCodeParser();
};

#endif
