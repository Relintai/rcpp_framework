#ifndef COLOR_H
#define COLOR_H

#include <inttypes.h>

#ifdef SDL_AVAILABLE
#include <SDL.h>
#endif

class Color {
public:
	uint32_t to_key() const;
	void from_key(const uint32_t key);

#ifdef SDL_AVAILABLE
	SDL_Color to_sdl_color() const;
	void from_sdl_color(const SDL_Color &key);
#endif

	Color();
	Color(uint8_t p_r, uint8_t p_g, uint8_t p_b, uint8_t p_a = 255);
	Color(const uint32_t key);
	virtual ~Color();

	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};

#endif