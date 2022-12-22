#ifndef FILE_CACHE_H
#define FILE_CACHE_H

#include "core/containers/hash_map.h"
#include "core/containers/map.h"
#include "core/os/os.h"
#include "core/os/rw_lock.h"
#include "core/string/ustring.h"
#include "core/containers/vector.h"

#include "core/object/reference.h"

class FileCache : public Reference {
	GDCLASS(FileCache, Reference);

public:
	String get_wwwroot();
	void set_wwwroot(const String &val);

	String get_wwwroot_abs();

	int get_cache_invalidation_time();
	void set_cache_invalidation_time(const int &val);

	//Note: file path should be the url you want to access the file with, including lead slash
	//e.g. http://127.0.0.1/a/b/d.jpg -> /a/b/d.jpg
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

	RWLock _lock;
	Map<String, CacheEntry *> cache_map;

	String _wwwroot_orig;
	String _wwwroot;

	struct RegisteredFileEntry {
		String orig_path;
		String lowercase_path;
	};

	Vector<RegisteredFileEntry> _registered_files;
};

#endif
