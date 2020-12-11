#ifndef RDN_APPLICATION_H
#define RDN_APPLICATION_H

#include "core/application.h"
#include "core/object.h"

#include "modules/message_page/message_page.h"

class RDNApplication : public Application {
public:
    static void index(Object *instance, Request *request);

    static void session_middleware_func(Object* instance, Request *request);

    static void message_page_func(Object *instance, Request *request);

    virtual void setup_routes();
    virtual void setup_middleware();

    virtual void migrate();

    RDNApplication();
    ~RDNApplication();

    MessagePage *message_page;
};

#endif