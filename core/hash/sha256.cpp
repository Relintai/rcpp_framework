#include "sha256.h"

SHA256::SHA256() {

}

SHA256::~SHA256() {

}

SHA256 *SHA256::get() {
	if (_sha_256_creation_func == nullptr) {
		printf("Error: static SHA256 *get(): creation_func == nullptr\n");
		return nullptr;
	}

	return _sha_256_creation_func();
}

SHA256 *(*SHA256::_sha_256_creation_func)(void) = nullptr;
