#ifndef GDNATIVE_GDNATIVE_H
#define GDNATIVE_GDNATIVE_H

/*************************************************************************/
/*  gdnative.h                                                           */
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

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) || defined(__ANDROID__) || defined(GDNATIVE_LINUX_BSD_WEB)
#define GDCALLINGCONV
#elif defined(__APPLE__)
#include "TargetConditionals.h"
#if TARGET_OS_IPHONE
#define GDCALLINGCONV __attribute__((visibility("default")))
#elif TARGET_OS_MAC
#define GDCALLINGCONV __attribute__((sysv_abi))
#endif
#else // Linux/BSD/Web
#if defined(__aarch64__)
#define GDCALLINGCONV
#else
#define GDCALLINGCONV __attribute__((sysv_abi))
#endif
#endif

#define GDAPI GDCALLINGCONV

// This is for libraries *using* the header, NOT PANDEMONIUM EXPOSING STUFF!!
#if !defined(GDN_EXPORT)
#if defined(_WIN32)
#define GDN_EXPORT __declspec(dllexport)
#elif defined(__GNUC__)
#define GDN_EXPORT __attribute__((visibility("default")))
#else
#define GDN_EXPORT
#endif
#endif

#include <stdbool.h>
#include <stdint.h>

#define PANDEMONIUM_API_VERSION 1

////// Error

typedef enum {
	PANDEMONIUM_OK, // (0)
	PANDEMONIUM_FAILED, ///< Generic fail error
	PANDEMONIUM_ERR_UNAVAILABLE, ///< What is requested is unsupported/unavailable
	PANDEMONIUM_ERR_UNCONFIGURED, ///< The object being used hasn't been properly set up yet
	PANDEMONIUM_ERR_UNAUTHORIZED, ///< Missing credentials for requested resource
	PANDEMONIUM_ERR_PARAMETER_RANGE_ERROR, ///< Parameter given out of range (5)
	PANDEMONIUM_ERR_OUT_OF_MEMORY, ///< Out of memory
	PANDEMONIUM_ERR_FILE_NOT_FOUND,
	PANDEMONIUM_ERR_FILE_BAD_DRIVE,
	PANDEMONIUM_ERR_FILE_BAD_PATH,
	PANDEMONIUM_ERR_FILE_NO_PERMISSION, // (10)
	PANDEMONIUM_ERR_FILE_ALREADY_IN_USE,
	PANDEMONIUM_ERR_FILE_CANT_OPEN,
	PANDEMONIUM_ERR_FILE_CANT_WRITE,
	PANDEMONIUM_ERR_FILE_CANT_READ,
	PANDEMONIUM_ERR_FILE_UNRECOGNIZED, // (15)
	PANDEMONIUM_ERR_FILE_CORRUPT,
	PANDEMONIUM_ERR_FILE_MISSING_DEPENDENCIES,
	PANDEMONIUM_ERR_FILE_EOF,
	PANDEMONIUM_ERR_CANT_OPEN, ///< Can't open a resource/socket/file
	PANDEMONIUM_ERR_CANT_CREATE, // (20)
	PANDEMONIUM_ERR_QUERY_FAILED,
	PANDEMONIUM_ERR_ALREADY_IN_USE,
	PANDEMONIUM_ERR_LOCKED, ///< resource is locked
	PANDEMONIUM_ERR_TIMEOUT,
	PANDEMONIUM_ERR_CANT_CONNECT, // (25)
	PANDEMONIUM_ERR_CANT_RESOLVE,
	PANDEMONIUM_ERR_CONNECTION_ERROR,
	PANDEMONIUM_ERR_CANT_ACQUIRE_RESOURCE,
	PANDEMONIUM_ERR_CANT_FORK,
	PANDEMONIUM_ERR_INVALID_DATA, ///< Data passed is invalid (30)
	PANDEMONIUM_ERR_INVALID_PARAMETER, ///< Parameter passed is invalid
	PANDEMONIUM_ERR_ALREADY_EXISTS, ///< When adding, item already exists
	PANDEMONIUM_ERR_DOES_NOT_EXIST, ///< When retrieving/erasing, it item does not exist
	PANDEMONIUM_ERR_DATABASE_CANT_READ, ///< database is full
	PANDEMONIUM_ERR_DATABASE_CANT_WRITE, ///< database is full (35)
	PANDEMONIUM_ERR_COMPILATION_FAILED,
	PANDEMONIUM_ERR_METHOD_NOT_FOUND,
	PANDEMONIUM_ERR_LINK_FAILED,
	PANDEMONIUM_ERR_SCRIPT_FAILED,
	PANDEMONIUM_ERR_CYCLIC_LINK, // (40)
	PANDEMONIUM_ERR_INVALID_DECLARATION,
	PANDEMONIUM_ERR_DUPLICATE_SYMBOL,
	PANDEMONIUM_ERR_PARSE_ERROR,
	PANDEMONIUM_ERR_BUSY,
	PANDEMONIUM_ERR_SKIP, // (45)
	PANDEMONIUM_ERR_HELP, ///< user requested help!!
	PANDEMONIUM_ERR_BUG, ///< a bug in the software certainly happened, due to a double check failing or unexpected behavior.
	PANDEMONIUM_ERR_PRINTER_ON_FIRE, /// the parallel port printer is engulfed in flames
} pandemonium_error;

////// bool

typedef bool pandemonium_bool;

#define PANDEMONIUM_TRUE 1
#define PANDEMONIUM_FALSE 0

/////// int

typedef int pandemonium_int;

/////// real

// TODO add real_t -> float/double support!
// If the engine is built with doubles as real_t this won't work, as the structs won't line up!

