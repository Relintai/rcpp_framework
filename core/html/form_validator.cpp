#include "form_validator.h"

#include "core/http/request.h"

//FormFieldEntry

bool FormFieldEntry::validate(Request *request, const std::string &data, std::vector<std::string> *errors) {
	return true;
}

FormFieldEntry::FormFieldEntry() {
}

FormFieldEntry::~FormFieldEntry() {
}

//FormField

void FormField::add_entry(FormFieldEntry *field) {
	fields.push_back(field);
}

bool FormField::validate(Request *request, std::vector<std::string> *errors) {
	std::string param = request->get_parameter(name);

	bool valid = true;

	for (int i = 0; i < fields.size(); ++i) {
		if (!fields[i]->validate(request, param, errors)) {
			valid = false;
		}
	}

	return valid;
}

FormField::FormField() {
}
FormField::~FormField() {
	for (int i = 0; i < fields.size(); ++i) {
		delete fields[i];
	}

	fields.clear();
}

//FormValidator

bool FormValidator::validate(Request *request, std::vector<std::string> *errors) {
	bool valid = true;

	for (int i = 0; i < fields.size(); ++i) {

		if (!fields[i]->validate(request, errors)) {
			valid = false;
		}
	}

	return valid;
}

void FormValidator::add_field(FormField *field) {
	fields.push_back(field);
}

FormValidator::FormValidator() {
}

FormValidator::~FormValidator() {
	for (int i = 0; i < fields.size(); ++i) {
		delete fields[i];
	}

	fields.clear();
}
