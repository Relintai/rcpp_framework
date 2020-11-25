#ifndef FILE_CACHE_H
#define FILE_CACHE_H

#include <string>
#include <set>

class FileCache {
public:
    std::string wwwroot;

    //Note: file path should be the url you want to access the file with, inculding lead slash
    //e.g. http://127.0.0.1/a/b/d.jpg -> /a/b/d.jpg
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