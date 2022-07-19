#ifndef HTML_PARSER_H
#define HTML_PARSER_H

#include "core/ustring.h"
#include "core/vector.h"

#include "core/reference.h"

class HTMLParserAttribute : public Reference {
	GDCLASS(HTMLParserAttribute, Reference);

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

	HTMLParserAttribute();
	virtual ~HTMLParserAttribute();
};

class HTMLParserTag : public Reference {
	GDCLASS(HTMLParserTag, Reference);

public:
	enum HTMLParserTagType {
		HTML_PARSER_TAG_TYPE_NONE = 0,
		HTML_PARSER_TAG_TYPE_OPENING_TAG,
		HTML_PARSER_TAG_TYPE_CLOSING_TAG,
		HTML_PARSER_TAG_TYPE_SELF_CLOSING_TAG,
		HTML_PARSER_TAG_TYPE_COMMENT,
		HTML_PARSER_TAG_TYPE_DOCTYPE,
		HTML_PARSER_TAG_TYPE_CONTENT
	};

	int type;

	String tag;
	String data;

	Vector<Ref<HTMLParserTag>> tags;
	Vector<Ref<HTMLParserAttribute>> attributes;

	Ref<HTMLParserTag> get_first(const String &t);
	Ref<HTMLParserTag> get_first(const String &t, const String &attrib, const String &val);

	String get_attribute_value(const String &attrib);

	Ref<HTMLParserAttribute> get_attribute(const String &attrib);
	bool has_attribute(const String &attrib);

	Ref<HTMLParserAttribute> get_attribute(const String &attrib, const String &contains_val);
	bool has_attribute(const String &attrib, const String &contains_val);

	void process();
	void parse_args(const String &args);

	String convert_to_string(const int level = 0) const;
	void print() const;

	HTMLParserTag();
	virtual ~HTMLParserTag();
};

class HTMLParser : public Reference {
	GDCLASS(HTMLParser, Reference);

public:
	Ref<HTMLParserTag> root;

	void parse(const String &data);
	//void parse_tag(const String &data, const int index);

	String convert_to_string() const;
	void print() const;

	HTMLParser();
	virtual ~HTMLParser();
};

#endif
