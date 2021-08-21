#include "resource.h"

#if DATABASES_ENABLED
#include "core/database/database.h"
#include "core/database/database_manager.h"
#endif

int Resource::get_id() {
	return _id;
}
void Resource::set_id(const int value) {
	_id = value;

	changed();
}

bool Resource::get_dirty() {
	return _dirty;
}
void Resource::set_dirty(const bool value) {
	_dirty = value;
}

std::string Resource::get_resource_name() {
	return _resource_name;
}
void Resource::set_resource_name(const std::string &name) {
	_resource_name = name;

	changed();
}

void Resource::changed() {
	_dirty = true;
}

void Resource::save() {
#if DATABASES_ENABLED
	sql_save();
#else
	file_save();
#endif
}
void Resource::load() {
#if DATABASES_ENABLED
	sql_load();
#else
	file_load();
#endif
}
void Resource::migrate() {
#if DATABASES_ENABLED
	//todo!

	sql_delete_tables();
	sql_create_tables();

	//sql_migrate();
#else
	file_ensure_directory_exist();
#endif
}

#if DATABASES_ENABLED
void Resource::sql_save() {
	sql_save(DatabaseManager::get_singleton()->ddb);
}
void Resource::sql_load() {
	sql_load(DatabaseManager::get_singleton()->ddb);
}
void Resource::sql_migrate() {
	sql_migrate(DatabaseManager::get_singleton()->ddb);
}
void Resource::sql_create_tables() {
	sql_create_tables(DatabaseManager::get_singleton()->ddb);
}
void Resource::sql_delete_tables() {
	sql_delete_tables(DatabaseManager::get_singleton()->ddb);
}

void Resource::sql_save(Database *db) {
}
void Resource::sql_load(Database *db) {
}
void Resource::sql_migrate(Database *db) {
}
void Resource::sql_create_tables(Database *db) {
}
void Resource::sql_delete_tables(Database *db) {
}
#endif

void Resource::file_save() {
}
void Resource::file_load() {
}
void Resource::file_ensure_directory_exist() {
}
std::string Resource::file_get_base_path() {
	return "./resources/";
}

std::string Resource::to_json(rapidjson::Document *document) {
	return "";
}
void Resource::from_json(const std::string &data) {
}

Resource::Resource() {
	_id = 0;
	_dirty = false;
	_resource_name = get_class();
}

Resource::~Resource() {
}