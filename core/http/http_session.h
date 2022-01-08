#ifndef HTTP_SESSION_H
#define HTTP_SESSION_H

#include "core/string.h"

#include "core/object.h"
#include "core/reference.h"
#include <map>
#include <mutex>

class HTTPSession : public Object {
	RCPP_OBJECT(HTTPSession, Object);

public:
	void add_object(const String &key, Object *obj);
	void remove_object(const String &key);
	Object *get_object(const String &key);

	void add_reference(const String &key, const Ref<Reference> &ref);
	void remove_reference(const String &key);
	Ref<Reference> get_reference(const String &key);

	void add_int(const String &key, const int val);
	void remove_int(const String &key);
	int get_int(const String &key);

	String session_id;
	int id;

	void clear();
	void reset();

	std::map<String, int> get_int_data();

	HTTPSession();
	~HTTPSession();

protected:
	std::mutex _mutex;

	//todo make something similar to godot's variant. (Or get godot's variant lol)
	std::map<String, Object *> _data;
	std::map<String, Ref<Reference> > _reference_data;
	std::map<String, int> _int_data;
};

#endif