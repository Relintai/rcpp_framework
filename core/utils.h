#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

class Utils {
public:
	static void newline_to_br(std::string *str);
	//htmlspecialchars

	static void markdown_to_html(std::string *str);

	static std::string get_pagination(const std::string base_url, const uint32_t max, const uint32_t current_index, const uint32_t max_visible_links = 10);
	static std::string get_pagination_links(const std::string base_url, const std::vector<std::string> &links, const uint32_t current_index, const uint32_t max_visible_links = 10);

	static void str_replace(std::string *str, const std::string &from, const std::string &to);

protected:
};

#endif