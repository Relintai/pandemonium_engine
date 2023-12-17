#ifndef BBCODE_PARSER_H
#define BBCODE_PARSER_H

/*************************************************************************/
/*  bbcode_parser.h                                                      */
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

#include "core/object/reference.h"

class BBCodeParserAttribute : public Reference {
	GDCLASS(BBCodeParserAttribute, Reference);

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

	BBCodeParserAttribute();
	virtual ~BBCodeParserAttribute();

protected:
	static void _bind_methods();

	String _attribute;
	String _data;
	bool _single;
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

	int get_type();
	void set_type(const int &val);

	String get_tag();
	void set_tag(const String &val);

	String get_data();
	void set_data(const String &val);

	void add_child_tag(const Ref<BBCodeParserTag> &tag);
	void remote_child_tag(const int index);
	Ref<BBCodeParserTag> get_child_tag(const int index);
	int get_child_tag_count() const;
	void clear_child_tags();

	Vector<Variant> get_child_tags();
	void set_child_tags(const Vector<Variant> &val);

	void add_child_attribute(const Ref<BBCodeParserAttribute> &tag);
	void remote_child_attribute(const int index);
	Ref<BBCodeParserAttribute> get_child_attribute(const int index);
	int get_child_attribute_count() const;
	void clear_child_attributes();

	Vector<Variant> get_attributes();
	void set_attributes(const Vector<Variant> &val);

	Ref<BBCodeParserTag> get_first(const String &t);
	Ref<BBCodeParserTag> get_firstc(const String &t, const String &attrib, const String &val);

	String get_attribute_value(const String &attrib);

	Ref<BBCodeParserAttribute> get_attribute(const String &attrib);
	bool has_attribute(const String &attrib);

	Ref<BBCodeParserAttribute> get_attributec(const String &attrib, const String &contains_val);
	bool has_attributec(const String &attrib, const String &contains_val);

	void process();
	void parse_args(const String &args);

	String convert_to_string(const int level = 0) const;
	void print() const;

	BBCodeParserTag();
	virtual ~BBCodeParserTag();

protected:
	static void _bind_methods();

	int _type;

	String _tag;
	String _data;

	Vector<Ref<BBCodeParserTag>> _tags;
	Vector<Ref<BBCodeParserAttribute>> _attributes;
};

class BBCodeParser : public Reference {
	GDCLASS(BBCodeParser, Reference);

public:
	Ref<BBCodeParserTag> get_root();

	void parse(const String &data);
	//void parse_tag(const String &data, const int index);

	String convert_to_string() const;
	void print() const;

	BBCodeParser();
	virtual ~BBCodeParser();

protected:
	static void _bind_methods();

	Ref<BBCodeParserTag> _root;
};

VARIANT_ENUM_CAST(BBCodeParserTag::BBCodeParserTagType);

#endif
