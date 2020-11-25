#include "file_cache.h"

#include <tinydir/tinydir.h>

#include <iostream>

void FileCache::wwwroot_register_file(const std::string &file_path) {
	registered_files.insert(file_path);
}

void FileCache::wwwroot_deregister_file(const std::string &file_path) {
	registered_files.erase(file_path);
}

bool FileCache::wwwroot_has_file(const std::string &file_path) {
	return registered_files.find(file_path) != registered_files.end();
}

void FileCache::wwwroot_refresh_cache() {
	registered_files.clear();

	wwwroot_evaluate_dir(wwwroot.c_str());
}

void FileCache::wwwroot_evaluate_dir(const char *path) {
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

			wwwroot_evaluate_dir(file.path);
		}

		tinydir_next(&dir);
	}

	tinydir_close(&dir);
}

bool FileCache::get_cached_body(const std::string &path, std::string *body) {
	//TODO ERROR MACRO body == null

	//this shouldn't need mutexes

	CacheEntry *e = cache_map[path];

	if (!e) {
		return false;
	}

	int64_t current_timestamp = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	int64_t diff = current_timestamp - e->timestamp;

	if (diff > cache_invalidation_time) {
		return false;
	}

	body->append(e->body);

	return true;
}

void FileCache::set_cached_body(const std::string &path, const std::string &body) {
	cache_mutex.lock();

	CacheEntry *e = cache_map[path];

	if (!e) {
		e = new CacheEntry();
		cache_map[path] = e;
	}

	int64_t current_timestamp = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	e->timestamp = current_timestamp;
	e->body = body;

	cache_mutex.unlock();
}

FileCache::FileCache(bool singleton) {
	if (singleton) {
		if (_instance) {
			printf("FileCache: Filecache instance is set as singleton, but an another FileCache instance is already set up as singleton! Ignoring setting!\n");
		} else {
			_instance = this;
		}
	}

	cache_invalidation_time = 1;
}

FileCache::~FileCache() {
	registered_files.clear();

	if (_instance == this)
		_instance = nullptr;
}

FileCache *FileCache::get_singleton() {
	return _instance;
}

FileCache *FileCache::_instance = nullptr;
