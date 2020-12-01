#include <iostream>
#include <string>

#include "core/application.h"
#include "core/file_cache.h"
#include "core/http_server.h"

#include "rdn_application.h"

#include "core/database_manager.h"

#include "database/db_init.h"

//#include "database/mysql/mysql_query_builder.h"

#define MAIN_CLASS RDNApplication

int main(int argc, char **argv) {
	initialize_database_backends();

	FileCache *file_cache = new FileCache(true);
	file_cache->wwwroot = "./www";
	file_cache->wwwroot_refresh_cache();

	DatabaseManager *dbm = new DatabaseManager();
	uint32_t index = dbm->create_database("mysql");

	Database *db = dbm->databases[0];
	//db->_builder_creation_func = MysqlQueryBuilder::create;
	db->connect("");

	//MysqlQueryBuilder * b = new MysqlQueryBuilder();



	Application *app = new MAIN_CLASS();

	app->setup_routes();
	app->setup_middleware();

	HTTPServer *server = new HTTPServer();

	server->port = 8080;
	server->initialize();
	server->main_loop();

	delete server;
	delete app;
	delete dbm;
	delete file_cache;

	return 0;
}