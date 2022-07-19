#include "bbcode_parser.h"

#include "core/error_macros.h"
#include "core/log/logger.h"

String BBCodeParserAttribute::get_attribute() {
	return _attribute;
}
void BBCodeParserAttribute::set_attribute(const String &val) {
	_attribute = val;
}

String BBCodeParserAttribute::get_data() {
	return _data;
}
void BBCodeParserAttribute::set_data(const String &val) {
	_data = val;
}

bool BBCodeParserAttribute::get_single() {
	return _single;
}
void BBCodeParserAttribute::set_single(const bool &val) {
	_single = val;
}

bool BBCodeParserAttribute::match_attrib(const String &attrib) {
	return _attribute == attrib;
}
bool BBCodeParserAttribute::match_data(const String &d) {
	return _data == d;
}
bool BBCodeParserAttribute::match_all_data(const Vector<String> &d) {
	// todo
	return false;
}
bool BBCodeParserAttribute::match_all_data_bind(const PoolStringArray &d) {
	// todo
	return false;
}
bool BBCodeParserAttribute::contains_data(const String &d) {
	return _data.find(d) != -1;
}

String BBCodeParserAttribute::convert_to_string() const {
	if (_single) {
		return _attribute;
	}

	if (_data.find_char('"' == -1)) {
		return _attribute + "=\"" + _data + "\"";
	} else {
		return _attribute + "=\'" + _data + "\'";
	}
}

void BBCodeParserAttribute::print() const {
	PLOG_MSG(convert_to_string());
}

BBCodeParserAttribute::BBCodeParserAttribute() {
	_single = false;
}

BBCodeParserAttribute::~BBCodeParserAttribute() {
}

void BBCodeParserAttribute::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_attribute"), &BBCodeParserAttribute::get_attribute);
	ClassDB::bind_method(D_METHOD("set_attribute", "val"), &BBCodeParserAttribute::set_attribute);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "attribute"), "set_attribute", "get_attribute");

	ClassDB::bind_method(D_METHOD("get_data"), &BBCodeParserAttribute::get_data);
	ClassDB::bind_method(D_METHOD("set_data", "val"), &BBCodeParserAttribute::set_data);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "data"), "set_data", "get_data");

	ClassDB::bind_method(D_METHOD("get_single"), &BBCodeParserAttribute::get_single);
	ClassDB::bind_method(D_METHOD("set_single", "val"), &BBCodeParserAttribute::set_single);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "single"), "set_single", "get_single");

	ClassDB::bind_method(D_METHOD("match_attrib", "attrib"), &BBCodeParserAttribute::match_attrib);
	ClassDB::bind_method(D_METHOD("match_data", "data"), &BBCodeParserAttribute::match_data);
	ClassDB::bind_method(D_METHOD("match_all_data", "data"), &BBCodeParserAttribute::match_all_data_bind);
	ClassDB::bind_method(D_METHOD("contains_data", "data"), &BBCodeParserAttribute::contains_data);

	ClassDB::bind_method(D_METHOD("convert_to_string"), &BBCodeParserAttribute::convert_to_string);
	ClassDB::bind_method(D_METHOD("print"), &BBCodeParserAttribute::print);
};

void BBCodeParserTag::add_child_tag(const Ref<BBCodeParserTag> &tag) {
	_tags.push_back(tag);
}
void BBCodeParserTag::remote_child_tag(const int index) {
	ERR_FAIL_INDEX(index, _tags.size());

	_tags.remove(index);
}
Ref<BBCodeParserTag> BBCodeParserTag::get_child_tag(const int index) {
	ERR_FAIL_INDEX_V(index, _tags.size(), Ref<BBCodeParserTag>());

	return _tags[index];
}
int BBCodeParserTag::get_child_tag_count() const {
	return _tags.size();
}
void BBCodeParserTag::clear_child_tags() {
	_tags.clear();
}

Vector<Variant> BBCodeParserTag::get_child_tags() {
	Vector<Variant> r;
	for (int i = 0; i < _tags.size(); i++) {
		r.push_back(_tags[i].get_ref_ptr());
	}
	return r;
}

void BBCodeParserTag::set_child_tags(const Vector<Variant> &val) {
	_tags.clear();
	for (int i = 0; i < val.size(); i++) {
		Ref<BBCodeParserAttribute> e = Ref<BBCodeParserAttribute>(val[i]);
		_tags.push_back(e);
	}
}

