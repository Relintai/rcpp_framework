#ifndef PAGED_ARTICLE_H
#define PAGED_ARTICLE_H

#include <map>
#include "core/containers/vector.h"
#include "core/string.h"

#include "core/file_cache.h"
#include "core/http/web_node.h"

#include "core/http/request.h"
#include "paged_article_entry.h"

class PagedArticle : public WebNode {
	RCPP_OBJECT(PagedArticle, WebNode);

public:
	void handle_request_main(Request *request);

	void load();
	void load_folder(const String &folder, const String &path);
	void generate_summary();

	PagedArticle();
	~PagedArticle();

	String articles_folder;
	String serve_folder;

protected:
	String summary_page;
	std::map<String, String *> pages;
	FileCache *file_cache;
};

#endif