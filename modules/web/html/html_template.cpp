/*************************************************************************/
/*  html_template.cpp                                                    */
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

#include "html_template.h"

#include "core/string/string_builder.h"

#include "core/containers/local_vector.h"

#include "../http/web_server_request.h"
#include "html_template_data.h"

// Templates

int HTMLTemplate::get_template_count() const {
	return _templates.size();
}
Ref<HTMLTemplateData> HTMLTemplate::get_template(const int p_index) {
	ERR_FAIL_INDEX_V(p_index, _templates.size(), Ref<HTMLTemplateData>());

	return _templates[p_index];
}
void HTMLTemplate::add_template(const Ref<HTMLTemplateData> &p_template) {
	_templates.push_back(p_template);
}
void HTMLTemplate::remove_template(const int p_index) {
	ERR_FAIL_INDEX(p_index, _templates.size());

	_templates.remove(p_index);
}

void HTMLTemplate::clear_templates() {
	_templates.clear();
}

Vector<Variant> HTMLTemplate::get_templates() {
	Vector<Variant> r;
	for (int i = 0; i < _templates.size(); i++) {
		r.push_back(_templates[i].get_ref_ptr());
	}
	return r;
}
void HTMLTemplate::set_templates(const Vector<Variant> &data) {
	_templates.clear();
	for (int i = 0; i < data.size(); i++) {
		Ref<HTMLTemplateData> e = Ref<HTMLTemplateData>(data[i]);
		_templates.push_back(e);
	}
}

// Overrides

bool HTMLTemplate::has_template_override(const StringName &p_name) const {
	return _template_overrides.has(p_name);
}
String HTMLTemplate::get_template_override(const StringName &p_name) const {
	const String *val = _template_overrides.getptr(p_name);

	if (!val) {
		return String();
	}

	return *val;
}
void HTMLTemplate::set_template_override(const StringName &p_name, const String &p_value) {
	_template_overrides[p_name] = p_value;
}
void HTMLTemplate::remove_template_override(const StringName &p_name) {
	_template_overrides.erase(p_name);
}

void HTMLTemplate::clear_template_overrides() {
	_template_overrides.clear();
}

Dictionary HTMLTemplate::get_template_overrides() const {
	Dictionary ret;

	for (const HashMap<StringName, String>::Element *E = _template_overrides.front(); E; E = E->next) {
		ret[E->key()] = E->value();
	}

	return ret;
}
void HTMLTemplate::set_template_overrides(const Dictionary &p_dict) {
	get_template_overrides();

	Array keys = p_dict.keys();

	for (int i = 0; i < keys.size(); ++i) {
		Variant k = keys[i];
		Variant::Type t = k.get_type();

		if (t != Variant::STRING && t != Variant::STRING_NAME) {
			continue;
		}

		_template_overrides[k] = String(p_dict[k]);
	}
}

HashMap<StringName, String> HTMLTemplate::get_template_overrides_map() const {
	return _template_overrides;
}
void HTMLTemplate::set_template_overrides_map(const HashMap<StringName, String> &p_map) {
	_template_overrides = p_map;
}

// Defaults

bool HTMLTemplate::has_template_default(const StringName &p_name) const {
	return _template_defaults.has(p_name);
}
String HTMLTemplate::get_template_default(const StringName &p_name) const {
	const String *val = _template_defaults.getptr(p_name);

	if (!val) {
		return String();
	}

	return *val;
}
void HTMLTemplate::set_template_default(const StringName &p_name, const String &p_value) {
	_template_defaults[p_name] = p_value;
}
void HTMLTemplate::remove_template_default(const StringName &p_name) {
	_template_defaults.erase(p_name);
}

void HTMLTemplate::clear_template_defaults() {
	_template_defaults.clear();
}

Dictionary HTMLTemplate::get_template_defaults() const {
	Dictionary ret;

	for (const HashMap<StringName, String>::Element *E = _template_defaults.front(); E; E = E->next) {
		ret[E->key()] = E->value();
	}

	return ret;
}
void HTMLTemplate::set_template_defaults(const Dictionary &p_dict) {
	clear_template_defaults();

	Array keys = p_dict.keys();

	for (int i = 0; i < keys.size(); ++i) {
		Variant k = keys[i];
		Variant::Type t = k.get_type();

		if (t != Variant::STRING && t != Variant::STRING_NAME) {
			continue;
		}

		_template_defaults[k] = String(p_dict[k]);
	}
}

