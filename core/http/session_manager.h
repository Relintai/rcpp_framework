#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include "core/object.h"

#include <map>
#include <mutex>
#include <string>
#include <vector>

class HTTPSession;
class Request;

class SessionManager : public Object {
public:
	void add_session(HTTPSession *session);
	void remove_session(HTTPSession *session);
	void delete_session(const std::string &session_id);
	void save_session(HTTPSession *session);
	HTTPSession *get_session(const std::string &session_id);
	HTTPSession *create_session();

	void load_sessions();

	void clear();

	virtual std::string generate_session_id(const std::string &base = "");

	static void session_setup_middleware(Object *instance, Request *request);

	virtual void migrate();
	virtual void create_table();
	virtual void drop_table();

	static SessionManager *get_singleton();

	SessionManager();
	~SessionManager();

	std::map<std::string, HTTPSession *> _sessions;
	std::vector<HTTPSession *> _sessions_vec;
	std::mutex _mutex;

protected:
	static SessionManager *_self;

	static std::string _table_name;
	static std::string _data_table_name;
};

#endif