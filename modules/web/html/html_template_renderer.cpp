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
// set(<key (str), no need for "">, <value>)  ret value(?)  (if value is null, unset)
// set_if(<bool expr>, <key (str), no need for "">, <value>)  ret value(?)  (if value is null, unset)
// get(<ket (str), no need for "">, <default>)
// int(), float(), str()
// range(<var1>, <var2 opt>, <var 3 opt>)
// keys(<var1 Dict>)
// values(<var1 Dict>)
// tr() translate
// trt()
// nltobr(); and/or maybe br()? there is also pbr
// sel(<bool>, <if true val>, <if false val>), select() ? s() ?

// {{ <var> }} Should this still remain an alternate to {{ p(<var1>) }} ?
// maybe just an other syntax for supressed output?:
// {{%  }}
// {{#  }}
// {{:  }}
// {{;  }}
// Or maybe:
// {{p <var1> }}
// {{% <var1> }}
// {{# <var1> }}
// {{= <var1> }}
// {{: <var1> }}
// p, pr, etc methods should return strings, then these should just work

// Could be:
// PrintNode -> bool raw,bool supressed, fill in during compile
// So {{ <expr> }} : whatever is returned gets printed as-is, maybe except for nulls, escaped (except it the outer ENode is pr, pbr, etc)
// So {{; <expr> }} Run expression, no output
// So {{% <expr> }} Explicit raw

// Comments?
// {{ ... // <comment up to }}>  }}
// {{ /* }}
// commented out
// {{ */ }}
// Tokenizer could just start ignoring everything after /* in non text mode up to */, so technically it doesn't even need }}{{
// {{ stuff <next token>/* <tokenizer just ignores it>  ....}} {{ .........  */ }} Netx token would be }}
// Or could return it as TK_COMMENT for debugging

// No assignment operator, set should be good enough

// operators: All variant ops including in  (note that it can't do assignment)

// ternary _template_text:  Not good enough, needs raw, normal version etc. sel() method instead, or maybe normal ternary operator: <> ? <> : <>
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

#include "core/string/string_buffer.h"
#include "core/string/string_builder.h"

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
	_error_set = false;
	str_ofs = 0;
	_tokenizer_in_text_mode = true;

	_root = NULL;
	_nodes = NULL;

	_execution_error = false;
	_dirty = true;
}

