#include "html_builder.h"

HTMLTag *HTMLTag::str(const std::string &str) {
	result += " " + str;

	return this;
}

HTMLTag *HTMLTag::style(const std::string &val) {
	attrib("style", val);

	return this;
}

HTMLTag *HTMLTag::href(const std::string &val) {
	attrib("href", val);

	return this;
}

HTMLTag *HTMLTag::cls(const std::string &val) {
	attrib("class", val);

	return this;
}
HTMLTag *HTMLTag::id(const std::string &val) {
	attrib("id", val);

	return this;
}
HTMLTag *HTMLTag::name(const std::string &val) {
	attrib("name", val);

	return this;
}

HTMLTag *HTMLTag::method(const std::string &val) {
	attrib("method", val);

	return this;
}

HTMLTag *HTMLTag::type(const std::string &val) {
	attrib("type", val);

	return this;
}

HTMLTag *HTMLTag::attrib(const std::string &attr, const std::string &val) {
	result += " " + attr + "=\"" + val + "\"";

	return this;
}

HTMLTag *HTMLTag::start(const std::string &p_tag, const bool p_simple) {
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

bool HTMLTag::has_data() {
	return result.size() > 0;
}

HTMLTag::HTMLTag() {
	simple = true;
}

void HTMLBuilder::comment(const std::string &val) {
	write_tag();

	result += "<!--" + val + "-->";
}

HTMLTag *HTMLBuilder::doctype() {
	write_tag();

	return tag.start("!DOCTYPE");
}

void HTMLBuilder::doctype(const std::string &val) {
	write_tag();

	result += "<!DOCTYPE " + val + ">";
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

void HTMLBuilder::ca() {
	write_tag();
	result += "</a>";
}

void HTMLBuilder::cabbr() {
	write_tag();
	result += "</abbr>";
}

void HTMLBuilder::cacronym() {
	write_tag();
	result += "</acronym>";
}

void HTMLBuilder::caddress() {
	write_tag();
	result += "</address>";
}

void HTMLBuilder::capplet() {
	write_tag();
	result += "</applet>";
}

void HTMLBuilder::carea() {
	write_tag();
	result += "</area>";
}

void HTMLBuilder::carticle() {
	write_tag();
	result += "</article>";
}

void HTMLBuilder::caside() {
	write_tag();
	result += "</aside>";
}

void HTMLBuilder::caudio() {
	write_tag();
	result += "</audio>";
}

void HTMLBuilder::cb() {
	write_tag();
	result += "</b>";
}

void HTMLBuilder::cbasefont() {
	write_tag();
	result += "</basefont>";
}

void HTMLBuilder::cbdi() {
	write_tag();
	result += "</bdi>";
}

void HTMLBuilder::cbdo() {
	write_tag();
	result += "</bdo>";
}

void HTMLBuilder::cbig() {
	write_tag();
	result += "</big>";
}

void HTMLBuilder::cblockquote() {
	write_tag();
	result += "</blockquote>";
}

void HTMLBuilder::cbody() {
	write_tag();
	result += "</body>";
}

void HTMLBuilder::cbutton() {
	write_tag();
	result += "</button>";
}

void HTMLBuilder::ccanvas() {
	write_tag();
	result += "</canvas>";
}

void HTMLBuilder::ccaption() {
	write_tag();
	result += "</caption>";
}

void HTMLBuilder::ccenter() {
	write_tag();
	result += "</center>";
}

void HTMLBuilder::ccite() {
	write_tag();
	result += "</cite>";
}

void HTMLBuilder::ccode() {
	write_tag();
	result += "</code>";
}

void HTMLBuilder::ccol() {
	write_tag();
	result += "</col>";
}

void HTMLBuilder::ccolgroup() {
	write_tag();
	result += "</colgroup>";
}

void HTMLBuilder::cdata() {
	write_tag();
	result += "</data>";
}

void HTMLBuilder::cdatalist() {
	write_tag();
	result += "</datalist>";
}

void HTMLBuilder::cdd() {
	write_tag();
	result += "</dd>";
}

void HTMLBuilder::cdel() {
	write_tag();
	result += "</del>";
}

void HTMLBuilder::cdetails() {
	write_tag();
	result += "</details>";
}

void HTMLBuilder::cdfn() {
	write_tag();
	result += "</dfn>";
}

void HTMLBuilder::cdialog() {
	write_tag();
	result += "</dialog>";
}

void HTMLBuilder::cdir() {
	write_tag();
	result += "</dir>";
}

void HTMLBuilder::cdiv() {
	write_tag();
	result += "</div>";
}

void HTMLBuilder::cdl() {
	write_tag();
	result += "</dl>";
}

void HTMLBuilder::cdt() {
	write_tag();
	result += "</dt>";
}

void HTMLBuilder::cem() {
	write_tag();
	result += "</em>";
}

void HTMLBuilder::cembed() {
	write_tag();
	result += "</embed>";
}

void HTMLBuilder::cfieldset() {
	write_tag();
	result += "</fieldset>";
}

void HTMLBuilder::cfigcaption() {
	write_tag();
	result += "</figcaption>";
}

void HTMLBuilder::cfigure() {
	write_tag();
	result += "</figure>";
}

void HTMLBuilder::cfont() {
	write_tag();
	result += "</font>";
}

void HTMLBuilder::cfooter() {
	write_tag();
	result += "</footer>";
}

void HTMLBuilder::cform() {
	write_tag();
	result += "</form>";
}

void HTMLBuilder::cframe() {
	write_tag();
	result += "</frame>";
}

void HTMLBuilder::cframeset() {
	write_tag();
	result += "</frameset>";
}

void HTMLBuilder::ch1() {
	write_tag();
	result += "</h1>";
}

void HTMLBuilder::ch2() {
	write_tag();
	result += "</h2>";
}

void HTMLBuilder::ch3() {
	write_tag();
	result += "</h3>";
}

void HTMLBuilder::ch4() {
	write_tag();
	result += "</h4>";
}

void HTMLBuilder::ch5() {
	write_tag();
	result += "</h5>";
}

void HTMLBuilder::ch6() {
	write_tag();
	result += "</h6>";
}

void HTMLBuilder::chead() {
	write_tag();
	result += "</head>";
}

void HTMLBuilder::cheader() {
	write_tag();
	result += "</header>";
}

void HTMLBuilder::chr() {
	write_tag();
	result += "</hr>";
}

void HTMLBuilder::chtml() {
	write_tag();
	result += "</html>";
}

void HTMLBuilder::ci() {
	write_tag();
	result += "</i>";
}

void HTMLBuilder::ciframe() {
	write_tag();
	result += "</iframe>";
}

void HTMLBuilder::cimg() {
	write_tag();
	result += "</img>";
}

void HTMLBuilder::cinput() {
	write_tag();
	result += "</input>";
}

void HTMLBuilder::cins() {
	write_tag();
	result += "</ins>";
}

void HTMLBuilder::ckbd() {
	write_tag();
	result += "</kbd>";
}

void HTMLBuilder::clabel() {
	write_tag();
	result += "</label>";
}

void HTMLBuilder::clegend() {
	write_tag();
	result += "</legend>";
}

void HTMLBuilder::cli() {
	write_tag();
	result += "</li>";
}

void HTMLBuilder::clink() {
	write_tag();
	result += "</link>";
}

void HTMLBuilder::cmain() {
	write_tag();
	result += "</main>";
}

void HTMLBuilder::cmap() {
	write_tag();
	result += "</map>";
}

void HTMLBuilder::cmark() {
	write_tag();
	result += "</mark>";
}

void HTMLBuilder::cmeta() {
	write_tag();
	result += "</meta>";
}

void HTMLBuilder::cmeter() {
	write_tag();
	result += "</meter>";
}

void HTMLBuilder::cnav() {
	write_tag();
	result += "</nav>";
}

void HTMLBuilder::cnoframes() {
	write_tag();
	result += "</noframes>";
}

void HTMLBuilder::cnoscript() {
	write_tag();
	result += "</noscript>";
}

void HTMLBuilder::cobject() {
	write_tag();
	result += "</object>";
}

void HTMLBuilder::c_ol() {
	write_tag();
	result += "</ol>";
}

void HTMLBuilder::coptgroup() {
	write_tag();
	result += "</optgroup>";
}

void HTMLBuilder::coption() {
	write_tag();
	result += "</option>";
}

void HTMLBuilder::coutput() {
	write_tag();
	result += "</output>";
}

void HTMLBuilder::cp() {
	write_tag();
	result += "</p>";
}

void HTMLBuilder::cparam() {
	write_tag();
	result += "</param>";
}

void HTMLBuilder::cpicture() {
	write_tag();
	result += "</picture>";
}

void HTMLBuilder::cpre() {
	write_tag();
	result += "</pre>";
}

void HTMLBuilder::cprogress() {
	write_tag();
	result += "</progress>";
}

void HTMLBuilder::cq() {
	write_tag();
	result += "</a>";
}

void HTMLBuilder::crp() {
	write_tag();
	result += "</rp>";
}

void HTMLBuilder::crt() {
	write_tag();
	result += "</rt>";
}

void HTMLBuilder::cruby() {
	write_tag();
	result += "</ruby>";
}

void HTMLBuilder::cs() {
	write_tag();
	result += "</s>";
}

void HTMLBuilder::csamp() {
	write_tag();
	result += "</samp>";
}

void HTMLBuilder::cscript() {
	write_tag();
	result += "</script>";
}

void HTMLBuilder::csection() {
	write_tag();
	result += "</section>";
}

void HTMLBuilder::cselect() {
	write_tag();
	result += "</select>";
}

void HTMLBuilder::csmall() {
	write_tag();
	result += "</small>";
}

void HTMLBuilder::csource() {
	write_tag();
	result += "</source>";
}

void HTMLBuilder::cspan() {
	write_tag();
	result += "</span>";
}

void HTMLBuilder::cstrike() {
	write_tag();
	result += "</strike>";
}

void HTMLBuilder::cstrong() {
	write_tag();
	result += "</strong>";
}

void HTMLBuilder::cstyle() {
	write_tag();
	result += "</style>";
}

void HTMLBuilder::csub() {
	write_tag();
	result += "</sub>";
}

void HTMLBuilder::csummary() {
	write_tag();
	result += "</summary>";
}

void HTMLBuilder::csup() {
	write_tag();
	result += "</sup>";
}

void HTMLBuilder::csvg() {
	write_tag();
	result += "</svg>";
}

void HTMLBuilder::ctable() {
	write_tag();
	result += "</table>";
}

void HTMLBuilder::ctbody() {
	write_tag();
	result += "</tbody>";
}

void HTMLBuilder::ctd() {
	write_tag();
	result += "</td>";
}

void HTMLBuilder::ctemplateh() {
	write_tag();
	result += "</templateh>";
}

void HTMLBuilder::ctextarea() {
	write_tag();
	result += "</textarea>";
}

void HTMLBuilder::ctfoot() {
	write_tag();
	result += "</tfoot>";
}

void HTMLBuilder::cth() {
	write_tag();
	result += "</th>";
}

void HTMLBuilder::cthead() {
	write_tag();
	result += "</thead>";
}

void HTMLBuilder::ctime() {
	write_tag();
	result += "</time>";
}

void HTMLBuilder::ctitle() {
	write_tag();
	result += "</title>";
}

void HTMLBuilder::ctr() {
	write_tag();
	result += "</tr>";
}

void HTMLBuilder::ctrack() {
	write_tag();
	result += "</track>";
}

void HTMLBuilder::ctt() {
	write_tag();
	result += "</tt>";
}

void HTMLBuilder::cu() {
	write_tag();
	result += "</u>";
}

void HTMLBuilder::cul() {
	write_tag();
	result += "</ul>";
}

void HTMLBuilder::cvar() {
	write_tag();
	result += "</var>";
}

void HTMLBuilder::cvideo() {
	write_tag();
	result += "</video>";
}

void HTMLBuilder::cwbr() {
	write_tag();
	result += "</wbr>";
}

void HTMLBuilder::w(const std::string &val) {
	write_tag();

	result += val;
}

//TODO!
void HTMLBuilder::we(const std::string &val) {
	printf("HTMLBuilder::write_excaped NYI!");

	write_tag();

	result += val;
}

void HTMLBuilder::write_tag() {
	if (tag.has_data()) {
		tag.close();
		result += tag.result;
		tag.reset();
	}
}

HTMLBuilder::HTMLBuilder() {
}

HTMLBuilder::~HTMLBuilder() {
}