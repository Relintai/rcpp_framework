#include "database_manager.h"

void DatabaseManager::load() {
	//go thourgh settings, and create all the defined db backends
}

uint32_t DatabaseManager::create_database(const String &name) {
    Database *db = _create_database(name);

    if (!db) {
		printf("create_database: %s, returned db is null!", name.c_str());
        return -1;
	}

	printf("Database %s successfully created!\n", name.c_str());

    databases.push_back(db);

	if (ddb == nullptr) {
		printf("Database %s has been set as the default database!\n", name.c_str());
		ddb = db;
	}

    return databases.size() - 1;
}

DatabaseManager *DatabaseManager::get_singleton() {
	return _instance;
}

void DatabaseManager::_register_db_creation_func(const String &name, std::function<Database *()> func) {
	if (!func) {
		printf("_register_db_creation_func: %s, func is wrong!", name.c_str());
        return;
	}

	_db_creation_func_map[name] = func;
}

void DatabaseManager::_unregister_db_creation_func(const String &name) {
	_db_creation_func_map.erase(name);
}

Database *DatabaseManager::_create_database(const String &name) {
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

	ddb = nullptr;
}

DatabaseManager::~DatabaseManager() {
	_instance = nullptr;

	for (uint32_t i = 0; i < databases.size(); ++i) {
		delete databases[i];
	}
}

DatabaseManager *DatabaseManager::_instance = nullptr;

std::map<String, std::function<Database *()> > DatabaseManager::_db_creation_func_map;