#ifndef HTML_BUILDER_BIND_H
#define HTML_BUILDER_BIND_H

#include "core/object.h"
#include "core/ustring.h"

#include "core/reference.h"

class _HTMLBuilder;

class _HTMLTag : public Reference {
	GDCLASS(_HTMLTag, Reference)

public:
	bool simple;
	String result;

	_HTMLTag *str(const String &str);
	_HTMLTag *style(const String &val);
	_HTMLTag *href(const String &val);
	_HTMLTag *cls(const String &val);
	_HTMLTag *clsse(const String &val); //se -> skip empty
	_HTMLTag *id(const String &val);
	_HTMLTag *name(const String &val);
	_HTMLTag *content(const String &val);
	_HTMLTag *value(const String &val);
	_HTMLTag *accept(const String &val);
	_HTMLTag *src(const String &val);
	_HTMLTag *alt(const String &val);
	_HTMLTag *inputmode(const String &val);
	_HTMLTag *list(const String &val);

	_HTMLTag *autocomplete(const String &val);

	_HTMLTag *autocomplete_off();
	_HTMLTag *autocomplete_on();
	_HTMLTag *autocomplete_name();
	_HTMLTag *autocomplete_name_honorific_prefix();
	_HTMLTag *autocomplete_name_given_name();
	_HTMLTag *autocomplete_name_additional_name();
	_HTMLTag *autocomplete_name_family_name();
	_HTMLTag *autocomplete_name_honorific_suffix();
	_HTMLTag *autocomplete_name_nickname();
	_HTMLTag *autocomplete_email();
	_HTMLTag *autocomplete_username();
	_HTMLTag *autocomplete_new_password();
	_HTMLTag *autocomplete_current_password();
	_HTMLTag *autocomplete_one_time_code();
	_HTMLTag *autocomplete_organization_title();
	_HTMLTag *autocomplete_organization();
	_HTMLTag *autocomplete_street_address();
	_HTMLTag *autocomplete_address_line1();
	_HTMLTag *autocomplete_address_line2();
	_HTMLTag *autocomplete_address_line3();
	_HTMLTag *autocomplete_address_level_1();
	_HTMLTag *autocomplete_address_level_2();
	_HTMLTag *autocomplete_address_level_3();
	_HTMLTag *autocomplete_address_level_4();
	_HTMLTag *autocomplete_country();
	_HTMLTag *autocomplete_country_name();
	_HTMLTag *autocomplete_postal_code();
	_HTMLTag *autocomplete_cc_name();
	_HTMLTag *autocomplete_cc_given_name();
	_HTMLTag *autocomplete_cc_additional_name();
	_HTMLTag *autocomplete_cc_family_name();
	_HTMLTag *autocomplete_cc_number();
	_HTMLTag *autocomplete_cc_exp();
	_HTMLTag *autocomplete_cc_exp_month();
	_HTMLTag *autocomplete_cc_exp_year();
	_HTMLTag *autocomplete_cc_csc();
	_HTMLTag *autocomplete_cc_type();
	_HTMLTag *autocomplete_transaction_currency();
	_HTMLTag *autocomplete_transaction_amount();
	_HTMLTag *autocomplete_language();
	_HTMLTag *autocomplete_bday();
	_HTMLTag *autocomplete_bday_day();
	_HTMLTag *autocomplete_bday_month();
	_HTMLTag *autocomplete_bday_year();
	_HTMLTag *autocomplete_sex();
	_HTMLTag *autocomplete_tel();
	_HTMLTag *autocomplete_tel_country_code();
	_HTMLTag *autocomplete_tel_national();
	_HTMLTag *autocomplete_tel_area_code();
	_HTMLTag *autocomplete_tel_local();
	_HTMLTag *autocomplete_tel_extension();
	_HTMLTag *autocomplete_impp();
	_HTMLTag *autocomplete_url();
	_HTMLTag *autocomplete_photo();

