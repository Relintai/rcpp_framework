#ifndef FORM_H
#define FORM_H

#include <map>
#include <string>
#include <vector>

class Request;

//This might be converted to a form validator

class FormField {
public:
    std::string name;
    std::string label;
	std::map<std::string, std::string> attribues;

    virtual std::string render();

    virtual bool validate(Request *request, std::vector<std::string> *errors);

    FormField();
    virtual ~FormField();
};

class InputFormField : public FormField {
    std::string render();
};

class InputTextFormField : public InputFormField {
    std::string render();
};

class InputPasswordFormField : public InputFormField {
    std::string render();
};

class Form {
public:
	std::string name;
	std::map<std::string, std::string> attribues;
	std::vector<FormField *> fields;

    //call Theme->render(); in it, and that will go though all attribs and call their renders
    virtual std::string render();

	Form();
	virtual ~Form();
};

#endif