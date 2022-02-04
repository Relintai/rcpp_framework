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

	virtual void render_entries(const Vector<String> &list_entries);
    virtual String render_page(const int page_index, const int page_count, const Vector<String> &list_entries, const int efrom, const int eto);
	virtual String render_entry(const String &list_entry);
	virtual void render_no_entries_response();

    void _notification(const int what);

	ListPage();
	~ListPage();

	bool paginate;
    int max_visible_navigation_links;
	int entry_per_page;
	String folder;

protected:
	Vector<String> _pages;
	String _no_entries_response;
};

#endif