void BBCodeParserTag::add_child_attribute(const Ref<BBCodeParserAttribute> &tag) {
	_attributes.push_back(tag);
}
void BBCodeParserTag::remote_child_attribute(const int index) {
	ERR_FAIL_INDEX(index, _tags.size());

	_attributes.remove(index);
}
Ref<BBCodeParserAttribute> BBCodeParserTag::get_child_attribute(const int index) {
	ERR_FAIL_INDEX_V(index, _tags.size(), Ref<BBCodeParserAttribute>());

	return _attributes[index];
}
int BBCodeParserTag::get_child_attribute_count() const {
	return _attributes.size();
}
void BBCodeParserTag::clear_child_attributes() {
	_attributes.clear();
}

Vector<Variant> BBCodeParserTag::get_attributes() {
	Vector<Variant> r;
	for (int i = 0; i < _attributes.size(); i++) {
		r.push_back(_attributes[i].get_ref_ptr());
	}
	return r;
}

void BBCodeParserTag::set_attributes(const Vector<Variant> &val) {
	_attributes.clear();
	for (int i = 0; i < val.size(); i++) {
		Ref<BBCodeParserAttribute> e = Ref<BBCodeParserAttribute>(val[i]);
		_attributes.push_back(e);
	}
}

Ref<BBCodeParserTag> BBCodeParserTag::get_first(const String &t) {
	if (_tag == t) {
		return Ref<BBCodeParserTag>(this);
	}

	for (int i = 0; i < _tags.size(); ++i) {
		Ref<BBCodeParserTag> ht = _tags.write[i]->get_first(t);

		if (ht.is_valid()) {
			return ht;
		}
	}

	return Ref<BBCodeParserTag>();
}

Ref<BBCodeParserTag> BBCodeParserTag::get_firstc(const String &t, const String &attrib, const String &val) {
	if (_tag == t) {
		if (has_attributec(attrib, val)) {
			return Ref<BBCodeParserTag>(this);
		}
	}

	for (int i = 0; i < _tags.size(); ++i) {
		Ref<BBCodeParserTag> ht = _tags.write[i]->get_firstc(t, attrib, val);

		if (ht.is_valid()) {
			return ht;
		}
	}

	return Ref<BBCodeParserTag>();
}

String BBCodeParserTag::get_attribute_value(const String &attrib) {
	Ref<BBCodeParserAttribute> a = get_attribute(attrib);

	if (a.is_valid()) {
		return a->get_data();
	}

	return "";
}

Ref<BBCodeParserAttribute> BBCodeParserTag::get_attribute(const String &attrib) {
	for (int i = 0; i < _attributes.size(); ++i) {
		Ref<BBCodeParserAttribute> a = _attributes[i];

		if (a->match_attrib(attrib)) {
			return a;
		}
	}

	return Ref<BBCodeParserAttribute>();
}

bool BBCodeParserTag::has_attribute(const String &attrib) {
	for (int i = 0; i < _attributes.size(); ++i) {
		Ref<BBCodeParserAttribute> a = _attributes[i];

		if (a->match_attrib(attrib)) {
			return true;
		}
	}

	return false;
}

Ref<BBCodeParserAttribute> BBCodeParserTag::get_attributec(const String &attrib, const String &contains_val) {
	for (int i = 0; i < _attributes.size(); ++i) {
		Ref<BBCodeParserAttribute> a = _attributes[i];

		if (a->match_attrib(attrib) && a->contains_data(contains_val)) {
			return a;
		}
	}

	return Ref<BBCodeParserAttribute>();
}

bool BBCodeParserTag::has_attributec(const String &attrib, const String &contains_val) {
	for (int i = 0; i < _attributes.size(); ++i) {
		Ref<BBCodeParserAttribute> a = _attributes[i];

		if (a->match_attrib(attrib) && a->contains_data(contains_val)) {
			return true;
		}
	}

	return false;
}

