#include "settings.h"

#include <stdio.h>
#include <stdlib.h>

void Settings::parse_file(const std::string &path) {
	FILE *f = fopen(path.c_str(), "r");

	if (!f) {
		printf("Settings::parse_file: Error opening file!");
		return;
	}

	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET); /* same as rewind(f); */

	std::string config_str;
	config_str.resize(fsize);

	fread(&config_str[0], 1, fsize, f);
	fclose(f);

	settings.Parse(config_str.c_str());
}

Settings *Settings::get_singleton() {
	return _singleton;
}

Settings::Settings(const bool singleton) {
	if (singleton) {
		if (_singleton) {
			printf("Settings singleton overridden!\n");
		}

		_singleton = this;
	}
}

Settings::~Settings() {
	if (_singleton == this) {
		_singleton = nullptr;
	}
}

Settings *Settings::_singleton = nullptr;