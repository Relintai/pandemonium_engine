#ifndef PANDEMONIUM_NATIVESCRIPT_H
#define PANDEMONIUM_NATIVESCRIPT_H

/*************************************************************************/
/*  pandemonium_nativescript.h                                           */
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

#include <gdn/gdnative.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	PANDEMONIUM_METHOD_RPC_MODE_DISABLED,
	PANDEMONIUM_METHOD_RPC_MODE_REMOTE,
	PANDEMONIUM_METHOD_RPC_MODE_MASTER,
	PANDEMONIUM_METHOD_RPC_MODE_PUPPET,
	PANDEMONIUM_METHOD_RPC_MODE_REMOTESYNC,
	PANDEMONIUM_METHOD_RPC_MODE_MASTERSYNC,
	PANDEMONIUM_METHOD_RPC_MODE_PUPPETSYNC,
} pandemonium_method_rpc_mode;

typedef enum {
	PANDEMONIUM_PROPERTY_HINT_NONE, ///< no hint provided.
	PANDEMONIUM_PROPERTY_HINT_RANGE, ///< hint_text = "min,max,step,slider; //slider is optional"
	PANDEMONIUM_PROPERTY_HINT_EXP_RANGE, ///< hint_text = "min,max,step", exponential edit
	PANDEMONIUM_PROPERTY_HINT_ENUM, ///< hint_text= "val1,val2,val3,etc"
	PANDEMONIUM_PROPERTY_HINT_EXP_EASING, /// exponential easing function (Math::ease) use "attenuation" hint string to revert (flip h), "full" to also include in/out. (ie: "attenuation,inout")
	PANDEMONIUM_PROPERTY_HINT_LENGTH, ///< hint_text= "length" (as integer)
	PANDEMONIUM_PROPERTY_HINT_BUTTON, // Use a button in the inspector for this property. The property's type has to be Variant::NIL. hint_text="call_func:name/theme_type" -> calls call_func(const StringName &p_property) on press, optional: ":name/theme_type" -> get_theme_icon("name", "theme_type")
	PANDEMONIUM_PROPERTY_HINT_KEY_ACCEL, ///< hint_text= "length" (as integer)
	PANDEMONIUM_PROPERTY_HINT_FLAGS, ///< hint_text= "flag1,flag2,etc" (as bit flags)
	PANDEMONIUM_PROPERTY_HINT_LAYERS_2D_RENDER,
	PANDEMONIUM_PROPERTY_HINT_LAYERS_2D_PHYSICS,
	PANDEMONIUM_PROPERTY_HINT_LAYERS_2D_NAVIGATION,
	PANDEMONIUM_PROPERTY_HINT_LAYERS_3D_RENDER,
	PANDEMONIUM_PROPERTY_HINT_LAYERS_3D_PHYSICS,
	PANDEMONIUM_PROPERTY_HINT_LAYERS_3D_NAVIGATION,
	PANDEMONIUM_PROPERTY_HINT_LAYERS_AVOIDANCE,
	PANDEMONIUM_PROPERTY_HINT_FILE, ///< a file path must be passed, hint_text (optionally) is a filter "*.png,*.wav,*.doc,"
	PANDEMONIUM_PROPERTY_HINT_DIR, ///< a directory path must be passed
	PANDEMONIUM_PROPERTY_HINT_GLOBAL_FILE, ///< a file path must be passed, hint_text (optionally) is a filter "*.png,*.wav,*.doc,"
	PANDEMONIUM_PROPERTY_HINT_GLOBAL_DIR, ///< a directory path must be passed
	PANDEMONIUM_PROPERTY_HINT_RESOURCE_TYPE, ///< a resource object type
	PANDEMONIUM_PROPERTY_HINT_MULTILINE_TEXT, ///< used for string properties that can contain multiple lines
	PANDEMONIUM_PROPERTY_HINT_PLACEHOLDER_TEXT, ///< used to set a placeholder text for string properties
	PANDEMONIUM_PROPERTY_HINT_COLOR_NO_ALPHA, ///< used for ignoring alpha component when editing a color
	PANDEMONIUM_PROPERTY_HINT_IMAGE_COMPRESS_LOSSY,
	PANDEMONIUM_PROPERTY_HINT_IMAGE_COMPRESS_LOSSLESS,
	PANDEMONIUM_PROPERTY_HINT_OBJECT_ID,
	PANDEMONIUM_PROPERTY_HINT_TYPE_STRING, ///< a type string, the hint is the base type to choose
	PANDEMONIUM_PROPERTY_HINT_NODE_PATH_TO_EDITED_NODE, ///< so something else can provide this (used in scripts)
	PANDEMONIUM_PROPERTY_HINT_METHOD_OF_VARIANT_TYPE, ///< a method of a type
	PANDEMONIUM_PROPERTY_HINT_METHOD_OF_BASE_TYPE, ///< a method of a base type
	PANDEMONIUM_PROPERTY_HINT_METHOD_OF_INSTANCE, ///< a method of an instance
	PANDEMONIUM_PROPERTY_HINT_METHOD_OF_SCRIPT, ///< a method of a script & base
	PANDEMONIUM_PROPERTY_HINT_PROPERTY_OF_VARIANT_TYPE, ///< a property of a type
	PANDEMONIUM_PROPERTY_HINT_PROPERTY_OF_BASE_TYPE, ///< a property of a base type
	PANDEMONIUM_PROPERTY_HINT_PROPERTY_OF_INSTANCE, ///< a property of an instance
	PANDEMONIUM_PROPERTY_HINT_PROPERTY_OF_SCRIPT, ///< a property of a script & base
	PANDEMONIUM_PROPERTY_HINT_OBJECT_TOO_BIG, ///< object is too big to send
	PANDEMONIUM_PROPERTY_HINT_NODE_PATH_VALID_TYPES,
	PANDEMONIUM_PROPERTY_HINT_SAVE_FILE, ///< a file path must be passed, hint_text (optionally) is a filter "*.png,*.wav,*.doc,". This opens a save dialog
	PANDEMONIUM_PROPERTY_HINT_ENUM_SUGGESTION, ///< hint_text= "val1,val2,val3,etc"
	PANDEMONIUM_PROPERTY_HINT_LINK,
	PANDEMONIUM_PROPERTY_HINT_MAX,
} pandemonium_property_hint;