void BBCodeParserTag::process() {
	if (_type != BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_NONE) {
		return;
	}

	if (_data.size() < 2) {
		return;
	}

	ERR_FAIL_COND(_data[0] != '[');
	ERR_FAIL_COND(_data[_data.size() - 1] != ']');

	int start_index = 1;
	if (_data[1] == '/') {
		++start_index;

		_type = BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_CLOSING_TAG;
	} else {
		String tag_text;

		if (_data[_data.size() - 2] == '/') {
			// will catch all that looks like <br/>
			// tags that look like <br> will be caught later in a post process, in a way
			// which also tries to catch erroneously not closed tags that supposed to be closed
			_type = BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_SELF_CLOSING_TAG;

			tag_text = _data.substr(1, _data.size() - 3);
		} else {
			_type = BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_OPENING_TAG;

			tag_text = _data.substr(1, _data.size() - 2);
		}

		int fspc_index = tag_text.find_char(' ');

		if (fspc_index == -1) {
			// no args

			int feq_ind = tag_text.find_char('=');
			if (feq_ind == -1) {
				_tag = tag_text;
				return;
			}

			//Tag is like: [color=white]
			//tag will be like color
			_tag = tag_text.substr(0, feq_ind);

			//Add color=white as argument
			parse_args(tag_text);

			return;
		}

		// grab the tag itself
		_tag = tag_text.substr(0, fspc_index);

		if (fspc_index + 1 == tag_text.size()) {
			// no args, but had a space like <br />
			return;
		}

		String args = tag_text.substr(fspc_index + 1, tag_text.size() - fspc_index - 1);
		parse_args(args);
	}

	int tag_end_index = _data.find_char(' ', start_index);

	if (tag_end_index == -1) {
		// simple tag
		_tag = _data.substr(start_index, _data.size() - start_index - 1);
		return;
	}
}

