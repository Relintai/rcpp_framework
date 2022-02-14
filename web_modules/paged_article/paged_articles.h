#ifndef PAGED_ARTICLES_H
#define PAGED_ARTICLES_H

#include "core/containers/vector.h"
#include "core/string.h"

#include "web/file_cache.h"
#include "web/http/web_node.h"

#include "web/http/request.h"

// todo pagination

class PagedArticles : public WebNode {
	RCPP_OBJECT(PagedArticles, WebNode);

public:
	void _handle_request_main(Request *request);

	void render_index(Request *request);
	void render_preview(Request *request);

	void load();
	void generate_index_page();

	void _notification(const int what);

	PagedArticles();
	~PagedArticles();

	String folder;

protected:
	String index_page;
};

#endif