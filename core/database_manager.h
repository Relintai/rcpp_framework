#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <vector>

#include "database.h"

class DatabaseManager {
public:
    std::vector<Database *> databases;

    void load();

    DatabaseManager *get_singleton();

    DatabaseManager();
    ~DatabaseManager();

private:
    static DatabaseManager * _instance;
};

#endif