#include "resource.h"

#include <vector>

#if DATABASES_ENABLED
#include "core/database/database.h"
#include "core/database/database_manager.h"
#endif

//For properties

typedef std::string String;
typedef std::vector<int> IntVector;
typedef std::vector<float> FloatVector;
typedef std::vector<std::string> StringVector;
typedef std::vector<bool> BoolVector;
typedef std::vector<Ref<Resource> > ResourceVector;

#define RESOURCE_PROPERTY_GET_IMPL(property_type, type_enum, default_value, func_name)                                                                                                                                                               \
	ResourcePropertyBase *p = _property_map[property];                                                                                                                                                                                               \
                                                                                                                                                                                                                                                     \
	if (!p) {                                                                                                                                                                                                                                        \
		printf("ERROR Resource:: func_name property can't be found! %s\n", property.c_str());                                                                                                                                                        \
                                                                                                                                                                                                                                                     \
		return default_value;                                                                                                                                                                                                                        \
	}                                                                                                                                                                                                                                                \
                                                                                                                                                                                                                                                     \
	if (p->type != type_enum) {                                                                                                                                                                                                                      \
		/*If this happens property binding code in this class is incorrect*/                                                                                                                                                                         \
		printf("ERROR Resource:: func_name property's type is not property_type! %s\n", property.c_str());                                                                                                                                           \
                                                                                                                                                                                                                                                     \
		return default_value;                                                                                                                                                                                                                        \
	}                                                                                                                                                                                                                                                \
                                                                                                                                                                                                                                                     \
	ResourceProperty<std::function<property_type(Resource *)>, std::function<void(Resource *, property_type)> > *pp = static_cast<ResourceProperty<std::function<property_type(Resource *)>, std::function<void(Resource *, property_type)> > *>(p); \
                                                                                                                                                                                                                                                     \
	if (!pp) {                                                                                                                                                                                                                                       \
		printf("ERROR Resource:: func_name dynamic cast failed! %s\n", property.c_str());                                                                                                                                                            \
                                                                                                                                                                                                                                                     \
		return default_value;                                                                                                                                                                                                                        \
	}                                                                                                                                                                                                                                                \
                                                                                                                                                                                                                                                     \
	return pp->getter(this);

#define RESOURCE_PROPERTY_SET_IMPL(property_type, type_enum, func_name)                                                                                                                                                                              \
	ResourcePropertyBase *p = _property_map[property];                                                                                                                                                                                               \
                                                                                                                                                                                                                                                     \
	if (!p) {                                                                                                                                                                                                                                        \
		printf("ERROR Resource:: func_name property can't be found! %s\n", property.c_str());                                                                                                                                                        \
                                                                                                                                                                                                                                                     \
		return;                                                                                                                                                                                                                                      \
	}                                                                                                                                                                                                                                                \
                                                                                                                                                                                                                                                     \
	if (p->type != type_enum) {                                                                                                                                                                                                                      \
		/*If this happens property binding code in this class is incorrect*/                                                                                                                                                                         \
		printf("ERROR Resource:: func_name property's type is not property_type! %s\n", property.c_str());                                                                                                                                           \
                                                                                                                                                                                                                                                     \
		return;                                                                                                                                                                                                                                      \
	}                                                                                                                                                                                                                                                \
                                                                                                                                                                                                                                                     \
	ResourceProperty<std::function<property_type(Resource *)>, std::function<void(Resource *, property_type)> > *pp = static_cast<ResourceProperty<std::function<property_type(Resource *)>, std::function<void(Resource *, property_type)> > *>(p); \
                                                                                                                                                                                                                                                     \
	if (!pp) {                                                                                                                                                                                                                                       \
		printf("ERROR Resource:: func_name dynamic cast failed! %s\n", property.c_str());                                                                                                                                                            \
                                                                                                                                                                                                                                                     \
		return;                                                                                                                                                                                                                                      \
	}                                                                                                                                                                                                                                                \
                                                                                                                                                                                                                                                     \
	return pp->setter(this, data);

