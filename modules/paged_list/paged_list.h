#ifndef PAGED_LIST_H
#define PAGED_LIST_H

#include "core/string.h"

#include "core/http/web_node.h"

#include "modules/list_page/list_page.h"
#include "modules/paged_article/paged_article.h"

class PagedList : public WebNode {
	RCPP_OBJECT(PagedList, WebNode);

public:
	void handle_request_main(Request *request);

	void load();

	PagedList();
	~PagedList();

	ListPage *main_page;
	PagedArticle *articles;
	String folder;
	String base_path;
};

#endif