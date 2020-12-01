#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <vector>
#include <map>
#include <functional>

#include "database.h"

class DatabaseManager {
public:
    std::vector<Database *> databases;

    void load();

    static DatabaseManager *get_singleton();

    //note: not threadsafe, create these at the start of your program!
    uint32_t create_database(const std::string &name);

    static void _register_db_creation_func(const std::string &name, std::function<Database*()> func);
    static void _unregister_db_creation_func(const std::string &name);

    static Database *_create_database(const std::string &name);

    DatabaseManager();
    ~DatabaseManager();

private:
    static DatabaseManager * _instance;

    static std::map<std::string, std::function<Database *()> > _db_creation_func_map;
};

#endif