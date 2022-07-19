#ifndef HTML_PARSER_H
#define HTML_PARSER_H

#include "core/ustring.h"
#include "core/vector.h"

#include "core/reference.h"

class HTMLParserAttribute : public Reference {
	GDCLASS(HTMLParserAttribute, Reference);

public:
	String get_attribute();
	void set_attribute(const String &val);

	String get_data();
	void set_data(const String &val);

	bool get_single();
	void set_single(const bool &val);

	bool match_attrib(const String &attrib);
	bool match_data(const String &d);
	bool match_data(const Vector<String> &d);
	bool contains_data(const String &d);

	String convert_to_string() const;
	void print() const;

	HTMLParserAttribute();
	virtual ~HTMLParserAttribute();

protected:
	String _attribute;
	String _data;
	bool _single;
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

	int get_type();
	void set_type(const int &val);

	String get_tag();
	void set_tag(const String &val);

	String get_data();
	void set_data(const String &val);

	void add_child_tag(const Ref<HTMLParserTag> &tag);
	void remote_child_tag(const int index);
	Ref<HTMLParserTag> get_child_tag(const int index);
	int get_child_tag_count() const;
	void clear_child_tags();

	Vector<Variant> get_child_tags();
	void set_child_tags(const Vector<Variant> &val);

	void add_child_attribute(const Ref<HTMLParserAttribute> &tag);
	void remote_child_attribute(const int index);
	Ref<HTMLParserAttribute> get_child_attribute(const int index);
	int get_child_attribute_count() const;
	void clear_child_attributes();

	Vector<Variant> get_attributes();
	void set_attributes(const Vector<Variant> &val);

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

protected:
	int _type;

	String _tag;
	String _data;

	Vector<Ref<HTMLParserTag>> _tags;
	Vector<Ref<HTMLParserAttribute>> _attributes;
};

class HTMLParser : public Reference {
	GDCLASS(HTMLParser, Reference);

public:
	Ref<HTMLParserTag> get_root();

	void parse(const String &data);
	//void parse_tag(const String &data, const int index);

	String convert_to_string() const;
	void print() const;

	HTMLParser();
	virtual ~HTMLParser();

protected:
	Ref<HTMLParserTag> _root;
};

#endif
