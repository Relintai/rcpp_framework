#ifndef HTML_BUILDER_H
#define HTML_BUILDER_H

#include "core/string.h"

class Request;
class HTMLBuilder;

class HTMLTag {
public:
	bool simple;
	String result;

	HTMLTag *str(const String &str);
	HTMLTag *style(const String &val);
	HTMLTag *href(const String &val);
	HTMLTag *cls(const String &val);
	HTMLTag *id(const String &val);
	HTMLTag *name(const String &val);
	HTMLTag *content(const String &val);
	HTMLTag *value(const String &val);
	HTMLTag *method(const String &val);
	HTMLTag *type(const String &val);
	HTMLTag *placeholder(const String &val);
	HTMLTag *rel(const String &val);
	HTMLTag *rel_stylesheet();
	HTMLTag *charset(const String &val);
	HTMLTag *charset_utf_8();

	HTMLTag *attrib(const String &attr, const String &val);

	HTMLTag *start(const String &p_new_tag, const bool p_simple = false);
	HTMLTag *reset();
	HTMLTag *close();

	HTMLBuilder *f();

	bool has_data();

	HTMLTag();

	HTMLBuilder *owner;
};

class HTMLBuilder {
public:
	String result;

	HTMLBuilder *comment(const String &val);
	HTMLTag *doctype();
	HTMLBuilder *doctype(const String &val);

	HTMLTag *a();
	HTMLTag *abbr();
	HTMLTag *acronym(); //Not supported in HTML5.
	HTMLTag *address();
	HTMLTag *applet(); //Not supported in HTML5.
	HTMLTag *area();
	HTMLTag *article();
	HTMLTag *aside();
	HTMLTag *audio();
	HTMLTag *b();
	HTMLTag *basefont(); //Not supported in HTML5.
	HTMLTag *bdi();
	HTMLTag *bdo();
	HTMLTag *big(); //Not supported in HTML5.
	HTMLTag *blockquote();
	HTMLTag *body();
	HTMLTag *br();
	HTMLTag *button();
	HTMLTag *canvas();
	HTMLTag *caption();
	HTMLTag *center(); //Not supported in HTML5.
	HTMLTag *cite();
	HTMLTag *code();
	HTMLTag *col();
	HTMLTag *colgroup();
	HTMLTag *data();
	HTMLTag *datalist();
	HTMLTag *dd();
	HTMLTag *del();
	HTMLTag *details();
	HTMLTag *dfn();
	HTMLTag *dialog();
	HTMLTag *dir(); // Not supported in HTML5.
	HTMLTag *div();
	HTMLTag *dl();
	HTMLTag *dt();
	HTMLTag *em();
	HTMLTag *embed();
	HTMLTag *fieldset();
	HTMLTag *figcaption();
	HTMLTag *figure();
	HTMLTag *font(); //Not supported in HTML5.
	HTMLTag *footer();
	HTMLTag *form();
	HTMLTag *frame(); //Not supported in HTML5.
	HTMLTag *frameset(); //Not supported in HTML5.
	HTMLTag *h1();
	HTMLTag *h2();
	HTMLTag *h3();
	HTMLTag *h4();
	HTMLTag *h5();
	HTMLTag *h6();
	HTMLTag *head();
	HTMLTag *header();
	HTMLTag *hr();
	HTMLTag *html();

	HTMLTag *i();
	HTMLTag *iframe();
	HTMLTag *img();
	HTMLTag *input();
	HTMLTag *ins();
	HTMLTag *kbd();
	HTMLTag *label();
	HTMLTag *legend();
	HTMLTag *li();
	HTMLTag *link();
	HTMLTag *main();
	HTMLTag *map();
	HTMLTag *mark();
	HTMLTag *meta();
	HTMLTag *meter();

	HTMLTag *nav();
	HTMLTag *noframes(); //Not supported in HTML5.
	HTMLTag *noscript();
	HTMLTag *object();
	HTMLTag *ol();
	HTMLTag *optgroup();
	HTMLTag *option();
	HTMLTag *output();
	HTMLTag *p();
	HTMLTag *param();
	HTMLTag *picture();
	HTMLTag *pre();
	HTMLTag *progress();
	HTMLTag *q();
	HTMLTag *rp();

	HTMLTag *rt();
	HTMLTag *ruby();
	HTMLTag *s();
	HTMLTag *samp();
	HTMLTag *script();
	HTMLTag *section();
	HTMLTag *select();
	HTMLTag *small();
	HTMLTag *source();
	HTMLTag *span();
	HTMLTag *strike(); //Not supported in HTML5
	HTMLTag *strong();
	HTMLTag *style();
	HTMLTag *sub();
	HTMLTag *summary();
	HTMLTag *sup();

	HTMLTag *svg();
	HTMLTag *table();
	HTMLTag *tbody();
	HTMLTag *td();
	HTMLTag *templateh();
	HTMLTag *textarea();
	HTMLTag *tfoot();
	HTMLTag *th();
	HTMLTag *thead();
	HTMLTag *time();
	HTMLTag *title();
	HTMLTag *tr();
	HTMLTag *track();
	HTMLTag *tt(); //Not supported in HTML5.
	HTMLTag *u();
	HTMLTag *ul();
	HTMLTag *var();
	HTMLTag *video();
	HTMLTag *wbr();

