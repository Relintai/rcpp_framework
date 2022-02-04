#include "resource.h"

Resource::Resource() :
		Reference() {
	id = 0;
	dirty = false;
}

Resource::~Resource() {
}