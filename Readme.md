# rcpp framework

This is a c++ html framework, with a different core design than the current mainstream html frameworks.

It's more similar to Codeigniter 3, but of course it's still different, because it's a c++ app.

Since it's a normal c++ app, lots of things can be implemented in an unusual (compared to how html frameworks work) 
albeit lot more efficient way. Also standard app/game like optimizations can be implemented.
For example it's possible to load and cache users, and only save them in a 
diffrent thread when necessary (they get dirty).

It's still in an experimental state, it should be expected to change a lot.

If you want to try it clone https://github.com/Relintai/rcpp_cms_project instead, it will set this up for you.

Only works on linux for now. Eventually I'll reimplement compile on wiondows.

Supports SQlite (built in), MySQL, and PostgreSQL databases.

## Compiling

The framework itself doesn't contains a main, and an application implementation.

You will need to provide these in order to compile the project.

You can add these as command line parameters like:

` scons main_file="../main.cpp" folders="../app/;../app_folder_2" `

This should be handled by the project's build script though.

Check the https://github.com/Relintai/rcpp_cms_project for actual compilation instructions.

## Core

Contains core classes.

### FileCahce

Will store all existing files from a directory, so you don't have to constantly check the existence
of files using the kernel.

If you pass true to it's constructor like `FileCache(true);` that instance will get set as a singleton.

You can evaluate a directory structure using `void wwwroot_evaluate_dir(const char *path, const bool should_exist = true);`.

### Application

Think about this class as a complete web application.

It contains helpers for basic routing, and has support for middlewares.

Inherit from this, and create one in your main.cpp.

The virtual methods should be called in this order after creation:

```
    app->load_settings();
    app->setup_routes();
    app->setup_middleware();
```

#### Overview

The HTTP server will turn an http request into a `Request`, then it will call the

`void handle_request(Request *request);` of it's set `Application` instance.

The `handle_request` method will set up request's middleware stack, and then
it will call `request->next_stage();`.

Note: the middlware stack might be removed later, as Request now contains a pointer
to it's owner application.

#### Middlewares

Set up your middlewares in the `setup_middleware` method.

A middleware can be any method with this signature: `void middleware(Object *instance, Request *request);`.

Any methods added to the `middlewares` vector is considered a middleware.

Every request will go through all middlewares in order, until they gets sent. For example with the `send()` method.

If a request doesn't gets sent, that connection will remain open, until a timeout occurs!

Example middleware addition using a lambda:

```
void Application::setup_middleware() {
	middlewares.push_back(HandlerInstance([this](Object *instance, Request *request){ this->default_routing_middleware(instance, request); }));
}
```

#### Routing

Set up your routes in the `setup_routes` method.

By default routing is handled by the `default_routing_middleware` method in `Application`.

It will look if a file exists in the main wwwroot first using the current path. If found it will send back that file.

Then it checks if it just needs to call the index method. If so it will just call that.

If there are multiple segments in the uri, it will grab the first, check if there is a handler
for that segment, if so it will call that handler, after a `request->push_path();`.

As you can see this middleware is simple, it will not handle slugs, and complex url patterns.
This is by design, you should implement those by hand in the actual handlers.

#### Error Handlers

Set up your error handlers in the `setup_routes` method.

Set your default fallback error handler method into the `default_error_handler_func` like:

``` default_error_handler_func = Application::default_fallback_error_handler; ```

With the `error_handler_map` you can set different error handler methods for different
error codes like:

``` error_handler_map[404] = Application::default_404_error_handler; ```

#### Updates

Sometimes `Request`s need updates, for example while sending big files. As files gets sent in chunks.
This is necessary, because loading lots big files into the memory is probably not a good idea.

So when a request needs to get updated from time to time like this, they can be registered into
the `Application` instance using `void register_request_update(Request *request);`.
When finished they can be removed with `void unregister_request_update(Request *request);`.

Note `Application::update();` will get called by the http server.

## Databases



Eventually you will be able to disable these.

### Sqlite3

The framework contains an sqlite implementation, so you should always have this available.

### MySQL

Install MySQL or MariaDB for your linux distro, you should also get a package which contains the development headers.

It should be picked up by default if present.

### PostgreSQL

Install PostgreSQL for your linux distro, you should also get a package which contains the development headers.

It should be picked up by default if present.

Note that helper classes are not yet implemented for this backend right now.

