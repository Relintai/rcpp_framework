#ifndef SHARED_RESOURCE_H
#define SHARED_RESOURCE_H

#include "core/string.h"

#include "resource.h"
#include "core/threading/mutex.h"

class SharedResource : public Resource {
	RCPP_OBJECT(SharedResource, Resource);

public:
	void lock();
	void unlock();

	SharedResource();
	~SharedResource();

private:
	//todo add read write lock instead of this
	Mutex _mutex;
};

#endif