	//closing tags c prefix means close
	//Note simple tags should not have these like <br>
	//Note that I might have a few that shouldn't be here, those will be removed as I find them
	HTMLBuilder *ca();
	HTMLBuilder *cabbr();
	HTMLBuilder *cacronym();
	HTMLBuilder *caddress();
	HTMLBuilder *capplet();
	HTMLBuilder *carea();
	HTMLBuilder *carticle();
	HTMLBuilder *caside();
	HTMLBuilder *caudio();
	HTMLBuilder *cb();
	HTMLBuilder *cbasefont();
	HTMLBuilder *cbdi();
	HTMLBuilder *cbdo();
	HTMLBuilder *cbig();
	HTMLBuilder *cblockquote();
	HTMLBuilder *cbody();
	HTMLBuilder *cbutton();
	HTMLBuilder *ccanvas();

	HTMLBuilder *ccaption();
	HTMLBuilder *ccenter();
	HTMLBuilder *ccite();
	HTMLBuilder *ccode();
	HTMLBuilder *ccol();
	HTMLBuilder *ccolgroup();
	HTMLBuilder *cdata();
	HTMLBuilder *cdatalist();
	HTMLBuilder *cdd();
	HTMLBuilder *cdel();
	HTMLBuilder *cdetails();
	HTMLBuilder *cdfn();
	HTMLBuilder *cdialog();
	HTMLBuilder *cdir();
	HTMLBuilder *cdiv();
	HTMLBuilder *cdl();
	HTMLBuilder *cdt();

	HTMLBuilder *cem();
	HTMLBuilder *cembed();
	HTMLBuilder *cfieldset();
	HTMLBuilder *cfigcaption();
	HTMLBuilder *cfigure();
	HTMLBuilder *cfont();
	HTMLBuilder *cfooter();
	HTMLBuilder *cform();
	HTMLBuilder *cframe();
	HTMLBuilder *cframeset();
	HTMLBuilder *ch1();
	HTMLBuilder *ch2();
	HTMLBuilder *ch3();
	HTMLBuilder *ch4();
	HTMLBuilder *ch5();
	HTMLBuilder *ch6();
	HTMLBuilder *chead();
	HTMLBuilder *cheader();
	HTMLBuilder *chr();
	HTMLBuilder *chtml();

	HTMLBuilder *ci();
	HTMLBuilder *ciframe();
	HTMLBuilder *cimg();
	HTMLBuilder *cinput();
	HTMLBuilder *cins();
	HTMLBuilder *ckbd();
	HTMLBuilder *clabel();
	HTMLBuilder *clegend();
	HTMLBuilder *cli();
	HTMLBuilder *clink();
	HTMLBuilder *cmain();
	HTMLBuilder *cmap();
	HTMLBuilder *cmark();
	HTMLBuilder *cmeta();
	HTMLBuilder *cmeter();

	HTMLBuilder *cnav();
	HTMLBuilder *cnoframes();
	HTMLBuilder *cnoscript();
	HTMLBuilder *cobject();
	HTMLBuilder *c_ol();
	HTMLBuilder *coptgroup();
	HTMLBuilder *coption();
	HTMLBuilder *coutput();
	HTMLBuilder *cp();
	HTMLBuilder *cparam();
	HTMLBuilder *cpicture();
	HTMLBuilder *cpre();
	HTMLBuilder *cprogress();
	HTMLBuilder *cq();
	HTMLBuilder *crp();

	HTMLBuilder *crt();
	HTMLBuilder *cruby();
	HTMLBuilder *cs();
	HTMLBuilder *csamp();
	HTMLBuilder *cscript();
	HTMLBuilder *csection();
	HTMLBuilder *cselect();
	HTMLBuilder *csmall();
	HTMLBuilder *csource();
	HTMLBuilder *cspan();
	HTMLBuilder *cstrike();
	HTMLBuilder *cstrong();
	HTMLBuilder *cstyle();
	HTMLBuilder *csub();
	HTMLBuilder *csummary();
	HTMLBuilder *csup();

	HTMLBuilder *csvg();
	HTMLBuilder *ctable();
	HTMLBuilder *ctbody();
	HTMLBuilder *ctd();
	HTMLBuilder *ctemplateh();
	HTMLBuilder *ctextarea();
	HTMLBuilder *ctfoot();
	HTMLBuilder *cth();
	HTMLBuilder *cthead();
	HTMLBuilder *ctime();
	HTMLBuilder *ctitle();
	HTMLBuilder *ctr();
	HTMLBuilder *ctrack();
	HTMLBuilder *ctt();
	HTMLBuilder *cu();
	HTMLBuilder *cul();
	HTMLBuilder *cvar();
	HTMLBuilder *cvideo();
	HTMLBuilder *cwbr();

	void f();

	//write
	HTMLBuilder *w(const String &val);

	HTMLBuilder *wn(const double val, int p_decimals = -1);
	HTMLBuilder *wns(const double val);
	HTMLBuilder *wr(const double val, const bool p_trailing = true);
	HTMLBuilder *wi(const int64_t val, const int base = 10, const bool capitalize_hex = false);
	HTMLBuilder *wui(const uint64_t val, const int base = 10, const bool capitalize_hex = false);

	//write_escaped
	HTMLBuilder *we(const String &val);

	HTMLBuilder *write_tag();

	HTMLBuilder();
	virtual ~HTMLBuilder();

protected:
	HTMLTag tag;
};

#endif