#include "directory.h"

Error Directory::open(const String &path, bool skip_specials) {
	if (_open) {
		return ERR_CANT_ACQUIRE_RESOURCE;
	}

	_skip_specials = skip_specials;

	if (tinydir_open(&_dir, path.c_str()) == -1) {
		return FAILED;
	}

	_open = true;

	return OK;
}

void Directory::close() {
	if (!_open) {
		return;
	}

	tinydir_close(&_dir);

	_open = false;
}

bool Directory::has_next() {
	return _dir.has_next;
}
void Directory::next() {
	tinydir_next(&_dir);

	_read_file_result = tinydir_readfile(&_dir, &_file);

	if (_skip_specials && current_is_ok()) {
		if ((_file.name[0] == '.' && _file.name[1] == '\0') || (_file.name[0] == '.' && _file.name[1] == '.' && _file.name[1] == '\0')) {
			next();
		}
	}
}

bool Directory::current_is_ok() {
	return _read_file_result == 01;
}
String Directory::current_get_name() {
	return _file.name;
}
String Directory::current_get_path() {
	return _file.path;
}
String Directory::current_get_extension() {
	return _file.extension;
}
char *Directory::current_get_name_cstr() {
	return _file.name;
}
char *Directory::current_get_path_cstr() {
	return _file.path;
}
char *Directory::current_get_extension_cstr() {
	return _file.extension;
}
bool Directory::current_is_file() {
	return !_file.is_dir;
}
bool Directory::current_is_directory() {
	return _file.is_dir;
}

String Directory::read_file(const String &path) {
	FILE *f = fopen(path.c_str(), "r");

	String fd;

	ERR_FAIL_COND_V_MSG(!f, fd, "Error opening file! " + path);

	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET); /* same as rewind(f); */

	fread(fd.dataw(), 1, fsize, f);
	fclose(f);

	return fd;
}
void Directory::read_file_into(const String &path, String *str) {
	ERR_FAIL_COND(!str);

	FILE *f = fopen(path.c_str(), "r");

	ERR_FAIL_COND_MSG(!f, "Error opening file! " + path);

	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET); /* same as rewind(f); */

	str->resize(fsize);

	fread(str->dataw(), 1, fsize, f);
	fclose(f);
}

bool Directory::is_open() {
	return _open;
}
bool Directory::is_closed() {
	return !_open;
}

Directory::Directory() {
	_skip_specials = false;
	_read_file_result = 0;
	_open = false;
}
Directory::~Directory() {
	if (is_open()) {
		close();
	}
}
