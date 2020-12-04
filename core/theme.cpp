#include "theme.h"

#include "form.h"

Theme::Theme() {
}

Theme::~Theme() {
}

THEME_CORE_STATIC_DEFINES(SiteTheme);

void SiteTheme::add_header(Request *request, std::string *output) {
	output->append(
			"<html>"
			"<head>"
			"</head>"
			"<body>");
}

void SiteTheme::add_footer(Request *request, std::string *output) {
	output->append(
			"</body>"
			"</html>");
}

void SiteTheme::render_index_page(Request *request, std::string *output) {
    add_header(request, output);

    output->append("<p>Test HTML Body</p>");

    add_footer(request, output);
}

void SiteTheme::render_form(Request *request, Form* form, std::string *output) {

}

SiteTheme::SiteTheme() {
}

SiteTheme::~SiteTheme() {
}

TestSiteTheme::TestSiteTheme() {
}

TestSiteTheme::~TestSiteTheme() {
}

TestSiteTheme2::TestSiteTheme2() {
}

TestSiteTheme2::~TestSiteTheme2() {
}