HashMap<StringName, String> HTMLTemplate::get_template_defaults_map() const {
	return _template_defaults;
}
void HTMLTemplate::set_template_defaults_map(const HashMap<StringName, String> &p_map) {
	_template_defaults = p_map;
}

// Use

String HTMLTemplate::get_template_text(const StringName &p_name) {
	// First try overrides
	String *sptr = _template_overrides.getptr(p_name);

	if (sptr) {
		return *sptr;
	}

	// Go thourgh templates

	for (int i = 0; i < _templates.size(); ++i) {
		Ref<HTMLTemplateData> d = _templates[i];

		if (!d.is_valid()) {
			continue;
		}

		String r = d->get_template(p_name);

		if (!r.empty()) {
			return r;
		}
	}

	// At last try default

	sptr = _template_defaults.getptr(p_name);

	if (sptr) {
		return *sptr;
	}

	return String();
}

String HTMLTemplate::call_template_method(const TemplateExpressionMethods p_method, const Array &p_data, const bool p_first_var_decides_print) {
	int s = p_data.size();

	if (s == 0) {
		return String();
	}

	if (p_first_var_decides_print) {
		Variant v = p_data[0];

		if (!v) {
			return String();
		}
	}

	if (p_method != TEMPLATE_EXPRESSION_METHOD_VFORMAT) {
		int arg_start = 0;

		if (p_first_var_decides_print) {
			++arg_start;
		}

		StringBuilder ret;

		for (int i = arg_start; i < s; ++i) {
			switch (p_method) {
				case TEMPLATE_EXPRESSION_METHOD_PRINT: {
					ret += String(p_data[i]).xml_escape();
				} break;
				case TEMPLATE_EXPRESSION_METHOD_PRINT_RAW: {
					ret += String(p_data[i]);
				} break;
				case TEMPLATE_EXPRESSION_METHOD_PRINT_BR: {
					ret += String(p_data[i]).xml_escape().newline_to_br();
				} break;
				case TEMPLATE_EXPRESSION_METHOD_PRINT_RAW_BR: {
					ret += String(p_data[i]).newline_to_br();
				} break;
				default:
					break;
			}
		}

		return ret.as_string();
	} else {
		int arg_start = 1;

		if (p_first_var_decides_print) {
			++arg_start;
		}

		//VFormat

		ERR_FAIL_COND_V_MSG(s < arg_start, String(), "vformat requires at least one positional argument!");

		Array args;

		for (int i = arg_start; i < s; ++i) {
			args.push_back(p_data[i]);
		}

		String fstring = String(p_data[arg_start - 1]);

		bool error = false;
		String fmt = fstring.sprintf(args, &error);

		ERR_FAIL_COND_V_MSG(error, String(), "vformat error! Format string: " + fstring + " params: " + String(Variant(args)) + " error string: " + fmt);

		return fmt;
	}

	return String();
}

Variant HTMLTemplate::process_template_expression_variable(const String &p_variable, const Dictionary &p_data, const bool p_allow_missing) {
	// "XXX" // String
	// 'XXX' // String
	// var[1] // Array indexing
	// var["x"] // Dictionary indexing
	// () just gets used ar variable names, except for an outside one, that gets stripped: (var["x"]), also var[("X")], also var[(1)].
	// NO:
	// var[var[var[2]]] Recursive indexing doesn't work.

	// Remove outside brackets
	String variable = p_variable.strip_edges().lstrip("(").rstrip(")").strip_edges();

	if (variable.empty()) {
		return Variant();
	}

	// String
	if (variable.begins_with("\"")) {
		return variable.lstrip("\"").rstrip("\"");
	}

	// String
	if (variable.begins_with("'")) {
		return variable.lstrip("'").rstrip("'");
	}

	int lsqbrace_pos = variable.find("[");

	if (lsqbrace_pos == -1) {
		// Simplest case

		const Variant *element = p_data.getptr(Variant(p_variable));

		if (p_allow_missing && !element) {
			return Variant();
		}

		ERR_FAIL_COND_V_MSG(!element, Variant(), "The given Dictionary does not contain value! " + variable);

		return *element;
	}

	int rsqbrace_pos = variable.find_last("]");

	// Has no [, but has ]. Might be a bug, or just ] in name of variable. If it's not intentional, the error macro will get triggered.
	if (rsqbrace_pos == -1) {
		const Variant *element = p_data.getptr(Variant(p_variable));

		if (p_allow_missing && !element) {
			return Variant();
		}

		ERR_FAIL_COND_V_MSG(!element, Variant(), "The given Dictionary does not contain value! " + variable);

		return *element;
	}

	String var_name = variable.substr_index(0, lsqbrace_pos);

	const Variant *element = p_data.getptr(Variant(var_name));

	if (p_allow_missing && !element) {
		return Variant();
	}

	ERR_FAIL_COND_V_MSG(!element, Variant(), "The given Dictionary does not contain value! " + var_name + " Full variable: " + variable);

	String var_index = variable.substr_index(lsqbrace_pos + 1, rsqbrace_pos).lstrip("(").rstrip(")").strip_edges();
	Variant final_index;

	if (var_index.begins_with("\"")) {
		final_index = var_index.lstrip("\"").rstrip("\"");
	} else if (variable.begins_with("'")) {
		final_index = var_index.lstrip("'").rstrip("'");
	} else {
		// Try to convert to int, if can't leave as string
		if (var_index.is_valid_integer()) {
			final_index = var_index.to_int();
		} else {
			final_index = var_index;
		}
	}

	Variant v = *element;
	Variant r = v.get(final_index);

	return r;
}

