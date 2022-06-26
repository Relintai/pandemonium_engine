#include "file_cache.h"

#include "core/os/dir_access.h"
#include "core/os/file_access.h"

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

	int64_t current_timestamp = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	int64_t diff = current_timestamp - e->timestamp;

	if (diff > cache_invalidation_time) {
		return false;
	}

	body->operator+=(e->body);

	return true;
}

void FileCache::set_cached_body(const String &path, const String &body) {
	_lock.write_lock();

	CacheEntry *e = cache_map[path];

	if (!e) {
		e = new CacheEntry();
		cache_map[path] = e;
	}

	int64_t current_timestamp = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

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
			delete ce;
		}
	}

	cache_map.clear();

	_lock.write_unlock();
}

FileCache::FileCache() {
	cache_invalidation_time = 1;
}

FileCache::~FileCache() {
	registered_files.clear();
}
