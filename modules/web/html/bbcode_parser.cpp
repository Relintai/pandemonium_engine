#include "bbcode_parser.h"

#include "core/error_macros.h"

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

String BBCodeParserAttribute::to_string() {
	if (single) {
		return attribute;
	}

	if (data.find_char('"' == -1)) {
		return attribute + "=\"" + data + "\"";
	} else {
		return attribute + "=\'" + data + "\'";
	}
}

void BBCodeParserAttribute::print() {
	ERR_PRINT(to_string());
}

BBCodeParserAttribute::BBCodeParserAttribute() {
	single = false;
}

BBCodeParserAttribute::~BBCodeParserAttribute() {
}

BBCodeParserTag *BBCodeParserTag::get_first(const String &t) {
	if (tag == t) {
		return this;
	}

	for (int i = 0; i < tags.size(); ++i) {
		BBCodeParserTag *ht = tags[i]->get_first(t);

		if (ht) {
			return ht;
		}
	}

	return nullptr;
}

BBCodeParserTag *BBCodeParserTag::get_first(const String &t, const String &attrib, const String &val) {
	if (tag == t) {
		if (has_attribute(attrib, val)) {
			return this;
		}
	}

	for (int i = 0; i < tags.size(); ++i) {
		BBCodeParserTag *ht = tags[i]->get_first(t, attrib, val);

		if (ht) {
			return ht;
		}
	}

	return nullptr;
}

String BBCodeParserTag::get_attribute_value(const String &attrib) {
	BBCodeParserAttribute *a = get_attribute(attrib);

	if (a) {
		return a->data;
	}

	return "";
}

BBCodeParserAttribute *BBCodeParserTag::get_attribute(const String &attrib) {
	for (int i = 0; i < attributes.size(); ++i) {
		BBCodeParserAttribute *a = attributes[i];

		if (a->match_attrib(attrib)) {
			return a;
		}
	}

	return nullptr;
}

bool BBCodeParserTag::has_attribute(const String &attrib) {
	for (int i = 0; i < attributes.size(); ++i) {
		BBCodeParserAttribute *a = attributes[i];

		if (a->match_attrib(attrib)) {
			return true;
		}
	}

	return false;
}

BBCodeParserAttribute *BBCodeParserTag::get_attribute(const String &attrib, const String &contains_val) {
	for (int i = 0; i < attributes.size(); ++i) {
		BBCodeParserAttribute *a = attributes[i];

		if (a->match_attrib(attrib) && a->contains_data(contains_val)) {
			return a;
		}
	}

	return nullptr;
}

