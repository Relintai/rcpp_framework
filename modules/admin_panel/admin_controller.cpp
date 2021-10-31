#include "admin_controller.h"

#include "core/html/form_validator.h"
#include "core/html/html_builder.h"
#include "core/http/cookie.h"
#include "core/http/http_session.h"
#include "core/http/request.h"
#include "core/http/session_manager.h"

AdminController::AdminController() :
		Controller() {

}

AdminController::~AdminController() {
}
