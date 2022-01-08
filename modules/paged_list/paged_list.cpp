#include "paged_list.h"

#include "core/http/request.h"

#include <iostream>

#include "core/file_cache.h"

#include "core/database/database_manager.h"

#include "core/html/html_builder.h"

void PagedList::index(Request *request) {
	const String path = request->get_current_path_segment();

	if (request->get_remaining_segment_count() == 0) {
		main_page->handle_request_main(request);
		return;
	}

	articles->index(request);
}

void PagedList::load() {
	main_page->folder = folder;
	main_page->load();

	articles->base_path = base_path;
	articles->folder = folder;
	articles->load();
}

PagedList::PagedList() :
		Object() {

	main_page = new ListPage();
	articles = new PagedArticle();
}

PagedList::~PagedList() {
	delete main_page;
	delete articles;
}
