#include <string.h>
#include <iostream>
#include <string>

#include "core/application.h"
#include "core/file_cache.h"
#include "core/http_server.h"

#include "rdn_application.h"

#include "core/database_manager.h"

#include "database/db_init.h"

#include "core/settings.h"

#define MAIN_CLASS RDNApplication

void create_databases() {

	Settings *settings = Settings::get_singleton();

	if (!settings) {
		printf("create_databases: Settings singleton is null!");
		return;
	}

/*
	rapidjson::Value dbs = settings->settings["databases"];

	if (!dbs.IsArray()) {
		printf("create_databases: dbs !dbs.IsArray()!");
		return;
	}
*/

	DatabaseManager *dbm = DatabaseManager::get_singleton();

	uint32_t index = dbm->create_database("mysql");
	Database *db = dbm->databases[0];
	//db->_builder_creation_func = MysqlQueryBuilder::create;
	db->connect("");
}

int main(int argc, char **argv) {
	bool migrate = false;

	for (int i = 1; i < argc; ++i) {
		const char *a = argv[i];

		if (a[0] == 'm') {
			migrate = true;
		}
	}

	initialize_database_backends();

	Settings *settings = new Settings(true);

	settings->parse_file("settings.json");

	FileCache *file_cache = new FileCache(true);
	file_cache->wwwroot = "./www";
	file_cache->wwwroot_refresh_cache();

	DatabaseManager *dbm = new DatabaseManager();

	create_databases();

	Application *app = new MAIN_CLASS();

	app->load_settings();
	app->setup_routes();
	app->setup_middleware();

	HTTPServer *server = new HTTPServer();

	server->port = 8080;
	server->initialize();

	if (!migrate) {
		server->main_loop();
	} else {
		printf("Running migrations.\n");
		app->migrate();
	}

	delete server;
	delete app;
	delete dbm;
	delete file_cache;
	delete settings;

	return 0;
}