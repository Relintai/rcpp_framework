#ifndef RESOURCE_H
#define RESOURCE_H

#include <functional>
#include <map>
#include <string>

#include "reference.h"

#include "rapidjson/document.h"

#if DATABASES_ENABLED
class Database;
#endif

class Resource : public Reference {
	RCPP_OBJECT(Resource, Reference);

public:
	int id;
	bool dirty;
	std::string resource_name;

	void changed();

	virtual void save();
	virtual void load();
	virtual void migrate();

#if DATABASES_ENABLED
	virtual void sql_save();
	virtual void sql_load();
	virtual void sql_migrate();
	virtual void sql_create_tables();
	virtual void sql_delete_tables();

	virtual void sql_save(Database *db);
	virtual void sql_load(Database *db);
	virtual void sql_migrate(Database *db);
	virtual void sql_create_tables(Database *db);
	virtual void sql_delete_tables(Database *db);
#endif

	virtual void file_save();
	virtual void file_load();
	virtual void file_ensure_directory_exist();
	virtual std::string file_get_base_path();

	virtual std::string to_json(rapidjson::Document *into = nullptr);
	virtual void from_json(const std::string &data);

	Resource();
	~Resource();

private:
};

#endif