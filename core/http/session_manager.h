#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "core/object.h"

#include <map>
#include <mutex>
#include <string>
#include <vector>

class HTTPSession;

class SessionManager : public Object {
public:
	void add_session(HTTPSession *session);
	void remove_session(HTTPSession *session);
	void delete_session(const std::string &session_id);
	HTTPSession *get_session(const std::string &session_id);
	HTTPSession *create_session();

	void clear();

	virtual std::string generate_session_id(const std::string &base = "");

	static SessionManager *get_singleton();

	SessionManager();
	~SessionManager();

	std::map<std::string, HTTPSession *> _sessions;
	std::vector<HTTPSession *> _sessions_vec;
	std::mutex _mutex;

protected:
	static SessionManager *_self;
};

#endif