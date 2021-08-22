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

	void file_save();
	void file_load();
	void file_ensure_directory_exist();
	std::string file_get_base_path();
	static void file_load_all();
	static std::string file_get_path();
	static void file_set_path(const std::string &path);

	std::string to_json(rapidjson::Document *into = nullptr);
	void from_json(const std::string &data);

	User();
	~User();
};

#endif