#ifndef UTILS_H
#define UTILS_H

#include <string>

class Utils {
    public:
    static void newline_to_br(std::string *str);
    //htmlspecialchars

    static void markdown_to_html(std::string *str);

    static void str_replace(std::string *str, const std::string& from, const std::string& to);
protected:
};


#endif