#ifndef USER_CONTROLLER_H
#define USER_CONTROLLER_H

#include "core/object.h"

#include <string>
#include "user.h"

class Request;
class FormValidator;

class UserController : public Object {
public:
	virtual void handle_request_default(Request *request);

	virtual void handle_login_request_default(Request *request);
	virtual void handle_register_request_default(Request *request);

	virtual void handle_request(Ref<User> &user, Request *request);
	virtual void handle_main_page_request(Ref<User> &user, Request *request);
	virtual void handle_settings_request(Ref<User> &user, Request *request);
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