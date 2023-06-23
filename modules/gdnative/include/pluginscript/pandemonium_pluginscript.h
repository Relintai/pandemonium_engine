/**************************************************************************/
/*  pandemonium_pluginscript.h                                                  */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             PANDEMONIUM ENGINE                               */
/*                        https://pandemoniumengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#ifndef PANDEMONIUM_PLUGINSCRIPT_H
#define PANDEMONIUM_PLUGINSCRIPT_H

#include <gdn/gdnative.h>
#include <nativescript/pandemonium_nativescript.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void pandemonium_pluginscript_instance_data;
typedef void pandemonium_pluginscript_script_data;
typedef void pandemonium_pluginscript_language_data;

// --- Instance ---

// TODO: use pandemonium_string_name for faster lookup ?
typedef struct {
	pandemonium_pluginscript_instance_data *(*init)(pandemonium_pluginscript_script_data *p_data, pandemonium_object *p_owner);
	void (*finish)(pandemonium_pluginscript_instance_data *p_data);

	pandemonium_bool (*set_prop)(pandemonium_pluginscript_instance_data *p_data, const pandemonium_string *p_name, const pandemonium_variant *p_value);
	pandemonium_bool (*get_prop)(pandemonium_pluginscript_instance_data *p_data, const pandemonium_string *p_name, pandemonium_variant *r_ret);

	pandemonium_variant (*call_method)(pandemonium_pluginscript_instance_data *p_data,
			const pandemonium_string_name *p_method, const pandemonium_variant **p_args,
			int p_argcount, pandemonium_variant_call_error *r_error);

	void (*notification)(pandemonium_pluginscript_instance_data *p_data, int p_notification);

	//this is used by script languages that keep a reference counter of their own
	//you can make make Ref<> not die when it reaches zero, so deleting the reference
	//depends entirely from the script.
	// Note: You can set those function pointer to NULL if not needed.
	void (*refcount_incremented)(pandemonium_pluginscript_instance_data *p_data);
	bool (*refcount_decremented)(pandemonium_pluginscript_instance_data *p_data); // return true if it can die
} pandemonium_pluginscript_instance_desc;

// --- Script ---

typedef struct {
	pandemonium_pluginscript_script_data *data;
	pandemonium_string_name name;
	pandemonium_bool is_tool;
	pandemonium_string_name base;

	// Member lines format: {<string>: <int>}
	pandemonium_dictionary member_lines;
	// Method info dictionary format
	// {
	//  name: <string>
	//  args: [<dict:property>]
	//  default_args: [<variant>]
	//  return: <dict:property>
	//  flags: <int>
	//  rpc_mode: <int:pandemonium_method_rpc_mode>
	// }
	pandemonium_array methods;
	// Same format than for methods
	pandemonium_array signals;
	// Property info dictionary format
	// {
	//  name: <string>
	//  type: <int:pandemonium_variant_type>
	//  hint: <int:pandemonium_property_hint>
	//  hint_string: <string>
	//  usage: <int:pandemonium_property_usage_flags>
	//  default_value: <variant>
	//  rset_mode: <int:pandemonium_method_rpc_mode>
	// }
	pandemonium_array properties;
} pandemonium_pluginscript_script_manifest;

typedef struct {
	pandemonium_pluginscript_script_manifest (*init)(pandemonium_pluginscript_language_data *p_data, const pandemonium_string *p_path, const pandemonium_string *p_source, pandemonium_error *r_error);
	void (*finish)(pandemonium_pluginscript_script_data *p_data);
	pandemonium_pluginscript_instance_desc instance_desc;
} pandemonium_pluginscript_script_desc;

// --- Language ---

typedef struct {
	pandemonium_string_name signature;
	pandemonium_int call_count;
	pandemonium_int total_time; // In microseconds
	pandemonium_int self_time; // In microseconds
} pandemonium_pluginscript_profiling_data;

typedef struct {
	const char *name;
	const char *type;
	const char *extension;
	const char **recognized_extensions; // NULL terminated array
	pandemonium_pluginscript_language_data *(*init)();
	void (*finish)(pandemonium_pluginscript_language_data *p_data);
	const char **reserved_words; // NULL terminated array
	const char **comment_delimiters; // NULL terminated array
	const char **string_delimiters; // NULL terminated array
	pandemonium_bool has_named_classes;
	pandemonium_bool supports_builtin_mode;

	pandemonium_string (*get_template_source_code)(pandemonium_pluginscript_language_data *p_data, const pandemonium_string *p_class_name, const pandemonium_string *p_base_class_name);
	pandemonium_bool (*validate)(pandemonium_pluginscript_language_data *p_data, const pandemonium_string *p_script, int *r_line_error, int *r_col_error, pandemonium_string *r_test_error, const pandemonium_string *p_path, pandemonium_pool_string_array *r_functions);
	int (*find_function)(pandemonium_pluginscript_language_data *p_data, const pandemonium_string *p_function, const pandemonium_string *p_code); // Can be NULL
	pandemonium_string (*make_function)(pandemonium_pluginscript_language_data *p_data, const pandemonium_string *p_class, const pandemonium_string *p_name, const pandemonium_pool_string_array *p_args);
	pandemonium_error (*complete_code)(pandemonium_pluginscript_language_data *p_data, const pandemonium_string *p_code, const pandemonium_string *p_path, pandemonium_object *p_owner, pandemonium_array *r_options, pandemonium_bool *r_force, pandemonium_string *r_call_hint);
	void (*auto_indent_code)(pandemonium_pluginscript_language_data *p_data, pandemonium_string *p_code, int p_from_line, int p_to_line);

	void (*add_global_constant)(pandemonium_pluginscript_language_data *p_data, const pandemonium_string *p_variable, const pandemonium_variant *p_value);
	pandemonium_string (*debug_get_error)(pandemonium_pluginscript_language_data *p_data);
	int (*debug_get_stack_level_count)(pandemonium_pluginscript_language_data *p_data);
	int (*debug_get_stack_level_line)(pandemonium_pluginscript_language_data *p_data, int p_level);
	pandemonium_string (*debug_get_stack_level_function)(pandemonium_pluginscript_language_data *p_data, int p_level);
	pandemonium_string (*debug_get_stack_level_source)(pandemonium_pluginscript_language_data *p_data, int p_level);
	void (*debug_get_stack_level_locals)(pandemonium_pluginscript_language_data *p_data, int p_level, pandemonium_pool_string_array *p_locals, pandemonium_array *p_values, int p_max_subitems, int p_max_depth);
	void (*debug_get_stack_level_members)(pandemonium_pluginscript_language_data *p_data, int p_level, pandemonium_pool_string_array *p_members, pandemonium_array *p_values, int p_max_subitems, int p_max_depth);
	void (*debug_get_globals)(pandemonium_pluginscript_language_data *p_data, pandemonium_pool_string_array *p_locals, pandemonium_array *p_values, int p_max_subitems, int p_max_depth);
	pandemonium_string (*debug_parse_stack_level_expression)(pandemonium_pluginscript_language_data *p_data, int p_level, const pandemonium_string *p_expression, int p_max_subitems, int p_max_depth);

	// TODO: could this stuff be moved to the pandemonium_pluginscript_language_desc ?
	void (*get_public_functions)(pandemonium_pluginscript_language_data *p_data, pandemonium_array *r_functions);
	void (*get_public_constants)(pandemonium_pluginscript_language_data *p_data, pandemonium_dictionary *r_constants);

	void (*profiling_start)(pandemonium_pluginscript_language_data *p_data);
	void (*profiling_stop)(pandemonium_pluginscript_language_data *p_data);
	int (*profiling_get_accumulated_data)(pandemonium_pluginscript_language_data *p_data, pandemonium_pluginscript_profiling_data *r_info, int p_info_max);
	int (*profiling_get_frame_data)(pandemonium_pluginscript_language_data *p_data, pandemonium_pluginscript_profiling_data *r_info, int p_info_max);
	void (*profiling_frame)(pandemonium_pluginscript_language_data *p_data);

	pandemonium_pluginscript_script_desc script_desc;
} pandemonium_pluginscript_language_desc;

void GDAPI pandemonium_pluginscript_register_language(const pandemonium_pluginscript_language_desc *language_desc);

#ifdef __cplusplus
}
#endif

#endif // PANDEMONIUM_PLUGINSCRIPT_H
