#ifndef USER_CONTROLLER_H
#define USER_CONTROLLER_H

#include "core/string.h"
#include "core/containers/vector.h"

#include "core/object.h"

#include "user.h"

class Request;
class FormValidator;

class UserController : public Object {
	RCPP_OBJECT(UserController, Object);
	
public:
	virtual void handle_request_default(Request *request);

	struct LoginRequestData {
		std::string error_str;
		std::string uname_val;
		std::string pass_val;
	};

	virtual void handle_login_request_default(Request *request);
	virtual void render_login_request_default(Request *request, LoginRequestData *data);

	struct RegisterRequestData {
		std::string error_str;
		std::string uname_val;
		std::string email_val;
		std::string pass_val;
		std::string pass_check_val;
	};

	virtual void handle_register_request_default(Request *request);
	virtual void render_register_request_default(Request *request, RegisterRequestData *data);
	virtual void render_register_success(Request *request);

	virtual void render_already_logged_in_error(Request *request);
	virtual void render_login_success(Request *request);

	virtual void handle_request(Ref<User> &user, Request *request);
	virtual void handle_main_page_request(Ref<User> &user, Request *request);

	struct SettingsRequestData {
		std::string error_str;

		std::string uname_val;
		std::string email_val;
		std::string pass_val;
		std::string pass_check_val;
	};

	virtual void handle_settings_request(Ref<User> &user, Request *request);
	virtual void render_settings_request(Ref<User> &user, Request *request, SettingsRequestData *data);

	virtual void handle_password_reset_request(Ref<User> &user, Request *request);
	virtual void handle_logout_request(Ref<User> &user, Request *request);
	virtual void handle_delete_request(Ref<User> &user, Request *request);

	virtual void create_validators();

	static void user_session_setup_middleware(Object *instance, Request *request);

	static UserController *get_singleton();

	UserController();
	~UserController();

protected:
	static UserController *_self;

	static FormValidator *_login_validator;
	static FormValidator *_registration_validator;
	static FormValidator *_profile_validator;
};

#endif