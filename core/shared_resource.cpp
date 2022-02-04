#include "shared_resource.h"

void SharedResource::read_lock() {
	_lock.read_lock();
}
void SharedResource::read_unlock() {
	_lock.read_unlock();
}
Error SharedResource::read_try_lock() {
	return _lock.read_try_lock();
}

void SharedResource::write_lock() {
	_lock.write_lock();
}
void SharedResource::write_unlock() {
	_lock.write_unlock();
}
Error SharedResource::write_try_lock() {
	return _lock.write_try_lock();
}

SharedResource::SharedResource() :
		Resource() {
}

SharedResource::~SharedResource() {
}