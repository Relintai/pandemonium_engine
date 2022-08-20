#include "html_parser.h"

#include "core/error/error_macros.h"
#include "core/log/logger.h"
#include "core/object/class_db.h"

String HTMLParserAttribute::get_attribute() {
	return _attribute;
}
void HTMLParserAttribute::set_attribute(const String &val) {
	_attribute = val;
}

String HTMLParserAttribute::get_data() {
	return _data;
}
void HTMLParserAttribute::set_data(const String &val) {
	_data = val;
}

bool HTMLParserAttribute::get_single() {
	return _single;
}
void HTMLParserAttribute::set_single(const bool &val) {
	_single = val;
}

bool HTMLParserAttribute::match_attrib(const String &attrib) {
	return _attribute == attrib;
}
bool HTMLParserAttribute::match_data(const String &d) {
	return _data == d;
}
bool HTMLParserAttribute::match_all_data(const Vector<String> &d) {
	// todo
	return false;
}
bool HTMLParserAttribute::match_all_data_bind(const PoolStringArray &d) {
	// todo
	return false;
}
bool HTMLParserAttribute::contains_data(const String &d) {
	return _data.find(d) != -1;
}

String HTMLParserAttribute::convert_to_string() const {
	if (_single) {
		return _attribute;
	}

	if (_data.find("\"") == -1) {
		return _attribute + "=\"" + _data + "\"";
	} else {
		return _attribute + "=\'" + _data + "\'";
	}
}

void HTMLParserAttribute::print() const {
	PLOG_MSG(convert_to_string());
}

HTMLParserAttribute::HTMLParserAttribute() {
	_single = false;
}

HTMLParserAttribute::~HTMLParserAttribute() {
}

void HTMLParserAttribute::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_attribute"), &HTMLParserAttribute::get_attribute);
	ClassDB::bind_method(D_METHOD("set_attribute", "val"), &HTMLParserAttribute::set_attribute);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "attribute"), "set_attribute", "get_attribute");

	ClassDB::bind_method(D_METHOD("get_data"), &HTMLParserAttribute::get_data);
	ClassDB::bind_method(D_METHOD("set_data", "val"), &HTMLParserAttribute::set_data);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "data"), "set_data", "get_data");

	ClassDB::bind_method(D_METHOD("get_single"), &HTMLParserAttribute::get_single);
	ClassDB::bind_method(D_METHOD("set_single", "val"), &HTMLParserAttribute::set_single);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "single"), "set_single", "get_single");

	ClassDB::bind_method(D_METHOD("match_attrib", "attrib"), &HTMLParserAttribute::match_attrib);
	ClassDB::bind_method(D_METHOD("match_data", "data"), &HTMLParserAttribute::match_data);
	ClassDB::bind_method(D_METHOD("match_all_data", "data"), &HTMLParserAttribute::match_all_data_bind);
	ClassDB::bind_method(D_METHOD("contains_data", "data"), &HTMLParserAttribute::contains_data);

	ClassDB::bind_method(D_METHOD("convert_to_string"), &HTMLParserAttribute::convert_to_string);
	ClassDB::bind_method(D_METHOD("print"), &HTMLParserAttribute::print);
};

int HTMLParserTag::get_type() {
	return _type;
}
void HTMLParserTag::set_type(const int &val) {
	_type = val;
}

String HTMLParserTag::get_tag() {
	return _tag;
}
void HTMLParserTag::set_tag(const String &val) {
	_tag = val;
}

String HTMLParserTag::get_data() {
	return _data;
}
void HTMLParserTag::set_data(const String &val) {
	_data = val;
}

void HTMLParserTag::add_child_tag(const Ref<HTMLParserTag> &tag) {
	_tags.push_back(tag);
}
void HTMLParserTag::remote_child_tag(const int index) {
	ERR_FAIL_INDEX(index, _tags.size());

	_tags.remove(index);
}
Ref<HTMLParserTag> HTMLParserTag::get_child_tag(const int index) {
	ERR_FAIL_INDEX_V(index, _tags.size(), Ref<HTMLParserTag>());

	return _tags[index];
}
int HTMLParserTag::get_child_tag_count() const {
	return _tags.size();
}
void HTMLParserTag::clear_child_tags() {
	_tags.clear();
}

