#ifndef LIST_PAGE_H
#define LIST_PAGE_H

#include "core/containers/vector.h"
#include "core/string.h"

#include "core/object.h"

#include "core/http/request.h"


class ListPage : public Object {
    RCPP_OBJECT(ListPage, Object);

public:
    void index(Request *request);

    void load();
    
    ListPage();
    ~ListPage();

    Vector<String> list_entries;
    String folder;
};

#endif