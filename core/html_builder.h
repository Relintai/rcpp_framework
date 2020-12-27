#ifndef HTML_BUILDER_H
#define HTML_BUILDER_H

#include <map>
#include <string>
#include <vector>

class Request;

//This might be converted to a form validator

class HTMLTag {
public:
	bool simple;
	std::string result;

	HTMLTag *str(const std::string &str);
	HTMLTag *style(const std::string &val);
	HTMLTag *href(const std::string &val);
	HTMLTag *cls(const std::string &val);
	HTMLTag *id(const std::string &val);
	HTMLTag *name(const std::string &val);
	HTMLTag *method(const std::string &val);
	HTMLTag *type(const std::string &val);
	HTMLTag *rel(const std::string &val);
	HTMLTag *rel_stylesheet();
	HTMLTag *charset(const std::string &val);
	HTMLTag *charset_utf_8();

	HTMLTag *attrib(const std::string &attr, const std::string &val);

	HTMLTag *start(const std::string &p_new_tag, const bool p_simple = false);
	HTMLTag *reset();
	HTMLTag *close();

	bool has_data();

	HTMLTag();
};

class HTMLBuilder {
public:
	std::string result;

	void comment(const std::string &val);
	HTMLTag *doctype();
	void doctype(const std::string &val);

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
    void ca();
	void cabbr();
	void cacronym();
	void caddress();
	void capplet();
	void carea();
	void carticle();
	void caside();
	void caudio();
	void cb();
	void cbasefont();
	void cbdi();
	void cbdo();
	void cbig();
	void cblockquote();
	void cbody();
	void cbutton();
	void ccanvas();

	void ccaption();
	void ccenter();
	void ccite();
	void ccode();
	void ccol();
	void ccolgroup();
	void cdata();
	void cdatalist();
	void cdd();
	void cdel();
	void cdetails();
	void cdfn();
	void cdialog();
	void cdir();
	void cdiv();
	void cdl();
	void cdt();
    
	void cem();
	void cembed();
	void cfieldset();
	void cfigcaption();
	void cfigure();
	void cfont();
	void cfooter();
	void cform();
	void cframe();
	void cframeset();
	void ch1();
	void ch2();
	void ch3();
	void ch4();
	void ch5();
	void ch6();
	void chead();
	void cheader();
	void chr();
	void chtml();

	void ci();
	void ciframe();
	void cimg();
	void cinput();
	void cins();
	void ckbd();
	void clabel();
	void clegend();
	void cli();
	void clink();
	void cmain();
	void cmap();
	void cmark();
	void cmeta();
	void cmeter();

	void cnav();
	void cnoframes();
	void cnoscript();
	void cobject();
	void c_ol();
	void coptgroup();
	void coption();
	void coutput();
	void cp();
	void cparam();
	void cpicture();
	void cpre();
	void cprogress();
	void cq();
	void crp();

	void crt();
	void cruby();
	void cs();
	void csamp();
	void cscript();
	void csection();
	void cselect();
	void csmall();
	void csource();
	void cspan();
	void cstrike();
	void cstrong();
	void cstyle();
	void csub();
	void csummary();
	void csup();

	void csvg();
	void ctable();
	void ctbody();
	void ctd();
	void ctemplateh();
	void ctextarea();
	void ctfoot();
	void cth();
	void cthead();
	void ctime();
	void ctitle();
	void ctr();
	void ctrack();
	void ctt();
	void cu();
	void cul();
	void cvar();
	void cvideo();
	void cwbr();

    //write
	void w(const std::string &val);
    //write_escaped
	void we(const std::string &val);

	void write_tag();

	HTMLBuilder();
	virtual ~HTMLBuilder();

protected:
	HTMLTag tag;
};

#endif