#include "file_cache.h"

#include "core/os/dir_access.h"
#include "core/os/file_access.h"
#include "core/os/os.h"
#include "core/string/print_string.h"
#include "core/config/project_settings.h"

String FileCache::get_wwwroot() {
	return _wwwroot_orig;
}
void FileCache::set_wwwroot(const String &val) {
	_wwwroot_orig = val;
}

String FileCache::get_wwwroot_abs() {
	return _wwwroot;
}

int FileCache::get_cache_invalidation_time() {
	return static_cast<int>(cache_invalidation_time);
}
void FileCache::set_cache_invalidation_time(const int &val) {
	cache_invalidation_time = static_cast<uint64_t>(val);
}

void FileCache::wwwroot_register_file(const String &file_path) {
	RegisteredFileEntry e;
	e.orig_path = file_path;
	e.lowercase_path = file_path.to_lower();

	_registered_files.push_back(e);
}

void FileCache::wwwroot_deregister_file(const String &file_path) {
	for (int i = 0; i < _registered_files.size(); ++i) {
		const RegisteredFileEntry &e = _registered_files[i];

		if (file_path == e.orig_path) {
			_registered_files.remove(i);
			return;
		}
	}
}

bool FileCache::wwwroot_has_file(const String &file_path) {
	//return registered_files.has(file_path);

	for (int i = 0; i < _registered_files.size(); ++i) {
		const RegisteredFileEntry &e = _registered_files[i];

		if (file_path == e.lowercase_path) {
			return true;
		}
	}

	return false;
}

int FileCache::wwwroot_get_file_index(const String &file_path) {
	for (int i = 0; i < _registered_files.size(); ++i) {
		const RegisteredFileEntry &e = _registered_files[i];

		if (file_path == e.lowercase_path) {
			return i;
		}
	}

	return -1;
}

String FileCache::wwwroot_get_file_orig_path(const int index) {
	ERR_FAIL_INDEX_V(index, _registered_files.size(), "");

	return _registered_files[index].orig_path;
}

String FileCache::wwwroot_get_file_orig_path_abs(const int index) {
	return get_wwwroot_abs() + wwwroot_get_file_orig_path(index);
}

void FileCache::wwwroot_refresh_cache() {
	_lock.write_lock();

	_registered_files.clear();

	if (_wwwroot_orig != "") {
		_wwwroot = DirAccess::get_filesystem_abspath_for(_wwwroot_orig);
		_wwwroot = _wwwroot.path_clean_end_slash();

		wwwroot_evaluate_dir(_wwwroot);
	}

	_lock.write_unlock();
}

void FileCache::wwwroot_evaluate_dir(const String &path, const bool should_exist) {
	DirAccess *da = DirAccess::open(path);

	ERR_FAIL_COND_MSG(!da, "Error opening wwwroot! folder: " + path);

	da->list_dir_begin();
	String f = da->get_next();

	while (f != String()) {
		if (f == "." || f == "..") {
			f = da->get_next();
			continue;
		}

		if (!da->current_is_dir()) {
			String np = path + "/" + f;
			np = np.substr(_wwwroot.size() - 1, np.size() - _wwwroot.size());
			wwwroot_register_file(np);

		} else {
			wwwroot_evaluate_dir(path + "/" + f);
		}

		f = da->get_next();
	}
	da->list_dir_end();

	memdelete(da);
}

bool FileCache::get_cached_body(const String &path, String *body) {
	//TODO ERROR MACRO body == null

	_lock.read_lock();
	CacheEntry *e = cache_map[path];
	_lock.read_unlock();

	if (!e) {
		return false;
	}

	if (cache_invalidation_time > 0) {
		uint64_t current_timestamp = OS::get_singleton()->get_unix_time();
		uint64_t diff = current_timestamp - e->timestamp;

		if (diff > cache_invalidation_time) {
			return false;
		}
	}

	body->operator+=(e->body);

	return true;
}

bool FileCache::has_cached_body(const String &path) {
	//TODO ERROR MACRO body == null

	_lock.read_lock();
	CacheEntry *e = cache_map[path];
	_lock.read_unlock();

	if (!e) {
		return false;
	}

	if (cache_invalidation_time > 0) {
		uint64_t current_timestamp = OS::get_singleton()->get_unix_time();
		uint64_t diff = current_timestamp - e->timestamp;

		if (diff > cache_invalidation_time) {
			return false;
		}
	}

	return true;
}

String FileCache::get_cached_body_bind(const String &path) {
	//TODO ERROR MACRO body == null

	_lock.read_lock();
	CacheEntry *e = cache_map[path];
	_lock.read_unlock();

	if (!e) {
		return "";
	}

	if (cache_invalidation_time > 0) {
		uint64_t current_timestamp = OS::get_singleton()->get_unix_time();
		uint64_t diff = current_timestamp - e->timestamp;

		if (diff > cache_invalidation_time) {
			return "";
		}
	}

	return e->body;
}

void FileCache::set_cached_body(const String &path, const String &body) {
	_lock.write_lock();

	CacheEntry *e = cache_map[path];

	if (!e) {
		e = memnew(CacheEntry());
		cache_map[path] = e;
	}

	uint64_t current_timestamp = OS::get_singleton()->get_unix_time();

	e->timestamp = current_timestamp;
	e->body = body;

	_lock.write_unlock();
}

void FileCache::clear() {
	_lock.write_lock();

	_registered_files.clear();

	for (Map<String, CacheEntry *>::Element *E = cache_map.front(); E; E++) {
		CacheEntry *ce = E->get();

		if (ce) {
			memdelete(ce);
		}
	}

	cache_map.clear();

	_lock.write_unlock();
}

FileCache::FileCache() {
	cache_invalidation_time = 0;
}

FileCache::~FileCache() {
	_registered_files.clear();
}

void FileCache::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_wwwroot"), &FileCache::get_wwwroot);
	ClassDB::bind_method(D_METHOD("set_wwwroot", "val"), &FileCache::set_wwwroot);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "wwwroot"), "set_wwwroot", "get_wwwroot");

	ClassDB::bind_method(D_METHOD("get_cache_invalidation_time"), &FileCache::get_cache_invalidation_time);
	ClassDB::bind_method(D_METHOD("set_cache_invalidation_time", "val"), &FileCache::set_cache_invalidation_time);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "cache_invalidation_time"), "set_cache_invalidation_time", "get_cache_invalidation_time");

	ClassDB::bind_method(D_METHOD("wwwroot_register_file", "file_path"), &FileCache::wwwroot_register_file);
	ClassDB::bind_method(D_METHOD("wwwroot_deregister_file", "file_path"), &FileCache::wwwroot_deregister_file);
	ClassDB::bind_method(D_METHOD("wwwroot_has_file", "file_path"), &FileCache::wwwroot_has_file);
	ClassDB::bind_method(D_METHOD("wwwroot_refresh_cache"), &FileCache::wwwroot_refresh_cache);
	ClassDB::bind_method(D_METHOD("wwwroot_evaluate_dir", "file_path", "should_exist "), &FileCache::wwwroot_evaluate_dir, true);

	ClassDB::bind_method(D_METHOD("get_cached_body", "path"), &FileCache::get_cached_body_bind);
	ClassDB::bind_method(D_METHOD("has_cached_body", "path"), &FileCache::has_cached_body);
	ClassDB::bind_method(D_METHOD("set_cached_body", "path", "body"), &FileCache::set_cached_body);

	ClassDB::bind_method(D_METHOD("clear"), &FileCache::clear);
}
