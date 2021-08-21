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
	enum ResourcePropertyFlags {
		PROPERTY_FLAG_NONE = 0,
		PROPERTY_FLAG_SQL_EXCAPE = 1 << 0,
		PROPERTY_FLAG_SANITIZE_HTML_SPECIAL_CHARS = 1 << 1,
		PROPERTY_FLAG_DONT_SERIALIZE = 1 << 2,

		PROPERTY_FLAG_USER_INPUT = PROPERTY_FLAG_SQL_EXCAPE,
		PROPERTY_FLAG_VISIBLE_INPUT = PROPERTY_FLAG_SQL_EXCAPE | PROPERTY_FLAG_SANITIZE_HTML_SPECIAL_CHARS,
	};

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

	std::string to_json(rapidjson::Document *into = nullptr);
	void from_json(const std::string &data);

	Resource();
	~Resource();

protected:
	enum ResourcePropertyType {
		TYPE_NULL = 0,
		TYPE_INT,
		TYPE_FLOAT,
		TYPE_STRING,
		TYPE_BOOL,
		TYPE_RESOURCE,
		TYPE_VECTOR_INT,
		TYPE_VECTOR_FLOAT,
		TYPE_VECTOR_STRING,
		TYPE_VECTOR_BOOL,
		TYPE_VECTOR_RESOURCE,
	};

	struct ResourcePropertyBase {
		ResourcePropertyType type;

		ResourcePropertyBase() {
			type = TYPE_NULL;
		}
	};

	template <class G, class S>
	struct ResourceProperty : public ResourcePropertyBase {
		G getter;
		S setter;
	};

	std::map<std::string, ResourcePropertyBase *> _property_map;

private:
	int _id;
	bool _dirty;
	std::string _resource_name;
};

#endif