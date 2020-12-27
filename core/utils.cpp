#include "utils.h"

#include <sstream>
#include <memory>
#include <maddy/parser.h>

void Utils::newline_to_br(std::string *str) {
	str_replace(str, "\r\n", "<br>");
	str_replace(str, "\n", "<br>");
}

void Utils::markdown_to_html(std::string *str) {
	std::shared_ptr<maddy::ParserConfig> config = std::make_shared<maddy::ParserConfig>();
	config->isEmphasizedParserEnabled = true; 
	config->isHTMLWrappedInParagraph = true;

	std::shared_ptr<maddy::Parser> parser = std::make_shared<maddy::Parser>(config);

	std::stringstream ss((*str));

	std::string htmlOutput = parser->Parse(ss);

	(*str) = htmlOutput;
}

void Utils::str_replace(std::string *str, const std::string &from, const std::string &to) {
	if (from.empty())
		return;

	size_t start_pos = 0;
	while ((start_pos = str->find(from, start_pos)) != std::string::npos) {
		str->replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
}
