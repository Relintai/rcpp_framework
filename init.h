#ifndef FRAMEWORK_INIT_H
#define FRAMEWORK_INIT_H

#if DATABASES_ENABLED
#include "database_backends/db_init.h"
#endif

#include "platform/platform_initializer.h"

// Backends
#include "crypto_backends/hash_hashlib/setup.h"

void initialize_framework() {
#if DATABASES_ENABLED
	initialize_database_backends();
#endif

	backend_hash_hashlib_install_providers();

	PlatformInitializer::allocate_all();
}

void initialize_framework_args(int argc, char **argv, char **envp) {
	initialize_framework();

	PlatformInitializer::arg_setup(argc, argv, envp);
}

void uninitialize_framework() {
	PlatformInitializer::free_all();
}

#endif