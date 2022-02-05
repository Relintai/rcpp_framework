
#ifndef BUTTON_H
#define BUTTON_H

#include "widget.h"

#include <SDL.h>

#include "core/rect2.h"
#include "renderer/sprite.h"

#include <functional>

class Button : public Widget {
public:
	enum ButtonState {
		BUTTON_STATE_UP,
		BUTTON_STATE_HOVER,
		BUTTON_STATE_DOWN,
		BUTTON_STATE_OFF,
	};

	void _event(const SDL_Event &ev);
	void _update(float delta);
	void _render();

	std::function<void(void)> on_click;

	Button();
	virtual ~Button();

	ButtonState state;

	Sprite *up;
	Sprite *down;
	Sprite *hover;
	Sprite *off;
};

#endif
