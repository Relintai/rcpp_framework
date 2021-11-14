#ifndef USER_MODEL_H
#define USER_MODEL_H

#include "core/string.h"
#include "core/containers/vector.h"

#include "core/object.h"

#include "user.h"

class UserModel : public Object {
	RCPP_OBJECT(UserModel, Object);
	
public:
	virtual Ref<User> get_user(const int id);
	virtual Ref<User> get_user(const String &user_name_input);
	virtual void save_user(Ref<User> &user);

	virtual Vector<Ref<User> > get_all();

	virtual Ref<User> create_user(); 

	bool is_username_taken(const String &user_name_input);
	bool is_email_taken(const String &email_input);

	virtual bool check_password(const Ref<User> &user, const String &p_password);
	virtual void create_password(Ref<User> &user, const String &p_password);
	virtual String hash_password(const Ref<User> &user, const String &p_password);

	virtual void create_table();
	virtual void drop_table();
	virtual void migrate();
	virtual void create_test_users();

	static UserModel *get_singleton();

	UserModel();
	~UserModel();

protected:
	static UserModel *_self;

	String _file_path;

	static String _path;
	static String _table_name;
};

#endif