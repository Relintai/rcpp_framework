#ifndef PAGED_LIST_H
#define PAGED_LIST_H

#include "core/string.h"

#include "core/http/web_node.h"

#include "modules/list_page/list_page.h"
#include "modules/paged_article/paged_article.h"

// Inherit from PagedArticles and override generate_index_page -> load and process md files in the set folder
// SHould probably be called something else. PagedArticlesMDIndex ?

class PagedList : public WebNode {
	RCPP_OBJECT(PagedList, WebNode);

public:
	void handle_request_main(Request *request);

	void load();

	PagedList();
	~PagedList();

	String folder;
	String base_path;

	ListPage *main_page;
	PagedArticle *articles;
};

#endif