#include "resource.h"

std::string Resource::to_json(rapidjson::Document *into) {
	return "";
}
void Resource::from_json(const std::string &data) {
}

Resource::Resource() :
		Reference() {
	id = 0;
}

Resource::~Resource() {
}