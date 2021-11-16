#include "html_builder.h"
#include "core/string.h"

HTMLTag *HTMLTag::str(const String &str) {
	result += " " + str;

	return this;
}

HTMLTag *HTMLTag::style(const String &val) {
	attrib("style", val);

	return this;
}

HTMLTag *HTMLTag::href(const String &val) {
	attrib("href", val);

	return this;
}

HTMLTag *HTMLTag::cls(const String &val) {
	attrib("class", val);

	return this;
}

HTMLTag *HTMLTag::id(const String &val) {
	attrib("id", val);

	return this;
}

HTMLTag *HTMLTag::name(const String &val) {
	attrib("name", val);

	return this;
}

HTMLTag *HTMLTag::content(const String &val) {
	attrib("content", val);

	return this;
}

HTMLTag *HTMLTag::value(const String &val) {
	attrib("value", val);

	return this;
}

HTMLTag *HTMLTag::accept(const String &val) {
	attrib("accept", val);

	return this;
}

HTMLTag *HTMLTag::src(const String &val) {
	attrib("src", val);

	return this;
}

HTMLTag *HTMLTag::alt(const String &val) {
	attrib("alt", val);

	return this;
}

HTMLTag *HTMLTag::autocomplete(const String &val) {
	attrib("autocomplete", val);

	return this;
}

HTMLTag *HTMLTag::inputmode(const String &val) {
	attrib("inputmode", val);

	return this;
}

HTMLTag *HTMLTag::list(const String &val) {
	attrib("list", val);

	return this;
}

HTMLTag *HTMLTag::checked(const bool val) {
	if (val) {
		result += " checked";
	}

	return this;
}

HTMLTag *HTMLTag::autofocus(const bool val) {
	if (val) {
		result += " autofocus";
	}

	return this;
}

HTMLTag *HTMLTag::disabled(const bool val) {
	if (val) {
		result += " disabled";
	}

	return this;
}

HTMLTag *HTMLTag::multiple(const bool val) {
	if (val) {
		result += " multiple";
	}

	return this;
}

HTMLTag *HTMLTag::required(const bool val) {
	if (val) {
		result += " required";
	}

	return this;
}

HTMLTag *HTMLTag::spellcheck(const bool val) {
	if (val) {
		attrib("spellcheck", "true");
	} else {
		attrib("spellcheck", "false");
	}

	return this;
}

HTMLTag *HTMLTag::max(const String &val) {
	attrib("max", val);

	return this;
}

HTMLTag *HTMLTag::min(const String &val) {
	attrib("min", val);

	return this;
}

HTMLTag *HTMLTag::step(const String &val) {
	attrib("step", val);

	return this;
}
HTMLTag *HTMLTag::step_any() {
	attrib("step", "any");

	return this;
}

HTMLTag *HTMLTag::minlength(const int val) {
	attrib("minlength", String::num(val));

	return this;
}
HTMLTag *HTMLTag::minlength(const String &val) {
	attrib("minlength", val);

	return this;
}
HTMLTag *HTMLTag::maxlength(const int val) {
	attrib("maxlength", String::num(val));

	return this;
}
HTMLTag *HTMLTag::maxlength(const String &val) {
	attrib("maxlength", val);

	return this;
}
HTMLTag *HTMLTag::size(const int val) {
	attrib("size", String::num(val));

	return this;
}
HTMLTag *HTMLTag::size(const String &val) {
	attrib("size", val);

	return this;
}

HTMLTag *HTMLTag::width(const int val) {
	attrib("width", String::num(val));

	return this;
}

HTMLTag *HTMLTag::width(const String &val) {
	attrib("width", val);

	return this;
}

HTMLTag *HTMLTag::height(const int val) {
	attrib("height", String::num(val));

	return this;
}

HTMLTag *HTMLTag::height(const String &val) {
	attrib("height", val);

	return this;
}

HTMLTag *HTMLTag::pattern(const String &val) {
	attrib("pattern", val);

	return this;
}

HTMLTag *HTMLTag::method(const String &val) {
	attrib("method", val);

	return this;
}

HTMLTag *HTMLTag::action(const String &val) {
	attrib("action", val);

	return this;
}

HTMLTag *HTMLTag::type(const String &val) {
	attrib("type", val);

	return this;
}

HTMLTag *HTMLTag::placeholder(const String &val) {
	attrib("placeholder", val);

	return this;
}

HTMLTag *HTMLTag::fora(const String &val) {
	attrib("for", val);

	return this;
}

HTMLTag *HTMLTag::rel(const String &val) {
	attrib("rel", val);

	return this;
}

HTMLTag *HTMLTag::rel_stylesheet() {
	attrib("rel", "stylesheet");

	return this;
}

HTMLTag *HTMLTag::charset(const String &val) {
	attrib("charset", val);

	return this;
}

HTMLTag *HTMLTag::charset_utf_8() {
	attrib("charset", "utf-8");

	return this;
}

HTMLTag *HTMLTag::itbutton() {
	attrib("type", "button");

	return this;
}
HTMLTag *HTMLTag::itcheckbox() {
	attrib("type", "checkbox");

	return this;
}
HTMLTag *HTMLTag::itcolor() {
	attrib("type", "color");

	return this;
}
HTMLTag *HTMLTag::itdate() {
	attrib("type", "date");

	return this;
}
HTMLTag *HTMLTag::itdatetime_local() {
	attrib("type", "datetime_local");

	return this;
}
HTMLTag *HTMLTag::itemail() {
	attrib("type", "email");

	return this;
}
HTMLTag *HTMLTag::itfile() {
	attrib("type", "file");

	return this;
}
HTMLTag *HTMLTag::ithidden() {
	attrib("type", "hidden");

	return this;
}
HTMLTag *HTMLTag::itimage() {
	attrib("type", "image");

	return this;
}
HTMLTag *HTMLTag::itmonth() {
	attrib("type", "month");

	return this;
}
HTMLTag *HTMLTag::itnumber() {
	attrib("type", "number");

	return this;
}
HTMLTag *HTMLTag::itpassword() {
	attrib("type", "password");

	return this;
}
HTMLTag *HTMLTag::itradio() {
	attrib("type", "radio");

	return this;
}
HTMLTag *HTMLTag::itrange() {
	attrib("type", "range");

	return this;
}
HTMLTag *HTMLTag::itreset() {
	attrib("type", "reset");

	return this;
}
HTMLTag *HTMLTag::itsearch() {
	attrib("type", "search");

	return this;
}
HTMLTag *HTMLTag::itsubmit() {
	attrib("type", "submit");

	return this;
}
HTMLTag *HTMLTag::ittel() {
	attrib("type", "tel");

	return this;
}
HTMLTag *HTMLTag::ittext() {
	attrib("type", "text");

	return this;
}
HTMLTag *HTMLTag::ittime() {
	attrib("type", "time");

	return this;
}
HTMLTag *HTMLTag::iturl() {
	attrib("type", "url");

	return this;
}
HTMLTag *HTMLTag::itweek() {
	attrib("type", "week");

	return this;
}

