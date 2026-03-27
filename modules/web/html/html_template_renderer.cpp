/*************************************************************************/
/*  html_template_renderer.cpp                                           */
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

// New builtin funcs:
// exists(<var name>)
// equals(<var1>, <var2>)
// approx_equals(<var1>, <var2>)
// mod(<var1>, <var2>)
// set(<key (str), no need for "">, <value>)  ret value  (if value is null, unset)
// set_if(<bool expr>, <key (str), no need for "">, <value>)  ret value  (if value is null, unset)
// int(), float(), str()
// range(<var1>, <var2 opt>, <var 3 opt>)
// keys(<var1 Dict>)
// values(<var1 Dict>)
// tr() translate
// trt()

// No assignment operator, set should be good enough

// operators: All variant ops including in  (note that it can't do assignment)
// ternary _template_text:
// {{? <bool expr>; <true expr>; <false expr> }}
// Could also just parse: Equivalent to qr, qp etc (so if false expr is null, just emit "", same for true expr)
// {{? <bool expr>; <true expr> }}
// Could also just parse: Equivalent to qr, qp etc (so if false expr is null, just emit "", same for true expr)
// {{? <bool expr>;; <false expr> }}
// assignment? Just assign to the input dictionary

// if? If there are bool _template_texts and ternary, an if should probably also be here
// It should still be relatively simple, as there are no scopes, and no real proper variable declarations, and stack
// {{if <bool expr> }}
// {{elif <bool expr> }}
// {{else}}
// {{endif}}

// foreach?
// It does need variable declaration, but it could be just added to the input dictionary.
// Also with the range built in, this could also double as normal for like in gdscript
// {{for <variable declaration> in <collection> }}
// {{endfor}}
// Variant can do the iteration, that mechanism should just be used.
// in _execute
// variant iter init, while !iter end, set name to data dict, call body expr chain

// other stuff:
// Recursive indexing now should work

// Variant call / Object call?
// likely not a good idea, skip

// Array and dictionary construction?
// No need, do arrays and dicts in template scripts
// Should probably be done anyway, not much complexity, but good flexibility
// [], and {} (or could be pscript like: |{}|, but here it's not really necessary)

#include "html_template_renderer.h"

String HTMLTemplaterenderer::render(const Dictionary &p_data, const bool p_show_error) {
	ERR_FAIL_COND_V_MSG(_error_set, String(), "There was previously a parse error: " + _error_str + ".");

	Dictionary data = p_data;
	StringBuilder html;

	_execution_error = false;
	Variant output;
	String error_txt;

	bool err = _execute(data, html, _root, output, error_txt);

	if (err) {
		_execution_error = true;
		_error_str = error_txt;
		ERR_FAIL_COND_V_MSG(p_show_error, html.as_string(), _error_str);
	}

	return html.as_string();
}

bool HTMLTemplaterenderer::compile(const String &p_text) {
	return false;
}

String HTMLTemplaterenderer::get_error_str() {
	return _error_str;
}

HTMLTemplaterenderer::HTMLTemplaterenderer() {
}

HTMLTemplaterenderer::~HTMLTemplaterenderer() {
}

void HTMLTemplaterenderer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("render", "data", "p_show_error"), &HTMLTemplaterenderer::render, DEFVAL(true));
	ClassDB::bind_method(D_METHOD("compile", "text"), &HTMLTemplaterenderer::compile);
	ClassDB::bind_method(D_METHOD("get_error_str"), &HTMLTemplaterenderer::get_error_str);
}

const char *HTMLTemplaterenderer::func_name[HTMLTemplaterenderer::FUNC_MAX] = {
	"p",
	"pr",
	"pb",
	"prb",
	"vf",
	"qp",
	"qpr",
	"qpb",
	"qprb",
	"qvf",
	"exists",
	"equals",
	"approx_equals",
};


HTMLTemplaterenderer::BuiltinFunc HTMLTemplaterenderer::find_function(const String &p_string) {
	for (int i = 0; i < FUNC_MAX; i++) {
		if (p_string == func_name[i]) {
			return BuiltinFunc(i);
		}
	}

	return FUNC_MAX;
}

