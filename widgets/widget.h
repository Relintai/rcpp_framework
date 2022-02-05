
#ifndef WIDGET_H
#define WIDGET_H

#include <SDL.h>

#include "core/rect2.h"
#include "core/vector.h"

//remove sdl dependency
//should use my own events
//render should have a parameter for the current renderer
//it couls store state in a different class -> it could be a mix between an immediate mode gui and normal oop gui
//Or maybe the state could be stored separately? Or have support for storing the state in widgets, and also in a separate data object. Also needs to be fast.
//Or just support using state from (also adding a deafult state to) an HTTPRequest
//it sould do it's layout in a normalized coordinate system 
//the renderer can turn it to what it needs
//Shold probably have a widget renderer that encapsulates a normal renderer
//could have 2 type of renderers -> one for httprequests and a normal one

class Widget {
public:
	enum StretchFlags {
		STRETCH_HORIZONTAL = 1 << 0,
		STRETCH_VERTICAL = 1 << 1,

		STRETCH_OFF = 0,
		STRETCH_BOTH = STRETCH_HORIZONTAL | STRETCH_VERTICAL,
	};

	void event(const SDL_Event &ev);
	void update(float delta);
	void render();

	virtual void _event(const SDL_Event &ev);
	virtual void _update(float delta);
	virtual void _render();

	Widget();
	virtual ~Widget();

	float min_size;
	int stretch_flags;
	float stretch_ratio;

	Rect2 rect;

private:
	Vector<Widget *> _children;
};

#endif
