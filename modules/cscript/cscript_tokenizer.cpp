/*************************************************************************/
/*  cscript_tokenizer.cpp                                                */
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

#include "cscript_tokenizer.h"

#include "core/containers/rb_map.h"
#include "core/io/marshalls.h"
#include "core/string/print_string.h"
#include "cscript_functions.h"

OAHashMap<String, int> *CScriptTokenizer::token_hashtable = NULL;

const char *CScriptTokenizer::token_names[TK_MAX] = {
	"Empty",
	"Identifier",
	"Constant",
	"Self",
	"Built-In Type",
	"Built-In Func",
	"In",
	"'=='",
	"'!='",
	"'<'",
	"'<='",
	"'>'",
	"'>='",
	"'and'",
	"'or'",
	"'not'",
	"'+'",
	"'-'",
	"'*'",
	"'/'",
	"'%'",
	"'<<'",
	"'>>'",
	"'='",
	"'+='",
	"'-='",
	"'*='",
	"'/='",
	"'%='",
	"'<<='",
	"'>>='",
	"'&='",
	"'|='",
	"'^='",
	"'&'",
	"'|'",
	"'^'",
	"'~'",
	//"Plus Plus",
	//"Minus Minus",
	"if",
	"elif",
	"else",
	"for",
	"while",
	"break",
	"continue",
	"pass",
	"return",
	"match",
	"func",
	"class",
	"class_name",
	"extends",
	"is",
	"onready",
	"tool",
	"static",
	"export",
	"setget",
	"const",
	"var",
	"as",
	"void",
	"enum",
	"preload",
	"assert",
	"signal",
	"breakpoint",
	"'['",
	"']'",
	"'{'",
	"'}'",
	"'('",
	"')'",
	"','",
	"';'",
	"'.'",
	"'?'",
	"':'",
	"'$'",
	"'->'",
	"'\\n'",
	"PI",
	"TAU",
	"_",
	"INF",
	"NAN",
	"Error",
	"EOF",
	"Cursor"
};

struct _bit {
	Variant::Type type;
	const char *text;
};
//built in types

static const _bit _type_list[] = {
	//types
	{ Variant::BOOL, "bool" },
	{ Variant::INT, "int" },
	{ Variant::REAL, "float" },
	{ Variant::STRING, "String" },
	{ Variant::RECT2, "Rect2" },
	{ Variant::RECT2I, "Rect2i" },
	{ Variant::VECTOR2, "Vector2" },
	{ Variant::VECTOR2I, "Vector2i" },
	{ Variant::VECTOR3, "Vector3" },
	{ Variant::VECTOR3I, "Vector3i" },
	{ Variant::VECTOR4, "Vector4" },
	{ Variant::VECTOR4I, "Vector4i" },
	{ Variant::PLANE, "Plane" },
	{ Variant::QUATERNION, "Quaternion" },
	{ Variant::AABB, "AABB" },
	{ Variant::BASIS, "Basis" },
	{ Variant::TRANSFORM, "Transform" },
	{ Variant::TRANSFORM2D, "Transform2D" },
	{ Variant::PROJECTION, "Projection" },
	{ Variant::COLOR, "Color" },
	{ Variant::NODE_PATH, "NodePath" },
	{ Variant::RID, "RID" },
	{ Variant::OBJECT, "Object" },
	{ Variant::STRING_NAME, "StringName" },
	{ Variant::DICTIONARY, "Dictionary" },
	{ Variant::ARRAY, "Array" },
	{ Variant::POOL_BYTE_ARRAY, "PoolByteArray" },
	{ Variant::POOL_INT_ARRAY, "PoolIntArray" },
	{ Variant::POOL_REAL_ARRAY, "PoolRealArray" },
	{ Variant::POOL_STRING_ARRAY, "PoolStringArray" },
	{ Variant::POOL_VECTOR2_ARRAY, "PoolVector2Array" },
	{ Variant::POOL_VECTOR2I_ARRAY, "PoolVector2iArray" },
	{ Variant::POOL_VECTOR3_ARRAY, "PoolVector3Array" },
	{ Variant::POOL_VECTOR3I_ARRAY, "PoolVector3iArray" },
	{ Variant::POOL_VECTOR4_ARRAY, "PoolVector4Array" },
	{ Variant::POOL_VECTOR4I_ARRAY, "PoolVector4iArray" },
	{ Variant::POOL_COLOR_ARRAY, "PoolColorArray" },
	{ Variant::VARIANT_MAX, nullptr },
};

struct _kws {
	CScriptTokenizer::Token token;
	const char *text;
};

static const _kws _keyword_list[] = {
	//ops
	{ CScriptTokenizer::TK_OP_IN, "in" },
	{ CScriptTokenizer::TK_OP_NOT, "not" },
	{ CScriptTokenizer::TK_OP_OR, "or" },
	{ CScriptTokenizer::TK_OP_AND, "and" },
	//func
	{ CScriptTokenizer::TK_PR_FUNCTION, "func" },
	{ CScriptTokenizer::TK_PR_CLASS, "class" },
	{ CScriptTokenizer::TK_PR_CLASS_NAME, "class_name" },
	{ CScriptTokenizer::TK_PR_EXTENDS, "extends" },
	{ CScriptTokenizer::TK_PR_IS, "is" },
	{ CScriptTokenizer::TK_PR_ONREADY, "onready" },
	{ CScriptTokenizer::TK_PR_TOOL, "tool" },
	{ CScriptTokenizer::TK_PR_STATIC, "static" },
	{ CScriptTokenizer::TK_PR_EXPORT, "export" },
	{ CScriptTokenizer::TK_PR_SETGET, "setget" },
	{ CScriptTokenizer::TK_PR_VAR, "var" },
	{ CScriptTokenizer::TK_PR_AS, "as" },
	{ CScriptTokenizer::TK_PR_VOID, "void" },
	{ CScriptTokenizer::TK_PR_PRELOAD, "preload" },
	{ CScriptTokenizer::TK_PR_ASSERT, "assert" },
	{ CScriptTokenizer::TK_PR_SIGNAL, "signal" },
	{ CScriptTokenizer::TK_PR_BREAKPOINT, "breakpoint" },
	{ CScriptTokenizer::TK_PR_CONST, "const" },
	{ CScriptTokenizer::TK_PR_ENUM, "enum" },
	//controlflow
	{ CScriptTokenizer::TK_CF_IF, "if" },
	{ CScriptTokenizer::TK_CF_ELIF, "elif" },
	{ CScriptTokenizer::TK_CF_ELSE, "else" },
	{ CScriptTokenizer::TK_CF_FOR, "for" },
	{ CScriptTokenizer::TK_CF_WHILE, "while" },
	{ CScriptTokenizer::TK_CF_BREAK, "break" },
	{ CScriptTokenizer::TK_CF_CONTINUE, "continue" },
	{ CScriptTokenizer::TK_CF_RETURN, "return" },
	{ CScriptTokenizer::TK_CF_MATCH, "match" },
	{ CScriptTokenizer::TK_CF_PASS, "pass" },
	{ CScriptTokenizer::TK_SELF, "self" },
	{ CScriptTokenizer::TK_CONST_PI, "PI" },
	{ CScriptTokenizer::TK_CONST_TAU, "TAU" },
	{ CScriptTokenizer::TK_WILDCARD, "_" },
	{ CScriptTokenizer::TK_CONST_INF, "INF" },
	{ CScriptTokenizer::TK_CONST_NAN, "NAN" },
	{ CScriptTokenizer::TK_ERROR, nullptr }
};