	_HTMLTag *onclick(const String &val);

	_HTMLTag *checked(const bool val = true);
	_HTMLTag *selected(const bool val = true);
	_HTMLTag *autofocus(const bool val = true);
	_HTMLTag *disabled(const bool val = true);
	_HTMLTag *multiple(const bool val = true);
	_HTMLTag *required(const bool val = true);
	_HTMLTag *spellcheck(const bool val);

	_HTMLTag *max(const String &val);
	_HTMLTag *min(const String &val);
	_HTMLTag *step(const String &val);
	_HTMLTag *step_any();

	_HTMLTag *minlength(const int val);
	_HTMLTag *minlength(const String &val);
	_HTMLTag *maxlength(const int val);
	_HTMLTag *maxlength(const String &val);
	_HTMLTag *size(const int val);
	_HTMLTag *size(const String &val);

	_HTMLTag *width(const int val);
	_HTMLTag *width(const String &val);
	_HTMLTag *height(const int val);
	_HTMLTag *height(const String &val);

	_HTMLTag *pattern(const String &val);

	_HTMLTag *method(const String &val);
	_HTMLTag *method_get();
	_HTMLTag *method_post();

	_HTMLTag *action(const String &val);
	_HTMLTag *type(const String &val);
	_HTMLTag *placeholder(const String &val);
	_HTMLTag *fora(const String &val); // for attrib -> for is reserved keyword

	_HTMLTag *rel(const String &val);
	_HTMLTag *rel_stylesheet();
	_HTMLTag *rel_alternate();
	_HTMLTag *rel_author();
	_HTMLTag *rel_bookmark();
	_HTMLTag *rel_external();
	_HTMLTag *rel_help();
	_HTMLTag *rel_license();
	_HTMLTag *rel_next();
	_HTMLTag *rel_nofollow();
	_HTMLTag *rel_noopener();
	_HTMLTag *rel_noreferrer();
	_HTMLTag *rel_prev();
	_HTMLTag *rel_search();
	_HTMLTag *rel_tag();

	_HTMLTag *charset(const String &val);
	_HTMLTag *charset_utf_8();

	_HTMLTag *itbutton();
	_HTMLTag *itcheckbox();
	_HTMLTag *itcolor();
	_HTMLTag *itdate();
	_HTMLTag *itdatetime_local();
	_HTMLTag *itemail();
	_HTMLTag *itfile();
	_HTMLTag *ithidden();
	_HTMLTag *itimage();
	_HTMLTag *itmonth();
	_HTMLTag *itnumber();
	_HTMLTag *itpassword();
	_HTMLTag *itradio();
	_HTMLTag *itrange();
	_HTMLTag *itreset();
	_HTMLTag *itsearch();
	_HTMLTag *itsubmit();
	_HTMLTag *ittel();
	_HTMLTag *ittext();
	_HTMLTag *ittime();
	_HTMLTag *iturl();
	_HTMLTag *itweek();

	_HTMLTag *inputmode_none();
	_HTMLTag *inputmode_text();
	_HTMLTag *inputmode_decimal();
	_HTMLTag *inputmode_numeric();
	_HTMLTag *inputmode_tel();
	_HTMLTag *inputmode_search();
	_HTMLTag *inputmode_email();
	_HTMLTag *inputmode_url();

	_HTMLTag *attrib(const String &attr, const String &val);

	_HTMLTag *start(const String &p_new_tag, const bool p_simple = false);
	_HTMLTag *reset();
	_HTMLTag *close();

	_HTMLBuilder *f();

	bool has_data();

	_HTMLTag();

	_HTMLBuilder *owner;

protected:
	static void _bind_methods();
};

class _HTMLBuilder : public Reference {
	GDCLASS(_HTMLBuilder, Reference)

public:
	String result;

	_HTMLBuilder *comment(const String &val);
	_HTMLTag *doctype();
	_HTMLBuilder *doctype(const String &val);

