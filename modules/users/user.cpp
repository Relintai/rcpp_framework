#include "user.h"

#include "core/http/http_session.h"
#include "core/http/session_manager.h"

void User::save() {
}

void User::load() {
	//unregister_sessions()
	//load_code
	//register_sessions()
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

void User::register_sessions() {
	if (sessions.size() == 0) {
		return;
	}

	SessionManager *sm = SessionManager::get_singleton();

	if (!sm) {
		printf("ERROR: UserManager::add_user SessionManager singleton is null, please allocate one!\n");
		return;
	}

	for (int i = 0; i < sessions.size(); ++i) {
		HTTPSession *session = new HTTPSession();
		session->session_id = sessions[i];
		session->add_object("user", this);

		sm->add_session(session);
	}
}

void User::unregister_sessions() {
	if (sessions.size() == 0) {
		return;
	}

	SessionManager *sm = SessionManager::get_singleton();

	if (!sm) {
		printf("ERROR: UserManager::remove_user SessionManager singleton is null, please allocate one!\n");
		return;
	}

	for (int i = 0; i < sessions.size(); ++i) {
		sm->delete_session(sessions[i]);
	}
}

User::User() :
		Object() {

	id = 0;
	rank = 0;
	banned = false;
	locked = false;
}

User::~User() {
	unregister_sessions();
}