Vector<Variant> HTMLParserTag::get_child_tags() {
	Vector<Variant> r;
	for (int i = 0; i < _tags.size(); i++) {
		r.push_back(_tags[i].get_ref_ptr());
	}
	return r;
}

void HTMLParserTag::set_child_tags(const Vector<Variant> &val) {
	_tags.clear();
	for (int i = 0; i < val.size(); i++) {
		Ref<HTMLParserAttribute> e = Ref<HTMLParserAttribute>(val[i]);
		_tags.push_back(e);
	}
}

void HTMLParserTag::add_child_attribute(const Ref<HTMLParserAttribute> &tag) {
	_attributes.push_back(tag);
}
void HTMLParserTag::remote_child_attribute(const int index) {
	ERR_FAIL_INDEX(index, _tags.size());

	_attributes.remove(index);
}
Ref<HTMLParserAttribute> HTMLParserTag::get_child_attribute(const int index) {
	ERR_FAIL_INDEX_V(index, _tags.size(), Ref<HTMLParserAttribute>());

	return _attributes[index];
}
int HTMLParserTag::get_child_attribute_count() const {
	return _attributes.size();
}
void HTMLParserTag::clear_child_attributes() {
	_attributes.clear();
}

Vector<Variant> HTMLParserTag::get_attributes() {
	Vector<Variant> r;
	for (int i = 0; i < _attributes.size(); i++) {
		r.push_back(_attributes[i].get_ref_ptr());
	}
	return r;
}

void HTMLParserTag::set_attributes(const Vector<Variant> &val) {
	_attributes.clear();
	for (int i = 0; i < val.size(); i++) {
		Ref<HTMLParserAttribute> e = Ref<HTMLParserAttribute>(val[i]);
		_attributes.push_back(e);
	}
}

Ref<HTMLParserTag> HTMLParserTag::get_first(const String &t) {
	if (_tag == t) {
		return Ref<HTMLParserTag>(this);
	}

	for (int i = 0; i < _tags.size(); ++i) {
		Ref<HTMLParserTag> ht = _tags.write[i]->get_first(t);

		if (ht.is_valid()) {
			return ht;
		}
	}

	return Ref<HTMLParserTag>();
}

Ref<HTMLParserTag> HTMLParserTag::get_firstc(const String &t, const String &attrib, const String &val) {
	if (_tag == t) {
		if (has_attributec(attrib, val)) {
			return Ref<HTMLParserTag>(this);
		}
	}

	for (int i = 0; i < _tags.size(); ++i) {
		Ref<HTMLParserTag> ht = _tags.write[i]->get_firstc(t, attrib, val);

		if (ht.is_valid()) {
			return ht;
		}
	}

	return Ref<HTMLParserTag>();
}

String HTMLParserTag::get_attribute_value(const String &attrib) {
	Ref<HTMLParserAttribute> a = get_attribute(attrib);

	if (a.is_valid()) {
		return a->get_data();
	}

	return "";
}

Ref<HTMLParserAttribute> HTMLParserTag::get_attribute(const String &attrib) {
	for (int i = 0; i < _attributes.size(); ++i) {
		Ref<HTMLParserAttribute> a = _attributes[i];

		if (a->match_attrib(attrib)) {
			return a;
		}
	}

	return Ref<HTMLParserAttribute>();
}

bool HTMLParserTag::has_attribute(const String &attrib) {
	for (int i = 0; i < _attributes.size(); ++i) {
		Ref<HTMLParserAttribute> a = _attributes[i];

		if (a->match_attrib(attrib)) {
			return true;
		}
	}

	return false;
}

Ref<HTMLParserAttribute> HTMLParserTag::get_attributec(const String &attrib, const String &contains_val) {
	for (int i = 0; i < _attributes.size(); ++i) {
		Ref<HTMLParserAttribute> a = _attributes[i];

		if (a->match_attrib(attrib) && a->contains_data(contains_val)) {
			return a;
		}
	}

	return Ref<HTMLParserAttribute>();
}

