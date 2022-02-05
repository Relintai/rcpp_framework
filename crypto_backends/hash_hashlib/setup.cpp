#include "setup.h"

#include "hlib_sha256.h"

void backend_hash_hashlib_install_providers() {
    HashLibSHA256::set_as_active_implementation();
}

void backend_hash_hashlib_uninstall_providers() {
    HashLibSHA256::unset_as_active_implementation();
}

