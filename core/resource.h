#ifndef RESOURCE_H
#define RESOURCE_H

#include <functional>
#include <map>
#include <string>

#include "libs/rapidjson/document.h"
#include "reference.h"

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

	std::string to_json(rapidjson::Document *document = nullptr);
	void from_json(const std::string &data);

	//todo add a variant like class. (Or variant itself from godot.)
	int get_int(const std::string &property);
	float get_float(const std::string &property);
	std::string get_string(const std::string &property);
	bool get_bool(const std::string &property);
	Ref<Resource> get_resource(const std::string &property);

	std::vector<int> get_int_vector(const std::string &property);
	std::vector<float> get_float_vector(const std::string &property);
	std::vector<std::string> get_string_vector(const std::string &property);
	std::vector<bool> get_bool_vector(const std::string &property);
	std::vector<Ref<Resource> > get_resource_vector(const std::string &property);

	void set_int(const std::string &property, const int data);
	void set_float(const std::string &property, const float data);
	void set_string(const std::string &property, const std::string &data);
	void set_bool(const std::string &property, const bool data);
	void set_resource(const std::string &property, const Ref<Resource> &data);

	void set_int_vector(const std::string &property, const std::vector<int> &data);
	void set_float_vector(const std::string &property, const std::vector<float> &data);
	void set_string_vector(const std::string &property, const std::vector<std::string> &data);
	void set_bool_vector(const std::string &property, const std::vector<bool> &data);
	void set_resource_vector(const std::string &property, const std::vector<Ref<Resource> > &data);

	void add_property_int(const std::string &name, std::function<int(Resource *)> getter, std::function<void(Resource *, int)> setter, const int property_flags = 0);
	void add_property_float(const std::string &name, std::function<float(Resource *)> getter, std::function<void(Resource *, float)> setter, const int property_flags = 0);
	void add_property_string(const std::string &name, std::function<std::string(Resource *)> getter, std::function<void(Resource *, std::string)> setter, const int property_flags = 0);
	void add_property_bool(const std::string &name, std::function<bool(Resource *)> getter, std::function<void(Resource *, bool)> setter, const int property_flags = 0);
	void add_property_resource(const std::string &name, std::function<Ref<Resource>(Resource *)> getter, std::function<void(Resource *, Ref<Resource>)> setter, const int property_flags = 0);

	void add_property_int_vector(const std::string &name, std::function<std::vector<int>(Resource *)> getter, std::function<void(Resource *, std::vector<int>)> setter, const int property_flags = 0);
	void add_property_float_vector(const std::string &name, std::function<std::vector<float>(Resource *)> getter, std::function<void(Resource *, std::vector<float>)> setter, const int property_flags = 0);
	void add_property_string_vector(const std::string &name, std::function<std::vector<std::string>(Resource *)> getter, std::function<void(Resource *, std::vector<std::string>)> setter, const int property_flags = 0);
	void add_property_bool_vector(const std::string &name, std::function<std::vector<bool>(Resource *)> getter, std::function<void(Resource *, std::vector<bool>)> setter, const int property_flags = 0);
	void add_property_resource_vector(const std::string &name, std::function<std::vector<Ref<Resource> >(Resource *)> getter, std::function<void(Resource *, std::vector<Ref<Resource> >)> setter, const int property_flags = 0);

	virtual void register_properties();

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