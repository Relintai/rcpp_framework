#ifndef RESOURCE_H
#define RESOURCE_H

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

	virtual std::string to_json(rapidjson::Document *into = nullptr);
	virtual void from_json(const std::string &data);

	Resource();
	~Resource();

private:
};

#endif