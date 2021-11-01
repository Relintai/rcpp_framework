#ifndef PAGED_LIST_H
#define PAGED_LIST_H

#include "core/object.h"

#include "core/string.h"

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
	String folder;
	String base_path;
};

#endif