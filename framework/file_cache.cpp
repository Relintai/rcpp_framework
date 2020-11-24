#include "file_cache.h"

#include <tinydir/tinydir.h>

#include <iostream>

void FileCache::register_file(const std::string &file_path) {
	registered_files.insert(file_path);
}

void FileCache::deregister_file(const std::string &file_path) {
	registered_files.erase(file_path);
}

bool FileCache::has_file(const std::string &file_path) {
    return registered_files.find(file_path) != registered_files.end();
}

void FileCache::refresh() {
	registered_files.clear();

	evaluate_dir(wwwroot.c_str());
}

void FileCache::evaluate_dir(const char *path) {
	tinydir_dir dir;
	if (tinydir_open(&dir, path) == -1) {
		printf("Error opening wwwroot!\n");
		return;
	}

	while (dir.has_next) {
		tinydir_file file;
		if (tinydir_readfile(&dir, &file) == -1) {
			tinydir_next(&dir);
			continue;
		}

		if (!file.is_dir) {
			std::string np = file.path;
			np = np.substr(wwwroot.size(), np.size() - wwwroot.size());

			registered_files.insert(np);
		} else {
			if (file.name[0] == '.' && file.name[1] == '\0' || file.name[0] == '.' && file.name[1] == '.') {
				tinydir_next(&dir);
				continue;
			}

			evaluate_dir(file.path);
		}

		tinydir_next(&dir);
	}

	tinydir_close(&dir);
}

FileCache::FileCache() {
	_instance = this;
}

FileCache::~FileCache() {
	registered_files.clear();

	_instance = nullptr;
}

FileCache *FileCache::get_instance() {
	return _instance;
}

FileCache *FileCache::_instance = nullptr;