// Prepare the hash table for parsing as a one off at startup.
void CScriptTokenizer::initialize() {
	token_hashtable = memnew((OAHashMap<String, int>));

	token_hashtable->insert("null", 0);
	token_hashtable->insert("true", 1);
	token_hashtable->insert("false", 2);

	// _type_list
	int id = TOKEN_HASH_TABLE_TYPE_START;
	int idx = 0;
	while (_type_list[idx].text) {
		token_hashtable->insert(_type_list[idx].text, id++);
		idx++;
	}

	// built in funcs
	id = TOKEN_HASH_TABLE_BUILTIN_START;
	for (int j = 0; j < CScriptFunctions::FUNC_MAX; j++) {
		token_hashtable->insert(CScriptFunctions::get_func_name(CScriptFunctions::Function(j)), id++);
	}

	// keywords
	id = TOKEN_HASH_TABLE_KEYWORD_START;
	idx = 0;
	while (_keyword_list[idx].text) {
		token_hashtable->insert(_keyword_list[idx].text, id++);
		idx++;
	}
}

void CScriptTokenizer::terminate() {
	if (token_hashtable) {
		memdelete(token_hashtable);
		token_hashtable = nullptr;
	}
}

// return whether found
bool CScriptTokenizerText::_parse_identifier(const String &p_str) {
	// N.B. CScriptTokenizer::initialize() must have been called before using this function,
	// else token_hashtable will be NULL.
	const int *found = token_hashtable->lookup_ptr(p_str);

	if (found) {
		int id = *found;
		if (id < TOKEN_HASH_TABLE_TYPE_START) {
			switch (id) {
				case 0: {
					_make_constant(Variant());
				} break;
				case 1: {
					_make_constant(true);
				} break;
				case 2: {
					_make_constant(false);
				} break;
				default: {
					DEV_ASSERT(0);
				} break;
			}
			return true;
		} else {
			// type list
			if (id < TOKEN_HASH_TABLE_BUILTIN_START) {
				int idx = id - TOKEN_HASH_TABLE_TYPE_START;
				_make_type(_type_list[idx].type);
				return true;
			}

			// built in func
			if (id < TOKEN_HASH_TABLE_KEYWORD_START) {
				int idx = id - TOKEN_HASH_TABLE_BUILTIN_START;
				_make_built_in_func(CScriptFunctions::Function(idx));
				return true;
			}

			// keyword
			int idx = id - TOKEN_HASH_TABLE_KEYWORD_START;
			_make_token(_keyword_list[idx].token);
			return true;
		}

		return true;
	}

	// not found
	return false;
}

const char *CScriptTokenizer::get_token_name(Token p_token) {
	ERR_FAIL_INDEX_V(p_token, TK_MAX, "<error>");
	return token_names[p_token];
}

bool CScriptTokenizer::is_token_literal(int p_offset, bool variable_safe) const {
	switch (get_token(p_offset)) {
		// Can always be literal:
		case TK_IDENTIFIER:

		case TK_PR_ONREADY:
		case TK_PR_TOOL:
		case TK_PR_STATIC:
		case TK_PR_EXPORT:
		case TK_PR_SETGET:
		case TK_PR_SIGNAL:
			return true;

		// Literal for non-variables only:
		case TK_BUILT_IN_TYPE:
		case TK_BUILT_IN_FUNC:

		case TK_OP_IN:
			//case TK_OP_NOT:
			//case TK_OP_OR:
			//case TK_OP_AND:

		case TK_PR_CLASS:
		case TK_PR_CONST:
		case TK_PR_ENUM:
		case TK_PR_PRELOAD:
		case TK_PR_FUNCTION:
		case TK_PR_EXTENDS:
		case TK_PR_ASSERT:
		case TK_PR_VAR:

		case TK_CF_IF:
		case TK_CF_ELIF:
		case TK_CF_ELSE:
		case TK_CF_FOR:
		case TK_CF_WHILE:
		case TK_CF_BREAK:
		case TK_CF_CONTINUE:
		case TK_CF_RETURN:
		case TK_CF_MATCH:
		case TK_CF_PASS:
		case TK_SELF:
		case TK_CONST_PI:
		case TK_CONST_TAU:
		case TK_WILDCARD:
		case TK_CONST_INF:
		case TK_CONST_NAN:
		case TK_ERROR:
			return !variable_safe;

		case TK_CONSTANT: {
			switch (get_token_constant(p_offset).get_type()) {
				case Variant::NIL:
				case Variant::BOOL:
					return true;
				default:
					return false;
			}
		}
		default:
			return false;
	}
}

