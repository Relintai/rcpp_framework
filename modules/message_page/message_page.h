#ifndef MESSAGE_PAGE_H
#define MESSAGE_PAGE_H

#include <vector>
#include <string>

#include "core/object.h"

#include "core/request.h"


class MessagePage : public Object {
    RCPP_OBJECT(MessagePage, Object);

public:
    void index(Request *request);

    void migrate();

    MessagePage();
    ~MessagePage();

    std::vector<std::string> messages;
};

#endif