#ifndef RESOURCE_H
#define RESOURCE_H

#include "core/containers/vector.h"
#include "core/string.h"

#include "rapidjson/document.h"
#include "reference.h"

#if DATABASES_ENABLED
class Database;
class QueryBuilder;
#endif

//Based on Godot Engine's object method_bind initializers
#define RCPP_RESOURCE(m_class, m_inherits)                                           \
public:                                                                              \
	static ResourcePropertyDB properties;                                            \
                                                                                     \
	static void initialize_class(ResourcePropertyDB *pdb = nullptr) {                \
                                                                                     \
		if (pdb == nullptr) {                                                        \
			pdb = &m_class::properties;                                              \
		}                                                                            \
                                                                                     \
		m_inherits::initialize_class(pdb);                                           \
                                                                                     \
		if (m_class::_get_bind_properties() != m_inherits::_get_bind_properties()) { \
			_bind_properties(pdb);                                                   \
		}                                                                            \
	}                                                                                \
                                                                                     \
protected:                                                                           \
	inline static void (*_get_bind_properties())() {                                 \
		return &m_class::_bind_properties;                                           \
	}                                                                                \
                                                                                     \
private:

//add db_get etc templates to this

//#if DATABASES_ENABLED
//static Ref<RBACRank> db_get(const int id) {
//	return Resource::db_get<RBACRank>(id);
//}
//#endif

//etc

//ResourcePropertyDB * -> bind properties param

class Resource : public Reference {
	RCPP_OBJECT(Resource, Reference);

public:
	//uncomment!
	//static ResourcePropertyDB properties;

	int id;
	//since we will need getters and setters, this should be added
	bool dirty;

	virtual String to_json(rapidjson::Document *into = nullptr);
	virtual void from_json(const String &data);

	virtual String to_ini() { return ""; }
	virtual void from_ini(const String &data) {}

#if DATABASES_ENABLED
	virtual void db_load() {}
	//should use transactions if a resource has subresources, when qg is null it should start and end a transaction.
	virtual void db_save(QueryBuilder *qb = nullptr) {}

	//these could be a template, so just every subclass can return itself with these. Also these could use transactions
	template <class T>
	static Ref<T> db_get(const int id);
	static Ref<Resource> db_get_all();
	static void db_save_all(const Vector<Ref<Resource> > &resources);
#endif

	//add serialization to and from requests
	//could also build a form validator automatically using the property registration api

	//Resource::register<T>() foreach parents cast call
	//RBACRank::register()

	Resource();
	~Resource();

protected:
	inline static void (*_get_bind_properties())() {
		return &Resource::_bind_properties;
	}

	//add ResourcePropertyDB *pdb param
	//register id
	static void _bind_properties() {}

public: //should be protected, but bug in clang++
	//init like the macro
	static void initialize_class() {}

private:
};

//minden resource derivaltnak:
//PropertyDB db;
//minden benne van

//RBACRanks::PropertyDB -> minden benne van, osclass is
//RBACPermission::PropertyDB -> minden bene van osok is

template <class T>
Ref<T> Resource::db_get(const int id) {
	Ref<T> data;
	data.instance();

	return data;
}

#endif