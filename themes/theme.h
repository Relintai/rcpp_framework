#ifndef THEME_H
#define THEME_H

#include<map>
#include<string>

/*
class ThemeMap {
public:
    THEME_CLASS *current_theme;
    std::map<std::string, THEME_CLASS> theme_map;

    ThemeMap() {
        _instance = this;
    }
    ~ThemeMap() {
        _instance = nullptr;
    }

    static ThemeMap *get_singleton() {
         return _instance;
    }

private:
    static ThemeMap *_instance;
};

ThemeMap *ThemeMap::_instance = nullptr;
*/

class Theme {
public:


    Theme();
    virtual ~Theme();
};


//forumtheme
//sitetheme
//...
//all singleton
//not templated
//and a theme map

#endif