typedef float pandemonium_real;

/////// Object (forward declared)
typedef void pandemonium_object;

/////// String

#include <gdn/string.h>

/////// String name

#include <gdn/string_name.h>

////// Vector2

#include <gdn/vector2.h>

////// Vector2i

#include <gdn/vector2i.h>

////// Rect2

#include <gdn/rect2.h>

////// Rect2i

#include <gdn/rect2i.h>

////// Vector3

#include <gdn/vector3.h>

////// Vector3i

#include <gdn/vector3i.h>

////// Vector4

#include <gdn/vector4.h>

////// Vector4i

#include <gdn/vector4i.h>

////// Transform2D

#include <gdn/transform2d.h>

/////// Plane

#include <gdn/plane.h>

/////// Quaternion

#include <gdn/quaternion.h>

/////// AABB

#include <gdn/aabb.h>

/////// Basis

#include <gdn/basis.h>

/////// Transform

#include <gdn/transform.h>

/////// Projection

#include <gdn/projection.h>

/////// Color

#include <gdn/color.h>

/////// NodePath

#include <gdn/node_path.h>

/////// RID

#include <gdn/rid.h>

/////// Dictionary

#include <gdn/dictionary.h>

/////// Array

#include <gdn/array.h>

// single API file for Pool*Array
#include <gdn/pool_arrays.h>

void GDAPI pandemonium_object_destroy(pandemonium_object *p_o);

////// Variant

#include <gdn/variant.h>

////// Singleton API

pandemonium_object GDAPI *pandemonium_global_get_singleton(char *p_name); // result shouldn't be freed

////// MethodBind API

typedef struct {
	uint8_t _dont_touch_that[1]; // TODO
} pandemonium_method_bind;

pandemonium_method_bind GDAPI *pandemonium_method_bind_get_method(const char *p_classname, const char *p_methodname);
void GDAPI pandemonium_method_bind_ptrcall(pandemonium_method_bind *p_method_bind, pandemonium_object *p_instance, const void **p_args, void *p_ret);
pandemonium_variant GDAPI pandemonium_method_bind_call(pandemonium_method_bind *p_method_bind, pandemonium_object *p_instance, const pandemonium_variant **p_args, const int p_arg_count, pandemonium_variant_call_error *p_call_error);
////// Script API

typedef struct pandemonium_gdnative_api_version {
	unsigned int major;
	unsigned int minor;
} pandemonium_gdnative_api_version;

typedef struct pandemonium_gdnative_api_struct pandemonium_gdnative_api_struct;

struct pandemonium_gdnative_api_struct {
	unsigned int type;
	pandemonium_gdnative_api_version version;
	const pandemonium_gdnative_api_struct *next;
};

#define GDNATIVE_VERSION_COMPATIBLE(want, have) (want.major == have.major && want.minor <= have.minor)

typedef struct {
	pandemonium_bool in_editor;
	uint64_t core_api_hash;
	uint64_t editor_api_hash;
	uint64_t no_api_hash;
	void (*report_version_mismatch)(const pandemonium_object *p_library, const char *p_what, pandemonium_gdnative_api_version p_want, pandemonium_gdnative_api_version p_have);
	void (*report_loading_error)(const pandemonium_object *p_library, const char *p_what);
	pandemonium_object *gd_native_library; // pointer to GDNativeLibrary that is being initialized
	const struct pandemonium_gdnative_core_api_struct *api_struct;
	const pandemonium_string *active_library_path;
} pandemonium_gdnative_init_options;

typedef struct {
	pandemonium_bool in_editor;
} pandemonium_gdnative_terminate_options;

// Calling convention?
typedef pandemonium_object *(*pandemonium_class_constructor)();

pandemonium_class_constructor GDAPI pandemonium_get_class_constructor(const char *p_classname);

pandemonium_dictionary GDAPI pandemonium_get_global_constants();

////// GDNative procedure types
typedef void (*pandemonium_gdnative_init_fn)(pandemonium_gdnative_init_options *);
typedef void (*pandemonium_gdnative_terminate_fn)(pandemonium_gdnative_terminate_options *);
typedef pandemonium_variant (*pandemonium_gdnative_procedure_fn)(pandemonium_array *);

////// System Functions

typedef pandemonium_variant (*native_call_cb)(void *, pandemonium_array *);
void GDAPI pandemonium_register_native_call_type(const char *p_call_type, native_call_cb p_callback);

//using these will help Pandemonium track how much memory is in use in debug mode
void GDAPI *pandemonium_alloc(int p_bytes);
void GDAPI *pandemonium_realloc(void *p_ptr, int p_bytes);
void GDAPI pandemonium_free(void *p_ptr);

//print using Pandemonium's error handler list
void GDAPI pandemonium_print_error(const char *p_description, const char *p_function, const char *p_file, int p_line);
void GDAPI pandemonium_print_warning(const char *p_description, const char *p_function, const char *p_file, int p_line);
void GDAPI pandemonium_print(const pandemonium_string *p_message);

bool GDAPI pandemonium_is_instance_valid(const pandemonium_object *p_object);

//tags used for safe dynamic casting
void GDAPI *pandemonium_get_class_tag(const pandemonium_string_name *p_class);
pandemonium_object GDAPI *pandemonium_object_cast_to(const pandemonium_object *p_object, void *p_class_tag);

// equivalent of GDScript's instance_from_id
pandemonium_object GDAPI *pandemonium_instance_from_id(pandemonium_int p_instance_id);

#ifdef __cplusplus
}
#endif

#endif // GDNATIVE_GDNATIVE_H
