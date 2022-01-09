#ifndef HASHLIB_SHA256_H
#define HASHLIB_SHA256_H

#include "core/hash/sha256.h"

#include "./hash-library/sha256.h"

class HashLibSHA256 : public SHA256 {
public:
	String compute(const void *data, size_t num_bytes);
	String compute(const String &text);

	void add(const void *data, size_t num_bytes);
	String get_hash();
	void get_hash(unsigned char *buffer, size_t buffer_len);

	void reset();

	static void set_as_active_implementation();
	static void unset_as_active_implementation();

	HashLibSHA256();
	~HashLibSHA256();

protected:
	static Ref<SHA256> _creation_func();

	_SHA256 hasher;
};

#endif