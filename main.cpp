#include <iostream>
#include <string>

#include "core/http_server.h"
#include "core/application.h"
#include "core/file_cache.h"

#include "rdn_application.h"

#include "database/mysql/mysql_connection.h"

#define MAIN_CLASS RDNApplication

int main(int argc, char **argv) {
    FileCache *file_cache = new FileCache(true);
    file_cache->wwwroot = "./www";
    file_cache->wwwroot_refresh_cache();

    //MysqlConnection  *conn;
    //conn = new MysqlConnection();

    Application *app = new MAIN_CLASS();

    app->setup_routes();
    app->setup_middleware();

	HTTPServer *server = new HTTPServer();

	server->port = 8080;
	server->initialize();
	server->main_loop();

	delete server;
    delete app;
    delete file_cache;
   // delete conn;

	return 0;
}