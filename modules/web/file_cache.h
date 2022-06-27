#ifndef FILE_CACHE_H
#define FILE_CACHE_H

#include "core/map.h"
#include "core/os/os.h"
#include "core/os/rw_lock.h"
#include "core/set.h"
#include "core/ustring.h"

#include "core/reference.h"

class FileCache : public Reference {
	GDCLASS(FileCache, Reference);

public:
	String get_wwwroot();
	void set_wwwroot(const String &val);

	int get_cache_invalidation_time();
	void set_cache_invalidation_time(const int &val);

	//Note: file path should be the url you want to access the file with, inculding lead slash
	//e.g. http://127.0.0.1/a/b/d.jpg -> /a/b/d.jpg
	void wwwroot_register_file(const String &file_path);
	void wwwroot_deregister_file(const String &file_path);
	bool wwwroot_has_file(const String &file_path);
	void wwwroot_refresh_cache();
	void wwwroot_evaluate_dir(const String &path, const bool should_exist = true);

	bool get_cached_body(const String &path, String *body);
	bool has_cached_body(const String &path);
	String get_cached_body_bind(const String &path);
	void set_cached_body(const String &path, const String &body);

	void clear();

	FileCache();
	~FileCache();

	String wwwroot;
	uint64_t cache_invalidation_time;

	Set<String> registered_files;

protected:
	static void _bind_methods();

	struct CacheEntry {
		uint64_t timestamp;
		String body;

		CacheEntry() {
			timestamp = 0;
		}
	};

	RWLock _lock;
	Map<String, CacheEntry *> cache_map;
};

#endif
