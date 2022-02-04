#ifndef RESOURCE_H
#define RESOURCE_H

#include "core/containers/vector.h"
#include "core/string.h"

#include "reference.h"

#if DATABASES_ENABLED
class Database;
class QueryBuilder;
#endif

class Resource : public Reference {
	RCPP_OBJECT(Resource, Reference);

public:
	int id;
	bool dirty;

	Resource();
	~Resource();

private:
};

#endif