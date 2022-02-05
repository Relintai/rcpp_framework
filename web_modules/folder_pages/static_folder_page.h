#ifndef STATIC_FOLDER_PAGE_H
#define STATIC_FOLDER_PAGE_H

#include "core/string.h"

#include "core/http/web_node.h"

//StaticFolderPage
//Just serve files
//404 if doesnt exist

//staticsitefolderpage
//index file support per dir
//could render md -> if .md -> check if changed -> cache -> file date
//could render bbcode -> if .bb

//static file list
//same as StaticFolderPage but can render file list

class StaticFolderPage : public WebNode {
	RCPP_OBJECT(StaticFolderPage, WebNode);

public:
	void handle_request_main(Request *request);

	void load();

	StaticFolderPage();
	~StaticFolderPage();

	String folder;
};

#endif