HTMLTag *HTMLTag::inputmode_none() {
	attrib("inputmode", "none");

	return this;
}
HTMLTag *HTMLTag::inputmode_text() {
	attrib("inputmode", "text");

	return this;
}
HTMLTag *HTMLTag::inputmode_decimal() {
	attrib("inputmode", "decimal");

	return this;
}
HTMLTag *HTMLTag::inputmode_numeric() {
	attrib("inputmode", "numeric");

	return this;
}
HTMLTag *HTMLTag::inputmode_tel() {
	attrib("inputmode", "tel");

	return this;
}
HTMLTag *HTMLTag::inputmode_search() {
	attrib("inputmode", "search");

	return this;
}
HTMLTag *HTMLTag::inputmode_email() {
	attrib("inputmode", "email");

	return this;
}
HTMLTag *HTMLTag::inputmode_url() {
	attrib("inputmode", "url");

	return this;
}

HTMLTag *HTMLTag::attrib(const String &attr, const String &val) {
	result += " " + attr + "=\"" + val + "\"";

	return this;
}

HTMLTag *HTMLTag::start(const String &p_tag, const bool p_simple) {
	simple = p_simple;

	result = "<" + p_tag;

	return this;
}

HTMLTag *HTMLTag::reset() {
	result.clear();

	return this;
}

HTMLTag *HTMLTag::close() {
	if (simple)
		result += "/>";
	else
		result += ">";

	return this;
}

HTMLBuilder *HTMLTag::f() {
	return owner;
}

bool HTMLTag::has_data() {
	return result.size() > 0;
}

HTMLTag::HTMLTag() {
	simple = true;
	owner = nullptr;
}

HTMLBuilder *HTMLBuilder::comment(const String &val) {
	write_tag();

	result += "<!--" + val + "-->";

	return this;
}

HTMLTag *HTMLBuilder::doctype() {
	write_tag();

	return tag.start("!DOCTYPE");
}

HTMLBuilder *HTMLBuilder::doctype(const String &val) {
	write_tag();

	result += "<!DOCTYPE " + val + ">";

	return this;
}

HTMLTag *HTMLBuilder::a() {
	write_tag();

	return tag.start("a");
}
HTMLTag *HTMLBuilder::abbr() {
	write_tag();

	return tag.start("abbr");
}

HTMLTag *HTMLBuilder::acronym() { //Not supported in HTML5. Use <abbr> instead. Defines an acronym
	write_tag();

	return tag.start("acronym");
}

HTMLTag *HTMLBuilder::address() {
	write_tag();

	return tag.start("address");
}

HTMLTag *HTMLBuilder::applet() { //Not supported in HTML5. Use <embed> or <object> instead. Defines an embedded applet
	write_tag();

	return tag.start("applet");
}

HTMLTag *HTMLBuilder::area() {
	write_tag();

	return tag.start("area");
}

HTMLTag *HTMLBuilder::article() {
	write_tag();

	return tag.start("article");
}

HTMLTag *HTMLBuilder::aside() {
	write_tag();

	return tag.start("aside");
}

HTMLTag *HTMLBuilder::audio() {
	write_tag();

	return tag.start("audio");
}

HTMLTag *HTMLBuilder::b() {
	write_tag();

	return tag.start("b");
}

HTMLTag *HTMLBuilder::basefont() { //Not supported in HTML5. Use CSS instead. Specifies a default color, size, and font for all text in a document
	write_tag();

	return tag.start("basefont");
}

HTMLTag *HTMLBuilder::bdi() {
	write_tag();

	return tag.start("bdi");
}

HTMLTag *HTMLBuilder::bdo() {
	write_tag();

	return tag.start("bdo");
}

HTMLTag *HTMLBuilder::big() { //Not supported in HTML5. Use CSS instead. Defines big text
	write_tag();

	return tag.start("big");
}

HTMLTag *HTMLBuilder::blockquote() {
	write_tag();

	return tag.start("blockquote");
}

HTMLTag *HTMLBuilder::body() {
	write_tag();

	return tag.start("body");
}

HTMLTag *HTMLBuilder::br() {
	write_tag();

	return tag.start("br", true);
}

HTMLTag *HTMLBuilder::button() {
	write_tag();

	return tag.start("button");
}

HTMLTag *HTMLBuilder::canvas() {
	write_tag();

	return tag.start("canvas");
}

HTMLTag *HTMLBuilder::caption() {
	write_tag();

	return tag.start("caption");
}

HTMLTag *HTMLBuilder::center() { //Not supported in HTML5. Use CSS instead. Defines centered text
	write_tag();

	return tag.start("center");
}

HTMLTag *HTMLBuilder::cite() {
	write_tag();

	return tag.start("cite");
}

HTMLTag *HTMLBuilder::code() {
	write_tag();

	return tag.start("code");
}

HTMLTag *HTMLBuilder::col() {
	write_tag();

	return tag.start("col");
}

HTMLTag *HTMLBuilder::colgroup() {
	write_tag();

	return tag.start("colgroup");
}

HTMLTag *HTMLBuilder::data() {
	write_tag();

	return tag.start("cite");
}

HTMLTag *HTMLBuilder::datalist() {
	write_tag();

	return tag.start("datalist");
}