typedef enum {

	PANDEMONIUM_PROPERTY_USAGE_STORAGE = 1,
	PANDEMONIUM_PROPERTY_USAGE_EDITOR = 2,
	PANDEMONIUM_PROPERTY_USAGE_NETWORK = 4,
	PANDEMONIUM_PROPERTY_USAGE_EDITOR_HELPER = 8,
	PANDEMONIUM_PROPERTY_USAGE_CHECKABLE = 16, //used for editing global variables
	PANDEMONIUM_PROPERTY_USAGE_CHECKED = 32, //used for editing global variables
	PANDEMONIUM_PROPERTY_USAGE_INTERNATIONALIZED = 64, //hint for internationalized strings
	PANDEMONIUM_PROPERTY_USAGE_GROUP = 128, //used for grouping props in the editor
	PANDEMONIUM_PROPERTY_USAGE_CATEGORY = 256,
	PANDEMONIUM_PROPERTY_USAGE_ARRAY = 512, // Used in the inspector to group properties as elements of an array.
	PANDEMONIUM_PROPERTY_USAGE_NO_INSTANCE_STATE = 2048,
	PANDEMONIUM_PROPERTY_USAGE_RESTART_IF_CHANGED = 4096,
	PANDEMONIUM_PROPERTY_USAGE_SCRIPT_VARIABLE = 8192,
	PANDEMONIUM_PROPERTY_USAGE_STORE_IF_NULL = 16384,
	PANDEMONIUM_PROPERTY_USAGE_ANIMATE_AS_TRIGGER = 32768,
	PANDEMONIUM_PROPERTY_USAGE_UPDATE_ALL_IF_MODIFIED = 65536,
	PANDEMONIUM_PROPERTY_USAGE_SCRIPT_DEFAULT_VALUE = 1 << 17,
	PANDEMONIUM_PROPERTY_USAGE_CLASS_IS_ENUM = 1 << 18,
	PANDEMONIUM_PROPERTY_USAGE_NIL_IS_VARIANT = 1 << 19,
	PANDEMONIUM_PROPERTY_USAGE_INTERNAL = 1 << 20,
	PANDEMONIUM_PROPERTY_USAGE_DO_NOT_SHARE_ON_DUPLICATE = 1 << 21, // If the object is duplicated also this property will be duplicated
	PANDEMONIUM_PROPERTY_USAGE_HIGH_END_GFX = 1 << 22,
	PANDEMONIUM_PROPERTY_USAGE_NODE_PATH_FROM_SCENE_ROOT = 1 << 23,
	PANDEMONIUM_PROPERTY_USAGE_RESOURCE_NOT_PERSISTENT = 1 << 24,
	PANDEMONIUM_PROPERTY_USAGE_KEYING_INCREMENTS = 1 << 25, // Used in inspector to increment property when keyed in animation player

	PANDEMONIUM_PROPERTY_USAGE_DEFAULT = PANDEMONIUM_PROPERTY_USAGE_STORAGE | PANDEMONIUM_PROPERTY_USAGE_EDITOR | PANDEMONIUM_PROPERTY_USAGE_NETWORK,
	PANDEMONIUM_PROPERTY_USAGE_DEFAULT_INTL = PANDEMONIUM_PROPERTY_USAGE_STORAGE | PANDEMONIUM_PROPERTY_USAGE_EDITOR | PANDEMONIUM_PROPERTY_USAGE_NETWORK | PANDEMONIUM_PROPERTY_USAGE_INTERNATIONALIZED,
	PANDEMONIUM_PROPERTY_USAGE_NOEDITOR = PANDEMONIUM_PROPERTY_USAGE_STORAGE | PANDEMONIUM_PROPERTY_USAGE_NETWORK,
} pandemonium_property_usage_flags;

