#include "color.h"

uint32_t ColorUInt8::to_key() const {
	uint32_t val = 0;

	val |= static_cast<uint32_t>(r) << 24;
	val |= static_cast<uint32_t>(g) << 16;
	val |= static_cast<uint32_t>(b) << 8;
	val |= static_cast<uint32_t>(a) << 0;

	return val;
}

void ColorUInt8::from_key(const uint32_t key) {
	r = (key & 0xFF000000) >> 24;
	g = (key & 0x00FF0000) >> 16;
	b = (key & 0x0000FF00) >> 8;
	a = (key & 0x000000FF) >> 0;
}

#ifdef SDL_AVAILABLE
SDL_Color ColorUInt8::to_sdl_color() const {
    SDL_Color c;

    c.r = r;
    c.g = g;
    c.b = b;
    c.a = a;

    return c;
}

void ColorUInt8::from_sdl_color(const SDL_Color &key) {
    r = key.r;
    g = key.g;
    b = key.b;
    a = key.a;
}
#endif

ColorUInt8::ColorUInt8() {
	r = 0;
	g = 0;
	b = 0;
	a = 255;
}

ColorUInt8::ColorUInt8(uint8_t p_r, uint8_t p_g, uint8_t p_b, uint8_t p_a) {
	r = p_r;
	g = p_g;
	b = p_b;
	a = p_a;
}

ColorUInt8::ColorUInt8(const uint32_t key) {
	from_key(key);
}

ColorUInt8::~ColorUInt8() {
}