HTMLTag *HTMLBuilder::dd() {
	write_tag();

	return tag.start("dd");
}

HTMLTag *HTMLBuilder::del() {
	write_tag();

	return tag.start("del");
}

HTMLTag *HTMLBuilder::details() {
	write_tag();

	return tag.start("details");
}

HTMLTag *HTMLBuilder::dfn() {
	write_tag();

	return tag.start("dfn");
}

HTMLTag *HTMLBuilder::dialog() {
	write_tag();

	return tag.start("dialog");
}

HTMLTag *HTMLBuilder::dir() { // Not supported in HTML5. Use <ul> instead.
	write_tag();

	return tag.start("dir");
}

HTMLTag *HTMLBuilder::div() {
	write_tag();

	return tag.start("div");
}

HTMLTag *HTMLBuilder::dl() {
	write_tag();

	return tag.start("dl");
}

HTMLTag *HTMLBuilder::dt() {
	write_tag();

	return tag.start("dt");
}

HTMLTag *HTMLBuilder::em() {
	write_tag();

	return tag.start("em");
}

HTMLTag *HTMLBuilder::embed() {
	write_tag();

	return tag.start("embed");
}

HTMLTag *HTMLBuilder::fieldset() {
	write_tag();

	return tag.start("fieldset");
}
HTMLTag *HTMLBuilder::figcaption() {
	write_tag();

	return tag.start("figcaption");
}

HTMLTag *HTMLBuilder::figure() {
	write_tag();

	return tag.start("figure");
}

HTMLTag *HTMLBuilder::font() { //Not supported in HTML5.
	write_tag();

	return tag.start("font");
}

HTMLTag *HTMLBuilder::footer() {
	write_tag();

	return tag.start("footer");
}

HTMLTag *HTMLBuilder::form() {
	write_tag();

	return tag.start("form");
}

HTMLTag *HTMLBuilder::frame() { //Not supported in HTML5.
	write_tag();

	return tag.start("frame");
}

HTMLTag *HTMLBuilder::frameset() { //Not supported in HTML5.
	write_tag();

	return tag.start("frameset");
}

HTMLTag *HTMLBuilder::h1() {
	write_tag();

	return tag.start("h1");
}

HTMLTag *HTMLBuilder::h2() {
	write_tag();

	return tag.start("h2");
}

HTMLTag *HTMLBuilder::h3() {
	write_tag();

	return tag.start("h3");
}

HTMLTag *HTMLBuilder::h4() {
	write_tag();

	return tag.start("h4");
}

HTMLTag *HTMLBuilder::h5() {
	write_tag();

	return tag.start("h5");
}

HTMLTag *HTMLBuilder::h6() {
	write_tag();

	return tag.start("h6");
}

HTMLTag *HTMLBuilder::head() {
	write_tag();

	return tag.start("head");
}

HTMLTag *HTMLBuilder::header() {
	write_tag();

	return tag.start("header");
}

HTMLTag *HTMLBuilder::hr() {
	write_tag();

	return tag.start("hr");
}

HTMLTag *HTMLBuilder::html() {
	write_tag();

	return tag.start("html");
}

HTMLTag *HTMLBuilder::i() {
	write_tag();

	return tag.start("i");
}

HTMLTag *HTMLBuilder::iframe() {
	write_tag();

	return tag.start("iframe");
}

HTMLTag *HTMLBuilder::img() {
	write_tag();

	return tag.start("img");
}

HTMLTag *HTMLBuilder::input() {
	write_tag();

	return tag.start("input");
}

HTMLTag *HTMLBuilder::ins() {
	write_tag();

	return tag.start("ins");
}

HTMLTag *HTMLBuilder::kbd() {
	write_tag();

	return tag.start("kbd");
}

HTMLTag *HTMLBuilder::label() {
	write_tag();

	return tag.start("label");
}

HTMLTag *HTMLBuilder::legend() {
	write_tag();

	return tag.start("legend");
}

HTMLTag *HTMLBuilder::li() {
	write_tag();

	return tag.start("li");
}

HTMLTag *HTMLBuilder::link() {
	write_tag();

	return tag.start("link");
}

HTMLTag *HTMLBuilder::main() {
	write_tag();

	return tag.start("main");
}

HTMLTag *HTMLBuilder::map() {
	write_tag();

	return tag.start("map");
}
HTMLTag *HTMLBuilder::mark() {
	write_tag();

	return tag.start("mark");
}

HTMLTag *HTMLBuilder::meta() {
	write_tag();

	return tag.start("meta");
}

HTMLTag *HTMLBuilder::meter() {
	write_tag();

	return tag.start("meter");
}

HTMLTag *HTMLBuilder::nav() {
	write_tag();

	return tag.start("nav");
}

HTMLTag *HTMLBuilder::noframes() { //Not supported in HTML5.
	write_tag();

	return tag.start("noframes");
}

HTMLTag *HTMLBuilder::noscript() {
	write_tag();

	return tag.start("noscript");
}

HTMLTag *HTMLBuilder::object() {
	write_tag();

	return tag.start("object");
}

HTMLTag *HTMLBuilder::ol() {
	write_tag();

	return tag.start("ol");
}

HTMLTag *HTMLBuilder::optgroup() {
	write_tag();

	return tag.start("optgroup");
}

HTMLTag *HTMLBuilder::option() {
	write_tag();

	return tag.start("option");
}

HTMLTag *HTMLBuilder::output() {
	write_tag();

	return tag.start("output");
}

HTMLTag *HTMLBuilder::p() {
	write_tag();

	return tag.start("p");
}

HTMLTag *HTMLBuilder::param() {
	write_tag();

	return tag.start("param");
}

HTMLTag *HTMLBuilder::picture() {
	write_tag();

	return tag.start("picture");
}

HTMLTag *HTMLBuilder::pre() {
	write_tag();

	return tag.start("pre");
}

HTMLTag *HTMLBuilder::progress() {
	write_tag();

	return tag.start("progress");
}

HTMLTag *HTMLBuilder::q() {
	write_tag();

	return tag.start("q");
}

HTMLTag *HTMLBuilder::rp() {
	write_tag();

	return tag.start("rp");
}

