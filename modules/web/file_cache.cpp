#include "file_cache.h"

#include "core/os/dir_access.h"
#include "core/os/file_access.h"

String FileCache::get_wwwroot() {
	return wwwroot;
}
void FileCache::set_wwwroot(const String &val) {
	wwwroot = val;
}

int FileCache::get_cache_invalidation_time() {
	return cache_invalidation_time;
}
void FileCache::set_cache_invalidation_time(const int &val) {
	cache_invalidation_time = val;
}

void FileCache::wwwroot_register_file(const String &file_path) {
	registered_files.insert(file_path);
}

void FileCache::wwwroot_deregister_file(const String &file_path) {
	registered_files.erase(file_path);
}

bool FileCache::wwwroot_has_file(const String &file_path) {
	return registered_files.has(file_path);
}

void FileCache::wwwroot_refresh_cache() {
	_lock.write_lock();

	registered_files.clear();

	wwwroot.path_clean_end_slash();

	wwwroot_evaluate_dir(wwwroot);

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
			np = np.substr(wwwroot.size(), np.size() - wwwroot.size());
			registered_files.insert(np);
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

	registered_files.clear();

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
	registered_files.clear();
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
