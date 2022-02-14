#ifndef PAGED_ARTICLE_H
#define PAGED_ARTICLE_H

#include "core/containers/vector.h"
#include "core/string.h"
#include <map>

#include "web/file_cache.h"
#include "web/http/web_node.h"

#include "web/http/request.h"

class PagedArticle : public WebNode {
	RCPP_OBJECT(PagedArticle, WebNode);

public:
	void handle_request_main(Request *request);

	void render(Request *request);
	void render_preview(Request *request);

	void load();
	void load_folder(const String &folder, const String &path);
	String get_index_page();
	String get_summary();

	virtual void generate_summary();

	void _notification(const int what);

	PagedArticle();
	~PagedArticle();

	String articles_folder;
	bool serve_folder_relative;
	String serve_folder;

protected:
	String index_page;
	String summary;
	std::map<String, String *> pages;
	FileCache *file_cache;
};

#endif