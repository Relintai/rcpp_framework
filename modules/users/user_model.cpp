#include "user_model.h"

#include "user.h"

UserModel *UserModel::get_singleton() {
	return _self;
}

UserModel::UserModel() :
		Object() {

	if (_self) {
		printf("UserModel::UserModel(): Error! self is not null!/n");
	}

	_self = this;
}

UserModel::~UserModel() {
	if (_self == this) {
		_self = nullptr;
	}
}

UserModel *UserModel::_self = nullptr;