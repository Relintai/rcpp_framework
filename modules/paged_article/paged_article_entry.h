#ifndef PAGED_ARTICLE_ENTRY_H
#define PAGED_ARTICLE_ENTRY_H

#include <map>
#include "core/containers/vector.h"
#include "core/string.h"

#include "core/file_cache.h"
#include "core/http/web_node.h"

#include "core/http/request.h"

#include "core/reference.h"

class PagedArticleEntry : public Reference {
	RCPP_OBJECT(PagedArticleEntry, Reference);
public:
	String url;
	String summary_page;
	std::map<String, String *> pages;
	FileCache *file_cache;

	PagedArticleEntry() {
		file_cache = new FileCache();
	}
	~PagedArticleEntry() {
		for (std::map<String, String *>::iterator it = pages.begin(); it != pages.end(); ++it) {
			delete ((*it).second);
		}

		pages.clear();
		
		delete file_cache;
	}
};

#endif