HTMLTag *HTMLBuilder::rt() {
	write_tag();

	return tag.start("rt");
}

HTMLTag *HTMLBuilder::ruby() {
	write_tag();

	return tag.start("ruby");
}

HTMLTag *HTMLBuilder::s() {
	write_tag();

	return tag.start("s");
}

HTMLTag *HTMLBuilder::samp() {
	write_tag();

	return tag.start("samp");
}

HTMLTag *HTMLBuilder::script() {
	write_tag();

	return tag.start("script");
}

HTMLTag *HTMLBuilder::section() {
	write_tag();

	return tag.start("section");
}

HTMLTag *HTMLBuilder::select() {
	write_tag();

	return tag.start("select");
}

HTMLTag *HTMLBuilder::small() {
	write_tag();

	return tag.start("small");
}

HTMLTag *HTMLBuilder::source() {
	write_tag();

	return tag.start("source");
}

HTMLTag *HTMLBuilder::span() {
	write_tag();

	return tag.start("span");
}

HTMLTag *HTMLBuilder::strike() { //Not supported in HTML5
	write_tag();

	return tag.start("strike");
}

HTMLTag *HTMLBuilder::strong() {
	write_tag();

	return tag.start("strong");
}

HTMLTag *HTMLBuilder::style() {
	write_tag();

	return tag.start("style");
}

HTMLTag *HTMLBuilder::sub() {
	write_tag();

	return tag.start("sub");
}

HTMLTag *HTMLBuilder::summary() {
	write_tag();

	return tag.start("summary");
}

HTMLTag *HTMLBuilder::sup() {
	write_tag();

	return tag.start("sup");
}

HTMLTag *HTMLBuilder::svg() {
	write_tag();

	return tag.start("svg");
}

HTMLTag *HTMLBuilder::table() {
	write_tag();

	return tag.start("table");
}

HTMLTag *HTMLBuilder::tbody() {
	write_tag();

	return tag.start("tbody");
}

HTMLTag *HTMLBuilder::td() {
	write_tag();

	return tag.start("td");
}

HTMLTag *HTMLBuilder::templateh() {
	write_tag();

	return tag.start("template");
}

HTMLTag *HTMLBuilder::textarea() {
	write_tag();

	return tag.start("textarea");
}

HTMLTag *HTMLBuilder::tfoot() {
	write_tag();

	return tag.start("tfoot");
}

HTMLTag *HTMLBuilder::th() {
	write_tag();

	return tag.start("th");
}

HTMLTag *HTMLBuilder::thead() {
	write_tag();

	return tag.start("thead");
}

HTMLTag *HTMLBuilder::time() {
	write_tag();

	return tag.start("time");
}

HTMLTag *HTMLBuilder::title() {
	write_tag();

	return tag.start("title");
}

HTMLTag *HTMLBuilder::tr() {
	write_tag();

	return tag.start("tr");
}

HTMLTag *HTMLBuilder::track() {
	write_tag();

	return tag.start("track");
}

HTMLTag *HTMLBuilder::tt() { //Not supported in HTML5.
	write_tag();

	return tag.start("tt");
}

HTMLTag *HTMLBuilder::u() {
	write_tag();

	return tag.start("u");
}

HTMLTag *HTMLBuilder::ul() {
	write_tag();

	return tag.start("ul");
}

HTMLTag *HTMLBuilder::var() {
	write_tag();

	return tag.start("var");
}

HTMLTag *HTMLBuilder::video() {
	write_tag();

	return tag.start("video");
}

HTMLTag *HTMLBuilder::wbr() {
	write_tag();

	return tag.start("wbr");
}

//Closing tags

HTMLBuilder *HTMLBuilder::ca() {
	write_tag();
	result += "</a>";

	return this;
}

HTMLBuilder *HTMLBuilder::cabbr() {
	write_tag();
	result += "</abbr>";

	return this;
}

HTMLBuilder *HTMLBuilder::cacronym() {
	write_tag();
	result += "</acronym>";

	return this;
}

HTMLBuilder *HTMLBuilder::caddress() {
	write_tag();
	result += "</address>";

	return this;
}

HTMLBuilder *HTMLBuilder::capplet() {
	write_tag();
	result += "</applet>";

	return this;
}

HTMLBuilder *HTMLBuilder::carea() {
	write_tag();
	result += "</area>";

	return this;
}

HTMLBuilder *HTMLBuilder::carticle() {
	write_tag();
	result += "</article>";

	return this;
}

HTMLBuilder *HTMLBuilder::caside() {
	write_tag();
	result += "</aside>";

	return this;
}

HTMLBuilder *HTMLBuilder::caudio() {
	write_tag();
	result += "</audio>";

	return this;
}

HTMLBuilder *HTMLBuilder::cb() {
	write_tag();
	result += "</b>";

	return this;
}

HTMLBuilder *HTMLBuilder::cbasefont() {
	write_tag();
	result += "</basefont>";

	return this;
}

HTMLBuilder *HTMLBuilder::cbdi() {
	write_tag();
	result += "</bdi>";

	return this;
}

HTMLBuilder *HTMLBuilder::cbdo() {
	write_tag();
	result += "</bdo>";

	return this;
}

HTMLBuilder *HTMLBuilder::cbig() {
	write_tag();
	result += "</big>";

	return this;
}

HTMLBuilder *HTMLBuilder::cblockquote() {
	write_tag();
	result += "</blockquote>";

	return this;
}

HTMLBuilder *HTMLBuilder::cbody() {
	write_tag();
	result += "</body>";

	return this;
}

HTMLBuilder *HTMLBuilder::cbutton() {
	write_tag();
	result += "</button>";

	return this;
}

HTMLBuilder *HTMLBuilder::ccanvas() {
	write_tag();
	result += "</canvas>";

	return this;
}

HTMLBuilder *HTMLBuilder::ccaption() {
	write_tag();
	result += "</caption>";

	return this;
}

HTMLBuilder *HTMLBuilder::ccenter() {
	write_tag();
	result += "</center>";

	return this;
}

HTMLBuilder *HTMLBuilder::ccite() {
	write_tag();
	result += "</cite>";

	return this;
}

HTMLBuilder *HTMLBuilder::ccode() {
	write_tag();
	result += "</code>";

	return this;
}

