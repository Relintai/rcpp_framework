#ifndef MESSAGE_PAGE_H
#define MESSAGE_PAGE_H

#include "core/containers/vector.h"
#include "core/string.h"

#include "core/object.h"

#include "core/http/request.h"


class MessagePage : public Object {
    RCPP_OBJECT(MessagePage, Object);

public:
    void index(Request *request);

    void migrate();

    MessagePage();
    ~MessagePage();

    Vector<String> messages;
};

#endif