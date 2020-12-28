#include "list_page.h"

#include "core/query_builder.h"
#include "core/query_result.h"
#include "core/table_builder.h"
#include "core/utils.h"

#include <tinydir/tinydir.h>
#include <iostream>

void ListPage::index(Request *request) {
	std::string r = "";

	for (uint32_t i = 0; i < list_entries.size(); ++i) {
		r += "<div class=\"list_entry\">" + list_entries[i] + "</div>";
	}

	request->body += r;

	request->compile_and_send_body();
}

void ListPage::load() {
	if (folder == "") {
		printf("Error: ListPage::load called, but a folder is not set!");
		return;
	}

	std::vector<std::string> files;

	tinydir_dir dir;
	if (tinydir_open(&dir, folder.c_str()) == -1) {
		printf("Error opening ListPage::folder!\n");
		return;
	}

	while (dir.has_next) {
		tinydir_file file;
		if (tinydir_readfile(&dir, &file) == -1) {
			tinydir_next(&dir);
			continue;
		}

		if (!file.is_dir) {
			std::string np = file.path;

			files.push_back(np);
		}

		tinydir_next(&dir);
	}

	tinydir_close(&dir);

	std::sort(files.begin(), files.end());

	for (uint32_t i = 0; i < files.size(); ++i) {
		FILE *f = fopen(files[i].c_str(), "r");

		if (!f) {
			printf("Settings::parse_file: Error opening file!\n");
			return;
		}

		fseek(f, 0, SEEK_END);
		long fsize = ftell(f);
		fseek(f, 0, SEEK_SET); /* same as rewind(f); */

		std::string fd;
		fd.resize(fsize);

		fread(&fd[0], 1, fsize, f);
		fclose(f);

		Utils::markdown_to_html(&fd);

		list_entries.push_back(fd);
	}
}

ListPage::ListPage() :
		Object() {
}

ListPage::~ListPage() {
}