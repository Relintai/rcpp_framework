#ifndef SHA256_H
#define SHA256_H

#include <string>

class SHA256 {
public:
	virtual std::string compute(const void *data, size_t num_bytes) = 0;
	virtual std::string compute(const std::string &text) = 0;

	virtual void add(const void *data, size_t num_bytes) = 0;
	virtual std::string get_hash() = 0;
	virtual void get_hash(unsigned char *buffer, size_t buffer_len) = 0;

	virtual void reset() = 0;

	static SHA256 *get();

	SHA256();
	virtual ~SHA256();

protected:
	static SHA256 *(*_sha_256_creation_func)(void);
};

#endif