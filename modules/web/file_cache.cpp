/*************************************************************************/
/*  file_cache.cpp                                                       */
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

#include "file_cache.h"

#include "core/os/dir_access.h"
#include "core/os/file_access.h"
#include "core/os/os.h"

String FileCache::get_wwwroot() {
	return _wwwroot_orig;
}
void FileCache::set_wwwroot(const String &val) {
	_wwwroot_orig = val;
	_wwwroot = _wwwroot_orig.path_clean_end_slash();

	if (!_wwwroot.empty()) {
		_wwwroot_abs = DirAccess::get_filesystem_abspath_for(_wwwroot_orig).path_clean_end_slash();
	} else {
		_wwwroot_abs = "";
	}
}

String FileCache::get_wwwroot_abs() {
	return _wwwroot_abs;
}

uint64_t FileCache::get_cache_invalidation_time() {
	return _cache_invalidation_time;
}
void FileCache::set_cache_invalidation_time(const uint64_t val) {
	_cache_invalidation_time = val;
}

bool FileCache::wwwroot_has_file(const String &p_file_url_path) {
	if (p_file_url_path.empty() || p_file_url_path == "/") {
		return false;
	}

	String fp = _wwwroot_abs + p_file_url_path;

	fp = fp.simplify_path();

	// Don't allow going outside wwwroot
	if (!fp.begins_with(_wwwroot_abs)) {
		return false;
	}

	if (!FileAccess::exists(fp)) {
		return false;
	}

	Error err;
	FileAccess *f = FileAccess::open(fp, FileAccess::READ, &err);

	if (!f) {
		return false;
	}

	if (err != OK) {
		memdelete(f);
		return false;
	}

	if (fp.begins_with("res://")) {
		return true;
	}

	String absp = f->get_path_absolute();
	memdelete(f);

	// likely a directory walking attempt. e.g. ../../../../../etc/passwd
	if (!absp.begins_with(_wwwroot_abs)) {
		return false;
	}

	return true;
}

String FileCache::wwwroot_get_file_abspath(const String &p_file_url_path) {
	if (p_file_url_path.empty() || p_file_url_path == "/") {
		return String();
	}

	String fp = _wwwroot_abs + p_file_url_path;

	fp = fp.simplify_path();

	// Don't allow going outside wwwroot
	if (!fp.begins_with(_wwwroot_abs)) {
		return String();
	}

	if (!FileAccess::exists(fp)) {
		return String();
	}

	Error err;
	FileAccess *f = FileAccess::open(fp, FileAccess::READ, &err);

	if (!f) {
		return String();
	}

	if (err != OK) {
		memdelete(f);
		return String();
	}

	if (fp.begins_with("res://")) {
		return fp;
	}

	String absp = f->get_path_absolute();
	memdelete(f);

	//likely a directory walking attempt. e.g. ../../../../../etc/passwd
	if (!absp.begins_with(_wwwroot_abs)) {
		return String();
	}

	return absp;
}

bool FileCache::wwwroot_has_folder(const String &p_url_folder_path) {
	if (p_url_folder_path.empty()) {
		return false;
	}

	String fp = _wwwroot_abs + p_url_folder_path;

	fp = fp.simplify_path();

	// Don't allow going outside wwwroot
	if (!fp.begins_with(_wwwroot_abs)) {
		return false;
	}

	if (!DirAccess::exists(fp)) {
		return false;
	}

	if (fp.begins_with("res://")) {
		return true;
	}

	String absp = DirAccess::get_filesystem_abspath_for(fp);

	//likely a directory walking attempt. e.g. ../../../../../etc/passwd
	if (!absp.begins_with(_wwwroot_abs)) {
		return false;
	}

	return true;
}
String FileCache::wwwroot_get_folder_abspath(const String &p_url_folder_path) {
	if (p_url_folder_path.empty()) {
		return String();
	}

	String fp = _wwwroot_abs + p_url_folder_path;

	fp = fp.simplify_path();

	// Don't allow going outside wwwroot
	if (!fp.begins_with(_wwwroot_abs)) {
		return String();
	}

	if (!DirAccess::exists(fp)) {
		return String();
	}

	if (fp.begins_with("res://")) {
		return fp;
	}

	String absp = DirAccess::get_filesystem_abspath_for(fp);

	//likely a directory walking attempt. e.g. ../../../../../etc/passwd
	if (!absp.begins_with(_wwwroot_abs)) {
		return String();
	}

	return String();
}

bool FileCache::wwwroot_path_exists(const String &p_url_path) {
	if (p_url_path.empty()) {
		return false;
	}

	String fp = _wwwroot_abs + p_url_path;

	fp = fp.simplify_path();

	// Don't allow going outside wwwroot
	if (!fp.begins_with(_wwwroot_abs)) {
		return false;
	}

	DirAccess *da = DirAccess::create_for_path(fp);

	if (!da) {
		return false;
	}

	bool exists = da->dir_exists(fp) || da->file_exists(fp);
	memdelete(da);

	return exists;
}

