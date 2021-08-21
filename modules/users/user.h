#ifndef USER_H
#define USER_H

#include "core/resource.h"

#include <string>
#include <vector>
#include <mutex>

class Request;
class FormValidator;

class User : public Resource {
	RCPP_OBJECT(User, Resource);

public:
	std::string nameui;
	std::string emailui;
	int rank;
	std::string pre_salt;
	std::string post_salt;
	std::string password_hash;
	bool banned;
	std::vector<std::string> sessions;
	std::string password_reset_token;
	bool locked;

	virtual void save();
	virtual void load();
	virtual void load(const std::string &p_name);
	virtual void load(const int p_id);
	virtual void changed();
	virtual void update();

	virtual bool check_password(const std::string &p_password);
	virtual void create_password(const std::string &p_password);
	virtual std::string hash_password(const std::string &p_password);

	static void handle_request_default(Request *request);

	static void handle_login_request_default(Request *request);
	static void handle_register_request_default(Request *request);

	virtual void handle_request(Request *request);
	virtual void handle_main_page_request(Request *request);
	virtual void handle_settings_request(Request *request);
	virtual void handle_password_reset_request(Request *request);
	virtual void handle_logout_request(Request *request);
	virtual void handle_delete_request(Request *request);

	static void create_validators();

	void register_sessions();
	void unregister_sessions();

	User();
	~User();

	static FormValidator *_login_validator;
	static FormValidator *_registration_validator;
	static FormValidator *_profile_validator;

	std::mutex _mutex;
};

#endif