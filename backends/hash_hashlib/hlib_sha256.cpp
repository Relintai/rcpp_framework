#include "hlib_sha256.h"

String HashLibSHA256::compute(const void *data, size_t num_bytes) {
	return hasher.operator()(data, num_bytes);
}

String HashLibSHA256::compute(const String &text) {
	return hasher.operator()(text);
}

void HashLibSHA256::add(const void *data, size_t num_bytes) {
	hasher.add(data, num_bytes);
}

String HashLibSHA256::get_hash() {
	return hasher.getHash();
}

void HashLibSHA256::get_hash(unsigned char *buffer, size_t buffer_len) {
	if (buffer_len < _SHA256::HashBytes) {
		printf("HashLibSHA256::get_hash error buffer_len < SHA256::HashBytes\n");
		return;
	}

	hasher.getHash(buffer);
}

void HashLibSHA256::reset() {
	hasher.reset();
}

void HashLibSHA256::set_as_active_implementation() {
	if (_sha_256_creation_func != nullptr) {
		printf("Warning: HashLibSHA256::set_as_active_implementation: creation_func != nullptr\n");
	}

	_sha_256_creation_func = _creation_func;
}

void HashLibSHA256::unset_as_active_implementation() {
	if (_sha_256_creation_func == _creation_func) {
		_sha_256_creation_func = nullptr;
	}
}

HashLibSHA256::HashLibSHA256() {
}

HashLibSHA256::~HashLibSHA256() {
}

SHA256 *HashLibSHA256::_creation_func() {
	return new HashLibSHA256();
}