	_HTMLTag *a();
	_HTMLTag *abbr();
	_HTMLTag *acronym(); // Not supported in HTML5.
	_HTMLTag *address();
	_HTMLTag *applet(); // Not supported in HTML5.
	_HTMLTag *area();
	_HTMLTag *article();
	_HTMLTag *aside();
	_HTMLTag *audio();
	_HTMLTag *b();
	_HTMLTag *basefont(); // Not supported in HTML5.
	_HTMLTag *bdi();
	_HTMLTag *bdo();
	_HTMLTag *big(); // Not supported in HTML5.
	_HTMLTag *blockquote();
	_HTMLTag *body();
	_HTMLTag *br();
	_HTMLTag *button();
	_HTMLTag *canvas();
	_HTMLTag *caption();
	_HTMLTag *center(); // Not supported in HTML5.
	_HTMLTag *cite();
	_HTMLTag *code();
	_HTMLTag *col();
	_HTMLTag *colgroup();
	_HTMLTag *data();
	_HTMLTag *datalist();
	_HTMLTag *dd();
	_HTMLTag *del();
	_HTMLTag *details();
	_HTMLTag *dfn();
	_HTMLTag *dialog();
	_HTMLTag *dir(); // Not supported in HTML5.
	_HTMLTag *div();
	_HTMLTag *dl();
	_HTMLTag *dt();
	_HTMLTag *em();
	_HTMLTag *embed();
	_HTMLTag *fieldset();
	_HTMLTag *figcaption();
	_HTMLTag *figure();
	_HTMLTag *font(); // Not supported in HTML5.
	_HTMLTag *footer();
	_HTMLTag *form();
	_HTMLTag *frame(); // Not supported in HTML5.
	_HTMLTag *frameset(); // Not supported in HTML5.
	_HTMLTag *h1();
	_HTMLTag *h2();
	_HTMLTag *h3();
	_HTMLTag *h4();
	_HTMLTag *h5();
	_HTMLTag *h6();
	_HTMLTag *head();
	_HTMLTag *header();
	_HTMLTag *hr();
	_HTMLTag *html();

	_HTMLTag *i();
	_HTMLTag *iframe();
	_HTMLTag *img();
	_HTMLTag *input();
	_HTMLTag *ins();
	_HTMLTag *kbd();
	_HTMLTag *label();
	_HTMLTag *legend();
	_HTMLTag *li();
	_HTMLTag *link();
	_HTMLTag *main();
	_HTMLTag *map();
	_HTMLTag *mark();
	_HTMLTag *meta();
	_HTMLTag *meter();

	_HTMLTag *nav();
	_HTMLTag *noframes(); // Not supported in HTML5.
	_HTMLTag *noscript();
	_HTMLTag *object();
	_HTMLTag *ol();
	_HTMLTag *optgroup();
	_HTMLTag *option();
	_HTMLTag *output();
	_HTMLTag *p();
	_HTMLTag *param();
	_HTMLTag *picture();
	_HTMLTag *pre();
	_HTMLTag *progress();
	_HTMLTag *q();
	_HTMLTag *rp();

	_HTMLTag *rt();
	_HTMLTag *ruby();
	_HTMLTag *s();
	_HTMLTag *samp();
	_HTMLTag *script();
	_HTMLTag *section();
	_HTMLTag *select();
	_HTMLTag *small();
	_HTMLTag *source();
	_HTMLTag *span();
	_HTMLTag *strike(); // Not supported in HTML5
	_HTMLTag *strong();
	_HTMLTag *style();
	_HTMLTag *sub();
	_HTMLTag *summary();
	_HTMLTag *sup();

