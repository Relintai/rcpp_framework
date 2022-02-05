#include "sha256.h"

SHA256::SHA256() {

}

SHA256::~SHA256() {

}

Ref<SHA256> SHA256::get() {
	if (_sha_256_creation_func == nullptr) {
		printf("Error: static SHA256 *get(): creation_func == nullptr\n");
		return nullptr;
	}

	return _sha_256_creation_func();
}

Ref<SHA256> (*SHA256::_sha_256_creation_func)(void) = nullptr;