String HTMLTemplate::process_template_expression(const String &p_expression, const Dictionary &p_data) {
	// Supported:
	// var // equivalent to p(var)
	// (var) // equivalent to p(var)
	// p(var) // print, escaped, also includes to string cast
	// pr(var) // print_raw, not escaped, also includes to string
	// pb(var) // print_newline_to_br, escaped, turns newlines into <br>, also includes to string cast
	// prb(var) // print_raw_newline_to_br, not escaped, turns newlines into <br>, also includes to string cast
	// vf("%d %d", var1, var2) // vformat
	// qp(var1, var2) // Same as p, but only prints when it's first argument evaluates to true
	// qpr(var1, var2) // Same as pr, but only prints when it's first argument evaluates to true
	// qpb(var1, var2) // Same as pb, but only prints when it's first argument evaluates to true
	// qprb(var1, var2) // Same as prb, but only prints when it's first argument evaluates to true
	// qvf(var1, "%d %d", var1, var2) // Same as vf, but only prints when it's first argument evaluates to true
	// p(var[1]) // Array indexing
	// p(var["x"]) // Dictionary indexing
	// p(var1, var2) All methods can do multiple arguments
	// Not supported:
	// p(var[var[var[2]]]) Recursive indexing.
	// No actual method calls. Even though it should be relatively trivial to implement, it's probably not a good idea.

	String expression = p_expression.strip_edges();
	int expression_length = expression.length();

	if (expression_length == 0) {
		return String();
	}

	int i = 0;
	int method_name_end_index = 0;

	while (i < expression_length) {
		CharType current_token = expression[i];

		switch (current_token) {
			case '(': {
				// Found start '('
				method_name_end_index = i;
				goto method_name_search_done;
			} break;
			case ')': {
				ERR_FAIL_V_MSG(String(), "There is an error in the syntax of an expression! Erroneously placed ). Expression: " + p_expression);
			} break;
			case '"':
			case '\'':
			case '[':
			case ']': {
				// Encountering any of these before a '(' means that a variable is just on it's own.
				// Encountering a '(' ends the search.
				goto method_name_search_done;
			} break;
			default:
				break;
		}

		++i;
	}

method_name_search_done:

	TemplateExpressionMethods call_method = TEMPLATE_EXPRESSION_METHOD_PRINT;
	bool first_var_decides_print = false;

	// This will be zero even if (var)
	if (method_name_end_index != 0) {
		//method_name_end_index points to a '(', substr_index does not include end index.
		String method_name = expression.substr_index(0, method_name_end_index).strip_edges();

		if (method_name == "p") {
			//default, needs to be checked so no error
		} else if (method_name == "pr") {
			call_method = TEMPLATE_EXPRESSION_METHOD_PRINT_RAW;
		} else if (method_name == "pb") {
			call_method = TEMPLATE_EXPRESSION_METHOD_PRINT_BR;
		} else if (method_name == "prb") {
			call_method = TEMPLATE_EXPRESSION_METHOD_PRINT_RAW_BR;
		} else if (method_name == "vf") {
			call_method = TEMPLATE_EXPRESSION_METHOD_VFORMAT;
		} else if (method_name == "qp") {
			//default, needs to be checked so no error
			first_var_decides_print = true;
		} else if (method_name == "qpr") {
			call_method = TEMPLATE_EXPRESSION_METHOD_PRINT_RAW;
			first_var_decides_print = true;
		} else if (method_name == "qpb") {
			call_method = TEMPLATE_EXPRESSION_METHOD_PRINT_BR;
			first_var_decides_print = true;
		} else if (method_name == "qprb") {
			call_method = TEMPLATE_EXPRESSION_METHOD_PRINT_RAW_BR;
			first_var_decides_print = true;
		} else if (method_name == "qvf") {
			call_method = TEMPLATE_EXPRESSION_METHOD_VFORMAT;
			first_var_decides_print = true;
		} else {
			ERR_FAIL_V_MSG(String(), "There is an error in the syntax of an expression! Not a valid method!. Method: " + method_name + " Expression: " + p_expression);
		}
	}

	// From vf("%d %d", var1, var2) this ends up being ("%d %d", var1, var2)
	// Note this can be (((var))) too!
	String variables_str = expression.substr_index(method_name_end_index, expression.length()).strip_edges();

	// Get rid of all '(' from beginning, and ')' from end
	variables_str = variables_str.lstrip("(").rstrip(")");
	int variables_str_length = variables_str.length();

	i = 0;
	bool in_string = false;
	CharType current_string_type = '"';
	bool escape_next = false;
	int last_variable_end_index = 0;
	LocalVector<String> variables;

	// Find all variables, note we can't just split because of strings
	while (i < variables_str_length) {
		CharType current_token = variables_str[i];

		if (escape_next) {
			escape_next = false;
			continue;
		}

		switch (current_token) {
			case ',': {
				// Nothing to do if we are in a string.
				if (in_string) {
					break;
				}

				// last_variable_end_index = v
				// i = ^
				//      v
				// var1, var2, var3
				//           ^
				// in substr_index end index is not included:
				String current_variable_str = variables_str.substr_index(last_variable_end_index, i).strip_edges();
				variables.push_back(current_variable_str);

				// next var:

				// last_variable_end_index = i + 1 means:
				// var1, var2, var3
				//      ^

				last_variable_end_index = i + 1;
			} break;
			case '"': {
				if (in_string) {
					if (current_string_type == '"') {
						in_string = false;
					}

					break;
				}

				// String start
				in_string = true;
				current_string_type = '"';
			} break;
			case '\'': {
				if (in_string) {
					if (current_string_type == '\'') {
						in_string = false;
					}

					break;
				}

				// String start
				in_string = true;
				current_string_type = '\'';
			} break;
			case '\\': {
				// There is nothing we can escape outside of strings
				// This way beginning string symbol cannot be escaped. (Which is bad syntax anyway).
				if (in_string) {
					escape_next = true;
				}
			} break;
			default: {
			} break;
		}

		++i;
	}

	// Also add the last entry
	String current_variable_str = variables_str.substr_index(last_variable_end_index, variables_str_length).strip_edges();

	if (!current_variable_str.empty()) {
		variables.push_back(current_variable_str);
	}

	Array final_values;
	final_values.resize(variables.size());

	// Finally let's just process the variables themselves, and generate the final output
	for (uint32_t vi = 0; vi < variables.size(); ++vi) {
		String variable = variables[vi];

		final_values.set(vi, process_template_expression_variable(variable, p_data, first_var_decides_print && vi == 0));
	}

	return call_template_method(call_method, final_values, first_var_decides_print);
}

