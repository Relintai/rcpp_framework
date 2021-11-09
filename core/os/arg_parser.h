#ifndef ARG_PARSER_H
#define ARG_PARSER_H

#include <map>
#include <set>

#include "core/string.h"

#include "core/containers/vector.h"

class ArgParser {
public:
	virtual void setup(int argc, char **argv, char **envp);

	virtual void parse_executable_name(int argc, char **argv, int executable_index = 0);
	virtual void parse_args(int argc, char **argv, int executable_index = 0);
	virtual void parse_env(char **envp);
	virtual void post_process_args();

	bool has_arg(const String &arg) const;
	String *get_value(const String &arg);

	ArgParser();
	virtual ~ArgParser();

	String executable_name;

	Vector<String> env_variables;
	std::map<String, String> env_variables_map;

	Vector<String> arguments;
	std::map<String, String*> arguments_map;
	std::set<String> arguments_set;
};

#endif
