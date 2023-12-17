#ifndef HTML_PARSER_H
#define HTML_PARSER_H

/*************************************************************************/
/*  html_parser.h                                                        */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "core/containers/vector.h"
#include "core/string/ustring.h"
#include "core/variant/variant.h"

#include "core/object/reference.h"

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
	bool match_all_data(const Vector<String> &d);
	bool match_all_data_bind(const PoolStringArray &d);
	bool contains_data(const String &d);

	String convert_to_string() const;
	void print() const;

	HTMLParserAttribute();
	virtual ~HTMLParserAttribute();

protected:
	static void _bind_methods();

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
	Ref<HTMLParserTag> get_firstc(const String &t, const String &attrib, const String &val);

	String get_attribute_value(const String &attrib);

	Ref<HTMLParserAttribute> get_attribute(const String &attrib);
	bool has_attribute(const String &attrib);

	Ref<HTMLParserAttribute> get_attributec(const String &attrib, const String &contains_val);
	bool has_attributec(const String &attrib, const String &contains_val);

	void process();
	void parse_args(const String &args);

	String convert_to_string(const int level = 0) const;
	void print() const;

	HTMLParserTag();
	virtual ~HTMLParserTag();

protected:
	static void _bind_methods();

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
	static void _bind_methods();

	Ref<HTMLParserTag> _root;
};

VARIANT_ENUM_CAST(HTMLParserTag::HTMLParserTagType);

#endif
