#include "theme_map.h"

ThemeMap::ThemeMap() {
    _instance = this;
}

ThemeMap::~ThemeMap() {
    _instance = nullptr;
}

ThemeMap *ThemeMap::get_singleton() {
    return _instance;
}

ThemeMap *ThemeMap::_instance = nullptr;