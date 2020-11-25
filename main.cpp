#include <iostream>
#include <string>

#include "core/http_server.h"
#include "core/application.h"
#include "core/file_cache.h"

#include "rdn_application.h"

#define MAIN_CLASS RDNApplication

int main(int argc, char **argv) {
    FileCache *file_cache = new FileCache();
    file_cache->wwwroot = "./www";
    file_cache->refresh();

    Application *app = new MAIN_CLASS();

    app->setup_routes();

	HTTPServer *server = new HTTPServer();

	server->port = 8080;
	server->initialize();
	server->main_loop();

	delete server;
    delete app;
    delete file_cache;

	return 0;
}