StringName CScriptTokenizer::get_token_literal(int p_offset) const {
	Token token = get_token(p_offset);
	switch (token) {
		case TK_IDENTIFIER:
			return get_token_identifier(p_offset);
		case TK_BUILT_IN_TYPE: {
			Variant::Type type = get_token_type(p_offset);
			int idx = 0;

			while (_type_list[idx].text) {
				if (type == _type_list[idx].type) {
					return _type_list[idx].text;
				}
				idx++;
			}
		} break; // Shouldn't get here, stuff happens
		case TK_BUILT_IN_FUNC:
			return CScriptFunctions::get_func_name(get_token_built_in_func(p_offset));
		case TK_CONSTANT: {
			const Variant value = get_token_constant(p_offset);

			switch (value.get_type()) {
				case Variant::NIL:
					return "null";
				case Variant::BOOL:
					return value ? "true" : "false";
				default: {
				}
			}
		}
		case TK_OP_AND:
		case TK_OP_OR:
			break; // Don't get into default, since they can be non-literal
		default: {
			int idx = 0;

			while (_keyword_list[idx].text) {
				if (token == _keyword_list[idx].token) {
					return _keyword_list[idx].text;
				}
				idx++;
			}
		}
	}
	ERR_FAIL_V_MSG("", "Failed to get token literal.");
}

static bool _is_text_char(CharType c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_';
}

static bool _is_number(CharType c) {
	return (c >= '0' && c <= '9');
}

