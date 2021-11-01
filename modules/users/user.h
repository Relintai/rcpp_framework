#ifndef USER_H
#define USER_H

#include "core/string.h"

#include "core/resource.h"

#include <mutex>

class Request;
class FormValidator;

class User : public Resource {
	RCPP_OBJECT(User, Resource);

public:
	String name_user_input;
	String email_user_input;
	int rank;
	String pre_salt;
	String post_salt;
	String password_hash;
	bool banned;
	String password_reset_token;
	bool locked;

	String to_json(rapidjson::Document *into = nullptr);
	void from_json(const String &data);

	User();
	~User();
};

#endif