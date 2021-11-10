#include "rbac_user_initializer.h"

void UserInitializer::allocate_controller() {
	ERR_FAIL_COND(_controller);

	_controller = new RBACUserController();
}
void UserInitializer::free_controller() {
	if (_controller) {
		delete _controller;
		_controller = nullptr;
	}
}

void UserInitializer::allocate_model() {
	ERR_FAIL_COND(_model);

	_model = new RBACUserModel();
}
void UserInitializer::free_model() {
	if (_model) {
		delete _model;
		_model = nullptr;
	}
}

void UserInitializer::allocate_all() {
	allocate_model();
	allocate_controller();
}
void UserInitializer::free_all() {
	free_controller();
	free_model();
}

RBACUserController *UserInitializer::_controller = nullptr;
RBACUserModel *UserInitializer::_model = nullptr;