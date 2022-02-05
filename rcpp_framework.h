#ifndef RCPP_FRAMEWORK_H
#define RCPP_FRAMEWORK_H

#include "core/string.h"
#include "core/containers/vector.h"

#include "core/object.h"

class RCPPFramework : Object {
    RCPP_OBJECT(RCPPFramework, Object);

public:
    static void initialize();
    static void initialize(int argc, char **argv, char **envp);
    static void uninitialize();

    void manage_object(Object* obj);

    RCPPFramework();
    ~RCPPFramework();

    static RCPPFramework *get_singleton();

protected:
    Vector<Object *> _managed_objects;

    static RCPPFramework *_instance;
};

#endif