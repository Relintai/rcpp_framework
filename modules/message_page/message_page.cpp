#include "message_page.h"

#include "core/database.h"

#include "core/query_builder.h"

void MessagePage::index(Request *request) {
    QueryBuilder *b = db->get_builder();

    b->select("*")->from("tutorials_tbl")->finalize();

    db->query(b->query_result);

    delete b;

/*
    db->query("show databases;");
    db->query("show tables;");
    db->query("SELECT * FROM tutorials_tbl;");
*/

    std::string r = "<html><body>";

    for (uint32_t i = 0; i < messages.size(); ++i) {
        r += "<p>" + messages[i] + "</p><br>";
    }

    r += "</html></body>";

	request->response->setBody(r);
	request->send();
}

MessagePage::MessagePage() : Object() {
    messages.push_back("T message 1");
    messages.push_back("T message 2");
}

MessagePage::~MessagePage() {

}