
#include "web_node.h"

#include "http_enums.h"
#include "request.h"

#include "core/http/web_server.h"
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

String WebNode::get_full_uri(const bool slash_at_the_end) {
	if (slash_at_the_end) {
		return get_full_uri_parent(true) + _uri_segment + '/';
	} else {
		return get_full_uri_parent(true) + _uri_segment;
	}
}
String WebNode::get_full_uri_parent(const bool slash_at_the_end) {
	String uri = "/";

	WebNode *n = get_parent_webnode();

	while (n) {
		if (n->_uri_segment == "" || n->_uri_segment == '/') {
			break;
		}

		uri = "/" + n->_uri_segment + uri;

		n = n->get_parent_webnode();
	}

	if (!slash_at_the_end) {
		uri.pop_back();
	}

	return uri;
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
	_handle_request_main(request);
}

void WebNode::_handle_request_main(Request *request) {
	request->send_error(HTTP_STATUS_CODE_404_NOT_FOUND);
}

void WebNode::handle_error_send_request(Request *request, const int error_code) {
	// this is a fallback error handler.
	// Webroot implements a proper one
	request->compiled_body = "<html><body>Internal server error!</body></html>";
	request->set_status_code(HTTP_STATUS_CODE_503_SERVICE_UNAVAILABLE);
	request->send();
}

void WebNode::render_menu(Request *request) {
	WebNode *root = get_root();

	if (root) {
		root->_render_menu(request);
	}
}
void WebNode::_render_menu(Request *request) {
}

void WebNode::create_validators() {
}

void WebNode::create_table() {
}

void WebNode::drop_table() {
}

void WebNode::udpate_table() {
}

void WebNode::create_default_entries() {
}

void WebNode::migrate(const bool clear, const bool seed_db) {
	_migrate(clear, seed_db);

	for (int i = 0; i < _children.size(); ++i) {
		WebNode *c = Object::cast_to<WebNode>(_children[i]);
		c->migrate(clear, seed_db);
	}
}

void WebNode::_migrate(const bool clear, const bool seed_db) {
	if (clear) {
		drop_table();
		create_table();
	} else {
		udpate_table();
	}

	if (seed_db) {
		create_default_entries();
	}
}

WebServer *WebNode::get_server() {
	// todo this shoult probably be cached
	return Object::cast_to<WebServer>(get_tree());
}

WebNode *WebNode::get_root() {
	WebServer *s = get_server();

	if (!s) {
		return nullptr;
	}

	return s->get_web_root();
}

WebNode *WebNode::get_parent_webnode() {
	return Object::cast_to<WebNode>(get_parent());
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