String HTMLTemplate::render_template(const String &p_text, const Dictionary &p_data) {
	// {\{ Escaped {{
	// {\\{ -> {\{ etc
	// {{ p(var) }} // print, escaped, also includes to string cast
	// {{ pr(var) }} // print_raw, not escaped, also includes to string
	// {{ pb(var) }} // print_newline_to_br, escaped, turns newlines into <br>, also includes to string cast
	// {{ prb(var) }} // print_raw_newline_to_br, not escaped, turns newlines into <br>, also includes to string cast
	// {{ vf("%d %d", var1, var2) }} // vformat
	// {{ qp(var) }} // Same as p, but only prints when it's first argument evaluates to true
	// {{ qpr(var) }} // Same as pr, but only prints when it's first argument evaluates to true
	// {{ qpb(var) }} // Same as pb, but only prints when it's first argument evaluates to true
	// {{ qprb(var) }} // Same as prb, but only prints when it's first argument evaluates to true
	// {{ qvf("%d %d", var1, var2) }} // Same as vf, but only prints when it's first argument evaluates to true


	int text_length = p_text.length();

	if (text_length == 0) {
		return String();
	}

	StringBuilder result;

	int i = 0;
	int last_section_start = 0;
	bool in_string = false;
	CharType current_string_type = '"';
	int current_state = RENDER_TEMPLATE_STATE_NORMAL_TEXT;
	bool escape_next = false;

	while (i < text_length) {
		CharType current_token = p_text[i];

		switch (current_state) {
			case RENDER_TEMPLATE_STATE_NORMAL_TEXT: {
				if (escape_next) {
					escape_next = false;
					continue;
				}

				switch (current_token) {
					case '{': {
						// A { is encountered, might be an expression.
						current_state = RENDER_TEMPLATE_STATE_EXPRESSION_POTENTIAL_START;
					} break;
				}
			} break;
			case RENDER_TEMPLATE_STATE_EXPRESSION_POTENTIAL_START: {
				switch (current_token) {
					case '\\': {
						// The last token was {, The current token is \.
						// Just set escape_next to true
						// If the next token is {, it will be handled properly in "case '{':"
						// If it's something else "default:" will reset state back to proper
						// In case of {\\\\{ We need to turn it to {\\\{
						// This will go through all \'s
						escape_next = true;
					} break;
					case '{': {
						if (escape_next) {
							// i points to:    v
							// In case of {\\\\{ We need to turn it to {\\\{
							// cut here:     ^
							// We should have {\\\ .

							// We cut
							result += p_text.substr_index(last_section_start, i - 1);

							// Don't append the now missing {, it will be appended on the next normal text cut

							// Go back to normal state
							current_state = RENDER_TEMPLATE_STATE_NORMAL_TEXT;

							//                 ... {\\\\{
							// last_section_start:      ^
							last_section_start = i;

							escape_next = false;

							break;
						}

						// No escape happened, we had a {{. We are in an expression now.

						current_state = RENDER_TEMPLATE_STATE_EXPRESSION;

						// Cut text

						// i points to:       v
						//               ... {{
						// cut up to here:  ^   (Don't include {{ )
						result += p_text.substr_index(last_section_start, i - 1);

						// i points to:          v
						//                  ... {{
						// last_section_start:    ^ (Crop {{ so later it doesn't have to be handled. Having more { should result in an error.)
						last_section_start = i + 1;
					} break;
					default: {
						// Some other token encountered, just go back to normal
						current_state = RENDER_TEMPLATE_STATE_NORMAL_TEXT;
						escape_next = false;
					} break;
				}
			}
			case RENDER_TEMPLATE_STATE_EXPRESSION: {
				switch (current_token) {
					case '}': {
						// We only need to worry about being in a string here, the syntax does not use }-s for anything else.
						if (in_string) {
							break;
						}

						// i points to:      v
						//               ... }}
						// Expression has to end at next token.

						current_state = RENDER_TEMPLATE_STATE_EXPRESSION_END_NEXT;
					} break;
					case '\'': {
						// Previous token was \.
						if (escape_next) {
							escape_next = false;
							break;
						}

						if (in_string) {
							// Only end string with the correct , so "'", or '"' will work
							if (current_string_type == '\'') {
								in_string = false;
								break;
							}

							break;
						}

						// Start string, and remember string type
						current_string_type = '\'';
						in_string = true;
					} break;
					case '"': {
						// Previous token was \.
						if (escape_next) {
							escape_next = false;
							break;
						}

						if (in_string) {
							// Only end string with the correct , so "'", or '"' will work
							if (current_string_type == '"') {
								in_string = false;
								break;
							}

							break;
						}

						// Start string, and remember string type
						current_string_type = '"';
						in_string = true;
					} break;
					case '\\': {
						// Previous token was a \.
						if (escape_next) {
							escape_next = false;
							break;
						}

						// If we are in a string escape next character.
						// Otherwise it doesn't matters.
						if (in_string) {
							escape_next = true;
							break;
						}
					} break;
					default: {
						// Just set back escape_next
						// Could have been a \n in vformat for example.
						escape_next = false;
					} break;
				}
			} break;
			case RENDER_TEMPLATE_STATE_EXPRESSION_END_NEXT: {
				switch (current_token) {
					case '}': {
						// i points to:       v
						//               ... }}
						// Expression has ended

						// Grab expression

						// last_section_start:        v
						//                      ... {{ ... }}
						// i:                               ^

						// We want everything between {{ }} -s

						String expression = p_text.substr_index(last_section_start, i - 2);

						result += process_template_expression(expression, p_data);

						// i points to:          v
						//                  ... {{
						// last_section_start:    ^
						last_section_start = i + 1;

						current_state = RENDER_TEMPLATE_STATE_NORMAL_TEXT;
					} break;
					default: {
						// Some other token encountered, error in template

						result += p_text.substr_index(last_section_start, i);

						// Don't return half-rendered templates.
						ERR_FAIL_V_MSG(String(), "Error in template! One missing closing bracket encountered. Generated html so far:\n\n" + result);
					} break;
				}
			} break;
		}

		i += 1;
	}

	// Unterminated expression in template
	if (current_state != RENDER_TEMPLATE_STATE_NORMAL_TEXT) {
		if (in_string) {
			String c;
			c += current_string_type;
			ERR_FAIL_V_MSG(String(), "Error in template! Unterminated string of type " + c + " encountered. Generated html so far:\n\n" + result.as_string());
		}

		if (current_state == RENDER_TEMPLATE_STATE_EXPRESSION || current_state == RENDER_TEMPLATE_STATE_EXPRESSION_END_NEXT) {
			ERR_FAIL_V_MSG(String(), "Error in template! Unterminated expression encountered. Generated html so far:\n\n" + result.as_string());
		}

		// if current_state is RENDER_TEMPLATE_STATE_EXPRESSION_POTENTIAL_START, that is actually fine. Template just ends in {
	}

	// Get the last bit of text
	// If the template closes with }}, last_section_start should be == to text_length
	// If template closes like }}X last_section_start should be == to text_length - 1, in that case we still need to get the last character
	if (last_section_start <= text_length - 1) {
		result += p_text.substr_index(last_section_start, text_length);
	}

	return result.as_string();
}

