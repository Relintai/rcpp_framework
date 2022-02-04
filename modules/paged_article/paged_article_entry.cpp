#include "paged_article_entry.h"

PagedArticleEntry::PagedArticleEntry() {
	file_cache = new FileCache();
}

PagedArticleEntry::~PagedArticleEntry() {
	for (std::map<String, String *>::iterator it = pages.begin(); it != pages.end(); ++it) {
		delete ((*it).second);
	}

	pages.clear();

	delete file_cache;
}
