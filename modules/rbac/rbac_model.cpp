#include "rbac_model.h"

#include "core/database/database.h"
#include "core/database/database_manager.h"
#include "core/database/query_builder.h"
#include "core/database/query_result.h"
#include "core/database/table_builder.h"

void RBACModel::create_table() {
	Ref<TableBuilder> tb = DatabaseManager::get_singleton()->ddb->get_table_builder();

	tb->create_table(_table_name);
	tb->integer("id")->auto_increment()->next_row();
	tb->varchar("username", 60)->not_null()->next_row();
	tb->varchar("email", 100)->not_null()->next_row();
	tb->integer("rank")->not_null()->next_row();
	tb->varchar("pre_salt", 100)->next_row();
	tb->varchar("post_salt", 100)->next_row();
	tb->varchar("password_hash", 100)->next_row();
	tb->integer("banned")->next_row();
	tb->varchar("password_reset_token", 100)->next_row();
	tb->integer("locked")->next_row();
	tb->primary_key("id");
	tb->ccreate_table();
	tb->run_query();
	//tb->print();
}
void RBACModel::drop_table() {
	Ref<TableBuilder> tb = DatabaseManager::get_singleton()->ddb->get_table_builder();

	tb->drop_table_if_exists(_table_name)->run_query();
}
void RBACModel::migrate() {
	drop_table();
	create_table();
}

RBACModel *RBACModel::get_singleton() {
	return _self;
}

RBACModel::RBACModel() :
		Model() {

	if (_self) {
		printf("RBACModel::RBACModel(): Error! self is not null!/n");
	}

	_self = this;
}

RBACModel::~RBACModel() {
	if (_self == this) {
		_self = nullptr;
	}
}

RBACModel *RBACModel::_self = nullptr;