String HTMLTemplate::get_and_render_template(const StringName &p_name, const Dictionary &p_data) {
	String text = get_template_text(p_name);

	return render_template(text, p_data);
}

String HTMLTemplate::render(const Ref<WebServerRequest> &p_request, const Dictionary &p_data) {
	return call("_render", p_request, p_data);
}

String HTMLTemplate::_render(const Ref<WebServerRequest> &p_request, const Dictionary &p_data) {
	return "";
}

HTMLTemplate::HTMLTemplate() {
}

HTMLTemplate::~HTMLTemplate() {
}

void HTMLTemplate::_on_editor_template_button_pressed(const StringName &p_property) {
	String name = p_property;

	if (name.begins_with("template_override/")) {
		int scount = name.get_slice_count("/");

		String key = name.get_slicec('/', 1);

		if (scount == 2) {
			// This way add_key can also be used as a key
			if (key == "add_key_button") {
				if (!_editor_new_template_override_key.empty()) {
					_template_overrides[_editor_new_template_override_key] = "";

					_editor_new_template_override_key = "";

					property_list_changed_notify();
				}
			}

			return;
		}

		String property = name.get_slicec('/', 2);

		if (property == "delete_key_button") {
			_template_overrides.erase(key);
			property_list_changed_notify();
		}

		return;
	}

	if (name.begins_with("template_defaults/")) {
		int scount = name.get_slice_count("/");

		String key = name.get_slicec('/', 1);

		if (scount == 2) {
			// This way add_key can also be used as a key
			if (key == "add_key_button") {
				if (!_editor_new_template_default_key.empty()) {
					_template_defaults[_editor_new_template_default_key] = "";

					_editor_new_template_default_key = "";

					property_list_changed_notify();
				}

				return;
			}
		}

		String property = name.get_slicec('/', 2);

		if (property == "delete_key_button") {
			_template_defaults.erase(key);
			property_list_changed_notify();
		}

		return;
	}
}

