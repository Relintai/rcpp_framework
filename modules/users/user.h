#ifndef USER_H
#define USER_H

#include "core/resource.h"

#include <mutex>
#include <string>

class Request;
class FormValidator;

class User : public Resource {
	RCPP_OBJECT(User, Resource);

public:
	std::string name_user_input;
	std::string email_user_input;
	int rank;
	std::string pre_salt;
	std::string post_salt;
	std::string password_hash;
	bool banned;
	std::string password_reset_token;
	bool locked;

	std::string to_json(rapidjson::Document *into = nullptr);
	void from_json(const std::string &data);

	User();
	~User();
};

#endif