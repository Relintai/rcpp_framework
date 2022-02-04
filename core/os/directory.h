#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "core/string.h"
#include "core/error_list.h"
#include <tinydir/tinydir.h>

#include "core/reference.h"

class Directory : public Reference {
	RCPP_OBJECT(Directory, Reference);
public:
	Error open(const String &path, bool skip_specials = false);
	void close();

	bool has_next();
	void next();

	bool current_is_ok();
	String current_get_name();
	String current_get_path();
	String current_get_extension();
	char *current_get_name_cstr();
	char *current_get_path_cstr();
	char *current_get_extension_cstr();
	bool current_is_file();
	bool current_is_directory();

	String read_file(const String &path);
	void read_file_into(const String &path, String *str);

	bool is_open();
	bool is_closed();

	Directory();
	virtual ~Directory();

private:
	bool _skip_specials;
	int _read_file_result;
	tinydir_dir _dir;
	tinydir_file _file;

	bool _open;
};

#endif
