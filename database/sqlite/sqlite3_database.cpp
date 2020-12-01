#include "sqlite3_database.h"

#include "core/database_manager.h"

Database *SQLite3Database::_creation_func() {
	return new SQLite3Database();
}

void SQLite3Database::_register() {
	DatabaseManager::_register_db_creation_func("sqlite", SQLite3Database::_creation_func);
}

void SQLite3Database::_unregister() {
	DatabaseManager::_unregister_db_creation_func("sqlite");
}
