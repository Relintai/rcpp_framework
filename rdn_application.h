#ifndef RDN_APPLICATION_H
#define RDN_APPLICATION_H

#include "core/application.h"
#include "core/theme.h"
#include "core/object.h"

class RDNApplication : public Application {
public:
    static void index(Object *instance, Request *request);

    static void session_middleware_func(Object* instance, Request *request);

    virtual void setup_routes();
    virtual void setup_middleware();

    RDNApplication();
    ~RDNApplication();

    std::vector<Theme *> themes;
};

#endif