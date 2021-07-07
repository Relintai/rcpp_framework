#ifndef PAGED_LIST_H
#define PAGED_LIST_H

#include "core/object.h"

#include <string>

#include "modules/list_page/list_page.h"
#include "modules/paged_article/paged_article.h"

class PagedList : public Object {
public:
	void index(Request *request);

	void load();

	PagedList();
	~PagedList();

	ListPage *main_page;
	PagedArticle *articles;
	std::string folder;
	std::string base_path;
};

#endif