/*************************************************************************/
/*  gdnative.cpp                                                         */
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

#include "gdn/gdnative.h"

#include "core/config/engine.h"
#include "core/error/error_macros.h"
#include "core/global_constants.h"
#include "core/object/class_db.h"
#include "core/os/os.h"
#include "core/variant/variant.h"

#include "../gdnative.h"

#ifdef __cplusplus
extern "C" {
#endif

void GDAPI pandemonium_object_destroy(pandemonium_object *p_o) {
	memdelete((Object *)p_o);
}

// Singleton API

pandemonium_object GDAPI *pandemonium_global_get_singleton(char *p_name) {
	return (pandemonium_object *)Engine::get_singleton()->get_singleton_object(String(p_name));
} // result shouldn't be freed

// MethodBind API

pandemonium_method_bind GDAPI *pandemonium_method_bind_get_method(const char *p_classname, const char *p_methodname) {
	MethodBind *mb = ClassDB::get_method(StringName(p_classname), StringName(p_methodname));
	// MethodBind *mb = ClassDB::get_method("Node", "get_name");
	return (pandemonium_method_bind *)mb;
}

void GDAPI pandemonium_method_bind_ptrcall(pandemonium_method_bind *p_method_bind, pandemonium_object *p_instance, const void **p_args, void *p_ret) {
	MethodBind *mb = (MethodBind *)p_method_bind;
	Object *o = (Object *)p_instance;
	mb->ptrcall(o, p_args, p_ret);
}

pandemonium_variant GDAPI pandemonium_method_bind_call(pandemonium_method_bind *p_method_bind, pandemonium_object *p_instance, const pandemonium_variant **p_args, const int p_arg_count, pandemonium_variant_call_error *p_call_error) {
	MethodBind *mb = (MethodBind *)p_method_bind;
	Object *o = (Object *)p_instance;
	const Variant **args = (const Variant **)p_args;

	pandemonium_variant ret;
	pandemonium_variant_new_nil(&ret);

	Variant *ret_val = (Variant *)&ret;

	Variant::CallError r_error;
	*ret_val = mb->call(o, args, p_arg_count, r_error);

	if (p_call_error) {
		p_call_error->error = (pandemonium_variant_call_error_error)r_error.error;
		p_call_error->argument = r_error.argument;
		p_call_error->expected = (pandemonium_variant_type)r_error.expected;
	}

	return ret;
}

pandemonium_class_constructor GDAPI pandemonium_get_class_constructor(const char *p_classname) {
	ClassDB::ClassInfo *class_info = ClassDB::classes.getptr(StringName(p_classname));
	if (class_info) {
		return (pandemonium_class_constructor)class_info->creation_func;
	}
	return nullptr;
}

pandemonium_dictionary GDAPI pandemonium_get_global_constants() {
	pandemonium_dictionary constants;
	pandemonium_dictionary_new(&constants);
	Dictionary *p_constants = (Dictionary *)&constants;
	const int constants_count = GlobalConstants::get_global_constant_count();
	for (int i = 0; i < constants_count; ++i) {
		const char *name = GlobalConstants::get_global_constant_name(i);
		int value = GlobalConstants::get_global_constant_value(i);
		(*p_constants)[name] = value;
	}
	return constants;
}

// System functions
void GDAPI pandemonium_register_native_call_type(const char *p_call_type, native_call_cb p_callback) {
	GDNativeCallRegistry::get_singleton()->register_native_call_type(StringName(p_call_type), p_callback);
}

void GDAPI *pandemonium_alloc(int p_bytes) {
	return memalloc(p_bytes);
}

void GDAPI *pandemonium_realloc(void *p_ptr, int p_bytes) {
	return memrealloc(p_ptr, p_bytes);
}

void GDAPI pandemonium_free(void *p_ptr) {
	memfree(p_ptr);
}

void GDAPI pandemonium_print_error(const char *p_description, const char *p_function, const char *p_file, int p_line) {
	_err_print_error(p_function, p_file, p_line, p_description, ERR_HANDLER_ERROR);
}

void GDAPI pandemonium_print_warning(const char *p_description, const char *p_function, const char *p_file, int p_line) {
	_err_print_error(p_function, p_file, p_line, p_description, ERR_HANDLER_WARNING);
}

void GDAPI pandemonium_print(const pandemonium_string *p_message) {
	print_line(*(String *)p_message);
}

void _gdnative_report_version_mismatch(const pandemonium_object *p_library, const char *p_ext, pandemonium_gdnative_api_version p_want, pandemonium_gdnative_api_version p_have) {
	String message = "Error loading GDNative file ";
	GDNativeLibrary *library = (GDNativeLibrary *)p_library;

	message += library->get_current_library_path() + ": Extension \"" + p_ext + "\" can't be loaded.\n";

	Dictionary versions;
	versions["have_major"] = p_have.major;
	versions["have_minor"] = p_have.minor;
	versions["want_major"] = p_want.major;
	versions["want_minor"] = p_want.minor;

	message += String("Got version {have_major}.{have_minor} but needs {want_major}.{want_minor}!").format(versions);

	_err_print_error("gdnative_init", library->get_current_library_path().utf8().ptr(), 0, message.utf8().ptr());
}

void _gdnative_report_loading_error(const pandemonium_object *p_library, const char *p_what) {
	String message = "Error loading GDNative file ";
	GDNativeLibrary *library = (GDNativeLibrary *)p_library;

	message += library->get_current_library_path() + ": " + p_what;

	_err_print_error("gdnative_init", library->get_current_library_path().utf8().ptr(), 0, message.utf8().ptr());
}

bool GDAPI pandemonium_is_instance_valid(const pandemonium_object *p_object) {
	return ObjectDB::instance_validate((Object *)p_object);
}

pandemonium_object GDAPI *pandemonium_instance_from_id(pandemonium_int p_instance_id) {
	return (pandemonium_object *)ObjectDB::get_instance((ObjectID)p_instance_id);
}

void *pandemonium_get_class_tag(const pandemonium_string_name *p_class) {
	StringName class_name = *(StringName *)p_class;
	ClassDB::ClassInfo *class_info = ClassDB::classes.getptr(class_name);
	return class_info ? class_info->class_ptr : nullptr;
}

pandemonium_object *pandemonium_object_cast_to(const pandemonium_object *p_object, void *p_class_tag) {
	if (!p_object) {
		return nullptr;
	}
	Object *o = (Object *)p_object;

	return o->is_class_ptr(p_class_tag) ? (pandemonium_object *)o : nullptr;
}

#ifdef __cplusplus
}
#endif
