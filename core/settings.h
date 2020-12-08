#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

#include "rapidjson/document.h"

class Settings {
    public:
    rapidjson::Document settings;

    void parse_file(const std::string &path);

    static Settings *get_singleton();

    Settings(const bool singleton = false);
    virtual ~Settings();

protected:
    static Settings *_singleton;
};


#endif