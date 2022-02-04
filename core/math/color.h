#ifndef COLOR_H
#define COLOR_H

#include "core/typedefs.h"
#include <inttypes.h>

//Todo add float version
//Not sure but probably that should be the default

class ColorUInt8 {
public:
	uint32_t to_key() const;
	void from_key(const uint32_t key);

	_FORCE_INLINE_ uint8_t &operator[](int idx) {
		return components[idx];
	}
	_FORCE_INLINE_ const uint8_t &operator[](int idx) const {
		return components[idx];
	}

	ColorUInt8();
	ColorUInt8(uint8_t p_r, uint8_t p_g, uint8_t p_b, uint8_t p_a = 255);
	ColorUInt8(const uint32_t key);
	virtual ~ColorUInt8();

	union {
		struct {
			uint8_t r;
			uint8_t g;
			uint8_t b;
			uint8_t a;
		};
		uint8_t components[4];
	};
};

typedef ColorUInt8 Color;

#endif