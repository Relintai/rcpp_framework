#ifndef THEME_MAP_H
#define THEME_MAP_H

#ifndef THEME_CLASS_INCLUDE
#define THEME_CLASS_INCLUDE "theme.h"
#endif

#ifndef THEME_CLASS
#define THEME_CLASS Theme
#endif

#include THEME_CLASS_INCLUDE

class ThemeMap {
public:
    THEME_CLASS *current_theme;
    std::map<std::string, THEME_CLASS> theme_map;

    ThemeMap();
    ~ThemeMap();

    static ThemeMap *get_singleton();

private:
    static ThemeMap *_instance;
};

#endif