#ifndef HTML_TEMPLATE_RENDERER_H
#define HTML_TEMPLATE_RENDERER_H

/*************************************************************************/
/*  html_template_renderer.h                                             */
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

#include "core/string/string_builder.h"

#include "core/object/resource.h"

class HTMLTemplateRenderResult : public Reference {
	GDCLASS(HTMLTemplateRenderResult, Reference);

public:
	String get_html() const;
	void set_html(const String &p_value);

	bool get_had_error() const;
	void set_had_error(const bool p_value);

	String get_error_text() const;
	void set_error_text(const String &p_value);

	HTMLTemplateRenderResult();
	~HTMLTemplateRenderResult();

protected:
	static void _bind_methods();

	String _html;
	bool _had_error;
	String _error_text;
};

class HTMLTemplateRenderer : public Reference {
	GDCLASS(HTMLTemplateRenderer, Reference);

public:
	String render(const Dictionary &p_data, bool &r_execution_error, String &r_error_txt, const bool p_show_error = false);
	Ref<HTMLTemplateRenderResult> render_result(const Dictionary &p_data, const bool p_show_error = false);

	bool compile(const String &p_text, const int p_start_line = 1);

	String get_compile_error_str();

	HTMLTemplateRenderer();
	~HTMLTemplateRenderer();

protected:
	static void _bind_methods();

protected:
	enum BuiltinFunc {
		FUNC_PRINT,
		FUNC_PRINT_RAW,
		FUNC_PRINT_BR,
		FUNC_PRINT_RAW_BR,
		FUNC_VFORMAT,
		FUNC_QPRINT,
		FUNC_QPRINT_RAW,
		FUNC_QPRINT_BR,
		FUNC_QPRINT_RAW_BR,
		FUNC_QVFORMAT,
		FUNC_EXISTS,
		FUNC_APPROX_EQUALS,
		FUNC_SETVAR,
		FUNC_SETVAR_IF,
		FUNC_GETVAR,
		FUNC_RANGE,
		FUNC_KEYS,
		FUNC_VALUES,
		FUNC_TR,
		FUNC_TRT,
		FUNC_SELECT,
		FUNC_STR,
		FUNC_MAX
	};

	static const char *func_name[FUNC_MAX];

	bool validate_func_argument_count(BuiltinFunc p_func, const int p_arg_count, bool p_set_error = true);
	static String get_func_name(BuiltinFunc p_func);
	static void exec_func(BuiltinFunc p_func, const Variant **p_inputs, const int p_input_count, Dictionary &p_data, Variant *r_return, Variant::CallError &r_error, String &r_error_str);
	static BuiltinFunc find_function(const String &p_string);

	enum TokenType {
		TK_HTML_DATA,
		TK_CURLY_BRACKET_OPEN,
		TK_CURLY_BRACKET_CLOSE,
		TK_DOUBLE_CURLY_BRACKET_OPEN,
		TK_DOUBLE_CURLY_BRACKET_CLOSE,
		TK_BRACKET_OPEN,
		TK_BRACKET_CLOSE,
		TK_PARENTHESIS_OPEN,
		TK_PARENTHESIS_CLOSE,
		TK_SEMI_COLON,
		TK_IDENTIFIER,
		TK_BUILTIN_FUNC,
		TK_CONSTANT,
		TK_BASIC_TYPE,
		TK_COLON,
		TK_COMMA,
		TK_PERIOD,
		TK_FOR,
		TK_ENDFOR,
		TK_IF,
		TK_ELIF,
		TK_ELSE,
		TK_ENDIF,
		TK_OP_IN,
		TK_OP_EQUAL,
		TK_OP_NOT_EQUAL,
		TK_OP_LESS,
		TK_OP_LESS_EQUAL,
		TK_OP_GREATER,
		TK_OP_GREATER_EQUAL,
		TK_OP_AND,
		TK_OP_OR,
		TK_OP_NOT,
		TK_OP_ADD,
		TK_OP_SUB,
		TK_OP_MUL,
		TK_OP_DIV,
		TK_OP_MOD,
		TK_OP_SHIFT_LEFT,
		TK_OP_SHIFT_RIGHT,
		TK_OP_BIT_AND,
		TK_OP_BIT_OR,
		TK_OP_BIT_XOR,
		TK_OP_BIT_INVERT,
		TK_EOF,
		TK_ERROR,
		TK_MAX
	};

	static const char *token_name[TK_MAX];

	struct Token {
		TokenType type;
		Variant value;
	};

	static String stringify_token(const Token &tk);

	String _compile_error_str;
	bool _compile_error_set;

	void _compile_set_error(const String &p_err) {
		if (_compile_error_set) {
			return;
		}
		_compile_error_str = p_err + vformat(" near line %d.", _current_line);
		_compile_error_set = true;
	}

	bool _tokenizer_in_text_mode;
	int _str_ofs;
	int _current_line;

	Error _get_token(Token &r_token);

	// Expression Nodes

