#ifndef PLATFORM_H
#define PLATFORM_H

#include "arg_parser.h"

class Platform {
public:
	virtual void arg_setup(int argc, char **argv, char **envp);

	virtual String get_executable_path();

	static Platform *get_singleton();

	Platform();
	virtual ~Platform();

	ArgParser arg_parser;

protected:
	static Platform *_self;
};

#endif
