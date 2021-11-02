#include "rbac_model.h"

#include "core/database/database.h"
#include "core/database/database_manager.h"
#include "core/database/query_builder.h"
#include "core/database/query_result.h"
#include "core/database/table_builder.h"

#define RBAC_RANK_TABLE "rbac_ranks"
#define RBAC_PERMISSION_TABLE "rbac_permissions"

std::map<int, Ref<RBACRank> > RBACModel::load_permissions() {
	std::map<int, Ref<RBACRank> > ranks;

	Ref<QueryBuilder> qb = DatabaseManager::get_singleton()->ddb->get_query_builder();

	qb->select("id,name,name_internal,settings,rank_permissions")->from(RBAC_RANK_TABLE);
	Ref<QueryResult> res =  qb->run();

	while (res->next_row()) {
		Ref<RBACRank> r;
		r.instance();

		r->id = res->get_cell_int(0);
		r->name = res->get_cell_str(1);
		r->name_internal = res->get_cell_str(2);
		r->settings = res->get_cell_str(3);
		r->rank_permissions = res->get_cell_int(4);

		ranks[r->id] = r;
	}

	qb->reset();
	qb->select("id,rank_id,name,url,revoke,sort_order,permissions")->from(RBAC_PERMISSION_TABLE);
	res = qb->run();

	while (res->next_row()) {
		Ref<RBACPermission> p;
		p.instance();

		p->id = res->get_cell_int(0);
		p->rank_id = res->get_cell_int(1);
		p->name = res->get_cell_str(2);
		p->url = res->get_cell_str(3);
		p->revoke = res->get_cell_bool(4);
		p->sort_order = res->get_cell_int(5);
		p->permissions = res->get_cell_int(6);

		Ref<RBACRank> r = ranks[p->rank_id];

		if (!r.is_valid()) {
			RLOG_ERR("RBACModel::load_permissions: !r.is_valid()!");
			continue;
		}

		r->permissions.push_back(p);
	}

	for (std::map<int, Ref<RBACRank> >::iterator i = ranks.begin(); i != ranks.end(); ++i) {
		Ref<RBACRank> r = i->second;

		if (r.is_valid()) {
			r->sort_permissions();
		}
	}

	return ranks;
}

void RBACModel::save(const Ref<RBACRank> &rank) {
	save_rank(rank);

	for (int i = 0; i < rank->permissions.size(); ++i) {
		Ref<RBACPermission> permission = rank->permissions[i];

		int rid = rank->id;

		if (permission->rank_id != rid) {
			permission->rank_id = rid;
		}

		save_permission(permission);
	}
}

void RBACModel::save_rank(const Ref<RBACRank> &rank) {
	Ref<QueryBuilder> qb = DatabaseManager::get_singleton()->ddb->get_query_builder();

	if (rank->id == 0) {
		qb->insert(RBAC_RANK_TABLE, "name,name_internal,settings,rank_permissions")->values();
		qb->val(rank->name)->val(rank->name_internal)->val(rank->settings)->val(rank->rank_permissions);
		qb->cvalues();
		qb->select_last_insert_id();
		Ref<QueryResult> res = qb->run();
		//qb->print();

		Ref<RBACRank> r = rank;

		r->id = res->get_last_insert_rowid();
	} else {
		qb->update(RBAC_RANK_TABLE)->set();
		qb->setp("name", rank->name);
		qb->setp("name_internal", rank->name_internal);
		qb->setp("settings", rank->settings);
		qb->setp("rank_permissions", rank->rank_permissions);
		qb->cset();
		qb->where()->wp("id", rank->id);
		qb->end_command();
		qb->run_query();
		//qb->print();
	}
}

void RBACModel::save_permission(const Ref<RBACPermission> &permission) {
	Ref<QueryBuilder> qb = DatabaseManager::get_singleton()->ddb->get_query_builder();

	if (permission->id == 0) {
		qb->insert(RBAC_PERMISSION_TABLE, "rank_id,name,url,revoke,sort_order,permissions")->values();
		qb->val(permission->rank_id)->val(permission->name)->val(permission->url)->val(permission->revoke);
		qb->val(permission->sort_order)->val(permission->permissions);
		qb->cvalues();
		qb->select_last_insert_id();
		Ref<QueryResult> res = qb->run();
		//qb->print();

		Ref<RBACPermission> r = permission;

		r->id = res->get_last_insert_rowid();
	} else {
		qb->update(RBAC_PERMISSION_TABLE)->set();
		qb->setp("rank_id", permission->rank_id);
		qb->setp("name", permission->name);
		qb->setp("url", permission->url);
		qb->setp("revoke", permission->revoke);
		qb->setp("sort_order", permission->sort_order);
		qb->setp("permissions", permission->permissions);
		qb->cset();
		qb->where()->wp("id", permission->id);
		qb->end_command();
		qb->run_query();
		//qb->print();
	}
}

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
