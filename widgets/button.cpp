#include "button.h"

#include "math.h"

void Button::_event(const SDL_Event &ev) {
	if (state == BUTTON_STATE_OFF) {
		return;
	}

	switch (ev.type) {
		case SDL_MOUSEMOTION: {
			int x = ev.motion.x;
			int y = ev.motion.y;

			if (rect.x < x && rect.x + rect.w > x && rect.y < y && rect.y + rect.w > y) {
				state = BUTTON_STATE_HOVER;
			} else {
				state = BUTTON_STATE_UP;
			}

			break;
		}
		case SDL_MOUSEBUTTONDOWN: {
			int x = ev.motion.x;
			int y = ev.motion.y;

			if (rect.x < x && rect.x + rect.w > x && rect.y < y && rect.y + rect.w > y) {
				state = BUTTON_STATE_DOWN;
			}

			break;
		}
		case SDL_MOUSEBUTTONUP: {
			int x = ev.motion.x;
			int y = ev.motion.y;

			if (rect.x < x && rect.x + rect.w > x && rect.y < y && rect.y + rect.w > y) {
				state = BUTTON_STATE_HOVER;

				if (on_click) {
					on_click();
				}
			} else {
				state = BUTTON_STATE_UP;
			}

			break;
		}
	}
}

void Button::_update(float delta) {
}

void Button::_render() {
	if (state == BUTTON_STATE_UP) {
		if (up) {
			up->set_transform(rect);
			up->draw();
		}
	} else if (state == BUTTON_STATE_HOVER) {
		if (hover) {
			hover->set_transform(rect);
			hover->draw();
		}
	} else if (state == BUTTON_STATE_DOWN) {
		if (down) {
			down->set_transform(rect);
			down->draw();
		}
	} else if (state == BUTTON_STATE_OFF) {
		if (off) {
			off->set_transform(rect);
			off->draw();
		}
	}
}

Button::Button() : Widget() {
	state = BUTTON_STATE_UP;

	up = nullptr;
	down = nullptr;
	hover = nullptr;
	off = nullptr;
}

Button::~Button() {
}
