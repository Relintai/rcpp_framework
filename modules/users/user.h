#ifndef USER_H
#define USER_H

#include "core/object.h"

#include <string>
#include <vector>

class Request;

class User : public Object {
	RCPP_OBJECT(User, Object);

public:
	int id;
	std::string name;
	std::string email;
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
	virtual void changed();
	virtual void update();

	static void handle_request_default(Request *request);

	static void handle_login_request_default(Request *request);
	static void handle_register_request_default(Request *request);

	virtual void handle_request(Request *request);
	virtual void handle_main_page_request(Request *request);
	virtual void handle_settings_request(Request *request);
	virtual void handle_password_reset_request(Request *request);
	virtual void handle_logout_request(Request *request);
	virtual void handle_delete_request(Request *request);

	void register_sessions();
	void unregister_sessions();

	User();
	~User();
};

#endif