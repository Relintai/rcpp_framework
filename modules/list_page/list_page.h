#ifndef LIST_PAGE_H
#define LIST_PAGE_H

#include <vector>
#include <string>

#include "core/object.h"

#include "core/request.h"


class ListPage : public Object {
    RCPP_OBJECT(ListPage, Object);

public:
    void index(Request *request);

    void load();
    
    ListPage();
    ~ListPage();

    std::vector<std::string> list_entries;
    std::string folder;
};

#endif