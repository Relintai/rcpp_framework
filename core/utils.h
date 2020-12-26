#ifndef UTILS_H
#define UTILS_H

#include <string>

#include <bbcpp/BBDocument.h>

class Utils {
    public:
    static void newline_to_br(std::string *str);
    //htmlspecialchars

    static void bbcode_evaluate_simple(std::string *str);
    static bbcpp::BBDocumentPtr bbcode(const std::string &str);

    static void str_replace(std::string *str, const std::string& from, const std::string& to);
protected:
    static void eval_node(std::string *str, bbcpp::BBNodePtr node);
    static void eval_element(std::string *str, bbcpp::BBElementPtr node);
};


#endif