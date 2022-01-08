
#include "web_node.h"

#include "request.h"

#include "core/settings/settings.h"

#ifdef DATABASES_ENABLED
#include "core/database/database.h"
#include "core/database/database_manager.h"
#include "core/database/query_builder.h"
#include "core/database/query_result.h"
#include "core/database/table_builder.h"
#endif

String WebNode::get_uri_segment() {
	return _uri_segment;
}
void WebNode::set_uri_segment(const String &val) {
	_uri_segment = val;
}

Settings *WebNode::get_settings() {
	if (_settings) {
		return _settings;
	}

	return Settings::get_singleton();
}
void WebNode::set_settings(Settings *settings) {
	_settings = settings;

	// todo send event to children when it's implemented?
}

#ifdef DATABASES_ENABLED

Database *WebNode::get_database() {
	if (_database) {
		return _database;
	}

	return DatabaseManager::get_singleton()->ddb;
}

Ref<TableBuilder> WebNode::get_table_builder() {
	Database *db = get_database();

	ERR_FAIL_COND_V(!db, Ref<TableBuilder>());

	return db->get_table_builder();
}

Ref<QueryBuilder> WebNode::get_query_builder() {
	Database *db = get_database();

	ERR_FAIL_COND_V(!db, Ref<QueryBuilder>());

	return db->get_query_builder();
}

void WebNode::set_database(Database *db) {
	_database = db;

	// todo send event to children when it's implemented?
}

#endif

void WebNode::handle_request_main(Request *request) {
}

void WebNode::create_validators() {
}

void WebNode::create_table() {
}

void WebNode::drop_table() {
}

void WebNode::migrate() {
}

void WebNode::create_default_entries() {
}

WebNode::WebNode() :
		Node() {
	// should look this up in parents when parented (and node parenting is implemented)
	// should have an event later when a parent gets one
#ifdef DATABASES_ENABLED
	_database = nullptr;
#endif

	// same for this
	_settings = nullptr;

	create_validators();
}

WebNode::~WebNode() {
}