HTMLBuilder *HTMLBuilder::ccol() {
	write_tag();
	result += "</col>";

	return this;
}

HTMLBuilder *HTMLBuilder::ccolgroup() {
	write_tag();
	result += "</colgroup>";

	return this;
}

HTMLBuilder *HTMLBuilder::cdata() {
	write_tag();
	result += "</data>";

	return this;
}

HTMLBuilder *HTMLBuilder::cdatalist() {
	write_tag();
	result += "</datalist>";

	return this;
}

HTMLBuilder *HTMLBuilder::cdd() {
	write_tag();
	result += "</dd>";

	return this;
}

HTMLBuilder *HTMLBuilder::cdel() {
	write_tag();
	result += "</del>";

	return this;
}

HTMLBuilder *HTMLBuilder::cdetails() {
	write_tag();
	result += "</details>";

	return this;
}

HTMLBuilder *HTMLBuilder::cdfn() {
	write_tag();
	result += "</dfn>";

	return this;
}

HTMLBuilder *HTMLBuilder::cdialog() {
	write_tag();
	result += "</dialog>";

	return this;
}

HTMLBuilder *HTMLBuilder::cdir() {
	write_tag();
	result += "</dir>";

	return this;
}

HTMLBuilder *HTMLBuilder::cdiv() {
	write_tag();
	result += "</div>";

	return this;
}

HTMLBuilder *HTMLBuilder::cdl() {
	write_tag();
	result += "</dl>";

	return this;
}

HTMLBuilder *HTMLBuilder::cdt() {
	write_tag();
	result += "</dt>";

	return this;
}

HTMLBuilder *HTMLBuilder::cem() {
	write_tag();
	result += "</em>";

	return this;
}

HTMLBuilder *HTMLBuilder::cembed() {
	write_tag();
	result += "</embed>";

	return this;
}

HTMLBuilder *HTMLBuilder::cfieldset() {
	write_tag();
	result += "</fieldset>";

	return this;
}

HTMLBuilder *HTMLBuilder::cfigcaption() {
	write_tag();
	result += "</figcaption>";

	return this;
}

HTMLBuilder *HTMLBuilder::cfigure() {
	write_tag();
	result += "</figure>";

	return this;
}

HTMLBuilder *HTMLBuilder::cfont() {
	write_tag();
	result += "</font>";

	return this;
}

HTMLBuilder *HTMLBuilder::cfooter() {
	write_tag();
	result += "</footer>";

	return this;
}

HTMLBuilder *HTMLBuilder::cform() {
	write_tag();
	result += "</form>";

	return this;
}

HTMLBuilder *HTMLBuilder::cframe() {
	write_tag();
	result += "</frame>";

	return this;
}

HTMLBuilder *HTMLBuilder::cframeset() {
	write_tag();
	result += "</frameset>";

	return this;
}

HTMLBuilder *HTMLBuilder::ch1() {
	write_tag();
	result += "</h1>";

	return this;
}

HTMLBuilder *HTMLBuilder::ch2() {
	write_tag();
	result += "</h2>";

	return this;
}

HTMLBuilder *HTMLBuilder::ch3() {
	write_tag();
	result += "</h3>";

	return this;
}

HTMLBuilder *HTMLBuilder::ch4() {
	write_tag();
	result += "</h4>";

	return this;
}

HTMLBuilder *HTMLBuilder::ch5() {
	write_tag();
	result += "</h5>";

	return this;
}

HTMLBuilder *HTMLBuilder::ch6() {
	write_tag();
	result += "</h6>";

	return this;
}

HTMLBuilder *HTMLBuilder::chead() {
	write_tag();
	result += "</head>";

	return this;
}

HTMLBuilder *HTMLBuilder::cheader() {
	write_tag();
	result += "</header>";

	return this;
}

HTMLBuilder *HTMLBuilder::chr() {
	write_tag();
	result += "</hr>";

	return this;
}

HTMLBuilder *HTMLBuilder::chtml() {
	write_tag();
	result += "</html>";

	return this;
}

HTMLBuilder *HTMLBuilder::ci() {
	write_tag();
	result += "</i>";

	return this;
}

HTMLBuilder *HTMLBuilder::ciframe() {
	write_tag();
	result += "</iframe>";

	return this;
}

HTMLBuilder *HTMLBuilder::cimg() {
	write_tag();
	result += "</img>";

	return this;
}

HTMLBuilder *HTMLBuilder::cinput() {
	write_tag();
	result += "</input>";

	return this;
}

HTMLBuilder *HTMLBuilder::cins() {
	write_tag();
	result += "</ins>";

	return this;
}

HTMLBuilder *HTMLBuilder::ckbd() {
	write_tag();
	result += "</kbd>";

	return this;
}

HTMLBuilder *HTMLBuilder::clabel() {
	write_tag();
	result += "</label>";

	return this;
}

HTMLBuilder *HTMLBuilder::clegend() {
	write_tag();
	result += "</legend>";

	return this;
}

HTMLBuilder *HTMLBuilder::cli() {
	write_tag();
	result += "</li>";

	return this;
}

HTMLBuilder *HTMLBuilder::clink() {
	write_tag();
	result += "</link>";

	return this;
}

HTMLBuilder *HTMLBuilder::cmain() {
	write_tag();
	result += "</main>";

	return this;
}

HTMLBuilder *HTMLBuilder::cmap() {
	write_tag();
	result += "</map>";

	return this;
}

HTMLBuilder *HTMLBuilder::cmark() {
	write_tag();
	result += "</mark>";

	return this;
}

HTMLBuilder *HTMLBuilder::cmeta() {
	write_tag();
	result += "</meta>";

	return this;
}

HTMLBuilder *HTMLBuilder::cmeter() {
	write_tag();
	result += "</meter>";

	return this;
}

HTMLBuilder *HTMLBuilder::cnav() {
	write_tag();
	result += "</nav>";

	return this;
}

HTMLBuilder *HTMLBuilder::cnoframes() {
	write_tag();
	result += "</noframes>";

	return this;
}

HTMLBuilder *HTMLBuilder::cnoscript() {
	write_tag();
	result += "</noscript>";

	return this;
}

