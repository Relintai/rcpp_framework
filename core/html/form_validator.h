#ifndef FORM_H
#define FORM_H

#include <ctype.h>
#include <map>
#include <string>
#include <vector>

class Request;
class FormField;

class FormFieldEntry {
public:
	virtual bool validate(Request *request, const FormField* field, const std::string &data, std::vector<std::string> *errors);

	FormFieldEntry();
	virtual ~FormFieldEntry();
};

class FormExistsFieldEntry : public FormFieldEntry {
public:
	virtual bool validate(Request *request, const FormField* field, const std::string &data, std::vector<std::string> *errors);

	FormExistsFieldEntry();
	~FormExistsFieldEntry();

	std::string not_exists_error;
};

class FormIntFieldEntry : public FormFieldEntry {
public:
	virtual bool validate(Request *request, const FormField* field, const std::string &data, std::vector<std::string> *errors);

	FormIntFieldEntry();
	~FormIntFieldEntry();

	std::string not_int_error;
};

class FormFloatFieldEntry : public FormFieldEntry {
public:
	virtual bool validate(Request *request, const FormField* field, const std::string &data, std::vector<std::string> *errors);

	FormFloatFieldEntry();
	~FormFloatFieldEntry();

	std::string not_float_error;
};

class FormAlphaFieldEntry : public FormFieldEntry {
public:
	virtual bool validate(Request *request, const FormField* field, const std::string &data, std::vector<std::string> *errors);

	FormAlphaFieldEntry();
	~FormAlphaFieldEntry();

	std::string not_alpha_error;
};

class FormAlphaNumericFieldEntry : public FormFieldEntry {
public:
	virtual bool validate(Request *request, const FormField* field, const std::string &data, std::vector<std::string> *errors);

	FormAlphaNumericFieldEntry();
	~FormAlphaNumericFieldEntry();

	std::string not_alpha_numeric_error;
};

class FormNeedsLowercaseCharacterFieldEntry : public FormFieldEntry {
public:
	virtual bool validate(Request *request, const FormField* field, const std::string &data, std::vector<std::string> *errors);

	FormNeedsLowercaseCharacterFieldEntry();
	~FormNeedsLowercaseCharacterFieldEntry();

	std::string does_not_have_lowercase_error;
};

class FormNeedsUppercaseCharacterFieldEntry : public FormFieldEntry {
public:
	virtual bool validate(Request *request, const FormField* field, const std::string &data, std::vector<std::string> *errors);

	FormNeedsUppercaseCharacterFieldEntry();
	~FormNeedsUppercaseCharacterFieldEntry();

	std::string does_not_have_uppercase_error;
};

class FormNeedsOtherCharacterFieldEntry : public FormFieldEntry {
public:
	virtual bool validate(Request *request, const FormField* field, const std::string &data, std::vector<std::string> *errors);

	FormNeedsOtherCharacterFieldEntry();
	~FormNeedsOtherCharacterFieldEntry();

	std::string does_not_have_other_error;
};

class FormMinimumLengthFieldEntry : public FormFieldEntry {
public:
	virtual bool validate(Request *request, const FormField* field, const std::string &data, std::vector<std::string> *errors);

	FormMinimumLengthFieldEntry();
	~FormMinimumLengthFieldEntry();

	int min_length;

	std::string does_not_have_min_length_errorf;
	std::string does_not_have_min_length_errors;
};

class FormMaximumLengthFieldEntry : public FormFieldEntry {
public:
	virtual bool validate(Request *request, const FormField* field, const std::string &data, std::vector<std::string> *errors);

	FormMaximumLengthFieldEntry();
	~FormMaximumLengthFieldEntry();

	int max_length;

	std::string does_not_have_max_length_errorf;
	std::string does_not_have_max_length_errors;
};

class FormEmailFieldEntry : public FormFieldEntry {
public:
	virtual bool validate(Request *request, const FormField* field, const std::string &data, std::vector<std::string> *errors);

	FormEmailFieldEntry();
	~FormEmailFieldEntry();

	std::string email_format_error;
};

class FormNeedToMatchOtherFieldEntry : public FormFieldEntry {
public:
	virtual bool validate(Request *request, const FormField* field, const std::string &data, std::vector<std::string> *errors);

	FormNeedToMatchOtherFieldEntry();
	~FormNeedToMatchOtherFieldEntry();

	std::string other_field;

	std::string does_not_match_error;
};

//FormField

class FormField {
public:
	std::string name;
    std::string human_name;

	bool _ignore_if_not_exists;

	bool _ignore_if_other_field_not_exists;
	std::string _ignore_if_other_field_not_exist_field;

	FormField *need_to_exist();
	FormField *need_to_be_int();
	FormField *need_to_be_float();
	FormField *need_to_be_alpha();
	FormField *need_to_be_alpha_numeric();
	FormField *need_to_have_lowercase_character();
	FormField *need_to_have_uppercase_character();
	FormField *need_to_have_other_character();
	FormField *need_minimum_length(const int min_length);
	FormField *need_maximum_length(const int max_length);
	FormField *need_to_be_email();
    FormField *need_to_match(const std::string &other);
	FormField *ignore_if_not_exists();
	FormField *ignore_if_other_field_not_exists(const std::string &other);

	void add_entry(FormFieldEntry *field);

	bool validate(Request *request, std::vector<std::string> *errors);

	FormField();
	virtual ~FormField();

	std::vector<FormFieldEntry *> fields;
};

//FormValidator

class FormValidator {
public:
	bool validate(Request *request, std::vector<std::string> *errors = nullptr);

	void add_field(FormField *field);
	FormField *new_field(const std::string &name, const std::string &human_name);

	FormValidator();
	virtual ~FormValidator();

	std::vector<FormField *> fields;
};

#endif