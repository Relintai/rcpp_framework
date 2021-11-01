#ifndef SHA256_H
#define SHA256_H

#include "core/string.h"

#include "core/reference.h"

class SHA256 : public Reference {
	RCPP_OBJECT(SHA256, Reference);

public:
	virtual String compute(const void *data, size_t num_bytes) = 0;
	virtual String compute(const String &text) = 0;

	virtual void add(const void *data, size_t num_bytes) = 0;
	virtual String get_hash() = 0;
	virtual void get_hash(unsigned char *buffer, size_t buffer_len) = 0;

	virtual void reset() = 0;

	static SHA256 *get();

	SHA256();
	virtual ~SHA256();

protected:
	static SHA256 *(*_sha_256_creation_func)(void);
};

#endif