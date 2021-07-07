#ifndef USER_H
#define USER_H

#include "core/object.h"

#include <string>

class User : public Object {
public:
	int id;
	std::string name;
	std::string email;
	int rank;
	std::string pre_salt;
	std::string post_salt;
	std::string password_hash;

	User();
	~User();
};

#endif