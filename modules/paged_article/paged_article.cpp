#include "paged_article.h"

#include "core/database/query_builder.h"
#include "core/database/query_result.h"
#include "core/database/table_builder.h"
#include "core/utils.h"

#include <tinydir/tinydir.h>
#include <iostream>

void PagedArticle::handle_request_main(Request *request) {
	const String &rp = request->get_current_path_segment();

	if (request->get_remaining_segment_count() > 1 && rp == "files") {
		String file_name = "/" + request->get_path_segment(request->get_current_segment_index() + 1);

		if (file_cache->wwwroot_has_file(file_name)) {
			String fp = file_cache->wwwroot + file_name;

			request->send_file(fp);
			return;
		}
	}

	if (rp == "") {
		// summary page
		request->body += summary_page;

		request->compile_and_send_body();
		request->pop_path();
		return;
	}

	const String *page = pages[rp];

	if (page == nullptr) {
		// bad url
		request->send_error(404);
		return;
	}

	request->body += (*page);
	request->compile_and_send_body();
	request->pop_path();
}

void PagedArticle::load() {
	ERR_FAIL_COND_MSG(articles_folder == "", "Error: PagedArticle::load called, but a articles_folder is not set!");

	tinydir_dir dir;
	ERR_FAIL_COND_MSG(tinydir_open(&dir, articles_folder.c_str()) == -1, "Error opening PagedArticle::folder! folder: " + articles_folder);

	Vector<String> files;

	while (dir.has_next) {
		tinydir_file file;
		if (tinydir_readfile(&dir, &file) == -1) {
			tinydir_next(&dir);
			continue;
		}

		if (!file.is_dir) {
			String np = file.name;

			files.push_back(np);
		}

		tinydir_next(&dir);
	}

	tinydir_close(&dir);

	if (files.size() == 0) {
		return;
	}

	files.sort_inc();

	for (uint32_t i = 0; i < files.size(); ++i) {
		String file_path = articles_folder;

		if (file_path.size() > 0 && file_path[file_path.size() - 1] != '/') {
			file_path += "/";
		}

		file_path += files[i];

		FILE *f = fopen(file_path.c_str(), "r");

		ERR_CONTINUE_MSG(!f, "PagedArticle::load_folder: Error opening file! " + file_path);

		fseek(f, 0, SEEK_END);
		long fsize = ftell(f);
		fseek(f, 0, SEEK_SET); /* same as rewind(f); */

		String fd;
		fd.resize(fsize);

		fread(&fd[0], 1, fsize, f);
		fclose(f);

		Utils::markdown_to_html(&fd);

		String pagination;

		pagination = Utils::get_pagination_links(get_full_uri(), files, i);

		String *finals = new String();

		(*finals) += pagination;
		(*finals) += fd;
		(*finals) += pagination;

		pages[files[i]] = finals;

		if (i == 0) {
			summary_page = (*finals);
		}
	}

	file_cache->clear();

	if (serve_folder != "") {
		file_cache->wwwroot = serve_folder;
		file_cache->wwwroot_refresh_cache();
	}

	generate_summary();
}

void PagedArticle::generate_summary() {
	if (summary_page != "") {
		return;
	}

	for (std::map<String, String *>::iterator it = pages.begin(); it != pages.end(); ++it) {
		String *s = (*it).second;

		if (s != nullptr) {
			summary_page = (*s);
		}
	}
}

PagedArticle::PagedArticle() :
		WebNode() {

	file_cache = new FileCache();
}

PagedArticle::~PagedArticle() {
	for (std::map<String, String *>::iterator it = pages.begin(); it != pages.end(); ++it) {
		delete ((*it).second);
	}

	pages.clear();

	delete file_cache;
}