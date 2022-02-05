#ifndef PAGED_ARTICLES_H
#define PAGED_ARTICLES_H

#include "core/containers/vector.h"
#include "core/string.h"

#include "core/file_cache.h"
#include "core/http/web_node.h"

#include "core/http/request.h"

//todo pagination

class PagedArticles : public WebNode {
	RCPP_OBJECT(PagedArticles, WebNode);

public:
	void _handle_request_main(Request *request);

	void load();
	void generate_summary();

	void _notification(const int what);

	PagedArticles();
	~PagedArticles();

	String folder;

protected:
	String summary_page;
};

#endif