void BBCodeParserTag::parse_args(const String &args) {
	_attributes.clear();

	int i = 0;
	while (i < args.size()) {
		if (args[i] == ' ') {
			//"trim"
			++i;
			continue;
		}

		int equals_index = args.find_char('=', i);

		Ref<BBCodeParserAttribute> a;
		a.instance();

		if (equals_index == -1) {
			a->set_attribute(args.substr(i, args.size() - i));
			a->set_single(true);
			_attributes.push_back(a);

			return;
		}

		a->set_attribute(args.substr(i, equals_index - i));

		// todo
		// a.trim();

		int next_char_index = equals_index + 1;

		if (next_char_index >= args.size()) {
			// an attribute looks like this "... attrib="
			_attributes.push_back(a);
			return;
		}

		// skip spaces
		while (args[next_char_index] == ' ') {
			++next_char_index;

			if (next_char_index >= args.size()) {
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

			a->set_data(args.substr(next_char_index, args.size() - next_char_index));
			_attributes.push_back(a);
			return;
		}

		a->set_data(args.substr(next_char_index, end_index - next_char_index));
		_attributes.push_back(a);

		i = end_index + 1;
	}
}

String BBCodeParserTag::convert_to_string(const int level) const {
	String s;

	s += String(" ").repeat(level);

	if (_type == BBCODE_PARSER_TAG_TYPE_CONTENT) {
		s += _data + "\n";

		if (_tags.size() != 0) {
			s += String(" ").repeat(level);
			s += "(!CONTENT TAG HAS TAGS!)\n";

			for (int i = 0; i < _tags.size(); ++i) {
				s += _tags[i]->convert_to_string(level + 1) + "\n";
			}
		}
	} else if (_type == BBCODE_PARSER_TAG_TYPE_OPENING_TAG) {
		int ln = level + 1;

		s += "[" + _tag;

		for (int i = 0; i < _attributes.size(); ++i) {
			s += " " + _attributes[i]->convert_to_string();
		}

		s += "]\n";

		for (int i = 0; i < _tags.size(); ++i) {
			s += _tags[i]->convert_to_string(ln);
		}

		s += String(" ").repeat(level);

		s += "[/" + _tag + "]\n";
	} else if (_type == BBCODE_PARSER_TAG_TYPE_CLOSING_TAG) {
		// BBCodeParserTag should handle this automatically
		// it's here for debugging purposes though
		s += "[/" + _tag + "(!)]";

		if (_tags.size() != 0) {
			s += String(" ").repeat(level);
			s += "(!CLOSING TAG HAS TAGS!)\n";

			for (int i = 0; i < _tags.size(); ++i) {
				s += _tags[i]->convert_to_string(level + 1) + "\n";
			}
		}
	} else if (_type == BBCODE_PARSER_TAG_TYPE_SELF_CLOSING_TAG) {
		s += "[" + _tag;

		for (int i = 0; i < _attributes.size(); ++i) {
			s += " " + _attributes[i]->convert_to_string();
		}

		s += "/]\n";

		if (_tags.size() != 0) {
			s += String(" ").repeat(level);

			for (int i = 0; i < _tags.size(); ++i) {
				s += _tags[i]->convert_to_string(level + 1) + "\n";
			}
		}
	} else if (_type == BBCODE_PARSER_TAG_TYPE_NONE) {
		for (int i = 0; i < _tags.size(); ++i) {
			s += _tags[i]->convert_to_string(level) + "\n";
			s += String(" ").repeat(level);
		}
	}

	return s;
}
void BBCodeParserTag::print() const {
	PLOG_MSG(convert_to_string());
}

BBCodeParserTag::BBCodeParserTag() {
	_type = BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_NONE;
}

BBCodeParserTag::~BBCodeParserTag() {
	_tags.clear();
	_attributes.clear();
}

void BBCodeParserTag::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_type"), &BBCodeParserTag::get_type);
	ClassDB::bind_method(D_METHOD("set_type", "val"), &BBCodeParserTag::set_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "type"), "set_type", "get_type");

	ClassDB::bind_method(D_METHOD("get_tag"), &BBCodeParserTag::get_tag);
	ClassDB::bind_method(D_METHOD("set_tag", "val"), &BBCodeParserTag::set_tag);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "tag"), "set_tag", "get_tag");

	ClassDB::bind_method(D_METHOD("get_data"), &BBCodeParserTag::get_data);
	ClassDB::bind_method(D_METHOD("set_data", "val"), &BBCodeParserTag::set_data);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "data"), "set_data", "get_data");

	ClassDB::bind_method(D_METHOD("add_child_tag", "tag"), &BBCodeParserTag::add_child_tag);
	ClassDB::bind_method(D_METHOD("remote_child_tag", "index"), &BBCodeParserTag::remote_child_tag);
	ClassDB::bind_method(D_METHOD("get_child_tag", "index"), &BBCodeParserTag::get_child_tag);
	ClassDB::bind_method(D_METHOD("get_child_tag_count"), &BBCodeParserTag::get_child_tag_count);
	ClassDB::bind_method(D_METHOD("clear_child_tags"), &BBCodeParserTag::clear_child_tags);

	ClassDB::bind_method(D_METHOD("get_child_tags"), &BBCodeParserTag::get_child_tags);
	ClassDB::bind_method(D_METHOD("set_child_tags", "val"), &BBCodeParserTag::set_child_tags);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "child_tags", PROPERTY_HINT_NONE, "17/17:BBCodeParserTag", PROPERTY_USAGE_DEFAULT, "BBCodeParserTag"), "set_child_tags", "get_child_tags");

	ClassDB::bind_method(D_METHOD("add_child_attribute", "tag"), &BBCodeParserTag::add_child_attribute);
	ClassDB::bind_method(D_METHOD("remote_child_attribute", "index"), &BBCodeParserTag::remote_child_attribute);
	ClassDB::bind_method(D_METHOD("get_child_attribute", "index"), &BBCodeParserTag::get_child_attribute);
	ClassDB::bind_method(D_METHOD("get_child_attribute_count"), &BBCodeParserTag::get_child_attribute_count);
	ClassDB::bind_method(D_METHOD("clear_child_attributes"), &BBCodeParserTag::clear_child_attributes);

	ClassDB::bind_method(D_METHOD("get_attributes"), &BBCodeParserTag::get_attributes);
	ClassDB::bind_method(D_METHOD("set_attributes", "val"), &BBCodeParserTag::set_attributes);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "attributes", PROPERTY_HINT_NONE, "17/17:BBCodeParserAttribute", PROPERTY_USAGE_DEFAULT, "BBCodeParserAttribute"), "set_attributes", "get_attributes");

	ClassDB::bind_method(D_METHOD("get_first", "t"), &BBCodeParserTag::get_first);
	ClassDB::bind_method(D_METHOD("get_firstc", "t", "attrib", "val"), &BBCodeParserTag::get_firstc);

	ClassDB::bind_method(D_METHOD("get_attribute_value", "attrib"), &BBCodeParserTag::get_attribute_value);

	ClassDB::bind_method(D_METHOD("get_attribute", "attrib"), &BBCodeParserTag::get_attribute);
	ClassDB::bind_method(D_METHOD("has_attribute", "attrib"), &BBCodeParserTag::has_attribute);

	ClassDB::bind_method(D_METHOD("get_attributec", "attrib", "contains_val"), &BBCodeParserTag::get_attributec);
	ClassDB::bind_method(D_METHOD("has_attributec", "attrib", "contains_val"), &BBCodeParserTag::has_attributec);

	ClassDB::bind_method(D_METHOD("process"), &BBCodeParserTag::process);
	ClassDB::bind_method(D_METHOD("parse_args", "args"), &BBCodeParserTag::parse_args);

	ClassDB::bind_method(D_METHOD("convert_to_string", "level"), &BBCodeParserTag::convert_to_string, 0);
	ClassDB::bind_method(D_METHOD("print"), &BBCodeParserTag::print);

	BIND_ENUM_CONSTANT(BBCODE_PARSER_TAG_TYPE_NONE);
	BIND_ENUM_CONSTANT(BBCODE_PARSER_TAG_TYPE_OPENING_TAG);
	BIND_ENUM_CONSTANT(BBCODE_PARSER_TAG_TYPE_CLOSING_TAG);
	BIND_ENUM_CONSTANT(BBCODE_PARSER_TAG_TYPE_SELF_CLOSING_TAG);
	BIND_ENUM_CONSTANT(BBCODE_PARSER_TAG_TYPE_CONTENT);
}

