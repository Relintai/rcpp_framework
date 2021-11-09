#ifndef PLATFORM_H
#define PLATFORM_H

#include "arg_parser.h"

class Platform {
public:
	virtual void arg_setup(int argc, char **argv, char **envp);

	static Platform *get_singleton();

	Platform();
	virtual ~Platform();

	ArgParser arg_parser;

protected:
	static Platform *_self;
};

#endif
