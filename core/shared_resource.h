#ifndef SHARED_RESOURCE_H
#define SHARED_RESOURCE_H

#include "core/string.h"

#include "resource.h"
#include "core/threading/rw_lock.h"

class SharedResource : public Resource {
	RCPP_OBJECT(SharedResource, Resource);

public:
	void read_lock();
	void read_unlock();
	Error read_try_lock();

	void write_lock();
	void write_unlock();
	Error write_try_lock();

	SharedResource();
	~SharedResource();

private:
	RWLock _lock;
};

#endif