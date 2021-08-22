#ifndef HTTP_SESSION_H
#define HTTP_SESSION_H

#include "core/object.h"
#include "core/reference.h"
#include <map>
#include <mutex>
#include <string>

class HTTPSession : public Object {
public:
	void add_object(const std::string &key, Object *obj);
	void remove_object(const std::string &key);
	Object *get_object(const std::string &key);

	void add_reference(const std::string &key, const Ref<Reference> &ref);
	void remove_reference(const std::string &key);
	Ref<Reference> get_reference(const std::string &key);

	void add_int(const std::string &key, const int val);
	void remove_int(const std::string &key);
	int get_int(const std::string &key);

	std::string session_id;
	int id;

	void clear();
	void reset();

	std::map<std::string, int> get_int_data();

	HTTPSession();
	~HTTPSession();

protected:
	std::mutex _mutex;

	//todo make something similar to godot's variant. (Or get godot's variant lol)
	std::map<std::string, Object *> _data;
	std::map<std::string, Ref<Reference> > _reference_data;
	std::map<std::string, int> _int_data;
};

#endif