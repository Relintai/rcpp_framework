#ifndef RCPP_FRAMEWORK_H
#define RCPP_FRAMEWORK_H

#include "core/containers/vector.h"
#include "core/string.h"

#include "core/object.h"

class RCPPFramework : public Object {
	RCPP_OBJECT(RCPPFramework, Object);

public:
	// Helper methods to allocate and destroy the singleton.
	// Note that creating / deleting an instance manually in your app will also work.
	static void create();
	static void destroy();
	// Usie these if you don't want to mess with the default settings.
	static void create_and_init();
	static void create_and_init(int argc, char **argv, char **envp);

	void initialize();
	void initialize(int argc, char **argv, char **envp);
	void uninitialize();

	void setup_args(int argc, char **argv, char **envp);

	virtual void load();
	virtual void migrate();

	void manage_object(Object *obj);

	RCPPFramework();
	~RCPPFramework();

	static RCPPFramework *get_singleton();

	bool allocate_settings_singleton;

#if DATABASES_ENABLED
	bool allocate_database_manager_singleton;

	// TODO Need a define for this
	bool allocate_db_settings_singleton;
#endif

#if WEB_ENABLED
	bool allocate_session_manager_singleton;
	bool allocate_file_cache_singleton;

	// By default it's set to "".
	// It will be ignored if you leave it.
	String www_root;
#endif

protected:
	virtual void _do_initialize();
	virtual void _do_uninitialize();

	bool _initialized;

	Vector<Object *> _managed_objects;

	static RCPPFramework *_instance;
};

#endif