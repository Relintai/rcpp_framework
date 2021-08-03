#include "user.h"

void User::save() {

}

void User::load() {

}

void User::load(const std::string &p_name) {
	name = p_name;

	load();
}

void User::changed() {
	save();
}

void User::update() {

}

User::User() :
		Object() {

	id = 0;
	rank = 0;
	banned = false;
	locked = false;
}

User::~User() {
}