bool HTMLTemplate::_set(const StringName &p_name, const Variant &p_value) {
	String name = p_name;

	if (name.begins_with("template_override/")) {
		int scount = name.get_slice_count("/");

		String key = name.get_slicec('/', 1);

		if (scount == 2) {
			// This way add_key can also be used as a key
			if (key == "add_key") {
				_editor_new_template_override_key = p_value;
				return true;
			} else if (key == "add_key_button") {
				return true;
			}
		}

		String property = name.get_slicec('/', 2);

		if (property == "value") {
			_template_overrides[key] = String(p_value);
		}

		return true;
	}

	if (name.begins_with("template_defaults/")) {
		int scount = name.get_slice_count("/");

		String key = name.get_slicec('/', 1);

		if (scount == 2) {
			// This way add_key can also be used as a key
			if (key == "add_key") {
				_editor_new_template_default_key = p_value;
				return true;
			} else if (key == "add_key_button") {
				return true;
			}
		}

		String property = name.get_slicec('/', 2);

		if (property == "value") {
			_template_defaults[key] = String(p_value);
		}

		return true;
	}

	return false;
}

bool HTMLTemplate::_get(const StringName &p_name, Variant &r_ret) const {
	String name = p_name;

	if (name.begins_with("template_override/")) {
		int scount = name.get_slice_count("/");

		String key = name.get_slicec('/', 1);

		if (scount == 2) {
			// This way add_key can also be used as a key
			if (key == "add_key") {
				r_ret = _editor_new_template_override_key;
				return true;
			}
		}

		String property = name.get_slicec('/', 2);

		if (property == "value") {
			if (!_template_overrides.has(key)) {
				return false;
			}

			r_ret = _template_overrides[key];
			return true;
		}

		return true;
	}

	if (name.begins_with("template_defaults/")) {
		int scount = name.get_slice_count("/");

		String key = name.get_slicec('/', 1);

		if (scount == 2) {
			// This way add_key can also be used as a key
			if (key == "add_key") {
				r_ret = _editor_new_template_default_key;
				return true;
			}
		}

		String property = name.get_slicec('/', 2);

		if (property == "value") {
			if (!_template_defaults.has(key)) {
				return false;
			}

			r_ret = _template_defaults[key];
			return true;
		}

		return true;
	}

	return false;
}

