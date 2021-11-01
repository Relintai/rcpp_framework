#include "rbac_model.h"

#include "core/database/database.h"
#include "core/database/database_manager.h"
#include "core/database/query_builder.h"
#include "core/database/query_result.h"
#include "core/database/table_builder.h"

#define RBAC_RANK_TABLE "rbac_ranks"
#define RBAC_PERMISSION_TABLE "rbac_permissions"

void RBACModel::create_table() {
	Ref<TableBuilder> tb = DatabaseManager::get_singleton()->ddb->get_table_builder();

	tb->create_table(RBAC_RANK_TABLE);
	tb->integer("id")->auto_increment()->next_row();
	tb->varchar("name", 60)->not_null()->next_row();
	tb->varchar("name_internal", 100)->not_null()->next_row();
	tb->varchar("settings", 200)->not_null()->next_row();
	tb->integer("rank_permissions")->not_null()->next_row();
	tb->primary_key("id");
	tb->ccreate_table();
	//tb->run_query();
	//tb->print();

	//tb->result = "";

	tb->create_table(RBAC_PERMISSION_TABLE);
	tb->integer("id")->auto_increment()->next_row();
	tb->integer("rank_id")->not_null()->next_row();
	tb->varchar("name", 60)->not_null()->next_row();
	tb->varchar("url", 100)->not_null()->next_row();

	tb->integer("revoke")->not_null()->next_row();
	tb->integer("sort_order")->not_null()->next_row();
	tb->integer("permissions")->not_null()->next_row();

	tb->primary_key("id");
	tb->foreign_key("rank_id")->references(RBAC_RANK_TABLE, "id");
	tb->ccreate_table();
	tb->run_query();
	//tb->print();
}
void RBACModel::drop_table() {
	Ref<TableBuilder> tb = DatabaseManager::get_singleton()->ddb->get_table_builder();

	tb->drop_table_if_exists(RBAC_PERMISSION_TABLE)->drop_table_if_exists(RBAC_RANK_TABLE)->run_query();
	//tb->print();
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
