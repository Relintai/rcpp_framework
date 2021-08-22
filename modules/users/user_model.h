#ifndef USER_MODEL_H
#define USER_MODEL_H

#include "core/object.h"

#include "user.h"
#include <string>
#include <vector>

class UserModel : public Object {
public:
	Ref<User> get_user(const int id);
	Ref<User> get_user(const std::string &user_name_input);
	void save_user(Ref<User> &user);

	std::vector<Ref<User> > get_all();

	bool is_username_taken(const std::string &user_name_input);
	bool is_email_taken(const std::string &email_input);

	virtual bool check_password(const Ref<User> &user, const std::string &p_password);
	virtual void create_password(Ref<User> &user, const std::string &p_password);
	virtual std::string hash_password(const Ref<User> &user, const std::string &p_password);

	virtual void create_table();
	virtual void drop_table();
	virtual void migrate();

	static UserModel *get_singleton();

	UserModel();
	~UserModel();

protected:
	static UserModel *_self;

	std::string _file_path;

	static std::string _path;
	static std::string _table_name;
};

#endif