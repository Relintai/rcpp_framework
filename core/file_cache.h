#ifndef FILE_CACHE_H
#define FILE_CACHE_H

#include <string>
#include <set>
#include <map>
#include <chrono>
#include <mutex>

class FileCache {
public:
    std::string wwwroot;
    int cache_invalidation_time;

    //Note: file path should be the url you want to access the file with, inculding lead slash
    //e.g. http://127.0.0.1/a/b/d.jpg -> /a/b/d.jpg
    void wwwroot_register_file(const std::string &file_path);
    void wwwroot_deregister_file(const std::string &file_path);
    bool wwwroot_has_file(const std::string &file_path);
    void wwwroot_refresh_cache();
    void wwwroot_evaluate_dir(const char *path);

    bool get_cached_body(const std::string &path, std::string *body);
    void set_cached_body(const std::string &path, const std::string &body);

    FileCache(bool singleton = false);
    virtual ~FileCache();

    static FileCache *get_singleton();

    std::set<std::string> registered_files;

protected:

    struct CacheEntry {
        int64_t timestamp;
        std::string body;

        CacheEntry() {
            timestamp = 0;
        }
    };

    std::mutex cache_mutex;

    std::map<std::string, CacheEntry *> cache_map;

private:
    static FileCache *_instance;
};

#endif