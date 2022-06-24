#include "file_cache.h"

#include "core/os/directory.h"

#include <iostream>

void FileCache::wwwroot_register_file(const String &file_path) {
	registered_files.insert(file_path);
}

void FileCache::wwwroot_deregister_file(const String &file_path) {
	registered_files.erase(file_path);
}

bool FileCache::wwwroot_has_file(const String &file_path) {
	return registered_files.find(file_path) != registered_files.end();
}

void FileCache::wwwroot_refresh_cache() {
	_lock.write_lock();

	registered_files.clear();

	wwwroot.path_clean_end_slash();

	wwwroot_evaluate_dir(wwwroot.c_str());

	_lock.write_unlock();
}

void FileCache::wwwroot_evaluate_dir(const char *path, const bool should_exist) {
	Ref<Directory> dir;
	dir.instance();

	ERR_FAIL_COND_MSG(dir->open_dir(path) != OK, "Error opening wwwroot! folder: " + String(path));

	while (dir->next()) {
		if (dir->current_is_file()) {
			String np = dir->current_get_path_cstr();

			np = np.substr(wwwroot.size(), np.size() - wwwroot.size());

			registered_files.insert(np);
		} else {
			wwwroot_evaluate_dir(dir->current_get_path_cstr());
		}
	}

	dir->close_dir();
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

	body->append_str(e->body);

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

	for (std::map<String, CacheEntry *>::iterator E = cache_map.begin(); E != cache_map.end(); E++) {
		CacheEntry * ce = E->second;

		if (ce) {
			delete ce;
		}
	}
	
	cache_map.clear();

	_lock.write_unlock();
}

FileCache::FileCache(bool singleton) {
	if (singleton) {
		if (_instance) {
			printf("FileCache: Filecache instance is set as singleton, but an another FileCache instance is already set up as singleton! Ignoring setting!\n");
		} else {
			_instance = this;
		}
	}

	cache_invalidation_time = 1;
}

FileCache::~FileCache() {
	registered_files.clear();

	if (_instance == this)
		_instance = nullptr;
}

FileCache *FileCache::get_singleton() {
	return _instance;
}

FileCache *FileCache::_instance = nullptr;
