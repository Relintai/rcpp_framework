#include "widget.h"

void Widget::event(const SDL_Event &ev) {
	_event(ev);
}
void Widget::update(float delta) {
	_update(delta);
}
void Widget::render() {
	_render();
}

void Widget::_event(const SDL_Event &ev) {
}
void Widget::_update(float delta) {
}
void Widget::_render() {
}

Widget::Widget() {
	min_size = 0;
	stretch_flags = STRETCH_OFF;
	stretch_ratio = 1;
}
Widget::~Widget() {
}
