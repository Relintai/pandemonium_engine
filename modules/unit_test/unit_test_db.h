#ifndef UNIT_TEST_MANAGER_H
#define UNIT_TEST_MANAGER_H

/*************************************************************************/
/*  unit_test_db.h                                                       */
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

#include "core/object/object.h"

class UnitTestDB : public Object {
	GDCLASS(UnitTestDB, Object);

public:
	//UnitTests api

	//testing hint -> auto set (probably from main) if --test arg (should automatically set the scene root ast TestRunner eventually)
	//main -> --test_engine, or --load_engine_tests or --register_engine_tests ? -> register module tests
	//should probably have folders settings (also in project settings) -> only load them if necessary

	//api for registering unit tests manually -> keep them separate

	//api for getting all unit tests -> load everything from folders, append stuff manually registered -> return it

	static UnitTestDB *get_singleton();

	UnitTestDB();
	~UnitTestDB();

protected:
	static void _bind_methods();

private:
	static UnitTestDB *_instance;
};

#endif