HTMLBuilder *HTMLBuilder::cobject() {
	write_tag();
	result += "</object>";

	return this;
}

HTMLBuilder *HTMLBuilder::c_ol() {
	write_tag();
	result += "</ol>";

	return this;
}

HTMLBuilder *HTMLBuilder::coptgroup() {
	write_tag();
	result += "</optgroup>";

	return this;
}

HTMLBuilder *HTMLBuilder::coption() {
	write_tag();
	result += "</option>";

	return this;
}

HTMLBuilder *HTMLBuilder::coutput() {
	write_tag();
	result += "</output>";

	return this;
}

HTMLBuilder *HTMLBuilder::cp() {
	write_tag();
	result += "</p>";

	return this;
}

HTMLBuilder *HTMLBuilder::cparam() {
	write_tag();
	result += "</param>";

	return this;
}

HTMLBuilder *HTMLBuilder::cpicture() {
	write_tag();
	result += "</picture>";

	return this;
}

HTMLBuilder *HTMLBuilder::cpre() {
	write_tag();
	result += "</pre>";

	return this;
}

HTMLBuilder *HTMLBuilder::cprogress() {
	write_tag();
	result += "</progress>";

	return this;
}

HTMLBuilder *HTMLBuilder::cq() {
	write_tag();
	result += "</a>";

	return this;
}

HTMLBuilder *HTMLBuilder::crp() {
	write_tag();
	result += "</rp>";

	return this;
}

HTMLBuilder *HTMLBuilder::crt() {
	write_tag();
	result += "</rt>";

	return this;
}

HTMLBuilder *HTMLBuilder::cruby() {
	write_tag();
	result += "</ruby>";

	return this;
}

HTMLBuilder *HTMLBuilder::cs() {
	write_tag();
	result += "</s>";

	return this;
}

HTMLBuilder *HTMLBuilder::csamp() {
	write_tag();
	result += "</samp>";

	return this;
}

HTMLBuilder *HTMLBuilder::cscript() {
	write_tag();
	result += "</script>";

	return this;
}

HTMLBuilder *HTMLBuilder::csection() {
	write_tag();
	result += "</section>";

	return this;
}

HTMLBuilder *HTMLBuilder::cselect() {
	write_tag();
	result += "</select>";

	return this;
}

HTMLBuilder *HTMLBuilder::csmall() {
	write_tag();
	result += "</small>";

	return this;
}

HTMLBuilder *HTMLBuilder::csource() {
	write_tag();
	result += "</source>";

	return this;
}

HTMLBuilder *HTMLBuilder::cspan() {
	write_tag();
	result += "</span>";

	return this;
}

HTMLBuilder *HTMLBuilder::cstrike() {
	write_tag();
	result += "</strike>";

	return this;
}

HTMLBuilder *HTMLBuilder::cstrong() {
	write_tag();
	result += "</strong>";

	return this;
}

HTMLBuilder *HTMLBuilder::cstyle() {
	write_tag();
	result += "</style>";

	return this;
}

HTMLBuilder *HTMLBuilder::csub() {
	write_tag();
	result += "</sub>";

	return this;
}

HTMLBuilder *HTMLBuilder::csummary() {
	write_tag();
	result += "</summary>";

	return this;
}

HTMLBuilder *HTMLBuilder::csup() {
	write_tag();
	result += "</sup>";

	return this;
}

HTMLBuilder *HTMLBuilder::csvg() {
	write_tag();
	result += "</svg>";

	return this;
}

HTMLBuilder *HTMLBuilder::ctable() {
	write_tag();
	result += "</table>";

	return this;
}

HTMLBuilder *HTMLBuilder::ctbody() {
	write_tag();
	result += "</tbody>";

	return this;
}

HTMLBuilder *HTMLBuilder::ctd() {
	write_tag();
	result += "</td>";

	return this;
}

HTMLBuilder *HTMLBuilder::ctemplateh() {
	write_tag();
	result += "</templateh>";

	return this;
}

HTMLBuilder *HTMLBuilder::ctextarea() {
	write_tag();
	result += "</textarea>";

	return this;
}

HTMLBuilder *HTMLBuilder::ctfoot() {
	write_tag();
	result += "</tfoot>";

	return this;
}

HTMLBuilder *HTMLBuilder::cth() {
	write_tag();
	result += "</th>";

	return this;
}

HTMLBuilder *HTMLBuilder::cthead() {
	write_tag();
	result += "</thead>";

	return this;
}

HTMLBuilder *HTMLBuilder::ctime() {
	write_tag();
	result += "</time>";

	return this;
}

HTMLBuilder *HTMLBuilder::ctitle() {
	write_tag();
	result += "</title>";

	return this;
}

HTMLBuilder *HTMLBuilder::ctr() {
	write_tag();
	result += "</tr>";

	return this;
}

HTMLBuilder *HTMLBuilder::ctrack() {
	write_tag();
	result += "</track>";

	return this;
}

HTMLBuilder *HTMLBuilder::ctt() {
	write_tag();
	result += "</tt>";

	return this;
}

HTMLBuilder *HTMLBuilder::cu() {
	write_tag();
	result += "</u>";

	return this;
}

HTMLBuilder *HTMLBuilder::cul() {
	write_tag();
	result += "</ul>";

	return this;
}

HTMLBuilder *HTMLBuilder::cvar() {
	write_tag();
	result += "</var>";

	return this;
}

HTMLBuilder *HTMLBuilder::cvideo() {
	write_tag();
	result += "</video>";

	return this;
}

HTMLBuilder *HTMLBuilder::cwbr() {
	write_tag();
	result += "</wbr>";

	return this;
}

HTMLTag *HTMLBuilder::input_button() {
	write_tag();

	return tag.start("input")->itbutton();
}

HTMLTag *HTMLBuilder::input_checkbox() {
	write_tag();

	return tag.start("input")->itcheckbox();
}

HTMLTag *HTMLBuilder::input_color() {
	write_tag();

	return tag.start("input")->itcolor();
}

HTMLTag *HTMLBuilder::input_date() {
	write_tag();

	return tag.start("input")->itdate();
}

