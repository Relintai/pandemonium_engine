#ifndef FILE_CACHE_H
#define FILE_CACHE_H

/*************************************************************************/
/*  file_cache.h                                                         */
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

#include "core/containers/hash_map.h"
#include "core/containers/rb_map.h"
#include "core/containers/vector.h"
#include "core/os/os.h"
#include "core/os/rw_lock.h"
#include "core/string/ustring.h"

#include "core/object/reference.h"

class FileCache : public Reference {
	GDCLASS(FileCache, Reference);

public:
	enum PathCacheMode {
		PATH_CACHE_MODE_OFF,
		PATH_CACHE_MODE_STATIC,
		//TIMED?
	};

	PathCacheMode get_path_cache_mode() const;
	void set_path_cache_mode(const PathCacheMode p_mode);

	String get_wwwroot();
	void set_wwwroot(const String &val);

	String get_wwwroot_abs();

	int get_cache_invalidation_time();
	void set_cache_invalidation_time(const int &val);

	//Note: file path should be the url you want to access the file with, including lead slash
	//e.g. http://127.0.0.1/a/b/d.jpg -> /a/b/d.jpg
	String wwwroot_get_file_abspath(const String &file_path);

	void wwwroot_register_file(const String &file_path);
	void wwwroot_deregister_file(const String &file_path);
	bool wwwroot_has_file(const String &file_path);
	//return -1 if does not exists
	int wwwroot_get_file_index(const String &file_path);
	String wwwroot_get_file_orig_path(const int index);
	String wwwroot_get_file_orig_path_abs(const int index);

	void wwwroot_refresh_cache();
	void wwwroot_evaluate_dir(const String &path, const bool should_exist = true);

	bool get_cached_body(const String &path, String *body);
	bool has_cached_body(const String &path);
	String get_cached_body_bind(const String &path);
	void set_cached_body(const String &path, const String &body);

	void clear();

	FileCache();
	~FileCache();

	uint64_t cache_invalidation_time;

protected:
	static void _bind_methods();

	struct CacheEntry {
		uint64_t timestamp;
		String body;

		CacheEntry() {
			timestamp = 0;
		}
	};

	RWLock _body_lock;
	RBMap<String, CacheEntry *> cache_map;

	RWLock _cache_lock;
	PathCacheMode _path_cache_mode;
	String _wwwroot_orig;
	String _wwwroot;
	String _wwwroot_abs;

	struct RegisteredFileEntry {
		String orig_path;
		String lowercase_path;
	};

	Vector<RegisteredFileEntry> _registered_files;
};

VARIANT_ENUM_CAST(FileCache::PathCacheMode);

#endif