const char *HTMLTemplaterenderer::token_name[TK_MAX] = {
	"CURLY BRACKET OPEN",
	"CURLY BRACKET CLOSE",
	"BRACKET OPEN",
	"BRACKET CLOSE",
	"PARENTHESIS OPEN",
	"PARENTHESIS CLOSE",
	"IDENTIFIER",
	"BUILTIN FUNC",
	"CONSTANT",
	"BASIC TYPE",
	"COLON",
	"COMMA",
	"PERIOD",
	"OP IN",
	"OP EQUAL",
	"OP NOT EQUAL",
	"OP LESS",
	"OP LESS EQUAL",
	"OP GREATER",
	"OP GREATER EQUAL",
	"OP AND",
	"OP OR",
	"OP NOT",
	"OP ADD",
	"OP SUB",
	"OP MUL",
	"OP DIV",
	"OP MOD",
	"OP SHIFT LEFT",
	"OP SHIFT RIGHT",
	"OP BIT AND",
	"OP BIT OR",
	"OP BIT XOR",
	"OP BIT INVERT",
	"EOF",
	"ERROR"
};

static bool _is_number(CharType c) {
	return (c >= '0' && c <= '9');
}

static bool _is_hex_digit(char32_t c) {
	return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

static bool _is_binary_digit(char32_t c) {
	return (c == '0' || c == '1');
}

Error HTMLTemplaterenderer::_get_token(Token &r_token) {
	while (true) {
#define GET_CHAR() (str_ofs >= _template_text.length() ? 0 : _template_text[str_ofs++])

		CharType cchar = GET_CHAR();

		switch (cchar) {
			case 0: {
				r_token.type = TK_EOF;
				return OK;
			};
			case '{': {
				r_token.type = TK_CURLY_BRACKET_OPEN;
				return OK;
			};
			case '}': {
				r_token.type = TK_CURLY_BRACKET_CLOSE;
				return OK;
			};
			case '[': {
				r_token.type = TK_BRACKET_OPEN;
				return OK;
			};
			case ']': {
				r_token.type = TK_BRACKET_CLOSE;
				return OK;
			};
			case '(': {
				r_token.type = TK_PARENTHESIS_OPEN;
				return OK;
			};
			case ')': {
				r_token.type = TK_PARENTHESIS_CLOSE;
				return OK;
			};
			case ',': {
				r_token.type = TK_COMMA;
				return OK;
			};
			case ':': {
				r_token.type = TK_COLON;
				return OK;
			};
			case '=': {
				cchar = GET_CHAR();
				if (cchar == '=') {
					r_token.type = TK_OP_EQUAL;
				} else {
					_set_error("Expected '='");
					r_token.type = TK_ERROR;
					return ERR_PARSE_ERROR;
				}
				return OK;
			};
			case '!': {
				if (_template_text[str_ofs] == '=') {
					r_token.type = TK_OP_NOT_EQUAL;
					str_ofs++;
				} else {
					r_token.type = TK_OP_NOT;
				}
				return OK;
			};
			case '>': {
				if (_template_text[str_ofs] == '=') {
					r_token.type = TK_OP_GREATER_EQUAL;
					str_ofs++;
				} else if (_template_text[str_ofs] == '>') {
					r_token.type = TK_OP_SHIFT_RIGHT;
					str_ofs++;
				} else {
					r_token.type = TK_OP_GREATER;
				}
				return OK;
			};
			case '<': {
				if (_template_text[str_ofs] == '=') {
					r_token.type = TK_OP_LESS_EQUAL;
					str_ofs++;
				} else if (_template_text[str_ofs] == '<') {
					r_token.type = TK_OP_SHIFT_LEFT;
					str_ofs++;
				} else {
					r_token.type = TK_OP_LESS;
				}
				return OK;
			};
			case '+': {
				r_token.type = TK_OP_ADD;
				return OK;
			};
			case '-': {
				r_token.type = TK_OP_SUB;
				return OK;
			};
			case '/': {
				r_token.type = TK_OP_DIV;
				return OK;
			};
			case '*': {
				r_token.type = TK_OP_MUL;
				return OK;
			};
			case '%': {
				r_token.type = TK_OP_MOD;
				return OK;
			};
			case '&': {
				if (_template_text[str_ofs] == '&') {
					r_token.type = TK_OP_AND;
					str_ofs++;
				} else {
					r_token.type = TK_OP_BIT_AND;
				}
				return OK;
			};
			case '|': {
				if (_template_text[str_ofs] == '|') {
					r_token.type = TK_OP_OR;
					str_ofs++;
				} else {
					r_token.type = TK_OP_BIT_OR;
				}
				return OK;
			};
			case '^': {
				r_token.type = TK_OP_BIT_XOR;

				return OK;
			};
			case '~': {
				r_token.type = TK_OP_BIT_INVERT;

				return OK;
			};
			case '\'':
			case '"': {
				String str;
				while (true) {
					CharType ch = GET_CHAR();

					if (ch == 0) {
						_set_error("Unterminated String");
						r_token.type = TK_ERROR;
						return ERR_PARSE_ERROR;
					} else if (ch == cchar) {
						// cchar contain a corresponding quote symbol
						break;
					} else if (ch == '\\') {
						//escaped characters...

						CharType next = GET_CHAR();
						if (next == 0) {
							_set_error("Unterminated String");
							r_token.type = TK_ERROR;
							return ERR_PARSE_ERROR;
						}
						CharType res = 0;

						switch (next) {
							case 'b':
								res = 8;
								break;
							case 't':
								res = 9;
								break;
							case 'n':
								res = 10;
								break;
							case 'f':
								res = 12;
								break;
							case 'r':
								res = 13;
								break;
							case 'u': {
								//hexnumbarh - oct is deprecated

								for (int j = 0; j < 4; j++) {
									CharType c = GET_CHAR();

									if (c == 0) {
										_set_error("Unterminated String");
										r_token.type = TK_ERROR;
										return ERR_PARSE_ERROR;
									}
									if (!(_is_number(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))) {
										_set_error("Malformed hex constant in string");
										r_token.type = TK_ERROR;
										return ERR_PARSE_ERROR;
									}
									CharType v;
									if (_is_number(c)) {
										v = c - '0';
									} else if (c >= 'a' && c <= 'f') {
										v = c - 'a';
										v += 10;
									} else if (c >= 'A' && c <= 'F') {
										v = c - 'A';
										v += 10;
									} else {
										ERR_PRINT("BUG");
										v = 0;
									}

									res <<= 4;
									res |= v;
								}

							} break;
							//case '\"': res='\"'; break;
							//case '\\': res='\\'; break;
							//case '/': res='/'; break;
							default: {
								res = next;
								//r_err_str="Invalid escape sequence";
								//return ERR_PARSE_ERROR;
							} break;
						}

						str += res;

					} else {
						str += ch;
					}
				}

				r_token.type = TK_CONSTANT;
				r_token.value = str;
				return OK;

			} break;
			default: {
				if (cchar <= 32) {
					break;
				}

				CharType next_char = (str_ofs >= _template_text.length()) ? 0 : _template_text[str_ofs];
				if (_is_number(cchar) || (cchar == '.' && _is_number(next_char))) {
					//a number

					String num;
#define READING_SIGN 0
#define READING_INT 1
#define READING_HEX 2
#define READING_BIN 3
#define READING_DEC 4
#define READING_EXP 5
#define READING_DONE 6
					int reading = READING_INT;

					CharType c = cchar;
					bool exp_sign = false;
					bool exp_beg = false;
					bool bin_beg = false;
					bool hex_beg = false;
					bool is_float = false;
					bool is_first_char = true;

					while (true) {
						switch (reading) {
							case READING_INT: {
								if (_is_number(c)) {
									if (is_first_char && c == '0') {
										if (next_char == 'b') {
											reading = READING_BIN;
										} else if (next_char == 'x') {
											reading = READING_HEX;
										}
									}
								} else if (c == '.') {
									reading = READING_DEC;
									is_float = true;
								} else if (c == 'e') {
									reading = READING_EXP;
									is_float = true;
								} else {
									reading = READING_DONE;
								}

							} break;
							case READING_BIN: {
								if (bin_beg && !_is_binary_digit(c)) {
									reading = READING_DONE;
								} else if (c == 'b') {
									bin_beg = true;
								}

							} break;
							case READING_HEX: {
								if (hex_beg && !_is_hex_digit(c)) {
									reading = READING_DONE;
								} else if (c == 'x') {
									hex_beg = true;
								}

							} break;
							case READING_DEC: {
								if (_is_number(c)) {
								} else if (c == 'e') {
									reading = READING_EXP;

								} else {
									reading = READING_DONE;
								}

							} break;
							case READING_EXP: {
								if (_is_number(c)) {
									exp_beg = true;

								} else if ((c == '-' || c == '+') && !exp_sign && !exp_beg) {
									exp_sign = true;

								} else {
									reading = READING_DONE;
								}
							} break;
						}

						if (reading == READING_DONE) {
							break;
						}
						num += String::chr(c);
						c = GET_CHAR();
						is_first_char = false;
					}

					if (c != 0) {
						str_ofs--;
					}

					r_token.type = TK_CONSTANT;

					if (is_float) {
						r_token.value = num.to_double();
					} else if (bin_beg) {
						r_token.value = num.bin_to_int64();
					} else if (hex_beg) {
						r_token.value = num.hex_to_int64();
					} else {
						r_token.value = num.to_int64();
					}
					return OK;

				} else if ((cchar >= 'A' && cchar <= 'Z') || (cchar >= 'a' && cchar <= 'z') || cchar == '_') {
					String id;
					bool first = true;

					while ((cchar >= 'A' && cchar <= 'Z') || (cchar >= 'a' && cchar <= 'z') || cchar == '_' || (!first && _is_number(cchar))) {
						id += String::chr(cchar);
						cchar = GET_CHAR();
						first = false;
					}

					str_ofs--; //go back one

					if (id == "in") {
						r_token.type = TK_OP_IN;
					} else if (id == "null") {
						r_token.type = TK_CONSTANT;
						r_token.value = Variant();
					} else if (id == "true") {
						r_token.type = TK_CONSTANT;
						r_token.value = true;
					} else if (id == "false") {
						r_token.type = TK_CONSTANT;
						r_token.value = false;
					} else if (id == "PI") {
						r_token.type = TK_CONSTANT;
						r_token.value = Math_PI;
					} else if (id == "TAU") {
						r_token.type = TK_CONSTANT;
						r_token.value = Math_TAU;
					} else if (id == "INF") {
						r_token.type = TK_CONSTANT;
						r_token.value = Math_INF;
					} else if (id == "NAN") {
						r_token.type = TK_CONSTANT;
						r_token.value = Math_NAN;
					} else if (id == "not") {
						r_token.type = TK_OP_NOT;
					} else if (id == "or") {
						r_token.type = TK_OP_OR;
					} else if (id == "and") {
						r_token.type = TK_OP_AND;
					} else {
						for (int i = 0; i < Variant::VARIANT_MAX; i++) {
							if (id == Variant::get_type_name(Variant::Type(i))) {
								r_token.type = TK_BASIC_TYPE;
								r_token.value = i;
								return OK;
							}
						}

						BuiltinFunc bifunc = find_function(id);
						if (bifunc != FUNC_MAX) {
							r_token.type = TK_BUILTIN_FUNC;
							r_token.value = bifunc;
							return OK;
						}

						r_token.type = TK_IDENTIFIER;
						r_token.value = id;
					}

					return OK;

				} else if (cchar == '.') {
					// Handled down there as we support '.[0-9]' as numbers above
					r_token.type = TK_PERIOD;
					return OK;

				} else {
					_set_error("Unexpected character.");
					r_token.type = TK_ERROR;
					return ERR_PARSE_ERROR;
				}
			}
		}
#undef GET_CHAR
	}

	r_token.type = TK_ERROR;
	return ERR_PARSE_ERROR;
}

bool HTMLTemplaterenderer::_execute(Dictionary &p_data, StringBuilder &p_html, ExpressionNode *p_node, Variant &r_ret, String &r_error_str) {
	return false;
}
