#include "rcpp_framework.h"

#include "core/error_macros.h"

#if DATABASES_ENABLED
#include "database_backends/db_init.h"
#endif

#include "platform/platform_initializer.h"

// Backends
#include "crypto_backends/hash_hashlib/setup.h"

void RCPPFramework::initialize() {
	if (get_singleton() != nullptr) {
		RLOG_ERR("RCPPFramework: has already beed initialized!");
	}

	new RCPPFramework();

#if DATABASES_ENABLED
	initialize_database_backends();
#endif

	backend_hash_hashlib_install_providers();

	PlatformInitializer::allocate_all();
}

void RCPPFramework::initialize(int argc, char **argv, char **envp) {
	initialize();

	PlatformInitializer::arg_setup(argc, argv, envp);
}

void RCPPFramework::uninitialize() {
	delete _instance;

	PlatformInitializer::free_all();
}

void RCPPFramework::manage_object(Object *obj) {
	_managed_objects.push_back(obj);
}

RCPPFramework::RCPPFramework() {
	_instance = this;
}

RCPPFramework::~RCPPFramework() {
	//delete in reverse order added
	for (int i = _managed_objects.size() - 1; i >= 0; --i) {
		delete _managed_objects[i];
	}

	_managed_objects.clear();

	_instance = nullptr;
}

RCPPFramework *RCPPFramework::get_singleton() {
	return _instance;
}

RCPPFramework *RCPPFramework::_instance = nullptr;
