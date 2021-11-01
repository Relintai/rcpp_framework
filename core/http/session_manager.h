#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include "core/string.h"
#include "core/containers/vector.h"

#include "core/object.h"

#include <map>
#include <mutex>

class HTTPSession;
class Request;

class SessionManager : public Object {
public:
	void add_session(HTTPSession *session);
	void remove_session(HTTPSession *session);
	void delete_session(const String &session_id);
	void save_session(HTTPSession *session);
	HTTPSession *get_session(const String &session_id);
	HTTPSession *create_session();

	void load_sessions();

	void clear();

	virtual String generate_session_id(const String &base = "");

	static void session_setup_middleware(Object *instance, Request *request);

	virtual void migrate();
	virtual void create_table();
	virtual void drop_table();

	static SessionManager *get_singleton();

	SessionManager();
	~SessionManager();

	std::map<String, HTTPSession *> _sessions;
	Vector<HTTPSession *> _sessions_vec;
	std::mutex _mutex;

protected:
	static SessionManager *_self;

	static String _table_name;
	static String _data_table_name;
};

#endif