void HTMLTemplate::_get_property_list(List<PropertyInfo> *p_list) const {
	for (const HashMap<StringName, String>::Element *E = _template_overrides.front(); E; E = E->next) {
		p_list->push_back(PropertyInfo(Variant::STRING, "template_override/" + E->key() + "/value", PROPERTY_HINT_MULTILINE_TEXT));
		p_list->push_back(PropertyInfo(Variant::NIL, "template_override/" + E->key() + "/delete_key_button", PROPERTY_HINT_BUTTON, "_on_editor_template_button_pressed:Close/EditorIcons"));
	}

	p_list->push_back(PropertyInfo(Variant::STRING, "template_override/add_key"));
	p_list->push_back(PropertyInfo(Variant::NIL, "template_override/add_key_button", PROPERTY_HINT_BUTTON, "_on_editor_template_button_pressed:Add/EditorIcons"));

	for (const HashMap<StringName, String>::Element *E = _template_defaults.front(); E; E = E->next) {
		p_list->push_back(PropertyInfo(Variant::STRING, "template_defaults/" + E->key() + "/value", PROPERTY_HINT_MULTILINE_TEXT));
		p_list->push_back(PropertyInfo(Variant::NIL, "template_defaults/" + E->key() + "/delete_key_button", PROPERTY_HINT_BUTTON, "_on_editor_template_button_pressed:Close/EditorIcons"));
	}

	p_list->push_back(PropertyInfo(Variant::STRING, "template_defaults/add_key"));
	p_list->push_back(PropertyInfo(Variant::NIL, "template_defaults/add_key_button", PROPERTY_HINT_BUTTON, "_on_editor_template_button_pressed:Add/EditorIcons"));
}

