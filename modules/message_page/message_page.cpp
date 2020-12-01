#include "message_page.h"

void MessagePage::index(Request *request) {

    std::string r = "<html><body>";

    for (uint32_t i = 0; i < messages.size(); ++i) {
        r += "<p>" + messages[i] + "</p><br>";
    }

    r += "</html></body>";

	request->response->setBody(r);
	request->send();
}

MessagePage::MessagePage() {
    messages.push_back("t message 1");
    messages.push_back("t message 2");
}

MessagePage::~MessagePage() {

}