typedef struct {
	pandemonium_method_rpc_mode rset_type;

	pandemonium_int type;
	pandemonium_property_hint hint;
	pandemonium_string hint_string;
	pandemonium_property_usage_flags usage;
	pandemonium_variant default_value;
} pandemonium_property_attributes;

typedef struct {
	// instance pointer, method_data - return user data
	GDCALLINGCONV void *(*create_func)(pandemonium_object *, void *);
	void *method_data;
	GDCALLINGCONV void (*free_func)(void *);
} pandemonium_instance_create_func;

typedef struct {
	// instance pointer, method data, user data
	GDCALLINGCONV void (*destroy_func)(pandemonium_object *, void *, void *);
	void *method_data;
	GDCALLINGCONV void (*free_func)(void *);
} pandemonium_instance_destroy_func;

void GDAPI pandemonium_nativescript_register_class(void *p_gdnative_handle, const char *p_name, const char *p_base, pandemonium_instance_create_func p_create_func, pandemonium_instance_destroy_func p_destroy_func);

void GDAPI pandemonium_nativescript_register_tool_class(void *p_gdnative_handle, const char *p_name, const char *p_base, pandemonium_instance_create_func p_create_func, pandemonium_instance_destroy_func p_destroy_func);

typedef struct {
	pandemonium_method_rpc_mode rpc_type;
} pandemonium_method_attributes;

typedef struct {
	// instance pointer, method data, user data, num args, args - return result as variant
	GDCALLINGCONV pandemonium_variant (*method)(pandemonium_object *, void *, void *, int, pandemonium_variant **);
	void *method_data;
	GDCALLINGCONV void (*free_func)(void *);
} pandemonium_instance_method;

void GDAPI pandemonium_nativescript_register_method(void *p_gdnative_handle, const char *p_name, const char *p_function_name, pandemonium_method_attributes p_attr, pandemonium_instance_method p_method);

