#ifndef LIST_PAGE_H
#define LIST_PAGE_H

#include "core/containers/vector.h"
#include "core/string.h"

#include "core/http/web_node.h"

#include "core/http/request.h"


class ListPage : public WebNode {
    RCPP_OBJECT(ListPage, WebNode);

public:
    void handle_request_main(Request *request);

    void load();
    
    ListPage();
    ~ListPage();

    Vector<String> list_entries;
    String folder;
};

#endif