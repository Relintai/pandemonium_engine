#ifndef FILE_CACHE_H
#define FILE_CACHE_H

#include "core/string.h"

#include <set>
#include <map>
#include <chrono>
#include "core/threading/rw_lock.h"

#include "core/object.h"


class FileCache : public Object {
    RCPP_OBJECT(FileCache, Object);

public:
    String wwwroot;
    int cache_invalidation_time;

    //Note: file path should be the url you want to access the file with, inculding lead slash
    //e.g. http://127.0.0.1/a/b/d.jpg -> /a/b/d.jpg
    void wwwroot_register_file(const String &file_path);
    void wwwroot_deregister_file(const String &file_path);
    bool wwwroot_has_file(const String &file_path);
    void wwwroot_refresh_cache();
    void wwwroot_evaluate_dir(const char *path, const bool should_exist = true);

    bool get_cached_body(const String &path, String *body);
    void set_cached_body(const String &path, const String &body);

    void clear();

    FileCache(bool singleton = false);
    virtual ~FileCache();

    static FileCache *get_singleton();

    std::set<String> registered_files;

protected:

    struct CacheEntry {
        int64_t timestamp;
        String body;

        CacheEntry() {
            timestamp = 0;
        }
    };

    RWLock _lock;
    std::map<String, CacheEntry *> cache_map;

private:
    static FileCache *_instance;
};

#endif