#ifndef MESSAGE_PAGE_H
#define MESSAGE_PAGE_H

#include "core/containers/vector.h"
#include "core/string.h"

#include "core/http/web_node.h"

#include "core/http/request.h"


class MessagePage : public WebNode {
    RCPP_OBJECT(MessagePage, WebNode);

public:
    void handle_request_main(Request *request);

    void migrate();

    MessagePage();
    ~MessagePage();

    Vector<String> messages;
};

#endif