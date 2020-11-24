#ifndef FILE_CACHE_H
#define FILE_CACHE_H

#include <string>
#include <set>

class FileCache {
public:
    std::string wwwroot;

    void register_file(const std::string &file_path);
    void deregister_file(const std::string &file_path);

    bool has_file(const std::string &file_path);

    void refresh();
    void evaluate_dir(const char *path);

    FileCache();
    virtual ~FileCache();

    static FileCache *get_instance();

    std::set<std::string> registered_files;

private:
    static FileCache *_instance;
};

#endif