#ifndef CSCRIPT_FUNCTION_H
#define CSCRIPT_FUNCTION_H

/*************************************************************************/
/*  cscript_function.h                                                   */
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

#include "core/containers/pair.h"
#include "core/containers/self_list.h"
#include "core/object/reference.h"
#include "core/object/script_language.h"
#include "core/os/thread.h"
#include "core/string/string_name.h"
#include "core/variant/variant.h"

class CScriptInstance;
class CScript;

struct CScriptDataType {
	bool has_type;
	enum {
		UNINITIALIZED,
		BUILTIN,
		NATIVE,
		SCRIPT,
		CSCRIPT,
	} kind;
	Variant::Type builtin_type;
	StringName native_type;
	Script *script_type;
	Ref<Script> script_type_ref;

	bool is_type(const Variant &p_variant, bool p_allow_implicit_conversion = false) const {
		if (!has_type) {
			return true; // Can't type check
		}

		switch (kind) {
			case UNINITIALIZED:
				break;
			case BUILTIN: {
				Variant::Type var_type = p_variant.get_type();
				bool valid = builtin_type == var_type;
				if (!valid && p_allow_implicit_conversion) {
					valid = Variant::can_convert_strict(var_type, builtin_type);
				}
				return valid;
			} break;
			case NATIVE: {
				if (p_variant.get_type() == Variant::NIL) {
					return true;
				}
				if (p_variant.get_type() != Variant::OBJECT) {
					return false;
				}

				Object *obj = p_variant.operator Object *();
				if (!obj) {
					return false;
				}

				if (!ClassDB::is_parent_class(obj->get_class_name(), native_type)) {
					// Try with underscore prefix
					StringName underscore_native_type = "_" + native_type;
					if (!ClassDB::is_parent_class(obj->get_class_name(), underscore_native_type)) {
						return false;
					}
				}
				return true;
			} break;
			case SCRIPT:
			case CSCRIPT: {
				if (p_variant.get_type() == Variant::NIL) {
					return true;
				}
				if (p_variant.get_type() != Variant::OBJECT) {
					return false;
				}

				Object *obj = p_variant.operator Object *();
				if (!obj) {
					return false;
				}

				Ref<Script> base = obj && obj->get_script_instance() ? obj->get_script_instance()->get_script() : nullptr;
				bool valid = false;
				while (base.is_valid()) {
					if (base == script_type) {
						valid = true;
						break;
					}
					base = base->get_base_script();
				}
				return valid;
			} break;
		}
		return false;
	}

	operator PropertyInfo() const {
		PropertyInfo info;
		if (has_type) {
			switch (kind) {
				case UNINITIALIZED:
					break;
				case BUILTIN: {
					info.type = builtin_type;
				} break;
				case NATIVE: {
					info.type = Variant::OBJECT;
					info.class_name = native_type;
				} break;
				case SCRIPT:
				case CSCRIPT: {
					info.type = Variant::OBJECT;
					info.class_name = script_type->get_instance_base_type();
				} break;
			}
		} else {
			info.type = Variant::NIL;
			info.usage |= PROPERTY_USAGE_NIL_IS_VARIANT;
		}
		return info;
	}

	CScriptDataType() :
			has_type(false),
			kind(UNINITIALIZED),
			builtin_type(Variant::NIL),
			script_type(nullptr) {}
};

class CScriptFunction {
public:
	enum Opcode {
		OPCODE_OPERATOR,
		OPCODE_EXTENDS_TEST,
		OPCODE_IS_BUILTIN,
		OPCODE_SET,
		OPCODE_GET,
		OPCODE_SET_NAMED,
		OPCODE_GET_NAMED,
		OPCODE_SET_MEMBER,
		OPCODE_GET_MEMBER,
		OPCODE_ASSIGN,
		OPCODE_ASSIGN_TRUE,
		OPCODE_ASSIGN_FALSE,
		OPCODE_ASSIGN_TYPED_BUILTIN,
		OPCODE_ASSIGN_TYPED_NATIVE,
		OPCODE_ASSIGN_TYPED_SCRIPT,
		OPCODE_CAST_TO_BUILTIN,
		OPCODE_CAST_TO_NATIVE,
		OPCODE_CAST_TO_SCRIPT,
		OPCODE_CONSTRUCT, //only for basic types!!
		OPCODE_CONSTRUCT_ARRAY,
		OPCODE_CONSTRUCT_DICTIONARY,
		OPCODE_CALL,
		OPCODE_CALL_RETURN,
		OPCODE_CALL_BUILT_IN,
		OPCODE_CALL_SELF,
		OPCODE_CALL_SELF_BASE,
		OPCODE_JUMP,
		OPCODE_JUMP_IF,
		OPCODE_JUMP_IF_NOT,
		OPCODE_JUMP_TO_DEF_ARGUMENT,
		OPCODE_RETURN,
		OPCODE_ITERATE_BEGIN,
		OPCODE_ITERATE,
		OPCODE_ASSERT,
		OPCODE_BREAKPOINT,
		OPCODE_LINE,
		OPCODE_END
	};