static bool _is_hex(CharType c) {
	return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

static bool _is_bin(CharType c) {
	return (c == '0' || c == '1');
}

void CScriptTokenizerText::_make_token(Token p_type) {
	TokenData &tk = tk_rb[tk_rb_pos];

	tk.type = p_type;
	tk.line = line;
	tk.col = column;

	tk_rb_pos = (tk_rb_pos + 1) % TK_RB_SIZE;
}
void CScriptTokenizerText::_make_identifier(const StringName &p_identifier) {
	TokenData &tk = tk_rb[tk_rb_pos];

	tk.type = TK_IDENTIFIER;
	tk.identifier = p_identifier;
	tk.line = line;
	tk.col = column;

	tk_rb_pos = (tk_rb_pos + 1) % TK_RB_SIZE;
}

void CScriptTokenizerText::_make_built_in_func(CScriptFunctions::Function p_func) {
	TokenData &tk = tk_rb[tk_rb_pos];

	tk.type = TK_BUILT_IN_FUNC;
	tk.func = p_func;
	tk.line = line;
	tk.col = column;

	tk_rb_pos = (tk_rb_pos + 1) % TK_RB_SIZE;
}
void CScriptTokenizerText::_make_constant(const Variant &p_constant) {
	TokenData &tk = tk_rb[tk_rb_pos];

	tk.type = TK_CONSTANT;
	tk.constant = p_constant;
	tk.line = line;
	tk.col = column;

	tk_rb_pos = (tk_rb_pos + 1) % TK_RB_SIZE;
}

void CScriptTokenizerText::_make_type(const Variant::Type &p_type) {
	TokenData &tk = tk_rb[tk_rb_pos];

	tk.type = TK_BUILT_IN_TYPE;
	tk.vtype = p_type;
	tk.line = line;
	tk.col = column;

	tk_rb_pos = (tk_rb_pos + 1) % TK_RB_SIZE;
}

void CScriptTokenizerText::_make_error(const String &p_error) {
	error_flag = true;
	last_error = p_error;

	TokenData &tk = tk_rb[tk_rb_pos];
	tk.type = TK_ERROR;
	tk.constant = p_error;
	tk.line = line;
	tk.col = column;
	tk_rb_pos = (tk_rb_pos + 1) % TK_RB_SIZE;
}

void CScriptTokenizerText::_make_newline(int p_indentation, int p_tabs) {
	TokenData &tk = tk_rb[tk_rb_pos];
	tk.type = TK_NEWLINE;
	tk.constant = Vector2(p_indentation, p_tabs);
	tk.line = line;
	tk.col = column;
	tk_rb_pos = (tk_rb_pos + 1) % TK_RB_SIZE;
}

void CScriptTokenizerText::_advance() {
	if (error_flag) {
		//parser broke
		_make_error(last_error);
		return;
	}

	if (code_pos >= len) {
		_make_token(TK_EOF);
		return;
	}
#define GETCHAR(m_ofs) ((m_ofs + code_pos) >= len ? 0 : _code[m_ofs + code_pos])
#define INCPOS(m_amount)      \
	{                         \
		code_pos += m_amount; \
		column += m_amount;   \
	}
	while (true) {
		bool is_string_name = false;
		StringMode string_mode = STRING_DOUBLE_QUOTE;

		switch (GETCHAR(0)) {
			case 0:
				_make_token(TK_EOF);
				break;
			case '\\':
				INCPOS(1);
				if (GETCHAR(0) == '\r') {
					INCPOS(1);
				}

				if (GETCHAR(0) != '\n') {
					_make_error("Expected newline after '\\'.");
					return;
				}

				INCPOS(1);
				line++;

				while (GETCHAR(0) == ' ' || GETCHAR(0) == '\t') {
					INCPOS(1);
				}

				continue;
			case '\t':
			case '\r':
			case ' ':
				INCPOS(1);
				continue;
			case '#': { // line comment skip
#ifdef DEBUG_ENABLED
				String comment;
#endif // DEBUG_ENABLED
				while (GETCHAR(0) != '\n') {
#ifdef DEBUG_ENABLED
					comment += GETCHAR(0);
#endif // DEBUG_ENABLED
					code_pos++;
					if (GETCHAR(0) == 0) { //end of file
						//_make_error("Unterminated Comment");
						_make_token(TK_EOF);
						return;
					}
				}
#ifdef DEBUG_ENABLED
				String comment_content = comment.trim_prefix("#").trim_prefix(" ");
				if (comment_content.begins_with("warning-ignore:")) {
					String code = comment_content.get_slice(":", 1);
					warning_skips.push_back(Pair<int, String>(line, code.strip_edges().to_lower()));
				} else if (comment_content.begins_with("warning-ignore-all:")) {
					String code = comment_content.get_slice(":", 1);
					warning_global_skips.insert(code.strip_edges().to_lower());
				} else if (comment_content.strip_edges() == "warnings-disable") {
					ignore_warnings = true;
				}
#endif // DEBUG_ENABLED
				FALLTHROUGH;
			}
			case '\n': {
				line++;
				INCPOS(1);
				bool used_spaces = false;
				int tabs = 0;
				column = 1;
				int i = 0;
				while (true) {
					if (GETCHAR(i) == ' ') {
						i++;
						used_spaces = true;
					} else if (GETCHAR(i) == '\t') {
						if (used_spaces) {
							_make_error("Spaces used before tabs on a line");
							return;
						}
						i++;
						tabs++;
					} else {
						break; // not indentation anymore
					}
				}

				_make_newline(i, tabs);
				return;
			}
			case '/': {
				switch (GETCHAR(1)) {
					case '=': { // diveq

						_make_token(TK_OP_ASSIGN_DIV);
						INCPOS(1);

					} break;
					default:
						_make_token(TK_OP_DIV);
				}
			} break;
			case '=': {
				if (GETCHAR(1) == '=') {
					_make_token(TK_OP_EQUAL);
					INCPOS(1);

				} else {
					_make_token(TK_OP_ASSIGN);
				}

			} break;
			case '<': {
				if (GETCHAR(1) == '=') {
					_make_token(TK_OP_LESS_EQUAL);
					INCPOS(1);
				} else if (GETCHAR(1) == '<') {
					if (GETCHAR(2) == '=') {
						_make_token(TK_OP_ASSIGN_SHIFT_LEFT);
						INCPOS(1);
					} else {
						_make_token(TK_OP_SHIFT_LEFT);
					}
					INCPOS(1);
				} else {
					_make_token(TK_OP_LESS);
				}

			} break;
			case '>': {
				if (GETCHAR(1) == '=') {
					_make_token(TK_OP_GREATER_EQUAL);
					INCPOS(1);
				} else if (GETCHAR(1) == '>') {
					if (GETCHAR(2) == '=') {
						_make_token(TK_OP_ASSIGN_SHIFT_RIGHT);
						INCPOS(1);

					} else {
						_make_token(TK_OP_SHIFT_RIGHT);
					}
					INCPOS(1);
				} else {
					_make_token(TK_OP_GREATER);
				}

			} break;
			case '!': {
				if (GETCHAR(1) == '=') {
					_make_token(TK_OP_NOT_EQUAL);
					INCPOS(1);
				} else {
					_make_token(TK_OP_NOT);
				}

			} break;
			//case '"' //string - no strings in shader
			//case '\'' //string - no strings in shader
			case '{':
				_make_token(TK_CURLY_BRACKET_OPEN);
				break;
			case '}':
				_make_token(TK_CURLY_BRACKET_CLOSE);
				break;
			case '[':
				_make_token(TK_BRACKET_OPEN);
				break;
			case ']':
				_make_token(TK_BRACKET_CLOSE);
				break;
			case '(':
				_make_token(TK_PARENTHESIS_OPEN);
				break;
			case ')':
				_make_token(TK_PARENTHESIS_CLOSE);
				break;
			case ',':
				_make_token(TK_COMMA);
				break;
			case ';':
				_make_token(TK_SEMICOLON);
				break;
			case '?':
				_make_token(TK_QUESTION_MARK);
				break;
			case ':':
				_make_token(TK_COLON); //for methods maybe but now useless.
				break;
			case '$':
				_make_token(TK_DOLLAR); //for the get_node() shortener
				break;
			case '^': {
				if (GETCHAR(1) == '=') {
					_make_token(TK_OP_ASSIGN_BIT_XOR);
					INCPOS(1);
				} else {
					_make_token(TK_OP_BIT_XOR);
				}

			} break;
			case '~':
				_make_token(TK_OP_BIT_INVERT);
				break;
			case '&': {
				if (GETCHAR(1) == '&') {
					_make_token(TK_OP_AND);
					INCPOS(1);
				} else if (GETCHAR(1) == '=') {
					_make_token(TK_OP_ASSIGN_BIT_AND);
					INCPOS(1);
				} else {
					_make_token(TK_OP_BIT_AND);
				}
			} break;
			case '|': {
				if (GETCHAR(1) == '|') {
					_make_token(TK_OP_OR);
					INCPOS(1);
				} else if (GETCHAR(1) == '=') {
					_make_token(TK_OP_ASSIGN_BIT_OR);
					INCPOS(1);
				} else {
					_make_token(TK_OP_BIT_OR);
				}
			} break;
			case '*': {
				if (GETCHAR(1) == '=') {
					_make_token(TK_OP_ASSIGN_MUL);
					INCPOS(1);
				} else {
					_make_token(TK_OP_MUL);
				}
			} break;
			case '+': {
				if (GETCHAR(1) == '=') {
					_make_token(TK_OP_ASSIGN_ADD);
					INCPOS(1);
					/*
				}  else if (GETCHAR(1)=='+') {
					_make_token(TK_OP_PLUS_PLUS);
					INCPOS(1);
				*/
				} else {
					_make_token(TK_OP_ADD);
				}

			} break;
			case '-': {
				if (GETCHAR(1) == '=') {
					_make_token(TK_OP_ASSIGN_SUB);
					INCPOS(1);
				} else if (GETCHAR(1) == '>') {
					_make_token(TK_FORWARD_ARROW);
					INCPOS(1);
				} else {
					_make_token(TK_OP_SUB);
				}
			} break;
			case '%': {
				if (GETCHAR(1) == '=') {
					_make_token(TK_OP_ASSIGN_MOD);
					INCPOS(1);
				} else {
					_make_token(TK_OP_MOD);
				}
			} break;
			case '@':
				if (CharType(GETCHAR(1)) != '"' && CharType(GETCHAR(1)) != '\'') {
					_make_error("Unexpected '@'");
					return;
				}
				INCPOS(1);
				is_string_name = true;
				FALLTHROUGH;
			case '\'':
			case '"': {
				if (GETCHAR(0) == '\'') {
					string_mode = STRING_SINGLE_QUOTE;
				}

				int i = 1;
				if (string_mode == STRING_DOUBLE_QUOTE && GETCHAR(i) == '"' && GETCHAR(i + 1) == '"') {
					i += 2;
					string_mode = STRING_MULTILINE;
				}

				String str;
				while (true) {
					if (CharType(GETCHAR(i)) == 0) {
						_make_error("Unterminated String");
						return;
					} else if (string_mode == STRING_DOUBLE_QUOTE && CharType(GETCHAR(i)) == '"') {
						break;
					} else if (string_mode == STRING_SINGLE_QUOTE && CharType(GETCHAR(i)) == '\'') {
						break;
					} else if (string_mode == STRING_MULTILINE && CharType(GETCHAR(i)) == '\"' && CharType(GETCHAR(i + 1)) == '\"' && CharType(GETCHAR(i + 2)) == '\"') {
						i += 2;
						break;
					} else if (string_mode != STRING_MULTILINE && CharType(GETCHAR(i)) == '\n') {
						_make_error("Unexpected EOL at String.");
						return;
					} else if (CharType(GETCHAR(i)) == 0xFFFF) {
						//string ends here, next will be TK
						i--;
						break;
					} else if (CharType(GETCHAR(i)) == '\\') {
						//escaped characters...
						i++;
						CharType next = GETCHAR(i);
						if (next == 0) {
							_make_error("Unterminated String");
							return;
						}
						CharType res = 0;

						switch (next) {
							case 'a':
								res = 7;
								break;
							case 'b':
								res = 8;
								break;
							case 't':
								res = 9;
								break;
							case 'n':
								res = 10;
								break;
							case 'v':
								res = 11;
								break;
							case 'f':
								res = 12;
								break;
							case 'r':
								res = 13;
								break;
							case '\'':
								res = '\'';
								break;
							case '\"':
								res = '\"';
								break;
							case '\\':
								res = '\\';
								break;
							case '/':
								res = '/';
								break; //wtf

							case 'u': {
								//hexnumbarh - oct is deprecated
								i += 1;
								for (int j = 0; j < 4; j++) {
									CharType c = GETCHAR(i + j);
									if (c == 0) {
										_make_error("Unterminated String");
										return;
									}

									CharType v = 0;
									if (c >= '0' && c <= '9') {
										v = c - '0';
									} else if (c >= 'a' && c <= 'f') {
										v = c - 'a';
										v += 10;
									} else if (c >= 'A' && c <= 'F') {
										v = c - 'A';
										v += 10;
									} else {
										_make_error("Malformed hex constant in string");
										return;
									}

									res <<= 4;
									res |= v;
								}
								i += 3;

							} break;
							default: {
								_make_error("Invalid escape sequence");
								return;
							} break;
						}

						str += res;

					} else {
						if (CharType(GETCHAR(i)) == '\n') {
							line++;
							column = 1;
						}

						str += CharType(GETCHAR(i));
					}
					i++;
				}
				INCPOS(i);

				if (is_string_name) {
					_make_constant(StringName(str));
				} else {
					_make_constant(str);
				}

			} break;
			case 0xFFFF: {
				_make_token(TK_CURSOR);
			} break;
			default: {
				if (_is_number(GETCHAR(0)) || (GETCHAR(0) == '.' && _is_number(GETCHAR(1)))) {
					// parse number
					bool period_found = false;
					bool exponent_found = false;
					bool hexa_found = false;
					bool bin_found = false;
					bool sign_found = false;

					String str;
					int i = 0;

					while (true) {
						if (GETCHAR(i) == '.') {
							if (period_found || exponent_found) {
								_make_error("Invalid numeric constant at '.'");
								return;
							} else if (bin_found) {
								_make_error("Invalid binary constant at '.'");
								return;
							} else if (hexa_found) {
								_make_error("Invalid hexadecimal constant at '.'");
								return;
							}
							period_found = true;
						} else if (GETCHAR(i) == 'x') {
							if (hexa_found || bin_found || str.length() != 1 || !((i == 1 && str[0] == '0') || (i == 2 && str[1] == '0' && str[0] == '-'))) {
								_make_error("Invalid numeric constant at 'x'");
								return;
							}
							hexa_found = true;
						} else if (hexa_found && _is_hex(GETCHAR(i))) {
						} else if (!hexa_found && GETCHAR(i) == 'b') {
							if (bin_found || str.length() != 1 || !((i == 1 && str[0] == '0') || (i == 2 && str[1] == '0' && str[0] == '-'))) {
								_make_error("Invalid numeric constant at 'b'");
								return;
							}
							bin_found = true;
						} else if (!hexa_found && GETCHAR(i) == 'e') {
							if (exponent_found || bin_found) {
								_make_error("Invalid numeric constant at 'e'");
								return;
							}
							exponent_found = true;
						} else if (_is_number(GETCHAR(i))) {
							//all ok

						} else if (bin_found && _is_bin(GETCHAR(i))) {
						} else if ((GETCHAR(i) == '-' || GETCHAR(i) == '+') && exponent_found) {
							if (sign_found) {
								_make_error("Invalid numeric constant at '-'");
								return;
							}
							sign_found = true;
						} else if (GETCHAR(i) == '_') {
							i++;
							continue; // Included for readability, shouldn't be a part of the string
						} else {
							break;
						}

						str += CharType(GETCHAR(i));
						i++;
					}

					if (!(_is_number(str[str.length() - 1]) || (hexa_found && _is_hex(str[str.length() - 1])))) {
						_make_error("Invalid numeric constant: " + str);
						return;
					}

					INCPOS(i);
					if (hexa_found) {
						int64_t val = str.hex_to_int64();
						_make_constant(val);
					} else if (bin_found) {
						int64_t val = str.bin_to_int64();
						_make_constant(val);
					} else if (period_found || exponent_found) {
						double val = str.to_double();
						_make_constant(val);
					} else {
						int64_t val = str.to_int64();
						_make_constant(val);
					}

					return;
				}

				if (GETCHAR(0) == '.') {
					//parse period
					_make_token(TK_PERIOD);
					break;
				}

				if (_is_text_char(GETCHAR(0))) {
					// parse identifier
					String str;
					str += CharType(GETCHAR(0));

					int i = 1;
					while (_is_text_char(GETCHAR(i))) {
						str += CharType(GETCHAR(i));
						i++;
					}

					// Detect preset keywords / functions using hashtable.
					bool found = _parse_identifier(str);

					if (!found) {
						_make_identifier(str);
					}

					INCPOS(str.length());

					return;
				}

				_make_error("Unknown character");
				return;

			} break;
		}

		INCPOS(1);
		break;
	}
}

void CScriptTokenizerText::set_code(const String &p_code) {
	code = p_code;
	len = p_code.length();
	if (len) {
		_code = &code[0];
	} else {
		_code = nullptr;
	}
	code_pos = 0;
	line = 1; //it is stand-ar-ized that lines begin in 1 in code..
	column = 1; //the same holds for columns
	tk_rb_pos = 0;
	error_flag = false;
#ifdef DEBUG_ENABLED
	ignore_warnings = false;
#endif // DEBUG_ENABLED
	last_error = "";
	for (int i = 0; i < MAX_LOOKAHEAD + 1; i++) {
		_advance();
	}
}

CScriptTokenizerText::Token CScriptTokenizerText::get_token(int p_offset) const {
	ERR_FAIL_COND_V(p_offset <= -MAX_LOOKAHEAD, TK_ERROR);
	ERR_FAIL_COND_V(p_offset >= MAX_LOOKAHEAD, TK_ERROR);

	int ofs = (TK_RB_SIZE + tk_rb_pos + p_offset - MAX_LOOKAHEAD - 1) % TK_RB_SIZE;
	return tk_rb[ofs].type;
}

int CScriptTokenizerText::get_token_line(int p_offset) const {
	ERR_FAIL_COND_V(p_offset <= -MAX_LOOKAHEAD, -1);
	ERR_FAIL_COND_V(p_offset >= MAX_LOOKAHEAD, -1);

	int ofs = (TK_RB_SIZE + tk_rb_pos + p_offset - MAX_LOOKAHEAD - 1) % TK_RB_SIZE;
	return tk_rb[ofs].line;
}

int CScriptTokenizerText::get_token_column(int p_offset) const {
	ERR_FAIL_COND_V(p_offset <= -MAX_LOOKAHEAD, -1);
	ERR_FAIL_COND_V(p_offset >= MAX_LOOKAHEAD, -1);

	int ofs = (TK_RB_SIZE + tk_rb_pos + p_offset - MAX_LOOKAHEAD - 1) % TK_RB_SIZE;
	return tk_rb[ofs].col;
}

const Variant &CScriptTokenizerText::get_token_constant(int p_offset) const {
	ERR_FAIL_COND_V(p_offset <= -MAX_LOOKAHEAD, tk_rb[0].constant);
	ERR_FAIL_COND_V(p_offset >= MAX_LOOKAHEAD, tk_rb[0].constant);

	int ofs = (TK_RB_SIZE + tk_rb_pos + p_offset - MAX_LOOKAHEAD - 1) % TK_RB_SIZE;
	ERR_FAIL_COND_V(tk_rb[ofs].type != TK_CONSTANT, tk_rb[0].constant);
	return tk_rb[ofs].constant;
}

StringName CScriptTokenizerText::get_token_identifier(int p_offset) const {
	ERR_FAIL_COND_V(p_offset <= -MAX_LOOKAHEAD, StringName());
	ERR_FAIL_COND_V(p_offset >= MAX_LOOKAHEAD, StringName());

	int ofs = (TK_RB_SIZE + tk_rb_pos + p_offset - MAX_LOOKAHEAD - 1) % TK_RB_SIZE;
	ERR_FAIL_COND_V(tk_rb[ofs].type != TK_IDENTIFIER, StringName());
	return tk_rb[ofs].identifier;
}

CScriptFunctions::Function CScriptTokenizerText::get_token_built_in_func(int p_offset) const {
	ERR_FAIL_COND_V(p_offset <= -MAX_LOOKAHEAD, CScriptFunctions::FUNC_MAX);
	ERR_FAIL_COND_V(p_offset >= MAX_LOOKAHEAD, CScriptFunctions::FUNC_MAX);

	int ofs = (TK_RB_SIZE + tk_rb_pos + p_offset - MAX_LOOKAHEAD - 1) % TK_RB_SIZE;
	ERR_FAIL_COND_V(tk_rb[ofs].type != TK_BUILT_IN_FUNC, CScriptFunctions::FUNC_MAX);
	return tk_rb[ofs].func;
}

Variant::Type CScriptTokenizerText::get_token_type(int p_offset) const {
	ERR_FAIL_COND_V(p_offset <= -MAX_LOOKAHEAD, Variant::NIL);
	ERR_FAIL_COND_V(p_offset >= MAX_LOOKAHEAD, Variant::NIL);

	int ofs = (TK_RB_SIZE + tk_rb_pos + p_offset - MAX_LOOKAHEAD - 1) % TK_RB_SIZE;
	ERR_FAIL_COND_V(tk_rb[ofs].type != TK_BUILT_IN_TYPE, Variant::NIL);
	return tk_rb[ofs].vtype;
}

int CScriptTokenizerText::get_token_line_indent(int p_offset) const {
	ERR_FAIL_COND_V(p_offset <= -MAX_LOOKAHEAD, 0);
	ERR_FAIL_COND_V(p_offset >= MAX_LOOKAHEAD, 0);

	int ofs = (TK_RB_SIZE + tk_rb_pos + p_offset - MAX_LOOKAHEAD - 1) % TK_RB_SIZE;
	ERR_FAIL_COND_V(tk_rb[ofs].type != TK_NEWLINE, 0);
	return tk_rb[ofs].constant.operator Vector2().x;
}

int CScriptTokenizerText::get_token_line_tab_indent(int p_offset) const {
	ERR_FAIL_COND_V(p_offset <= -MAX_LOOKAHEAD, 0);
	ERR_FAIL_COND_V(p_offset >= MAX_LOOKAHEAD, 0);

	int ofs = (TK_RB_SIZE + tk_rb_pos + p_offset - MAX_LOOKAHEAD - 1) % TK_RB_SIZE;
	ERR_FAIL_COND_V(tk_rb[ofs].type != TK_NEWLINE, 0);
	return tk_rb[ofs].constant.operator Vector2().y;
}

String CScriptTokenizerText::get_token_error(int p_offset) const {
	ERR_FAIL_COND_V(p_offset <= -MAX_LOOKAHEAD, String());
	ERR_FAIL_COND_V(p_offset >= MAX_LOOKAHEAD, String());

	int ofs = (TK_RB_SIZE + tk_rb_pos + p_offset - MAX_LOOKAHEAD - 1) % TK_RB_SIZE;
	ERR_FAIL_COND_V(tk_rb[ofs].type != TK_ERROR, String());
	return tk_rb[ofs].constant;
}

void CScriptTokenizerText::advance(int p_amount) {
	ERR_FAIL_COND(p_amount <= 0);
	for (int i = 0; i < p_amount; i++) {
		_advance();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

#define BYTECODE_VERSION 13

Error CScriptTokenizerBuffer::set_code_buffer(const Vector<uint8_t> &p_buffer) {
	const uint8_t *buf = p_buffer.ptr();
	int total_len = p_buffer.size();
	ERR_FAIL_COND_V(p_buffer.size() < 24 || p_buffer[0] != 'G' || p_buffer[1] != 'D' || p_buffer[2] != 'S' || p_buffer[3] != 'C', ERR_INVALID_DATA);

	int version = decode_uint32(&buf[4]);
	ERR_FAIL_COND_V_MSG(version > BYTECODE_VERSION, ERR_INVALID_DATA, "Bytecode is too recent! Please use a newer engine version.");

	int identifier_count = decode_uint32(&buf[8]);
	int constant_count = decode_uint32(&buf[12]);
	int line_count = decode_uint32(&buf[16]);
	int token_count = decode_uint32(&buf[20]);

	const uint8_t *b = &buf[24];
	total_len -= 24;

	identifiers.resize(identifier_count);
	for (int i = 0; i < identifier_count; i++) {
		int len = decode_uint32(b);
		ERR_FAIL_COND_V(len > total_len, ERR_INVALID_DATA);
		b += 4;
		Vector<uint8_t> cs;
		cs.resize(len);
		for (int j = 0; j < len; j++) {
			cs.write[j] = b[j] ^ 0xb6;
		}

		cs.write[cs.size() - 1] = 0;
		String s;
		s.parse_utf8((const char *)cs.ptr());
		b += len;
		total_len -= len + 4;
		identifiers.write[i] = s;
	}

	constants.resize(constant_count);
	for (int i = 0; i < constant_count; i++) {
		Variant v;
		int len;
		// An object cannot be constant, never decode objects
		Error err = decode_variant(v, b, total_len, &len, false);
		if (err) {
			return err;
		}
		b += len;
		total_len -= len;
		constants.write[i] = v;
	}

	ERR_FAIL_COND_V(line_count * 8 > total_len, ERR_INVALID_DATA);

	for (int i = 0; i < line_count; i++) {
		uint32_t token = decode_uint32(b);
		b += 4;
		uint32_t linecol = decode_uint32(b);
		b += 4;

		lines.insert(token, linecol);
		total_len -= 8;
	}

	tokens.resize(token_count);

	for (int i = 0; i < token_count; i++) {
		ERR_FAIL_COND_V(total_len < 1, ERR_INVALID_DATA);

		if ((*b) & TOKEN_BYTE_MASK) { //little endian always
			ERR_FAIL_COND_V(total_len < 4, ERR_INVALID_DATA);

			tokens.write[i] = decode_uint32(b) & ~TOKEN_BYTE_MASK;
			b += 4;
		} else {
			tokens.write[i] = *b;
			b += 1;
			total_len--;
		}
	}

	token = 0;

	return OK;
}

Vector<uint8_t> CScriptTokenizerBuffer::parse_code_string(const String &p_code) {
	Vector<uint8_t> buf;

	RBMap<StringName, int> identifier_map;
	HashMap<Variant, int, VariantHasher, VariantComparator> constant_map;
	RBMap<uint32_t, int> line_map;
	Vector<uint32_t> token_array;

	CScriptTokenizerText tt;
	tt.set_code(p_code);
	int line = -1;

	while (true) {
		if (tt.get_token_line() != line) {
			line = tt.get_token_line();
			line_map[line] = token_array.size();
		}

		uint32_t token = tt.get_token();
		switch (tt.get_token()) {
			case TK_IDENTIFIER: {
				StringName id = tt.get_token_identifier();
				if (!identifier_map.has(id)) {
					int idx = identifier_map.size();
					identifier_map[id] = idx;
				}
				token |= identifier_map[id] << TOKEN_BITS;
			} break;
			case TK_CONSTANT: {
				const Variant &c = tt.get_token_constant();
				if (!constant_map.has(c)) {
					int idx = constant_map.size();
					constant_map[c] = idx;
				}
				token |= constant_map[c] << TOKEN_BITS;
			} break;
			case TK_BUILT_IN_TYPE: {
				token |= tt.get_token_type() << TOKEN_BITS;
			} break;
			case TK_BUILT_IN_FUNC: {
				token |= tt.get_token_built_in_func() << TOKEN_BITS;

			} break;
			case TK_NEWLINE: {
				token |= tt.get_token_line_indent() << TOKEN_BITS;
			} break;
			case TK_ERROR: {
				ERR_FAIL_V(Vector<uint8_t>());
			} break;
			default: {
			}
		};

		token_array.push_back(token);

		if (tt.get_token() == TK_EOF) {
			break;
		}
		tt.advance();
	}

	//reverse maps

	RBMap<int, StringName> rev_identifier_map;
	for (RBMap<StringName, int>::Element *E = identifier_map.front(); E; E = E->next()) {
		rev_identifier_map[E->get()] = E->key();
	}

	RBMap<int, Variant> rev_constant_map;
	const Variant *K = nullptr;
	while ((K = constant_map.next(K))) {
		rev_constant_map[constant_map[*K]] = *K;
	}

	RBMap<int, uint32_t> rev_line_map;
	for (RBMap<uint32_t, int>::Element *E = line_map.front(); E; E = E->next()) {
		rev_line_map[E->get()] = E->key();
	}

	//save header
	buf.resize(24);
	buf.write[0] = 'G';
	buf.write[1] = 'D';
	buf.write[2] = 'S';
	buf.write[3] = 'C';
	encode_uint32(BYTECODE_VERSION, &buf.write[4]);
	encode_uint32(identifier_map.size(), &buf.write[8]);
	encode_uint32(constant_map.size(), &buf.write[12]);
	encode_uint32(line_map.size(), &buf.write[16]);
	encode_uint32(token_array.size(), &buf.write[20]);

	//save identifiers

	for (RBMap<int, StringName>::Element *E = rev_identifier_map.front(); E; E = E->next()) {
		CharString cs = String(E->get()).utf8();
		int len = cs.length() + 1;
		int extra = 4 - (len % 4);
		if (extra == 4) {
			extra = 0;
		}

		uint8_t ibuf[4];
		encode_uint32(len + extra, ibuf);
		for (int i = 0; i < 4; i++) {
			buf.push_back(ibuf[i]);
		}
		for (int i = 0; i < len; i++) {
			buf.push_back(cs[i] ^ 0xb6);
		}
		for (int i = 0; i < extra; i++) {
			buf.push_back(0 ^ 0xb6);
		}
	}

	for (RBMap<int, Variant>::Element *E = rev_constant_map.front(); E; E = E->next()) {
		int len;
		// Objects cannot be constant, never encode objects
		Error err = encode_variant(E->get(), nullptr, len, false);
		ERR_FAIL_COND_V_MSG(err != OK, Vector<uint8_t>(), "Error when trying to encode Variant.");
		int pos = buf.size();
		buf.resize(pos + len);
		encode_variant(E->get(), &buf.write[pos], len, false);
	}

	for (RBMap<int, uint32_t>::Element *E = rev_line_map.front(); E; E = E->next()) {
		uint8_t ibuf[8];
		encode_uint32(E->key(), &ibuf[0]);
		encode_uint32(E->get(), &ibuf[4]);
		for (int i = 0; i < 8; i++) {
			buf.push_back(ibuf[i]);
		}
	}

	for (int i = 0; i < token_array.size(); i++) {
		uint32_t token = token_array[i];

		if (token & ~TOKEN_MASK) {
			uint8_t buf4[4];
			encode_uint32(token_array[i] | TOKEN_BYTE_MASK, &buf4[0]);
			for (int j = 0; j < 4; j++) {
				buf.push_back(buf4[j]);
			}
		} else {
			buf.push_back(token);
		}
	}

	return buf;
}

CScriptTokenizerBuffer::Token CScriptTokenizerBuffer::get_token(int p_offset) const {
	int offset = token + p_offset;

	if (offset < 0 || offset >= tokens.size()) {
		return TK_EOF;
	}

	return CScriptTokenizerBuffer::Token(tokens[offset] & TOKEN_MASK);
}

StringName CScriptTokenizerBuffer::get_token_identifier(int p_offset) const {
	int offset = token + p_offset;

	ERR_FAIL_INDEX_V(offset, tokens.size(), StringName());
	uint32_t identifier = tokens[offset] >> TOKEN_BITS;
	ERR_FAIL_UNSIGNED_INDEX_V(identifier, (uint32_t)identifiers.size(), StringName());

	return identifiers[identifier];
}

CScriptFunctions::Function CScriptTokenizerBuffer::get_token_built_in_func(int p_offset) const {
	int offset = token + p_offset;
	ERR_FAIL_INDEX_V(offset, tokens.size(), CScriptFunctions::FUNC_MAX);
	return CScriptFunctions::Function(tokens[offset] >> TOKEN_BITS);
}

Variant::Type CScriptTokenizerBuffer::get_token_type(int p_offset) const {
	int offset = token + p_offset;
	ERR_FAIL_INDEX_V(offset, tokens.size(), Variant::NIL);

	return Variant::Type(tokens[offset] >> TOKEN_BITS);
}

int CScriptTokenizerBuffer::get_token_line(int p_offset) const {
	int offset = token + p_offset;
	int pos = lines.find_nearest(offset);

	if (pos < 0) {
		return -1;
	}
	if (pos >= lines.size()) {
		pos = lines.size() - 1;
	}

	uint32_t l = lines.getv(pos);
	return l & TOKEN_LINE_MASK;
}
int CScriptTokenizerBuffer::get_token_column(int p_offset) const {
	int offset = token + p_offset;
	int pos = lines.find_nearest(offset);
	if (pos < 0) {
		return -1;
	}
	if (pos >= lines.size()) {
		pos = lines.size() - 1;
	}

	uint32_t l = lines.getv(pos);
	return l >> TOKEN_LINE_BITS;
}
int CScriptTokenizerBuffer::get_token_line_indent(int p_offset) const {
	int offset = token + p_offset;
	ERR_FAIL_INDEX_V(offset, tokens.size(), 0);
	return tokens[offset] >> TOKEN_BITS;
}
const Variant &CScriptTokenizerBuffer::get_token_constant(int p_offset) const {
	int offset = token + p_offset;
	ERR_FAIL_INDEX_V(offset, tokens.size(), nil);
	uint32_t constant = tokens[offset] >> TOKEN_BITS;
	ERR_FAIL_UNSIGNED_INDEX_V(constant, (uint32_t)constants.size(), nil);
	return constants[constant];
}
String CScriptTokenizerBuffer::get_token_error(int p_offset) const {
	ERR_FAIL_V(String());
}

void CScriptTokenizerBuffer::advance(int p_amount) {
	ERR_FAIL_INDEX(p_amount + token, tokens.size());
	token += p_amount;
}
CScriptTokenizerBuffer::CScriptTokenizerBuffer() {
	token = 0;
}
