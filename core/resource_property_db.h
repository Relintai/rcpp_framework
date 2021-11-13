#ifndef RESOURCE_PROPERTY_DB_H
#define RESOURCE_PROPERTY_DB_H

#include "core/containers/vector.h"
#include "core/string.h"

struct ResourcePropertyBind {
	enum PropertyBindType {
		PROPERTY_TYPE_NONE = 0,
		PROPERTY_TYPE_INT = 1,
		PROPERTY_TYPE_REAL,
		PROPERTY_TYPE_STRING,
		PROPERTY_TYPE_RESOURCE,
		PROPERTY_TYPE_INT_VECTOR,
		PROPERTY_TYPE_REAL_VECTOR,
		PROPERTY_TYPE_STRING_VECTOR,
		PROPERTY_TYPE_RESOURCE_VECTOR,
	};

	PropertyBindType type;
	String name;

    //querybuilderhez?
    virtual String get_value_as_string() { return ""; }

    ResourcePropertyBind() {
        type = PROPERTY_TYPE_NONE;
    }
};

class ResourcePropertyDB {
public:

    //add_int()
    //add_string
    //etc
    //nem kell template
    //eleg ha resourcekent adja vissya
    //beassignolodik es jo lesz
    //automata serializacios rendsyer

    //get prop count
    //get prop name index


	ResourcePropertyDB();
	~ResourcePropertyDB();
};

//qb save

//insert into table_name (where to store?)
//store default table -> + add support for table name param

//foreach props
//get name
//endforeach

//values
//fporeach propes
//get as string?
//vagy prop->add value to qb(qb)

//if array v resource tyr -> skip

//endforeach

//foreach props -> if array add to transact

//foreach again
//if resource -> add to transact etc
//if res arr foreach add to transact

#endif