	_HTMLTag *svg();
	_HTMLTag *table();
	_HTMLTag *tbody();
	_HTMLTag *td();
	_HTMLTag *templateh();
	_HTMLTag *textarea();
	_HTMLTag *tfoot();
	_HTMLTag *th();
	_HTMLTag *thead();
	_HTMLTag *time();
	_HTMLTag *title();
	_HTMLTag *tr();
	_HTMLTag *track();
	_HTMLTag *tt(); // Not supported in HTML5.
	_HTMLTag *u();
	_HTMLTag *ul();
	_HTMLTag *var();
	_HTMLTag *video();
	_HTMLTag *wbr();

	//_HTMLBuilder *a(const String &href, const String &cls = "", const String &id = "");
	_HTMLBuilder *fa(const String &href, const String &body, const String &cls = "", const String &id = "");

	_HTMLBuilder *div(const String &cls, const String &id = "");
	_HTMLBuilder *fdiv(const String &body, const String &cls = "", const String &id = "");

	_HTMLBuilder *textarea(const String &name, const String &cls = "", const String &id = "");
	_HTMLBuilder *ftextarea(const String &name, const String &body, const String &cls = "", const String &id = "");

	_HTMLBuilder *select(const String &name, const String &cls = "", const String &id = "");

	_HTMLTag *option(const String &value);
	_HTMLBuilder *foption(const String &value, const String &body, const bool selected = false);

	// closing tags c prefix means close
	// Note simple tags should not have these like <br>
	// Note that I might have a few that shouldn't be here, those will be removed as I find them
	_HTMLBuilder *ca();
	_HTMLBuilder *cabbr();
	_HTMLBuilder *cacronym();
	_HTMLBuilder *caddress();
	_HTMLBuilder *capplet();
	_HTMLBuilder *carea();
	_HTMLBuilder *carticle();
	_HTMLBuilder *caside();
	_HTMLBuilder *caudio();
	_HTMLBuilder *cb();
	_HTMLBuilder *cbasefont();
	_HTMLBuilder *cbdi();
	_HTMLBuilder *cbdo();
	_HTMLBuilder *cbig();
	_HTMLBuilder *cblockquote();
	_HTMLBuilder *cbody();
	_HTMLBuilder *cbutton();
	_HTMLBuilder *ccanvas();

	_HTMLBuilder *ccaption();
	_HTMLBuilder *ccenter();
	_HTMLBuilder *ccite();
	_HTMLBuilder *ccode();
	_HTMLBuilder *ccol();
	_HTMLBuilder *ccolgroup();
	_HTMLBuilder *cdata();
	_HTMLBuilder *cdatalist();
	_HTMLBuilder *cdd();
	_HTMLBuilder *cdel();
	_HTMLBuilder *cdetails();
	_HTMLBuilder *cdfn();
	_HTMLBuilder *cdialog();
	_HTMLBuilder *cdir();
	_HTMLBuilder *cdiv();
	_HTMLBuilder *cdl();
	_HTMLBuilder *cdt();

	_HTMLBuilder *cem();
	_HTMLBuilder *cembed();
	_HTMLBuilder *cfieldset();
	_HTMLBuilder *cfigcaption();
	_HTMLBuilder *cfigure();
	_HTMLBuilder *cfont();
	_HTMLBuilder *cfooter();
	_HTMLBuilder *cform();
	_HTMLBuilder *cframe();
	_HTMLBuilder *cframeset();
	_HTMLBuilder *ch1();
	_HTMLBuilder *ch2();
	_HTMLBuilder *ch3();
	_HTMLBuilder *ch4();
	_HTMLBuilder *ch5();
	_HTMLBuilder *ch6();
	_HTMLBuilder *chead();
	_HTMLBuilder *cheader();
	_HTMLBuilder *chr();
	_HTMLBuilder *chtml();

	_HTMLBuilder *ci();
	_HTMLBuilder *ciframe();
	_HTMLBuilder *cimg();
	_HTMLBuilder *cinput();
	_HTMLBuilder *cins();
	_HTMLBuilder *ckbd();
	_HTMLBuilder *clabel();
	_HTMLBuilder *clegend();
	_HTMLBuilder *cli();
	_HTMLBuilder *clink();
	_HTMLBuilder *cmain();
	_HTMLBuilder *cmap();
	_HTMLBuilder *cmark();
	_HTMLBuilder *cmeta();
	_HTMLBuilder *cmeter();