Ref<BBCodeParserTag> BBCodeParser::get_root() {
	return _root;
}

void BBCodeParser::parse(const String &data) {
	Vector<Ref<BBCodeParserTag>> tags;

	// split into tags
	for (int i = 0; i < data.size(); ++i) {
		if (data[i] == '[') {
			// tag
			for (int j = i + 1; j < data.size(); ++j) {
				if (data[j] == ']') {
					Ref<BBCodeParserTag> t;
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

			for (int j = i + 1; j < data.size(); ++j) {
				if (data[j] == '[') {
					Ref<BBCodeParserTag> t;
					t.instance();

					t->set_data(data.substr(i, j - i));
					t->set_type(BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_CONTENT);

					tags.push_back(t);

					i = j - 1;
					break;
				}
			}
		}
	}

	_root.instance();

	// process tags into hierarchical order
	Vector<Ref<BBCodeParserTag>> tag_stack;
	for (int i = 0; i < tags.size(); ++i) {
		Ref<BBCodeParserTag> t = tags[i];

		ERR_CONTINUE(!t.is_valid());

		if (t->get_type() == BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_NONE) {
			ERR_PRINT("BBCodeParser::parse: t->type == BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_NONE!");
			//memdelete(t);
			tags.write[i].unref();
			continue;
		} else if (t->get_type() == BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_OPENING_TAG) {
			tag_stack.push_back(t);

			tags.write[i].unref();
			continue;
		} else if (t->get_type() == BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_SELF_CLOSING_TAG) {
			if (tag_stack.size() == 0) {
				_root->add_child_tag(t);
			} else {
				tag_stack.write[tag_stack.size() - 1]->add_child_tag(t);
			}

			tags.write[i].unref();
			continue;
		} else if (t->get_type() == BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_CONTENT) {
			if (tag_stack.size() == 0) {
				_root->add_child_tag(t);
			} else {
				tag_stack.write[tag_stack.size() - 1]->add_child_tag(t);
			}

			tags.write[i].unref();
			continue;
		} else if (t->get_type() == BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_CLOSING_TAG) {
			if (tag_stack.size() == 0) {
				tags.write[i].unref();

				// ill-formed html
				continue;
			}

			// find it's pair
			int tag_index = 0;
			for (int j = tag_stack.size() - 1; j > 0; --j) {
				Ref<BBCodeParserTag> ts = tag_stack[j];

				// we sould only have opening tags on the stack
				if (ts->get_tag() == t->get_tag()) {
					// found
					tag_index = j;
					break;
				}
			}

			Ref<BBCodeParserTag> opening_tag = tag_stack[tag_index];

			// mark everything else that we found before finding the opening tag as self closing, and add them to out opening tag
			// If the html is ill formed, it just grabs everything from the tag stack
			for (int j = tag_index + 1; j < tag_stack.size(); ++j) {
				Ref<BBCodeParserTag> ts = tag_stack[j];

				ts->set_type(BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_SELF_CLOSING_TAG);
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

	// add everything remaining on the stack to root
	for (int i = 0; i < tag_stack.size(); ++i) {
		_root->add_child_tag(tag_stack[i]);
	}

	for (int i = 0; i < tags.size(); ++i) {
		Ref<BBCodeParserTag> t = tags[i];

		if (t.is_valid()) {
			ERR_PRINT("BBCodeParser::parse(const String &data): tag was not processed!\n");
			t->print();
		}
	}
}

String BBCodeParser::convert_to_string() const {
	if (!_root.is_valid()) {
		return "";
	}

	return _root->convert_to_string();
}
void BBCodeParser::print() const {
	if (_root.is_valid()) {
		_root->print();
	}
}

BBCodeParser::BBCodeParser() {
}

BBCodeParser::~BBCodeParser() {
	_root.unref();
}

void BBCodeParser::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_root"), &BBCodeParser::get_root);
	ClassDB::bind_method(D_METHOD("parse", "data"), &BBCodeParser::parse);
	ClassDB::bind_method(D_METHOD("convert_to_string"), &BBCodeParser::convert_to_string);
	ClassDB::bind_method(D_METHOD("print"), &BBCodeParser::print);
};
