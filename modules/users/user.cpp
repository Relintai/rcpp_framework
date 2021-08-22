#include "user.h"

#include "rapidjson/document.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/stringbuffer.h"
#include <rapidjson/writer.h>
#include <tinydir/tinydir.h>
#include <cstdio>

#include "core/database/database_manager.h"
#include "core/database/query_builder.h"
#include "core/database/query_result.h"
#include "core/database/table_builder.h"

#include "core/html/form_validator.h"
#include "core/html/html_builder.h"
#include "core/http/cookie.h"
#include "core/http/http_session.h"
#include "core/http/request.h"
#include "core/http/session_manager.h"
#include "core/utils.h"
#include "user_model.h"

void User::file_save() {
}
void User::file_load() {
	FILE *f = fopen(_file_path.c_str(), "r");

	if (!f) {
		printf("FileBasedUser::load: Error opening file! %s\n", _file_path.c_str());
		return;
	}

	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET); // same as rewind(f);

	std::string fd;
	fd.resize(fsize);

	fread(&fd[0], 1, fsize, f);
	fclose(f);

	from_json(fd);
}
void User::file_ensure_directory_exist() {
}
std::string User::file_get_base_path() {
	return _path;
}

void User::file_load_all() {
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

			User *u = new User();
			u->load(np);

			UserManager::get_singleton()->add_user(u);
		}

		tinydir_next(&dir);
	}

	tinydir_close(&dir);
	*/
}

std::string User::file_get_path() {
	return _path;
}

void User::file_set_path(const std::string &path) {
	_path = path;

	if (_path.size() > 0) {
		if (_path[_path.size() - 1] != '/') {
			_path += '/';
		}
	}
}

std::string User::to_json(rapidjson::Document *into) {
	rapidjson::Document *document;

	if (into) {
		document = into;
	} else {
		document = new rapidjson::Document();
	}

	document->SetObject();

	document->AddMember("id", id, document->GetAllocator());

	document->AddMember("name", rapidjson::Value(name_user_input.c_str(), document->GetAllocator()), document->GetAllocator());
	document->AddMember("email", rapidjson::Value(email_user_input.c_str(), document->GetAllocator()), document->GetAllocator());
	document->AddMember("rank", rank, document->GetAllocator());
	document->AddMember("pre_salt", rapidjson::Value(pre_salt.c_str(), document->GetAllocator()), document->GetAllocator());
	document->AddMember("post_salt", rapidjson::Value(post_salt.c_str(), document->GetAllocator()), document->GetAllocator());
	document->AddMember("password_hash", rapidjson::Value(password_hash.c_str(), document->GetAllocator()), document->GetAllocator());
	document->AddMember("banned", banned, document->GetAllocator());
	document->AddMember("password_reset_token", rapidjson::Value(password_reset_token.c_str(), document->GetAllocator()), document->GetAllocator());
	document->AddMember("locked", locked, document->GetAllocator());

	rapidjson::Value sa(rapidjson::Type::kArrayType);
	rapidjson::Document::AllocatorType &allocator = document->GetAllocator();

	for (int i = 0; i < sessions.size(); i++) {
		sa.PushBack(rapidjson::Value(sessions[i].c_str(), document->GetAllocator()), allocator);
	}

	document->AddMember("sessions", sa, document->GetAllocator());

	if (into) {
		return "";
	}

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	document->Accept(writer);

	std::string s = buffer.GetString();

	delete document;

	return s;
}
void User::from_json(const std::string &p_data) {

	rapidjson::Document data;
	data.Parse(p_data.c_str());

	rapidjson::Value uobj = data.GetObject();

	id = uobj["id"].GetInt();
	name_user_input = uobj["name"].GetString();
	email_user_input = uobj["email"].GetString();
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
}

User::User() :
		Resource() {

	rank = 0;
	banned = false;
	locked = false;
}

User::~User() {
}

std::string User::_path = "./";
std::string User::_table_name = "users";
