#ifndef FILE_BASED_USER_H
#define FILE_BASED_USER_H

#include "user.h"

class FileBasedUser : public User {
public:
	static std::string get_path();
	static void set_path(const std::string &path);

	void save();
	void load(const std::string &p_name);
	void load();

	static void load_all();

	FileBasedUser();
	~FileBasedUser();

protected:
	std::string _file_path;

	static std::string _path;
};

#endif