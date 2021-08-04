
#include "cookie.h"

Cookie::Cookie(const std::string &p_key, const std::string &p_value) {
	http_only = true;
	secure = false;

	key = p_key;
	value = p_value;
}

Cookie::Cookie() {
	http_only = true;
	secure = false;
}

Cookie::~Cookie() {
}