#define RESOURCE_PROPERTY_ADD_IMPL(property_type, type_enum)                                                                                                                                                                               \
	ResourceProperty<std::function<property_type(Resource *)>, std::function<void(Resource *, property_type)> > *prop = new ResourceProperty<std::function<property_type(Resource *)>, std::function<void(Resource *, property_type)> >(); \
                                                                                                                                                                                                                                           \
	prop->type = type_enum;                                                                                                                                                                                                                \
	prop->getter = getter;                                                                                                                                                                                                                 \
	prop->setter = setter;                                                                                                                                                                                                                 \
                                                                                                                                                                                                                                           \
	_property_map[name] = prop;

//Properties stuff end

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

//properties

int Resource::get_int(const std::string &property) {
	RESOURCE_PROPERTY_GET_IMPL(int, TYPE_INT, 0, get_int);
}
float Resource::get_float(const std::string &property) {
	RESOURCE_PROPERTY_GET_IMPL(float, TYPE_FLOAT, 0, get_float);
}
std::string Resource::get_string(const std::string &property) {
	RESOURCE_PROPERTY_GET_IMPL(String, TYPE_STRING, "", get_string);
}
bool Resource::get_bool(const std::string &property) {
	RESOURCE_PROPERTY_GET_IMPL(bool, TYPE_BOOL, 0, get_bool);
}
Ref<Resource> Resource::get_resource(const std::string &property) {
	RESOURCE_PROPERTY_GET_IMPL(Ref<Resource>, TYPE_RESOURCE, Ref<Resource>(), get_resource);
}

std::vector<int> Resource::get_int_vector(const std::string &property) {
	RESOURCE_PROPERTY_GET_IMPL(IntVector, TYPE_VECTOR_INT, IntVector(), get_int_vector);
}
std::vector<float> Resource::get_float_vector(const std::string &property) {
	RESOURCE_PROPERTY_GET_IMPL(FloatVector, TYPE_VECTOR_FLOAT, FloatVector(), get_float_vector);
}
std::vector<std::string> Resource::get_string_vector(const std::string &property) {
	RESOURCE_PROPERTY_GET_IMPL(StringVector, TYPE_VECTOR_STRING, StringVector(), get_string_vector);
}
std::vector<bool> Resource::get_bool_vector(const std::string &property) {
	RESOURCE_PROPERTY_GET_IMPL(BoolVector, TYPE_VECTOR_BOOL, BoolVector(), get_bool_vector);
}
std::vector<Ref<Resource> > Resource::get_resource_vector(const std::string &property) {
	RESOURCE_PROPERTY_GET_IMPL(ResourceVector, TYPE_VECTOR_RESOURCE, ResourceVector(), get_resource_vector);
}

void Resource::set_int(const std::string &property, const int data) {
	RESOURCE_PROPERTY_SET_IMPL(int, TYPE_INT, set_int);
}
void Resource::set_float(const std::string &property, const float data) {
	RESOURCE_PROPERTY_SET_IMPL(float, TYPE_FLOAT, get_float);
}
void Resource::set_string(const std::string &property, const std::string &data) {
	RESOURCE_PROPERTY_SET_IMPL(String, TYPE_STRING, get_string);
}
void Resource::set_bool(const std::string &property, const bool data) {
	RESOURCE_PROPERTY_SET_IMPL(bool, TYPE_BOOL, get_bool);
}
void Resource::set_resource(const std::string &property, const Ref<Resource> &data) {
	RESOURCE_PROPERTY_SET_IMPL(Ref<Resource>, TYPE_RESOURCE, get_resource);
}

void Resource::set_int_vector(const std::string &property, const std::vector<int> &data) {
	RESOURCE_PROPERTY_SET_IMPL(IntVector, TYPE_VECTOR_INT, get_int_vector);
}

