#include "bbcode_parser.h"

#include "core/error_macros.h"
#include "core/log/logger.h"

bool BBCodeParserAttribute::match_attrib(const String &attrib) {
	return attribute == attrib;
}
bool BBCodeParserAttribute::match_data(const String &d) {
	return data == d;
}
bool BBCodeParserAttribute::match_data(const Vector<String> &d) {
	// todo
	return false;
}
bool BBCodeParserAttribute::contains_data(const String &d) {
	return data.find(d) != -1;
}

String BBCodeParserAttribute::convert_to_string() const {
	if (single) {
		return attribute;
	}

	if (data.find_char('"' == -1)) {
		return attribute + "=\"" + data + "\"";
	} else {
		return attribute + "=\'" + data + "\'";
	}
}

void BBCodeParserAttribute::print() const {
	PLOG_MSG(convert_to_string());
}

BBCodeParserAttribute::BBCodeParserAttribute() {
	single = false;
}

BBCodeParserAttribute::~BBCodeParserAttribute() {
}

Ref<BBCodeParserTag> BBCodeParserTag::get_first(const String &t) {
	if (tag == t) {
		return Ref<BBCodeParserTag>(this);
	}

	for (int i = 0; i < tags.size(); ++i) {
		Ref<BBCodeParserTag> ht = tags.write[i]->get_first(t);

		if (ht.is_valid()) {
			return ht;
		}
	}

	return Ref<BBCodeParserTag>();
}

Ref<BBCodeParserTag> BBCodeParserTag::get_first(const String &t, const String &attrib, const String &val) {
	if (tag == t) {
		if (has_attribute(attrib, val)) {
			return Ref<BBCodeParserTag>(this);
		}
	}

	for (int i = 0; i < tags.size(); ++i) {
		Ref<BBCodeParserTag> ht = tags.write[i]->get_first(t, attrib, val);

		if (ht.is_valid()) {
			return ht;
		}
	}

	return Ref<BBCodeParserTag>();
}

String BBCodeParserTag::get_attribute_value(const String &attrib) {
	Ref<BBCodeParserAttribute> a = get_attribute(attrib);

	if (a.is_valid()) {
		return a->data;
	}

	return "";
}

Ref<BBCodeParserAttribute> BBCodeParserTag::get_attribute(const String &attrib) {
	for (int i = 0; i < attributes.size(); ++i) {
		Ref<BBCodeParserAttribute> a = attributes[i];

		if (a->match_attrib(attrib)) {
			return a;
		}
	}

	return Ref<BBCodeParserAttribute>();
}

bool BBCodeParserTag::has_attribute(const String &attrib) {
	for (int i = 0; i < attributes.size(); ++i) {
		Ref<BBCodeParserAttribute> a = attributes[i];

		if (a->match_attrib(attrib)) {
			return true;
		}
	}

	return false;
}

Ref<BBCodeParserAttribute> BBCodeParserTag::get_attribute(const String &attrib, const String &contains_val) {
	for (int i = 0; i < attributes.size(); ++i) {
		Ref<BBCodeParserAttribute> a = attributes[i];

		if (a->match_attrib(attrib) && a->contains_data(contains_val)) {
			return a;
		}
	}

	return Ref<BBCodeParserAttribute>();
}

bool BBCodeParserTag::has_attribute(const String &attrib, const String &contains_val) {
	for (int i = 0; i < attributes.size(); ++i) {
		Ref<BBCodeParserAttribute> a = attributes[i];

		if (a->match_attrib(attrib) && a->contains_data(contains_val)) {
			return true;
		}
	}

	return false;
}