HTMLTemplaterenderer::~HTMLTemplaterenderer() {
	// root will also be freed automatically
	if (_nodes) {
		memdelete(_nodes);
	}
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

String HTMLTemplaterenderer::get_func_name(BuiltinFunc p_func) {
	ERR_FAIL_INDEX_V(p_func, FUNC_MAX, String());
	return func_name[p_func];
}

int HTMLTemplaterenderer::get_func_argument_count(BuiltinFunc p_func) {
#if 0
	switch (p_func) {
		case FUNC_MAX: {
		}
	}
#endif
	return 0;
}

#define VALIDATE_ARG_NUM(m_arg)                                          \
	if (!p_inputs[m_arg]->is_num()) {                                    \
		r_error.error = Variant::CallError::CALL_ERROR_INVALID_ARGUMENT; \
		r_error.argument = m_arg;                                        \
		r_error.expected = Variant::REAL;                                \
		return;                                                          \
	}

void HTMLTemplaterenderer::exec_func(BuiltinFunc p_func, const Variant **p_inputs, Variant *r_return, Variant::CallError &r_error, String &r_error_str) {
	r_error.error = Variant::CallError::CALL_OK;
#if 0
	switch (p_func) {
		default: {
		}
	}
#endif
}

HTMLTemplaterenderer::BuiltinFunc HTMLTemplaterenderer::find_function(const String &p_string) {
	for (int i = 0; i < FUNC_MAX; i++) {
		if (p_string == func_name[i]) {
			return BuiltinFunc(i);
		}
	}

	return FUNC_MAX;
}

const char *HTMLTemplaterenderer::token_name[TK_MAX] = {
	"HTML DATA",
	"CURLY BRACKET OPEN",
	"CURLY BRACKET CLOSE",
	"DOUBLE CURLY BRACKET OPEN",
	"DOUBLE CURLY BRACKET CLOSE",
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
	// Text mode

	if (_tokenizer_in_text_mode) {
		r_token.type = TK_HTML_DATA;

		StringBuffer<256> buf;

		while (true) {
#define GET_CHAR() (str_ofs >= _template_text.length() ? 0 : _template_text[str_ofs++])

			CharType cchar = GET_CHAR();

			switch (cchar) {
				case 0: {
					if (buf.length() == 0) {
						r_token.type = TK_EOF;
					} else {
						r_token.value = buf.as_string();
					}

					return OK;
				};
				case '{': {
					if (_template_text[str_ofs] == '{') {
						// {{

						if (buf.length() != 0) {
							--str_ofs; // so next token will be double curly
							r_token.value = buf.as_string();
						} else {
							r_token.type = TK_DOUBLE_CURLY_BRACKET_OPEN;
						}

						return OK;
					} else if (_template_text[str_ofs] == '\\') {
						// Escapes
						// {\{ -> {{ in html, {\\{ -> {\{ in html, {\\\{ -> {\\{ in html

						// Consuming a single \ (this one) should do the trick
						++str_ofs;
					} else {
						// Just a { in html
						buf.append(cchar);
					}

					break;
				};
				default: {
					buf.append(cchar);
					break;
				};
			}
#undef GET_CHAR
		}

		// Should never get here
		r_token.type = TK_ERROR;
		return OK;
	}

	// in expression mode
	while (true) {
#define GET_CHAR() (str_ofs >= _template_text.length() ? 0 : _template_text[str_ofs++])

		CharType cchar = GET_CHAR();

		switch (cchar) {
			case 0: {
				r_token.type = TK_EOF;
				return OK;
			};
			case '{': {
				if (_template_text[str_ofs] == '{') {
					r_token.type = TK_DOUBLE_CURLY_BRACKET_OPEN;
					str_ofs++;
				} else {
					r_token.type = TK_CURLY_BRACKET_OPEN;
				}

				return OK;
			};
			case '}': {
				if (_template_text[str_ofs] == '}') {
					r_token.type = TK_DOUBLE_CURLY_BRACKET_CLOSE;
					str_ofs++;
				} else {
					r_token.type = TK_CURLY_BRACKET_CLOSE;
				}

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

HTMLTemplaterenderer::ENode *HTMLTemplaterenderer::_parse_expression() {
	Vector<ExpressionNode> expression;

	while (true) {
		//keep appending stuff to expression
		ENode *expr = nullptr;

		Token tk;
		_get_token(tk);
		if (_error_set) {
			return nullptr;
		}

		switch (tk.type) {
			//if htmldata
			//if just return new enode.
			//expression should be empty, but that probably can't happen, it's a parser bug
			//double open
			//if true unexoected
			//_tokenizer_in_text_mode = false;
			//double close
			//_tokenizer_in_text_mode = true; or onexpoected
			case TK_CURLY_BRACKET_OPEN: {
				//a dictionary
				DictionaryNode *dn = alloc_node<DictionaryNode>();

				while (true) {
					int cofs = str_ofs;
					_get_token(tk);
					if (tk.type == TK_CURLY_BRACKET_CLOSE) {
						break;
					}
					str_ofs = cofs; //revert
					//parse an expression
					ENode *subexpr = _parse_expression();
					if (!subexpr) {
						return nullptr;
					}
					dn->dict.push_back(subexpr);

					_get_token(tk);
					if (tk.type != TK_COLON) {
						_set_error("Expected ':'");
						return nullptr;
					}

					subexpr = _parse_expression();
					if (!subexpr) {
						return nullptr;
					}

					dn->dict.push_back(subexpr);

					cofs = str_ofs;
					_get_token(tk);
					if (tk.type == TK_COMMA) {
						//all good
					} else if (tk.type == TK_CURLY_BRACKET_CLOSE) {
						str_ofs = cofs;
					} else {
						_set_error("Expected ',' or '}'");
					}
				}

				expr = dn;
			} break;
			case TK_BRACKET_OPEN: {
				//an array

				ArrayNode *an = alloc_node<ArrayNode>();

				while (true) {
					int cofs = str_ofs;
					_get_token(tk);
					if (tk.type == TK_BRACKET_CLOSE) {
						break;
					}
					str_ofs = cofs; //revert
					//parse an expression
					ENode *subexpr = _parse_expression();
					if (!subexpr) {
						return nullptr;
					}
					an->array.push_back(subexpr);

					cofs = str_ofs;
					_get_token(tk);
					if (tk.type == TK_COMMA) {
						//all good
					} else if (tk.type == TK_BRACKET_CLOSE) {
						str_ofs = cofs;
					} else {
						_set_error("Expected ',' or ']'");
					}
				}

				expr = an;
			} break;
			case TK_PARENTHESIS_OPEN: {
				//a suexpression
				ENode *e = _parse_expression();
				if (_error_set) {
					return nullptr;
				}
				_get_token(tk);
				if (tk.type != TK_PARENTHESIS_CLOSE) {
					_set_error("Expected ')'");
					return nullptr;
				}

				expr = e;

			} break;
			case TK_IDENTIFIER: {
				String identifier = tk.value;

				int cofs = str_ofs;
				_get_token(tk);
				if (tk.type == TK_PARENTHESIS_OPEN) {
					//function call
					CallNode *func_call = alloc_node<CallNode>();
					func_call->method = identifier;
					SelfNode *self_node = alloc_node<SelfNode>();
					func_call->base = self_node;

					while (true) {
						int cofs2 = str_ofs;
						_get_token(tk);
						if (tk.type == TK_PARENTHESIS_CLOSE) {
							break;
						}
						str_ofs = cofs2; //revert
						//parse an expression
						ENode *subexpr = _parse_expression();
						if (!subexpr) {
							return nullptr;
						}

						func_call->arguments.push_back(subexpr);

						cofs2 = str_ofs;
						_get_token(tk);
						if (tk.type == TK_COMMA) {
							//all good
						} else if (tk.type == TK_PARENTHESIS_CLOSE) {
							str_ofs = cofs2;
						} else {
							_set_error("Expected ',' or ')'");
						}
					}

					expr = func_call;
				} else {
					//named indexing
					str_ofs = cofs;

					InputNode *input = alloc_node<InputNode>();
					input->name = identifier;
					expr = input;
				}
			} break;
			case TK_CONSTANT: {
				ConstantNode *constant = alloc_node<ConstantNode>();
				constant->value = tk.value;
				expr = constant;
			} break;
			case TK_BASIC_TYPE: {
				//constructor..

				Variant::Type bt = Variant::Type(int(tk.value));
				_get_token(tk);
				if (tk.type != TK_PARENTHESIS_OPEN) {
					_set_error("Expected '('");
					return nullptr;
				}

				ConstructorNode *constructor = alloc_node<ConstructorNode>();
				constructor->data_type = bt;

				while (true) {
					int cofs = str_ofs;
					_get_token(tk);
					if (tk.type == TK_PARENTHESIS_CLOSE) {
						break;
					}
					str_ofs = cofs; //revert
					//parse an expression
					ENode *subexpr = _parse_expression();
					if (!subexpr) {
						return nullptr;
					}

					constructor->arguments.push_back(subexpr);

					cofs = str_ofs;
					_get_token(tk);
					if (tk.type == TK_COMMA) {
						//all good
					} else if (tk.type == TK_PARENTHESIS_CLOSE) {
						str_ofs = cofs;
					} else {
						_set_error("Expected ',' or ')'");
					}
				}

				expr = constructor;

			} break;
			case TK_BUILTIN_FUNC: {
				//builtin function

				_get_token(tk);
				if (tk.type != TK_PARENTHESIS_OPEN) {
					_set_error("Expected '('");
					return nullptr;
				}

				BuiltinFuncNode *bifunc = alloc_node<BuiltinFuncNode>();
				bifunc->func = BuiltinFunc(int(tk.value));

				while (true) {
					int cofs = str_ofs;
					_get_token(tk);
					if (tk.type == TK_PARENTHESIS_CLOSE) {
						break;
					}
					str_ofs = cofs; //revert
					//parse an expression
					ENode *subexpr = _parse_expression();
					if (!subexpr) {
						return nullptr;
					}

					bifunc->arguments.push_back(subexpr);

					cofs = str_ofs;
					_get_token(tk);
					if (tk.type == TK_COMMA) {
						//all good
					} else if (tk.type == TK_PARENTHESIS_CLOSE) {
						str_ofs = cofs;
					} else {
						_set_error("Expected ',' or ')'");
					}
				}

				int expected_args = get_func_argument_count(bifunc->func);
				if (bifunc->arguments.size() != expected_args) {
					_set_error("Builtin func '" + get_func_name(bifunc->func) + "' expects " + itos(expected_args) + " arguments.");
				}

				expr = bifunc;

			} break;
			case TK_OP_SUB: {
				ExpressionNode e;
				e.is_op = true;
				e.op = Variant::OP_NEGATE;
				expression.push_back(e);
				continue;
			} break;
			case TK_OP_NOT: {
				ExpressionNode e;
				e.is_op = true;
				e.op = Variant::OP_NOT;
				expression.push_back(e);
				continue;
			} break;

			default: {
				_set_error("Expected expression.");
				return nullptr;
			} break;
		}

		//before going to operators, must check indexing!

		while (true) {
			int cofs2 = str_ofs;
			_get_token(tk);
			if (_error_set) {
				return nullptr;
			}

			bool done = false;

			switch (tk.type) {
				case TK_BRACKET_OPEN: {
					//value indexing

					IndexNode *index = alloc_node<IndexNode>();
					index->base = expr;

					ENode *what = _parse_expression();
					if (!what) {
						return nullptr;
					}

					index->index = what;

					_get_token(tk);
					if (tk.type != TK_BRACKET_CLOSE) {
						_set_error("Expected ']' at end of index.");
						return nullptr;
					}
					expr = index;

				} break;
				case TK_PERIOD: {
					//named indexing or function call
					_get_token(tk);
					if (tk.type != TK_IDENTIFIER) {
						_set_error("Expected identifier after '.'");
						return nullptr;
					}

					StringName identifier = tk.value;

					int cofs = str_ofs;
					_get_token(tk);
					if (tk.type == TK_PARENTHESIS_OPEN) {
						//function call
						CallNode *func_call = alloc_node<CallNode>();
						func_call->method = identifier;
						func_call->base = expr;

						while (true) {
							int cofs3 = str_ofs;
							_get_token(tk);
							if (tk.type == TK_PARENTHESIS_CLOSE) {
								break;
							}
							str_ofs = cofs3; //revert
							//parse an expression
							ENode *subexpr = _parse_expression();
							if (!subexpr) {
								return nullptr;
							}

							func_call->arguments.push_back(subexpr);

							cofs3 = str_ofs;
							_get_token(tk);
							if (tk.type == TK_COMMA) {
								//all good
							} else if (tk.type == TK_PARENTHESIS_CLOSE) {
								str_ofs = cofs3;
							} else {
								_set_error("Expected ',' or ')'");
							}
						}

						expr = func_call;
					} else {
						//named indexing
						str_ofs = cofs;

						NamedIndexNode *index = alloc_node<NamedIndexNode>();
						index->base = expr;
						index->name = identifier;
						expr = index;
					}

				} break;
				default: {
					str_ofs = cofs2;
					done = true;
				} break;
			}

			if (done) {
				break;
			}
		}

		//push expression
		{
			ExpressionNode e;
			e.is_op = false;
			e.node = expr;
			expression.push_back(e);
		}

		//ok finally look for an operator

		int cofs = str_ofs;
		_get_token(tk);
		if (_error_set) {
			return nullptr;
		}

		Variant::Operator op = Variant::OP_MAX;

		switch (tk.type) {
			case TK_OP_IN:
				op = Variant::OP_IN;
				break;
			case TK_OP_EQUAL:
				op = Variant::OP_EQUAL;
				break;
			case TK_OP_NOT_EQUAL:
				op = Variant::OP_NOT_EQUAL;
				break;
			case TK_OP_LESS:
				op = Variant::OP_LESS;
				break;
			case TK_OP_LESS_EQUAL:
				op = Variant::OP_LESS_EQUAL;
				break;
			case TK_OP_GREATER:
				op = Variant::OP_GREATER;
				break;
			case TK_OP_GREATER_EQUAL:
				op = Variant::OP_GREATER_EQUAL;
				break;
			case TK_OP_AND:
				op = Variant::OP_AND;
				break;
			case TK_OP_OR:
				op = Variant::OP_OR;
				break;
			case TK_OP_NOT:
				op = Variant::OP_NOT;
				break;
			case TK_OP_ADD:
				op = Variant::OP_ADD;
				break;
			case TK_OP_SUB:
				op = Variant::OP_SUBTRACT;
				break;
			case TK_OP_MUL:
				op = Variant::OP_MULTIPLY;
				break;
			case TK_OP_DIV:
				op = Variant::OP_DIVIDE;
				break;
			case TK_OP_MOD:
				op = Variant::OP_MODULE;
				break;
			case TK_OP_SHIFT_LEFT:
				op = Variant::OP_SHIFT_LEFT;
				break;
			case TK_OP_SHIFT_RIGHT:
				op = Variant::OP_SHIFT_RIGHT;
				break;
			case TK_OP_BIT_AND:
				op = Variant::OP_BIT_AND;
				break;
			case TK_OP_BIT_OR:
				op = Variant::OP_BIT_OR;
				break;
			case TK_OP_BIT_XOR:
				op = Variant::OP_BIT_XOR;
				break;
			case TK_OP_BIT_INVERT:
				op = Variant::OP_BIT_NEGATE;
				break;
			default: {
			};
		}

		if (op == Variant::OP_MAX) { //stop appending stuff
			str_ofs = cofs;
			break;
		}

		//push operator and go on
		{
			ExpressionNode e;
			e.is_op = true;
			e.op = op;
			expression.push_back(e);
		}
	}

	/* Reduce the set set of expressions and place them in an operator tree, respecting precedence */

	while (expression.size() > 1) {
		int next_op = -1;
		int min_priority = 0xFFFFF;
		bool is_unary = false;

		for (int i = 0; i < expression.size(); i++) {
			if (!expression[i].is_op) {
				continue;
			}

			int priority;

			bool unary = false;

			switch (expression[i].op) {
				case Variant::OP_BIT_NEGATE:
					priority = 0;
					unary = true;
					break;
				case Variant::OP_NEGATE:
					priority = 1;
					unary = true;
					break;

				case Variant::OP_MULTIPLY:
					priority = 2;
					break;
				case Variant::OP_DIVIDE:
					priority = 2;
					break;
				case Variant::OP_MODULE:
					priority = 2;
					break;

				case Variant::OP_ADD:
					priority = 3;
					break;
				case Variant::OP_SUBTRACT:
					priority = 3;
					break;

				case Variant::OP_SHIFT_LEFT:
					priority = 4;
					break;
				case Variant::OP_SHIFT_RIGHT:
					priority = 4;
					break;

				case Variant::OP_BIT_AND:
					priority = 5;
					break;
				case Variant::OP_BIT_XOR:
					priority = 6;
					break;
				case Variant::OP_BIT_OR:
					priority = 7;
					break;

				case Variant::OP_LESS:
					priority = 8;
					break;
				case Variant::OP_LESS_EQUAL:
					priority = 8;
					break;
				case Variant::OP_GREATER:
					priority = 8;
					break;
				case Variant::OP_GREATER_EQUAL:
					priority = 8;
					break;

				case Variant::OP_EQUAL:
					priority = 8;
					break;
				case Variant::OP_NOT_EQUAL:
					priority = 8;
					break;

				case Variant::OP_IN:
					priority = 10;
					break;

				case Variant::OP_NOT:
					priority = 11;
					unary = true;
					break;
				case Variant::OP_AND:
					priority = 12;
					break;
				case Variant::OP_OR:
					priority = 13;
					break;

				default: {
					_set_error("Parser bug, invalid operator in expression: " + itos(expression[i].op));
					return nullptr;
				}
			}

			if (priority < min_priority) {
				// < is used for left to right (default)
				// <= is used for right to left

				next_op = i;
				min_priority = priority;
				is_unary = unary;
			}
		}

		if (next_op == -1) {
			_set_error("Yet another parser bug....");
			ERR_FAIL_V(nullptr);
		}

		// OK! create operator..
		if (is_unary) {
			int expr_pos = next_op;
			while (expression[expr_pos].is_op) {
				expr_pos++;
				if (expr_pos == expression.size()) {
					//can happen..
					_set_error("Unexpected end of expression...");
					return nullptr;
				}
			}

			//consecutively do unary operators
			for (int i = expr_pos - 1; i >= next_op; i--) {
				OperatorNode *op = alloc_node<OperatorNode>();
				op->op = expression[i].op;
				op->nodes[0] = expression[i + 1].node;
				op->nodes[1] = nullptr;
				expression.write[i].is_op = false;
				expression.write[i].node = op;
				expression.remove(i + 1);
			}

		} else {
			if (next_op < 1 || next_op >= (expression.size() - 1)) {
				_set_error("Parser bug...");
				ERR_FAIL_V(nullptr);
			}

			OperatorNode *op = alloc_node<OperatorNode>();
			op->op = expression[next_op].op;

			if (expression[next_op - 1].is_op) {
				_set_error("Parser bug...");
				ERR_FAIL_V(nullptr);
			}

			if (expression[next_op + 1].is_op) {
				// this is not invalid and can really appear
				// but it becomes invalid anyway because no binary op
				// can be followed by a unary op in a valid combination,
				// due to how precedence works, unaries will always disappear first

				_set_error("Unexpected two consecutive operators.");
				return nullptr;
			}

			op->nodes[0] = expression[next_op - 1].node; //expression goes as left
			op->nodes[1] = expression[next_op + 1].node; //next expression goes as right

			//replace all 3 nodes by this operator and make it an expression
			expression.write[next_op - 1].node = op;
			expression.remove(next_op);
			expression.remove(next_op);
		}
	}

	return expression[0].node;
}

bool HTMLTemplaterenderer::_compile_expression() {
	if (!_dirty) {
		return _error_set;
	}

	if (_nodes) {
		memdelete(_nodes);
		_nodes = nullptr;
		_root = nullptr;
	}

	_error_str = String();
	_error_set = false;
	str_ofs = 0;

	// TODO
	//_root = _parse_expression();

	if (_error_set) {
		_root = nullptr;
		if (_nodes) {
			memdelete(_nodes);
		}
		_nodes = nullptr;
		return true;
	}

	_dirty = false;
	return false;
}

bool HTMLTemplaterenderer::_execute(Dictionary &p_data, StringBuilder &p_html, ENode *p_node, Variant &r_ret, String &r_error_str) {
	return false;
}