HTMLTag *HTMLBuilder::input_datetime_local() {
	write_tag();

	return tag.start("input")->itdatetime_local();
}

HTMLTag *HTMLBuilder::input_email() {
	write_tag();

	return tag.start("input")->itemail();
}

HTMLTag *HTMLBuilder::input_file() {
	write_tag();

	return tag.start("input")->itfile();
}

HTMLTag *HTMLBuilder::input_hidden() {
	write_tag();

	return tag.start("input")->ithidden();
}

HTMLTag *HTMLBuilder::input_image() {
	write_tag();

	return tag.start("input")->itimage();
}

HTMLTag *HTMLBuilder::input_month() {
	write_tag();

	return tag.start("input")->itmonth();
}

HTMLTag *HTMLBuilder::input_number() {
	write_tag();

	return tag.start("input")->itnumber();
}

HTMLTag *HTMLBuilder::input_password() {
	write_tag();

	return tag.start("input")->itpassword();
}

HTMLTag *HTMLBuilder::input_radio() {
	write_tag();

	return tag.start("input")->itradio();
}

HTMLTag *HTMLBuilder::input_range() {
	write_tag();

	return tag.start("input")->itrange();
}

HTMLTag *HTMLBuilder::input_reset() {
	write_tag();

	return tag.start("input")->itreset();
}

HTMLTag *HTMLBuilder::input_search() {
	write_tag();

	return tag.start("input")->itsearch();
}

HTMLTag *HTMLBuilder::input_submit() {
	write_tag();

	return tag.start("input")->itsubmit();
}

HTMLTag *HTMLBuilder::input_tel() {
	write_tag();

	return tag.start("input")->ittel();
}

HTMLTag *HTMLBuilder::input_text() {
	write_tag();

	return tag.start("input")->ittext();
}

HTMLTag *HTMLBuilder::input_time() {
	write_tag();

	return tag.start("input")->ittime();
}

HTMLTag *HTMLBuilder::input_url() {
	write_tag();

	return tag.start("input")->iturl();
}

HTMLTag *HTMLBuilder::input_week() {
	write_tag();

	return tag.start("input")->itweek();
}

HTMLBuilder *HTMLBuilder::label(const String& pfor, const String& plabel, const String& cls, const String& id) {
	HTMLTag *t = label();

	t->fora(pfor);

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	w(plabel);

	clabel();

	return this;
}

HTMLTag *HTMLBuilder::input_button(const String &name, const String &value, const String &cls, const String &id) {
	HTMLTag *t = input_button();

	t->name(name);

	if (value != "") {
		t->value(value);
	}

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	return t;
}

HTMLTag *HTMLBuilder::input_checkbox(const String &name, const String &value, const String &cls, const String &id) {
	HTMLTag *t = input_checkbox();

	t->name(name);

	if (value != "") {
		t->value(value);
	}

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	return t;
}

HTMLTag *HTMLBuilder::input_color(const String &name, const String &value, const String &cls, const String &id) {
	HTMLTag *t = input_color();

	t->name(name);

	if (value != "") {
		t->value(value);
	}

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	return t;
}

HTMLTag *HTMLBuilder::input_date(const String &name, const String &value, const String &cls, const String &id, const String &date_min, const String &date_max, const String &date_step) {
	HTMLTag *t = input_date();

	t->name(name);

	if (value != "") {
		t->value(value);
	}

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	if (date_min != "") {
		t->min(date_min);
	}

	if (date_max != "") {
		t->max(date_max);
	}

	if (date_step != "") {
		t->step(date_step);
	}

	return t;
}

HTMLTag *HTMLBuilder::input_datetime_local(const String &name, const String &value, const String &cls, const String &id, const String &date_min, const String &date_max, const String &date_step) {
	HTMLTag *t = input_datetime_local();

	t->name(name);

	if (value != "") {
		t->value(value);
	}

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	if (date_min != "") {
		t->min(date_min);
	}

	if (date_max != "") {
		t->max(date_max);
	}

	if (date_step != "") {
		t->step(date_step);
	}

	return t;
}

HTMLTag *HTMLBuilder::input_email(const String &name, const String &value, const String &placeholder, const String &cls, const String &id) {
	HTMLTag *t = input_email();

	t->name(name);

	if (value != "") {
		t->value(value);
	}

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	if (placeholder != "") {
		t->placeholder(placeholder);
	}

	return t;
}

HTMLTag *HTMLBuilder::input_file(const String &name, const String &accept, const String &cls, const String &id) {
	HTMLTag *t = input_email();

	t->name(name);

	if (accept != "") {
		t->accept(accept);
	}

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	return t;
}

HTMLTag *HTMLBuilder::input_image(const String &name, const String &src, const String &alt, const String &cls, const String &id, const int width, const int height) {
	HTMLTag *t = input_image();

	t->name(name);

	if (src != "") {
		t->src(src);
	}

	if (alt != "") {
		t->alt(alt);
	}

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	if (width != 0) {
		t->width(width);
	}

	if (height != 0) {
		t->height(height);
	}

	return t;
}

HTMLTag *HTMLBuilder::input_month(const String &name, const String &cls, const String &id) {
	HTMLTag *t = input_month();

	t->name(name);

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	return t;
}

HTMLTag *HTMLBuilder::input_number(const String &name, const String &vmin, const String &vmax, const String &cls, const String &id) {
	HTMLTag *t = input_number();

	t->name(name);

	if (vmin != "") {
		t->min(vmin);
	}

	if (vmax != "") {
		t->max(vmax);
	}

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	return t;
}

HTMLTag *HTMLBuilder::input_password(const String &name, const String &value, const String &placeholder, const String &cls, const String &id, const String &minlength, const String &maxlength, const String &size) {
	HTMLTag *t = input_password();

	t->name(name);

	if (value != "") {
		t->value(value);
	}

	if (placeholder != "") {
		t->placeholder(placeholder);
	}

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	if (minlength != "") {
		t->minlength(minlength);
	}

	if (maxlength != "") {
		t->maxlength(maxlength);
	}

	if (size != "") {
		t->size(size);
	}

	return t;
}

