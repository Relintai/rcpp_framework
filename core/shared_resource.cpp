#include "shared_resource.h"

void SharedResource::lock() {
	_mutex.lock();
}
void SharedResource::unlock() {
	_mutex.unlock();
}

SharedResource::SharedResource() :
		Resource() {
}

SharedResource::~SharedResource() {
}