	struct ENode {
		enum Type {
			TYPE_INPUT,
			TYPE_CONSTANT,
			TYPE_INDEX,
			TYPE_NAMED_INDEX,
			TYPE_BUILTIN_FUNC,
			TYPE_CALL,
			TYPE_OPERATOR,
			TYPE_ARRAY,
			TYPE_DICTIONARY,
			TYPE_CONSTRUCTOR,
			TYPE_BLOCK,
			TYPE_CONTROL_FLOW,
			TYPE_HTML_DATA,
			TYPE_PRINT,
			TYPE_IF,
			TYPE_FOREACH,
		};

		ENode *next;

		Type type;

		ENode() {
			type = Type::TYPE_CONSTANT;
			next = nullptr;
		}
		virtual ~ENode() {
			if (next) {
				memdelete(next);
			}
		}
	};

	struct ExpressionNode {
		bool is_op;
		union {
			Variant::Operator op;
			ENode *node;
		};
	};

	struct InputNode : public ENode {
		StringName name;

		InputNode() {
			type = TYPE_INPUT;
		}
	};

	struct ConstantNode : public ENode {
		Variant value;

		ConstantNode() {
			type = TYPE_CONSTANT;
		}
	};

	struct IndexNode : public ENode {
		ENode *base;
		ENode *index;

		IndexNode() {
			type = TYPE_INDEX;
			base = nullptr;
			index = nullptr;
		}
	};

	struct NamedIndexNode : public ENode {
		ENode *base;
		StringName name;

		NamedIndexNode() {
			type = TYPE_NAMED_INDEX;
			base = nullptr;
		}
	};

	struct BuiltinFuncNode : public ENode {
		BuiltinFunc func;
		Vector<ENode *> arguments;
		BuiltinFuncNode() {
			type = TYPE_BUILTIN_FUNC;
			func = BuiltinFunc::FUNC_PRINT;
		}
	};

	struct OperatorNode : public ENode {
		Variant::Operator op;

		ENode *nodes[2];

		OperatorNode() {
			type = TYPE_OPERATOR;
			nodes[0] = nullptr;
			nodes[1] = nullptr;
		}
	};

	struct CallNode : public ENode {
		ENode *base;
		StringName method;
		Vector<ENode *> arguments;

		CallNode() {
			type = TYPE_CALL;
			base = nullptr;
		}
	};

	struct ArrayNode : public ENode {
		Vector<ENode *> array;

		ArrayNode() {
			type = TYPE_ARRAY;
		}
	};

	struct DictionaryNode : public ENode {
		Vector<ENode *> dict;

		DictionaryNode() {
			type = TYPE_DICTIONARY;
		}
	};

	struct ConstructorNode : public ENode {
		Variant::Type data_type;
		Vector<ENode *> arguments;

		ConstructorNode() {
			type = TYPE_CONSTRUCTOR;
		}
	};

	// Base ControlFlowNode for logical separation
	// Only here for easier understanding of logic
	struct ControlFlowNode : public ENode {
		ControlFlowNode() {
			type = Type::TYPE_CONTROL_FLOW;
		}
	};

	// Contains a list of ControlFlowNodes.
	// Normal expressions are always in Control Nodes (like in if conditions, etc)
	struct BlockNode : public ENode {
		ENode *parent_node;
		Vector<ControlFlowNode *> block;

		BlockNode() {
			type = Type::TYPE_BLOCK;
			parent_node = NULL;
		}
	};

	// ControlFlowNodes

	// Raw html outside of {{, etc
	struct HTMLDataNode : public ControlFlowNode {
		String value;

		HTMLDataNode() {
			type = TYPE_HTML_DATA;
		}
	};

	// So {{ <expr> }} : whatever is returned gets printed as-is, maybe except for nulls, escaped (except it the outer ENode is pr, pbr, etc)
	// So {{; <expr> }} Run expression, no output
	// So {{% <expr> }} Explicit raw
	struct PrintNode : public ControlFlowNode {
		ENode *expr;

		bool raw;
		bool supressed;

		PrintNode() {
			type = TYPE_PRINT;
			raw = false;
			supressed = false;
		}
	};

	// {{if <bool expr> }}
	// {{elif <bool expr> }}
	// {{else}}
	// {{endif}}
	struct IfNode : public ControlFlowNode {
		ENode *condition;
		BlockNode *body;
		IfNode *next_if;
		bool else_branch;

		IfNode() {
			type = TYPE_IF;
			condition = nullptr;
			body = nullptr;
			next_if = nullptr;
			else_branch = false;
		}
	};

	// {{for <variable declaration> in <collection> }}
	// {{endfor}}
	struct ForeachNode : public ControlFlowNode {
		StringName iter_variable;
		ENode *iter_init_expr;
		BlockNode *body;

		ForeachNode() {
			type = TYPE_FOREACH;
			iter_init_expr = nullptr;
			body = nullptr;
		}
	};

	template <class T>
	T *alloc_node() {
		T *node = memnew(T);
		node->next = _nodes;
		_nodes = node;
		return node;
	}

	ENode *_parse_expression(Token &tk, bool p_skip_next_token_get = false);
	void _parse_control_flow(BlockNode *p_parent_block, Token &tk, bool p_skip_next_token_get = false);

	BlockNode *_root;
	ENode *_nodes;

	String _template_text;

	bool _execute(Dictionary &p_data, StringBuilder &p_html, ENode *p_node, Variant &r_ret, String &r_error_str);
};

#endif
