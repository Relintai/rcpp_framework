#include "static_folder_page.h"

#include "web/http/request.h"

#include <iostream>
#include "web/file_cache.h"
#include "database/database_manager.h"
#include "web/html/html_builder.h"

void StaticFolderPage::handle_request_main(Request *request) {
}

void StaticFolderPage::load() {
}

StaticFolderPage::StaticFolderPage() :
		WebNode() {
}

StaticFolderPage::~StaticFolderPage() {
}