	enum Address {
		ADDR_BITS = 24,
		ADDR_MASK = ((1 << ADDR_BITS) - 1),
		ADDR_TYPE_MASK = ~ADDR_MASK,
		ADDR_TYPE_SELF = 0,
		ADDR_TYPE_CLASS = 1,
		ADDR_TYPE_MEMBER = 2,
		ADDR_TYPE_CLASS_CONSTANT = 3,
		ADDR_TYPE_LOCAL_CONSTANT = 4,
		ADDR_TYPE_STACK = 5,
		ADDR_TYPE_STACK_VARIABLE = 6,
		ADDR_TYPE_GLOBAL = 7,
		ADDR_TYPE_NAMED_GLOBAL = 8,
		ADDR_TYPE_NIL = 9
	};

	struct StackDebug {
		int line;
		int pos;
		bool added;
		StringName identifier;
	};

private:
	friend class CScriptCompiler;

	StringName source;

	mutable Variant nil;
	mutable Variant *_constants_ptr;
	int _constant_count;
	const StringName *_global_names_ptr;
	int _global_names_count;
#ifdef TOOLS_ENABLED
	const StringName *_named_globals_ptr;
	int _named_globals_count;
#endif
	const int *_default_arg_ptr;
	int _default_arg_count;
	const int *_code_ptr;
	int _code_size;
	int _argument_count;
	int _stack_size;
	int _call_size;
	int _initial_line;
	bool _static;

	CScript *_script;

	StringName name;
	Vector<Variant> constants;
	Vector<StringName> global_names;
#ifdef TOOLS_ENABLED
	Vector<StringName> named_globals;
#endif
	Vector<int> default_arguments;
	Vector<int> code;
	Vector<CScriptDataType> argument_types;
	CScriptDataType return_type;

#ifdef TOOLS_ENABLED
	Vector<StringName> arg_names;
#endif

	List<StackDebug> stack_debug;

	_FORCE_INLINE_ Variant *_get_variant(int p_address, CScriptInstance *p_instance, CScript *p_script, Variant &self, Variant &static_ref, Variant *p_stack, String &r_error) const;
	_FORCE_INLINE_ String _get_call_error(const Variant::CallError &p_err, const String &p_where, const Variant **argptrs) const;

	friend class CScriptLanguage;

	SelfList<CScriptFunction> function_list;
#ifdef DEBUG_ENABLED
	CharString func_cname;
	const char *_func_cname;

	struct Profile {
		StringName signature;
		uint64_t call_count;
		uint64_t self_time;
		uint64_t total_time;
		uint64_t frame_call_count;
		uint64_t frame_self_time;
		uint64_t frame_total_time;
		uint64_t last_frame_call_count;
		uint64_t last_frame_self_time;
		uint64_t last_frame_total_time;
	} profile;

#endif

public:
	struct CallState {
		CScript *script;
		CScriptInstance *instance;
#ifdef DEBUG_ENABLED
		StringName function_name;
		String script_path;
#endif
		Vector<uint8_t> stack;
		int stack_size;
		Variant self;
		uint32_t alloca_size;
		int ip;
		int line;
		int defarg;
		Variant result;
	};

	_FORCE_INLINE_ bool is_static() const {
		return _static;
	}

	const int *get_code() const; //used for debug
	int get_code_size() const;
	Variant get_constant(int p_idx) const;
	StringName get_global_name(int p_idx) const;
	StringName get_name() const;
	int get_max_stack_size() const;
	int get_default_argument_count() const;
	int get_default_argument_addr(int p_idx) const;
	CScriptDataType get_return_type() const;
	CScriptDataType get_argument_type(int p_idx) const;
	CScript *get_script() const {
		return _script;
	}
	StringName get_source() const {
		return source;
	}

	void debug_get_stack_member_state(int p_line, List<Pair<StringName, int>> *r_stackvars) const;

	_FORCE_INLINE_ bool is_empty() const {
		return _code_size == 0;
	}

	int get_argument_count() const {
		return _argument_count;
	}
	StringName get_argument_name(int p_idx) const {
#ifdef TOOLS_ENABLED
		ERR_FAIL_INDEX_V(p_idx, arg_names.size(), StringName());
		return arg_names[p_idx];
#else
		return StringName();
#endif
	}
	Variant get_default_argument(int p_idx) const {
		ERR_FAIL_INDEX_V(p_idx, default_arguments.size(), Variant());
		return default_arguments[p_idx];
	}

	Variant call(CScriptInstance *p_instance, const Variant **p_args, int p_argcount, Variant::CallError &r_err);

	CScriptFunction();
	~CScriptFunction();
};

#endif // CSCRIPT_FUNCTION_H