void BBCodeParserTag::process() {
	if (type != BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_NONE) {
		return;
	}

	if (data.size() < 2) {
		return;
	}

	ERR_FAIL_COND(data[0] != '[');
	ERR_FAIL_COND(data[data.size() - 1] != ']');

	int start_index = 1;
	if (data[1] == '/') {
		++start_index;

		type = BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_CLOSING_TAG;
	} else {
		String tag_text;

		if (data[data.size() - 2] == '/') {
			// will catch all that looks like <br/>
			// tags that look like <br> will be caught later in a post process, in a way
			// which also tries to catch erroneously not closed tags that supposed to be closed
			type = BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_SELF_CLOSING_TAG;

			tag_text = data.substr(1, data.size() - 3);
		} else {
			type = BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_OPENING_TAG;

			tag_text = data.substr(1, data.size() - 2);
		}

		int fspc_index = tag_text.find_char(' ');

		if (fspc_index == -1) {
			// no args

			int feq_ind = tag_text.find_char('=');
			if (feq_ind == -1) {
				tag = tag_text;
				return;
			}

			//Tag is like: [color=white]
			//tag will be like color
			tag = tag_text.substr(0, feq_ind);

			//Add color=white as argument
			parse_args(tag_text);

			return;
		}

		// grab the tag itself
		tag = tag_text.substr(0, fspc_index);

		if (fspc_index + 1 == tag_text.size()) {
			// no args, but had a space like <br />
			return;
		}

		String args = tag_text.substr(fspc_index + 1, tag_text.size() - fspc_index - 1);
		parse_args(args);
	}

	int tag_end_index = data.find_char(' ', start_index);

	if (tag_end_index == -1) {
		// simple tag
		tag = data.substr(start_index, data.size() - start_index - 1);
		return;
	}
}

void BBCodeParserTag::parse_args(const String &args) {
	attributes.clear();

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
			a->attribute = args.substr(i, args.size() - i);
			a->single = true;
			attributes.push_back(a);

			return;
		}

		a->attribute = args.substr(i, equals_index - i);

		// todo
		// a.trim();

		int next_char_index = equals_index + 1;

		if (next_char_index >= args.size()) {
			// an attribute looks like this "... attrib="
			attributes.push_back(a);
			return;
		}

		// skip spaces
		while (args[next_char_index] == ' ') {
			++next_char_index;

			if (next_char_index >= args.size()) {
				// an attribute looks like this "... attrib=     "
				attributes.push_back(a);
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

			a->data = args.substr(next_char_index, args.size() - next_char_index);
			attributes.push_back(a);
			return;
		}

		a->data = args.substr(next_char_index, end_index - next_char_index);
		attributes.push_back(a);

		i = end_index + 1;
	}
}

String BBCodeParserTag::convert_to_string(const int level) const {
	String s;

	s += String(" ").repeat(level);

	if (type == BBCODE_PARSER_TAG_TYPE_CONTENT) {
		s += data + "\n";

		if (tags.size() != 0) {
			s += String(" ").repeat(level);
			s += "(!CONTENT TAG HAS TAGS!)\n";

			for (int i = 0; i < tags.size(); ++i) {
				s += tags[i]->convert_to_string(level + 1) + "\n";
			}
		}
	} else if (type == BBCODE_PARSER_TAG_TYPE_OPENING_TAG) {
		int ln = level + 1;

		s += "[" + tag;

		for (int i = 0; i < attributes.size(); ++i) {
			s += " " + attributes[i]->convert_to_string();
		}

		s += "]\n";

		for (int i = 0; i < tags.size(); ++i) {
			s += tags[i]->convert_to_string(ln);
		}

		s += String(" ").repeat(level);

		s += "[/" + tag + "]\n";
	} else if (type == BBCODE_PARSER_TAG_TYPE_CLOSING_TAG) {
		// BBCodeParserTag should handle this automatically
		// it's here for debugging purposes though
		s += "[/" + tag + "(!)]";

		if (tags.size() != 0) {
			s += String(" ").repeat(level);
			s += "(!CLOSING TAG HAS TAGS!)\n";

			for (int i = 0; i < tags.size(); ++i) {
				s += tags[i]->convert_to_string(level + 1) + "\n";
			}
		}
	} else if (type == BBCODE_PARSER_TAG_TYPE_SELF_CLOSING_TAG) {
		s += "[" + tag;

		for (int i = 0; i < attributes.size(); ++i) {
			s += " " + attributes[i]->convert_to_string();
		}

		s += "/]\n";

		if (tags.size() != 0) {
			s += String(" ").repeat(level);

			for (int i = 0; i < tags.size(); ++i) {
				s += tags[i]->convert_to_string(level + 1) + "\n";
			}
		}
	} else if (type == BBCODE_PARSER_TAG_TYPE_NONE) {
		for (int i = 0; i < tags.size(); ++i) {
			s += tags[i]->convert_to_string(level) + "\n";
			s += String(" ").repeat(level);
		}
	}

	return s;
}
void BBCodeParserTag::print() const {
	PLOG_MSG(convert_to_string());
}

