#ifndef FORM_H
#define FORM_H

#include <map>
#include <string>
#include <vector>

class Request;

class FormFieldEntry {
public:
	virtual bool validate(Request *request, const std::string &data, std::vector<std::string> *errors);

	FormFieldEntry();
	virtual ~FormFieldEntry();
};

class FormField {
public:
	std::string name;

	void add_entry(FormFieldEntry *field);

	bool validate(Request *request, std::vector<std::string> *errors);

	FormField();
	virtual ~FormField();

	std::vector<FormFieldEntry *> fields;
};

class FormValidator {
public:
    bool validate(Request *request, std::vector<std::string> *errors = nullptr);

	void add_field(FormField *field);

	FormValidator();
	virtual ~FormValidator();

	std::vector<FormField *> fields;
};

#endif