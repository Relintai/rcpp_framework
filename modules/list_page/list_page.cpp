#include "list_page.h"

#include "core/query_builder.h"
#include "core/table_builder.h"
#include "core/query_result.h"

void ListPage::index(Request *request) {
	std::string r = "";

	for (uint32_t i = 0; i < messages.size(); ++i) {
		r += "<p>" + messages[i] + "</p><br>";
	}

	r += "";

    request->body += r;

    request->compile_and_send_body();
}

ListPage::ListPage() :
		Object() {

	messages.push_back("T message 1");
	messages.push_back("T message 2");
}

ListPage::~ListPage() {
}