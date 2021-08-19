#include "db_based_user.h"

#include "core/database/database_manager.h"
#include "core/database/table_builder.h"
#include "core/database/query_builder.h"
#include "user_manager.h"
#include <cstdio>

void DBBasedUser::save() {
	QueryBuilder *b = DatabaseManager::get_singleton()->ddb->get_query_builder();

	if (id == 0) {
		b->insert(_table_name);
	} else {
		//todo
	}


	delete b;

	/*
	//todo sanitize name!
	_file_path = _path + name;

	rapidjson::Document document;
	document.SetObject();

	document.AddMember("id", id, document.GetAllocator());

	document.AddMember("name", rapidjson::Value(name.c_str(), document.GetAllocator()), document.GetAllocator());
	document.AddMember("email", rapidjson::Value(email.c_str(), document.GetAllocator()), document.GetAllocator());
	document.AddMember("rank", rank, document.GetAllocator());
	document.AddMember("pre_salt", rapidjson::Value(pre_salt.c_str(), document.GetAllocator()), document.GetAllocator());
	document.AddMember("post_salt", rapidjson::Value(post_salt.c_str(), document.GetAllocator()), document.GetAllocator());
	document.AddMember("password_hash", rapidjson::Value(password_hash.c_str(), document.GetAllocator()), document.GetAllocator());
	document.AddMember("banned", banned, document.GetAllocator());
	document.AddMember("password_reset_token", rapidjson::Value(password_reset_token.c_str(), document.GetAllocator()), document.GetAllocator());
	document.AddMember("locked", locked, document.GetAllocator());

	rapidjson::Value sa(rapidjson::Type::kArrayType);
	rapidjson::Document::AllocatorType &allocator = document.GetAllocator();

	for (int i = 0; i < sessions.size(); i++) {
		sa.PushBack(rapidjson::Value(sessions[i].c_str(), document.GetAllocator()), allocator);
	}

	document.AddMember("sessions", sa, document.GetAllocator());

	FILE *fp = fopen(_file_path.c_str(), "w");

	char writeBuffer[65536];
	rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));

	rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
	document.Accept(writer);

	fclose(fp);
	*/
}

void DBBasedUser::load() {
	unregister_sessions();

	/*
	FILE *f = fopen(_file_path.c_str(), "r");

	if (!f) {
		printf("DBBasedUser::load: Error opening file! %s\n", _file_path.c_str());
		return;
	}

	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET); // same as rewind(f);

	std::string fd;
	fd.resize(fsize);

	fread(&fd[0], 1, fsize, f);
	fclose(f);

	rapidjson::Document data;
	data.Parse(fd.c_str());

	rapidjson::Value uobj = data.GetObject();

	id = uobj["id"].GetInt();
	name = uobj["name"].GetString();
	email = uobj["email"].GetString();
	rank = uobj["rank"].GetInt();
	pre_salt = uobj["pre_salt"].GetString();
	post_salt = uobj["post_salt"].GetString();
	password_hash = uobj["password_hash"].GetString();
	banned = uobj["banned"].GetBool();

	password_reset_token = uobj["password_reset_token"].GetString();
	locked = uobj["locked"].GetBool();

	const rapidjson::Value &sess = uobj["sessions"].GetArray();

	for (rapidjson::Value::ConstValueIterator itr = sess.Begin(); itr != sess.End(); ++itr) {
		sessions.push_back(itr->GetString());
	}
*/
	register_sessions();
}

void DBBasedUser::migrate() {
	TableBuilder *tb = DatabaseManager::get_singleton()->ddb->get_table_builder();

	tb->drop_table_if_exists(_table_name)->run_query();
	tb->drop_table_if_exists(_table_name + "_sessions")->run_query();
	//tb->print();

	tb->result = "";

	tb->create_table(_table_name);
	tb->integer("id")->not_null()->auto_increment()->next_row();
	tb->varchar("username", 60)->not_null()->next_row();
	tb->integer("rank")->not_null()->next_row();
	tb->varchar("pre_salt", 100)->next_row();
	tb->varchar("post_salt", 100)->next_row();
	tb->varchar("password_hash", 100)->next_row();
	tb->integer("banned")->next_row();
	tb->varchar("password_reset_token", 100)->next_row();
	tb->integer("locked")->next_row();
	tb->primary_key("id");
	tb->ccreate_table();
	tb->run();
	//tb->print();

	tb->result = "";

	tb->create_table(_table_name + "_sessions");
	tb->integer("user_id")->not_null()->next_row();
	tb->varchar("session_id", 100)->next_row();
	tb->foreign_key("user_id");
	tb->references("user", "id");
	tb->ccreate_table();
	//tb->print();
	tb->run();

	delete tb;
}

void DBBasedUser::load_all() {
	/*
	tinydir_dir dir;
	if (tinydir_open(&dir, _path.c_str()) == -1) {
		return;
	}

	while (dir.has_next) {
		tinydir_file file;
		if (tinydir_readfile(&dir, &file) == -1) {
			tinydir_next(&dir);
			continue;
		}

		if (!file.is_dir) {
			std::string np = file.path;
			np = np.substr(_path.size(), np.size() - _path.size());

			DBBasedUser *u = new DBBasedUser();
			u->load(np);

			UserManager::get_singleton()->add_user(u);
		}

		tinydir_next(&dir);
	}

	tinydir_close(&dir);
	*/
}

DBBasedUser::DBBasedUser() :
		User() {
}

DBBasedUser::~DBBasedUser() {
}

std::string DBBasedUser::_table_name = "users";