typedef struct {
	// instance pointer, method data, user data, value
	GDCALLINGCONV void (*set_func)(pandemonium_object *, void *, void *, pandemonium_variant *);
	void *method_data;
	GDCALLINGCONV void (*free_func)(void *);
} pandemonium_property_set_func;

typedef struct {
	// instance pointer, method data, user data, value
	GDCALLINGCONV pandemonium_variant (*get_func)(pandemonium_object *, void *, void *);
	void *method_data;
	GDCALLINGCONV void (*free_func)(void *);
} pandemonium_property_get_func;

void GDAPI pandemonium_nativescript_register_property(void *p_gdnative_handle, const char *p_name, const char *p_path, pandemonium_property_attributes *p_attr, pandemonium_property_set_func p_set_func, pandemonium_property_get_func p_get_func);

typedef struct {
	pandemonium_string name;
	pandemonium_int type;
	pandemonium_property_hint hint;
	pandemonium_string hint_string;
	pandemonium_property_usage_flags usage;
	pandemonium_variant default_value;
} pandemonium_signal_argument;

typedef struct {
	pandemonium_string name;
	int num_args;
	pandemonium_signal_argument *args;
	int num_default_args;
	pandemonium_variant *default_args;
} pandemonium_signal;

void GDAPI pandemonium_nativescript_register_signal(void *p_gdnative_handle, const char *p_name, const pandemonium_signal *p_signal);

void GDAPI *pandemonium_nativescript_get_userdata(pandemonium_object *p_instance);

// method registering with argument names

typedef struct {
	pandemonium_string name;

	pandemonium_variant_type type;
	pandemonium_property_hint hint;
	pandemonium_string hint_string;
} pandemonium_method_arg;

void GDAPI pandemonium_nativescript_set_method_argument_information(void *p_gdnative_handle, const char *p_name, const char *p_function_name, int p_num_args, const pandemonium_method_arg *p_args);

// documentation

void GDAPI pandemonium_nativescript_set_class_documentation(void *p_gdnative_handle, const char *p_name, pandemonium_string p_documentation);
void GDAPI pandemonium_nativescript_set_method_documentation(void *p_gdnative_handle, const char *p_name, const char *p_function_name, pandemonium_string p_documentation);
void GDAPI pandemonium_nativescript_set_property_documentation(void *p_gdnative_handle, const char *p_name, const char *p_path, pandemonium_string p_documentation);
void GDAPI pandemonium_nativescript_set_signal_documentation(void *p_gdnative_handle, const char *p_name, const char *p_signal_name, pandemonium_string p_documentation);

// type tag API

void GDAPI pandemonium_nativescript_set_global_type_tag(int p_idx, const char *p_name, const void *p_type_tag);
const void GDAPI *pandemonium_nativescript_get_global_type_tag(int p_idx, const char *p_name);

void GDAPI pandemonium_nativescript_set_type_tag(void *p_gdnative_handle, const char *p_name, const void *p_type_tag);
const void GDAPI *pandemonium_nativescript_get_type_tag(const pandemonium_object *p_object);

// instance binding API

typedef struct {
	GDCALLINGCONV void *(*alloc_instance_binding_data)(void *, const void *, pandemonium_object *);
	GDCALLINGCONV void (*free_instance_binding_data)(void *, void *);
	GDCALLINGCONV void (*refcount_incremented_instance_binding)(void *, pandemonium_object *);
	GDCALLINGCONV bool (*refcount_decremented_instance_binding)(void *, pandemonium_object *);
	void *data;
	GDCALLINGCONV void (*free_func)(void *);
} pandemonium_instance_binding_functions;

int GDAPI pandemonium_nativescript_register_instance_binding_data_functions(pandemonium_instance_binding_functions p_binding_functions);
void GDAPI pandemonium_nativescript_unregister_instance_binding_data_functions(int p_idx);

void GDAPI *pandemonium_nativescript_get_instance_binding_data(int p_idx, pandemonium_object *p_object);

void GDAPI pandemonium_nativescript_profiling_add_data(const char *p_signature, uint64_t p_time);

#ifdef __cplusplus
}
#endif

#endif // PANDEMONIUM_NATIVESCRIPT_H
