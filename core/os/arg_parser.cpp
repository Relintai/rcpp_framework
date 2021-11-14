#include "arg_parser.h"

#include "core/error_macros.h"

void ArgParser::setup(int argc, char **argv, char **envp) {
	parse_executable_name(argc, argv);
	parse_args(argc, argv);
	parse_env(envp);
}

void ArgParser::parse_executable_name(int argc, char **argv, int executable_index) {
	if (argc <= executable_index) {
		return;
	}

	executable_name = argv[executable_index];
}
void ArgParser::parse_args(int argc, char **argv, int executable_index) {
	for (int i = 0; i < argc; ++i) {
		if (i == argc) {
			continue;
		}

		arguments.push_back(String(argv[i]));
	}

	post_process_args();
}
void ArgParser::parse_env(char **envp) {
}

void ArgParser::post_process_args() {
	bool main_arg = false;
	String arg_command;
	String arg_main_command;

	for (int i = 0; i < arguments.size(); ++i) {
		arg_command = arguments[i];

		if (arg_command.size() > 0) {
			if (arg_command[0] == '-') {
				if (main_arg) {
					arguments_set.insert(arg_main_command);
				}

				main_arg = true;
				arg_main_command = arg_command;
			} else {
				if (main_arg) {
					main_arg = false;
					arguments_set.insert(arg_main_command);
					arguments_map[arg_main_command] = &arg_command;
				} else {
					arguments_set.insert(arg_command);
				}
			}
		}
	}

	if (main_arg) {
		arguments_set.insert(arg_main_command);
	}
}

bool ArgParser::has_arg(const String &arg) const {
	std::set<String>::iterator search = arguments_set.find(arg);

	return (search != arguments_set.end());
}

String *ArgParser::get_value(const String &arg) {
	return arguments_map[arg];
}

ArgParser::ArgParser() {
}
ArgParser::~ArgParser() {
}
