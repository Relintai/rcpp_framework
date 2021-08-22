#include "user_controller.h"

#include "user.h"

UserController *UserController::get_singleton() {
	return _self;
}

UserController::UserController() :
		Object() {

	if (_self) {
		printf("UserController::UserController(): Error! self is not null!/n");
	}

	_self = this;
}

UserController::~UserController() {
	if (_self == this) {
		_self = nullptr;
	}
}

UserController *UserController::_self = nullptr;