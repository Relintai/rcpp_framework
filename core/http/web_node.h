#ifndef WEB_NODE_H
#define WEB_NODE_H

#include "core/nodes/node.h"
#include "core/reference.h"

class Request;
class Settings;

#ifdef DATABASES_ENABLED
class DataBase;
class TableBuilder;
class QueryBuilder;
#endif

class WebNode : public Node {
	RCPP_OBJECT(WebNode, Node);

public:
	Settings *get_settings();
	void set_settings(Settings *settings);

#ifdef DATABASES_ENABLED
	Database *get_database();
	Ref<TableBuilder> get_table_builder();
	Ref<QueryBuilder> get_query_builder();
	void set_database(Database *db);
#endif

	virtual void handle_request_main(Request *request);
	virtual void create_validators();

	virtual void create_table();
	virtual void drop_table();
	virtual void migrate();
	virtual void create_default_entries();

	WebNode();
	~WebNode();

protected:
	Settings *_settings;

#ifdef DATABASES_ENABLED
	Database *_database;
#endif
};

#endif