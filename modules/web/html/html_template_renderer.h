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

class HTMLTemplaterenderer : public Reference {
	GDCLASS(HTMLTemplaterenderer, Reference);

public:
	String render(const Dictionary &p_data, const bool p_show_error = true);
	bool compile(const String &p_text);

	String get_error_str();

	HTMLTemplaterenderer();
	~HTMLTemplaterenderer();

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
		FUNC_EQUALS,
		FUNC_APPROX_EQUALS,
		FUNC_MAX
	};

	static const char *func_name[FUNC_MAX];

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
		TK_IDENTIFIER,
		TK_BUILTIN_FUNC,
		TK_CONSTANT,
		TK_BASIC_TYPE,
		TK_COLON,
		TK_COMMA,
		TK_PERIOD,
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

	String _error_str;
	bool _error_set;

	void _set_error(const String &p_err) {
		if (_error_set) {
			return;
		}
		_error_str = p_err;
		_error_set = true;
	}

	bool _tokenizer_in_text_mode;
	int str_ofs;

	Error _get_token(Token &r_token);

	// Expression Nodes

	struct ENode {
		enum Type {
			TYPE_CONSTANT,
			TYPE_INDEX,
			TYPE_NAMED_INDEX,
			TYPE_BUILTIN_FUNC,
			TYPE_OPERATOR,
			TYPE_BLOCK,
			TYPE_CONTROL_FLOW,
			TYPE_HTML_DATA,
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

	struct ControlFlowNode : public ENode {
		ControlFlowNode() {
			type = Type::TYPE_CONTROL_FLOW;
		}
	};

	struct BlockNode : public ENode {
		Vector<ControlFlowNode *> block;

		BlockNode() {
			type = Type::TYPE_BLOCK;
		}
	};

	// ControlFlowNodes

	struct HTMLDataNode : public ControlFlowNode {
		String value;

		HTMLDataNode() {
			type = TYPE_HTML_DATA;
		}
	};

	struct IfNode : public ControlFlowNode {
		ENode *condition;
		BlockNode *body;
		BlockNode *body_else;

		IfNode() {
			type = TYPE_IF;
			condition = nullptr;
			body = nullptr;
			body_else = nullptr;
		}
	};

	struct ForeachNode : public ControlFlowNode {
		ENode *condition;
		BlockNode *body;

		ForeachNode() {
			type = TYPE_FOREACH;
			condition = nullptr;
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

	BlockNode *_root;
	ENode *_nodes;

	String _template_text;

	bool _execution_error;
	bool _dirty;

	bool _execute(Dictionary &p_data, StringBuilder &p_html, ENode *p_node, Variant &r_ret, String &r_error_str);
};

#endif
