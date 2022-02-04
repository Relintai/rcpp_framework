#include "paged_list.h"

#include "core/http/request.h"

#include <iostream>

#include "core/file_cache.h"

#include "core/database/database_manager.h"

#include "core/html/html_builder.h"

void PagedList::handle_request_main(Request *request) {
	const String path = request->get_current_path_segment();

	if (request->get_remaining_segment_count() == 0) {
		main_page->handle_request_main(request);
		return;
	}

	articles->handle_request_main(request);
}

void PagedList::load() {
	main_page->folder = folder;
	main_page->load();

	articles->articles_folder = folder;
	articles->serve_folder = folder + "/files";
	articles->load();
}

PagedList::PagedList() :
		WebNode() {

	main_page = new ListPage();
	articles = new PagedArticle();
}

PagedList::~PagedList() {
	delete main_page;
	delete articles;
}