	_HTMLBuilder *cnav();
	_HTMLBuilder *cnoframes();
	_HTMLBuilder *cnoscript();
	_HTMLBuilder *cobject();
	_HTMLBuilder *c_ol();
	_HTMLBuilder *coptgroup();
	_HTMLBuilder *coption();
	_HTMLBuilder *coutput();
	_HTMLBuilder *cp();
	_HTMLBuilder *cparam();
	_HTMLBuilder *cpicture();
	_HTMLBuilder *cpre();
	_HTMLBuilder *cprogress();
	_HTMLBuilder *cq();
	_HTMLBuilder *crp();

	_HTMLBuilder *crt();
	_HTMLBuilder *cruby();
	_HTMLBuilder *cs();
	_HTMLBuilder *csamp();
	_HTMLBuilder *cscript();
	_HTMLBuilder *csection();
	_HTMLBuilder *cselect();
	_HTMLBuilder *csmall();
	_HTMLBuilder *csource();
	_HTMLBuilder *cspan();
	_HTMLBuilder *cstrike();
	_HTMLBuilder *cstrong();
	_HTMLBuilder *cstyle();
	_HTMLBuilder *csub();
	_HTMLBuilder *csummary();
	_HTMLBuilder *csup();

	_HTMLBuilder *csvg();
	_HTMLBuilder *ctable();
	_HTMLBuilder *ctbody();
	_HTMLBuilder *ctd();
	_HTMLBuilder *ctemplateh();
	_HTMLBuilder *ctextarea();
	_HTMLBuilder *ctfoot();
	_HTMLBuilder *cth();
	_HTMLBuilder *cthead();
	_HTMLBuilder *ctime();
	_HTMLBuilder *ctitle();
	_HTMLBuilder *ctr();
	_HTMLBuilder *ctrack();
	_HTMLBuilder *ctt();
	_HTMLBuilder *cu();
	_HTMLBuilder *cul();
	_HTMLBuilder *cvar();
	_HTMLBuilder *cvideo();
	_HTMLBuilder *cwbr();

	_HTMLTag *form_get();
	_HTMLTag *form_post();
	_HTMLBuilder *form_get(const String &action, const String &cls = "", const String &id = "");
	_HTMLBuilder *form_post(const String &action, const String &cls = "", const String &id = "");
	// will add a csrf token from request
	//_HTMLBuilder *form_post(const String &action, Request *request, const String &cls = "", const String &id = "");

	_HTMLTag *input_button();
	_HTMLTag *input_checkbox();
	_HTMLTag *input_color();
	_HTMLTag *input_date();
	_HTMLTag *input_datetime_local();
	_HTMLTag *input_email();
	_HTMLTag *input_file();
	_HTMLTag *input_hidden();
	_HTMLTag *input_image();
	_HTMLTag *input_month();
	_HTMLTag *input_number();
	_HTMLTag *input_password();
	_HTMLTag *input_radio();
	_HTMLTag *input_range();
	_HTMLTag *input_reset();
	_HTMLTag *input_search();
	_HTMLTag *input_submit();
	_HTMLTag *input_tel();
	_HTMLTag *input_text();
	_HTMLTag *input_time();
	_HTMLTag *input_url();
	_HTMLTag *input_week();

	_HTMLBuilder *label(const String &pfor, const String &plabel, const String &cls = "", const String &id = "");