void HTMLTemplate::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_on_editor_template_button_pressed"), &HTMLTemplate::_on_editor_template_button_pressed);

	// Templates
	ClassDB::bind_method(D_METHOD("get_template_count"), &HTMLTemplate::get_template_count);
	ClassDB::bind_method(D_METHOD("get_template", "index"), &HTMLTemplate::get_template);
	ClassDB::bind_method(D_METHOD("add_template", "template"), &HTMLTemplate::add_template);
	ClassDB::bind_method(D_METHOD("remove_template", "index"), &HTMLTemplate::remove_template);

	ClassDB::bind_method(D_METHOD("clear_templates"), &HTMLTemplate::clear_templates);

	ClassDB::bind_method(D_METHOD("get_templates"), &HTMLTemplate::get_templates);
	ClassDB::bind_method(D_METHOD("set_templates", "data"), &HTMLTemplate::set_templates);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "templates", PROPERTY_HINT_NONE, "23/20:HTMLTemplateData", PROPERTY_USAGE_DEFAULT, "HTMLTemplateData"), "set_templates", "get_templates");

	// Overrides
	ClassDB::bind_method(D_METHOD("has_template_override", "name"), &HTMLTemplate::has_template_override);
	ClassDB::bind_method(D_METHOD("get_template_override", "name"), &HTMLTemplate::get_template_override);
	ClassDB::bind_method(D_METHOD("set_template_override", "name", "value"), &HTMLTemplate::set_template_override);
	ClassDB::bind_method(D_METHOD("remove_template_override", "name"), &HTMLTemplate::remove_template_override);

	ClassDB::bind_method(D_METHOD("clear_template_overrides"), &HTMLTemplate::clear_template_overrides);

	ClassDB::bind_method(D_METHOD("get_template_overrides"), &HTMLTemplate::get_template_overrides);
	ClassDB::bind_method(D_METHOD("set_template_overrides", "dict"), &HTMLTemplate::set_template_overrides);
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "template_overrides", PROPERTY_HINT_NONE, "", 0), "set_template_overrides", "get_template_overrides");

	// Defaults
	ClassDB::bind_method(D_METHOD("has_template_default", "name"), &HTMLTemplate::has_template_default);
	ClassDB::bind_method(D_METHOD("get_template_default", "name"), &HTMLTemplate::get_template_default);
	ClassDB::bind_method(D_METHOD("set_template_default", "name", "value"), &HTMLTemplate::set_template_default);
	ClassDB::bind_method(D_METHOD("remove_template_default", "name"), &HTMLTemplate::remove_template_default);

	ClassDB::bind_method(D_METHOD("clear_template_defaults"), &HTMLTemplate::clear_template_defaults);

	ClassDB::bind_method(D_METHOD("get_template_defaults"), &HTMLTemplate::get_template_defaults);
	ClassDB::bind_method(D_METHOD("set_template_defaults", "dict"), &HTMLTemplate::set_template_defaults);
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "template_defaults", PROPERTY_HINT_NONE, "", 0), "set_template_defaults", "get_template_defaults");

	// Use

	ClassDB::bind_method(D_METHOD("get_template_text", "name"), &HTMLTemplate::get_template_text);

	ClassDB::bind_method(D_METHOD("call_template_method", "method", "data", "first_var_decides_print"), &HTMLTemplate::call_template_method);
	ClassDB::bind_method(D_METHOD("process_template_expression_variable", "variable", "data", "allow_missing"), &HTMLTemplate::process_template_expression_variable, false);
	ClassDB::bind_method(D_METHOD("process_template_expression", "expression", "data"), &HTMLTemplate::process_template_expression);
	ClassDB::bind_method(D_METHOD("render_template", "text", "data"), &HTMLTemplate::render_template);

	ClassDB::bind_method(D_METHOD("get_and_render_template", "name", "data"), &HTMLTemplate::get_and_render_template);

	BIND_VMETHOD(MethodInfo(Variant::STRING, "_render",
			PropertyInfo(Variant::OBJECT, "request", PROPERTY_HINT_RESOURCE_TYPE, "WebServerRequest"),
			PropertyInfo(Variant::DICTIONARY, "data")));

	ClassDB::bind_method(D_METHOD("render", "request", "data"), &HTMLTemplate::render);
	ClassDB::bind_method(D_METHOD("_render", "request", "data"), &HTMLTemplate::_render);

	// Enums
	BIND_ENUM_CONSTANT(TEMPLATE_EXPRESSION_METHOD_PRINT);
	BIND_ENUM_CONSTANT(TEMPLATE_EXPRESSION_METHOD_PRINT_RAW);
	BIND_ENUM_CONSTANT(TEMPLATE_EXPRESSION_METHOD_PRINT_BR);
	BIND_ENUM_CONSTANT(TEMPLATE_EXPRESSION_METHOD_PRINT_RAW_BR);
	BIND_ENUM_CONSTANT(TEMPLATE_EXPRESSION_METHOD_VFORMAT);
}
