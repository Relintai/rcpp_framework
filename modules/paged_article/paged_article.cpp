#include "paged_article.h"

#include "core/query_builder.h"
#include "core/query_result.h"
#include "core/table_builder.h"
#include "core/utils.h"

#include <tinydir/tinydir.h>
#include <iostream>

#include "core/application.h"

void PagedArticle::index(Request *request) {
	const std::string r = request->get_current_path_segment();

	Article *s = pages[r];

	if (s == nullptr) {
		Application::get_instance()->send_error(404, request);
		return;
	}

	request->push_path();

	const std::string rp = request->get_current_path_segment();

	if (request->get_remaining_segment_count() > 1 && rp == "files") {
		std::string file_name = "/" + request->get_path_segment(request->get_current_segment_index() + 1);

		if (s->file_cache->wwwroot_has_file(file_name)) {
			std::string fp = s->file_cache->wwwroot + file_name;

			request->send_file(fp);
			return;
		}
	}

	if (rp == "") {
		//summary page
		request->body += s->summary_page;

		request->compile_and_send_body();
		request->pop_path();
		return;
	}

	const std::string *page = s->pages[rp];

	if (page == nullptr) {
		//bad url
		Application::get_instance()->send_error(404, request);
		return;
	}

	request->body += (*page);
	request->compile_and_send_body();
	request->pop_path();
}

void PagedArticle::load() {
	if (folder == "") {
		printf("Error: PagedArticle::load called, but a folder is not set!");
		return;
	}

	if (base_path.size() > 0 && base_path[base_path.size() - 1] == '/') {
		base_path.pop_back();
	}

	tinydir_dir dir;
	if (tinydir_open(&dir, folder.c_str()) == -1) {
		printf("Error opening PagedArticle::folder!\n");
		return;
	}

	while (dir.has_next) {
		tinydir_file file;
		if (tinydir_readfile(&dir, &file) == -1) {
			tinydir_next(&dir);
			continue;
		}

		if (file.is_dir) {
			if (file.name[0] == '.' || (file.name[0] == '.' && file.name[1] == '.')) {
				tinydir_next(&dir);
				continue;
			}

			std::string np = file.path;
			std::string fn = file.name;

			std::string ff = base_path + "/" + fn;

			Article *a = load_folder(np, ff);

			if (a) {

				std::string p = file.name;

				a->url = p;
				pages[p] = a;
			}

			a->file_cache->wwwroot = ("." + ff + "/files");
			a->file_cache->wwwroot_refresh_cache();
		}

		tinydir_next(&dir);
	}

	tinydir_close(&dir);

	generate_summaries();
}

Article *PagedArticle::load_folder(const std::string &folder, const std::string &path) {
	printf("PagedArticle: loading: %s\n", folder.c_str());

	std::vector<std::string> files;

	tinydir_dir dir;
	if (tinydir_open(&dir, folder.c_str()) == -1) {
		printf("PagedArticle::load_folder: Error opening folder %s!\n", folder.c_str());
		return nullptr;
	}

	while (dir.has_next) {
		tinydir_file file;
		if (tinydir_readfile(&dir, &file) == -1) {
			tinydir_next(&dir);
			continue;
		}

		if (!file.is_dir) {
			std::string np = file.name;

			files.push_back(np);
		}

		tinydir_next(&dir);
	}

	tinydir_close(&dir);

	if (files.size() == 0) {
		return nullptr;
	}

	std::sort(files.begin(), files.end());

	Article *article = new Article();

	for (uint32_t i = 0; i < files.size(); ++i) {
		std::string file_path = folder;

		if (file_path.size() > 0 && file_path[file_path.size() - 1] != '/')
			file_path += "/";

		file_path += files[i].c_str();

		FILE *f = fopen(file_path.c_str(), "r");

		if (!f) {
			printf("PagedArticle::load_folder: Error opening file! %s\n", file_path.c_str());
			continue;
		}

		fseek(f, 0, SEEK_END);
		long fsize = ftell(f);
		fseek(f, 0, SEEK_SET); /* same as rewind(f); */

		std::string fd;
		fd.resize(fsize);

		fread(&fd[0], 1, fsize, f);
		fclose(f);

		Utils::markdown_to_html(&fd);

		std::string pagination;

		pagination = Utils::get_pagination_links(path, files, i);

		std::string *finals = new std::string();

		(*finals) += pagination;
		(*finals) += fd;
		(*finals) += pagination;

		article->pages[files[i]] = finals;

		if (i == 0) {
			article->summary_page = (*finals);
		}
	}

	return article;
}

void PagedArticle::generate_summaries() {
	for (std::map<std::string, Article *>::iterator it = pages.begin(); it != pages.end(); ++it) {
		generate_summary((*it).second);
	}
}

void PagedArticle::generate_summary(Article *article) {
	if (article->summary_page != "") {
		return;
	}

	for (std::map<std::string, std::string *>::iterator it = article->pages.begin(); it != article->pages.end(); ++it) {
		std::string *s = (*it).second;

		if (s != nullptr) {
			article->summary_page = (*s);
		}
	}
}

PagedArticle::PagedArticle() :
		Object() {
}

PagedArticle::~PagedArticle() {
	pages.clear();
}