bool HTMLParserTag::has_attributec(const String &attrib, const String &contains_val) {
	for (int i = 0; i < _attributes.size(); ++i) {
		Ref<HTMLParserAttribute> a = _attributes[i];

		if (a->match_attrib(attrib) && a->contains_data(contains_val)) {
			return true;
		}
	}

	return false;
}

void HTMLParserTag::process() {
	if (_type != HTMLParserTag::HTML_PARSER_TAG_TYPE_NONE) {
		return;
	}

	if (_data.length() < 2) {
		return;
	}

	ERR_FAIL_COND(_data[0] != '<');
	ERR_FAIL_COND(_data[_data.length() - 1] != '>');

	int start_index = 1;
	if (_data[1] == '/') {
		++start_index;

		_type = HTMLParserTag::HTML_PARSER_TAG_TYPE_CLOSING_TAG;
	} else if (_data[1] == '!') {
		if (_data.length() < 8) {
			return;
		}

		// test for comment. <!-- -->
		++start_index;
		if (_data[2] == '-' && _data[3] == '-') {
			_type = HTMLParserTag::HTML_PARSER_TAG_TYPE_COMMENT;

			int comment_start_index = _data.find_char(' ', 3);

			if (comment_start_index == -1) {
				comment_start_index = 4;
			}

			_tag = _data.substr(comment_start_index, _data.length() - comment_start_index - 3);
		}

		if (_data.length() < 11) {
			return;
		}

		// test for doctype. <!doctype >
		if (_data.substr(2, 8).to_lower() != "doctype ") {
			return;
		}

		_type = HTMLParserTag::HTML_PARSER_TAG_TYPE_DOCTYPE;

		_tag = _data.substr(2 + 8, _data.length() - 2 - 8 - 1);
	} else {
		String tag_text;

		if (_data[_data.length() - 2] == '/') {
			// will catch all that looks like <br/>
			// tags that look like <br> will be caught later in a post process, in a way
			// which also tries to catch erroneously not closed tags that supposed to be closed
			_type = HTMLParserTag::HTML_PARSER_TAG_TYPE_SELF_CLOSING_TAG;

			tag_text = _data.substr(1, _data.length() - 3);
		} else {
			_type = HTMLParserTag::HTML_PARSER_TAG_TYPE_OPENING_TAG;

			tag_text = _data.substr(1, _data.length() - 2);
		}

		int fspc_index = tag_text.find_char(' ');

		if (fspc_index == -1) {
			// no args
			_tag = tag_text;
			return;
		}

		// grab the tag itself
		_tag = tag_text.substr(0, fspc_index);

		if (fspc_index + 1 == tag_text.length()) {
			// no args, but had a space like <br />
			return;
		}

		String args = tag_text.substr(fspc_index + 1, tag_text.length() - fspc_index - 1);
		parse_args(args);
	}

	int tag_end_index = _data.find_char(' ', start_index);

	if (tag_end_index == -1) {
		// simple tag
		_tag = _data.substr(start_index, _data.length() - start_index - 1);
		return;
	}
}

void HTMLParserTag::parse_args(const String &args) {
	_attributes.clear();

	int i = 0;
	while (i < args.length()) {
		if (args[i] == ' ') {
			//"trim"
			++i;
			continue;
		}

		int equals_index = args.find_char('=', i);

		Ref<HTMLParserAttribute> a;
		a.instance();

		if (equals_index == -1) {
			a->set_attribute(args.substr(i, args.length() - i));
			a->set_single(true);
			_attributes.push_back(a);

			return;
		}

		a->set_attribute(args.substr(i, equals_index - i));

		// todo
		// a.trim();

		int next_char_index = equals_index + 1;

		if (next_char_index >= args.length()) {
			// an attribute looks like this "... attrib="
			_attributes.push_back(a);
			return;
		}

		// skip spaces
		while (args[next_char_index] == ' ') {
			++next_char_index;

			if (next_char_index >= args.length()) {
				// an attribute looks like this "... attrib=     "
				_attributes.push_back(a);
				return;
			}
		}

		char c = args[next_char_index];
		char find_char = ' ';

		if (c == '"' || c == '\'') {
			++next_char_index;
			find_char = c;
		}

		int end_index = args.find_char(find_char, next_char_index);

		if (end_index == -1) {
			// missing closing ' or " if c is ' or "
			// else missing parameter

			a->set_data(args.substr(next_char_index, args.length() - next_char_index - 1));
			_attributes.push_back(a);
			return;
		}

		a->set_data(args.substr(next_char_index, end_index - next_char_index));
		_attributes.push_back(a);

		i = end_index + 1;
	}
}

