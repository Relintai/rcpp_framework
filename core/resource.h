#ifndef RESOURCE_H
#define RESOURCE_H

#include "core/string.h"

#include "reference.h"
#include "rapidjson/document.h"

#if DATABASES_ENABLED
class Database;
#endif

class Resource : public Reference {
	RCPP_OBJECT(Resource, Reference);

public:
	int id;

	virtual String to_json(rapidjson::Document *into = nullptr);
	virtual void from_json(const String &data);

	Resource();
	~Resource();

private:
};

#endif