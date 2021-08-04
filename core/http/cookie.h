#ifndef COOKIE_H
#define COOKIE_H

#include <string>

class Cookie {
public:
	//todo date
	std::string domain;
	std::string path;
	std::string key;
	std::string value;
	bool http_only;
	bool secure;

	Cookie();
	Cookie(const std::string &p_key, const std::string &p_value);
	~Cookie();
};

#endif