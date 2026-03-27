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
// ternary expression:
// {{? <bool expr>; <true expr>; <false expr> }}
// Could also just parse: Equivalent to qr, qp etc (so if false expr is null, just emit "", same for true expr)
// {{? <bool expr>; <true expr> }}
// Could also just parse: Equivalent to qr, qp etc (so if false expr is null, just emit "", same for true expr)
// {{? <bool expr>;; <false expr> }}
// assignment? Just assign to the input dictionary

// if? If there are bool expressions and ternary, an if should probably also be here
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

	for (int i = 0; i < _block.size(); ++i) {
		bool err = _execute(data, html, _block.write[i], output, error_txt);

		if (err) {
			_execution_error = true;
			_error_str = error_txt;
			ERR_FAIL_COND_V_MSG(p_show_error, html.as_string(), _error_str);
		}
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

#if 0
static bool _is_number(CharType c) {
	return (c >= '0' && c <= '9');
}

static bool _is_hex_digit(char32_t c) {
	return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

static bool _is_binary_digit(char32_t c) {
	return (c == '0' || c == '1');
}
#endif

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

const char *HTMLTemplaterenderer::token_name[TK_MAX] = {
	"EOF",
	"ERROR",
};

bool HTMLTemplaterenderer::_execute(Dictionary &p_data, StringBuilder &p_html, ExpressionNode *p_node, Variant &r_ret, String &r_error_str) {
	return false;
}