HTMLTag *HTMLBuilder::input_radio(const String &name, const String &value, const String &cls, const String &id) {
	HTMLTag *t = input_password();

	t->name(name);

	if (value != "") {
		t->value(value);
	}

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	return t;
}

HTMLTag *HTMLBuilder::input_range(const String &name, const String &value, const String &vmin, const String &vmax, const String &vstep, const String &cls, const String &id) {
	HTMLTag *t = input_range();

	t->name(name);

	if (value != "") {
		t->value(value);
	}

	if (vmin != "") {
		t->min(vmin);
	}

	if (vmax != "") {
		t->max(vmax);
	}

	if (vstep != "") {
		t->step(vstep);
	}

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	return t;
}

HTMLTag *HTMLBuilder::input_reset(const String &name, const String &value, const String &cls, const String &id) {
	HTMLTag *t = input_reset();

	t->name(name);

	if (value != "") {
		t->value(value);
	}

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	return t;
}

HTMLTag *HTMLBuilder::input_search(const String &name, const String &value, const String &placeholder, const String &cls, const String &id, const String &minlength, const String &maxlength, const String &size, const String &pattern) {
	HTMLTag *t = input_search();

	t->name(name);

	if (value != "") {
		t->value(value);
	}

	if (placeholder != "") {
		t->placeholder(placeholder);
	}

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	if (minlength != "") {
		t->minlength(minlength);
	}

	if (maxlength != "") {
		t->maxlength(maxlength);
	}

	if (size != "") {
		t->size(size);
	}

	if (pattern != "") {
		t->pattern(pattern);
	}

	return t;
}

HTMLTag *HTMLBuilder::input_submit(const String &value, const String &cls, const String &id) {
	HTMLTag *t = input_submit();

	t->value(value);

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	return t;
}

HTMLTag *HTMLBuilder::input_tel(const String &name, const String &value, const String &placeholder, const String &cls, const String &id, const String &minlength, const String &maxlength, const String &size, const String &pattern) {
	HTMLTag *t = input_tel();

	t->name(name);

	if (value != "") {
		t->value(value);
	}

	if (placeholder != "") {
		t->placeholder(placeholder);
	}

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	if (minlength != "") {
		t->minlength(minlength);
	}

	if (maxlength != "") {
		t->maxlength(maxlength);
	}

	if (size != "") {
		t->size(size);
	}

	if (pattern != "") {
		t->pattern(pattern);
	}

	return t;
}

HTMLTag *HTMLBuilder::input_text(const String &name, const String &value, const String &placeholder, const String &cls, const String &id, const String &minlength, const String &maxlength, const String &size) {
	HTMLTag *t = input_text();

	t->name(name);

	if (value != "") {
		t->value(value);
	}

	if (placeholder != "") {
		t->placeholder(placeholder);
	}

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	if (minlength != "") {
		t->minlength(minlength);
	}

	if (maxlength != "") {
		t->maxlength(maxlength);
	}

	if (size != "") {
		t->size(size);
	}

	return t;
}

HTMLTag *HTMLBuilder::input_time(const String &name, const String &cls, const String &id, const String &vmin, const String &vmax, const String &vstep) {
	HTMLTag *t = input_time();

	t->name(name);

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	if (vmin != "") {
		t->min(vmin);
	}

	if (vmax != "") {
		t->max(vmax);
	}

	if (vstep != "") {
		t->step(vstep);
	}

	return t;
}

HTMLTag *HTMLBuilder::input_url(const String &name, const String &value, const String &placeholder, const String &cls, const String &id, const String &minlength, const String &maxlength, const String &size) {
	HTMLTag *t = input_url();

	t->name(name);

	if (value != "") {
		t->value(value);
	}

	if (placeholder != "") {
		t->placeholder(placeholder);
	}

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	if (minlength != "") {
		t->minlength(minlength);
	}

	if (maxlength != "") {
		t->maxlength(maxlength);
	}

	if (size != "") {
		t->size(size);
	}

	return t;
}

HTMLTag *HTMLBuilder::input_week(const String& name, const String& cls, const String& id, const String& vmin, const String& vmax) {
	HTMLTag *t = input_week();

	t->name(name);

	if (cls != "") {
		t->cls(cls);
	}

	if (id != "") {
		t->id(id);
	}

	if (vmin != "") {
		t->min(vmin);
	}

	if (vmax != "") {
		t->max(vmax);
	}

	return t;
}


void HTMLBuilder::f() {
	write_tag();
}

HTMLBuilder *HTMLBuilder::w(const String &val) {
	write_tag();

	result += val;

	return this;
}

HTMLBuilder *HTMLBuilder::wn(const double val, int p_decimals) {
	write_tag();

	result += String::num(val, p_decimals);

	return this;
}
HTMLBuilder *HTMLBuilder::wns(const double val) {
	write_tag();

	result += String::num_scientific(val);

	return this;
}
HTMLBuilder *HTMLBuilder::wr(const double val, const bool p_trailing) {
	write_tag();

	result += String::num_real(val, p_trailing);

	return this;
}
HTMLBuilder *HTMLBuilder::wi(const int64_t val, const int base, const bool capitalize_hex) {
	write_tag();

	result += String::num_int64(val, base, capitalize_hex);

	return this;
}
HTMLBuilder *HTMLBuilder::wui(const uint64_t val, const int base, const bool capitalize_hex) {
	write_tag();

	result += String::num_uint64(val, base, capitalize_hex);

	return this;
}

HTMLBuilder *HTMLBuilder::wbn(const bool val) {
	write_tag();

	result += String::bool_num(val);

	return this;
}
HTMLBuilder *HTMLBuilder::wbs(const bool val) {
	write_tag();

	result += String::bool_str(val);

	return this;
}

//TODO!
HTMLBuilder *HTMLBuilder::we(const String &val) {
	printf("HTMLBuilder::write_excaped NYI!");

	write_tag();

	result += val;

	return this;
}

HTMLBuilder *HTMLBuilder::write_tag() {
	if (tag.has_data()) {
		tag.close();
		result += tag.result;
		tag.reset();
	}

	return this;
}

HTMLBuilder::HTMLBuilder() {
	tag.owner = this;
}

HTMLBuilder::~HTMLBuilder() {
}