String HTMLParserTag::convert_to_string(const int level) const {
	String s;

	s += String(" ").repeat(level);

	if (_type == HTML_PARSER_TAG_TYPE_CONTENT) {
		s += _data + "\n";

		if (_tags.size() != 0) {
			s += String(" ").repeat(level);
			s += "(!CONTENT TAG HAS TAGS!)\n";

			for (int i = 0; i < _tags.size(); ++i) {
				s += _tags[i]->convert_to_string(level + 1) + "\n";
			}
		}
	} else if (_type == HTML_PARSER_TAG_TYPE_OPENING_TAG) {
		int ln = level + 1;

		s += "<" + _tag;

		for (int i = 0; i < _attributes.size(); ++i) {
			s += " " + _attributes[i]->convert_to_string();
		}

		s += ">\n";

		for (int i = 0; i < _tags.size(); ++i) {
			s += _tags[i]->convert_to_string(ln);
		}

		s += String(" ").repeat(level);

		s += "</" + _tag + ">\n";
	} else if (_type == HTML_PARSER_TAG_TYPE_CLOSING_TAG) {
		// HTMLParserTag should handle this automatically
		// it's here for debugging purposes though
		s += "</" + _tag + "(!)>";

		if (_tags.size() != 0) {
			s += String(" ").repeat(level);
			s += "(!CLOSING TAG HAS TAGS!)\n";

			for (int i = 0; i < _tags.size(); ++i) {
				s += _tags[i]->convert_to_string(level + 1) + "\n";
			}
		}
	} else if (_type == HTML_PARSER_TAG_TYPE_SELF_CLOSING_TAG) {
		s += "<" + _tag;

		for (int i = 0; i < _attributes.size(); ++i) {
			s += " " + _attributes[i]->convert_to_string();
		}

		s += "/>\n";

		if (_tags.size() != 0) {
			s += String(" ").repeat(level);
			s += "(!SELF CLOSING TAG HAS TAGS!)\n";

			for (int i = 0; i < _tags.size(); ++i) {
				s += _tags[i]->convert_to_string(level + 1) + "\n";
			}
		}
	} else if (_type == HTML_PARSER_TAG_TYPE_COMMENT) {
		s += "<!-- " + _data + " -->\n";

		if (_tags.size() != 0) {
			s += String(" ").repeat(level);
			s += "(!COMMENT TAG HAS TAGS!)\n";

			for (int i = 0; i < _tags.size(); ++i) {
				s += _tags[i]->convert_to_string(level + 1) + "\n";
			}
		}
	} else if (_type == HTML_PARSER_TAG_TYPE_DOCTYPE) {
		s += _data + "\n";

		if (_tags.size() != 0) {
			s += String(" ").repeat(level);
			s += "(!DOCTYPE TAG HAS TAGS!)\n";

			for (int i = 0; i < _tags.size(); ++i) {
				s += _tags[i]->convert_to_string(level + 1) + "\n";
			}
		}
	} else if (_type == HTML_PARSER_TAG_TYPE_NONE) {
		s += _data + "\n";
		for (int i = 0; i < _tags.size(); ++i) {
			s += _tags[i]->convert_to_string(level) + "\n";
			s += String(" ").repeat(level);
		}
	}

	return s;
}
void HTMLParserTag::print() const {
	PLOG_MSG(convert_to_string());
}

HTMLParserTag::HTMLParserTag() {
	_type = HTMLParserTag::HTML_PARSER_TAG_TYPE_NONE;
}

HTMLParserTag::~HTMLParserTag() {
	_tags.clear();
	_attributes.clear();
}

