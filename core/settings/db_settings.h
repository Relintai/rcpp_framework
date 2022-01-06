#ifndef DB_SETTINGS_H
#define DB_SETTINGS_H

#include "settings.h"

class DBSettings : public Settings {
	RCPP_OBJECT(DBSettings, Settings);

public:
	virtual void set_value(const String &key, const Variant &value);

	void load();

	void migrate();

	void set_table(const String &table);

	DBSettings(const bool singleton = false);
	virtual ~DBSettings();

protected:
	String _table;

	std::map<String, int> _key_map;
};

#endif