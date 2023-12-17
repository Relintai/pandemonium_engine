#ifndef UNIT_TEST_H
#define UNIT_TEST_H

/*************************************************************************/
/*  unit_test.h                                                          */
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

#include "core/string/ustring.h"

#include "core/object/reference.h"

class QueryResult;

class UnitTest : public Reference {
	GDCLASS(UnitTest, Reference);

public:
	// ThreadPoolJob like api
	// asserts, prints, file check helpers, etc etc
	// bool process() -> return true when finished
	// bool _process()

	//sub_section_start(name)
	//sub_section_end()

	// test(name);
	// testing(name); //maybe?

	// assert(a == "b", "Testing whenther a == b")
	// assert_equals(a, "b")    ?  -> could generate text automatically

	//---
	/*

	bool _process() {
		testing("String int conversion.");
		assert(String::num(1) == "1");


		//or
		testing("String int conversion.");
		//cpp macro
		UTASSERT(String::num(1) == "1"); -> assert(String::num(1) == "1", __LINE__);

		sub_section_start(name)

	}

	*/
	///---

	// process type: process, physics process

	//api to get results

	//ajutomatically count tests, and some data (process time, physics and normal iteration nums)

	//get_runner() -> returns runner

	UnitTest();
	virtual ~UnitTest();

protected:
	static void _bind_methods();
};

#endif
