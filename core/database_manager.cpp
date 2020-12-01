#include "database_manager.h"

void DatabaseManager::load() {
	//go thourgh settings, and create all the defined db backends
}

uint32_t DatabaseManager::create_database(const std::string &name) {
    Database *db = _create_database(name);

    if (!db) {
		printf("create_database: %s, returned db is null!", name.c_str());
        return -1;
	}

    databases.push_back(db);

    return databases.size() - 1;
}

DatabaseManager *DatabaseManager::get_singleton() {
	return _instance;
}

void DatabaseManager::_register_db_creation_func(const std::string &name, std::function<Database *()> func) {
	if (!func) {
		printf("_register_db_creation_func: %s, func is wrong!", name.c_str());
        return;
	}

	_db_creation_func_map[name] = func;
}

void DatabaseManager::_unregister_db_creation_func(const std::string &name) {
	_db_creation_func_map.erase(name);
}

Database *DatabaseManager::_create_database(const std::string &name) {
	std::function<Database *()> func = _db_creation_func_map[name];

	if (!func) {
		printf("_create_database: %s, func is wrong!", name.c_str());
        return nullptr;
	}

    Database *db = func();

    if (!db) {
		printf("_create_database: %s, returned db is null!", name.c_str());
	}

    return db;
}

DatabaseManager::DatabaseManager() {
	_instance = this;
}

DatabaseManager::~DatabaseManager() {
	_instance = nullptr;

	for (uint32_t i = 0; i < databases.size(); ++i) {
		delete databases[i];
	}
}

DatabaseManager *DatabaseManager::_instance = nullptr;

std::map<std::string, std::function<Database *()> > DatabaseManager::_db_creation_func_map;