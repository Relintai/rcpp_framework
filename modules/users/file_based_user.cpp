#include "file_based_user.h"

#include "rapidjson/document.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/rapidjson.h"
#include "user_manager.h"
#include <rapidjson/writer.h>
#include <tinydir/tinydir.h>
#include <cstdio>

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
	//todo sanitize name!
	_file_path = _path + _nameui;

	rapidjson::Document document;
	document.SetObject();

	document.AddMember("id", get_id(), document.GetAllocator());

	document.AddMember("name", rapidjson::Value(_nameui.c_str(), document.GetAllocator()), document.GetAllocator());
	document.AddMember("email", rapidjson::Value(_emailui.c_str(), document.GetAllocator()), document.GetAllocator());
	document.AddMember("rank", _rank, document.GetAllocator());
	document.AddMember("pre_salt", rapidjson::Value(_pre_salt.c_str(), document.GetAllocator()), document.GetAllocator());
	document.AddMember("post_salt", rapidjson::Value(_post_salt.c_str(), document.GetAllocator()), document.GetAllocator());
	document.AddMember("password_hash", rapidjson::Value(_password_hash.c_str(), document.GetAllocator()), document.GetAllocator());
	document.AddMember("banned", _banned, document.GetAllocator());
	document.AddMember("password_reset_token", rapidjson::Value(_password_reset_token.c_str(), document.GetAllocator()), document.GetAllocator());
	document.AddMember("locked", _locked, document.GetAllocator());

	rapidjson::Value sa(rapidjson::Type::kArrayType);
	rapidjson::Document::AllocatorType &allocator = document.GetAllocator();

	for (int i = 0; i < _sessions.size(); i++) {
		sa.PushBack(rapidjson::Value(_sessions[i].c_str(), document.GetAllocator()), allocator);
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
	//todo sanitize name!
	_file_path = _path + p_name;

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

	set_id(uobj["id"].GetInt());
	_nameui = uobj["name"].GetString();
	_emailui = uobj["email"].GetString();
	_rank = uobj["rank"].GetInt();
	_pre_salt = uobj["pre_salt"].GetString();
	_post_salt = uobj["post_salt"].GetString();
	_password_hash = uobj["password_hash"].GetString();
	_banned = uobj["banned"].GetBool();

	_password_reset_token = uobj["password_reset_token"].GetString();
	_locked = uobj["locked"].GetBool();

	const rapidjson::Value &sess = uobj["sessions"].GetArray();

	for (rapidjson::Value::ConstValueIterator itr = sess.Begin(); itr != sess.End(); ++itr) {
		_sessions.push_back(itr->GetString());
	}

	register_sessions();
}

void FileBasedUser::load_all() {
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

			FileBasedUser *u = new FileBasedUser();
			u->load(np);

			UserManager::get_singleton()->add_user(u);
		}

		tinydir_next(&dir);
	}

	tinydir_close(&dir);
}

FileBasedUser::FileBasedUser() :
		User() {
}

FileBasedUser::~FileBasedUser() {
}

std::string FileBasedUser::_path = "./";