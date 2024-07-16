#ifndef SHADER_CACHE_GLES3_H
#define SHADER_CACHE_GLES3_H

/*************************************************************************/
/*  shader_cache_gles3.h                                                 */
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

#include "core/containers/local_vector.h"
#include "core/object/reference.h"

class DirAccess;
class String;

class ShaderCacheGLES3 {
	DirAccess *storage_da;
	String storage_path;
	uint64_t storage_size = 0;

	void _purge_excess();

public:
	static String hash_program(const char *const *p_platform_strings, const LocalVector<const char *> &p_vertex_strings, const LocalVector<const char *> &p_fragment_strings);

	bool retrieve(const String &p_program_hash, uint32_t *r_format, PoolByteArray *r_data);
	void store(const String &p_program_hash, uint32_t p_program_format, const PoolByteArray &p_program_data);
	void remove(const String &p_program_hash);

	ShaderCacheGLES3();
	~ShaderCacheGLES3();
};

#endif // SHADER_CACHE_GLES3_H
