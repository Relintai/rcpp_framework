#include "rcpp_framework.h"

#include "core/error_macros.h"

#include "core/settings/settings.h"

#if DATABASES_ENABLED
#include "database/database_manager.h"
#include "database_backends/db_init.h"

#include "database_modules/db_settings/db_settings.h"
#endif

#if WEB_ENABLED
#include "web/file_cache.h"
#include "web/http/session_manager.h"
#endif

#include "platform/platform_initializer.h"

// Backends
#include "crypto_backends/hash_hashlib/setup.h"

void RCPPFramework::create() {
	new RCPPFramework();
}
void RCPPFramework::destroy() {
	_instance->uninitialize();

	delete _instance;
}

void RCPPFramework::create_and_init() {
	new RCPPFramework();

	RCPPFramework::get_singleton()->initialize();
}

void RCPPFramework::create_and_init(int argc, char **argv, char **envp) {
	new RCPPFramework();

	RCPPFramework::get_singleton()->initialize();
	RCPPFramework::get_singleton()->setup_args(argc, argv, envp);
}

void RCPPFramework::initialize() {
	if (_initialized) {
		RLOG_ERR("(RCPPFramework) has already beed initialized!");
		return;
	}

	_initialized = true;

	_do_initialize();
}

void RCPPFramework::initialize(int argc, char **argv, char **envp) {
	initialize();
	setup_args(argc, argv, envp);
}

void RCPPFramework::setup_args(int argc, char **argv, char **envp) {
	ERR_FAIL_COND(!_initialized);

	PlatformInitializer::arg_setup(argc, argv, envp);
}

void RCPPFramework::uninitialize() {
	_do_uninitialize();
}

void RCPPFramework::load() {
#if DATABASES_ENABLED
	if (allocate_settings_singleton && allocate_db_settings_singleton) {
		RLOG_MSG("(RCPPFramework) Loading DBSettings singleton!");

		DBSettings::get_singleton()->load();
	}
#endif

#if WEB_ENABLED
	if (allocate_session_manager_singleton) {
		RLOG_MSG("(RCPPFramework) Loading SessionManager singleton!");

		::SessionManager::get_singleton()->load_sessions();
	}

	if (allocate_file_cache_singleton && www_root != "") {
		RLOG_MSG("(RCPPFramework) Loading FileCache singleton!");

		FileCache::get_singleton()->wwwroot = www_root;
		FileCache::get_singleton()->wwwroot_refresh_cache();
	}
#endif
}

void RCPPFramework::migrate() {
#if DATABASES_ENABLED
	if (allocate_settings_singleton && allocate_db_settings_singleton) {
		RLOG_MSG("(RCPPFramework) Migrating DBSettings singleton!");

		DBSettings::get_singleton()->migrate();
	}
#endif

#if WEB_ENABLED
	if (allocate_session_manager_singleton) {
		RLOG_MSG("(RCPPFramework) Migrating SessionManager singleton!");

		::SessionManager::get_singleton()->migrate();
	}
#endif
}

void RCPPFramework::manage_object(Object *obj) {
	ERR_FAIL_COND(!_initialized);
	ERR_FAIL_COND(!obj);

	_managed_objects.push_back(obj);
}

RCPPFramework::RCPPFramework() {
	_instance = this;

	_initialized = false;

	allocate_settings_singleton = true;

#if DATABASES_ENABLED
	allocate_database_manager_singleton = true;
	allocate_db_settings_singleton = true;
#endif

#if WEB_ENABLED
	allocate_session_manager_singleton = true;
	allocate_file_cache_singleton = true;
#endif
}

RCPPFramework::~RCPPFramework() {
	// delete in reverse order added
	for (int i = _managed_objects.size() - 1; i >= 0; --i) {
		delete _managed_objects[i];
	}

	_managed_objects.clear();

	_instance = nullptr;

	RLOG_MSG("(RCPPFramework) uninitialized!");
}

RCPPFramework *RCPPFramework::get_singleton() {
	return _instance;
}

RCPPFramework *RCPPFramework::_instance = nullptr;

void RCPPFramework::_do_initialize() {
#if DATABASES_ENABLED
	RLOG_MSG("(RCPPFramework) Initializing database backends!");
	initialize_database_backends();
#endif

	RLOG_MSG("(RCPPFramework) Initializing hash providers!");
	backend_hash_hashlib_install_providers();

	RLOG_MSG("(RCPPFramework) Initializing platforms!");
	PlatformInitializer::allocate_all();

	if (allocate_settings_singleton) {

		Settings *settings = nullptr;

#if DATABASES_ENABLED
		if (allocate_db_settings_singleton) {
			RLOG_MSG("(RCPPFramework) Allocating Settings (DBSettings) singleton!");
			settings = new DBSettings(true);
		} else {
			RLOG_MSG("(RCPPFramework) Allocating Settings (Settings) singleton!");
			settings = new Settings(true);
		}
#else
		settings = new Settings(true);

#endif

		manage_object(settings);
	}

#if DATABASES_ENABLED
	if (allocate_database_manager_singleton) {
		RLOG_MSG("(RCPPFramework) Allocating DatabaseManager singleton!");

		DatabaseManager *dbm = new DatabaseManager();
		manage_object(dbm);
	}
#endif

#if WEB_ENABLED
	if (allocate_session_manager_singleton) {
		RLOG_MSG("(RCPPFramework) Allocating SessionManager singleton!");

		::SessionManager *session_manager = new ::SessionManager();
		manage_object(session_manager);
	}

	if (allocate_file_cache_singleton) {
		RLOG_MSG("(RCPPFramework) Allocating FileCache singleton!");

		FileCache *file_cache = new FileCache(true);
		manage_object(file_cache);
	}
#endif

	RLOG_MSG("(RCPPFramework) Initialized!");
}

void RCPPFramework::_do_uninitialize() {
	RLOG_MSG("(RCPPFramework) Deleting managed objects!");
	for (int i = _managed_objects.size() - 1; i >= 0; --i) {
		delete _managed_objects[i];
	}

	_managed_objects.clear();

	RLOG_MSG("(RCPPFramework) Freeing platforms!");
	PlatformInitializer::free_all();

	RLOG_MSG("(RCPPFramework) Uninitialized!");
}