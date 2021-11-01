#ifndef PAGED_ARTICLE_H
#define PAGED_ARTICLE_H

#include <map>
#include "core/containers/vector.h"
#include "core/string.h"

#include "core/file_cache.h"
#include "core/object.h"

#include "core/http/request.h"

struct Article {
	String url;
	String summary_page;
	std::map<String, String *> pages;
	FileCache *file_cache;

	Article() {
		file_cache = new FileCache();
	}
	~Article() {
		for (std::map<String, String *>::iterator it = pages.begin(); it != pages.end(); ++it) {
			delete ((*it).second);
		}

		pages.clear();
		
		delete file_cache;
	}
};

class PagedArticle : public Object {
	RCPP_OBJECT(PagedArticle, Object);

public:
	void index(Request *request);

	void load();
	Article *load_folder(const String &folder, const String &path);
	void generate_summaries();
	void generate_summary(Article *article);

	PagedArticle();
	~PagedArticle();

	std::map<String, Article *> pages;
	String folder;
	String base_path;
};

#endif