void HTMLParserTag::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_type"), &HTMLParserTag::get_type);
	ClassDB::bind_method(D_METHOD("set_type", "val"), &HTMLParserTag::set_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "type"), "set_type", "get_type");

	ClassDB::bind_method(D_METHOD("get_tag"), &HTMLParserTag::get_tag);
	ClassDB::bind_method(D_METHOD("set_tag", "val"), &HTMLParserTag::set_tag);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "tag"), "set_tag", "get_tag");

	ClassDB::bind_method(D_METHOD("get_data"), &HTMLParserTag::get_data);
	ClassDB::bind_method(D_METHOD("set_data", "val"), &HTMLParserTag::set_data);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "data"), "set_data", "get_data");

	ClassDB::bind_method(D_METHOD("add_child_tag", "tag"), &HTMLParserTag::add_child_tag);
	ClassDB::bind_method(D_METHOD("remote_child_tag", "index"), &HTMLParserTag::remote_child_tag);
	ClassDB::bind_method(D_METHOD("get_child_tag", "index"), &HTMLParserTag::get_child_tag);
	ClassDB::bind_method(D_METHOD("get_child_tag_count"), &HTMLParserTag::get_child_tag_count);
	ClassDB::bind_method(D_METHOD("clear_child_tags"), &HTMLParserTag::clear_child_tags);

	ClassDB::bind_method(D_METHOD("get_child_tags"), &HTMLParserTag::get_child_tags);
	ClassDB::bind_method(D_METHOD("set_child_tags", "val"), &HTMLParserTag::set_child_tags);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "child_tags", PROPERTY_HINT_NONE, "23/19:HTMLParserTag", PROPERTY_USAGE_DEFAULT, "HTMLParserTag"), "set_child_tags", "get_child_tags");

	ClassDB::bind_method(D_METHOD("add_child_attribute", "tag"), &HTMLParserTag::add_child_attribute);
	ClassDB::bind_method(D_METHOD("remote_child_attribute", "index"), &HTMLParserTag::remote_child_attribute);
	ClassDB::bind_method(D_METHOD("get_child_attribute", "index"), &HTMLParserTag::get_child_attribute);
	ClassDB::bind_method(D_METHOD("get_child_attribute_count"), &HTMLParserTag::get_child_attribute_count);
	ClassDB::bind_method(D_METHOD("clear_child_attributes"), &HTMLParserTag::clear_child_attributes);

	ClassDB::bind_method(D_METHOD("get_attributes"), &HTMLParserTag::get_attributes);
	ClassDB::bind_method(D_METHOD("set_attributes", "val"), &HTMLParserTag::set_attributes);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "attributes", PROPERTY_HINT_NONE, "23/19:HTMLParserAttribute", PROPERTY_USAGE_DEFAULT, "HTMLParserAttribute"), "set_attributes", "get_attributes");

	ClassDB::bind_method(D_METHOD("get_first", "t"), &HTMLParserTag::get_first);
	ClassDB::bind_method(D_METHOD("get_firstc", "t", "attrib", "val"), &HTMLParserTag::get_firstc);

	ClassDB::bind_method(D_METHOD("get_attribute_value", "attrib"), &HTMLParserTag::get_attribute_value);

	ClassDB::bind_method(D_METHOD("get_attribute", "attrib"), &HTMLParserTag::get_attribute);
	ClassDB::bind_method(D_METHOD("has_attribute", "attrib"), &HTMLParserTag::has_attribute);

	ClassDB::bind_method(D_METHOD("get_attributec", "attrib", "contains_val"), &HTMLParserTag::get_attributec);
	ClassDB::bind_method(D_METHOD("has_attributec", "attrib", "contains_val"), &HTMLParserTag::has_attributec);

	ClassDB::bind_method(D_METHOD("process"), &HTMLParserTag::process);
	ClassDB::bind_method(D_METHOD("parse_args", "args"), &HTMLParserTag::parse_args);

	ClassDB::bind_method(D_METHOD("convert_to_string", "level"), &HTMLParserTag::convert_to_string, 0);
	ClassDB::bind_method(D_METHOD("print"), &HTMLParserTag::print);

	BIND_ENUM_CONSTANT(HTML_PARSER_TAG_TYPE_NONE);
	BIND_ENUM_CONSTANT(HTML_PARSER_TAG_TYPE_OPENING_TAG);
	BIND_ENUM_CONSTANT(HTML_PARSER_TAG_TYPE_CLOSING_TAG);
	BIND_ENUM_CONSTANT(HTML_PARSER_TAG_TYPE_SELF_CLOSING_TAG);
	BIND_ENUM_CONSTANT(HTML_PARSER_TAG_TYPE_COMMENT);
	BIND_ENUM_CONSTANT(HTML_PARSER_TAG_TYPE_DOCTYPE);
	BIND_ENUM_CONSTANT(HTML_PARSER_TAG_TYPE_CONTENT);
}

