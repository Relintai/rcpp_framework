#include "resource.h"

String Resource::to_json(rapidjson::Document *into) {
	return "";
}
void Resource::from_json(const String &data) {
}

Resource::Resource() :
		Reference() {
	id = 0;
}

Resource::~Resource() {
}