	_HTMLBuilder *input_button(const String &name, const String &value = "", const String &cls = "", const String &id = "");
	_HTMLBuilder *input_checkbox(const String &name, const String &value = "", const bool checked = false, const String &cls = "", const String &id = "");
	_HTMLBuilder *input_color(const String &name, const String &value = "", const String &cls = "", const String &id = "");
	_HTMLBuilder *input_date(const String &name, const String &value = "", const String &cls = "", const String &id = "", const String &date_min = "", const String &date_max = "", const String &date_step = "");
	_HTMLBuilder *input_datetime_local(const String &name, const String &value = "", const String &cls = "", const String &id = "", const String &date_min = "", const String &date_max = "", const String &date_step = "");
	_HTMLBuilder *input_email(const String &name, const String &value = "", const String &placeholder = "", const String &cls = "", const String &id = "");
	_HTMLBuilder *input_file(const String &name, const String &accept = "", const String &cls = "", const String &id = "");
	_HTMLBuilder *input_image(const String &name, const String &src = "", const String &alt = "", const String &cls = "", const String &id = "", const int width = 0, const int height = 0);
	_HTMLBuilder *input_month(const String &name, const String &cls = "", const String &id = "");
	_HTMLBuilder *input_number(const String &name, const String & = "", const String & = "", const String &cls = "", const String &id = "");
	_HTMLBuilder *input_password(const String &name, const String &value = "", const String &placeholder = "", const String &cls = "", const String &id = "", const String &minlength = "", const String &maxlength = "", const String &size = "");
	_HTMLBuilder *input_radio(const String &name, const String &value = "", const String &cls = "", const String &id = "");
	_HTMLBuilder *input_range(const String &name, const String &value = "", const String &vmin = "", const String &vmax = "", const String &vstep = "", const String &cls = "", const String &id = "");
	_HTMLBuilder *input_reset(const String &name, const String &value = "", const String &cls = "", const String &id = "");
	_HTMLBuilder *input_search(const String &name, const String &value = "", const String &placeholder = "", const String &cls = "", const String &id = "", const String &minlength = "", const String &maxlength = "", const String &size = "", const String &pattern = "");
	_HTMLBuilder *input_submit(const String &value, const String &cls = "", const String &id = "");
	_HTMLBuilder *input_tel(const String &name, const String &value = "", const String &placeholder = "", const String &cls = "", const String &id = "", const String &minlength = "", const String &maxlength = "", const String &size = "", const String &pattern = "");
	_HTMLBuilder *input_text(const String &name, const String &value = "", const String &placeholder = "", const String &cls = "", const String &id = "", const String &minlength = "", const String &maxlength = "", const String &size = "");
	_HTMLBuilder *input_time(const String &name, const String &cls = "", const String &id = "", const String &vmin = "", const String &vmax = "", const String &vstep = "");
	_HTMLBuilder *input_url(const String &name, const String &value = "", const String &placeholder = "", const String &cls = "", const String &id = "", const String &minlength = "", const String &maxlength = "", const String &size = "");
	_HTMLBuilder *input_week(const String &name, const String &cls = "", const String &id = "", const String &vmin = "", const String &vmax = "");
	_HTMLBuilder *input_hidden(const String &name, const String &value);

	_HTMLBuilder *csrf_token(const String &token);
	//_HTMLBuilder *csrf_token(Request *request);

	void f();

	// write
	_HTMLBuilder *w(const String &val);

	_HTMLBuilder *wn(const double val, int p_decimals = -1);
	_HTMLBuilder *wns(const double val);
	_HTMLBuilder *wr(const double val, const bool p_trailing = true);
	_HTMLBuilder *wi(const int64_t val, const int base = 10, const bool capitalize_hex = false);
	_HTMLBuilder *wui(const uint64_t val, const int base = 10, const bool capitalize_hex = false);
	_HTMLBuilder *wbn(const bool val);
	_HTMLBuilder *wbs(const bool val);

	// write_escaped
	_HTMLBuilder *we(const String &val);

	_HTMLBuilder *write_tag();

	_HTMLBuilder();
	virtual ~_HTMLBuilder();

protected:
	static void _bind_methods();

	_HTMLTag tag;
};

#endif
