#include "message_page.h"

#include "core/database.h"

#include "core/query_builder.h"
#include "core/table_builder.h"
#include "core/query_result.h"

void MessagePage::index(Request *request) {
	QueryBuilder *b = db->get_query_builder();

	b->select("text")->from("message_page")->finalize();

	QueryResult *res = db->query(b->query_result);

	std::vector<std::string> msgs;

	if (res) {
		while (res->next_row()) {
			msgs.push_back(res->get_cell(0));
		}
	}

	delete res;
	delete b;

	std::string r = "<html><body>";

	for (uint32_t i = 0; i < messages.size(); ++i) {
		r += "<p>" + messages[i] + "</p><br>";
	}

	for (uint32_t i = 0; i < msgs.size(); ++i) {
		r += "<p>" + msgs[i] + "</p><br>";
	}

	r += "</html></body>";

	request->body += r;

	request->compile_and_send_body();
}

void MessagePage::migrate() {
	TableBuilder *t = db->get_table_builder();

	t->drop_table("message_page");
	db->query_run(t->result);
	
	printf("%s\n", t->result.c_str());

	t->result.clear();

	t->create_table("message_page")->integer("id")->auto_increment()->primary_key()->next_row()->varchar("text", 30)->finalize();

	printf("%s\n", t->result.c_str());

	db->query(t->result);

	QueryBuilder *b = db->get_query_builder();

	b->insert("message_page", "default, 'aaewdwd'");

	printf("%s\n", b->query_result.c_str());

	db->query_run(b->query_result);

	b->query_result.clear();
	b->insert("message_page", "default, 'qqqqq'");

	printf("%s\n", b->query_result.c_str());

	db->query_run(b->query_result);

	delete t;
}

MessagePage::MessagePage() :
		Object() {
	messages.push_back("T message 1");
	messages.push_back("T message 2");
}

MessagePage::~MessagePage() {
}