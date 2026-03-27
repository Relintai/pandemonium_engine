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

	enum TokenType {
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

	struct TNode {
		enum Type {
			TYPE_HTML_DATA,
			TYPE_CONSTANT,
			TYPE_INDEX,
			TYPE_NAMED_INDEX,
			TYPE_BUILTIN_FUNC,
			TYPE_OPERATOR,
			TYPE_IF,
			TYPE_FOREACH,
		};

		TNode *next;

		Type type;

		TNode() {
			type = Type::TYPE_CONSTANT;
			next = nullptr;
		}
		virtual ~TNode() {
			if (next) {
				memdelete(next);
			}
		}
	};

	struct HTMLDataNode : public TNode {
		String value;
		HTMLDataNode() {
			type = TYPE_HTML_DATA;
		}
	};

	struct ConstantNode : public TNode {
		Variant value;
		ConstantNode() {
			type = TYPE_CONSTANT;
		}
	};

	struct IndexNode : public TNode {
		TNode *base;
		TNode *index;

		IndexNode() {
			type = TYPE_INDEX;
			base = nullptr;
			index = nullptr;
		}
	};

	struct NamedIndexNode : public TNode {
		TNode *base;
		StringName name;

		NamedIndexNode() {
			type = TYPE_NAMED_INDEX;
			base = nullptr;
		}
	};

	struct BuiltinFuncNode : public TNode {
		BuiltinFunc func;
		Vector<TNode *> arguments;
		BuiltinFuncNode() {
			type = TYPE_BUILTIN_FUNC;
			func = BuiltinFunc::FUNC_PRINT;
		}
	};

	struct OperatorNode : public TNode {
		Variant::Operator op;

		TNode *nodes[2];

		OperatorNode() {
			type = TYPE_OPERATOR;
			nodes[0] = nullptr;
			nodes[1] = nullptr;
		}
	};

	struct IfNode : public TNode {
		TNode *condition;
		TNode *body;
		TNode *body_else;

		IfNode() {
			type = TYPE_IF;
			condition = nullptr;
			body = nullptr;
			body_else = nullptr;
		}
	};

	struct ForeachNode : public TNode {
		TNode *condition;
		TNode *body;

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

	TNode *_root;
	TNode *_nodes;

	String _text;

	bool _execution_error;

	bool _execute(Dictionary &p_data, StringBuilder &p_html, TNode *p_node, Variant &r_ret, String &r_error_str);
};

#endif
