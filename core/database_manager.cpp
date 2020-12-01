#include "database_manager.h"

void DatabaseManager::load() {
    //go thourgh settings, and create all the defined db backends
}

DatabaseManager *DatabaseManager::get_singleton() {
    return _instance;
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