#include "list_page.h"

#include "core/html/html_builder.h"
#include "core/utils.h"
#include "core/math/math.h"

#include <tinydir/tinydir.h>
#include <iostream>

void ListPage::handle_request_main(Request *request) {
	if (_pages.size() == 0) {
		render_menu(request);
		request->body += _no_entries_response;
		request->compile_and_send_body();
		return;
	}

	const String &cs = request->get_current_path_segment();

	if (cs == "") {
		render_menu(request);
		request->body += _pages[0];
		request->compile_and_send_body();
		return;
	}

	if (!cs.is_uint()) {
		request->send_error(HTTP_STATUS_CODE_404_NOT_FOUND);
		return;
	}

	int p = cs.to_int();

	p = ((p == 0) ? (0) : (p - 1));

	if (p < 0 || p >= _pages.size()) {
		request->send_error(HTTP_STATUS_CODE_404_NOT_FOUND);
		return;
	}

	render_menu(request);
	request->body += _pages[p];
	request->compile_and_send_body();
}

void ListPage::load() {
	if (folder == "") {
		RLOG_ERR("Error: ListPage::load called, but a folder is not set!");
		return;
	}

	Vector<String> files;

	tinydir_dir dir;
	if (tinydir_open(&dir, folder.c_str()) == -1) {
		RLOG_ERR("Error opening ListPage::folder! folder: \n" + folder);
		return;
	}

	while (dir.has_next) {
		tinydir_file file;
		if (tinydir_readfile(&dir, &file) == -1) {
			tinydir_next(&dir);
			continue;
		}

		if (!file.is_dir) {
			String np = file.path;

			files.push_back(np);
		}

		tinydir_next(&dir);
	}

	tinydir_close(&dir);

	files.sort_inc();

	Vector<String> list_entries;

	for (uint32_t i = 0; i < files.size(); ++i) {
		FILE *f = fopen(files[i].c_str(), "r");

		if (!f) {
			RLOG_ERR("Settings::parse_file: Error opening file!\n");
			return;
		}

		fseek(f, 0, SEEK_END);
		long fsize = ftell(f);
		fseek(f, 0, SEEK_SET); /* same as rewind(f); */

		String fd;
		fd.resize(fsize);

		fread(&fd[0], 1, fsize, f);
		fclose(f);

		Utils::markdown_to_html(&fd);

		list_entries.push_back(fd);
	}

	render_entries(list_entries);
	render_no_entries_response();
}

void ListPage::render_entries(const Vector<String> &list_entries) {
	if (list_entries.size() == 0) {
		return;
	}

	int pages = Math::floorf_int(Math::divf(list_entries.size(), entry_per_page));
	for (int i = 0; i < pages; ++i) {
		String r = "";

		int efrom = i * entry_per_page;
		int eto = MIN((i + 1) * entry_per_page, list_entries.size());

		r = render_page(i, pages, list_entries, efrom, eto);
		_pages.push_back(r);
	}
}

String ListPage::render_page(const int page_index, const int page_count, const Vector<String> &list_entries, const int efrom, const int eto) {
	HTMLBuilder b;

	b.div(main_div_class);

	for (int i = efrom; i < eto; ++i) {
		b.w(render_entry(list_entries[i]));
	}

	b.w(Utils::get_pagination(get_full_uri(), page_count, page_index, max_visible_navigation_links));
	b.cdiv();

	return b.result;
}

String ListPage::render_entry(const String &list_entry) {
	HTMLBuilder b;

	b.div(main_div_class);
	b.div(empty_div_class)->w(list_entry)->cdiv();
	b.cdiv();

	return b.result;
}

void ListPage::render_no_entries_response() {
	HTMLBuilder b;

	b.div(empty_div_class)->w(placeholder_text)->cdiv();

	_no_entries_response = b.result;
}

void ListPage::_notification(const int what) {
	switch (what) {
		case NOTIFICATION_ENTER_TREE:
			load();
			break;
		case NOTIFICATION_EXIT_TREE:
			_pages.clear();
			break;
		default:
			break;
	}
}

ListPage::ListPage() :
		WebNode() {

	max_visible_navigation_links = 6;
	entry_per_page = 4;
	main_div_class = "list_page";
	entry_div_class = "list_entry";
	empty_div_class = "list_entry_empty";
	placeholder_text = "No content yet!";
}

ListPage::~ListPage() {
}