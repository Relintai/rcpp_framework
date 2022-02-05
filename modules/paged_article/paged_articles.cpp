#include "paged_articles.h"

#include "core/html/html_builder.h"

#include "core/os/directory.h"
#include "core/utils.h"
#include "paged_article.h"

#include <iostream>

void PagedArticles::_handle_request_main(Request *request) {
	render_menu(request);

	// summary page
	request->body += summary_page;

	request->compile_and_send_body();
}

void PagedArticles::load() {
	ERR_FAIL_COND_MSG(folder == "", "Error: PagedArticles::load called, but a folder is not set!");

	if (folder.size() > 0 && folder[folder.size() - 1] == '/') {
		folder.pop_back();
	}

	Ref<Directory> dir;
	dir.instance();

	ERR_FAIL_COND_MSG(dir->open_dir(folder) != OK, "Error opening PagedArticles::folder! folder: " + folder);

	while (dir->has_next()) {
		if (!dir->read()) {
			dir->next();
			continue;
		}

		if (dir->current_is_dir()) {
			if (dir->current_is_special_dir()) {
				dir->next();
				continue;
			}

			String np = dir->current_get_path();
			String fn = dir->current_get_name();

			String ff = folder + "/" + fn;

			PagedArticle *p = new PagedArticle();
			String seg = dir->current_get_name();
			p->articles_folder = ff;
			p->set_uri_segment(seg);
			add_child(p);
		}

		dir->next();
	}

	generate_summary();
}

void PagedArticles::generate_summary() {
	HTMLBuilder b;

	b.div("article_list");

	for (int i = 0; i < get_child_count(); ++i) {
		PagedArticle *a = Object::cast_to<PagedArticle>(get_child(i));

		if (a) {
			b.a(a->get_full_uri());
			b.div("article_list_entry")->w(a->get_summary())->cdiv();
			b.ca();
		}
	}

	b.cdiv();

	summary_page = b.result;
}

void PagedArticles::_notification(const int what) {
	switch (what) {
		case NOTIFICATION_ENTER_TREE:
			load();
			break;
		default:
			break;
	}

	WebNode::_notification(what);
}

PagedArticles::PagedArticles() :
		WebNode() {
}

PagedArticles::~PagedArticles() {
}