String FileCache::wwwroot_get_simplified_abs_path(const String &p_url_path) {
	String fp = _wwwroot_abs + p_url_path;

	fp = fp.simplify_path();

	// Don't allow going outside wwwroot
	if (!fp.begins_with(_wwwroot_abs)) {
		return String();
	}

	return fp;
}

bool FileCache::has_cached_body(const StringName &p_key) {
	_body_lock.read_lock();
	CacheEntry *eptr = _cache_map.getptr(p_key);

	if (!eptr) {
		_body_lock.read_unlock();
		return false;
	}

	if (_cache_invalidation_time > 0) {
		CacheEntry e = *eptr;
		_body_lock.read_unlock();

		uint64_t current_timestamp = OS::get_singleton()->get_unix_time();
		uint64_t diff = current_timestamp - e.timestamp;

		if (diff > _cache_invalidation_time) {
			return false;
		}
	}

	_body_lock.read_unlock();

	return true;
}

String FileCache::get_cached_body(const StringName &p_key) {
	_body_lock.read_lock();
	CacheEntry *eptr = _cache_map.getptr(p_key);

	if (!eptr) {
		_body_lock.read_unlock();
		return String();
	}

	CacheEntry e = *eptr;
	_body_lock.read_unlock();

	if (_cache_invalidation_time > 0) {
		uint64_t current_timestamp = OS::get_singleton()->get_unix_time();
		uint64_t diff = current_timestamp - e.timestamp;

		if (diff > _cache_invalidation_time) {
			_body_lock.write_lock();

			// Could have changed, need to check again
			const CacheEntry &ce = _cache_map[p_key];

			diff = current_timestamp - ce.timestamp;

			if (diff > _cache_invalidation_time) {
				_cache_map.erase(p_key);
				_body_lock.write_unlock();

				return String();
			} else {
				_body_lock.write_unlock();
				return ce.body;
			}
		}
	}

	return e.body;
}

void FileCache::set_cached_body(const StringName &p_key, const String &p_body) {
	_body_lock.write_lock();

	CacheEntry e;
	e.timestamp = OS::get_singleton()->get_unix_time();
	e.body = p_body;

	_cache_map[p_key] = e;

	_body_lock.write_unlock();
}

void FileCache::clear() {
	_body_lock.write_lock();
	_cache_map.clear();
	_body_lock.write_unlock();
}

void FileCache::clear_expired() {
	if (_cache_invalidation_time == 0) {
		return;
	}

	uint64_t current_timestamp = OS::get_singleton()->get_unix_time();

	_body_lock.write_lock();

	List<StringName> keys;
	_cache_map.get_key_list(&keys);

	for (List<StringName>::Element *E = keys.front(); E; E++) {
		const CacheEntry &ce = _cache_map[E->get()];

		uint64_t diff = current_timestamp - ce.timestamp;

		if (diff > _cache_invalidation_time) {
			_cache_map.erase(E->get());
		}
	}

	_body_lock.write_unlock();
}

FileCache::FileCache() {
	_cache_invalidation_time = 0;
}

FileCache::~FileCache() {
}

void FileCache::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_wwwroot"), &FileCache::get_wwwroot);
	ClassDB::bind_method(D_METHOD("set_wwwroot", "val"), &FileCache::set_wwwroot);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "wwwroot"), "set_wwwroot", "get_wwwroot");

	ClassDB::bind_method(D_METHOD("get_wwwroot_abs"), &FileCache::get_wwwroot_abs);

	ClassDB::bind_method(D_METHOD("get_cache_invalidation_time"), &FileCache::get_cache_invalidation_time);
	ClassDB::bind_method(D_METHOD("set_cache_invalidation_time", "val"), &FileCache::set_cache_invalidation_time);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "cache_invalidation_time"), "set_cache_invalidation_time", "get_cache_invalidation_time");

	ClassDB::bind_method(D_METHOD("wwwroot_has_file", "file_url_path"), &FileCache::wwwroot_has_file);
	ClassDB::bind_method(D_METHOD("wwwroot_get_file_abspath", "file_url_path"), &FileCache::wwwroot_get_file_abspath);

	ClassDB::bind_method(D_METHOD("wwwroot_has_folder", "url_folder_path"), &FileCache::wwwroot_has_folder);
	ClassDB::bind_method(D_METHOD("wwwroot_get_folder_abspath", "url_folder_path"), &FileCache::wwwroot_get_folder_abspath);

	ClassDB::bind_method(D_METHOD("wwwroot_path_exists", "url_path"), &FileCache::wwwroot_path_exists);

	ClassDB::bind_method(D_METHOD("wwwroot_get_simplified_abs_path", "url_path"), &FileCache::wwwroot_get_simplified_abs_path);

	ClassDB::bind_method(D_METHOD("has_cached_body", "key"), &FileCache::has_cached_body);
	ClassDB::bind_method(D_METHOD("get_cached_body", "key"), &FileCache::get_cached_body);
	ClassDB::bind_method(D_METHOD("set_cached_body", "key", "body"), &FileCache::set_cached_body);

	ClassDB::bind_method(D_METHOD("clear"), &FileCache::clear);
	ClassDB::bind_method(D_METHOD("clear_expired"), &FileCache::clear_expired);
}
