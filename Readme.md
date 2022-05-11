# rcpp framework

This is a c++ html framework, with a different core design than the current mainstream html frameworks.

It's more similar to Codeigniter 3, but of course it's still different, because it's a c++ app.

Since it's a normal c++ app, lots of things can be implemented in an unusual (compared to how html frameworks work) 
albeit lot more efficient way. Also standard app/game like optimizations can be implemented.
For example it's possible to load and cache users, and only save them in a 
different thread when necessary (they get dirty).

I consider it's design mostly stable, it needs cleanups, and some features, but it shouldn't change that much anymore fundamentally.

Note that most of this readme is outdated. It will be updated soon (-ish).
I ended up using this framework as part of my MSc thesis, and so I already managed to write proper up to date documentation for it,
which (at least the framework related things) will be reworked into a new readme / documentation, after it's accepted / graded / dealt with.

It likely wouldn't matter, but I decided I'd rather play it safe.

For the time being I recommend looking at my other projects that use this for usage examples.

If you want to try it clone https://github.com/Relintai/rcpp_cms_project instead, it will set this up for you.
Note that this needs to be updated aswell, will be done eventually.

Only works on linux for now. Eventually I'll reimplement compile on windows.

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

### HTTPServer

A simple HTTP server class. Create one in your `main()`, set up it's `application` pointer, to
actually point to a proper application instance, then you can call `main_loop()`.

`main_loop()` runs on the main thread, so this will be the main loop in your app.

Note: I'll add an async version later.

You can override `virtual void configure()` and `virtual void initialize();` to customize
the server's settings.

### Database

The core Database class contains the api for querying database backends.

These are the methods that backends implement right now:

```
    virtual void connect(const std::string &connection_str);
    virtual QueryResult *query(const std::string &query);
    virtual void query_run(const std::string &query);
```

Use these to set up database connections, and also to query the database.

You can also grab a proper `QueryBuilder` or `TableBuilder` instance for the backend using:

```
    virtual QueryBuilder *get_query_builder();
    virtual TableBuilder *get_table_builder();
```

### DatabaseManager

Can contain the active database backends. You should create one in your main, and set up
any active database backends with it.

It can instance database backends, so you don't need to include them. This is so later
these can be created from config files.

If you want to create an sqlite database, do it like:

```
    DatabaseManager *dbm = DatabaseManager::get_singleton();

    uint32_t index = dbm->create_database("sqlite");
    Database *db = dbm->databases[index];
    db->connect("database.sqlite");
```

The first created database will be set as default. This can be accesses using the `DatabaseManager`'s `ddb`
member variable. Like: DatabaseManager::get_singleton()->ddb;

### QueryBuilder

WIP, not yet finished. Will be used to provide an abstraction layer for the different database engines.

Similar to CodeIgniter 3's query builders.

### QueryResult

This is the base class for getting results from your database backend. The backends contain
the actual implementation for this. They'll also instance the proper one for themselves.

### TableBuilder

WIP, not yet finished. Will be used to provide an abstraction layer for the different database engines, 
but this one will help with table creation / manipulation.

Similar to how CodeIgniter 3 handles this.

### Utils

Common static utilities.

### Request

An HTTP request. It contains every information for http sessions.

Requests are pooled by default, you should now allocate them directly, instead
you should grab one from the `RequestPool` like: `Request *request = RequestPool::get_request();`.

#### Sending Data

It has these methods for sending data:

```
    void send();
    void send_file(const std::string &p_file_path);
    void send_error(int error_code);
```

`send()` will send the string that's set into `response`'s body.
If you want to set this directly do it like this: `request->response->setBody("<html>...etc</html>");`

After sending a request, it will automatically return to the pool.

Note: Later they'll probably be refcounted, but right now you need to make sure to call
one of the send() like methods, as without it, the connection will be kept alive for quite awhile.
Also it will lead to memory leaks.

#### HTTP Helpers

It also contains a few strings to help with the handling of http pages:

```
    std::string head;
    std::string body;
    std::string footer;
```

These can be compiled into `std::string compiled_body;` with using `void compile_body();`.

```
void Request::compile_body() {
	compiled_body.reserve(body.size() + head.size() + 13 + 14 + 15);

	//13
	compiled_body += "<html>"
					 "<head>";

	compiled_body += head;

	//14
	compiled_body += "</head>"
					 "<body>";

	compiled_body += body;
	compiled_body += footer;

	//15
	compiled_body += "</body>"
					 "</html>";

	response->setBody(compiled_body);
}
```

You can also use `void compile_and_send_body();` to both compile, and send the html.

#### URI stacks.

A Request contains a few helper methods to help with URI handling. These are:

```
    void setup_url_stack();
    std::string get_path() const;
    const std::string &get_path_full() const;
    const std::string &get_path_segment(const uint32_t i) const;
    const std::string &get_current_path_segment() const;
    uint32_t get_path_segment_count() const;
    uint32_t get_current_segment_index() const;
    uint32_t get_remaining_segment_count() const;
    void pop_path();
    void push_path();
```

With this URI's can be thought as a stack. You can push and pop path segments.

For example let's take this path: 'Test/Data/Id/2'.

By default `get_path()` will return the full string, and `get_current_path_segment()` will return `Test`.

However if we now use `push_path();` `get_path()` will return 'Data/Id/2', and `get_current_path_segment()` will return `Data`.

`pop_path();` is the opposite of `push_path();`.


### Settings

WIP, not yet finished.

### FileCahce

Will store all existing files from a directory, so you don't have to constantly check the existence
of files using the kernel.

If you pass true to it's constructor like `FileCache(true);` that instance will get set as a singleton.

You can evaluate a directory structure using `void wwwroot_evaluate_dir(const char *path, const bool should_exist = true);`.

### Form Validator

It's a wip class. Doesn't work yet.

### HTMLBuilder

Helps with creating htmls.

It has methods for all standard html tags, and has methods for all closing tags aswell.

A little example:

```
    HTMLBuilder b;

    //Add a div, with the class content: <div class="content">
    b.div()->cls("content");

    //header html tag: <header>
    b.header();

    //just write the string "My webpage"
    b.w("My webpage");

    //Add a span tag, with the class header_link: <span class="header_link">
    b.span()->cls("header_link");

    //Add a [ into the html: [
    b.w(" [ ");

    //Add a link: <a href="https://github.com/Relintai">
    b.a()->href("https://github.com/Relintai");

    //just write the Github string: Github
    b.w("Github");

    //Close the a tag: </a>
    b.ca();

    //Add a ] into the html: ]
    b.w(" ]");

    //close the header tag </header>
    b.cheader();

    //close the content div: </div>
    b.cdiv();

    //Finish the currently open tag:
    b.write_tag();

    //print the resulting string
    printf("%s\n", b.result.c_str());
```

The resulting html should look like this:
(Note that the generated string doesn't actually have newlines!)

```
<div class="content">
    <header> 
        My webpage<span class="header_link"> [ <a href="https://github.com/Relintai">Github</a> ]
    </header>
</div>
```

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

Note: the middleware stack might be removed later, as Request now contains a pointer
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

Note: Eventually you will be able to disable these one by one even if they can be compiled.

### Sqlite3

The framework contains an sqlite implementation, so you should always have this available.

### MySQL

Install MySQL or MariaDB for your linux distro, you should also get a package which contains the development headers.

It should be picked up by default if present.

### PostgreSQL

Install PostgreSQL for your linux distro, you should also get a package which contains the development headers.

It should be picked up by default if present.

Note that helper classes are not yet implemented for this backend right now.

## Libs

Don't forget to check the libraries in the libs folder, they can be directly included and used.


