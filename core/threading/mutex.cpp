#include "mutex.h"

void Mutex::lock() {
	_mutex.lock();
}
void Mutex::unlock() {
	_mutex.unlock();
}

Mutex::Mutex() {

}
Mutex::~Mutex() {

}