bool BBCodeParserTag::has_attribute(const String &attrib, const String &contains_val) {
	for (int i = 0; i < attributes.size(); ++i) {
		BBCodeParserAttribute *a = attributes[i];

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

		BBCodeParserAttribute *a = memnew(BBCodeParserAttribute);

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

String BBCodeParserTag::to_string(const int level) {
	String s;

	s += String(" ").repeat(level);

	if (type == BBCODE_PARSER_TAG_TYPE_CONTENT) {
		s += data + "\n";

		if (tags.size() != 0) {
			s += String(" ").repeat(level);
			s += "(!CONTENT TAG HAS TAGS!)\n";

			for (int i = 0; i < tags.size(); ++i) {
				s += tags[i]->to_string(level + 1) + "\n";
			}
		}
	} else if (type == BBCODE_PARSER_TAG_TYPE_OPENING_TAG) {
		int ln = level + 1;

		s += "[" + tag;

		for (int i = 0; i < attributes.size(); ++i) {
			s += " " + attributes[i]->to_string();
		}

		s += "]\n";

		for (int i = 0; i < tags.size(); ++i) {
			s += tags[i]->to_string(ln);
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
				s += tags[i]->to_string(level + 1) + "\n";
			}
		}
	} else if (type == BBCODE_PARSER_TAG_TYPE_SELF_CLOSING_TAG) {
		s += "[" + tag;

		for (int i = 0; i < attributes.size(); ++i) {
			s += " " + attributes[i]->to_string();
		}

		s += "/]\n";

		if (tags.size() != 0) {
			s += String(" ").repeat(level);

			for (int i = 0; i < tags.size(); ++i) {
				s += tags[i]->to_string(level + 1) + "\n";
			}
		}
	} else if (type == BBCODE_PARSER_TAG_TYPE_NONE) {
		for (int i = 0; i < tags.size(); ++i) {
			s += tags[i]->to_string(level) + "\n";
			s += String(" ").repeat(level);
		}
	}

	return s;
}
void BBCodeParserTag::print() {
	ERR_PRINT(to_string());
}

BBCodeParserTag::BBCodeParserTag() {
	type = BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_NONE;
}

BBCodeParserTag::~BBCodeParserTag() {
	for (int i = 0; i < tags.size(); ++i) {
		memdelete(tags[i]);
	}

	for (int i = 0; i < attributes.size(); ++i) {
		memdelete(attributes[i]);
	}
}

void BBCodeParser::parse(const String &data) {
	Vector<BBCodeParserTag *> tags;

	// split into tags
	for (int i = 0; i < data.size(); ++i) {
		if (data[i] == '[') {
			// tag
			for (int j = i + 1; j < data.size(); ++j) {
				if (data[j] == ']') {
					BBCodeParserTag *t = memnew(BBCodeParserTag);

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
					BBCodeParserTag *t = memnew(BBCodeParserTag);

					t->data = data.substr(i, j - i);
					t->type = BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_CONTENT;

					tags.push_back(t);

					i = j - 1;
					break;
				}
			}
		}
	}

	if (root) {
		memdelete(root);
	}

	root = memnew(BBCodeParserTag);

	// process tags into hierarchical order
	Vector<BBCodeParserTag *> tag_stack;
	for (int i = 0; i < tags.size(); ++i) {
		BBCodeParserTag *t = tags[i];

		ERR_CONTINUE_MSG(t == nullptr, "BBCodeParser::parse: t == nullptr!");

		if (t->type == BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_NONE) {
			ERR_PRINT("BBCodeParser::parse: t->type == BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_NONE!");
			memdelete(t);
			tags.write[i] = nullptr;
			continue;
		} else if (t->type == BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_OPENING_TAG) {
			tag_stack.push_back(t);

			tags.write[i] = nullptr;
			continue;
		} else if (t->type == BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_SELF_CLOSING_TAG) {
			if (tag_stack.size() == 0) {
				root->tags.push_back(t);
			} else {
				tag_stack[tag_stack.size() - 1]->tags.push_back(t);
			}

			tags.write[i] = nullptr;
			continue;
		} else if (t->type == BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_CONTENT) {
			if (tag_stack.size() == 0) {
				root->tags.push_back(t);
			} else {
				tag_stack[tag_stack.size() - 1]->tags.push_back(t);
			}

			tags.write[i] = nullptr;
			continue;
		} else if (t->type == BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_CLOSING_TAG) {
			if (tag_stack.size() == 0) {
				memdelete(t);
				tags.write[i] = nullptr;

				// ill-formed html
				continue;
			}

			// find it's pair
			int tag_index = 0;
			for (int j = tag_stack.size() - 1; j > 0; --j) {
				BBCodeParserTag *ts = tag_stack[j];

				// we sould only have opening tags on the stack
				if (ts->tag == t->tag) {
					// found
					tag_index = j;
					break;
				}
			}

			BBCodeParserTag *opening_tag = tag_stack[tag_index];

			// mark everything else that we found before finding the opening tag as self closing, and add them to out opening tag
			// If the html is ill formed, it just grabs everything from the tag stack
			for (int j = tag_index + 1; j < tag_stack.size(); ++j) {
				BBCodeParserTag *ts = tag_stack[j];

				ts->type = BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_SELF_CLOSING_TAG;
				opening_tag->tags.push_back(ts);
			}

			tag_stack.resize(tag_index);

			if (tag_stack.size() == 0) {
				root->tags.push_back(opening_tag);
			} else {
				tag_stack[tag_stack.size() - 1]->tags.push_back(opening_tag);
			}

			memdelete(t);
			tags.write[i] = nullptr;

			continue;
		}
	}

	// add everything remaining on the stack to root
	for (int i = 0; i < tag_stack.size(); ++i) {
		root->tags.push_back(tag_stack[i]);
	}

	for (int i = 0; i < tags.size(); ++i) {
		BBCodeParserTag *t = tags[i];

		if (t != nullptr) {
			ERR_PRINT("BBCodeParser::parse(const String &data): tag was not processed!\n");
			t->print();

			memdelete(t);
		}
	}
}

String BBCodeParser::to_string() {
	if (!root) {
		return "";
	}

	return root->to_string();
}
void BBCodeParser::print() {
	if (root) {
		root->print();
	}
}

BBCodeParser::BBCodeParser() {
	root = nullptr;
}

BBCodeParser::~BBCodeParser() {
	if (root) {
		memdelete(root);
	}
}
