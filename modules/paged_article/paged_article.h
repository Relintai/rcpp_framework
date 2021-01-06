#ifndef PAGED_ARTICLE_H
#define PAGED_ARTICLE_H

#include <map>
#include <string>
#include <vector>

#include "core/file_cache.h"
#include "core/object.h"

#include "core/request.h"

struct Article {
	std::string url;
	std::string summary_page;
	std::map<std::string, std::string *> pages;
	FileCache *file_cache;

	Article() {
		file_cache = new FileCache();
	}
	~Article() {
		for (std::map<std::string, std::string *>::iterator it = pages.begin(); it != pages.end(); ++it) {
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
	Article *load_folder(const std::string &folder, const std::string &path);
	void generate_summaries();
	void generate_summary(Article *article);

	PagedArticle();
	~PagedArticle();

	std::map<std::string, Article *> pages;
	std::string folder;
	std::string base_path;
};

#endif