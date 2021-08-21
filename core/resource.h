#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>

#include "libs/rapidjson/document.h"
#include "reference.h"

#if DATABASES_ENABLED
class Database;
#endif

class Resource : public Reference {
	RCPP_OBJECT(Resource, Reference);

public:
	int get_id();
	void set_id(const int value);

	bool get_dirty();
	void set_dirty(const bool value);

	std::string get_resource_name();
	void set_resource_name(const std::string &name);

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

	std::string to_json(rapidjson::Document *document = nullptr);
	void from_json(const std::string &data);

	//todo properties

	Resource();
	~Resource();

private:
	int _id;
	bool _dirty;
	std::string _resource_name;
};

#endif