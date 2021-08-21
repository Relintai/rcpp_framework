#ifndef RESOURCE_H
#define RESOURCE_H

#include "reference.h"

class Resource : public Reference {
	RCPP_OBJECT(Resource, Reference);

public:

	Resource();
	~Resource();

private:
};


#endif