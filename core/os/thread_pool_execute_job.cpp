/*************************************************************************/
/*  thread_pool_execute_job.cpp                                          */
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



#include "thread_pool_execute_job.h"

#include "core/variant/variant.h"

#include "core/os/os.h"

Variant ThreadPoolExecuteJob::get_object() const {
	return _object;
}
void ThreadPoolExecuteJob::set_object(const Variant &value) {
	_object = value;
}

StringName ThreadPoolExecuteJob::get_method() const {
	return _method;
}
void ThreadPoolExecuteJob::set_method(const StringName &value) {
	_method = value;
}

void ThreadPoolExecuteJob::_execute() {
	ERR_FAIL_COND(!_object);
	ERR_FAIL_COND(!_object->has_method(_method));

	Variant::CallError error;

	_object->call(_method, const_cast<const Variant **>(&_argptr), _argcount, error);
}

void ThreadPoolExecuteJob::setup(const Variant &obj, const StringName &p_method, VARIANT_ARG_DECLARE) {
	set_complete(false);
	set_cancelled(false);
	_object = obj;
	_method = p_method;

	_argptr[0] = p_arg1;
	_argptr[1] = p_arg2;
	_argptr[2] = p_arg3;
	_argptr[3] = p_arg4;
	_argptr[4] = p_arg5;

	for (int i = 4; i >= 0; --i) {
		if (_argptr[i].get_type() != Variant::NIL) {
			_argcount = i + 1;
			break;
		}
	}

	if (!_object || !_object->has_method(p_method)) {
		set_complete(true);

		ERR_FAIL_COND(!_object);
		ERR_FAIL_COND(!_object->has_method(p_method));
	}
}

Variant ThreadPoolExecuteJob::_setup_bind(const Variant **p_args, int p_argcount, Variant::CallError &r_error) {
	if (p_argcount < 2) {
		r_error.error = Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;

		r_error.argument = 1;
		return Variant();
	}

	if (p_args[0]->get_type() != Variant::OBJECT) {
		r_error.error = Variant::CallError::CALL_ERROR_INVALID_ARGUMENT;

		r_error.argument = 0;
		r_error.expected = Variant::OBJECT;
		return Variant();
	}

	if (p_args[1]->get_type() != Variant::STRING) {
		r_error.error = Variant::CallError::CALL_ERROR_INVALID_ARGUMENT;

		r_error.argument = 1;
		r_error.expected = Variant::STRING;
		return Variant();
	}

	set_complete(false);
	_object = *p_args[0];

	StringName sn = *p_args[1];
	_method = sn;

	if (p_argcount > 2) {
		_argcount = 1;
		_argptr[0] = p_args[2];
	}

	if (p_argcount > 3) {
		_argcount = 2;
		_argptr[1] = p_args[3];
	}

	if (p_argcount > 4) {
		_argcount = 3;
		_argptr[2] = p_args[4];
	}

	if (p_argcount > 5) {
		_argcount = 4;
		_argptr[3] = p_args[5];
	}

	if (p_argcount > 6) {
		_argcount = 5;
		_argptr[4] = p_args[6];
	}

	if (!_object || !_object->has_method(_method)) {
		set_complete(true);
	}

	r_error.error = Variant::CallError::CALL_OK;

	return Variant();
}

ThreadPoolExecuteJob::ThreadPoolExecuteJob() {
	_object = NULL;

	_argcount = 0;

	_argptr = memnew_arr(Variant, 5);
}
ThreadPoolExecuteJob::~ThreadPoolExecuteJob() {
	memdelete_arr(_argptr);
}

void ThreadPoolExecuteJob::_bind_methods() {
	MethodInfo mi;
	mi.arguments.push_back(PropertyInfo(Variant::OBJECT, "obj"));
	mi.arguments.push_back(PropertyInfo(Variant::STRING, "method"));

	mi.name = "setup";
	ClassDB::bind_vararg_method(METHOD_FLAGS_DEFAULT, "setup", &ThreadPoolExecuteJob::_setup_bind, mi);
}
