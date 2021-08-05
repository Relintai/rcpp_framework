#include "file_based_user.h"

#include "rapidjson/document.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/rapidjson.h"
#include <rapidjson/writer.h>
#include <tinydir/tinydir.h>
#include <cstdio>
#include "user_manager.h"

std::string FileBasedUser::get_path() {
	return _path;
}

void FileBasedUser::set_path(const std::string &path) {
	_path = path;

	if (_path.size() > 0) {
		if (_path[_path.size() - 1] != '/') {
			_path += '/';
		}
	}
}

void FileBasedUser::save() {
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
}

void FileBasedUser::load(const std::string &p_name) {
	name = p_name;

	_file_path = _path + name;

	load();
}

void FileBasedUser::load() {
	unregister_sessions();

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

	register_sessions();
}

void FileBasedUser::load_all() {
	
}

FileBasedUser::FileBasedUser() :
		User() {
}

FileBasedUser::~FileBasedUser() {
}

std::string FileBasedUser::_path = "./";