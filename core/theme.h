#ifndef THEME_H
#define THEME_H

#include "request.h"
#include <map>
#include <string>

#define THEME_CORE(_class_name)                               \
public:                                                       \
	std::string theme_name;                                   \
	static _class_name *default_theme;                        \
	static std::map<std::string, _class_name *> theme_map;    \
                                                              \
	void register_theme(const std::string &p_theme_name) {    \
		theme_name = p_theme_name;                            \
		theme_map[theme_name] = this;                         \
	}                                                         \
                                                              \
	void set_theme_as_default() {                             \
		default_theme = this;                                 \
	}                                                         \
                                                              \
	void register_theme() {                                   \
		register_theme(#_class_name);                         \
	}                                                         \
                                                              \
	void unregister_theme() {                                 \
		theme_map.erase(theme_name);                          \
		if (default_theme == this)                            \
			default_theme = nullptr;                          \
	}                                                         \
                                                              \
	_class_name *get_theme(const std::string &p_theme_name) { \
		_class_name *t = theme_map[p_theme_name];             \
		if (t)                                                \
			return t;                                         \
		else                                                  \
			return default_theme;                             \
	}

#define THEME_CORE_STATIC_DEFINES(_class_name)         \
	_class_name *_class_name::default_theme = nullptr; \
	std::map<std::string, _class_name *> _class_name::theme_map;

class Theme {
public:
	Theme();
	virtual ~Theme();
};

class SiteTheme : public Theme {
	THEME_CORE(SiteTheme);

public:
	virtual void add_header(Request *request, std::string *output);
	virtual void add_footer(Request *request, std::string *output);

	virtual void render_index_page(Request *request, std::string *output);

	SiteTheme();
	~SiteTheme();
};

class TestSiteTheme : public SiteTheme {
public:
	TestSiteTheme();
	~TestSiteTheme();
};

class TestSiteTheme2 : public SiteTheme {
public:
	TestSiteTheme2();
	~TestSiteTheme2();
};

#endif