#ifndef USER_H
#define USER_H

#include "core/resource.h"

#include <mutex>
#include <string>
#include <vector>

class Request;
class FormValidator;

class User : public Resource {
	RCPP_OBJECT(User, Resource);

public:
	std::string get_name_ui();
	void set_name_ui(const std::string &value);

	std::string get_email_ui();
	void set_email_ui(const std::string &value);

	int get_rank();
	void set_rank(const int value);

	std::string get_pre_salt();
	void set_pre_salt(const std::string &value);

	std::string get_post_salt();
	void set_post_salt(const std::string &value);

	std::string get_password_hash();
	void set_password_hash(const std::string &value);

	bool get_banned();
	void set_banned(const bool value);

	std::vector<std::string> get_sessions();
	void set_sessions(const std::vector<std::string> &value);

	std::string get_password_reset_token();
	void set_password_reset_token(const std::string &value);

	bool get_locked();
	void set_locked(const bool value);

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

	void register_properties();

	User();
	~User();

protected:
	std::string _nameui;
	std::string _emailui;
	int _rank;
	std::string _pre_salt;
	std::string _post_salt;
	std::string _password_hash;
	bool _banned;
	std::vector<std::string> _sessions;
	std::string _password_reset_token;
	bool _locked;

	static FormValidator *_login_validator;
	static FormValidator *_registration_validator;
	static FormValidator *_profile_validator;

	std::mutex _mutex;
};

#endif