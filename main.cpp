#include <iostream>
#include <string>

#include "core/application.h"
#include "core/file_cache.h"
#include "core/http_server.h"

#include "rdn_application.h"

#include "modules/message_page/message_page.h"

#include "core/database_manager.h"

#include "database/db_init.h"

#define MAIN_CLASS RDNApplication

int main(int argc, char **argv) {
	initialize_database_backends();

	FileCache *file_cache = new FileCache(true);
	file_cache->wwwroot = "./www";
	file_cache->wwwroot_refresh_cache();

	DatabaseManager *dbm = new DatabaseManager();
	//dbm->create_database("mysql");

	Application *app = new MAIN_CLASS();

	app->setup_routes();
	app->setup_middleware();

	HTTPServer *server = new HTTPServer();

	MessagePage *mp = new MessagePage();

	printf("%s\n", mp->get_class().c_str());

	server->port = 8080;
	server->initialize();
	server->main_loop();

	delete server;
	delete app;
	delete dbm;
	delete file_cache;
	delete mp;
	

	return 0;
}