BBCodeParserTag::BBCodeParserTag() {
	type = BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_NONE;
}

BBCodeParserTag::~BBCodeParserTag() {
	tags.clear();
	attributes.clear();
}

void BBCodeParser::parse(const String &data) {
	Vector<Ref<BBCodeParserTag>> tags;

	// split into tags
	for (int i = 0; i < data.size(); ++i) {
		if (data[i] == '[') {
			// tag
			for (int j = i + 1; j < data.size(); ++j) {
				if (data[j] == ']') {
					Ref<BBCodeParserTag> t = memnew(BBCodeParserTag);

					t->data = data.substr(i, j - i + 1);
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
					Ref<BBCodeParserTag> t = memnew(BBCodeParserTag);

					t->data = data.substr(i, j - i);
					t->type = BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_CONTENT;

					tags.push_back(t);

					i = j - 1;
					break;
				}
			}
		}
	}

	root.instance();

	// process tags into hierarchical order
	Vector<Ref<BBCodeParserTag>> tag_stack;
	for (int i = 0; i < tags.size(); ++i) {
		Ref<BBCodeParserTag> t = tags[i];

		ERR_CONTINUE(!t.is_valid());

		if (t->type == BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_NONE) {
			ERR_PRINT("BBCodeParser::parse: t->type == BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_NONE!");
			//memdelete(t);
			tags.write[i].unref();
			continue;
		} else if (t->type == BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_OPENING_TAG) {
			tag_stack.push_back(t);

			tags.write[i].unref();
			continue;
		} else if (t->type == BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_SELF_CLOSING_TAG) {
			if (tag_stack.size() == 0) {
				root->tags.push_back(t);
			} else {
				tag_stack.write[tag_stack.size() - 1]->tags.push_back(t);
			}

			tags.write[i].unref();
			continue;
		} else if (t->type == BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_CONTENT) {
			if (tag_stack.size() == 0) {
				root->tags.push_back(t);
			} else {
				tag_stack.write[tag_stack.size() - 1]->tags.push_back(t);
			}

			tags.write[i].unref();
			continue;
		} else if (t->type == BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_CLOSING_TAG) {
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
				if (ts->tag == t->tag) {
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

				ts->type = BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_SELF_CLOSING_TAG;
				opening_tag->tags.push_back(ts);
			}

			tag_stack.resize(tag_index);

			if (tag_stack.size() == 0) {
				root->tags.push_back(opening_tag);
			} else {
				tag_stack.write[tag_stack.size() - 1]->tags.push_back(opening_tag);
			}

			tags.write[i].unref();

			continue;
		}
	}

	// add everything remaining on the stack to root
	for (int i = 0; i < tag_stack.size(); ++i) {
		root->tags.push_back(tag_stack[i]);
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
	if (!root.is_valid()) {
		return "";
	}

	return root->convert_to_string();
}
void BBCodeParser::print() const {
	if (root.is_valid()) {
		root->print();
	}
}

BBCodeParser::BBCodeParser() {
}

BBCodeParser::~BBCodeParser() {
	root.unref();
}