Ref<HTMLParserTag> HTMLParser::get_root() {
	return _root;
}

void HTMLParser::parse(const String &data) {
	Vector<Ref<HTMLParserTag>> tags;

	// <script> content parsing is based on https://stackoverflow.com/questions/14574471/how-do-browsers-parse-a-script-tag-exactly
	const int STATE_NONE = 0;
	const int STATE_DATA_1 = 1;
	const int STATE_DATA_2 = 2;
	const int STATE_DATA_3 = 3;

	int state = STATE_NONE;

	// split into tags
	for (int i = 0; i < data.length(); ++i) {
		if (state == STATE_NONE) {
			if (data[i] == '<') {
				// tag

				if (data.is_word_at(i, "<script")) {
					// after the opening <script> tag, the parser goes to data1 state
					state = STATE_DATA_1;
					// no else, we need to process the tag istelf!
				}

				for (int j = i + 1; j < data.length(); ++j) {
					if (data[j] == '>') {
						Ref<HTMLParserTag> t;
						t.instance();

						t->set_data(data.substr(i, j - i + 1));
						t->process();

						tags.push_back(t);

						i = j;
						break;
					}
				}
			} else {
				// content

				for (int j = i + 1; j < data.length(); ++j) {
					if (data[j] == '<') {
						Ref<HTMLParserTag> t;
						t.instance();

						t->set_data(data.substr(i, j - i));
						t->set_type(HTMLParserTag::HTML_PARSER_TAG_TYPE_CONTENT);

						tags.push_back(t);

						i = j - 1;
						break;
					}
				}
			}
		} else {
			// script tag content

			bool done = false;
			for (int j = i; j < data.length(); ++j) {
				char c = data[j];

				if (c != '<' && c != '-') {
					continue;
				}

				if (data.is_word_at(j, "-->")) {
					// if --> is encountered while in any state, switch to data1 state
					state = STATE_DATA_1;
					continue;
				}

				if (state == STATE_DATA_1) {
					if (data.is_word_at(j, "<!--")) {
						// if <!-- is encountered while in data1 state, switch to data2 state
						state = STATE_DATA_2;
					} else if (data.is_word_at(j, "</script")) {
						// if </script[\s/>] is encountered while in any other state (than data3), stop parsing
						done = true;
					}

				} else if (state == STATE_DATA_2) {
					if (data.is_word_at(j, "<script")) {
						// if <script[\s/>] is encountered while in data2 state, switch to data3 state
						state = STATE_DATA_3;
					} else if (data.is_word_at(j, "</script")) {
						// if </script[\s/>] is encountered while in any other state (than data3), stop parsing
						done = true;
					}

				} else if (state == STATE_DATA_3) {
					// if </script[\s/>] is encountered while in data3 state, switch to data2 state
					if (data.is_word_at(j, "</script")) {
						state = STATE_DATA_2;
					}
				}

				if (done) {
					state = STATE_NONE;
					Ref<HTMLParserTag> t;
					t.instance();

					t->set_data(data.substr(i, j - i));
					t->set_type(HTMLParserTag::HTML_PARSER_TAG_TYPE_CONTENT);

					tags.push_back(t);

					i = j - 1;
					break;
				}
			}
		}
	}

	_root.instance();

	// process tags into hierarchical order
	Vector<Ref<HTMLParserTag>> tag_stack;
	for (int i = 0; i < tags.size(); ++i) {
		Ref<HTMLParserTag> t = tags[i];

		ERR_CONTINUE(!t.is_valid());

		if (t->get_type() == HTMLParserTag::HTML_PARSER_TAG_TYPE_NONE) {
			ERR_PRINT("HTMLParser::parse: t->type == HTMLParserTag::HTML_PARSER_TAG_TYPE_NONE! Tag content:");
			ERR_PRINT(tags[i]->convert_to_string());

			//memdelete(t);
			tags.write[i].unref();
			continue;
		} else if (t->get_type() == HTMLParserTag::HTML_PARSER_TAG_TYPE_OPENING_TAG) {
			tag_stack.push_back(t);

			tags.write[i].unref();
			continue;
		} else if (t->get_type() == HTMLParserTag::HTML_PARSER_TAG_TYPE_SELF_CLOSING_TAG) {
			if (tag_stack.size() == 0) {
				_root->add_child_tag(t);
			} else {
				tag_stack.write[tag_stack.size() - 1]->add_child_tag(t);
			}

			tags.write[i].unref();
			continue;
		} else if (t->get_type() == HTMLParserTag::HTML_PARSER_TAG_TYPE_CONTENT) {
			if (tag_stack.size() == 0) {
				_root->add_child_tag(t);
			} else {
				tag_stack.write[tag_stack.size() - 1]->add_child_tag(t);
			}

			tags.write[i].unref();
			continue;
		} else if (t->get_type() == HTMLParserTag::HTML_PARSER_TAG_TYPE_COMMENT) {
			if (tag_stack.size() == 0) {
				_root->add_child_tag(t);
			} else {
				tag_stack.write[tag_stack.size() - 1]->add_child_tag(t);
			}

			tags.write[i].unref();
			continue;
		} else if (t->get_type() == HTMLParserTag::HTML_PARSER_TAG_TYPE_DOCTYPE) {
			if (tag_stack.size() == 0) {
				_root->add_child_tag(t);
			} else {
				tag_stack.write[tag_stack.size() - 1]->add_child_tag(t);
			}

			tags.write[i].unref();
			continue;
		} else if (t->get_type() == HTMLParserTag::HTML_PARSER_TAG_TYPE_CLOSING_TAG) {
			if (tag_stack.size() == 0) {
				tags.write[i].unref();

				// ill-formed html
				continue;
			}

			// find it's pair
			int tag_index = 0;
			for (int j = tag_stack.size() - 1; j > 0; --j) {
				Ref<HTMLParserTag> ts = tag_stack[j];

				// we sould only have opening tags on the stack
				if (ts->get_tag() == t->get_tag()) {
					// found
					tag_index = j;
					break;
				}
			}

			Ref<HTMLParserTag> opening_tag = tag_stack[tag_index];

			// mark everything else that we found before finding the opening tag as self closing, and add them to out opening tag
			// If the html is ill formed, it just grabs everything from the tag stack
			for (int j = tag_index + 1; j < tag_stack.size(); ++j) {
				Ref<HTMLParserTag> ts = tag_stack[j];

				ts->set_type(HTMLParserTag::HTML_PARSER_TAG_TYPE_SELF_CLOSING_TAG);
				opening_tag->add_child_tag(ts);
			}

			tag_stack.resize(tag_index);

			if (tag_stack.size() == 0) {
				_root->add_child_tag(opening_tag);
			} else {
				tag_stack.write[tag_stack.size() - 1]->add_child_tag(opening_tag);
			}

			tags.write[i].unref();

			continue;
		}
	}

	// add everything remaining on the stack to _root
	for (int i = 0; i < tag_stack.size(); ++i) {
		_root->add_child_tag(tag_stack[i]);
	}

	for (int i = 0; i < tags.size(); ++i) {
		Ref<HTMLParserTag> t = tags[i];

		if (t.is_valid()) {
			ERR_PRINT("HTMLParser::parse(const String &data): tag was not processed!\n");
			t->print();
		}
	}
}

String HTMLParser::convert_to_string() const {
	if (!_root.is_valid()) {
		return "";
	}

	return _root->convert_to_string();
}
void HTMLParser::print() const {
	if (_root.is_valid()) {
		_root->print();
	}
}

HTMLParser::HTMLParser() {
}

HTMLParser::~HTMLParser() {
	_root.unref();
}

void HTMLParser::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_root"), &HTMLParser::get_root);
	ClassDB::bind_method(D_METHOD("parse", "data"), &HTMLParser::parse);
	ClassDB::bind_method(D_METHOD("convert_to_string"), &HTMLParser::convert_to_string);
	ClassDB::bind_method(D_METHOD("print"), &HTMLParser::print);
};