void Resource::set_float_vector(const std::string &property, const std::vector<float> &data) {
	RESOURCE_PROPERTY_SET_IMPL(FloatVector, TYPE_VECTOR_FLOAT, get_float_vector);
}
void Resource::set_string_vector(const std::string &property, const std::vector<std::string> &data) {
	RESOURCE_PROPERTY_SET_IMPL(StringVector, TYPE_VECTOR_STRING, get_string_vector);
}
void Resource::set_bool_vector(const std::string &property, const std::vector<bool> &data) {
	RESOURCE_PROPERTY_SET_IMPL(BoolVector, TYPE_VECTOR_BOOL, get_bool_vector);
}
void Resource::set_resource_vector(const std::string &property, const std::vector<Ref<Resource> > &data) {
	RESOURCE_PROPERTY_SET_IMPL(ResourceVector, TYPE_VECTOR_RESOURCE, get_resource_vector);
}

void Resource::add_property_int(const std::string &name, std::function<int(Resource *)> getter, std::function<void(Resource *, int)> setter, const int property_flags) {
	RESOURCE_PROPERTY_ADD_IMPL(int, TYPE_INT);
}
void Resource::add_property_float(const std::string &name, std::function<float(Resource *)> getter, std::function<void(Resource *, float)> setter, const int property_flags) {
	RESOURCE_PROPERTY_ADD_IMPL(float, TYPE_FLOAT);
}
void Resource::add_property_string(const std::string &name, std::function<std::string(Resource *)> getter, std::function<void(Resource *, std::string)> setter, const int property_flags) {
	RESOURCE_PROPERTY_ADD_IMPL(String, TYPE_STRING);
}
void Resource::add_property_bool(const std::string &name, std::function<bool(Resource *)> getter, std::function<void(Resource *, bool)> setter, const int property_flags) {
	RESOURCE_PROPERTY_ADD_IMPL(bool, TYPE_BOOL);
}
void Resource::add_property_resource(const std::string &name, std::function<Ref<Resource>(Resource *)> getter, std::function<void(Resource *, Ref<Resource>)> setter, const int property_flags) {
	RESOURCE_PROPERTY_ADD_IMPL(Ref<Resource>, TYPE_RESOURCE);
}

void Resource::add_property_int_vector(const std::string &name, std::function<std::vector<int>(Resource *)> getter, std::function<void(Resource *, std::vector<int>)> setter, const int property_flags) {
	RESOURCE_PROPERTY_ADD_IMPL(IntVector, TYPE_VECTOR_INT);
}
void Resource::add_property_float_vector(const std::string &name, std::function<std::vector<float>(Resource *)> getter, std::function<void(Resource *, std::vector<float>)> setter, const int property_flags) {
	RESOURCE_PROPERTY_ADD_IMPL(FloatVector, TYPE_VECTOR_FLOAT);
}
void Resource::add_property_string_vector(const std::string &name, std::function<std::vector<std::string>(Resource *)> getter, std::function<void(Resource *, std::vector<std::string>)> setter, const int property_flags) {
	RESOURCE_PROPERTY_ADD_IMPL(StringVector, TYPE_VECTOR_STRING);
}
void Resource::add_property_bool_vector(const std::string &name, std::function<std::vector<bool>(Resource *)> getter, std::function<void(Resource *, std::vector<bool>)> setter, const int property_flags) {
	RESOURCE_PROPERTY_ADD_IMPL(BoolVector, TYPE_VECTOR_BOOL);
}
void Resource::add_property_resource_vector(const std::string &name, std::function<std::vector<Ref<Resource> >(Resource *)> getter, std::function<void(Resource *, std::vector<Ref<Resource> >)> setter, const int property_flags) {
	RESOURCE_PROPERTY_ADD_IMPL(ResourceVector, TYPE_VECTOR_RESOURCE);
}

void Resource::register_properties() {
	add_property_int("id", &Resource::get_id, &Resource::set_id);
}

Resource::Resource() :
		Reference() {
	_id = 0;
	_dirty = false;
	_resource_name = get_class();

	register_properties();
}

Resource::~Resource() {
	std::map<std::string, ResourcePropertyBase *>::iterator it;

	for (it = _property_map.begin(); it != _property_map.end(); it++) {
		ResourcePropertyBase *p = it->second;

		if (p) {
			delete p;
		}
	}
}