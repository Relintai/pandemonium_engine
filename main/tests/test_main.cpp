/*************************************************************************/
/*  test_main.cpp                                                        */
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

#include "test_main.h"

#include "core/containers/list.h"

#ifdef DEBUG_ENABLED

#include "test_astar.h"
#include "test_basis.h"
#include "test_crypto.h"
#include "test_expression.h"
#include "test_gdscript.h"
#include "test_gui.h"
#include "test_math.h"
#include "test_oa_hash_map.h"
#include "test_ordered_hash_map.h"
#include "test_physics.h"
#include "test_physics_2d.h"
#include "test_render.h"
#include "test_shader_lang.h"
#include "test_string.h"
#include "test_theme.h"
#include "test_transform.h"
#include "test_xml_parser.h"

const char **tests_get_names() {
	static const char *test_names[] = {
		//"string",
		"math",
		"basis",
		"transform",
		"physics",
		"physics_2d",
		"render",
		"oa_hash_map",
		"gui",
		"shaderlang",
		"gd_tokenizer",
		"gd_parser",
		"gd_compiler",
		"gd_bytecode",
		"ordered_hash_map",
		"astar",
		"xml_parser",
		"theme",
		"expression",
		nullptr
	};

	return test_names;
}

MainLoop *test_main(String p_test, const List<String> &p_args) {
	//if (p_test == "string") {
	//	return TestString::test();
	//}

	if (p_test == "math") {
		return TestMath::test();
	}

	if (p_test == "basis") {
		return TestBasis::test();
	}

	if (p_test == "transform") {
		return TestTransform::test();
	}

	if (p_test == "physics") {
		return TestPhysics::test();
	}

	if (p_test == "physics_2d") {
		return TestPhysics2D::test();
	}

	if (p_test == "render") {
		return TestRender::test();
	}

	if (p_test == "oa_hash_map") {
		return TestOAHashMap::test();
	}

#ifndef _3D_DISABLED
	if (p_test == "gui") {
		return TestGUI::test();
	}
#endif

	if (p_test == "shaderlang") {
		return TestShaderLang::test();
	}

	if (p_test == "crypto") {
		return TestCrypto::test();
	}

	if (p_test == "gd_tokenizer") {
		return TestGDScript::test(TestGDScript::TEST_TOKENIZER);
	}

	if (p_test == "gd_parser") {
		return TestGDScript::test(TestGDScript::TEST_PARSER);
	}

	if (p_test == "gd_compiler") {
		return TestGDScript::test(TestGDScript::TEST_COMPILER);
	}

	if (p_test == "gd_bytecode") {
		return TestGDScript::test(TestGDScript::TEST_BYTECODE);
	}

	if (p_test == "ordered_hash_map") {
		return TestOrderedHashMap::test();
	}

	if (p_test == "astar") {
		return TestAStar::test();
	}

	if (p_test == "xml_parser") {
		return TestXMLParser::test();
	}

	if (p_test == "theme") {
		return TestTheme::test();
	}

	if (p_test == "expression") {
		return TestExpression::test();
	}

	print_line("Unknown test: " + p_test);
	return nullptr;
}

#else

const char **tests_get_names() {
	static const char *test_names[] = {
		NULL
	};

	return test_names;
}

MainLoop *test_main(String p_test, const List<String> &p_args) {
	return NULL;
}

#endif
